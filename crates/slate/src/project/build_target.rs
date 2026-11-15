use crate::backend::rust_ast::Runtime;
use slate_parser::compiler_options::CodeModel;
use slate_parser::ir::{MachineCodegen, Module};
use slate_parser::target::isa::TargetIsa;
use slate_parser::target::x86_isa::{X86Feature, X86Isa};
use slate_parser::target_info::TargetFamily;
use std::path::Path;

pub const I386_SPEC_FILE: &str = "i386-unknown-none.json";

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuildTarget {
    pub triple: String,
    pub spec: Option<String>,
    pub rustflags: Vec<String>,
    pub build_std: bool,
}

impl BuildTarget {
    pub fn for_module(module: &Module) -> Self {
        let target = &module.target;
        let host = Self {
            triple: target.triple.clone(),
            spec: None,
            rustflags: target
                .code16
                .then(|| "-Ctarget-feature=+16bit-mode,-32bit-mode".to_string())
                .into_iter()
                .collect(),
            build_std: false,
        };
        let TargetIsa::X86(isa) = target.isa else {
            return host;
        };
        if Runtime::for_module(module).has_std() || isa.features.contains(X86Feature::Sse) {
            return host;
        }
        let machine = module.machine;
        match target.family {
            TargetFamily::X86_64 if !target.code16 => Self {
                triple: "x86_64-unknown-none".to_string(),
                spec: None,
                rustflags: x86_64_flags(machine)
                    .chain(shared_flags(machine, isa))
                    .collect(),
                build_std: true,
            },
            TargetFamily::X86_64 | TargetFamily::X86 => Self {
                triple: I386_SPEC_FILE.to_string(),
                spec: Some(i386_spec(isa, target.code16)),
                rustflags: shared_flags(machine, isa).collect(),
                build_std: true,
            },
            _ => host,
        }
    }

    pub fn describe(&self) -> String {
        if self.rustflags.is_empty() {
            self.triple.clone()
        } else {
            format!("{} with {}", self.triple, self.rustflags.join(" "))
        }
    }

    fn is_host_default(&self) -> bool {
        self.triple == env!("SLATE_BUILD_TARGET")
            && self.spec.is_none()
            && self.rustflags.is_empty()
            && !self.build_std
    }

    pub fn write_config(target: Option<&Self>, crate_dir: &Path) -> Result<(), String> {
        let spec_path = crate_dir.join(I386_SPEC_FILE);
        match target.and_then(|target| target.spec.as_ref()) {
            Some(spec) => write(&spec_path, spec)?,
            None => remove(&spec_path)?,
        }
        let directory = crate_dir.join(".cargo");
        let path = directory.join("config.toml");
        let Some(target) = target.filter(|target| !target.is_host_default()) else {
            return remove(&path);
        };
        let mut config = format!("[build]\ntarget = \"{}\"\n", target.triple);
        if !target.rustflags.is_empty() {
            let flags: Vec<String> = target
                .rustflags
                .iter()
                .map(|flag| format!("\"{flag}\""))
                .collect();
            config.push_str(&format!("rustflags = [{}]\n", flags.join(", ")));
        }
        if target.build_std || target.spec.is_some() {
            config.push_str("\n[unstable]\n");
        }
        if target.build_std {
            config.push_str("build-std = [\"core\", \"compiler_builtins\"]\n");
        }
        if target.spec.is_some() {
            config.push_str("json-target-spec = true\n");
        }
        std::fs::create_dir_all(&directory)
            .map_err(|e| format!("create {}: {e}", directory.display()))?;
        write(&path, &config)
    }
}

pub fn project_build_target<'a>(
    units: impl IntoIterator<Item = (&'a str, &'a Module)>,
) -> Result<Option<BuildTarget>, String> {
    let mut first: Option<(&str, BuildTarget)> = None;
    for (stem, module) in units {
        let target = BuildTarget::for_module(module);
        match &first {
            None => first = Some((stem, target)),
            Some((first_stem, first_target)) if *first_target != target => {
                return Err(format!(
                    "units {first_stem} and {stem} build for different targets ({} and {}); translate them as separate projects",
                    first_target.describe(),
                    target.describe()
                ));
            }
            Some(_) => {}
        }
    }
    Ok(first.map(|(_, target)| target))
}

fn x86_64_flags(machine: MachineCodegen) -> impl Iterator<Item = String> {
    let code_model = match machine.code_model.unwrap_or(CodeModel::Small) {
        CodeModel::Kernel => None,
        model => Some(format!("-Ccode-model={}", model.spelling())),
    };
    let red_zone = (machine.red_zone != Some(false)).then(|| "-Cno-redzone=n".to_string());
    code_model.into_iter().chain(red_zone)
}

fn shared_flags(machine: MachineCodegen, isa: X86Isa) -> impl Iterator<Item = String> {
    let relocation = machine.pic.map(|pic| {
        let model = match pic.level {
            0 => "static",
            _ if pic.executable => "pie",
            _ => "pic",
        };
        format!("-Crelocation-model={model}")
    });
    let cf_protection = machine.cf_protection.and_then(|bits| {
        let level = match bits & 3 {
            1 => "branch",
            2 => "return",
            3 => "full",
            _ => return None,
        };
        Some(format!("-Zcf-protection={level}"))
    });
    let switches = [
        (
            isa.features.contains(X86Feature::RetpolineExternalThunk),
            "-Zretpoline-external-thunk",
        ),
        (machine.retpoline, "-Zretpoline"),
        (
            machine.indirect_branch_cs_prefix,
            "-Zindirect-branch-cs-prefix",
        ),
        (
            !isa.features.contains(X86Feature::X87),
            "-Ctarget-feature=-x87",
        ),
    ]
    .into_iter()
    .filter(|(on, _)| *on)
    .map(|(_, flag)| flag.to_string());
    relocation.into_iter().chain(cf_protection).chain(switches)
}

fn i386_spec(isa: X86Isa, code16: bool) -> String {
    let mode = if code16 {
        ",+16bit-mode,-32bit-mode"
    } else {
        ""
    };
    format!(
        r#"{{
  "arch": "x86",
  "cpu": "{cpu}",
  "data-layout": "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-i128:128-f64:32:64-f80:32-n8:16:32-S128",
  "llvm-target": "i386-unknown-none-elf",
  "target-pointer-width": 32,
  "max-atomic-width": 64,
  "os": "none",
  "panic-strategy": "abort",
  "disable-redzone": true,
  "features": "-mmx,-sse,+soft-float{mode}",
  "rustc-abi": "softfloat",
  "linker": "rust-lld",
  "linker-flavor": "gnu-lld",
  "relocation-model": "static",
  "position-independent-executables": false,
  "crt-objects-fallback": "false",
  "stack-probes": {{ "kind": "inline" }}
}}
"#,
        cpu = isa.arch.name()
    )
}

fn write(path: &Path, contents: &str) -> Result<(), String> {
    std::fs::write(path, contents).map_err(|e| format!("write {}: {e}", path.display()))
}

fn remove(path: &Path) -> Result<(), String> {
    if path.exists() {
        std::fs::remove_file(path).map_err(|e| format!("remove {}: {e}", path.display()))
    } else {
        Ok(())
    }
}
