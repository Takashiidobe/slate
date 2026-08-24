use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_get_bitfield(&mut self, op: &inst::GetBitfield) {
        let (place, needs_unsafe) =
            if let Some((storage, field, needs_unsafe)) = self.bitfield_accessor(&op.addr, false) {
                (
                    Expr::MethodCall {
                        recv: Box::new(storage),
                        method: field,
                        args: Vec::new(),
                    },
                    needs_unsafe,
                )
            } else {
                self.bitfield_place(&op.addr)
            };
        let value = if needs_unsafe {
            Self::unsafe_expr(place)
        } else {
            place
        };
        let value = self.truncate_bitfield_expr(&op.bitfield_info, value, Some(&op.result_ty));
        self.materialize_expr(&op.result, value, Some(&op.result_ty));
    }

    pub(super) fn lower_set_bitfield(&mut self, op: &inst::SetBitfield) {
        let value = self.truncate_bitfield_expr(
            &op.bitfield_info,
            self.operand_expr(&op.src),
            Some(&op.result_ty),
        );
        self.materialize_expr(&op.result, value.clone(), Some(&op.result_ty));
        if let Some((storage, field, needs_unsafe)) = self.bitfield_accessor(&op.addr, true) {
            let setter = Expr::MethodCall {
                recv: Box::new(storage),
                method: format!("set_{field}"),
                args: vec![value],
            };
            if needs_unsafe {
                self.push_stmt(Self::unsafe_stmt(Stmt::Expr(setter)));
            } else {
                self.push_stmt(Stmt::Expr(setter));
            }
        } else {
            let (place, needs_unsafe) = self.bitfield_place(&op.addr);
            if needs_unsafe {
                self.push_unsafe_assign(place, value);
            } else {
                self.push_assign(place, value);
            }
        }
    }

    pub(super) fn lower_get_element(&mut self, op: &inst::GetElement) {
        let index_expr = self.operand_expr(&op.index);
        let unbounded = self.member_ptrs.get(&op.base).is_some_and(|member| {
            member.field_is_trailing
                && matches!(&member.field_ty, Some(Type::Array { len: 0 | 1, .. }))
        });
        let array_len = self
            .value_type(&op.base)
            .and_then(CirType::pointee)
            .and_then(CirType::as_array)
            .map(|(_, len)| len);
        let out_of_bounds = array_len.is_some_and(|len| {
            self.known_arith_value(&op.index)
                .is_some_and(|value| value >= i128::from(len))
        });
        let elem_ty = op.result_ty.pointee().map(|ty| self.parent.rust_type(ty));
        if let Some(Val::Global(name)) = self.values.get(&op.base).cloned() {
            if let Some(labels) = self.parent.block_addr_globals.get(&name) {
                self.block_addr_element_ptrs.insert(
                    op.result.clone(),
                    BlockAddrElementPtr {
                        labels: labels.clone(),
                        index: index_expr.clone(),
                    },
                );
            }
            let declared_len = array_len.map(|len| len as usize);
            if let Some(base_expr) =
                self.global_array_literal_expr(&name, elem_ty.clone(), declared_len)
            {
                self.element_ptrs.insert(
                    op.result.clone(),
                    ElementPtr {
                        base: base_expr,
                        index: index_expr,
                        unsafe_access: false,
                        unbounded: false,
                        out_of_bounds: false,
                        elem_ty,
                    },
                );
                return;
            }
        }
        let base_expr = self.place_or_deref_expr(&op.base);
        let unsafe_access =
            unbounded || self.place_expr(&op.base).is_none() || self.ptr_requires_unsafe(&op.base);
        self.element_ptrs.insert(
            op.result.clone(),
            ElementPtr {
                base: base_expr,
                index: index_expr,
                unsafe_access,
                unbounded,
                out_of_bounds,
                elem_ty,
            },
        );
    }

    pub(super) fn lower_ptr_stride(&mut self, op: &inst::PtrStride) {
        let base_ty = self.value_type(&op.base);
        let function_pointer_stride = base_ty.is_some_and(is_cir_function_pointer_type)
            && is_cir_function_pointer_type(&op.result_ty);
        let base_expr = self.fn_ptr_aware_operand_expr(
            &op.base,
            function_pointer_stride.then_some(base_ty).flatten(),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let (method, args) =
            self.ptr_stride_method_and_args(&op.stride, self.operand_expr(&op.stride));
        let stride_expr = Self::unsafe_expr(Expr::MethodCall {
            recv: Box::new(base_expr),
            method,
            args,
        });
        let value = if function_pointer_stride {
            Expr::Transmute {
                from: Type::Ptr {
                    mutable: false,
                    inner: Box::new(Type::Unit),
                },
                to: self.parent.rust_type(&op.result_ty),
                expr: Box::new(stride_expr),
            }
        } else {
            stride_expr
        };
        self.materialize_expr(&op.result, value, Some(&op.result_ty));
    }

    pub(super) fn lower_ptr_diff(&mut self, op: &inst::PtrDiff) {
        let lhs = self.fn_ptr_aware_operand_expr(
            &op.lhs,
            self.value_type(&op.lhs),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let rhs = self.fn_ptr_aware_operand_expr(
            &op.rhs,
            self.value_type(&op.rhs),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let value = Self::unsafe_expr(Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(lhs),
                method: "offset_from".into(),
                args: vec![rhs],
            }),
            ty: self.parent.rust_type(&op.result_ty),
        });
        self.materialize_expr(&op.result, value, Some(&op.result_ty));
    }

    pub(super) fn lower_cast(&mut self, op: &inst::Cast) {
        let result = &op.result;
        let result_ty = &op.result_ty;
        let src = &op.src;
        let Some(src_ty) = self.value_type(src).cloned() else {
            return;
        };
        let src_ty = &src_ty;
        if fenv_is_constrained(&op.fenv) && self.try_lower_fenv_cast(result, result_ty, src, src_ty)
        {
            return;
        }
        if (is_cir_va_list_value_type(result_ty, &self.parent.aliases)
            || is_cir_va_list_value_type(src_ty, &self.parent.aliases))
            && let Some(place) = self.va_target_place(src)
        {
            self.va_places.insert(result.clone(), place.clone());
            self.values.insert(result.clone(), Val::Expr(place));
            return;
        }
        if let Some(operand_record) = src_ty.pointee().and_then(slate_record_name)
            && is_abi_coercion_record_name(operand_record)
            && let Some(real_record) = result_ty.pointee().and_then(slate_record_name)
            && !is_abi_coercion_record_name(real_record)
        {
            self.coerce_alloca_real_type
                .insert(src.clone(), (src.clone(), real_record.to_string()));
        } else if let Some(result_record) = result_ty.pointee().and_then(slate_record_name)
            && is_abi_coercion_record_name(result_record)
            && let Some(real_record) = src_ty.pointee().and_then(slate_record_name)
            && !is_abi_coercion_record_name(real_record)
        {
            self.coerce_alloca_real_type
                .insert(result.clone(), (src.clone(), real_record.to_string()));
        }
        let result_rust_ty = self.parent.rust_type(result_ty);
        let src_rust_ty = self.parent.rust_type(src_ty);
        if matches!(result_ty, CirType::Bool) && is_cir_function_pointer_type(src_ty) {
            self.materialize_expr(
                result,
                Expr::MethodCall {
                    recv: Box::new(self.function_pointer_operand_expr(src)),
                    method: "is_some".into(),
                    args: Vec::new(),
                },
                Some(result_ty),
            );
            return;
        }
        if matches!(result_ty, CirType::Bool) && !matches!(src_ty, CirType::Bool) {
            let value = if matches!(src_ty, CirType::Pointer { .. }) {
                self.pointer_operand_expr(src)
            } else {
                self.operand_expr(src)
            };
            self.materialize_expr(
                result,
                Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(value),
                    rhs: Box::new(zero_for_cir_type(src_ty)),
                },
                Some(result_ty),
            );
            return;
        }
        if is_cir_function_pointer_type(result_ty) && is_cir_function_pointer_type(src_ty) {
            if let Some(Val::Global(fn_name)) = self.values.get(src).cloned()
                && !self.parent.strings.contains_key(&fn_name)
            {
                let raw_ptr = Type::Ptr {
                    mutable: false,
                    inner: Box::new(Type::Unit),
                };
                let expr = Expr::Transmute {
                    from: raw_ptr.clone(),
                    to: result_rust_ty.clone(),
                    expr: Box::new(Expr::Cast {
                        expr: Box::new(Expr::Var(sanitize_ident(&fn_name))),
                        ty: raw_ptr,
                    }),
                };
                self.materialize_expr(result, expr, Some(result_ty));
                return;
            }
            let from = self
                .loaded_field_types
                .get(src)
                .cloned()
                .unwrap_or_else(|| src_rust_ty.clone());
            let value = self.function_pointer_operand_expr(src);
            let expr = if from == result_rust_ty {
                value
            } else {
                Expr::Transmute {
                    from,
                    to: result_rust_ty.clone(),
                    expr: Box::new(value),
                }
            };
            self.materialize_expr(result, expr, Some(result_ty));
            return;
        }
        if matches!(result_ty, CirType::Pointer { .. }) && is_cir_function_pointer_type(src_ty) {
            self.materialize_expr(
                result,
                Expr::Transmute {
                    from: src_rust_ty.clone(),
                    to: result_rust_ty.clone(),
                    expr: Box::new(self.function_pointer_operand_expr(src)),
                },
                Some(result_ty),
            );
            return;
        }
        if is_cir_function_pointer_type(src_ty) && !matches!(result_ty, CirType::Pointer { .. }) {
            self.materialize_expr(
                result,
                Expr::Cast {
                    expr: Box::new(Expr::Transmute {
                        from: src_rust_ty.clone(),
                        to: Type::Prim(Prim::Usize),
                        expr: Box::new(self.function_pointer_operand_expr(src)),
                    }),
                    ty: result_rust_ty.clone(),
                },
                Some(result_ty),
            );
            return;
        }
        if is_cir_function_pointer_type(result_ty) {
            let value = if matches!(src_ty, CirType::Pointer { .. }) {
                self.pointer_operand_expr(src)
            } else {
                self.operand_expr(src)
            };
            self.materialize_expr(
                result,
                Expr::Transmute {
                    from: Type::Prim(Prim::Usize),
                    to: result_rust_ty.clone(),
                    expr: Box::new(Expr::Cast {
                        expr: Box::new(value),
                        ty: Type::Prim(Prim::Usize),
                    }),
                },
                Some(result_ty),
            );
            return;
        }
        if bitint_generic_parts(&result_rust_ty).is_some()
            && bitint_generic_parts(&src_rust_ty).is_none()
            && resolved_integer_parts(src_ty, &self.parent.aliases).is_some()
        {
            let (signed, _) = resolved_integer_parts(src_ty, &self.parent.aliases).unwrap();
            let value = bitint_from_int_expr(&result_rust_ty, self.operand_expr(src), signed)
                .expect("checked bitint result type");
            self.materialize_expr(result, value, Some(result_ty));
            return;
        }
        if bitint_generic_parts(&src_rust_ty).is_some()
            && bitint_generic_parts(&result_rust_ty).is_none()
            && resolved_integer_parts(result_ty, &self.parent.aliases).is_some()
        {
            let (wide, _) = bitint_to_int_expr(&src_rust_ty, self.operand_expr(src))
                .expect("checked bitint source type");
            self.materialize_expr(
                result,
                Expr::Cast {
                    expr: Box::new(wide),
                    ty: result_rust_ty.clone(),
                },
                Some(result_ty),
            );
            return;
        }
        if is_wrapped_long_double(result_ty) && !is_long_double(src_ty) {
            let Some(shim) = f80_cast_from_name(&src_rust_ty) else {
                self.emit_todo("long double cast");
                return;
            };
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![self.operand_expr(src)],
                },
                Some(result_ty),
            );
            return;
        }
        if is_wrapped_long_double(src_ty) && !is_long_double(result_ty) {
            let Some(shim) = f80_cast_to_name(&result_rust_ty) else {
                self.emit_todo("long double cast");
                return;
            };
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![self.operand_expr(src)],
                },
                Some(result_ty),
            );
            return;
        }
        if let Some(Val::Global(name)) = self.values.get(src).cloned() {
            let string_bytes = self.parent.strings.get(&name).cloned().or_else(|| {
                let rust_name = sanitize_ident(&name);
                self.parent.strings.iter().find_map(|(candidate, bytes)| {
                    (sanitize_ident(candidate) == rust_name).then(|| bytes.clone())
                })
            });
            if matches!(result_ty, CirType::Pointer { .. })
                && let Some(bytes) = string_bytes
            {
                self.materialize_expr(
                    result,
                    Expr::Cast {
                        expr: Box::new(Expr::MethodCall {
                            recv: Box::new(Expr::ByteStr(bytes)),
                            method: "as_ptr".into(),
                            args: Vec::new(),
                        }),
                        ty: result_rust_ty.clone(),
                    },
                    Some(result_ty),
                );
                return;
            }
            if let Some(expr) = self.global_array_decay_expr(&name, result_ty) {
                self.materialize_expr(result, expr, Some(result_ty));
                return;
            }
            if matches!(result_ty, CirType::Pointer { .. })
                && !is_cir_function_pointer_type(result_ty)
            {
                self.materialize_expr(
                    result,
                    Expr::Cast {
                        expr: Box::new(Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(Expr::Var(sanitize_ident(&name))),
                        }),
                        ty: result_rust_ty.clone(),
                    },
                    Some(result_ty),
                );
                return;
            }
        }
        let value = if matches!(src_ty, CirType::Pointer { .. }) {
            self.pointer_operand_expr(src)
        } else {
            self.operand_expr(src)
        };
        let from = self.parent.rust_type(src_ty);
        let to = self.parent.rust_type(result_ty);
        let expr = if from == to {
            value
        } else if let Type::Array { elem, len } = &to
            && let Some(lane_bits) = bitint_vector_lane_bits(elem)
            && packed_mask_int_type(lane_bits * *len as u32).is_some_and(|packed| packed == from)
            && let Type::Prim(int_prim) = from
        {
            unpack_bitint_vector_expr(value, elem, *len as usize, lane_bits, int_prim)
        } else if let Type::Array { elem, len } = &from
            && let Some(lane_bits) = bitint_vector_lane_bits(elem)
            && packed_mask_int_type(lane_bits * *len as u32).is_some_and(|packed| packed == to)
            && let Type::Prim(int_prim) = to
        {
            pack_bitint_vector_expr(value, *len as usize, lane_bits, int_prim)
        } else if matches!(to, Type::Array { .. }) || matches!(from, Type::Array { .. }) {
            Expr::Transmute {
                from,
                to,
                expr: Box::new(value),
            }
        } else {
            Expr::Cast {
                expr: Box::new(value),
                ty: to,
            }
        };
        self.materialize_expr(result, expr, Some(result_ty));
    }

    fn try_lower_fenv_cast(
        &mut self,
        result: &str,
        result_ty: &CirType,
        src: &str,
        src_ty: &CirType,
    ) -> bool {
        let src_expr = self.operand_expr(src);
        let call = |name: String, arg: Expr| Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(name.into())),
            args: vec![arg],
        };
        let expr = match (src_ty, result_ty) {
            (CirType::Single, CirType::Double) => call("__slate_fenv_f32_to_f64".into(), src_expr),
            (CirType::Double, CirType::Single) => call("__slate_fenv_f64_to_f32".into(), src_expr),
            (CirType::Single, CirType::Bool) => call("__slate_fenv_f32_to_bool".into(), src_expr),
            (CirType::Double, CirType::Bool) => call("__slate_fenv_f64_to_bool".into(), src_expr),
            (CirType::Bool, CirType::Single) => call(
                "__slate_fenv_i64_to_f32".into(),
                Expr::Cast {
                    expr: Box::new(src_expr),
                    ty: Type::Prim(Prim::I64),
                },
            ),
            (CirType::Bool, CirType::Double) => call(
                "__slate_fenv_i64_to_f64".into(),
                Expr::Cast {
                    expr: Box::new(src_expr),
                    ty: Type::Prim(Prim::I64),
                },
            ),
            (src, dest)
                if let Some((signed, _)) = resolved_integer_parts(src, &self.parent.aliases)
                    && let Some(float_bits) = fenv_scalar_bits(dest) =>
            {
                let carrier = if signed { Prim::I64 } else { Prim::U64 };
                let carrier_name = if signed { "i64" } else { "u64" };
                let shim = format!("__slate_fenv_{carrier_name}_to_f{float_bits}");
                call(
                    shim,
                    Expr::Cast {
                        expr: Box::new(src_expr),
                        ty: Type::Prim(carrier),
                    },
                )
            }
            (src, dest)
                if let Some(float_bits) = fenv_scalar_bits(src)
                    && let Some((signed, _)) =
                        resolved_integer_parts(dest, &self.parent.aliases) =>
            {
                let carrier_name = if signed { "i64" } else { "u64" };
                let shim = format!("__slate_fenv_f{float_bits}_to_{carrier_name}");
                let carrier_ty = self.parent.rust_type(dest);
                Expr::Cast {
                    expr: Box::new(call(shim, src_expr)),
                    ty: carrier_ty,
                }
            }
            _ => return false,
        };
        self.parent.uses_fenv_shims.set(true);
        self.materialize_expr(result, expr, Some(result_ty));
        true
    }

    pub(super) fn place_expr(&self, ptr: &str) -> Option<Expr> {
        if let Some(member) = self.member_ptrs.get(ptr) {
            Some(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            })
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            Some(self.element_place_expr(element))
        } else if let Some(slot) = self.slot_place(ptr) {
            Some(slot)
        } else {
            self.global_place(ptr)
        }
    }

    pub(super) fn va_target_place(&self, ptr: &str) -> Option<Expr> {
        self.va_places
            .get(ptr)
            .cloned()
            .or_else(|| Some(self.place_or_deref_expr(ptr)))
    }

    pub(super) fn place_or_deref_expr(&self, ptr: &str) -> Expr {
        if self.aligned_slots.contains(ptr)
            && let Some(place) = self.slot_receiver(ptr)
        {
            return place;
        }
        self.place_expr(ptr).unwrap_or_else(|| Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(self.pointer_operand_expr(ptr)),
        })
    }

    pub(super) fn lower_get_member(&mut self, op: &inst::GetMember) {
        let index = op
            .index_attr
            .as_int()
            .and_then(|index| usize::try_from(index).ok());
        self.member_base_operand
            .insert(op.result.clone(), op.addr.clone());
        if let Some(outputs) = self.asm_outputs.get(&op.addr)
            && let Some(output) = index.and_then(|index| outputs.get(index))
        {
            self.forward_values
                .insert(op.result.clone(), output.clone());
            return;
        }
        let base = self.place_or_deref_expr(&op.addr);
        let logical_field = if op.name.is_empty() {
            index
                .map(|index| format!("__slate_anon_{index}"))
                .unwrap_or_else(|| sanitize_ident(&op.result).into_string())
        } else {
            sanitize_ident(&op.name).into_string()
        };
        let storage = self.bitfield_storage_member(&op.addr, &op.result_ty, index);
        let field = storage
            .as_ref()
            .map(|(field, _, _)| field.clone())
            .unwrap_or_else(|| logical_field.clone());
        let field_ty = storage
            .as_ref()
            .map(|(_, ty, _)| ty.clone())
            .or_else(|| self.member_field_type(&op.addr, &logical_field))
            .or_else(|| {
                self.record_name_from_base_type(&op.addr)
                    .and_then(|record| self.record_field_type_by_name(&record, &logical_field))
            });
        let unsafe_access = self.place_expr(&op.addr).is_none()
            || self.ptr_requires_unsafe(&op.addr)
            || self
                .value_type(&op.addr)
                .and_then(CirType::pointee)
                .is_some_and(|ty| self.parent.cir_type_is_union(ty));
        let field_is_trailing = self.member_field_is_trailing(&op.addr, &field);
        self.member_ptrs.insert(
            op.result.clone(),
            MemberPtr {
                base,
                field,
                field_ty,
                unsafe_access,
                bitfield_name: storage
                    .as_ref()
                    .and_then(|(_, _, wrapped)| wrapped.then_some(logical_field)),
                field_is_trailing,
            },
        );
    }

    fn bitfield_storage_member(
        &self,
        base_ptr: &str,
        result_ty: &CirType,
        index: Option<usize>,
    ) -> Option<(String, Type, bool)> {
        let record_name = self
            .value_type(base_ptr)
            .and_then(CirType::pointee)
            .map(|ty| self.parent.expand_alias(ty))
            .and_then(slate_record_name)?;
        let record_name = sanitize_ident(record_name).into_string();
        let record = self.parent.records.get(&record_name)?;
        let fields = self.parent.bitfield_storage_fields(record)?;
        let index = index?;
        let field = fields.get(index)?;
        let wrapper = self
            .parent
            .bitfield_storages
            .get(&(record_name, index))
            .map(|storage| Type::Custom(storage.wrapper.clone()));
        let ty = wrapper
            .clone()
            .or_else(|| result_ty.pointee().map(|ty| self.parent.rust_type(ty)))?;
        Some((
            sanitize_ident(&field.name).into_string(),
            ty,
            wrapper.is_some(),
        ))
    }

    fn member_field_is_trailing(&self, base_ptr: &str, field: &str) -> bool {
        let Some(record_name) = self
            .member_record_name(base_ptr)
            .or_else(|| self.record_name_from_base_type(base_ptr))
        else {
            return false;
        };
        let Some(record) = self.parent.records.get(&record_name) else {
            return false;
        };
        if record.kind == crate::frontend::c_ast::RecordKind::Union {
            return true;
        }
        record
            .fields
            .last()
            .is_some_and(|last| sanitize_ident(&last.name).as_str() == field)
    }

    pub(super) fn lower_extract_member(&mut self, op: &inst::ExtractMember) {
        let Some(index) = op
            .index
            .as_int()
            .and_then(|index| usize::try_from(index).ok())
        else {
            self.emit_todo("cir.extract_member");
            return;
        };
        let Some(field) = self.value_member_field(&op.record, index) else {
            self.emit_todo("cir.extract_member");
            return;
        };
        self.materialize_expr(
            &op.result,
            Expr::Field {
                base: Box::new(self.operand_expr(&op.record)),
                field,
            },
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_insert_member(&mut self, op: &inst::InsertMember) {
        let Some(index) = op
            .index
            .as_int()
            .and_then(|index| usize::try_from(index).ok())
        else {
            self.emit_todo("cir.insert_member");
            return;
        };
        let Some(record_name) =
            Some(self.parent.rust_type(&op.result_ty)).and_then(|ty| match ty {
                Type::Custom(name) => Some(name),
                _ => None,
            })
        else {
            self.emit_todo("cir.insert_member");
            return;
        };
        let Some(record) = self.parent.records.get(&record_name) else {
            self.emit_todo("cir.insert_member");
            return;
        };
        if record.kind != RecordKind::Struct {
            self.emit_todo("cir.insert_member");
            return;
        }
        if index >= record.fields.len() {
            self.emit_todo("cir.insert_member");
            return;
        }
        let base = self.operand_expr(&op.record);
        let value = self.operand_expr(&op.value);
        let fields = record
            .fields
            .iter()
            .enumerate()
            .map(|(i, field)| {
                let field = sanitize_ident(&field.name).into_string();
                let expr = if i == index {
                    value.clone()
                } else {
                    Expr::Field {
                        base: Box::new(base.clone()),
                        field: field.clone(),
                    }
                };
                (field, expr)
            })
            .collect();
        self.materialize_expr(
            &op.result,
            wrap_record_lit(
                record,
                Expr::StructLit {
                    name: record_lit_name(record),
                    fields,
                },
            ),
            Some(&op.result_ty),
        );
    }

    pub(super) fn value_member_field(&self, record: &str, index: usize) -> Option<String> {
        let record_ty = self.value_type(record)?;
        let Type::Custom(record_name) = self.parent.rust_type(record_ty) else {
            return None;
        };
        self.parent
            .records
            .get(&record_name)?
            .fields
            .get(index)
            .map(|field| sanitize_ident(&field.name).into_string())
    }

    pub(super) fn coerce_store_value(&self, ptr: &str, value: Expr, value_operand: &str) -> Expr {
        if self
            .slot_types
            .get(ptr)
            .is_some_and(|ty| matches!(ty, Type::FnPtr { .. }))
            && self.is_function_pointer_none_expr(&value)
        {
            return Expr::Value(RustValue::None);
        }
        let field_ty = self
            .member_ptrs
            .get(ptr)
            .and_then(|member| member.field_ty.as_ref())
            .or_else(|| self.slot_types.get(ptr));
        let Some(field_ty) = field_ty else {
            return value;
        };
        if let Type::Custom(enum_name) = field_ty
            && let Some(enm) = self.parent.enums.get(enum_name)
        {
            if let Some(value_int) = self.store_int_value(&value, value_operand)
                && let Some(variant) = enm
                    .variants
                    .iter()
                    .find(|variant| i128::from(variant.value) == value_int)
            {
                return Expr::Path(Path::new([
                    Ident::from(enum_name.as_str()),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ]));
            }
            return Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "mem", "transmute"].map(Ident::from),
                ))),
                args: vec![value],
            });
        }
        if self.parent.type_is_enum_ptr(field_ty) {
            return Expr::Cast {
                expr: Box::new(value),
                ty: field_ty.clone(),
            };
        }
        value
    }

    pub(super) fn store_int_value(&self, value: &Expr, value_operand: &str) -> Option<i128> {
        expr_int_value(value).or_else(|| {
            if let Some(value) = self.const_int_values.get(value_operand) {
                return Some(*value);
            }
            let Val::Expr(expr) = self.values.get(value_operand)? else {
                return None;
            };
            expr_int_value(expr)
        })
    }

    pub(super) fn member_record_name(&self, base_ptr: &str) -> Option<String> {
        if let Some(Type::Custom(record_name)) = self.slot_types.get(base_ptr) {
            return Some(record_name.clone());
        }
        if let Some(Type::Custom(record_name)) = self
            .member_ptrs
            .get(base_ptr)
            .and_then(|member| member.field_ty.as_ref())
        {
            return Some(record_name.clone());
        }
        if let Some(Type::Custom(record_name)) = self
            .element_ptrs
            .get(base_ptr)
            .and_then(|element| element.elem_ty.as_ref())
        {
            return Some(record_name.clone());
        }
        None
    }

    fn record_name_from_base_type(&self, base_ptr: &str) -> Option<String> {
        let record_name = self
            .value_type(base_ptr)
            .and_then(CirType::pointee)
            .map(|ty| self.parent.expand_alias(ty))
            .and_then(slate_record_name)?;
        Some(sanitize_ident(record_name).into_string())
    }

    pub(super) fn member_field_type(&self, base_ptr: &str, field: &str) -> Option<Type> {
        let record_name = self.member_record_name(base_ptr)?;
        self.record_field_type_by_name(&record_name, field)
    }

    pub(super) fn record_field_type_by_name(&self, record_name: &str, field: &str) -> Option<Type> {
        let record = self.parent.records.get(record_name)?;
        let index = record
            .fields
            .iter()
            .position(|candidate| sanitize_ident(&candidate.name).as_str() == field)?;
        let candidate = record.fields.get(index)?;
        if matches!(candidate.ty, CType::FuncPtr { .. })
            && let Some(field_ty) = self
                .parent
                .cir_record_field_types(record)
                .and_then(|types| types.get(index).cloned())
        {
            return Some(field_ty);
        }
        Some(self.parent.record_field_type(&candidate.ty))
    }

    pub(super) fn bitfield_place(&self, ptr: &str) -> (Expr, bool) {
        match self.place_expr(ptr) {
            Some(place) => (place, self.ptr_requires_unsafe(ptr)),
            None => (
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(ptr)),
                },
                true,
            ),
        }
    }

    fn bitfield_accessor(&self, ptr: &str, mutable: bool) -> Option<(Expr, String, bool)> {
        let member = self.member_ptrs.get(ptr)?;
        let field = member.bitfield_name.clone()?;
        let base_is_global = self.ptr_has_global_origin(ptr);
        let base = if base_is_global && !mutable {
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "read_volatile"].map(Ident::from),
                ))),
                args: vec![Expr::Macro {
                    name: "std::ptr::addr_of".into(),
                    args: vec![Expr::Field {
                        base: Box::new(member.base.clone()),
                        field: member.field.clone(),
                    }],
                }],
            }
        } else if base_is_global {
            Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::Unary {
                    op: UnaryOp::Raw(if mutable { Raw::Mut } else { Raw::Const }),
                    expr: Box::new(member.base.clone()),
                }),
            }
        } else {
            member.base.clone()
        };
        let storage = if base_is_global && !mutable {
            base
        } else {
            Expr::Field {
                base: Box::new(base),
                field: member.field.clone(),
            }
        };
        Some((storage, field, self.ptr_requires_unsafe(ptr)))
    }

    // shift up then arithmetic-shift down masks to `size` bits, sign-extending signed types.
    pub(super) fn truncate_bitfield_expr(
        &self,
        bitfield_info: &Attr,
        expr: Expr,
        ty: Option<&CirType>,
    ) -> Expr {
        let rust_ty = ty.map(|ty| self.parent.rust_type(ty));
        let bits = rust_ty.as_ref().and_then(|t| int_bits(&t.render()));
        match (self.bitfield_size(bitfield_info), bits) {
            (Some(size), Some(bits)) if size < bits => {
                // the storage field's own type may have different signedness (or, for a
                // shared multi-field storage unit, no fixed signedness at all) than this
                // particular logical bitfield, so reinterpret to the logical type first -
                // same-width `as` casts preserve the bit pattern, matching C semantics.
                let expr = Expr::Cast {
                    expr: Box::new(expr),
                    ty: rust_ty.expect("checked by `bits` above"),
                };
                let sh = Box::new(Expr::Value(RustValue::I64((bits - size) as i64)));
                Expr::Binary {
                    op: BinOp::Shr,
                    lhs: Box::new(Expr::Binary {
                        op: BinOp::Shl,
                        lhs: Box::new(expr),
                        rhs: sh.clone(),
                    }),
                    rhs: sh,
                }
            }
            _ => expr,
        }
    }

    pub(super) fn bitfield_size(&self, bitfield_info: &Attr) -> Option<u32> {
        self.bitfield_size_offset(bitfield_info)
            .map(|(size, _)| size)
    }

    pub(super) fn bitfield_size_offset(&self, bitfield_info: &Attr) -> Option<(u32, u32)> {
        let attr = self.parent.resolve_attr(bitfield_info);
        let Attr::BitfieldInfo { size, offset, .. } = attr else {
            return None;
        };
        Some((u32::try_from(*size).ok()?, u32::try_from(*offset).ok()?))
    }
    pub(super) fn global_array_literal_expr(
        &self,
        name: &str,
        elem_ty: Option<Type>,
        declared_len: Option<usize>,
    ) -> Option<Expr> {
        if let Some(bytes) = self.parent.strings.get(name) {
            let elem_ty = elem_ty.unwrap_or(Type::Prim(Prim::I8));
            let len = declared_len.unwrap_or(bytes.len());
            let elems = byte_array_elems(
                bytes,
                &Type::Array {
                    elem: Box::new(elem_ty),
                    len: len as u64,
                },
            );
            Some(render_array_literal_expr(
                &elems,
                len,
                Expr::Value(RustValue::I64(0)),
            ))
        } else if let Some(elems) = self.parent.const_arrays.get(name) {
            let len = declared_len.unwrap_or(elems.len());
            let default = elem_ty
                .as_ref()
                .map(|ty| self.parent.default_value_expr(ty))
                .unwrap_or(Expr::Value(RustValue::I64(0)));
            Some(render_array_literal_expr(elems, len, default))
        } else {
            None
        }
    }

    pub(super) fn ptr_requires_unsafe(&self, ptr: &str) -> bool {
        self.global_name(ptr).is_some()
            || self
                .member_ptrs
                .get(ptr)
                .is_some_and(|member| member.unsafe_access)
            || self
                .element_ptrs
                .get(ptr)
                .is_some_and(|element| element.unsafe_access)
    }

    fn ptr_has_global_origin(&self, ptr: &str) -> bool {
        let mut ptr = ptr;
        for _ in 0..64 {
            if self.global_name(ptr).is_some() {
                return true;
            }
            let Some(base) = self.member_base_operand.get(ptr) else {
                return false;
            };
            ptr = base;
        }
        false
    }

    pub(super) fn ptr_stride_method_and_args(
        &self,
        stride: &str,
        index: Expr,
    ) -> (String, Vec<Expr>) {
        if let Some(value) = self.const_int_values.get(stride)
            && *value >= 0
        {
            return ("add".into(), vec![int_value_expr(*value)]);
        }
        if self
            .value_type(stride)
            .is_some_and(|ty| self.cir_int_is_unsigned(ty))
        {
            return (
                "add".into(),
                vec![Expr::Cast {
                    expr: Box::new(index),
                    ty: Type::Prim(Prim::Usize),
                }],
            );
        }
        (
            "offset".into(),
            vec![Expr::Cast {
                expr: Box::new(index),
                ty: Type::Prim(Prim::Isize),
            }],
        )
    }

    pub(super) fn cir_int_is_unsigned(&self, ty: &CirType) -> bool {
        let resolved = self.parent.expand_alias(ty);
        resolved.as_integer().is_some_and(|(signed, _, _)| !signed)
    }
}

pub(super) fn bitint_vector_lane_bits(elem_ty: &Type) -> Option<u32> {
    let (_, bits, _, _) = bitint_generic_parts(elem_ty)?;
    bits.parse().ok()
}

pub(super) fn packed_mask_int_type(bits: u32) -> Option<Type> {
    Some(match bits {
        8 => Type::Prim(Prim::U8),
        16 => Type::Prim(Prim::U16),
        32 => Type::Prim(Prim::U32),
        64 => Type::Prim(Prim::U64),
        128 => Type::Prim(Prim::U128),
        _ => return None,
    })
}

pub(super) fn pack_bitint_vector_expr(
    value: Expr,
    len: usize,
    lane_bits: u32,
    int_prim: Prim,
) -> Expr {
    (0..len)
        .map(|i| {
            let lane = Expr::MethodCall {
                recv: Box::new(Expr::Index {
                    base: Box::new(value.clone()),
                    index: Box::new(Expr::Value(RustValue::Usize(i))),
                }),
                method: "to_u128".into(),
                args: Vec::new(),
            };
            Expr::Binary {
                op: BinOp::Shl,
                lhs: Box::new(Expr::Cast {
                    expr: Box::new(lane),
                    ty: Type::Prim(int_prim),
                }),
                rhs: Box::new(Expr::Value(RustValue::TypedUInt(
                    (i as u32 * lane_bits) as u128,
                    int_prim,
                ))),
            }
        })
        .reduce(|acc, bit| Expr::Binary {
            op: BinOp::BitOr,
            lhs: Box::new(acc),
            rhs: Box::new(bit),
        })
        .unwrap_or(Expr::Value(RustValue::TypedUInt(0, int_prim)))
}

fn unpack_bitint_vector_expr(
    value: Expr,
    elem_ty: &Type,
    len: usize,
    lane_bits: u32,
    int_prim: Prim,
) -> Expr {
    let (name, bits, limbs, bytes) =
        bitint_generic_parts(elem_ty).expect("checked bitint-vector element type");
    let ctor_path = format!("{name}::<{bits}, {limbs}, {bytes}>::from_u128");
    let lane_mask = (1u128 << lane_bits) - 1;
    let elems = (0..len)
        .map(|i| {
            let bit = Expr::Binary {
                op: BinOp::BitAnd,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Shr,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(RustValue::TypedUInt(
                        (i as u32 * lane_bits) as u128,
                        int_prim,
                    ))),
                }),
                rhs: Box::new(Expr::Value(RustValue::TypedUInt(lane_mask, int_prim))),
            };
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(ctor_path.clone().into())),
                args: vec![Expr::Cast {
                    expr: Box::new(bit),
                    ty: Type::Prim(Prim::U128),
                }],
            }
        })
        .collect();
    Expr::ArrayLit(elems)
}
