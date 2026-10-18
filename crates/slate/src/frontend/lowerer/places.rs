use super::*;

impl Tables<'_> {
    pub(super) fn array_subscript<'p>(
        &self,
        place: &'p ir::Place,
    ) -> Option<(&'p ir::Place, &'p ir::Value)> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return None;
        };
        let ValueKind::PointerOffset {
            pointer,
            amount,
            subtract: false,
            element,
            ..
        } = &pointer.node.value
        else {
            return None;
        };
        let ValueKind::ArrayDecay {
            place: array,
            length: Some(_),
        } = &pointer.node.value
        else {
            return None;
        };
        let ir::Type::Array {
            element: array_element,
            length: Some(1..),
        } = self.resolve_type(&array.ty)
        else {
            return None;
        };
        (**array_element == *element && self.indexable_array(array)).then_some((array, amount))
    }

    fn indexable_array(&self, place: &ir::Place) -> bool {
        if self.variably_modified(&place.ty) {
            return false;
        }
        match &place.kind {
            PlaceKind::Binding(id) => {
                !self.strings.contains_key(id) && !self.register_globals.contains_key(id)
            }
            PlaceKind::Field {
                base, bits: None, ..
            } => self.indexable_array(base),
            PlaceKind::Deref(_) => self
                .array_subscript(place)
                .is_some_and(|(array, _)| self.indexable_array(array)),
            _ => false,
        }
    }

    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        if let Some((array, _)) = self.array_subscript(place) {
            return self.place_is_unsafe(array);
        }
        match &place.kind {
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. } => true,
            PlaceKind::Binding(_) if self.variably_modified(&place.ty) => true,
            PlaceKind::Lane { base, .. } | PlaceKind::ComplexPart { base, .. } => {
                self.place_is_unsafe(base)
            }
            PlaceKind::Field { base, .. } => self.is_union(&base.ty) || self.place_is_unsafe(base),
            _ => self.place_is_static(place),
        }
    }

    pub(super) fn place_is_static(&self, place: &ir::Place) -> bool {
        if let Some((array, _)) = self.array_subscript(place) {
            return self.place_is_static(array);
        }
        match &place.kind {
            PlaceKind::Binding(id) => self.statics.contains(id),
            PlaceKind::CompoundLiteral { storage, .. } => {
                matches!(storage, ir::StorageDuration::Static)
            }
            PlaceKind::Field { base, .. }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::ComplexPart { base, .. } => self.place_is_static(base),
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn place_in_packed(&mut self, place: &ir::Place) -> bool {
        let PlaceKind::Field { base, .. } = &place.kind else {
            return false;
        };
        if self.lower_type(&base.ty).is_err() {
            return false;
        }
        matches!(
            self.tables.resolve_type(&base.ty),
            ir::Type::Defined(id)
                if matches!(
                    self.dependencies.records.get(&id.0),
                    Some(Record::Built(record)) if record.packed.is_some()
                )
        ) || self.place_in_packed(base)
    }

    fn reduced_alignment_access(&mut self, place: &ir::Place) -> bool {
        self.reduced_place(place)
            && !place.access.atomic
            && !place.access.volatile
            && Self::rooted_in_deref(place)
    }

    fn reduced_place(&mut self, place: &ir::Place) -> bool {
        place.access.align.is_some()
            || match &place.kind {
                PlaceKind::Deref(pointer) => self.reduced_pointer(pointer),
                PlaceKind::Field { base, .. }
                | PlaceKind::Lane { base, .. }
                | PlaceKind::ComplexPart { base, .. } => self.reduced_place(base),
                _ => false,
            }
    }

    fn reduced_pointer(&mut self, value: &ir::Value) -> bool {
        match &value.node.value {
            ValueKind::PointerOffset { pointer, .. } => self.reduced_pointer(pointer),
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.reduced_place(place) || self.place_in_packed(place)
            }
            _ => false,
        }
    }

    fn rooted_in_deref(place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Deref(_) | PlaceKind::Index { .. } => true,
            PlaceKind::Field {
                base, bits: None, ..
            }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::ComplexPart { base, .. } => Self::rooted_in_deref(base),
            _ => false,
        }
    }

    pub(super) fn lower_unaligned_read(&mut self, place: &ir::Place) -> Result<Option<Expr>> {
        if !self.reduced_alignment_access(place) || matches!(place.kind, PlaceKind::Lane { .. }) {
            return Ok(None);
        }
        Ok(Some(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::MethodCall {
                recv: Box::new(self.lower_address(place, false)?),
                method: "read_unaligned".into(),
                args: Vec::new(),
            })),
        }))))
    }

    pub(super) fn lower_unaligned_write(
        &mut self,
        place: &ir::Place,
        value: &Expr,
    ) -> Result<Option<Stmt>> {
        if !self.reduced_alignment_access(place) || matches!(place.kind, PlaceKind::Lane { .. }) {
            return Ok(None);
        }
        Ok(Some(Stmt::Unsafe {
            body: rust::Block {
                stmts: vec![Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(self.lower_address(place, true)?),
                    method: "write_unaligned".into(),
                    args: vec![value.clone()],
                })],
                tail: None,
            },
        }))
    }

    fn unaligned_lane<'p>(
        &mut self,
        place: &'p ir::Place,
    ) -> Option<(&'p ir::Place, &'p ir::Value)> {
        let PlaceKind::Lane { base, index } = &place.kind else {
            return None;
        };
        (matches!(base.kind, PlaceKind::Deref(_) | PlaceKind::Index { .. })
            || matches!(base.kind, PlaceKind::Binding(id) if self.dispatch_bindings.contains(&id))
            || self.reduced_alignment_access(base)
            || self.tables.array_subscript(base).is_some()
            || self.place_in_packed(base))
        .then_some((&**base, &**index))
    }

    pub(super) fn lower_lane_read(&mut self, place: &ir::Place) -> Result<Option<Expr>> {
        let Some((base, index)) = self.unaligned_lane(place) else {
            return Ok(None);
        };
        let vector = Expr::MethodCall {
            recv: Box::new(self.lower_address(base, false)?),
            method: "read_unaligned".into(),
            args: Vec::new(),
        };
        Ok(Some(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Index {
                base: Box::new(vector),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.lower_value(index)?),
                    ty: rust::Type::Prim(Prim::Usize),
                }),
            })),
        }))))
    }

    pub(super) fn lower_lane_write(
        &mut self,
        place: &ir::Place,
        value: &Expr,
    ) -> Result<Option<Stmt>> {
        let Some((base, index)) = self.unaligned_lane(place) else {
            return Ok(None);
        };
        let pointer = self.next_temp();
        let vector = self.next_temp();
        let operand = self.next_temp();
        let index = Expr::Cast {
            expr: Box::new(self.lower_value(index)?),
            ty: rust::Type::Prim(Prim::Usize),
        };
        let pointer_var = || Expr::Var(pointer.as_str().into());
        Ok(Some(Stmt::Unsafe {
            body: rust::Block {
                stmts: vec![
                    Stmt::Let {
                        name: operand.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(value.clone()),
                    },
                    Stmt::Let {
                        name: pointer.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(self.lower_address(base, true)?),
                    },
                    Stmt::Let {
                        name: vector.clone(),
                        mutable: true,
                        ty: None,
                        init: Some(Expr::MethodCall {
                            recv: Box::new(pointer_var()),
                            method: "read_unaligned".into(),
                            args: Vec::new(),
                        }),
                    },
                    Stmt::Assign {
                        target: Expr::Index {
                            base: Box::new(Expr::Var(vector.as_str().into())),
                            index: Box::new(index),
                        },
                        value: Expr::Var(operand.as_str().into()),
                    },
                    Stmt::Expr(Expr::MethodCall {
                        recv: Box::new(pointer_var()),
                        method: "write_unaligned".into(),
                        args: vec![Expr::Var(vector.as_str().into())],
                    }),
                ],
                tail: None,
            },
        }))
    }

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
        if let Some((array, index)) = tables.array_subscript(place) {
            return Ok(Expr::Index {
                base: Box::new(self.lower_place(array)?),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.lower_value(index)?),
                    ty: rust::Type::Prim(Prim::Usize),
                }),
            });
        }
        match place.kind {
            PlaceKind::Binding(id) if !tables.register_globals.contains_key(&id) => {
                Ok(self.lower_binding(id))
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
            PlaceKind::ComplexPart {
                ref base,
                imaginary,
            } => {
                self.lower_type(&base.ty)?;
                Ok(Expr::Field {
                    base: Box::new(self.lower_place(base)?),
                    field: if imaginary { "im" } else { "re" }.into(),
                })
            }
            PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment: None,
                ref initializer,
            } => self.lower_compound_literal(object, storage, initializer),
            PlaceKind::Temporary {
                object,
                ref initializer,
            } => self.lower_compound_literal(object, ir::StorageDuration::Automatic, initializer),
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
        if self.dispatch_vlas.contains(&id) {
            return control_flow::vla_storage(id);
        }
        let binding = Expr::Var(binding_name(id, &self.tables.bindings).as_str().into());
        if let Some(record) = self.tables.flexible_globals.get(&id) {
            return Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(binding),
                    }),
                    ty: rust::Type::Ptr {
                        mutable: true,
                        inner: Box::new(rust::Type::Custom(record.clone())),
                    },
                }),
            };
        }
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
                comments: Vec::new(),
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
