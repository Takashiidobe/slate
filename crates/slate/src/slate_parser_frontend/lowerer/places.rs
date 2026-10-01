use super::*;

impl Tables<'_> {
    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Deref(_) | PlaceKind::CompoundLiteral { .. } => true,
            PlaceKind::Field { base, .. } => self.is_union(&base.ty) || self.place_is_unsafe(base),
            _ => self.place_is_static(place),
        }
    }

    pub(super) fn place_is_static(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Binding(id) => self.statics.contains(id),
            PlaceKind::CompoundLiteral { storage, .. } => {
                matches!(storage, ir::StorageDuration::Static)
            }
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
            } if let Some(field) = tables
                .record_fields(&base.ty)
                .and_then(|fields| fields.get(index)) =>
            {
                self.lower_type(&base.ty)?;
                Ok(Expr::Field {
                    base: Box::new(self.lower_place(base)?),
                    field: field_name(field, index),
                })
            }
            PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment: None,
                ref initializer,
            } => self.lower_compound_literal(object, storage, initializer),
            _ => Err(Construct::Place {
                ir: place.to_string(),
            }
            .into()),
        }
    }

    fn lower_compound_literal(
        &mut self,
        object: BindingId,
        storage: ir::StorageDuration,
        initializer: &ir::Value,
    ) -> Result<Expr> {
        let name = format!("__slate_compound_{}", object.0);
        let ty = self.lower_type(&initializer.ty)?;
        let init = self.lower_value(initializer)?;
        if matches!(storage, ir::StorageDuration::Static) {
            let emitted =
                self.dependencies.compound_literals.iter().any(
                    |item| matches!(item, Item::Static { name: emitted, .. } if *emitted == name),
                );
            if emitted {
                return Ok(Expr::Var(name.into()));
            }
            self.dependencies.compound_literals.push(Item::Static {
                attrs: Vec::new(),
                vis: rust::Visibility::Private,
                mutable: true,
                name: name.clone(),
                ty,
                init,
            });
            return Ok(Expr::Var(name.into()));
        }
        self.hoisted.push(Stmt::Let {
            name: name.clone(),
            mutable: true,
            ty: Some(ty),
            init: Some(zeroed()),
        });
        Ok(Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(Expr::Block(Box::new(rust::Block {
                stmts: vec![Stmt::Assign {
                    target: Expr::Var(name.as_str().into()),
                    value: init,
                }],
                tail: Some(Box::new(Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(Expr::Var(name.into())),
                })),
            }))),
        })
    }
}
