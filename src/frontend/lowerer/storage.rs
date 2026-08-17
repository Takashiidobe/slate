use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn unique_local_name(&mut self, base: String) -> String {
        if !self.parent.globals.contains_key(&base)
            && self.declared_local_names.insert(base.clone())
        {
            return base;
        }
        let mut n = 2;
        loop {
            let candidate = format!("{base}{n}");
            if !self.parent.globals.contains_key(&candidate)
                && self.declared_local_names.insert(candidate.clone())
            {
                return candidate;
            }
            n += 1;
        }
    }

    pub(super) fn alloca_group_is_lowerable(&self, ops: &[Op]) -> bool {
        ops.iter().all(|op| {
            let Some((result, cir_ty)) = op.results.first() else {
                return false;
            };
            op.operands.is_empty()
                && !self.forward_allocas.contains(result)
                && !cir_ptr_pointee(cir_ty).is_some_and(|pointee| {
                    is_cir_va_list_record_type(pointee, &self.parent.aliases)
                })
                && !matches!(self.pointee_type(cir_ty), Some(Type::Custom(_)))
        })
    }

    pub(super) fn lower_alloca_group(&mut self, ops: &[Op]) {
        let frame_index = self.parent.generated_alloca_frames.len();
        let frame_name = format!("__SlateAllocaFrame{frame_index}");
        let frame_var = self.unique_local_name(format!("__slate_alloca_frame{frame_index}"));
        let mut fields = Vec::with_capacity(ops.len());
        let mut init = Vec::with_capacity(ops.len());
        let mut places = Vec::with_capacity(ops.len());

        for (field_index, op) in ops.iter().rev().enumerate() {
            let Some((result, cir_ty)) = op.results.first() else {
                return;
            };
            let ty = self.pointee_type(cir_ty).unwrap_or(Type::Prim(Prim::I32));
            let alignment = attr_int(op, "alignment")
                .and_then(|alignment| u32::try_from(alignment).ok())
                .unwrap_or_else(|| type_alignment(&ty));
            let over_aligned = alignment > type_alignment(&ty) && !matches!(ty, Type::Custom(_));
            if over_aligned {
                fields.push(aligned_type(ty.clone(), alignment));
                init.push(aligned_value(
                    self.parent.default_value_expr(&ty),
                    alignment,
                ));
            } else {
                fields.push(ty.clone());
                init.push(self.parent.default_value_expr(&ty));
            }
            let field = Expr::TupleField {
                base: Box::new(Expr::Var(frame_var.clone().into())),
                index: field_index,
            };
            places.push((
                result.clone(),
                ty,
                if over_aligned {
                    Expr::Unary {
                        op: UnaryOp::Deref,
                        expr: Box::new(field),
                    }
                } else {
                    field
                },
            ));
            if over_aligned {
                self.aligned_slots.insert(result.clone());
            }
        }

        self.parent.generated_alloca_frames.push(StructDef {
            attrs: vec![RustAttr::Repr(vec![Repr::C])],
            vis: Visibility::Private,
            field_vis: Visibility::Private,
            generics: Vec::new(),
            name: frame_name.clone(),
            fields: StructFields::Tuple(fields),
        });
        self.push_stmt(Stmt::Let {
            name: frame_var,
            mutable: true,
            ty: Some(Type::Custom(frame_name)),
            init: Some(Expr::TupleStructLit {
                name: self
                    .parent
                    .generated_alloca_frames
                    .last()
                    .expect("generated alloca frame")
                    .name
                    .clone(),
                fields: init,
            }),
        });
        for (result, ty, place) in places {
            self.slot_types.insert(result.clone(), ty);
            self.slot_places.insert(result, place);
        }
    }

    pub(super) fn lower_alloca(&mut self, op: &Op) {
        let Some((result, cir_ty)) = op.results.first() else {
            return;
        };
        // a forwarded compiler temp carries one SSA value: its single store
        // records the value and its single load reads it back, so no local.
        if self.forward_allocas.contains(result) {
            if let Some(ty) = self.pointee_type(cir_ty) {
                self.slot_types.insert(result.clone(), ty);
            }
            return;
        }
        // hoisted allocas were already declared above the dispatch loop
        // (or above a goto-target closure, in structured lowering).
        if self.hoisted.contains(result) {
            if let Some(count) = op.operands.first()
                && let Some(name) = self.values.get(result).and_then(|value| match value {
                    Val::Expr(Expr::MethodCall { recv, method, .. }) if method == "as_mut_ptr" => {
                        match &**recv {
                            Expr::Var(name) => Some(name.as_str().to_string()),
                            _ => None,
                        }
                    }
                    _ => None,
                })
            {
                let ty = self.pointee_type(cir_ty).unwrap_or(Type::Prim(Prim::I32));
                self.push_stmt(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.into())),
                    method: "resize".into(),
                    args: vec![
                        Expr::Cast {
                            expr: Box::new(self.operand_expr(count)),
                            ty: Type::Prim(Prim::Usize),
                        },
                        self.parent.default_value_expr(&ty),
                    ],
                }));
            }
            return;
        }
        let name = self.unique_local_name(
            sanitize_ident(attr_str(op, "name").unwrap_or(result.as_str())).into_string(),
        );
        if let Some(count) = op.operands.first() {
            let ty = self.pointee_type(cir_ty).unwrap_or(Type::Prim(Prim::I32));
            self.values.insert(
                result.clone(),
                Val::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.clone().into())),
                    method: "as_mut_ptr".into(),
                    args: Vec::new(),
                }),
            );
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![ty.clone()],
                }),
                init: Some(if self.hoisting_allocas {
                    Expr::VecLit(Vec::new())
                } else {
                    Expr::VecRepeat {
                        elem: Box::new(self.parent.default_value_expr(&ty)),
                        len: Box::new(Expr::Cast {
                            expr: Box::new(self.operand_expr(count)),
                            ty: Type::Prim(Prim::Usize),
                        }),
                    }
                }),
            });
            return;
        }
        let va_list_pointee = self
            .pointee_type(cir_ty)
            .filter(|ty| matches!(ty, Type::VaList) || is_boxed_va_args_type(ty));
        if self.va_allocas.contains(result) || va_list_pointee.is_some() {
            let ty = va_list_pointee.unwrap_or(if self.parent.va_list_boxed {
                Type::Custom("__SlateVaArgs".into())
            } else {
                Type::VaList
            });
            let boxed = is_boxed_va_args_type(&ty);
            self.slots.insert(result.clone(), name.clone());
            self.va_places
                .insert(result.clone(), Expr::Var(name.clone().into()));
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(ty),
                init: boxed.then(empty_va_args_expr),
            });
            return;
        }
        let mut ty = self.pointee_type(cir_ty).unwrap_or(Type::Prim(Prim::I32));
        if matches!(ty, Type::Prim(_))
            && let Some(enum_name) =
                attr_str(op, "name").and_then(|c_name| self.local_enum_types.get(c_name))
        {
            ty = Type::Custom(enum_name.clone());
        }
        let alignment = attr_int(op, "alignment")
            .and_then(|alignment| u32::try_from(alignment).ok())
            .filter(|alignment| *alignment > effective_type_alignment(&ty, &self.parent.records));
        self.slots.insert(result.clone(), name.clone());
        self.slot_types.insert(result.clone(), ty.clone());
        let init = self.parent.default_value_expr(&ty);
        if let Some(alignment) = alignment {
            self.aligned_slots.insert(result.clone());
            self.slot_places.insert(
                result.clone(),
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var(name.clone().into())),
                },
            );
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(aligned_type(ty, alignment)),
                init: Some(aligned_value(init, alignment)),
            });
            return;
        }
        self.push_stmt(Stmt::Let {
            name,
            mutable: true,
            ty: Some(ty),
            init: Some(init),
        });
    }

    pub(super) fn lower_store(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        if let Some(outputs) = self.asm_outputs.get(&op.operands[0]).cloned() {
            self.asm_outputs.insert(op.operands[1].clone(), outputs);
            return;
        }
        let operand_types = op_operand_types(op);
        let value_ty = operand_types.first();
        let ptr = &op.operands[1];
        let mut value = if value_ty.is_some_and(is_cir_function_pointer_type) {
            self.store_function_pointer_value(&op.operands[0], ptr, value_ty.unwrap())
        } else if value_ty.is_some_and(|ty| matches!(ty, CirType::Ptr(_))) {
            self.pointer_operand_expr(&op.operands[0])
        } else {
            self.operand_expr(&op.operands[0])
        };
        if value_ty.is_some_and(|ty| is_cir_va_list_value_type(ty, &self.parent.aliases)) {
            value = Expr::MethodCall {
                recv: Box::new(value),
                method: "clone".into(),
                args: vec![],
            };
        }
        value = self.coerce_store_value(ptr, value, &op.operands[0]);
        if self.forward_allocas.contains(ptr) {
            let value = self.forward_safe_value(value, value_ty);
            self.forward_values.insert(ptr.clone(), value);
            return;
        }
        if !attr_bool(op, "is_volatile") && self.try_atomic_store(op, ptr, value_ty, value.clone())
        {
            return;
        }
        if attr_bool(op, "is_volatile") {
            self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "write_volatile"].map(Ident::from),
                ))),
                args: vec![self.store_address_expr(ptr), value],
            })));
        } else if let Some(target) = self.place_expr(ptr) {
            if self.ptr_requires_unsafe(ptr) {
                self.push_unsafe_assign(target, value);
            } else {
                self.push_assign(target, value);
            }
        } else {
            self.push_unsafe_assign(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(ptr)),
                },
                value,
            );
        }
    }

    pub(super) fn lower_copy(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let dst = op.operands[0].clone();
        let src = op.operands[1].clone();
        let Some(value) = self.copy_source_value(&dst, &src) else {
            let d = self.pointer_operand_expr(&dst);
            let s = self.pointer_operand_expr(&src);
            self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::PtrCopy {
                src: Box::new(s),
                dst: Box::new(d),
                count: Box::new(Expr::Value(RustValue::I64(1))),
                overlapping: true,
            })));
            return;
        };
        if let Some(target) = self.place_expr(&dst) {
            if self.ptr_requires_unsafe(&dst) {
                self.push_unsafe_assign(target, value);
            } else {
                self.push_assign(target, value);
            }
        } else {
            self.push_unsafe_assign(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(&dst)),
                },
                value,
            );
        }
    }

    /// Resolve the by-value source of a `cir.copy`: a numeric/char const global
    /// renders to an array literal (padded to the destination length), while an
    /// aggregate local relies on the `Copy` derive of arrays and `#[repr(C)]`
    /// structs. Returns `None` when the source is opaque (raw pointer copy).
    pub(super) fn copy_source_value(&self, dst: &str, src: &str) -> Option<Expr> {
        let dst_ty = self.slot_types.get(dst).or_else(|| {
            self.member_ptrs
                .get(dst)
                .and_then(|member| member.field_ty.as_ref())
        });
        let dst_len = dst_ty.and_then(type_array_len).map(|len| len as usize);
        match self.values.get(src) {
            Some(Val::Global(name)) => {
                if let Some(bytes) = self.parent.strings.get(name) {
                    let ty = dst_ty?;
                    let elems = byte_array_elems(bytes, ty);
                    Some(render_array_literal_expr(
                        &elems,
                        dst_len.unwrap_or(elems.len()),
                        Expr::Value(RustValue::I64(0)),
                    ))
                } else if let Some(elems) = self.parent.const_arrays.get(name) {
                    let default = match dst_ty {
                        Some(Type::Array { elem, .. }) => self.parent.default_value_expr(elem),
                        _ => Expr::Value(RustValue::I64(0)),
                    };
                    Some(render_array_literal_expr(
                        elems,
                        dst_len.unwrap_or(elems.len()),
                        default,
                    ))
                } else if let Some(init) = self.parent.const_aggregates.get(name) {
                    let ty = dst_ty?;
                    let mut facts: std::collections::VecDeque<FloatingLiteralFact> = self
                        .parent
                        .global_floating_literals
                        .get(name)
                        .cloned()
                        .unwrap_or_default()
                        .into();
                    self.parent.render_const_value_expr(ty, init, &mut facts)
                } else if self.parent.const_zero_globals.contains(name) {
                    dst_ty.map(|ty| self.parent.default_value_expr(ty))
                } else {
                    None
                }
            }
            _ => self.slot_place(src),
        }
    }

    pub(super) fn lower_load(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        self.load_ptr_operand.insert(result.clone(), ptr.clone());
        if let Some(value) = self.forward_values.get(ptr) {
            self.values.insert(result.clone(), Val::Expr(value.clone()));
            return;
        }
        if let Some(expr) = self.block_addr_dispatch_expr(ptr) {
            self.indirect_target_values.insert(result.clone(), expr);
            self.lower_opaque_pointer(op, true);
            return;
        }
        if op_result_type(op).is_some_and(|ty| is_cir_va_list_value_type(ty, &self.parent.aliases))
            && let Some(place) = self.va_target_place(ptr)
        {
            self.va_places.insert(result.clone(), place.clone());
            self.values.insert(result.clone(), Val::Expr(place));
            return;
        }
        let mut value = if attr_bool(op, "is_volatile") {
            Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "read_volatile"].map(Ident::from),
                ))),
                args: vec![self.load_address_expr(ptr)],
            })
        } else if let Some(atomic) = self.atomic_load_expr(op, ptr) {
            atomic
        } else if let Some(global) = self.global_place(ptr) {
            Self::unsafe_expr(global)
        } else if let Some(member) = self.member_ptrs.get(ptr) {
            let place = Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            };
            if member.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = self.element_place_expr(element);
            if element.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(slot) = self.slot_place(ptr) {
            slot
        } else if op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .is_some_and(|ty| bitint_generic_parts(&ty).is_some())
        {
            Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "read_unaligned"].map(Ident::from),
                ))),
                args: vec![self.operand_expr(ptr)],
            })
        } else {
            Self::unsafe_deref_expr(self.operand_expr(ptr))
        };
        let field_ty = self
            .member_ptrs
            .get(ptr)
            .and_then(|member| member.field_ty.as_ref())
            .or_else(|| self.slot_types.get(ptr));
        if let Some(field_ty) = field_ty
            && let Some(result_ty) = op_result_type(op).map(|ty| self.parent.rust_type(ty))
            && ((self.parent.type_is_enum(field_ty) && matches!(result_ty, Type::Prim(_)))
                || (self.parent.type_is_enum_ptr(field_ty)
                    && matches!(result_ty, Type::Ptr { .. })))
        {
            value = Expr::Cast {
                expr: Box::new(value),
                ty: result_ty,
            };
        }
        if let Some(fn_ptr_ty @ Type::FnPtr { .. }) = field_ty {
            self.loaded_field_types
                .insert(result.clone(), fn_ptr_ty.clone());
            self.materialize_expr_as(result, value, fn_ptr_ty.clone());
        } else {
            self.materialize_expr(result, value, op_result_type(op));
        }
    }

    pub(super) fn block_addr_dispatch_expr(&self, ptr: &str) -> Option<Expr> {
        let element = self.block_addr_element_ptrs.get(ptr)?;
        let dispatch = self.dispatch.as_ref()?;
        let states: Option<Vec<Expr>> = element
            .labels
            .iter()
            .map(|label| {
                dispatch
                    .label_to_state
                    .get(label)
                    .map(|state| Expr::Value(RustValue::I64(*state as i64)))
            })
            .collect();
        Some(Expr::Index {
            base: Box::new(Expr::ArrayLit(states?)),
            index: Box::new(Expr::Cast {
                expr: Box::new(element.index.clone()),
                ty: Type::Prim(Prim::Usize),
            }),
        })
    }

    pub(super) fn load_address_expr(&self, ptr: &str) -> Expr {
        self.address_expr(ptr, false)
    }

    pub(super) fn store_address_expr(&self, ptr: &str) -> Expr {
        self.address_expr(ptr, true)
    }

    pub(super) fn address_expr(&self, ptr: &str, mutable: bool) -> Expr {
        let addr_of = |expr: Expr| Expr::AddrOf {
            mutable,
            expr: Box::new(expr),
        };
        if let Some(member) = self.member_ptrs.get(ptr) {
            let place = addr_of(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            });
            if member.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = addr_of(self.element_place_expr(element));
            if element.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(slot) = self.slot_place(ptr) {
            addr_of(slot)
        } else if let Some(global) = self.global_place(ptr) {
            let place = addr_of(global);
            if matches!(place, Expr::AddrOf { ref expr, .. } if matches!(**expr, Expr::Unary { op: UnaryOp::Deref, .. }))
            {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else {
            self.operand_expr(ptr)
        }
    }

    pub(super) fn global_name(&self, ptr: &str) -> Option<String> {
        let Some(Val::Global(name)) = self.values.get(ptr) else {
            return None;
        };
        let name = sanitize_ident(name).into_string();
        (self.parent.globals.contains_key(&name) || self.parent.extern_globals.contains_key(&name))
            .then_some(name)
    }

    pub(super) fn global_place(&self, ptr: &str) -> Option<Expr> {
        let name = self.global_name(ptr)?;
        let base = Expr::Var(name.clone().into());
        self.parent
            .globals
            .get(&name)
            .and_then(|global| global.alignment)
            .map(|_| Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(base.clone()),
            })
            .or(Some(base))
    }

    pub(super) fn global_array_decay_expr(&self, name: &str, result_ty: &CirType) -> Option<Expr> {
        let name = sanitize_ident(name).into_string();
        let ty = self
            .parent
            .globals
            .get(&name)
            .map(|global| &global.ty)
            .or_else(|| {
                self.parent
                    .extern_globals
                    .get(&name)
                    .map(|global| &global.ty)
            })?;
        let Type::Array { elem, .. } = ty else {
            return None;
        };
        let Type::Ptr { inner, .. } = self.parent.rust_type(result_ty) else {
            return None;
        };
        if inner.as_ref() != elem.as_ref() {
            return None;
        }
        Some(Expr::MethodCallGeneric {
            recv: Box::new(Expr::AddrOf {
                mutable: true,
                expr: Box::new(Expr::Var(name.into())),
            }),
            method: "cast".into(),
            type_args: vec![(**elem).clone()],
            args: Vec::new(),
        })
    }

    pub(super) fn lower_const(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        // MLIR may print a const as an attribute alias (e.g. `#false`); expand it.
        let attr = op
            .attr("value")
            .map(|attr| self.parent.resolve_attr(attr).clone());
        let result_ty = op_result_type(op);
        let const_int = attr.as_ref().and_then(Attr::as_int);

        if let Some(value) = const_int {
            self.const_int_values.insert(result.clone(), value);
        }
        if let Some(value) = const_int
            && let Some(expr) = self.next_layout_query_expr(value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(value) = const_int
            && let Some(expr) = self.next_macro_const_expr(value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(value) = const_int
            && let Some(expr) = self.next_enum_const_expr(op, value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(Attr::ConstVector { elements, .. }) = &attr
            && let Some(value) = const_vector_expr(elements)
        {
            self.materialize_expr(result, value, result_ty);
            return;
        }
        if let Some(Attr::ConstComplex { real, imag, .. }) = &attr
            && let Some(re) = complex_component_from_attr(real)
            && let Some(im) = complex_component_from_attr(imag)
        {
            let rust_ty = result_ty.map(|ty| self.parent.rust_type(ty));
            self.materialize_expr(
                result,
                complex_const_expr(rust_ty.as_ref(), re, im),
                result_ty,
            );
            return;
        }
        if let Some(Attr::GlobalView {
            symbol, indices, ..
        }) = &attr
            && let Some(result_ty) = result_ty
            && let Some(value) = self.parent.global_view_init_expr(
                symbol,
                indices,
                &self.parent.rust_type(result_ty),
            )
        {
            self.materialize_expr(result, value, Some(result_ty));
            return;
        }
        if let Some(b) = attr.as_ref().and_then(Attr::as_bool) {
            self.materialize_expr(result, Expr::Value(RustValue::Bool(b)), result_ty);
            return;
        }
        let floating_literal = self.ast_floating_literal(op);
        if let Some((signed, _)) = result_ty.and_then(parse_cir_int_type)
            && let Type::Prim(prim) = self.parent.rust_type(result_ty.unwrap())
        {
            let value = if signed {
                const_int
                    .filter(|value| i32::try_from(*value).is_err())
                    .map(|value| Expr::Value(RustValue::TypedInt(value, prim)))
            } else {
                attr.as_ref()
                    .and_then(Attr::as_u128)
                    .filter(|value| i32::try_from(*value).is_err())
                    .map(|value| Expr::Value(RustValue::TypedUInt(value, prim)))
            };
            if let Some(value) = value {
                self.materialize_expr(result, value, result_ty);
                return;
            }
        }
        if let Some(Attr::CirInt { text, .. }) = &attr
            && let Some(rust_ty) = result_ty.map(|ty| self.parent.rust_type(ty))
            && let Some(expr) = bitint_from_decimal_str_expr(&rust_ty, text)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        let is_ptr_null = matches!(
            &attr,
            Some(Attr::Dialect { dialect, mnemonic, raw: Some(raw), .. })
                if dialect == "cir" && mnemonic == "ptr" && raw.trim() == "null"
        );
        if is_ptr_null {
            let value = if result_ty.is_some_and(is_cir_function_pointer_type) {
                self.function_pointer_null_values.insert(result.clone());
                Expr::Value(RustValue::None)
            } else if result_ty.is_some_and(|ty| {
                matches!(self.parent.expand_alias(ty), CirType::Ptr(inner) if matches!(**inner, CirType::Void))
            }) {
                Expr::Cast {
                    expr: Box::new(Expr::Value(RustValue::NullPtr)),
                    ty: self.parent.rust_type(result_ty.unwrap()),
                }
            } else {
                Expr::Value(RustValue::NullPtr)
            };
            self.materialize_expr(result, value, result_ty);
            return;
        }
        let is_f128 = matches!(
            result_ty,
            Some(CirType::Float(clang_ir::ast::FloatKind::F128))
        );
        let macro_expr = if is_f128 || result_ty.is_some_and(is_long_double) {
            self.next_long_double_macro_const_expr(op, result_ty)
        } else if matches!(
            result_ty,
            Some(CirType::Float(
                clang_ir::ast::FloatKind::F32 | clang_ir::ast::FloatKind::F64
            ))
        ) {
            self.next_float_macro_const_expr(op, result_ty)
        } else {
            None
        };
        let value = if let Some(expr) = macro_expr {
            expr
        } else if is_f128 || result_ty.is_some_and(is_quad_long_double) {
            attr.as_ref()
                .and_then(|attr| match attr {
                    Attr::CirFloat { text, .. } => f128_from_text(text),
                    _ => None,
                })
                .unwrap_or_else(|| Expr::HexFloat("0.0f128".into()))
        } else if result_ty.is_some_and(is_long_double) {
            if !crate::cir::emit::uses_f64_long_double_abi()
                && let Some(expr) = floating_literal.as_ref().and_then(|fact| {
                    (fact.bit_width == 80)
                        .then(|| f80_literal_bits_expr(&fact.bits))
                        .flatten()
                })
            {
                expr
            } else if let Some(fp) = floating_literal.map(|fact| fact.value) {
                if crate::cir::emit::uses_f64_long_double_abi() {
                    fp_literal_expr(fp)
                } else {
                    typed_fp_literal_expr(Some(&crate::backend::rust_ast::Type::LongDouble), fp)
                }
            } else if let Some(expr) = attr.as_ref().and_then(|attr| match attr {
                Attr::CirFloat { text, .. } => long_double_from_text(text),
                _ => None,
            }) {
                expr
            } else if let Some(n) = const_int {
                Expr::Cast {
                    expr: Box::new(int_value_expr(n)),
                    ty: crate::backend::rust_ast::Type::Prim(Prim::F64),
                }
            } else {
                Expr::Value(0.0.into())
            }
        } else {
            attr.as_ref().and_then(scalar_attr_expr).unwrap_or_else(|| {
                let rust_ty = result_ty.map(|ty| self.parent.rust_type(ty));
                match rust_ty {
                    Some(ty @ Type::Custom(_)) => self.parent.default_value_expr(&ty),
                    _ => Expr::Value(RustValue::I64(0)),
                }
            })
        };
        self.materialize_expr(result, value, result_ty);
    }

    pub(super) fn next_layout_query_expr(
        &mut self,
        value: i128,
        result_ty: Option<&CirType>,
    ) -> Option<Expr> {
        let query = self.layout_queries.front()?;
        let expected = self.parent.layout_query_value(query)?;
        if expected != value {
            return None;
        }
        let expr = self.parent.layout_query_expr(query);
        self.layout_queries.pop_front();
        let mut expr = expr?;
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    pub(super) fn next_macro_const_expr(
        &mut self,
        value: i128,
        result_ty: Option<&CirType>,
    ) -> Option<Expr> {
        let macro_const = self.macro_consts.front()?;
        let known = crate::frontend::macros::lookup(&macro_const.name)?;
        let crate::frontend::macros::MacroValue::Integer { source, rust_path } = known.value else {
            return None;
        };
        if source != value {
            return None;
        }
        self.macro_consts.pop_front();
        let mut expr = Expr::Var(rust_path.into());
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    pub(super) fn next_float_macro_const_expr(
        &mut self,
        op: &Op,
        result_ty: Option<&CirType>,
    ) -> Option<Expr> {
        let macro_const = self.macro_consts.front()?;
        let known = crate::frontend::macros::lookup(&macro_const.name)?;
        if op
            .loc
            .as_deref()
            .and_then(|raw| self.parent.resolve_loc(raw))
            != Some(macro_const.loc)
        {
            return None;
        }
        let is_f32 = matches!(
            result_ty,
            Some(CirType::Float(clang_ir::ast::FloatKind::F32))
        );
        let bits: u64 = match known.value {
            crate::frontend::macros::MacroValue::Float { rust_bits, .. } if is_f32 => {
                u64::from(rust_bits)
            }
            crate::frontend::macros::MacroValue::Double { rust_bits, .. }
                if matches!(
                    result_ty,
                    Some(CirType::Float(clang_ir::ast::FloatKind::F64))
                ) =>
            {
                rust_bits
            }
            _ => return None,
        };
        self.macro_consts.pop_front();
        let func = if is_f32 {
            "f32::from_bits"
        } else {
            "f64::from_bits"
        };
        Some(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(func.into())),
            args: vec![Expr::Value(RustValue::I64(bits as i64))],
        })
    }

    pub(super) fn next_enum_const_expr(
        &mut self,
        op: &Op,
        value: i128,
        result_ty: Option<&CirType>,
    ) -> Option<Expr> {
        let enum_const = self.enum_consts.front()?;
        if enum_const.value as i128 != value {
            return None;
        }
        if op
            .loc
            .as_deref()
            .and_then(|raw| self.parent.resolve_loc(raw))
            != Some(enum_const.loc)
        {
            return None;
        }
        let enum_const = self.enum_consts.pop_front()?;
        let mut expr = Expr::Path(Path::new([
            sanitize_ident(&enum_const.enum_name),
            sanitize_ident(&enum_const.constant_name),
        ]));
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    pub(super) fn next_long_double_macro_const_expr(
        &mut self,
        op: &Op,
        result_ty: Option<&CirType>,
    ) -> Option<Expr> {
        let macro_const = self.macro_consts.front()?;
        let known = crate::frontend::macros::lookup(&macro_const.name)?;
        let crate::frontend::macros::MacroValue::LongDouble {
            rust_bits,
            f80_bytes,
            ..
        } = known.value
        else {
            return None;
        };
        if op
            .loc
            .as_deref()
            .and_then(|raw| self.parent.resolve_loc(raw))
            != Some(macro_const.loc)
        {
            return None;
        }
        self.macro_consts.pop_front();
        if crate::cir::emit::uses_f64_long_double_abi() {
            return Some(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("f64::from_bits".into())),
                args: vec![Expr::Value(RustValue::I64(rust_bits as i64))],
            });
        }
        if matches!(
            result_ty,
            Some(CirType::Float(clang_ir::ast::FloatKind::F128))
        ) || result_ty.is_some_and(is_quad_long_double)
        {
            return Some(Expr::HexFloat(format!(
                "f128::from_bits(0x{:032x})",
                u128::from(rust_bits)
            )));
        }
        let bytes = byte_array_elems(
            &f80_bytes,
            &crate::backend::rust_ast::Type::Array {
                elem: Box::new(crate::backend::rust_ast::Type::Prim(Prim::U8)),
                len: 10,
            },
        );
        Some(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
            args: vec![Expr::ArrayLit(bytes)],
        })
    }
}
