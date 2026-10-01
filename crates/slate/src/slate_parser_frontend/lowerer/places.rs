use super::*;

impl Tables<'_> {
    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Deref(_) => true,
            PlaceKind::Field { base, .. } => self.place_is_unsafe(base),
            _ => self.place_is_static(place),
        }
    }

    pub(super) fn place_is_static(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Binding(id) => self.statics.contains(id),
            PlaceKind::Field { base, .. } => self.place_is_static(base),
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_place(&mut self, place: &ir::Place) -> Result<Expr> {
        let tables = self.tables;
        match place.kind {
            PlaceKind::Binding(id) => {
                let binding = Expr::Var(binding_name(id, &tables.bindings).as_str().into());
                Ok(if tables.over_aligned.contains_key(&id) {
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
                expr: Box::new(self.lower_value(pointer)?),
            }),
            PlaceKind::Index {
                ref base,
                ref index,
            } => Ok(Expr::Index {
                base: Box::new(self.lower_value(base)?),
                index: Box::new(self.lower_value(index)?),
            }),
            PlaceKind::Field {
                ref base,
                index,
                bits: None,
            } if let Some(Some(name)) = tables
                .record_fields(&base.ty)
                .and_then(|fields| fields.get(index))
                .map(|field| &field.name) =>
            {
                self.lower_type(&base.ty)?;
                Ok(Expr::Field {
                    base: Box::new(self.lower_place(base)?),
                    field: name.clone(),
                })
            }
            _ => Err(super::Error::Unsupported(format!("place {place:?}"))),
        }
    }
}
