pub use crate::target::TargetError;
use std::path::Path;
use std::process::{Command, ExitStatus, Stdio};
use thiserror::Error;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Tool {
    Clang,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ToolOperation {
    EmitCir,
}

impl std::fmt::Display for ToolOperation {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::EmitCir => f.write_str("clang -emit-cir"),
        }
    }
}

#[derive(Debug, Error)]
pub enum EmitError {
    #[error(transparent)]
    Target(#[from] TargetError),
    #[error(transparent)]
    Normalize(#[from] clang_ir::Error),
    #[error("spawn {command}: {source}")]
    Spawn {
        tool: Tool,
        operation: ToolOperation,
        command: String,
        #[source]
        source: std::io::Error,
    },
    #[error("{operation} failed:\n{stderr}")]
    ToolFailed {
        tool: Tool,
        operation: ToolOperation,
        status: ExitStatus,
        stderr: String,
    },
}

fn home() -> String {
    std::env::var("HOME").expect("HOME not set")
}

pub fn clang() -> String {
    format!("{}/llvm-project/build-cir/bin/clang", home())
}

fn cir_opt() -> String {
    std::env::var("SLATE_CIR_OPT")
        .unwrap_or_else(|_| format!("{}/llvm-project/build-cir/bin/cir-opt", home()))
}

pub fn target_args() -> Result<Vec<String>, TargetError> {
    let mut args = crate::target::target_override_args(&crate::target::active_target())?;
    if let Ok(extra) = std::env::var("SLATE_CLANG_ARGS") {
        args.extend(extra.split_whitespace().map(str::to_string));
    }
    Ok(args)
}

pub fn emit_generic(src: &Path) -> Result<String, EmitError> {
    emit_generic_with_args(src, &[])
}

pub fn emit_generic_with_args(src: &Path, extra_args: &[String]) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(src, extra_args, &["--cir-canonicalize", "--mem2reg"])
}

pub fn emit_generic_with_args_flattened(
    src: &Path,
    extra_args: &[String],
) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(
        src,
        extra_args,
        &[
            "--verify-each=false",
            "--cir-canonicalize",
            "--cir-flatten-cfg",
            "--cir-goto-solver",
        ],
    )
}

pub fn emit_generic_with_args_cfg_flattened(
    src: &Path,
    extra_args: &[String],
) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(
        src,
        extra_args,
        &[
            "--verify-each=false",
            "--cir-canonicalize",
            "--cir-flatten-cfg",
        ],
    )
}

fn emit_generic_with_args_and_cir_opt_flags(
    src: &Path,
    extra_args: &[String],
    cir_opt_flags: &[&str],
) -> Result<String, EmitError> {
    let clang_command = clang();
    let mut cmd = Command::new(&clang_command);
    let target_args = target_args()?;
    cmd.args([
        "-fclangir",
        "-emit-cir",
        "-std=gnu23",
        "-Xclang",
        "-clangir-disable-passes",
        "-o",
        "-",
    ])
    .args(target_args)
    .args(extra_args)
    .arg(src)
    .stderr(Stdio::piped());
    let clang_out = cmd.output().map_err(|source| EmitError::Spawn {
        tool: Tool::Clang,
        operation: ToolOperation::EmitCir,
        command: clang_command,
        source,
    })?;
    if !clang_out.status.success() {
        return Err(EmitError::ToolFailed {
            tool: Tool::Clang,
            operation: ToolOperation::EmitCir,
            status: clang_out.status,
            stderr: String::from_utf8_lossy(&clang_out.stderr).into_owned(),
        });
    }

    Ok(
        clang_ir::Toolchain::with_cir_opt(cir_opt()).normalize_to_generic_with_flags(
            &String::from_utf8_lossy(&clang_out.stdout),
            cir_opt_flags,
        )?,
    )
}
