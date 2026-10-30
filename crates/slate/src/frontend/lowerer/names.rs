use super::*;

pub(super) fn path_keyword_free(name: &str) -> String {
    if matches!(name, "crate" | "self" | "Self" | "super") {
        format!("{name}_")
    } else {
        name.to_owned()
    }
}

pub(super) fn rust_binding_name(name: &str) -> String {
    const KEYWORDS: &[&str] = &[
        "as", "break", "const", "continue", "else", "enum", "extern", "false", "fn", "for", "if",
        "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return",
        "static", "struct", "trait", "true", "type", "unsafe", "use", "where", "while", "async",
        "await", "dyn",
    ];
    if let Some(synthetic) = name.strip_prefix('.') {
        format!("__slate_{}", synthetic.replace('.', "_"))
    } else if KEYWORDS.contains(&name) {
        format!("r#{name}")
    } else {
        path_keyword_free(name)
    }
}

pub(super) fn local_binding_name(name: &str, next: &mut u32, statics: &HashSet<String>) -> String {
    const PRELUDE_VARIANTS: &[&str] = &["Some", "None", "Ok", "Err"];
    let name = rust_binding_name(name);
    if statics.contains(&name) || PRELUDE_VARIANTS.contains(&name.as_str()) {
        format!("{}__{}", name.trim_start_matches("r#"), next_local(next))
    } else {
        name
    }
}

pub(super) fn owner_name(name: &str) -> String {
    rust_binding_name(name).trim_start_matches("r#").to_owned()
}

pub(super) fn next_local(next: &mut u32) -> u32 {
    let index = *next;
    *next += 1;
    index
}

fn source_line(files: &Files, loc: slate_parser::ast::Loc) -> usize {
    files
        .position(loc.file, loc.offset)
        .map_or(0, |(line, _)| line + 1)
}

fn unique_name(name: String, taken: &mut HashSet<String>) -> String {
    let mut candidate = name.clone();
    let mut suffix = 1;
    while !taken.insert(candidate.clone()) {
        suffix += 1;
        candidate = format!("{name}_{suffix}");
    }
    candidate
}

pub(super) fn alias_target<'m>(
    module: &'m ir::Module,
    global: &ir::Global,
) -> Option<&'m ir::Global> {
    let target = global.symbol.alias.as_deref()?;
    module
        .globals
        .iter()
        .map(|candidate| &**candidate)
        .find(|candidate| {
            candidate.definition
                && candidate.variable.name == target
                && candidate.variable.ty == global.variable.ty
                && candidate.symbol.alias.is_none()
        })
}

pub(super) fn global_names<'a>(
    module: &ir::Module,
    files: &Files,
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
    let mut taken = HashSet::new();
    let mut disambiguated = HashMap::new();
    for global in &module.globals {
        let name = rust_binding_name(&global.variable.name);
        if matches!(global.linkage, ir::Linkage::Internal) && owners[&name].len() > 1 {
            let line = source_line(files, global.expansion);
            let name = format!("{}__{line}", name.trim_start_matches("r#"));
            disambiguated.insert(global.variable.id, unique_name(name, &mut taken));
        }
    }
    module
        .globals
        .iter()
        .map(|global| {
            let target = alias_target(module, global).unwrap_or(global);
            let name = disambiguated
                .get(&target.variable.id)
                .cloned()
                .unwrap_or_else(|| rust_binding_name(&target.variable.name));
            (global.variable.id, name)
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
    next: &mut u32,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::Let(variable) => {
                bindings.insert(
                    variable.id,
                    local_binding_name(&variable.name, next, reserved),
                );
            }
            ir::Statement::Block(body)
            | ir::Statement::While { body, .. }
            | ir::Statement::DoWhile { body, .. }
            | ir::Statement::Switch { body, .. }
            | ir::Statement::Label { body, .. }
            | ir::Statement::Case { body, .. }
            | ir::Statement::Default { body, .. } => {
                collect_statement_names(body, bindings, reserved, next)
            }
            ir::Statement::OmpParallel(region) => {
                for private in &region.private {
                    bindings.insert(
                        private.copy.id,
                        local_binding_name(&private.copy.name, next, reserved),
                    );
                }
                collect_statement_names(&region.body, bindings, reserved, next);
            }
            ir::Statement::For { init, body, .. } => {
                collect_statement_names(init, bindings, reserved, next);
                collect_statement_names(body, bindings, reserved, next);
            }
            ir::Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_statement_names(then_body, bindings, reserved, next);
                if let Some(else_body) = else_body {
                    collect_statement_names(else_body, bindings, reserved, next);
                }
            }
            _ => {}
        }
    }
}

fn file_ident(files: &Files, loc: slate_parser::ast::Loc) -> String {
    files
        .path(loc.file)
        .file_name()
        .map(|name| name.to_string_lossy())
        .unwrap_or_default()
        .chars()
        .map(|c| if c.is_ascii_alphanumeric() { c } else { '_' })
        .collect()
}

pub(super) fn record_names(module: &ir::Module, files: &Files) -> HashMap<TypeId, String> {
    let mut counts = HashMap::<&str, usize>::new();
    for definition in &module.types {
        if let (Some(name), ir::TypeDefinitionKind::Record { .. }) =
            (&definition.name, &definition.kind)
        {
            *counts.entry(name).or_default() += 1;
        }
    }
    let mut taken = HashSet::new();
    module
        .types
        .iter()
        .filter(|definition| matches!(definition.kind, ir::TypeDefinitionKind::Record { .. }))
        .map(|definition| {
            let id = definition.value.id;
            let loc = definition.expansion;
            let line = source_line(files, loc);
            let name = match definition.name.as_deref() {
                Some(name) if counts[name] == 1 => name.to_owned(),
                Some(name) => unique_name(format!("{name}__{line}"), &mut taken),
                None => unique_name(
                    format!("__SlateRecord_{}_{line}", file_ident(files, loc)),
                    &mut taken,
                ),
            };
            (id, name)
        })
        .collect()
}
