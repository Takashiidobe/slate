use miette::{Diagnostic, Severity};
use serde_json::{Value, json};
use slate_parser::ast::Loc;
use slate_parser::files::Files;
use std::path::Path;

pub fn collect(diagnostic: &dyn Diagnostic, output: &mut Vec<Value>) {
    let labels: Vec<_> = diagnostic
        .labels()
        .into_iter()
        .flatten()
        .map(|label| {
            let contents = diagnostic
                .source_code()
                .and_then(|source| source.read_span(label.inner(), 0, 0).ok());
            json!({
                "file": contents.as_ref().and_then(|contents| contents.name()),
                "line": contents.as_ref().map(|contents| contents.line() + 1),
                "column": contents.as_ref().map(|contents| contents.column() + 1),
                "byteOffset": label.offset(),
                "byteLength": label.len(),
                "message": label.label(),
                "primary": label.primary(),
            })
        })
        .collect();
    let related: Vec<_> = diagnostic.related().into_iter().flatten().collect();
    if !labels.is_empty() || related.is_empty() {
        output.push(json!({
            "severity": match diagnostic.severity().unwrap_or(Severity::Error) {
                Severity::Error => "error",
                Severity::Warning => "warning",
                Severity::Advice => "note",
            },
            "message": diagnostic.to_string(),
            "code": diagnostic.code().map(|code| code.to_string()),
            "help": diagnostic.help().map(|help| help.to_string()),
            "labels": labels,
        }));
    }
    for related in related {
        collect(related, output);
    }
}

pub fn location(loc: Loc, files: &Files, message: &str, primary: bool) -> Value {
    json!({
        "file": files.get_path(loc.file).map(|path| path.display().to_string()),
        "line": files.position(loc.file, loc.offset).map(|(line, _)| line + 1),
        "column": files.position(loc.file, loc.offset).map(|(_, column)| column + 1),
        "byteOffset": loc.offset,
        "byteLength": loc.length,
        "message": message,
        "primary": primary,
    })
}

pub fn frontend(error: &crate::frontend::Error, files: &Files) -> Value {
    use crate::frontend::Error;
    let (site, message, help, context) = match error {
        Error::Unsupported { barrier, .. } => (
            &barrier.site,
            format!("unsupported C construct: {}", barrier.construct.label()),
            barrier.construct.to_string(),
            &barrier.context,
        ),
        Error::Invalid { invalid, .. } => (
            &invalid.site,
            "invalid slate-parser IR".to_owned(),
            invalid.invariant.to_string(),
            &invalid.context,
        ),
        _ => return plain(&error.to_string()),
    };
    let mut labels = vec![location(site.expansion, files, &message, true)];
    if site.spelling != site.expansion {
        labels.push(location(site.spelling, files, "spelled here", false));
    }
    labels.extend(
        context
            .iter()
            .map(|context| location(context.site.expansion, files, &context.label, false)),
    );
    json!({"severity": "error", "message": message, "help": help, "labels": labels})
}

pub fn plain(message: &str) -> Value {
    json!({"severity": "error", "message": message, "labels": []})
}

pub fn directive(
    error: &crate::frontend::directive_translate::DirectiveError,
    path: &Path,
) -> Value {
    use crate::frontend::directive_translate::DirectiveError;
    let line = match error {
        DirectiveError::UnsupportedDirective { line, .. }
        | DirectiveError::UnmappableDirectiveGuard { line, .. }
        | DirectiveError::ConditionalInBody { line, .. }
        | DirectiveError::VariantCapExceeded { line, .. }
        | DirectiveError::UnmappablePredicate { line, .. }
        | DirectiveError::UnselectableBranch { line, .. } => Some(*line),
        _ => None,
    };
    let mut diagnostic = plain(&error.to_string());
    if let Some(line) = line {
        diagnostic["labels"] = json!([{
            "file": path.display().to_string(),
            "line": line,
            "column": 1,
            "primary": true,
        }]);
    }
    diagnostic
}
