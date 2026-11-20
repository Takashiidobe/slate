use crate::sensitive::has_sensitive_attr;
use rustc_errors::DiagDecorator;
use rustc_hir::{Pat, PatKind};
use rustc_lint::{LateContext, LateLintPass, LintContext};
use rustc_middle::ty::{self, VariantDef};

rustc_lint_defs::declare_tool_lint! {
    pub slate::SENSITIVE_FIELD_REST,
    Warn,
    "`..` in a pattern skips a #[slate::sensitive] field"
}

rustc_lint_defs::declare_lint_pass!(SensitiveFieldRest => [SENSITIVE_FIELD_REST]);

impl<'tcx> LateLintPass<'tcx> for SensitiveFieldRest {
    fn check_pat(&mut self, cx: &LateContext<'tcx>, pat: &'tcx Pat<'tcx>) {
        if pat.span.in_external_macro(cx.sess().source_map()) {
            return;
        }
        let mentioned: Vec<usize> = match pat.kind {
            PatKind::Struct(_, fields, Some(_)) => fields
                .iter()
                .map(|field| cx.typeck_results().field_index(field.hir_id).as_usize())
                .collect(),
            PatKind::TupleStruct(_, elements, dotdot) if dotdot.as_opt_usize().is_some() => {
                let Some(variant) = pattern_variant(cx, pat) else {
                    return;
                };
                let gap = dotdot.as_opt_usize().unwrap_or(elements.len());
                let skipped = variant.fields.len().saturating_sub(elements.len());
                (0..elements.len())
                    .map(|position| {
                        if position >= gap {
                            position + skipped
                        } else {
                            position
                        }
                    })
                    .collect()
            }
            _ => return,
        };
        let Some(variant) = pattern_variant(cx, pat) else {
            return;
        };
        let skipped: Vec<String> = variant
            .fields
            .iter_enumerated()
            .filter(|(index, field)| {
                !mentioned.contains(&index.as_usize()) && has_sensitive_attr(cx.tcx, field.did)
            })
            .map(|(_, field)| field.name.to_string())
            .collect();
        if skipped.is_empty() {
            return;
        }
        let names = skipped.join("`, `");
        cx.tcx.emit_node_span_lint(
            SENSITIVE_FIELD_REST,
            pat.hir_id,
            pat.span,
            DiagDecorator(|diag| {
                diag.primary_message(format!("`..` skips sensitive field `{names}`"));
                diag.help("bind the field, or write `field: _` to show it was considered");
            }),
        );
    }
}

fn pattern_variant<'tcx>(cx: &LateContext<'tcx>, pat: &Pat<'tcx>) -> Option<&'tcx VariantDef> {
    let ty::Adt(adt, _) = cx.typeck_results().pat_ty(pat).peel_refs().kind() else {
        return None;
    };
    let res = match pat.kind {
        PatKind::Struct(ref qpath, ..) | PatKind::TupleStruct(ref qpath, ..) => {
            cx.qpath_res(qpath, pat.hir_id)
        }
        _ => return None,
    };
    Some(adt.variant_of_res(res))
}
