use std::collections::{HashMap, HashSet};

use super::macro_boundaries;
use super::rules::walk::{child_exprs_mut, stmt_exprs_mut};
use super::shadow::visit_names;
use crate::backend::codegen::is_rust_keyword;
use crate::backend::format::MACRO_LIST_HOLE;
use crate::backend::rust_ast::{
    CommentAttach, Expr, Ident, ImplItem, Item, MacroInlineReason, MacroOutcome, MacroParam,
    MacroParamShape, MacroRegion, Program, Stmt,
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
    level: u32,
    bound: HashSet<Ident>,
    single: Vec<Option<Expr>>,
    lists: Vec<Option<Vec<Expr>>>,
    failed: Option<MacroInlineReason>,
}

impl Holes<'_> {
    fn is_macro_local(&self, mut expr: &Expr) -> bool {
        while let Expr::MacroArg { expr: inner, .. } = expr {
            expr = inner;
        }
        matches!(expr, Expr::Var(name) if self.bound.contains(name))
    }

    fn shape(&self, index: u32) -> Option<MacroParamShape> {
        self.params.get(index as usize).map(|param| param.shape)
    }

    fn gather_argument_uses(&self, expr: &mut Expr, uses: &mut [Vec<Expr>]) {
        if let Some((index, value)) = cast_argument(expr, self.level) {
            uses[index as usize].push(value);
            return;
        }
        for child in child_exprs_mut(expr) {
            self.gather_argument_uses(child, uses);
        }
    }

    fn lift_casts_where_uses_agree(&self, template: &mut [Stmt]) {
        let mut uses = vec![Vec::new(); self.params.len()];
        for stmt in template.iter_mut() {
            let mut exprs = Vec::new();
            stmt_exprs_mut(stmt, &mut exprs);
            for expr in exprs {
                self.gather_argument_uses(expr, &mut uses);
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
                lift_casts(expr, &liftable, self.level);
            }
        }
    }

    fn erase_list(&mut self, args: &mut Vec<Expr>) {
        let mut erased = Vec::with_capacity(args.len());
        let mut remaining = std::mem::take(args).into_iter().peekable();
        while let Some(arg) = remaining.next() {
            let level = self.level;
            let index = match &arg {
                Expr::MacroArg {
                    level: found, expr, ..
                } if *found == level && self.is_macro_local(expr) => {
                    erased.push(arg);
                    continue;
                }
                Expr::MacroArg {
                    level: found,
                    index,
                    ..
                } if *found == level && self.shape(*index) == Some(MacroParamShape::List) => *index,
                _ => {
                    erased.push(arg);
                    continue;
                }
            };
            let mut run = vec![arg];
            while remaining.peek().is_some_and(
                |next| matches!(next, Expr::MacroArg { level: found, index: at, .. } if *found == level && *at == index),
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
                self.failed
                    .get_or_insert(MacroInlineReason::ConflictingArgument);
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
        if let Expr::MacroArg {
            level, expr: inner, ..
        } = expr
            && *level == self.level
            && self.is_macro_local(inner)
        {
            let mut local = std::mem::replace(&mut **inner, Expr::Str(String::new()));
            while let Expr::MacroArg { expr: inner, .. } = local {
                local = *inner;
            }
            *expr = local;
            return;
        }
        if let Expr::MacroArg {
            level,
            index,
            expr: inner,
        } = expr
            && *level == self.level
        {
            let index = *index as usize;
            if self.shape(index as u32) != Some(MacroParamShape::Expr) {
                self.failed.get_or_insert(MacroInlineReason::ArgumentShape);
                return;
            }
            let value = std::mem::replace(&mut **inner, Expr::Str(String::new()));
            let slot = &mut self.single[index];
            if slot.as_ref().is_some_and(|known| *known != value) {
                self.failed
                    .get_or_insert(MacroInlineReason::ConflictingArgument);
            }
            *slot = Some(value);
            *expr = Expr::Var(format!("${}", self.params[index].name).into());
            return;
        }
        for child in child_exprs_mut(expr) {
            self.erase(child);
        }
    }

    fn call_args(self) -> Result<(Vec<MacroParam>, Vec<Expr>), MacroInlineReason> {
        if let Some(reason) = self.failed {
            return Err(reason);
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
            return Err(MacroInlineReason::ListParamNotLast);
        }
        let mut params = Vec::with_capacity(used);
        let mut args = Vec::new();
        for ((param, single), list) in self.params.iter().zip(self.single).zip(self.lists) {
            match param.shape {
                MacroParamShape::Unused => continue,
                MacroParamShape::Expr => {
                    args.push(single.ok_or(MacroInlineReason::ParamWithoutArgument)?)
                }
                MacroParamShape::List => {
                    args.extend(list.ok_or(MacroInlineReason::ParamWithoutArgument)?)
                }
            }
            params.push(param.clone());
        }
        Ok((params, args))
    }
}

fn cast_argument(expr: &Expr, level: u32) -> Option<(u32, Expr)> {
    match expr {
        Expr::MacroArg {
            level: found,
            index,
            expr,
        } if *found == level => Some((*index, (**expr).clone())),
        Expr::Cast { expr, ty } => cast_argument(expr, level).map(|(index, value)| {
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

fn lift_casts(expr: &mut Expr, liftable: &[bool], level: u32) {
    for child in child_exprs_mut(expr) {
        lift_casts(child, liftable, level);
    }
    let Expr::Cast { expr: inner, .. } = expr else {
        return;
    };
    let index = match **inner {
        Expr::MacroArg {
            level: found,
            index,
            ..
        } if found == level && liftable[index as usize] => index,
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
        level,
        index,
        expr: Box::new(cast),
    };
}

fn holds_macro_arg(expr: &mut Expr) -> bool {
    matches!(expr, Expr::MacroArg { .. }) || child_exprs_mut(expr).into_iter().any(holds_macro_arg)
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

pub(crate) fn is_valid_macro_name(name: &str) -> bool {
    !is_rust_keyword(name) && !STD_MACROS.contains(&name)
}

fn candidate(
    region: &MacroRegion,
    body: &[Stmt],
    locals: &HashSet<Ident>,
) -> Result<Candidate, MacroInlineReason> {
    if !is_valid_macro_name(&region.name) {
        return Err(MacroInlineReason::ReservedMacroName);
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
        return Err(MacroInlineReason::OnlyComments);
    }
    let mut holes = Holes {
        params: &region.params,
        level: region.level,
        bound: bound_names(&mut template),
        single: vec![None; region.params.len()],
        lists: vec![None; region.params.len()],
        failed: None,
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
    for stmt in &mut template {
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        if exprs.into_iter().any(holds_macro_arg) {
            return Err(MacroInlineReason::ArgumentOutsideHole);
        }
    }
    let bound = bound_names(&mut template);
    let mut arg_stmts: Vec<Stmt> = args.iter().cloned().map(Stmt::Expr).collect();
    if reads_any(&mut arg_stmts, &bound) {
        return Err(MacroInlineReason::ArgumentReadsMacroLocal);
    }
    let captured: HashSet<Ident> = locals.difference(&bound).copied().collect();
    if reads_any(&mut template, &captured) {
        return Err(MacroInlineReason::BodyReadsCallerLocal);
    }
    normalize_generated(&mut template, &bound);
    Ok(Candidate {
        template: Template {
            params,
            body: template,
        },
        args,
        before,
        after,
    })
}

fn holds_region(stmts: &mut [Stmt]) -> bool {
    stmts.iter_mut().any(|stmt| {
        matches!(
            stmt,
            Stmt::Scope {
                macro_region: Some(_),
                ..
            }
        ) || stmt
            .child_bodies_mut()
            .into_iter()
            .any(|body| holds_region(body))
    })
}

fn nests_regions(stmts: &mut [Stmt]) -> bool {
    stmts.iter_mut().any(|stmt| {
        let in_region = matches!(
            stmt,
            Stmt::Scope {
                macro_region: Some(_),
                ..
            }
        );
        stmt.child_bodies_mut().into_iter().any(|body| {
            if in_region {
                holds_region(body)
            } else {
                nests_regions(body)
            }
        })
    })
}

fn visit_innermost_regions(
    stmts: &mut Vec<Stmt>,
    visit: &mut impl FnMut(&MacroRegion, &[Stmt]) -> Option<Vec<Stmt>>,
) {
    let mut out = Vec::with_capacity(stmts.len());
    for mut stmt in std::mem::take(stmts) {
        if let Stmt::Scope {
            body,
            macro_region: Some(region),
        } = &mut stmt
            && !holds_region(body)
        {
            if let Some(replacement) = visit(region, body) {
                out.extend(replacement);
                continue;
            }
            out.push(stmt);
            continue;
        }
        for body in stmt.child_bodies_mut() {
            visit_innermost_regions(body, visit);
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

fn choose_templates(program: &mut Program, chosen: &mut Vec<(String, Template, usize)>) {
    let mut seen: Vec<(String, Template, usize)> = Vec::new();
    for_each_body(&mut program.items, &mut |body, locals| {
        visit_innermost_regions(body, &mut |region, stmts| {
            if region.foldable
                && !chosen.iter().any(|(name, ..)| *name == region.name)
                && let Ok(found) = candidate(region, stmts, &locals)
            {
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
    let first_new = chosen.len();
    for (name, template, count) in seen {
        match chosen[first_new..]
            .iter_mut()
            .find(|(known, ..)| *known == name)
        {
            Some(best) if best.2 >= count => {}
            Some(best) => *best = (name, template, count),
            None => chosen.push((name, template, count)),
        }
    }
}

fn chosen_candidate(
    region: &MacroRegion,
    stmts: &[Stmt],
    locals: &HashSet<Ident>,
    chosen: &[(String, Template, usize)],
) -> Result<Candidate, MacroInlineReason> {
    let found = candidate(region, stmts, locals)?;
    chosen
        .iter()
        .any(|(name, template, _)| *name == region.name && *template == found.template)
        .then_some(found)
        .ok_or(MacroInlineReason::AnotherTemplateChosen)
}

fn fold_innermost_regions(program: &mut Program, chosen: &[(String, Template, usize)]) {
    let Program {
        items,
        macro_outcomes,
    } = program;
    for_each_body(items, &mut |body, locals| {
        visit_innermost_regions(body, &mut |region, stmts| {
            if !region.foldable {
                return (region.level > 0).then(|| {
                    let mut inline = stmts.to_vec();
                    macro_boundaries::strip_level(&mut inline, region.level);
                    inline
                });
            }
            match chosen_candidate(region, stmts, &locals, chosen) {
                Ok(found) => {
                    macro_outcomes.push(MacroOutcome {
                        name: region.name.clone(),
                        inline: None,
                    });
                    let mut out = found.before;
                    out.push(Stmt::Expr(Expr::Macro {
                        name: region.name.clone(),
                        args: found.args,
                    }));
                    out.extend(found.after);
                    Some(out)
                }
                Err(reason) if region.level > 0 => {
                    macro_outcomes.push(MacroOutcome {
                        name: region.name.clone(),
                        inline: Some(reason),
                    });
                    let mut inline = stmts.to_vec();
                    macro_boundaries::strip_level(&mut inline, region.level);
                    Some(inline)
                }
                Err(_) => None,
            }
        });
    });
}

fn report_remaining_regions(program: &mut Program, chosen: &[(String, Template, usize)]) {
    let Program {
        items,
        macro_outcomes,
    } = program;
    for_each_body(items, &mut |body, locals| {
        visit_innermost_regions(body, &mut |region, stmts| {
            if region.foldable
                && let Err(reason) = chosen_candidate(region, stmts, &locals, chosen)
            {
                macro_outcomes.push(MacroOutcome {
                    name: region.name.clone(),
                    inline: Some(reason),
                });
            }
            None
        });
    });
}

pub(super) fn fold(program: &mut Program) {
    let mut chosen: Vec<(String, Template, usize)> = Vec::new();
    loop {
        let mut nested = false;
        for_each_body(&mut program.items, &mut |body, _| {
            nested |= nests_regions(body);
        });
        choose_templates(program, &mut chosen);
        fold_innermost_regions(program, &chosen);
        if !nested {
            break;
        }
    }
    report_remaining_regions(program, &chosen);
    if chosen.is_empty() {
        return;
    }
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
