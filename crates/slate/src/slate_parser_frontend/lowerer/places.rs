use super::*;

pub(super) fn lower_place(place: &ir::Place, cx: &Context) -> Result<Expr> {
    match place.kind {
        PlaceKind::Binding(id) => {
            let binding = Expr::Var(binding_name(id, &cx.bindings).as_str().into());
            Ok(if cx.over_aligned.contains_key(&id) {
                Expr::TupleField {
                    base: Box::new(binding),
                    index: 0,
                }
            } else {
                binding
            })
        }
        PlaceKind::Deref(ref pointer) => Ok(Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(lower_value(pointer, cx)?),
        }),
        PlaceKind::Index {
            ref base,
            ref index,
        } => Ok(Expr::Index {
            base: Box::new(lower_value(base, cx)?),
            index: Box::new(lower_value(index, cx)?),
        }),
        PlaceKind::Field {
            ref base,
            index,
            bits: None,
        } if let Some(Some(name)) = record_fields(cx, &base.ty)
            .and_then(|fields| fields.get(index))
            .map(|field| &field.name) =>
        {
            lower_type(cx, &base.ty)?;
            Ok(Expr::Field {
                base: Box::new(lower_place(base, cx)?),
                field: name.clone(),
            })
        }
        _ => Err(super::Error::Unsupported(format!("place {place:?}"))),
    }
}

pub(super) fn place_is_unsafe(cx: &Context, place: &ir::Place) -> bool {
    match &place.kind {
        PlaceKind::Deref(_) => true,
        PlaceKind::Field { base, .. } => place_is_unsafe(cx, base),
        _ => place_is_static(cx, place),
    }
}

pub(super) fn place_is_static(cx: &Context, place: &ir::Place) -> bool {
    match &place.kind {
        PlaceKind::Binding(id) => cx.statics.contains(id),
        PlaceKind::Field { base, .. } => place_is_static(cx, base),
        _ => false,
    }
}
