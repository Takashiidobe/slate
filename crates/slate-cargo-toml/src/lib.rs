use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;
use std::path::PathBuf;

pub use toml::de::Error as DeError;
pub use toml::ser::Error as SerError;

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct Manifest {
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub cargo_features: Vec<CargoFeature>,
    pub package: Package,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub lib: Option<Lib>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub workspace: Option<Workspace>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub dependencies: BTreeMap<String, Dependency>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub build_dependencies: BTreeMap<String, Dependency>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub features: BTreeMap<String, Vec<String>>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub lints: BTreeMap<LintTool, BTreeMap<String, LintLevel>>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub profile: BTreeMap<String, Profile>,
    #[serde(flatten)]
    pub extra: toml::Table,
}

impl Manifest {
    pub fn new(name: impl Into<String>, version: impl Into<String>, edition: Edition) -> Self {
        Self {
            package: Package {
                name: name.into(),
                version: version.into(),
                edition,
                autobins: None,
                extra: toml::Table::new(),
            },
            ..Self::default()
        }
    }

    pub fn parse(text: &str) -> Result<Self, DeError> {
        toml::from_str(text)
    }

    pub fn to_toml(&self) -> Result<String, SerError> {
        toml::to_string(self)
    }

    pub fn profile_mut(&mut self, name: &str) -> &mut Profile {
        self.profile.entry(name.to_string()).or_default()
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub enum CargoFeature {
    ProfileRustflags,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct Package {
    pub name: String,
    pub version: String,
    pub edition: Edition,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub autobins: Option<bool>,
    #[serde(flatten)]
    pub extra: toml::Table,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Serialize, Deserialize)]
pub enum Edition {
    #[serde(rename = "2015")]
    E2015,
    #[serde(rename = "2018")]
    E2018,
    #[serde(rename = "2021")]
    E2021,
    #[default]
    #[serde(rename = "2024")]
    E2024,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct Lib {
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub path: Option<PathBuf>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub doctest: Option<bool>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub crate_type: Vec<String>,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
pub struct Workspace {}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
#[serde(untagged)]
pub enum Dependency {
    Version(String),
    Detailed(DependencyDetail),
}

impl Dependency {
    pub fn version(version: impl Into<String>) -> Self {
        Self::Version(version.into())
    }

    pub fn path(path: impl Into<PathBuf>) -> Self {
        Self::Detailed(DependencyDetail {
            path: Some(path.into()),
            ..DependencyDetail::default()
        })
    }

    pub fn without_default_features(self) -> Self {
        let mut detail = match self {
            Self::Version(version) => DependencyDetail {
                version: Some(version),
                ..DependencyDetail::default()
            },
            Self::Detailed(detail) => detail,
        };
        detail.default_features = Some(false);
        Self::Detailed(detail)
    }

    pub fn renamed_from(self, package: impl Into<String>) -> Self {
        let mut detail = match self {
            Self::Version(version) => DependencyDetail {
                version: Some(version),
                ..DependencyDetail::default()
            },
            Self::Detailed(detail) => detail,
        };
        detail.package = Some(package.into());
        Self::Detailed(detail)
    }
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct DependencyDetail {
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub version: Option<String>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub path: Option<PathBuf>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub package: Option<String>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub features: Vec<String>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub optional: Option<bool>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub default_features: Option<bool>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum LintTool {
    Rust,
    Clippy,
    Rustdoc,
    Cargo,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum LintLevel {
    Allow,
    Warn,
    Deny,
    Forbid,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct Profile {
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub inherits: Option<String>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub opt_level: Option<u8>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub debug: Option<bool>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub panic: Option<Panic>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub incremental: Option<bool>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub overflow_checks: Option<bool>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub debug_assertions: Option<bool>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub codegen_units: Option<u32>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub rustflags: Vec<String>,
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub package: BTreeMap<String, Profile>,
}

impl Profile {
    pub fn uses_rustflags(&self) -> bool {
        !self.rustflags.is_empty() || self.package.values().any(Profile::uses_rustflags)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum Panic {
    Unwind,
    Abort,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct CargoConfig {
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub build: Option<BuildConfig>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub unstable: Option<UnstableConfig>,
}

impl CargoConfig {
    pub fn parse(text: &str) -> Result<Self, DeError> {
        toml::from_str(text)
    }

    pub fn to_toml(&self) -> Result<String, SerError> {
        toml::to_string(self)
    }
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct BuildConfig {
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub target: Option<String>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub rustflags: Vec<String>,
}

#[derive(Debug, Clone, Default, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub struct UnstableConfig {
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub build_std: Vec<StdCrate>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub json_target_spec: Option<bool>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
pub enum StdCrate {
    Core,
    CompilerBuiltins,
    Alloc,
    Std,
}
