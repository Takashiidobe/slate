use crate::compile_commands::{CompileCommand, absolute_path};
use serde::Deserialize;
use slate_parser::compiler_args::{CompilerArgError, CompilerArgParser, CompilerFlavor};
use slate_parser::target_registry;
use std::collections::BTreeSet;
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

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Matrix {
    pub sources: Vec<PathBuf>,
    #[serde(default)]
    pub args: Vec<String>,
    pub targets: Vec<Target>,
    #[serde(default)]
    pub features: Vec<Feature>,
    #[serde(default)]
    pub invalid_features: Vec<Vec<String>>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Target {
    pub name: String,
    pub triple: String,
    pub flavor: String,
    pub sysroot: Option<String>,
    #[serde(default)]
    pub args: Vec<String>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Feature {
    pub name: String,
    pub defines: Vec<String>,
}

impl Matrix {
    pub fn read(path: &Path) -> Result<Self, MatrixError> {
        let bytes = std::fs::read(path).map_err(|source| MatrixError::Read {
            path: path.to_path_buf(),
            source,
        })?;
        let matrix: Self = serde_json::from_slice(&bytes).map_err(|source| MatrixError::Parse {
            path: path.to_path_buf(),
            source,
        })?;
        matrix.validate()?;
        Ok(matrix)
    }

    fn validate(&self) -> Result<(), MatrixError> {
        if self.sources.is_empty() {
            return Err(MatrixError::NoSources);
        }
        if self.targets.is_empty() {
            return Err(MatrixError::NoTargets);
        }
        let mut target_names = BTreeSet::new();
        for target in &self.targets {
            if !target_names.insert(target.name.as_str()) {
                return Err(MatrixError::DuplicateTarget(target.name.clone()));
            }
            target.validate()?;
        }
        let mut feature_names = BTreeSet::new();
        for feature in &self.features {
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
        for group in &self.invalid_features {
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
        Ok(())
    }

    pub fn is_invalid_combination(&self, features: &[String]) -> bool {
        self.invalid_features
            .iter()
            .any(|group| group.iter().all(|name| features.contains(name)))
    }

    pub fn commands(
        &self,
        project: &Path,
        target: &str,
        features: &[String],
    ) -> Result<Vec<CompileCommand>, MatrixError> {
        let args = self.arguments(target, features)?;
        self.sources
            .iter()
            .map(|source| command_for(project, source, &args))
            .collect()
    }

    pub fn command(
        &self,
        project: &Path,
        target: &str,
        features: &[String],
        source: &Path,
    ) -> Result<CompileCommand, MatrixError> {
        command_for(project, source, &self.arguments(target, features)?)
    }

    fn arguments(&self, target: &str, features: &[String]) -> Result<Vec<String>, MatrixError> {
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
        args.extend(
            self.features
                .iter()
                .filter(|feature| features.contains(&feature.name))
                .flat_map(|feature| &feature.defines)
                .map(|define| format!("-D{define}")),
        );
        Ok(crate::target::compiler_arguments(&args))
    }
}

fn command_for(
    project: &Path,
    source: &Path,
    args: &[String],
) -> Result<CompileCommand, MatrixError> {
    let file = absolute_path(project, source);
    match CompilerArgParser::parse_in(args.iter().cloned(), project) {
        Ok(args) => Ok(CompileCommand { file, args }),
        Err(source) => Err(MatrixError::Arguments { file, source }),
    }
}

impl Target {
    fn validate(&self) -> Result<(), MatrixError> {
        let flavor: CompilerFlavor =
            self.flavor
                .parse()
                .map_err(|flavor| MatrixError::UnknownFlavor {
                    target: self.name.clone(),
                    flavor,
                })?;
        let spec =
            target_registry::lookup(&self.triple).map_err(|_| MatrixError::UnsupportedTriple {
                target: self.name.clone(),
                triple: self.triple.clone(),
            })?;
        if spec.profile.predefines(flavor).is_none() {
            return Err(MatrixError::UnsupportedFlavor {
                target: self.name.clone(),
                triple: self.triple.clone(),
                flavor,
            });
        }
        Ok(())
    }
}

fn is_cargo_feature_name(name: &str) -> bool {
    let mut chars = name.chars();
    chars
        .next()
        .is_some_and(|first| first.is_ascii_alphanumeric() || first == '_')
        && chars.all(|c| c.is_ascii_alphanumeric() || matches!(c, '-' | '_' | '+'))
}
