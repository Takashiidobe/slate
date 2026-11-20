use crate::sensitive::sensitive_enum;
use rustc_attr_ir::lang_items::LangItem;
use rustc_errors::DiagDecorator;
use rustc_hir::def::{DefKind, Res};
use rustc_hir::{
    Arm, Expr, ExprKind, HirId, MatchSource, Pat, PatExpr, PatExprKind, PatKind, QPath,
};
use rustc_lint::{LateContext, LateLintPass, LintContext};
use rustc_span::Span;
use rustc_span::def_id::DefId;
use rustc_span::hygiene::{ExpnKind, MacroKind};

rustc_lint_defs::declare_tool_lint! {
    pub slate::SENSITIVE_WILDCARD,
    Warn,
    "catch-all match arm over a #[slate::sensitive] enum"
}

rustc_lint_defs::declare_lint_pass!(SensitiveWildcard => [SENSITIVE_WILDCARD]);

#[derive(Clone, PartialEq, Eq)]
struct Step(Option<DefId>, usize);

enum Leaf {
    Wild(Span),
    Ctor {
        enumeration: DefId,
        variant: Option<DefId>,
    },
}

impl<'tcx> LateLintPass<'tcx> for SensitiveWildcard {
    fn check_expr(&mut self, cx: &LateContext<'tcx>, expr: &'tcx Expr<'tcx>) {
        let ExprKind::Match(_, arms, MatchSource::Normal) = expr.kind else {
            return;
        };
        let from_matches = is_matches_expansion(expr.span);
        if !from_matches && expr.span.in_external_macro(cx.sess().source_map()) {
            return;
        }
        let leaves: Vec<Vec<(Vec<Step>, Leaf)>> = arms
            .iter()
            .map(|arm| {
                let mut out = Vec::new();
                collect(cx, arm.pat, &mut Vec::new(), &mut out);
                out
            })
            .collect();
        let ctors: Vec<(&[Step], DefId, Option<DefId>)> = leaves
            .iter()
            .flatten()
            .filter_map(|(path, leaf)| match leaf {
                Leaf::Ctor {
                    enumeration,
                    variant,
                } => Some((path.as_slice(), *enumeration, *variant)),
                Leaf::Wild(_) => None,
            })
            .collect();
        for (arm, arm_leaves) in arms.iter().zip(&leaves) {
            if is_rejecting(cx, arm) {
                continue;
            }
            for (path, leaf) in arm_leaves {
                let Leaf::Wild(span) = leaf else {
                    continue;
                };
                let covered: Vec<_> = ctors
                    .iter()
                    .filter(|(ctor, ..)| ctor.starts_with(path))
                    .collect();
                let Some(&&(_, enum_did, _)) = covered.first() else {
                    continue;
                };
                if is_projection(&covered) {
                    continue;
                }
                let span = if from_matches {
                    expr.span.source_callsite()
                } else {
                    *span
                };
                let name = cx.tcx.def_path_str(enum_did);
                cx.tcx.emit_node_span_lint(SENSITIVE_WILDCARD, arm.hir_id, span, DiagDecorator(|diag| {
                    diag.primary_message(format!(
                        "catch-all over sensitive enum `{name}` hides variants added later"
                    ));
                    diag.help(
                        "list the variants, or `#[expect(slate::sensitive_wildcard, reason = \"...\")]` on the arm or item",
                    );
                }));
            }
        }
    }
}

fn is_matches_expansion(span: Span) -> bool {
    matches!(span.ctxt().outer_expn_data().kind, ExpnKind::Macro(MacroKind::Bang, name) if name.as_str() == "matches")
}

fn collect<'tcx>(
    cx: &LateContext<'tcx>,
    pat: &'tcx Pat<'tcx>,
    path: &mut Vec<Step>,
    out: &mut Vec<(Vec<Step>, Leaf)>,
) {
    let ty = cx.typeck_results().pat_ty(pat);
    let sensitive = sensitive_enum(cx.tcx, ty);
    match pat.kind {
        PatKind::Wild | PatKind::Binding(_, _, _, None) => {
            out.push((path.clone(), Leaf::Wild(pat.span)));
        }
        PatKind::Binding(_, _, _, Some(inner))
        | PatKind::Ref(inner, ..)
        | PatKind::Deref(inner)
        | PatKind::Guard(inner, _) => collect(cx, inner, path, out),
        PatKind::Or(alternatives) => {
            for alternative in alternatives {
                collect(cx, alternative, path, out);
            }
        }
        PatKind::Tuple(elements, dotdot) => {
            let arity = match ty.peel_refs().kind() {
                rustc_middle::ty::Tuple(fields) => fields.len(),
                _ => elements.len(),
            };
            positional(cx, None, elements, dotdot.as_opt_usize(), arity, path, out);
        }
        PatKind::TupleStruct(ref qpath, elements, dotdot) => {
            let variant = variant_of(cx, qpath, pat.hir_id);
            mark_ctor(sensitive, variant, path, out);
            let arity = variant.map_or(elements.len(), |did| variant_arity(cx, did));
            positional(
                cx,
                variant,
                elements,
                dotdot.as_opt_usize(),
                arity,
                path,
                out,
            );
        }
        PatKind::Struct(ref qpath, fields, _) => {
            let variant = variant_of(cx, qpath, pat.hir_id);
            mark_ctor(sensitive, variant, path, out);
            for field in fields {
                let index = cx.typeck_results().field_index(field.hir_id).as_usize();
                path.push(Step(variant, index));
                collect(cx, field.pat, path, out);
                path.pop();
            }
        }
        PatKind::Expr(PatExpr {
            kind: PatExprKind::Path(qpath),
            hir_id,
            ..
        }) => mark_ctor(sensitive, variant_of(cx, qpath, *hir_id), path, out),
        _ => mark_ctor(sensitive, None, path, out),
    }
}

fn mark_ctor(
    sensitive: Option<DefId>,
    variant: Option<DefId>,
    path: &[Step],
    out: &mut Vec<(Vec<Step>, Leaf)>,
) {
    if let Some(enumeration) = sensitive {
        out.push((
            path.to_vec(),
            Leaf::Ctor {
                enumeration,
                variant,
            },
        ));
    }
}

fn is_projection(covered: &[&(&[Step], DefId, Option<DefId>)]) -> bool {
    covered.iter().all(|(path, _, variant)| {
        variant.is_some()
            && covered
                .iter()
                .filter(|(other, ..)| other == path)
                .all(|(_, _, other)| other == variant)
    })
}

fn positional<'tcx>(
    cx: &LateContext<'tcx>,
    variant: Option<DefId>,
    elements: &'tcx [Pat<'tcx>],
    dotdot: Option<usize>,
    arity: usize,
    path: &mut Vec<Step>,
    out: &mut Vec<(Vec<Step>, Leaf)>,
) {
    let skipped = arity.saturating_sub(elements.len());
    for (position, element) in elements.iter().enumerate() {
        let index = match dotdot {
            Some(gap) if position >= gap => position + skipped,
            _ => position,
        };
        path.push(Step(variant, index));
        collect(cx, element, path, out);
        path.pop();
    }
}

fn variant_of(cx: &LateContext<'_>, qpath: &QPath<'_>, hir_id: HirId) -> Option<DefId> {
    match cx.qpath_res(qpath, hir_id) {
        Res::Def(DefKind::Ctor(..), did) => Some(cx.tcx.parent(did)),
        Res::Def(_, did) => Some(did),
        _ => None,
    }
}

fn variant_arity(cx: &LateContext<'_>, did: DefId) -> usize {
    match cx.tcx.def_kind(did) {
        DefKind::Variant => {
            let adt = cx.tcx.adt_def(cx.tcx.parent(did));
            adt.variant_with_id(did).fields.len()
        }
        DefKind::Struct => cx.tcx.adt_def(did).non_enum_variant().fields.len(),
        _ => 0,
    }
}

fn is_rejecting(cx: &LateContext<'_>, arm: &Arm<'_>) -> bool {
    let mut expr = arm.body;
    loop {
        match expr.kind {
            ExprKind::Block(block, _) => match (block.expr, block.stmts.last()) {
                (Some(tail), _) => expr = tail,
                (None, Some(stmt)) => match stmt.kind {
                    rustc_hir::StmtKind::Expr(inner) | rustc_hir::StmtKind::Semi(inner) => {
                        expr = inner
                    }
                    _ => return false,
                },
                (None, None) => return false,
            },
            ExprKind::DropTemps(inner) => expr = inner,
            ExprKind::Ret(Some(inner)) => return is_err_ctor(cx, inner),
            ExprKind::Ret(None)
            | ExprKind::Break(..)
            | ExprKind::Continue(..)
            | ExprKind::Loop(..) => return false,
            _ => {
                return is_err_ctor(cx, expr) || cx.typeck_results().expr_ty(expr).is_never();
            }
        }
    }
}

fn is_err_ctor(cx: &LateContext<'_>, expr: &Expr<'_>) -> bool {
    let ExprKind::Call(callee, _) = expr.kind else {
        return false;
    };
    let ExprKind::Path(ref qpath) = callee.kind else {
        return false;
    };
    let Res::Def(DefKind::Ctor(..), did) = cx.qpath_res(qpath, callee.hir_id) else {
        return false;
    };
    cx.tcx.lang_items().get(LangItem::ResultErr) == Some(cx.tcx.parent(did))
}
