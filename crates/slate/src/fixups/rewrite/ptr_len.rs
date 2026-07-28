use std::collections::BTreeMap;

use crate::fixups::facts::{BindingId, BindingKind, FixupFacts, FunctionId, PtrLenSliceFact};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Item, Program, Type};

pub(in crate::fixups) struct PtrLen<'a> {
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PtrLen<'a> {
    pub(in crate::fixups) fn new(facts: &'a FixupFacts, logger: &'a mut dyn TraceLogger) -> Self {
        Self { facts, logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, self.facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::PtrLen,
                kind: "rewrite_ptr_len_slice_params".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "planned_pairs",
                    self.facts.ptr_len_slices.len().to_string(),
                )],
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
        if let Item::Fn(f) = item {
            if let Some(fn_plans) = plans.get(&f.name) {
                changed |= rewrite_function(f, fn_plans);
            }
            changed |= rewrite_calls_in_body(&mut f.body, &plans);
        }
    }
    changed
}

#[derive(Clone)]
struct Plan {
    ptr_index: usize,
    ptr_name: String,
    mutable: bool,
    elem: Type,
}

fn plans_from_facts(facts: &FixupFacts) -> BTreeMap<String, Vec<Plan>> {
    let mut grouped = BTreeMap::<(FunctionId, BindingId), Vec<&PtrLenSliceFact>>::new();
    for fact in &facts.ptr_len_slices {
        grouped
            .entry((fact.callee, fact.ptr_param))
            .or_default()
            .push(fact);
    }

    let mut plans = BTreeMap::<String, Vec<Plan>>::new();
    for ((function, ptr_param), calls) in grouped {
        let Some(function_fact) = facts.functions.iter().find(|fact| fact.id == function) else {
            continue;
        };
        let Some(ptr_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == ptr_param)
        else {
            continue;
        };
        let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
            continue;
        };
        let mutable = calls.iter().any(|call| call.mutable);
        let first = calls[0];
        plans
            .entry(function_fact.name.clone())
            .or_default()
            .push(Plan {
                ptr_index,
                ptr_name: ptr_binding.name.clone(),
                mutable,
                elem: first.elem_ty.clone(),
            });
    }
    plans
}

fn rewrite_function(f: &mut FnDef, plans: &[Plan]) -> bool {
    if plans.iter().any(|plan| f.params.len() <= plan.ptr_index) {
        return false;
    }

    let mut changed = false;
    for plan in plans {
        f.params[plan.ptr_index].ty = Type::Ref {
            mutable: plan.mutable,
            inner: Box::new(Type::Slice(Box::new(plan.elem.clone()))),
        };
        changed = true;
        changed |= rewrite_body_pointer_param(&mut f.body, &plan.ptr_name, plan.mutable);
    }
    changed
}

fn rewrite_body_pointer_param(body: &mut [IndentStmt], name: &str, mutable: bool) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        if matches!(expr, Expr::Var(var) if var.as_str() == name) {
            changed = true;
            *expr = Expr::MethodCall {
                recv: Box::new(Expr::Var(name.into())),
                method: if mutable { "as_mut_ptr" } else { "as_ptr" }.into(),
                args: Vec::new(),
            };
            return false;
        }
        true
    });
    changed
}

fn rewrite_calls_in_body(body: &mut [IndentStmt], plans: &BTreeMap<String, Vec<Plan>>) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args, .. } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(fn_plans) = plans.get(name.as_str()) else {
            return true;
        };
        if fn_plans.iter().any(|plan| args.len() <= plan.ptr_index) {
            return true;
        }
        let Some(array_names) = fn_plans
            .iter()
            .map(|plan| args.get(plan.ptr_index).and_then(array_pointer_arg))
            .collect::<Option<Vec<_>>>()
        else {
            return true;
        };

        for (plan, array_name) in fn_plans.iter().zip(array_names) {
            args[plan.ptr_index] = Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::from(array_name))),
                method: if plan.mutable {
                    "as_mut_slice"
                } else {
                    "as_slice"
                }
                .into(),
                args: Vec::new(),
            };
        }

        changed = true;
        false
    });
    changed
}

fn array_pointer_arg(expr: &Expr) -> Option<String> {
    match peel_pointer_view(expr) {
        Expr::Var(name) => Some(name.as_str().into()),
        _ => None,
    }
}

fn peel_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => peel_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            peel_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => peel_pointer_view(array),
        _ => expr,
    }
}
