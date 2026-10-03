use super::*;

impl Tables<'_> {
    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Deref(_) | PlaceKind::CompoundLiteral { .. } => true,
            PlaceKind::Binding(_) if self.variably_modified(&place.ty) => true,
            PlaceKind::Lane { base, .. } => self.place_is_unsafe(base),
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
            PlaceKind::Field { base, .. } | PlaceKind::Lane { base, .. } => {
                self.place_is_static(base)
            }
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_address(&mut self, place: &ir::Place, mutable: bool) -> Result<Expr> {
        if let Some(address) = self.vla_address(place)? {
            return Ok(address);
        }
        match &place.kind {
            PlaceKind::Deref(pointer) => self.lower_value(pointer),
            PlaceKind::Field { bits: Some(_), .. } => Err(Construct::Place {
                ir: place.to_string(),
            }
            .into()),
            _ => Ok(Expr::AddrOf {
                mutable,
                expr: Box::new(self.lower_place(place)?),
            }),
        }
    }

    pub(super) fn lower_place(&mut self, place: &ir::Place) -> Result<Expr> {
        let tables = self.tables;
        if let PlaceKind::Binding(_) = place.kind
            && let Some(address) = self.vla_address(place)?
        {
            return Ok(Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(address),
            });
        }
        match place.kind {
            PlaceKind::Binding(id) => Ok(self.lower_binding(id)),
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
            PlaceKind::Lane {
                ref base,
                ref index,
            } => Ok(Expr::Index {
                base: Box::new(self.lower_place(base)?),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.lower_value(index)?),
                    ty: rust::Type::Prim(Prim::Usize),
                }),
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

    pub(super) fn lower_binding(&self, id: BindingId) -> Expr {
        if self.dispatch_bindings.contains(&id) {
            return Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(control_flow::slot_pointer(id)),
            };
        }
        let binding = Expr::Var(binding_name(id, &self.tables.bindings).as_str().into());
        if self.tables.over_aligned.contains_key(&id) || self.aligned_locals.contains(&id) {
            Expr::TupleField {
                base: Box::new(binding),
                index: 0,
            }
        } else {
            binding
        }
    }

    pub(super) fn bit_field_accessor(
        &mut self,
        place: &ir::Place,
        kind: &str,
    ) -> Result<Option<(Expr, String)>> {
        let PlaceKind::Field {
            ref base,
            index,
            bits: Some(ref bits),
        } = place.kind
        else {
            return Ok(None);
        };
        let field = self
            .tables
            .record_fields(&base.ty)
            .and_then(|fields| fields.get(index))
            .ok_or_else(|| {
                Failure::from(Construct::Place {
                    ir: place.to_string(),
                })
            })?;
        self.lower_type(&base.ty)?;
        let storage = Expr::Field {
            base: Box::new(self.lower_place(base)?),
            field: bit_unit_name(bits.unit),
        };
        let storage = if self.tables.place_is_static(base) {
            Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(Expr::AddrOf {
                    mutable: kind == "set",
                    expr: Box::new(storage),
                }),
            }
        } else {
            storage
        };
        Ok(Some((
            storage,
            format!("__{kind}_{}", field_name(field, index)),
        )))
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
