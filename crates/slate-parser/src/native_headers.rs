use crate::compiler_args::CompilerFlavor;
use crate::target_info::TargetInfo;
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::process::{Command, Output, Stdio};
use std::sync::{Mutex, OnceLock};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct NativeHeaders {
    pub compiler: String,
    pub compiler_root: Option<PathBuf>,
    pub include: Vec<PathBuf>,
}

impl NativeHeaders {
    pub fn system_paths(&self, with_libc: bool) -> Vec<PathBuf> {
        self.include
            .iter()
            .filter(|path| with_libc || self.is_compiler_path(path))
            .cloned()
            .collect()
    }

    pub fn is_compiler_path(&self, path: &Path) -> bool {
        self.compiler_root
            .as_deref()
            .is_some_and(|root| canonical(path).starts_with(canonical(root)))
    }
}

#[derive(Debug, Clone, thiserror::Error)]
pub enum NativeHeaderError {
    #[error("could not run `{compiler}`: {reason}")]
    Spawn { compiler: String, reason: String },
    #[error("`{compiler}` failed: {stderr}")]
    Failed { compiler: String, stderr: String },
    #[error("`{compiler}` printed no system include directories")]
    NoSearchList { compiler: String },
    #[error("no MSVC toolchain found for {triple}")]
    MsvcNotFound { triple: String },
}

pub fn is_host(target: &TargetInfo) -> bool {
    crate::target_registry::host().is_some_and(|spec| spec.triple == target.triple)
}

pub fn discover(
    target: &TargetInfo,
    flavor: CompilerFlavor,
) -> Result<NativeHeaders, NativeHeaderError> {
    type Cache = Mutex<HashMap<(String, CompilerFlavor), Result<NativeHeaders, NativeHeaderError>>>;
    static CACHE: OnceLock<Cache> = OnceLock::new();
    let key = (target.triple.clone(), flavor);
    let cache = CACHE.get_or_init(Default::default);
    if let Some(cached) = cache.lock().ok().and_then(|cache| cache.get(&key).cloned()) {
        return cached;
    }
    let discovered = match flavor {
        CompilerFlavor::Clang => clang(&target.triple),
        CompilerFlavor::Gcc => gcc(),
        CompilerFlavor::Msvc => msvc(&target.triple),
    };
    if let Ok(mut cache) = cache.lock() {
        cache.insert(key, discovered.clone());
    }
    discovered
}

fn clang(triple: &str) -> Result<NativeHeaders, NativeHeaderError> {
    let target = format!("--target={triple}");
    let root = stdout_line("clang", &[&target, "-print-resource-dir"])?;
    Ok(NativeHeaders {
        compiler: "clang".into(),
        compiler_root: Some(PathBuf::from(root)),
        include: search_list("clang", &[&target])?,
    })
}

fn gcc() -> Result<NativeHeaders, NativeHeaderError> {
    let include = stdout_line("gcc", &["-print-file-name=include"])?;
    Ok(NativeHeaders {
        compiler: "gcc".into(),
        compiler_root: Path::new(&include).parent().map(Path::to_path_buf),
        include: search_list("gcc", &[])?,
    })
}

fn msvc(triple: &str) -> Result<NativeHeaders, NativeHeaderError> {
    let tool = find_msvc_tools::find_tool(triple, "cl.exe");
    let compiler_root = tool
        .as_ref()
        .and_then(|tool| tool.path().ancestors().nth(4))
        .map(Path::to_path_buf);
    let include = std::env::var_os("INCLUDE")
        .filter(|include| !include.is_empty())
        .or_else(|| {
            tool.as_ref()?
                .env()
                .into_iter()
                .find(|(name, _)| name == "INCLUDE")
                .map(|(_, value)| value.clone())
        })
        .ok_or_else(|| NativeHeaderError::MsvcNotFound {
            triple: triple.into(),
        })?;
    Ok(NativeHeaders {
        compiler: "cl.exe".into(),
        compiler_root,
        include: std::env::split_paths(&include)
            .filter(|path| !path.as_os_str().is_empty())
            .collect(),
    })
}

fn search_list(compiler: &str, args: &[&str]) -> Result<Vec<PathBuf>, NativeHeaderError> {
    let mut args = args.to_vec();
    args.extend(["-E", "-v", "-x", "c", "-"]);
    let output = run(compiler, &args)?;
    let include: Vec<PathBuf> = String::from_utf8_lossy(&output.stderr)
        .lines()
        .skip_while(|line| !line.starts_with("#include <...> search starts here:"))
        .skip(1)
        .take_while(|line| !line.starts_with("End of search list."))
        .filter(|line| !line.ends_with("(framework directory)"))
        .map(|line| PathBuf::from(line.trim()))
        .collect();
    if include.is_empty() {
        return Err(NativeHeaderError::NoSearchList {
            compiler: compiler.into(),
        });
    }
    Ok(include)
}

fn stdout_line(compiler: &str, args: &[&str]) -> Result<String, NativeHeaderError> {
    let output = run(compiler, args)?;
    Ok(String::from_utf8_lossy(&output.stdout).trim().to_string())
}

fn run(compiler: &str, args: &[&str]) -> Result<Output, NativeHeaderError> {
    let output = Command::new(compiler)
        .args(args)
        .env("LC_ALL", "C")
        .stdin(Stdio::null())
        .output()
        .map_err(|error| NativeHeaderError::Spawn {
            compiler: compiler.into(),
            reason: error.to_string(),
        })?;
    if !output.status.success() {
        return Err(NativeHeaderError::Failed {
            compiler: compiler.into(),
            stderr: String::from_utf8_lossy(&output.stderr).trim().to_string(),
        });
    }
    Ok(output)
}

fn canonical(path: &Path) -> PathBuf {
    path.canonicalize().unwrap_or_else(|_| path.to_path_buf())
}
