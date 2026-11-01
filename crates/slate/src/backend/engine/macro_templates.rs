use std::collections::{BTreeMap, HashMap, HashSet};

use super::rules::walk::{child_exprs, child_exprs_mut, stmt_exprs_mut};
use super::shadow::visit_names;
use crate::backend::rust_ast::{
    Expr, Ident, ImplItem, Item, Label, MacroParam, Program, Stmt, macro_template_base,
};
use crate::function_identity::CallBinding;

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

fn hole_parameter(expr: &Expr) -> Option<&str> {
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
    visit_exprs_mut(template, &mut |expr| {
        let value = hole_parameter(expr).and_then(|parameter| {
            let index = params.iter().position(|param| param.name == parameter)?;
            args.get(index).cloned()
        });
        if let Some(value) = value {
            *expr = value;
        }
    });
}

pub fn expand_template_calls(stmts: &mut Vec<Stmt>, templates: &BTreeMap<String, Item>) {
    let mut out = Vec::with_capacity(stmts.len());
    for mut stmt in std::mem::take(stmts) {
        if let Stmt::Expr(Expr::Macro { name, args }) = &stmt
            && let Some(Item::MacroRules { params, body, .. }) = templates.get(name)
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
        out.push(stmt);
    }
    *stmts = out;
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

pub(super) fn finish_holes(program: &mut Program) {
    for item in &mut program.items {
        if let Item::MacroRules { body, .. } = item {
            visit_exprs_mut(body, &mut |expr| {
                if let Some(parameter) = hole_parameter(expr) {
                    *expr = Expr::Var(format!("${parameter}").into());
                }
            });
        }
    }
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
    let mut plain = HashSet::new();
    let mut uses: BTreeMap<String, usize> = BTreeMap::new();
    for program in &mut programs {
        each_rules_name(
            &mut program.items,
            &mut |name| match macro_template_base(name) {
                Some(_) => {
                    uses.entry(name.clone()).or_default();
                }
                None => {
                    plain.insert(name.clone());
                }
            },
        );
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
        let mut suffix = 1;
        for (_, name) in variants {
            let mut fresh = if suffix == 1 {
                base.clone()
            } else {
                format!("{base}__{suffix}")
            };
            while plain.contains(&fresh) {
                suffix += 1;
                fresh = format!("{base}__{suffix}");
            }
            suffix += 1;
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
