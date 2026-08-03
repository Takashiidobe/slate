use crate::{c_ast, cir, ctx, directive_translate, fixups, lower, preprocess, rust_ast};
use std::path::Path;

pub fn translate(path: &Path) -> Result<String, String> {
    let (_, program) = lowered_program(path)?;
    if std::env::var("SLATE_RAW_LOWER").is_ok() {
        return Ok(program.emit());
    }
    Ok(fixups::apply_with(program, &skip_set_from_env()?).emit())
}

pub fn lowered_program(path: &Path) -> Result<(cir::ir::Module, rust_ast::Program), String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = preprocess::record_file(&source, &[])?;
    reject_active_unsupported(&pp, "translate")?;
    let diagnostics: Vec<_> = pp
        .directives
        .iter()
        .filter(|directive| {
            matches!(
                directive.name,
                preprocess::DirectiveName::Error | preprocess::DirectiveName::Warning
            )
        })
        .collect();
    for directive in &diagnostics {
        if let (Some(condition), None) = (&directive.condition, directive.active) {
            return Err(format!(
                "translate: cannot determine whether #{} at line {} is active because predicate `{}` cannot be evaluated",
                directive.name.as_str(),
                directive.line_start,
                preprocess::predicate_text(condition)
            ));
        }
    }
    let input = preprocess::clang_input(path, &source, &diagnostics)?;
    let cir_text = cir::emit::emit_generic_with_args(path, input.extra_args())?;
    let module = cir::parse_module(&cir_text)?;
    let unit = c_ast::parse_file_with_args(path, input.extra_args())?;

    let mut ctx = ctx::Ctx::default();
    let mut program = lower::lower(&module, &unit, &mut ctx);
    for d in &ctx.diagnostics.items {
        eprintln!("{:?}: {}", d.severity, d.message);
    }
    if ctx.diagnostics.has_errors() {
        return Err("lowering failed".into());
    }
    let index = program
        .items
        .iter()
        .take_while(|item| matches!(item, rust_ast::Item::CrateAttrs(_)))
        .count();
    program.items.splice(
        index..index,
        diagnostics
            .into_iter()
            .filter(|directive| directive.active == Some(true))
            .enumerate()
            .flat_map(|(index, directive)| {
                if directive.name == preprocess::DirectiveName::Error {
                    vec![rust_ast::Item::Macro {
                        name: "compile_error".into(),
                        args: vec![rust_ast::Expr::Str(directive.raw_payload.clone())],
                    }]
                } else {
                    directive_translate::warning_items(
                        &directive.raw_payload,
                        index,
                        None,
                        directive_translate::WarningBackend::Standalone,
                    )
                }
            }),
    );

    Ok((module, program))
}

pub fn reject_active_unsupported(
    pp: &preprocess::Preprocessing,
    context: &str,
) -> Result<(), String> {
    for directive in pp.directives.iter().filter(|directive| {
        directive.disposition() == preprocess::DirectiveDisposition::UnsupportedSemantic
    }) {
        if directive.is_clang_resolved_pragma() {
            continue;
        }
        match directive.active {
            Some(false) => {}
            Some(true) => return Err(format!("{context}: {}", directive.unsupported_message())),
            None => {
                let condition = directive.condition.as_ref().ok_or_else(|| {
                    format!(
                        "{context}: cannot determine whether {} is active",
                        directive.unsupported_message()
                    )
                })?;
                return Err(format!(
                    "{context}: cannot determine whether {} is active because predicate `{}` cannot be evaluated",
                    directive.unsupported_message(),
                    preprocess::predicate_text(condition)
                ));
            }
        }
    }
    Ok(())
}

pub fn reject_active_unsupported_file(path: &Path, context: &str) -> Result<(), String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = preprocess::record_file(&source, &[])?;
    reject_active_unsupported(&pp, context)
}

pub fn skip_set_from_env() -> Result<fixups::SkipSet, String> {
    match std::env::var("SLATE_SKIP_PASS") {
        Ok(name) if !name.trim().is_empty() => fixups::Pass::parse(name.trim())
            .map(fixups::SkipSet::skip)
            .ok_or_else(|| format!("unknown SLATE_SKIP_PASS: {name}")),
        _ => Ok(fixups::SkipSet::none()),
    }
}
