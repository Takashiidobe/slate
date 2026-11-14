use crate::compile_commands::{
    self, CompileCommand, CompileCommandsError, ReadOptions, UnitCommand, absolute_path,
};
use serde::Deserialize;
use serde::de::DeserializeOwned;
use slate_parser::compiler_args::{CompilerArgError, CompilerArgParser, CompilerFlavor};
use slate_parser::target_registry;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use thiserror::Error;

#[derive(Debug, Error)]
pub enum MatrixError {
    #[error("read matrix {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("parse matrix {path}: {source}")]
    Parse {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
    #[error("manifest {path} has version {version}, the supported version is 1")]
    UnsupportedVersion { path: PathBuf, version: u32 },
    #[error("matrix has no sources")]
    NoSources,
    #[error("matrix has no targets")]
    NoTargets,
    #[error("duplicate matrix target {0}")]
    DuplicateTarget(String),
    #[error("duplicate matrix feature {0}")]
    DuplicateFeature(String),
    #[error("matrix target {target}: unsupported target triple {triple}")]
    UnsupportedTriple { target: String, triple: String },
    #[error("matrix target {target}: {flavor}")]
    UnknownFlavor { target: String, flavor: String },
    #[error("matrix target {target}: {triple} does not support flavor {flavor}")]
    UnsupportedFlavor {
        target: String,
        triple: String,
        flavor: CompilerFlavor,
    },
    #[error("manifest target {target}: {source}")]
    CompileCommands {
        target: String,
        #[source]
        source: CompileCommandsError,
    },
    #[error("manifest target {target}: {} is compiled for {found}, not {triple}", file.display())]
    TargetMismatch {
        target: String,
        file: PathBuf,
        found: String,
        triple: String,
    },
    #[error(
        "manifest target {target}: {} already sets feature macro {name}, so feature {feature} cannot be turned off",
        file.display()
    )]
    FeatureInBaseCommand {
        target: String,
        file: PathBuf,
        name: String,
        feature: String,
    },
    #[error(
        "manifest targets {target} and {other} compile different sources: only {other} compiles {}",
        file.display()
    )]
    DifferentSources {
        target: String,
        other: String,
        file: PathBuf,
    },
    #[error("matrix feature {0:?} is not a valid Cargo feature name")]
    InvalidFeatureName(String),
    #[error("matrix feature {0} has no defines")]
    FeatureWithoutDefines(String),
    #[error("invalid_features entries need at least two features: {0:?}")]
    ShortInvalidGroup(Vec<String>),
    #[error("invalid_features names unknown matrix feature {0}")]
    InvalidGroupUnknownFeature(String),
    #[error("unknown matrix target {0}")]
    UnknownTarget(String),
    #[error("unknown matrix feature {0}")]
    UnknownFeature(String),
    #[error("unknown matrix source {}", .0.display())]
    UnknownSource(PathBuf),
    #[error("matrix features {} cannot be combined", .0.join(", "))]
    InvalidCombination(Vec<String>),
    #[error("cannot enumerate {features} matrix features, the limit is {limit}")]
    TooManyFeaturesToEnumerate { features: usize, limit: usize },
    #[error("{file}: {source}", file = file.display())]
    Arguments {
        file: PathBuf,
        #[source]
        source: CompilerArgError,
    },
}

#[derive(Debug)]
pub struct Matrix {
    pub sources: Vec<PathBuf>,
    pub targets: Vec<Target>,
    pub features: Vec<Feature>,
    pub invalid_features: Vec<Vec<String>>,
}

#[derive(Debug)]
pub struct Target {
    pub name: String,
    pub triple: String,
    units: BTreeMap<PathBuf, UnitCommand>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Feature {
    pub name: String,
    pub defines: Vec<String>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct MatrixFile {
    pub sources: Vec<PathBuf>,
    #[serde(default)]
    pub args: Vec<String>,
    pub targets: Vec<MatrixTarget>,
    #[serde(default)]
    pub features: Vec<Feature>,
    #[serde(default)]
    pub invalid_features: Vec<Vec<String>>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct MatrixTarget {
    pub name: String,
    pub triple: String,
    pub flavor: String,
    pub sysroot: Option<String>,
    #[serde(default)]
    pub args: Vec<String>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct Manifest {
    version: u32,
    targets: BTreeMap<String, ManifestTarget>,
    #[serde(default)]
    features: BTreeMap<String, Vec<String>>,
    #[serde(default)]
    invalid_features: Vec<Vec<String>>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct ManifestTarget {
    triple: String,
    flavor: String,
    sysroot: Option<String>,
    compile_commands: PathBuf,
    build_target: Option<String>,
}

fn read_json<T: DeserializeOwned>(path: &Path) -> Result<T, MatrixError> {
    let bytes = std::fs::read(path).map_err(|source| MatrixError::Read {
        path: path.to_path_buf(),
        source,
    })?;
    serde_json::from_slice(&bytes).map_err(|source| MatrixError::Parse {
        path: path.to_path_buf(),
        source,
    })
}

impl MatrixFile {
    pub fn resolve(self, project: &Path) -> Result<Matrix, MatrixError> {
        if self.sources.is_empty() {
            return Err(MatrixError::NoSources);
        }
        if self.targets.is_empty() {
            return Err(MatrixError::NoTargets);
        }
        let mut targets = Vec::new();
        for target in &self.targets {
            validate_target(&target.name, &target.triple, &target.flavor)?;
            let mut args = vec![
                format!("--flavor={}", target.flavor),
                format!("--target={}", target.triple),
            ];
            args.extend(
                target
                    .sysroot
                    .iter()
                    .map(|root| format!("--sysroot={root}")),
            );
            args.extend(target.args.iter().cloned());
            args.extend(self.args.iter().cloned());
            let units = self
                .sources
                .iter()
                .map(|source| {
                    let unit = UnitCommand {
                        file: absolute_path(project, source),
                        directory: project.to_path_buf(),
                        args: args.clone(),
                    };
                    (source.clone(), unit)
                })
                .collect();
            targets.push(Target {
                name: target.name.clone(),
                triple: target.triple.clone(),
                units,
            });
        }
        Matrix::new(self.sources, targets, self.features, self.invalid_features)
    }
}

impl Matrix {
    pub fn read(path: &Path, project: &Path) -> Result<Self, MatrixError> {
        read_json::<MatrixFile>(path)?.resolve(project)
    }

    pub fn read_manifest(path: &Path, project: &Path) -> Result<Self, MatrixError> {
        let manifest: Manifest = read_json(path)?;
        if manifest.version != 1 {
            return Err(MatrixError::UnsupportedVersion {
                path: path.to_path_buf(),
                version: manifest.version,
            });
        }
        if manifest.targets.is_empty() {
            return Err(MatrixError::NoTargets);
        }
        let mut targets = Vec::new();
        for (name, target) in &manifest.targets {
            targets.push(read_manifest_target(
                name,
                target,
                project,
                &manifest.features,
            )?);
        }
        let first = &targets[0];
        for target in &targets[1..] {
            let only_in = |having: &'_ Target, lacking: &'_ Target| {
                having
                    .units
                    .keys()
                    .find(|source| !lacking.units.contains_key(*source))
                    .map(|source| MatrixError::DifferentSources {
                        target: lacking.name.clone(),
                        other: having.name.clone(),
                        file: source.clone(),
                    })
            };
            if let Some(error) = only_in(first, target).or_else(|| only_in(target, first)) {
                return Err(error);
            }
        }
        let sources: Vec<PathBuf> = first.units.keys().cloned().collect();
        if sources.is_empty() {
            return Err(MatrixError::NoSources);
        }
        let features = manifest
            .features
            .into_iter()
            .map(|(name, defines)| Feature { name, defines })
            .collect();
        Matrix::new(sources, targets, features, manifest.invalid_features)
    }

    fn new(
        sources: Vec<PathBuf>,
        targets: Vec<Target>,
        features: Vec<Feature>,
        invalid_features: Vec<Vec<String>>,
    ) -> Result<Self, MatrixError> {
        let mut target_names = BTreeSet::new();
        for target in &targets {
            if !target_names.insert(target.name.as_str()) {
                return Err(MatrixError::DuplicateTarget(target.name.clone()));
            }
        }
        let mut feature_names = BTreeSet::new();
        for feature in &features {
            if !feature_names.insert(feature.name.as_str()) {
                return Err(MatrixError::DuplicateFeature(feature.name.clone()));
            }
            if !is_cargo_feature_name(&feature.name) {
                return Err(MatrixError::InvalidFeatureName(feature.name.clone()));
            }
            if feature.defines.is_empty() {
                return Err(MatrixError::FeatureWithoutDefines(feature.name.clone()));
            }
        }
        for group in &invalid_features {
            if group.len() < 2 {
                return Err(MatrixError::ShortInvalidGroup(group.clone()));
            }
            if let Some(unknown) = group
                .iter()
                .find(|name| !feature_names.contains(name.as_str()))
            {
                return Err(MatrixError::InvalidGroupUnknownFeature(unknown.clone()));
            }
        }
        Ok(Self {
            sources,
            targets,
            features,
            invalid_features,
        })
    }

    pub fn is_invalid_combination(&self, features: &[String]) -> bool {
        self.invalid_features
            .iter()
            .any(|group| group.iter().all(|name| features.contains(name)))
    }

    pub fn commands(
        &self,
        target: &str,
        features: &[String],
    ) -> Result<Vec<CompileCommand>, MatrixError> {
        let (target, defines) = self.selection(target, features)?;
        self.sources
            .iter()
            .map(|source| command_for(target.unit(source)?, &defines))
            .collect()
    }

    pub fn command(
        &self,
        target: &str,
        features: &[String],
        source: &Path,
    ) -> Result<CompileCommand, MatrixError> {
        let (target, defines) = self.selection(target, features)?;
        command_for(target.unit(source)?, &defines)
    }

    fn selection(
        &self,
        target: &str,
        features: &[String],
    ) -> Result<(&Target, Vec<String>), MatrixError> {
        let target = self
            .targets
            .iter()
            .find(|candidate| candidate.name == target)
            .ok_or_else(|| MatrixError::UnknownTarget(target.to_string()))?;
        if let Some(unknown) = features
            .iter()
            .find(|name| !self.features.iter().any(|feature| feature.name == **name))
        {
            return Err(MatrixError::UnknownFeature(unknown.clone()));
        }
        if let Some(group) = self
            .invalid_features
            .iter()
            .find(|group| group.iter().all(|name| features.contains(name)))
        {
            return Err(MatrixError::InvalidCombination(group.clone()));
        }
        let defines = self
            .features
            .iter()
            .filter(|feature| features.contains(&feature.name))
            .flat_map(|feature| &feature.defines)
            .map(|define| format!("-D{define}"))
            .collect();
        Ok((target, defines))
    }
}

impl Target {
    fn unit(&self, source: &Path) -> Result<&UnitCommand, MatrixError> {
        self.units
            .get(source)
            .ok_or_else(|| MatrixError::UnknownSource(source.to_path_buf()))
    }
}

fn command_for(unit: &UnitCommand, defines: &[String]) -> Result<CompileCommand, MatrixError> {
    let mut args = unit.args.clone();
    args.extend(defines.iter().cloned());
    match CompilerArgParser::parse_in(crate::target::compiler_arguments(&args), &unit.directory) {
        Ok(args) => Ok(CompileCommand {
            file: unit.file.clone(),
            args,
        }),
        Err(source) => Err(MatrixError::Arguments {
            file: unit.file.clone(),
            source,
        }),
    }
}

fn read_manifest_target(
    name: &str,
    target: &ManifestTarget,
    project: &Path,
    features: &BTreeMap<String, Vec<String>>,
) -> Result<Target, MatrixError> {
    let flavor = validate_target(name, &target.triple, &target.flavor)?;
    let mut extra_args = vec![format!("--target={}", target.triple)];
    extra_args.extend(
        target
            .sysroot
            .iter()
            .map(|root| format!("--sysroot={root}")),
    );
    let database = absolute_path(project, &target.compile_commands);
    let units = compile_commands::read_units(
        &[database],
        &ReadOptions {
            extra_args: &extra_args,
            flavor: Some(flavor),
            build_target: target.build_target.as_deref(),
        },
    )
    .map_err(|source| MatrixError::CompileCommands {
        target: name.to_string(),
        source,
    })?;
    let mut keyed = BTreeMap::new();
    for unit in units {
        if let Some(found) =
            command_targets(&unit.args).find(|found| !same_target(found, &target.triple))
        {
            return Err(MatrixError::TargetMismatch {
                target: name.to_string(),
                file: unit.file.clone(),
                found: found.to_string(),
                triple: target.triple.clone(),
            });
        }
        for macro_name in command_macros(&unit.args) {
            if let Some((feature, _)) = features
                .iter()
                .find(|(_, defines)| defines.iter().any(|define| macro_of(define) == macro_name))
            {
                return Err(MatrixError::FeatureInBaseCommand {
                    target: name.to_string(),
                    file: unit.file.clone(),
                    name: macro_name.to_string(),
                    feature: feature.clone(),
                });
            }
        }
        let key = unit
            .file
            .strip_prefix(project)
            .map_or_else(|_| unit.file.clone(), Path::to_path_buf);
        keyed.insert(key, unit);
    }
    Ok(Target {
        name: name.to_string(),
        triple: target.triple.clone(),
        units: keyed,
    })
}

fn command_targets(args: &[String]) -> impl Iterator<Item = &str> {
    args.iter().enumerate().filter_map(|(index, arg)| {
        arg.strip_prefix("--target=").or_else(|| {
            matches!(arg.as_str(), "-target" | "--target")
                .then(|| args.get(index + 1).map(String::as_str))
                .flatten()
        })
    })
}

fn same_target(found: &str, triple: &str) -> bool {
    match (
        target_registry::lookup(found),
        target_registry::lookup(triple),
    ) {
        (Ok(found), Ok(triple)) => found.triple == triple.triple,
        _ => found == triple,
    }
}

fn command_macros(args: &[String]) -> impl Iterator<Item = &str> {
    args.iter().enumerate().filter_map(|(index, arg)| {
        let value = ["-D", "/D", "-U", "/U"]
            .iter()
            .find_map(|flag| arg.strip_prefix(flag))?;
        let value = match value {
            "" => args.get(index + 1)?.as_str(),
            value => value,
        };
        Some(macro_of(value))
    })
}

fn macro_of(define: &str) -> &str {
    define.split_once('=').map_or(define, |(name, _)| name)
}

fn validate_target(name: &str, triple: &str, flavor: &str) -> Result<CompilerFlavor, MatrixError> {
    let flavor: CompilerFlavor = flavor
        .parse()
        .map_err(|flavor| MatrixError::UnknownFlavor {
            target: name.to_string(),
            flavor,
        })?;
    let spec = target_registry::lookup(triple).map_err(|_| MatrixError::UnsupportedTriple {
        target: name.to_string(),
        triple: triple.to_string(),
    })?;
    if spec.profile.predefines(flavor).is_none() {
        return Err(MatrixError::UnsupportedFlavor {
            target: name.to_string(),
            triple: triple.to_string(),
            flavor,
        });
    }
    Ok(flavor)
}

fn is_cargo_feature_name(name: &str) -> bool {
    let mut chars = name.chars();
    chars
        .next()
        .is_some_and(|first| first.is_ascii_alphanumeric() || first == '_')
        && chars.all(|c| c.is_ascii_alphanumeric() || matches!(c, '-' | '_' | '+'))
}
