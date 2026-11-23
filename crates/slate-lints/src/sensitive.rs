use rustc_ast::attr::AttributeExt;
use rustc_attr_ir::Attribute;
use rustc_middle::ty::{self, Ty, TyCtxt};
use rustc_span::{Symbol, def_id::DefId};

#[expect(
    deprecated,
    reason = "slate:: tool attributes are unparsed, so find_attr! cannot see them"
)]
pub fn has_sensitive_attr(tcx: TyCtxt<'_>, did: DefId) -> bool {
    tcx.get_all_attrs(did).iter().any(is_sensitive_attr)
}

#[expect(
    deprecated,
    reason = "slate:: tool attributes are unparsed, so find_attr! cannot see them"
)]
pub fn requires_exhaustive_match(tcx: TyCtxt<'_>, did: DefId) -> bool {
    tcx.get_all_attrs(did)
        .iter()
        .filter(|attr| is_sensitive_attr(attr))
        .any(|attr| {
            attr.meta_item_list().is_some_and(|args| {
                args.iter().any(|arg| {
                    arg.meta_item().is_some_and(|item| {
                        item.is_word() && item.has_name(Symbol::intern("exhaustive"))
                    })
                })
            })
        })
}

fn is_sensitive_attr(attr: &Attribute) -> bool {
    let path = attr.path();
    path.len() == 2 && path[0].as_str() == "slate" && path[1].as_str() == "sensitive"
}

pub fn sensitive_enum<'tcx>(tcx: TyCtxt<'tcx>, ty: Ty<'tcx>) -> Option<DefId> {
    match ty.peel_refs().kind() {
        ty::Adt(adt, _) if adt.is_enum() && has_sensitive_attr(tcx, adt.did()) => Some(adt.did()),
        _ => None,
    }
}
