use super::*;

pub(super) fn rust_binding_name(name: &str) -> String {
    const KEYWORDS: &[&str] = &[
        "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn",
        "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref",
        "return", "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe",
        "use", "where", "while", "async", "await", "dyn",
    ];
    if KEYWORDS.contains(&name) {
        format!("r#{name}")
    } else {
        name.to_owned()
    }
}

pub(super) fn local_binding_name(name: &str, id: BindingId, statics: &HashSet<String>) -> String {
    const PRELUDE_VARIANTS: &[&str] = &["Some", "None", "Ok", "Err"];
    let name = rust_binding_name(name);
    if statics.contains(&name) || PRELUDE_VARIANTS.contains(&name.as_str()) {
        format!("{}_{}", name.trim_start_matches("r#"), id.0)
    } else {
        name
    }
}

pub(super) fn global_names<'a>(
    module: &ir::Module,
    function_names: impl IntoIterator<Item = &'a str>,
) -> HashMap<BindingId, String> {
    let mut owners = HashMap::<String, HashSet<Option<BindingId>>>::new();
    for name in function_names {
        owners
            .entry(rust_binding_name(name))
            .or_default()
            .insert(None);
    }
    for global in &module.globals {
        owners
            .entry(rust_binding_name(&global.variable.name))
            .or_default()
            .insert(Some(global.variable.id));
    }
    module
        .globals
        .iter()
        .map(|global| {
            let id = global.variable.id;
            let name = rust_binding_name(&global.variable.name);
            let name = if matches!(global.linkage, ir::Linkage::Internal) && owners[&name].len() > 1
            {
                format!("{}_{}", name.trim_start_matches("r#"), id.0)
            } else {
                name
            };
            (id, name)
        })
        .collect()
}

pub(super) fn binding_name(id: BindingId, bindings: &HashMap<BindingId, String>) -> String {
    bindings
        .get(&id)
        .cloned()
        .unwrap_or_else(|| format!("__v{}", id.0))
}

pub(super) fn collect_statement_names(
    statements: &[slate_parser::ast::Span<ir::Statement>],
    bindings: &mut HashMap<BindingId, String>,
    reserved: &HashSet<String>,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::Let(variable) => {
                bindings.insert(
                    variable.id,
                    local_binding_name(&variable.name, variable.id, reserved),
                );
            }
            ir::Statement::Temporary { id, .. } => {
                bindings
                    .entry(*id)
                    .or_insert_with(|| format!("__v{}", id.0));
            }
            ir::Statement::Block(body)
            | ir::Statement::While { body, .. }
            | ir::Statement::DoWhile { body, .. }
            | ir::Statement::Switch { body, .. }
            | ir::Statement::Label { body, .. }
            | ir::Statement::Case { body, .. }
            | ir::Statement::Default { body, .. } => {
                collect_statement_names(body, bindings, reserved)
            }
            ir::Statement::For { init, body, .. } => {
                collect_statement_names(init, bindings, reserved);
                collect_statement_names(body, bindings, reserved);
            }
            ir::Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_statement_names(then_body, bindings, reserved);
                if let Some(else_body) = else_body {
                    collect_statement_names(else_body, bindings, reserved);
                }
            }
            _ => {}
        }
    }
}

pub(super) fn record_names(module: &ir::Module) -> HashMap<TypeId, String> {
    let mut counts = HashMap::<&str, usize>::new();
    for definition in &module.types {
        if let (Some(name), ir::TypeDefinitionKind::Record { .. }) =
            (&definition.name, &definition.kind)
        {
            *counts.entry(name).or_default() += 1;
        }
    }
    module
        .types
        .iter()
        .filter(|definition| matches!(definition.kind, ir::TypeDefinitionKind::Record { .. }))
        .map(|definition| {
            let id = definition.value.id;
            let name = match definition.name.as_deref() {
                Some(name) if counts[name] == 1 => name.to_owned(),
                Some(name) => format!("{name}_{}", id.0),
                None => format!("__SlateRecord{}", id.0),
            };
            (id, name)
        })
        .collect()
}

pub(super) fn is_camel_case(name: &str) -> bool {
    let name = name.trim_matches('_');
    let chars: Vec<char> = name.chars().collect();
    !chars.first().is_some_and(|first| first.is_lowercase())
        && !name.contains("__")
        && !chars.windows(2).any(|pair| {
            let has_case = |c: char| c.is_lowercase() || c.is_uppercase();
            (has_case(pair[0]) && pair[1] == '_') || (has_case(pair[1]) && pair[0] == '_')
        })
}
