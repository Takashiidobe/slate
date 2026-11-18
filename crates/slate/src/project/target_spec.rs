use serde::{Serialize, Serializer};

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
#[serde(rename_all = "kebab-case")]
pub struct TargetSpec {
    pub arch: Arch,
    pub cpu: String,
    pub data_layout: String,
    pub llvm_target: String,
    pub target_pointer_width: u32,
    pub max_atomic_width: u32,
    pub os: Os,
    pub panic_strategy: PanicStrategy,
    pub disable_redzone: bool,
    pub features: Features,
    pub rustc_abi: RustcAbi,
    pub linker: String,
    pub linker_flavor: LinkerFlavor,
    pub relocation_model: RelocationModel,
    pub position_independent_executables: bool,
    pub crt_objects_fallback: CrtObjectsFallback,
    pub stack_probes: StackProbes,
}

impl TargetSpec {
    pub fn to_json(&self) -> Result<String, serde_json::Error> {
        serde_json::to_string_pretty(self).map(|text| text + "\n")
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum Arch {
    X86,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum Os {
    None,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum PanicStrategy {
    Abort,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum RustcAbi {
    Softfloat,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "kebab-case")]
pub enum LinkerFlavor {
    GnuLld,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum RelocationModel {
    Static,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(rename_all = "lowercase")]
pub enum CrtObjectsFallback {
    False,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
#[serde(tag = "kind", rename_all = "lowercase")]
pub enum StackProbes {
    Inline,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Feature {
    pub enabled: bool,
    pub name: &'static str,
}

impl Feature {
    pub fn on(name: &'static str) -> Self {
        Self {
            enabled: true,
            name,
        }
    }

    pub fn off(name: &'static str) -> Self {
        Self {
            enabled: false,
            name,
        }
    }
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct Features(pub Vec<Feature>);

impl Serialize for Features {
    fn serialize<S: Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        let text = self
            .0
            .iter()
            .map(|feature| {
                format!(
                    "{}{}",
                    if feature.enabled { '+' } else { '-' },
                    feature.name
                )
            })
            .collect::<Vec<_>>()
            .join(",");
        serializer.serialize_str(&text)
    }
}
