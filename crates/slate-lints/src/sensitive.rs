use rustc_ast::attr::AttributeExt;
use rustc_attr_ir::Attribute;
use rustc_middle::ty::{self, Ty, TyCtxt};
use rustc_span::def_id::DefId;

#[expect(
    deprecated,
    reason = "slate:: tool attributes are unparsed, so find_attr! cannot see them"
)]
pub fn has_sensitive_attr(tcx: TyCtxt<'_>, did: DefId) -> bool {
    tcx.get_all_attrs(did).iter().any(is_sensitive_attr)
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
