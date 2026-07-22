use std::collections::BTreeMap;

use crate::fixups::facts::{BindingKind, FixupFacts};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Item, Prim, Program, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    StringParams::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct StringParams<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> StringParams<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, facts);
        if changed && let Some(before) = before {
            let planned_params = facts.string_param_lifts.len();
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::StringParams,
                kind: "rewrite_string_params".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact("planned_params", planned_params.to_string())],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let plans = plans_from_facts(facts);
    if plans.is_empty() {
        return false;
    }

    let mut changed = false;
    for item in &mut program.items {
        let Item::Fn(f) = item else {
            continue;
        };
        if let Some(plan) = plans.get(&f.name) {
            changed |= rewrite_function(f, plan);
        }
        changed |= rewrite_calls_in_body(&mut f.body, &plans);
    }
    changed
}

#[derive(Debug, Clone)]
struct Plan {
    params: Vec<ParamPlan>,
}

#[derive(Debug, Clone)]
struct ParamPlan {
    index: usize,
    name: String,
}

fn plans_from_facts(facts: &FixupFacts) -> BTreeMap<String, Plan> {
    let mut plans = BTreeMap::<String, Plan>::new();
    for fact in &facts.string_param_lifts {
        let Some(function) = facts
            .functions
            .iter()
            .find(|function| function.id == fact.callee)
        else {
            continue;
        };
        let Some(binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == fact.param)
        else {
            continue;
        };
        if !matches!(binding.kind, BindingKind::Param { .. }) {
            continue;
        }
        plans
            .entry(function.name.clone())
            .or_insert_with(|| Plan { params: Vec::new() })
            .params
            .push(ParamPlan {
                index: fact.index,
                name: binding.name.clone(),
            });
    }
    for plan in plans.values_mut() {
        plan.params.sort_by_key(|param| param.index);
    }
    plans
}

fn rewrite_function(f: &mut FnDef, plan: &Plan) -> bool {
    let mut changed = false;
    for param in &plan.params {
        if let Some(fn_param) = f.params.get_mut(param.index) {
            fn_param.ty = str_ref_type();
            changed = true;
        }
    }
    changed |= rewrite_param_aliases(&mut f.body, plan);
    changed
}

fn rewrite_param_aliases(body: &mut [IndentStmt], plan: &Plan) -> bool {
    let mut changed = false;
    for indent in body {
        match &mut indent.stmt {
            Stmt::Let {
                ty: Some(ty),
                init: Some(Expr::Var(source)),
                ..
            } if is_char_ptr(ty)
                && plan
                    .params
                    .iter()
                    .any(|param| source.as_str() == param.name) =>
            {
                *ty = str_ref_type();
                changed = true;
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                changed |= rewrite_param_aliases(then_body, plan);
                changed |= rewrite_param_aliases(else_body, plan);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                changed |= rewrite_param_aliases(body, plan);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                changed |= rewrite_param_aliases(&mut body.stmts, plan);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    changed |= rewrite_param_aliases(&mut arm.body, plan);
                }
            }
            _ => {}
        }
    }
    changed
}

fn rewrite_calls_in_body(
    body: &mut [crate::rust_ast::IndentStmt],
    plans: &BTreeMap<String, Plan>,
) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args, .. } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(plan) = plans.get(name.as_str()) else {
            return true;
        };
        for param in &plan.params {
            let Some(arg) = args.get_mut(param.index) else {
                continue;
            };
            if let Some(replacement) = lifted_arg(arg) {
                *arg = replacement;
                changed = true;
            }
        }
        true
    });
    changed
}

fn lifted_arg(expr: &Expr) -> Option<Expr> {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => lifted_arg(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            lifted_arg(recv)
        }
        Expr::ArrayPtr { array, .. } => lifted_arg(array),
        Expr::Var(name) => Some(Expr::Var(Ident::new(name.as_str()))),
        Expr::CStr(bytes) => {
            let payload = c_string_payload(bytes)?;
            Some(Expr::Str(std::str::from_utf8(payload).ok()?.to_string()))
        }
        _ => None,
    }
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn c_string_payload(bytes: &[u8]) -> Option<&[u8]> {
    let payload = bytes.strip_suffix(&[0])?;
    (!payload.contains(&0) && std::str::from_utf8(payload).is_ok()).then_some(payload)
}

fn is_char_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    matches!(&**inner, Type::Prim(Prim::I8 | Prim::U8))
}
