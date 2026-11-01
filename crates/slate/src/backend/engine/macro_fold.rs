use std::collections::{HashMap, HashSet};

use super::rules::walk::{child_exprs_mut, stmt_exprs_mut};
use super::shadow::visit_names;
use crate::backend::codegen::is_rust_keyword;
use crate::backend::format::MACRO_LIST_HOLE;
use crate::backend::rust_ast::{
    CommentAttach, Expr, Ident, ImplItem, Item, MacroParam, MacroParamShape, MacroRegion, Program,
    Stmt,
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

#[derive(Clone, PartialEq, Eq, Hash)]
struct Template {
    params: Vec<MacroParam>,
    body: Vec<Stmt>,
}

struct Candidate {
    template: Template,
    args: Vec<Expr>,
    before: Vec<Stmt>,
    after: Vec<Stmt>,
}

struct Holes<'a> {
    params: &'a [MacroParam],
    bound: HashSet<Ident>,
    single: Vec<Option<Expr>>,
    lists: Vec<Option<Vec<Expr>>>,
    failed: bool,
}

impl Holes<'_> {
    fn is_macro_local(&self, expr: &Expr) -> bool {
        matches!(expr, Expr::Var(name) if self.bound.contains(name))
    }

    fn shape(&self, index: u32) -> Option<MacroParamShape> {
        self.params.get(index as usize).map(|param| param.shape)
    }

    fn erase_list(&mut self, args: &mut Vec<Expr>) {
        let mut erased = Vec::with_capacity(args.len());
        let mut remaining = std::mem::take(args).into_iter().peekable();
        while let Some(arg) = remaining.next() {
            let index = match &arg {
                Expr::MacroArg { expr, .. } if self.is_macro_local(expr) => {
                    erased.push(arg);
                    continue;
                }
                Expr::MacroArg { index, .. }
                    if self.shape(*index) == Some(MacroParamShape::List) =>
                {
                    *index
                }
                _ => {
                    erased.push(arg);
                    continue;
                }
            };
            let mut run = vec![arg];
            while remaining.peek().is_some_and(
                |next| matches!(next, Expr::MacroArg { index: at, .. } if *at == index),
            ) {
                run.push(remaining.next().expect("peeked"));
            }
            let values: Vec<Expr> = run
                .into_iter()
                .map(|arg| match arg {
                    Expr::MacroArg { expr, .. } => *expr,
                    other => other,
                })
                .collect();
            let slot = &mut self.lists[index as usize];
            if slot.as_ref().is_some_and(|known| *known != values) {
                self.failed = true;
            }
            *slot = Some(values);
            let name = &self.params[index as usize].name;
            erased.push(Expr::Var(format!("{MACRO_LIST_HOLE}{name}").into()));
        }
        *args = erased;
    }

    fn erase(&mut self, expr: &mut Expr) {
        if let Expr::Call { binding, .. } = expr {
            *binding = CallBinding::Generated;
        }
        if let Expr::Call { args, .. } | Expr::Macro { args, .. } = expr {
            self.erase_list(args);
        }
        if let Expr::MacroArg { expr: inner, .. } = expr
            && self.is_macro_local(inner)
        {
            let local = std::mem::replace(&mut **inner, Expr::Str(String::new()));
            *expr = local;
            return;
        }
        if let Expr::MacroArg { index, expr: inner } = expr {
            let index = *index as usize;
            if self.shape(index as u32) != Some(MacroParamShape::Expr) {
                self.failed = true;
                return;
            }
            let value = std::mem::replace(&mut **inner, Expr::Str(String::new()));
            let slot = &mut self.single[index];
            if slot.as_ref().is_some_and(|known| *known != value) {
                self.failed = true;
            }
            *slot = Some(value);
            *expr = Expr::Var(format!("${}", self.params[index].name).into());
            return;
        }
        for child in child_exprs_mut(expr) {
            self.erase(child);
        }
    }

    fn call_args(self) -> Option<(Vec<MacroParam>, Vec<Expr>)> {
        if self.failed {
            return None;
        }
        let list_position = self
            .params
            .iter()
            .filter(|param| param.shape != MacroParamShape::Unused)
            .position(|param| param.shape == MacroParamShape::List);
        let used = self
            .params
            .iter()
            .filter(|param| param.shape != MacroParamShape::Unused)
            .count();
        if list_position.is_some_and(|position| position + 1 != used) {
            return None;
        }
        let mut params = Vec::with_capacity(used);
        let mut args = Vec::new();
        for ((param, single), list) in self.params.iter().zip(self.single).zip(self.lists) {
            match param.shape {
                MacroParamShape::Unused => continue,
                MacroParamShape::Expr => args.push(single?),
                MacroParamShape::List => args.extend(list?),
            }
            params.push(param.clone());
        }
        Some((params, args))
    }
}

fn is_generated(name: &str) -> Option<&str> {
    let prefix = name.trim_end_matches(|c: char| c.is_ascii_digit());
    (name.starts_with("__") && prefix.len() < name.len()).then_some(prefix)
}

fn bound_names(stmts: &mut [Stmt]) -> HashSet<Ident> {
    let mut bound = HashSet::new();
    for stmt in stmts {
        visit_names(stmt, |name, binds| {
            if binds {
                bound.insert(*name);
            }
        });
    }
    bound
}

fn reads_any(stmts: &mut [Stmt], names: &HashSet<Ident>) -> bool {
    let mut found = false;
    for stmt in stmts {
        visit_names(stmt, |name, _| found |= names.contains(name));
    }
    found
}

fn normalize_generated(body: &mut [Stmt], bound: &HashSet<Ident>) {
    let mut renamed: HashMap<Ident, Ident> = HashMap::new();
    for stmt in body {
        visit_names(stmt, |name, _| {
            if !bound.contains(name) {
                return;
            }
            let Some(prefix) = is_generated(name.as_str()) else {
                return;
            };
            let next = renamed.len();
            *name = *renamed
                .entry(*name)
                .or_insert_with(|| Ident::new(format!("{prefix}{next}")));
        });
    }
}

fn is_valid_macro_name(name: &str) -> bool {
    !is_rust_keyword(name) && !STD_MACROS.contains(&name)
}

fn candidate(region: &MacroRegion, body: &[Stmt], locals: &HashSet<Ident>) -> Option<Candidate> {
    if !region.foldable || !is_valid_macro_name(&region.name) {
        return None;
    }
    let mut before = Vec::new();
    let mut after = Vec::new();
    let mut template = Vec::with_capacity(body.len());
    for stmt in body {
        match stmt {
            Stmt::Comment(comment)
                if template.is_empty() && comment.attach != CommentAttach::Trailing =>
            {
                before.push(stmt.clone())
            }
            Stmt::Comment(_) => after.push(stmt.clone()),
            _ => template.push(stmt.clone()),
        }
    }
    if template.is_empty() {
        return None;
    }
    let mut holes = Holes {
        params: &region.params,
        bound: bound_names(&mut template),
        single: vec![None; region.params.len()],
        lists: vec![None; region.params.len()],
        failed: false,
    };
    for stmt in &mut template {
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            holes.erase(expr);
        }
    }
    let (params, args) = holes.call_args()?;
    let bound = bound_names(&mut template);
    let mut arg_stmts: Vec<Stmt> = args.iter().cloned().map(Stmt::Expr).collect();
    if reads_any(&mut arg_stmts, &bound) {
        return None;
    }
    let captured: HashSet<Ident> = locals.difference(&bound).copied().collect();
    if reads_any(&mut template, &captured) {
        return None;
    }
    normalize_generated(&mut template, &bound);
    Some(Candidate {
        template: Template {
            params,
            body: template,
        },
        args,
        before,
        after,
    })
}

fn visit_regions(
    stmts: &mut Vec<Stmt>,
    visit: &mut impl FnMut(&MacroRegion, &[Stmt]) -> Option<Vec<Stmt>>,
) {
    let mut out = Vec::with_capacity(stmts.len());
    for mut stmt in std::mem::take(stmts) {
        if let Stmt::Scope {
            body,
            macro_region: Some(region),
        } = &stmt
            && let Some(replacement) = visit(region, body)
        {
            out.extend(replacement);
            continue;
        }
        for body in stmt.child_bodies_mut() {
            visit_regions(body, visit);
        }
        out.push(stmt);
    }
    *stmts = out;
}

fn for_each_body(items: &mut [Item], visit: &mut impl FnMut(&mut Vec<Stmt>, HashSet<Ident>)) {
    for item in items {
        match item {
            Item::Fn(func) => {
                let mut locals = bound_names(&mut func.body);
                locals.extend(
                    func.params
                        .iter()
                        .map(|param| Ident::new(param.name.as_str())),
                );
                visit(&mut func.body, locals);
            }
            Item::InlineMod { items, .. } => for_each_body(items, visit),
            Item::Impl(impl_block) => {
                for impl_item in &mut impl_block.items {
                    if let ImplItem::Method(method) = impl_item
                        && let Expr::Block(block) = &mut method.body
                    {
                        let locals = bound_names(&mut block.stmts);
                        visit(&mut block.stmts, locals);
                    }
                }
            }
            _ => {}
        }
    }
}

pub(super) fn fold(program: &mut Program) {
    let mut seen: Vec<(String, Template, usize)> = Vec::new();
    for_each_body(&mut program.items, &mut |body, locals| {
        visit_regions(body, &mut |region, stmts| {
            if let Some(found) = candidate(region, stmts, &locals) {
                match seen
                    .iter_mut()
                    .find(|(name, template, _)| *name == region.name && *template == found.template)
                {
                    Some((.., count)) => *count += 1,
                    None => seen.push((region.name.clone(), found.template, 1)),
                }
            }
            None
        });
    });
    let mut chosen: Vec<(String, Template, usize)> = Vec::new();
    for (name, template, count) in seen {
        match chosen.iter_mut().find(|(known, ..)| *known == name) {
            Some(best) if best.2 >= count => {}
            Some(best) => *best = (name, template, count),
            None => chosen.push((name, template, count)),
        }
    }
    if chosen.is_empty() {
        return;
    }
    for_each_body(&mut program.items, &mut |body, locals| {
        visit_regions(body, &mut |region, stmts| {
            let found = candidate(region, stmts, &locals)?;
            chosen
                .iter()
                .any(|(name, template, _)| *name == region.name && *template == found.template)
                .then(|| {
                    let mut out = found.before;
                    out.push(Stmt::Expr(Expr::Macro {
                        name: region.name.clone(),
                        args: found.args,
                    }));
                    out.extend(found.after);
                    out
                })
        });
    });
    let at = program
        .items
        .iter()
        .position(|item| !matches!(item, Item::CrateAttrs(_) | Item::Comment(_)))
        .unwrap_or(program.items.len());
    program.items.splice(
        at..at,
        chosen
            .into_iter()
            .map(|(name, template, _)| Item::MacroRules {
                name,
                params: template.params,
                body: template.body,
            }),
    );
}
