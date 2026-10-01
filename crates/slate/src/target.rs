use slate_parser::compiler_args::{CompilerArgError, CompilerArgParser, CompilerArgs};
use slate_parser::target_info::TargetInfo;

pub fn active_target() -> String {
    std::env::var("SLATE_TARGET")
        .ok()
        .filter(|target| !target.trim().is_empty())
        .unwrap_or_else(|| env!("SLATE_BUILD_TARGET").to_string())
}

pub fn compiler_arguments(extra_args: &[String]) -> Vec<String> {
    std::iter::once(format!("--target={}", active_target()))
        .chain(
            std::env::var("SLATE_CLANG_ARGS")
                .unwrap_or_default()
                .split_whitespace()
                .map(str::to_string),
        )
        .chain(extra_args.iter().cloned())
        .collect()
}

pub fn parse_args(extra_args: &[String]) -> Result<CompilerArgs, CompilerArgError> {
    CompilerArgParser::parse(compiler_arguments(extra_args))
}

pub fn active_info() -> TargetInfo {
    let args = parse_args(&[]).unwrap_or_else(|error| panic!("active target required: {error}"));
    args.options.effective_target(args.target)
}
