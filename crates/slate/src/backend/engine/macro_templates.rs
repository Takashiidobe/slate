use std::collections::{BTreeMap, HashMap, HashSet};

use super::rules::walk::{child_exprs, child_exprs_mut, stmt_exprs_mut};
use super::shadow::visit_names;
use crate::backend::codegen::is_rust_keyword;
use crate::backend::rust_ast::{
    Block, Expr, Ident, ImplItem, Item, Label, MacroParam, Program, Stmt, Type, UnaryOp,
    macro_template_base,
};
use crate::function_identity::CallBinding;

const STD_MACROS: &[&str] = &[
    "assert",
    "assert_eq",
    "assert_ne",
    "cfg",
    "column",
    "compile_error",
    "concat",
    "dbg",
    "debug_assert",
    "debug_assert_eq",
    "debug_assert_ne",
    "env",
    "eprint",
    "eprintln",
    "file",
    "format",
    "format_args",
    "include",
    "include_bytes",
    "include_str",
    "line",
    "macro_rules",
    "matches",
    "module_path",
    "option_env",
    "panic",
    "print",
    "println",
    "stringify",
    "todo",
    "unimplemented",
    "unreachable",
    "vec",
    "write",
    "writeln",
];

pub(crate) fn is_valid_macro_name(name: &str) -> bool {
    !is_rust_keyword(name) && !STD_MACROS.contains(&name)
}

pub fn child_exprs_of(expr: &Expr) -> Vec<&Expr> {
    child_exprs(expr)
}

pub fn hole(parameter: &str) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(format!("${parameter}").into())),
        args: Vec::new(),
        binding: CallBinding::Generated,
    }
}

pub fn place_hole(parameter: &str) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Call {
            func: Box::new(Expr::Var(format!("$*{parameter}").into())),
            args: Vec::new(),
            binding: CallBinding::Generated,
        }),
    }
}

pub fn call_argument_parameter(parameter: &str, element: usize) -> String {
    format!("{parameter}__{element}")
}

pub fn mentions_hole(template: &mut [Stmt], parameter: &str) -> bool {
    let mut found = false;
    visit_exprs_mut(template, &mut |expr| {
        found |= hole_parameter(expr) == Some(parameter);
    });
    found
}

pub fn make_field_parameter(template: &mut [Stmt], parameter: &str, field_name: &str) -> bool {
    let mut found = false;
    let hole = format!("${parameter}");
    visit_exprs_mut(template, &mut |expr| {
        if let Expr::Field { field: name, .. } | Expr::MacroName(name) = expr
            && name == field_name
        {
            name.clone_from(&hole);
            found = true;
        }
    });
    found
}

pub fn replace_type(ty: &mut Type, concrete: &Type, parameter: &str) -> bool {
    if ty == concrete {
        *ty = Type::TyVar(Ident::new(format!("${parameter}")));
        return true;
    }
    match ty {
        Type::Ptr { inner, .. } | Type::Ref { inner, .. } | Type::Slice(inner) => {
            replace_type(inner, concrete, parameter)
        }
        Type::Array { elem, .. } => replace_type(elem, concrete, parameter),
        _ => false,
    }
}

fn each_template_type(stmts: &mut [Stmt], visit: &mut impl FnMut(&mut Type)) {
    for stmt in stmts.iter_mut() {
        if let Stmt::Let { ty: Some(ty), .. } = stmt {
            visit(ty);
        }
        for body in stmt.child_bodies_mut() {
            each_template_type(body, visit);
        }
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            visit_expr_mut(expr, &mut |expr| match expr {
                Expr::Cast { expr: operand, .. } if matches!(**operand, Expr::Value(_)) => {}
                Expr::Cast { ty, .. } | Expr::MacroType(ty) => visit(ty),
                Expr::Block(block) | Expr::Unsafe(block) => {
                    each_template_type(&mut block.stmts, visit);
                }
                _ => {}
            });
        }
    }
}

pub fn make_type_parameter(template: &mut [Stmt], parameter: &str, concrete: &Type) -> bool {
    let mut found = false;
    each_template_type(template, &mut |ty| {
        found |= replace_type(ty, concrete, parameter);
    });
    found
}

fn fill_type(ty: &mut Type, params: &[MacroParam], args: &[Expr]) {
    match ty {
        Type::TyVar(name) => {
            let Some(parameter) = name.as_str().strip_prefix('$') else {
                return;
            };
            let value = params
                .iter()
                .position(|param| param.name == parameter)
                .and_then(|index| args.get(index));
            if let Some(Expr::MacroType(concrete)) = value {
                *ty = concrete.clone();
            }
        }
        Type::Ptr { inner, .. } | Type::Ref { inner, .. } | Type::Slice(inner) => {
            fill_type(inner, params, args);
        }
        Type::Array { elem, .. } => fill_type(elem, params, args),
        _ => {}
    }
}

fn fill_field(field: &mut String, params: &[MacroParam], args: &[Expr]) {
    let Some(parameter) = field.strip_prefix('$') else {
        return;
    };
    let value = params
        .iter()
        .position(|param| param.name == parameter)
        .and_then(|index| args.get(index));
    if let Some(Expr::MacroName(name)) = value {
        field.clone_from(name);
    }
}

fn any_hole_name(expr: &Expr) -> Option<&str> {
    let Expr::Call { func, args, .. } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    if args.is_empty() {
        name.as_str().strip_prefix('$')
    } else {
        None
    }
}

fn hole_parameter(expr: &Expr) -> Option<&str> {
    any_hole_name(expr).filter(|name| !name.starts_with('*'))
}

fn place_pointer_parameter(expr: &Expr) -> Option<&str> {
    any_hole_name(expr)?.strip_prefix('*')
}

fn place_hole_parameter(expr: &Expr) -> Option<&str> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    place_pointer_parameter(expr)
}

fn place_of_argument(argument: &Expr) -> Expr {
    match argument {
        Expr::AddrOf { expr, .. } | Expr::Ref { expr, .. } => (**expr).clone(),
        other => Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(other.clone()),
        },
    }
}

pub fn make_place_parameter(template: &mut [Stmt], parameter: &str, read_is_unsafe: bool) {
    let read = if read_is_unsafe {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(place_hole(parameter))),
        }))
    } else {
        place_hole(parameter)
    };
    visit_exprs_mut(template, &mut |expr| {
        if hole_parameter(expr) == Some(parameter) {
            *expr = read.clone();
        }
    });
}

fn mutated_parameter(place: &Expr) -> Option<&str> {
    match place {
        Expr::Field { base, .. } | Expr::TupleField { base, .. } | Expr::Index { base, .. } => {
            mutated_parameter(base)
        }
        other => place_hole_parameter(other),
    }
}

fn reads_place_parameter(expr: &Expr, parameter: &str) -> bool {
    place_pointer_parameter(expr) == Some(parameter)
        || child_exprs(expr)
            .into_iter()
            .any(|child| reads_place_parameter(child, parameter))
}

fn expr_mutates_parameter(expr: &Expr, parameter: &str) -> bool {
    let here = match expr {
        Expr::AddrOf {
            mutable: true,
            expr,
        }
        | Expr::Ref {
            mutable: true,
            expr,
        }
        | Expr::ArrayPtr {
            mutable: true,
            array: expr,
        } => mutated_parameter(expr) == Some(parameter),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            mutated_parameter(recv) == Some(parameter)
        }
        Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. }
        | Expr::AtomicStore { .. }
        | Expr::AtomicFetch { .. }
        | Expr::AtomicSwap { .. }
        | Expr::AtomicCompareExchange { .. } => reads_place_parameter(expr, parameter),
        Expr::Block(block) | Expr::Unsafe(block) => stmts_mutate_parameter(&block.stmts, parameter),
        _ => false,
    };
    here || child_exprs(expr)
        .into_iter()
        .any(|child| expr_mutates_parameter(child, parameter))
}

fn stmts_mutate_parameter(stmts: &[Stmt], parameter: &str) -> bool {
    stmts.iter().any(|stmt| {
        if let Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } = stmt
            && mutated_parameter(target) == Some(parameter)
        {
            return true;
        }
        let mut stmt = stmt.clone();
        let mut exprs = Vec::new();
        stmt_exprs_mut(&mut stmt, &mut exprs);
        exprs
            .into_iter()
            .any(|expr| expr_mutates_parameter(expr, parameter))
            || stmt
                .child_bodies_mut()
                .into_iter()
                .any(|body| stmts_mutate_parameter(body, parameter))
    })
}

pub fn parameter_is_written(template: &[Stmt], parameter: &str) -> bool {
    stmts_mutate_parameter(template, parameter)
}

fn visit_exprs_mut(stmts: &mut [Stmt], visit: &mut impl FnMut(&mut Expr)) {
    for stmt in stmts {
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            visit_expr_mut(expr, visit);
        }
    }
}

fn visit_expr_mut(expr: &mut Expr, visit: &mut impl FnMut(&mut Expr)) {
    visit(expr);
    for child in child_exprs_mut(expr) {
        visit_expr_mut(child, visit);
    }
}

pub fn fill_holes(template: &mut [Stmt], params: &[MacroParam], args: &[Expr]) {
    let argument = |parameter: &str| {
        let index = params.iter().position(|param| param.name == parameter)?;
        args.get(index)
    };
    each_template_type(template, &mut |ty| fill_type(ty, params, args));
    visit_exprs_mut(template, &mut |expr| {
        match expr {
            Expr::Field { field, .. } | Expr::MacroName(field) => fill_field(field, params, args),
            _ => {}
        }
        let value = if let Some(parameter) = place_hole_parameter(expr) {
            argument(parameter).map(place_of_argument)
        } else if let Some(parameter) = place_pointer_parameter(expr) {
            argument(parameter).cloned()
        } else {
            hole_parameter(expr).and_then(argument).cloned()
        };
        if let Some(value) = value {
            *expr = value;
        }
    });
}

pub fn collapse_address_of_deref(stmts: &mut [Stmt]) {
    visit_exprs_mut(stmts, &mut |expr| {
        if let Expr::AddrOf { expr: inner, .. } = expr
            && let Expr::Unary {
                op: UnaryOp::Deref,
                expr: pointer,
            } = &**inner
        {
            *expr = (**pointer).clone();
        }
    });
}

pub fn expand_template_calls(stmts: &mut Vec<Stmt>, templates: &BTreeMap<String, Item>) {
    let mut out = Vec::with_capacity(stmts.len());
    for mut stmt in std::mem::take(stmts) {
        if let Stmt::Expr(Expr::Macro { name, args }) = &stmt
            && let Some(Item::MacroRules { params, body, .. }) = templates.get(name)
            && !matches!(body.last(), Some(Stmt::Return(Some(_))))
        {
            let mut body = body.clone();
            fill_holes(&mut body, params, args);
            expand_template_calls(&mut body, templates);
            out.extend(body);
            continue;
        }
        for body in stmt.child_bodies_mut() {
            expand_template_calls(body, templates);
        }
        let mut exprs = Vec::new();
        stmt_exprs_mut(&mut stmt, &mut exprs);
        for expr in exprs {
            visit_expr_mut(expr, &mut |expr| expand_value_call(expr, templates));
        }
        out.push(stmt);
    }
    *stmts = out;
}

fn expand_value_call(expr: &mut Expr, templates: &BTreeMap<String, Item>) {
    let Expr::Macro { name, args } = expr else {
        return;
    };
    let Some(Item::MacroRules { params, body, .. }) = templates.get(name) else {
        return;
    };
    let mut body = body.clone();
    fill_holes(&mut body, params, args);
    expand_template_calls(&mut body, templates);
    let Some(Stmt::Return(Some(value))) = body.pop() else {
        return;
    };
    *expr = if body.is_empty() {
        value
    } else {
        Expr::Block(Box::new(Block {
            stmts: body,
            tail: Some(Box::new(value)),
        }))
    };
}

fn template_local_base(name: &str) -> (String, bool) {
    let trimmed = name.trim_end_matches(|c: char| c.is_ascii_digit());
    if trimmed.len() == name.len() {
        return (name.to_string(), false);
    }
    if trimmed.ends_with("__") && trimmed.len() > 2 {
        return (trimmed.to_string(), true);
    }
    if name.starts_with("__") {
        return (trimmed.to_string(), false);
    }
    (name.to_string(), false)
}

fn each_label(stmts: &mut [Stmt], visit: &mut impl FnMut(&mut Label)) {
    for stmt in stmts {
        match stmt {
            Stmt::Loop {
                label: Some(label), ..
            }
            | Stmt::For {
                label: Some(label), ..
            }
            | Stmt::While {
                label: Some(label), ..
            }
            | Stmt::Break(Some(label))
            | Stmt::Continue(Some(label))
            | Stmt::LabeledBlock { label, .. } => visit(label),
            _ => {}
        }
        for body in stmt.child_bodies_mut() {
            each_label(body, visit);
        }
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            visit_expr_mut(expr, &mut |expr| {
                if let Expr::Block(block) | Expr::Unsafe(block) = expr {
                    each_label(&mut block.stmts, visit);
                }
            });
        }
    }
}

pub fn erase_binding_names(stmts: &mut [Stmt]) {
    let mut current = HashMap::new();
    let mut next = 0;
    for stmt in stmts.iter_mut() {
        visit_names(stmt, |name, binds| {
            if binds {
                next += 1;
                current.insert(*name, Ident::new(format!("__local_{next}")));
            }
            if let Some(fresh) = current.get(name) {
                *name = *fresh;
            }
        });
    }
    let erased = Ident::new("__label");
    each_label(stmts, &mut |label| *label.ident_mut() = erased);
}

pub fn normalize_locals(template: &mut [Stmt]) {
    let mut bound = Vec::new();
    let mut known = HashSet::new();
    for stmt in template.iter_mut() {
        visit_names(stmt, |name, binds| {
            if binds && known.insert(*name) {
                bound.push(*name);
            }
        });
    }
    let mut taken: HashSet<Ident> = HashSet::new();
    for stmt in template.iter_mut() {
        visit_names(stmt, |name, _| {
            if !known.contains(name) {
                taken.insert(*name);
            }
        });
    }
    let mut renamed = HashMap::new();
    for name in bound {
        let (base, needs_suffix) = template_local_base(name.as_str());
        let mut suffix = 1;
        let mut fresh = if needs_suffix {
            Ident::new(format!("{base}{suffix}"))
        } else {
            Ident::new(base.clone())
        };
        while !taken.insert(fresh) {
            suffix += 1;
            fresh = Ident::new(format!("{base}{suffix}"));
        }
        renamed.insert(name, fresh);
    }
    for stmt in template.iter_mut() {
        visit_names(stmt, |name, _| {
            if let Some(fresh) = renamed.get(name) {
                *name = *fresh;
            }
        });
    }
    let mut labels: HashMap<String, Ident> = HashMap::new();
    each_label(template, &mut |label| {
        let next = labels.len();
        let fresh = *labels
            .entry(label.as_str().to_string())
            .or_insert_with(|| Ident::new(format!("__slate_label_{next}")));
        *label.ident_mut() = fresh;
    });
}

fn place_parameter_indices(params: &[MacroParam], body: &mut [Stmt]) -> Vec<usize> {
    let mut names = HashSet::new();
    visit_exprs_mut(body, &mut |expr| {
        if let Some(parameter) = place_pointer_parameter(expr) {
            names.insert(parameter.to_string());
        }
    });
    params
        .iter()
        .enumerate()
        .filter(|(_, param)| names.contains(&param.name))
        .map(|(index, _)| index)
        .collect()
}

fn finish_expr(expr: &mut Expr) {
    let finished = if let Some(parameter) = place_hole_parameter(expr) {
        Some(Expr::Var(format!("${parameter}").into()))
    } else if let Some(parameter) = place_pointer_parameter(expr) {
        Some(Expr::AddrOf {
            mutable: true,
            expr: Box::new(Expr::Var(format!("${parameter}").into())),
        })
    } else {
        hole_parameter(expr).map(|parameter| Expr::Var(format!("${parameter}").into()))
    };
    if let Some(finished) = finished {
        *expr = finished;
    }
}

pub(super) type PlaceParameters = HashMap<String, Vec<usize>>;

pub(super) fn finish_holes(program: &mut Program) -> PlaceParameters {
    let mut places = HashMap::new();
    for item in &mut program.items {
        if let Item::MacroRules { name, params, body } = item {
            places.insert(name.clone(), place_parameter_indices(params, body));
            visit_exprs_mut(body, &mut finish_expr);
        }
    }
    places
}

pub(super) fn finish_place_arguments(program: &mut Program, places: &PlaceParameters) {
    each_body(&mut program.items, &mut |body| {
        visit_exprs_mut(body, &mut |expr| {
            if let Expr::Macro { name, args } = expr
                && let Some(indices) = places.get(name)
            {
                for &index in indices {
                    if let Some(argument) = args.get_mut(index) {
                        *argument = place_of_argument(argument);
                    }
                }
            }
        });
    });
}

fn each_body(items: &mut [Item], visit: &mut impl FnMut(&mut [Stmt])) {
    for item in items {
        match item {
            Item::Fn(func) => visit(&mut func.body),
            Item::MacroRules { body, .. } => visit(body),
            Item::Cfg { item, .. } => each_body(std::slice::from_mut(item), visit),
            Item::InlineMod { items, .. } => each_body(items, visit),
            Item::Impl(impl_block) => {
                for impl_item in &mut impl_block.items {
                    if let ImplItem::Method(method) = impl_item
                        && let Expr::Block(block) = &mut method.body
                    {
                        visit(&mut block.stmts);
                    }
                }
            }
            _ => {}
        }
    }
}

fn each_rules_name(items: &mut [Item], visit: &mut impl FnMut(&mut String)) {
    for item in items {
        match item {
            Item::MacroRules { name, .. } => visit(name),
            Item::Cfg { item, .. } => each_rules_name(std::slice::from_mut(item), visit),
            Item::InlineMod { items, .. } => each_rules_name(items, visit),
            _ => {}
        }
    }
}

fn each_call_name(stmts: &mut [Stmt], visit: &mut impl FnMut(&mut String)) {
    visit_exprs_mut(stmts, &mut |expr| {
        if let Expr::Macro { name, .. } = expr {
            visit(name);
        }
    });
}

pub fn assign_template_names<'a>(programs: impl Iterator<Item = &'a mut Program>) {
    let mut programs: Vec<&mut Program> = programs.collect();
    let mut uses: BTreeMap<String, usize> = BTreeMap::new();
    for program in &mut programs {
        each_rules_name(&mut program.items, &mut |name| {
            uses.entry(name.clone()).or_default();
        });
        each_body(&mut program.items, &mut |body| {
            each_call_name(body, &mut |name| {
                if macro_template_base(name).is_some() {
                    *uses.entry(name.clone()).or_default() += 1;
                }
            });
        });
    }
    let mut by_base: BTreeMap<String, Vec<(usize, String)>> = BTreeMap::new();
    for (name, count) in uses {
        let base = macro_template_base(&name).unwrap_or_default().to_string();
        by_base.entry(base).or_default().push((count, name));
    }
    let mut renamed: HashMap<String, String> = HashMap::new();
    for (base, mut variants) in by_base {
        variants.sort_by(|a, b| b.0.cmp(&a.0).then_with(|| a.1.cmp(&b.1)));
        for (index, (_, name)) in variants.into_iter().enumerate() {
            let fresh = match index {
                0 => base.clone(),
                _ => format!("{base}__{}", index + 1),
            };
            renamed.insert(name, fresh);
        }
    }
    let mut apply = |name: &mut String| {
        if let Some(fresh) = renamed.get(name) {
            name.clone_from(fresh);
        }
    };
    for program in &mut programs {
        each_rules_name(&mut program.items, &mut apply);
        each_body(&mut program.items, &mut |body| {
            each_call_name(body, &mut apply);
        });
    }
}
