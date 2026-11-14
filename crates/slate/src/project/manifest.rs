use crate::compile_commands::{self, ReadOptions, absolute_path};
use crate::project::matrix::{
    MatrixError, command_macros, command_targets, is_cargo_feature_name, macro_of, read_json,
    read_manifest_target, validate_target,
};
use serde::{Deserialize, Serialize};
use slate_parser::compiler_args::CompilerFlavor;
use slate_parser::target_registry;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use thiserror::Error;

#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub(crate) struct Manifest {
    pub(crate) version: u32,
    pub(crate) targets: BTreeMap<String, ManifestTarget>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub(crate) features: BTreeMap<String, Vec<String>>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub(crate) invalid_features: Vec<Vec<String>>,
}

#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub(crate) struct ManifestTarget {
    pub(crate) triple: String,
    pub(crate) flavor: String,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub(crate) sysroot: Option<String>,
    pub(crate) compile_commands: PathBuf,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub(crate) build_target: Option<String>,
}

#[derive(Debug, Error)]
pub enum ManifestError {
    #[error(transparent)]
    Manifest(#[from] MatrixError),
    #[error("write manifest {path}: {source}")]
    Write {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("manifest already has target {0}, pass --replace to overwrite it")]
    TargetExists(String),
    #[error("manifest has no target {0}")]
    UnknownTarget(String),
    #[error(
        "target {target}: neither the compiler name nor a --target in {} names a triple, pass --triple",
        database.display()
    )]
    NoTriple { target: String, database: PathBuf },
    #[error(
        "target {target}: {} compiles for several triples ({}), pass --triple",
        database.display(),
        triples.join(", ")
    )]
    SeveralTriples {
        target: String,
        database: PathBuf,
        triples: Vec<String>,
    },
    #[error(
        "target {target}: {} uses compilers of several flavors ({}), pass --flavor",
        database.display(),
        flavors.join(", ")
    )]
    SeveralFlavors {
        target: String,
        database: PathBuf,
        flavors: Vec<String>,
    },
    #[error("pass at least one define")]
    NoDefines,
    #[error("feature name {name:?} from {define} is not a valid Cargo feature name, pass --name")]
    DerivedFeatureName { define: String, name: String },
    #[error("manifest already has feature {0}")]
    FeatureExists(String),
    #[error("{first} and {second} both become feature {name}, add one with --name")]
    SameFeatureName {
        name: String,
        first: String,
        second: String,
    },
    #[error("{0} is given twice")]
    RepeatedDefine(String),
    #[error("feature {feature} already defines {name}")]
    DefineExists { name: String, feature: String },
    #[error(
        "the compile commands of target{} {} already set {name}, so a feature cannot turn it off",
        if targets.len() == 1 { "" } else { "s" },
        targets.join(", ")
    )]
    DefineInBaseCommands { name: String, targets: Vec<String> },
    #[error("manifest has no feature or define {0}")]
    UnknownFeature(String),
    #[error("a conflict needs at least two different features")]
    ShortConflict,
}

pub struct TargetAdd<'a> {
    pub manifest: &'a Path,
    pub project: &'a Path,
    pub name: &'a str,
    pub compile_commands: &'a Path,
    pub triple: Option<&'a str>,
    pub flavor: Option<&'a str>,
    pub sysroot: Option<&'a str>,
    pub build_target: Option<&'a str>,
    pub replace: bool,
}

impl Manifest {
    pub(crate) fn read(path: &Path) -> Result<Self, MatrixError> {
        let manifest: Self = read_json(path)?;
        if manifest.version != 1 {
            return Err(MatrixError::UnsupportedVersion {
                path: path.to_path_buf(),
                version: manifest.version,
            });
        }
        Ok(manifest)
    }

    fn open(path: &Path) -> Result<Self, ManifestError> {
        if path.exists() {
            Ok(Self::read(path)?)
        } else {
            Ok(Self {
                version: 1,
                targets: BTreeMap::new(),
                features: BTreeMap::new(),
                invalid_features: Vec::new(),
            })
        }
    }

    fn write(&self, path: &Path) -> Result<(), ManifestError> {
        let error = |source| ManifestError::Write {
            path: path.to_path_buf(),
            source,
        };
        let mut text = serde_json::to_string_pretty(self).expect("serialize manifest");
        text.push('\n');
        let mut temporary = path.as_os_str().to_owned();
        temporary.push(".tmp");
        std::fs::write(&temporary, text).map_err(error)?;
        std::fs::rename(&temporary, path).map_err(error)
    }

    fn feature_of(&self, name_or_define: &str) -> Option<String> {
        if self.features.contains_key(name_or_define) {
            return Some(name_or_define.to_string());
        }
        self.features
            .iter()
            .find(|(_, defines)| {
                defines
                    .iter()
                    .any(|define| macro_of(define) == macro_of(name_or_define))
            })
            .map(|(name, _)| name.clone())
    }
}

pub fn add_target(request: &TargetAdd) -> Result<String, ManifestError> {
    let mut manifest = Manifest::open(request.manifest)?;
    if manifest.targets.contains_key(request.name) && !request.replace {
        return Err(ManifestError::TargetExists(request.name.to_string()));
    }
    let project = absolute_path(Path::new("."), request.project);
    let database = absolute_path(Path::new("."), request.compile_commands);
    let flavor = request
        .flavor
        .map(|flavor| {
            flavor
                .parse::<CompilerFlavor>()
                .map_err(|flavor| MatrixError::UnknownFlavor {
                    target: request.name.to_string(),
                    flavor,
                })
        })
        .transpose()?;
    let (units, selected) = compile_commands::read_units(
        std::slice::from_ref(&database),
        &ReadOptions {
            extra_args: &[],
            flavor,
            build_target: request.build_target,
        },
    )
    .map_err(|source| MatrixError::CompileCommands {
        target: request.name.to_string(),
        source,
    })?;
    let flavor = match flavor {
        Some(flavor) => flavor.to_string(),
        None => {
            let flavors: BTreeSet<&str> = units
                .iter()
                .filter_map(|unit| unit.args.first()?.strip_prefix("--flavor="))
                .collect();
            match flavors.into_iter().collect::<Vec<_>>().as_slice() {
                [flavor] => flavor.to_string(),
                flavors => {
                    return Err(ManifestError::SeveralFlavors {
                        target: request.name.to_string(),
                        database,
                        flavors: flavors.iter().map(|flavor| flavor.to_string()).collect(),
                    });
                }
            }
        }
    };
    let triple = match request.triple {
        Some(triple) => canonical_triple(triple),
        None => {
            let triples: BTreeSet<String> = units
                .iter()
                .filter_map(|unit| command_targets(&unit.args).last())
                .map(canonical_triple)
                .collect();
            let mut triples = triples.into_iter();
            match (triples.next(), triples.len()) {
                (Some(triple), 0) => triple,
                (None, _) => {
                    return Err(ManifestError::NoTriple {
                        target: request.name.to_string(),
                        database,
                    });
                }
                (Some(first), _) => {
                    return Err(ManifestError::SeveralTriples {
                        target: request.name.to_string(),
                        database,
                        triples: std::iter::once(first).chain(triples).collect(),
                    });
                }
            }
        }
    };
    validate_target(request.name, &triple, &flavor)?;
    let target = ManifestTarget {
        triple,
        flavor,
        sysroot: request.sysroot.map(str::to_string),
        compile_commands: database
            .strip_prefix(&project)
            .map_or_else(|_| database.clone(), Path::to_path_buf),
        build_target: selected.as_ref().map(|selected| selected.name.clone()),
    };
    let resolved = read_manifest_target(request.name, &target, &project, &manifest.features)?;
    let mut report = format!(
        "target {}: {} {}, {} units from {}\n",
        request.name,
        target.triple,
        target.flavor,
        resolved.units.len(),
        target.compile_commands.display()
    );
    if let Some(selected) = &selected {
        report.push_str(&format!("build target {}", selected.name));
        if !selected.others.is_empty() {
            report.push_str(&format!(
                " (others: {}, pass --build-target to choose one)",
                selected.others.join(", ")
            ));
        }
        report.push('\n');
    }
    manifest.targets.insert(request.name.to_string(), target);
    manifest.write(request.manifest)?;
    Ok(report)
}

pub fn remove_targets(manifest_path: &Path, names: &[String]) -> Result<String, ManifestError> {
    let mut manifest = Manifest::read(manifest_path)?;
    let mut report = String::new();
    for name in names {
        if manifest.targets.remove(name).is_none() {
            return Err(ManifestError::UnknownTarget(name.clone()));
        }
        report.push_str(&format!("removed target {name}\n"));
    }
    manifest.write(manifest_path)?;
    Ok(report)
}

pub fn add_features(
    manifest_path: &Path,
    project: &Path,
    defines: &[String],
    name: Option<&str>,
) -> Result<String, ManifestError> {
    if defines.is_empty() {
        return Err(ManifestError::NoDefines);
    }
    let mut manifest = Manifest::open(manifest_path)?;
    let added: Vec<(String, Vec<String>)> = match name {
        Some(name) => {
            if !is_cargo_feature_name(name) {
                return Err(MatrixError::InvalidFeatureName(name.to_string()).into());
            }
            vec![(name.to_string(), defines.to_vec())]
        }
        None => defines
            .iter()
            .map(|define| {
                let name = macro_of(define).to_lowercase().replace('_', "-");
                if is_cargo_feature_name(&name) {
                    Ok((name, vec![define.clone()]))
                } else {
                    Err(ManifestError::DerivedFeatureName {
                        define: define.clone(),
                        name,
                    })
                }
            })
            .collect::<Result<_, _>>()?,
    };
    let mut names: BTreeMap<&str, &str> = BTreeMap::new();
    let mut seen: BTreeSet<&str> = BTreeSet::new();
    for (name, feature_defines) in &added {
        if manifest.features.contains_key(name) {
            return Err(ManifestError::FeatureExists(name.clone()));
        }
        if let Some(first) = names.insert(name, &feature_defines[0]) {
            return Err(ManifestError::SameFeatureName {
                name: name.clone(),
                first: first.to_string(),
                second: feature_defines[0].clone(),
            });
        }
        for define in feature_defines {
            if !seen.insert(macro_of(define)) {
                return Err(ManifestError::RepeatedDefine(macro_of(define).to_string()));
            }
            if let Some((feature, _)) = manifest.features.iter().find(|(_, defines)| {
                defines
                    .iter()
                    .any(|existing| macro_of(existing) == macro_of(define))
            }) {
                return Err(ManifestError::DefineExists {
                    name: macro_of(define).to_string(),
                    feature: feature.clone(),
                });
            }
        }
    }
    let project = absolute_path(Path::new("."), project);
    for macro_name in &seen {
        let mut targets = Vec::new();
        for (target_name, target) in &manifest.targets {
            let resolved = read_manifest_target(target_name, target, &project, &BTreeMap::new())?;
            if resolved
                .units
                .values()
                .any(|unit| command_macros(&unit.args).any(|found| found == *macro_name))
            {
                targets.push(target_name.clone());
            }
        }
        if !targets.is_empty() {
            return Err(ManifestError::DefineInBaseCommands {
                name: macro_name.to_string(),
                targets,
            });
        }
    }
    let mut report = String::new();
    for (name, defines) in added {
        report.push_str(&format!("feature {name}: {}\n", defines.join(" ")));
        manifest.features.insert(name, defines);
    }
    manifest.write(manifest_path)?;
    Ok(report)
}

pub fn remove_features(manifest_path: &Path, names: &[String]) -> Result<String, ManifestError> {
    let mut manifest = Manifest::read(manifest_path)?;
    let mut report = String::new();
    for name in names {
        let feature = manifest
            .feature_of(name)
            .ok_or_else(|| ManifestError::UnknownFeature(name.clone()))?;
        manifest.features.remove(&feature);
        report.push_str(&format!("removed feature {feature}\n"));
        manifest.invalid_features.retain(|group| {
            let keep = !group.contains(&feature);
            if !keep {
                report.push_str(&format!("removed conflict {}\n", group.join(" ")));
            }
            keep
        });
    }
    manifest.write(manifest_path)?;
    Ok(report)
}

pub fn add_conflict(manifest_path: &Path, names: &[String]) -> Result<String, ManifestError> {
    let mut manifest = Manifest::read(manifest_path)?;
    let group: BTreeSet<String> = names
        .iter()
        .map(|name| {
            manifest
                .feature_of(name)
                .ok_or_else(|| ManifestError::UnknownFeature(name.clone()))
        })
        .collect::<Result<_, _>>()?;
    if group.len() < 2 {
        return Err(ManifestError::ShortConflict);
    }
    let group: Vec<String> = group.into_iter().collect();
    let line = group.join(" ");
    if manifest.invalid_features.iter().any(|existing| {
        existing.len() == group.len() && existing.iter().all(|name| group.contains(name))
    }) {
        return Ok(format!("conflict {line} already present\n"));
    }
    manifest.invalid_features.push(group);
    manifest.write(manifest_path)?;
    Ok(format!("conflict {line}\n"))
}

fn canonical_triple(triple: &str) -> String {
    target_registry::lookup(triple)
        .map_or_else(|_| triple.to_string(), |spec| spec.triple.to_string())
}
