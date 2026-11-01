use std::collections::{HashMap, HashSet};

use super::rules::walk::{child_exprs, child_exprs_mut, stmt_exprs, stmt_exprs_mut};
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

    fn gather_argument_uses(expr: &mut Expr, uses: &mut [Vec<Expr>]) {
        if let Some((index, value)) = cast_argument(expr) {
            uses[index as usize].push(value);
            return;
        }
        for child in child_exprs_mut(expr) {
            Self::gather_argument_uses(child, uses);
        }
    }

    fn lift_casts_where_uses_agree(&self, template: &mut [Stmt]) {
        let mut uses = vec![Vec::new(); self.params.len()];
        for stmt in template.iter_mut() {
            let mut exprs = Vec::new();
            stmt_exprs_mut(stmt, &mut exprs);
            for expr in exprs {
                Self::gather_argument_uses(expr, &mut uses);
            }
        }
        let liftable: Vec<bool> = uses
            .iter()
            .enumerate()
            .map(|(index, uses)| {
                self.shape(index as u32) == Some(MacroParamShape::Expr)
                    && uses.windows(2).all(|pair| pair[0] == pair[1])
                    && !uses.iter().any(|value| self.is_macro_local(uncast(value)))
            })
            .collect();
        for stmt in template.iter_mut() {
            let mut exprs = Vec::new();
            stmt_exprs_mut(stmt, &mut exprs);
            for expr in exprs {
                lift_casts(expr, &liftable);
            }
        }
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

fn cast_argument(expr: &Expr) -> Option<(u32, Expr)> {
    match expr {
        Expr::MacroArg { index, expr } => Some((*index, (**expr).clone())),
        Expr::Cast { expr, ty } => cast_argument(expr).map(|(index, value)| {
            (
                index,
                Expr::Cast {
                    expr: Box::new(value),
                    ty: ty.clone(),
                },
            )
        }),
        _ => None,
    }
}

fn uncast(mut expr: &Expr) -> &Expr {
    while let Expr::Cast { expr: inner, .. } = expr {
        expr = inner;
    }
    expr
}

fn lift_casts(expr: &mut Expr, liftable: &[bool]) {
    for child in child_exprs_mut(expr) {
        lift_casts(child, liftable);
    }
    let Expr::Cast { expr: inner, .. } = expr else {
        return;
    };
    let index = match **inner {
        Expr::MacroArg { index, .. } if liftable[index as usize] => index,
        _ => return,
    };
    let Expr::MacroArg { expr: value, .. } =
        std::mem::replace(&mut **inner, Expr::Str(String::new()))
    else {
        return;
    };
    **inner = *value;
    let cast = std::mem::replace(expr, Expr::Str(String::new()));
    *expr = Expr::MacroArg {
        index,
        expr: Box::new(cast),
    };
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
    holes.lift_casts_where_uses_agree(&mut template);
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

fn binders(stmts: &mut [Stmt]) -> Vec<Ident> {
    let mut names = Vec::new();
    for stmt in stmts {
        visit_names(stmt, |name, binds| {
            if binds && !names.contains(name) {
                names.push(*name);
            }
        });
    }
    names
}

fn hole_index(expr: &Expr, params: &[MacroParam]) -> Option<usize> {
    let Expr::Var(name) = expr else {
        return None;
    };
    let name = name.as_str().strip_prefix('$')?;
    params.iter().position(|param| param.name == name)
}

fn bind_holes(pattern: &Expr, found: &Expr, params: &[MacroParam], slots: &mut [Option<Expr>]) {
    if let Some(index) = hole_index(pattern, params) {
        slots[index].get_or_insert_with(|| found.clone());
        return;
    }
    for (pattern, found) in child_exprs(pattern).into_iter().zip(child_exprs(found)) {
        bind_holes(pattern, found, params, slots);
    }
}

fn fill_holes(expr: &mut Expr, params: &[MacroParam], args: &[Expr]) {
    if let Some(index) = hole_index(expr, params) {
        *expr = args[index].clone();
        return;
    }
    for child in child_exprs_mut(expr) {
        fill_holes(child, params, args);
    }
}

fn normalized(mut stmts: Vec<Stmt>) -> Vec<Stmt> {
    let bound = bound_names(&mut stmts);
    normalize_generated(&mut stmts, &bound);
    stmts
}

struct NestedCall<'a> {
    name: &'a str,
    callee: &'a Template,
    enclosing_bound: &'a HashSet<Ident>,
}

impl NestedCall<'_> {
    fn args_for(&self, window: &[Stmt], following: &[Stmt]) -> Option<Vec<Expr>> {
        let params = &self.callee.params;
        let mut found = window.to_vec();
        let mut pattern = self.callee.body.clone();
        let found_binders = binders(&mut found);
        let pattern_binders = binders(&mut pattern);
        if found_binders.len() != pattern_binders.len() {
            return None;
        }
        let renamed: HashMap<Ident, Ident> = pattern_binders
            .into_iter()
            .zip(found_binders.iter().copied())
            .collect();
        for stmt in &mut pattern {
            visit_names(stmt, |name, _| {
                if let Some(target) = renamed.get(name) {
                    *name = *target;
                }
            });
        }
        let mut slots = vec![None; params.len()];
        for (pattern, found) in pattern.iter().zip(&found) {
            let (mut wanted, mut actual) = (Vec::new(), Vec::new());
            stmt_exprs(pattern, &mut wanted);
            stmt_exprs(found, &mut actual);
            for (wanted, actual) in wanted.into_iter().zip(actual) {
                bind_holes(wanted, actual, params, &mut slots);
            }
        }
        let args: Vec<Expr> = slots.into_iter().collect::<Option<_>>()?;
        let local: HashSet<Ident> = found_binders.into_iter().collect();
        let mut arg_stmts: Vec<Stmt> = args.iter().cloned().map(Stmt::Expr).collect();
        if reads_any(&mut arg_stmts, &local) || reads_any(&mut following.to_vec(), &local) {
            return None;
        }
        let shadowing: HashSet<Ident> = self.enclosing_bound.difference(&local).copied().collect();
        if reads_any(&mut pattern, &shadowing) {
            return None;
        }
        for stmt in &mut pattern {
            let mut exprs = Vec::new();
            stmt_exprs_mut(stmt, &mut exprs);
            for expr in exprs {
                fill_holes(expr, params, &args);
            }
        }
        (normalized(pattern) == normalized(found)).then_some(args)
    }

    fn replace_in(&self, body: &mut Vec<Stmt>) {
        for stmt in body.iter_mut() {
            for child in stmt.child_bodies_mut() {
                self.replace_in(child);
            }
        }
        let length = self.callee.body.len();
        let mut at = 0;
        while at + length <= body.len() {
            let shapes_agree = self
                .callee
                .body
                .iter()
                .zip(&body[at..at + length])
                .all(|(a, b)| std::mem::discriminant(a) == std::mem::discriminant(b));
            let args = shapes_agree
                .then(|| self.args_for(&body[at..at + length], &body[at + length..]))
                .flatten();
            if let Some(args) = args {
                body.splice(
                    at..at + length,
                    [Stmt::Expr(Expr::Macro {
                        name: self.name.to_string(),
                        args,
                    })],
                );
            }
            at += 1;
        }
    }
}

fn nest_macro_calls(
    chosen: &mut [(String, Template, usize)],
    mentions: &HashMap<String, Vec<String>>,
) {
    let raw: Vec<(String, Template)> = chosen
        .iter()
        .map(|(name, template, _)| (name.clone(), template.clone()))
        .collect();
    for (name, template, _) in chosen {
        let enclosing_bound = bound_names(&mut template.body);
        for (callee_name, callee) in &raw {
            let callable = callee_name != name
                && mentions
                    .get(name)
                    .is_some_and(|words| words.contains(callee_name))
                && callee
                    .params
                    .iter()
                    .all(|param| param.shape == MacroParamShape::Expr);
            if callable {
                NestedCall {
                    name: callee_name,
                    callee,
                    enclosing_bound: &enclosing_bound,
                }
                .replace_in(&mut template.body);
            }
        }
        template.body = normalized(std::mem::take(&mut template.body));
    }
}

pub(super) fn fold(program: &mut Program) {
    let mut seen: Vec<(String, Template, usize)> = Vec::new();
    let mut mentions: HashMap<String, Vec<String>> = HashMap::new();
    for_each_body(&mut program.items, &mut |body, locals| {
        visit_regions(body, &mut |region, stmts| {
            if let Some(found) = candidate(region, stmts, &locals) {
                mentions
                    .entry(region.name.clone())
                    .or_insert_with(|| region.mentions.clone());
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
    nest_macro_calls(&mut chosen, &mentions);
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
