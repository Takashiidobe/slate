use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_get_bitfield(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let (place, needs_unsafe) = self.bitfield_place(ptr);
        let value = if needs_unsafe {
            Self::unsafe_expr(place)
        } else {
            place
        };
        let value = self.truncate_bitfield_expr(op, value, Some(result_ty));
        self.materialize_expr(result, value, Some(result_ty));
    }

    pub(super) fn lower_set_bitfield(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let (Some(ptr), Some(src)) = (op.operands.first(), op.operands.get(1)) else {
            return;
        };
        let value = self.truncate_bitfield_expr(op, self.operand_expr(src), Some(result_ty));
        self.materialize_expr(result, value.clone(), Some(result_ty));
        let (place, needs_unsafe) = self.bitfield_place(ptr);
        if needs_unsafe {
            self.push_unsafe_assign(place, value);
        } else {
            self.push_assign(place, value);
        }
    }

    pub(super) fn lower_get_element(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let (Some(base), Some(index)) = (op.operands.first(), op.operands.get(1)) else {
            return;
        };
        let base_expr = self.place_or_deref_expr(base);
        let elem_ty = cir_ptr_inner(result_ty).map(|ty| self.parent.rust_type(ty));
        self.element_ptrs.insert(
            result.clone(),
            ElementPtr {
                base: base_expr,
                index: self.operand_expr(index),
                unsafe_access: self.place_expr(base).is_none() || self.ptr_requires_unsafe(base),
                unbounded: false,
                out_of_bounds: false,
                elem_ty,
            },
        );
    }

    pub(super) fn lower_ptr_stride(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let (Some(base), Some(stride)) = (op.operands.first(), op.operands.get(1)) else {
            return;
        };
        let (method, args) = self.ptr_stride_method_and_args(op, self.operand_expr(stride));
        let value = Self::unsafe_expr(Expr::MethodCall {
            recv: Box::new(self.pointer_operand_expr(base)),
            method,
            args,
        });
        self.materialize_expr(result, value, Some(result_ty));
    }

    pub(super) fn lower_ptr_diff(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let (Some(lhs), Some(rhs)) = (op.operands.first(), op.operands.get(1)) else {
            return;
        };
        let value = Self::unsafe_expr(Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.pointer_operand_expr(lhs)),
                method: "offset_from".into(),
                args: vec![self.pointer_operand_expr(rhs)],
            }),
            ty: self.parent.rust_type(result_ty),
        });
        self.materialize_expr(result, value, Some(result_ty));
    }

    pub(super) fn lower_cast(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let Some(src_ty) = op_operand_types(op).first() else {
            return;
        };
        if let Some(value) = self.values.get(src).cloned()
            && let Val::Global(name) = value
            && let Some(expr) = self.global_array_decay_expr(&name, result_ty)
        {
            self.materialize_expr(result, expr, Some(result_ty));
            return;
        }
        let value = self.operand_expr(src);
        let from = self.parent.rust_type(src_ty);
        let to = self.parent.rust_type(result_ty);
        let expr = if from == to {
            value
        } else {
            Expr::Cast {
                expr: Box::new(value),
                ty: to,
            }
        };
        self.materialize_expr(result, expr, Some(result_ty));
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

    pub(super) fn lower_get_member(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        self.member_base_operand
            .insert(result.clone(), base_ptr.clone());
        if let Some(outputs) = self.asm_outputs.get(base_ptr)
            && let Some(output) = aggregate_member_index(op).and_then(|index| outputs.get(index))
        {
            self.forward_values.insert(result.clone(), output.clone());
            return;
        }
        let base = self.place_or_deref_expr(base_ptr);
        let raw_field = attr_str(op, "name").unwrap_or(result.as_str());
        let logical_field = if raw_field.is_empty() {
            aggregate_member_index(op)
                .map(|index| format!("__slate_anon_{index}"))
                .unwrap_or_else(|| sanitize_ident(result.as_str()).into_string())
        } else {
            sanitize_ident(raw_field).into_string()
        };
        let storage = self.bitfield_storage_member(op);
        let field = storage
            .as_ref()
            .map(|(field, _, _)| field.clone())
            .unwrap_or_else(|| logical_field.clone());
        let field_ty = storage.as_ref().map(|(_, ty, _)| ty.clone()).or_else(|| {
            self.member_field_type(base_ptr, &logical_field)
                .or_else(|| self.member_field_type_from_op(op, &logical_field))
        });
        let unsafe_access = self.place_expr(base_ptr).is_none()
            || self.ptr_requires_unsafe(base_ptr)
            || self.op_base_is_union(op);
        let field_is_trailing = self.member_field_is_trailing(base_ptr, op, &field);
        self.member_ptrs.insert(
            result.clone(),
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

    pub(super) fn bitfield_storage_member(&self, op: &Op) -> Option<(String, Type, bool)> {
        let record_name = op_operand_types(op)
            .first()
            .and_then(cir_ptr_pointee)
            .map(|ty| self.parent.expand_alias(ty))
            .and_then(cir_record_name)?;
        let record_name = sanitize_ident(record_name).into_string();
        let record = self.parent.records.get(&record_name)?;
        let fields = self.parent.bitfield_storage_fields(record)?;
        let index = aggregate_member_index(op)?;
        let field = fields.get(index)?;
        let wrapper = self
            .parent
            .bitfield_storages
            .get(&(record_name, index))
            .map(|storage| Type::Custom(storage.wrapper.clone()));
        let ty = wrapper.clone().or_else(|| {
            op_result_type(op)
                .and_then(cir_ptr_pointee)
                .map(|ty| self.parent.rust_type(ty))
        })?;
        Some((
            sanitize_ident(&field.name).into_string(),
            ty,
            wrapper.is_some(),
        ))
    }

    pub(super) fn lower_extract_member(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(base) = op.operands.first() else {
            return;
        };
        let Some(field) = self.value_member_field(op, 0) else {
            self.emit_todo("cir.extract_member");
            return;
        };
        self.materialize_expr(
            result,
            Expr::Field {
                base: Box::new(self.operand_expr(base)),
                field,
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_insert_member(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let Some(index) = aggregate_member_index(op) else {
            self.emit_todo("cir.insert_member");
            return;
        };
        let Some(record_name) = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .and_then(|ty| match ty {
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
        let base = self.operand_expr(&op.operands[0]);
        let value = self.operand_expr(&op.operands[1]);
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
            result,
            wrap_record_lit(
                record,
                Expr::StructLit {
                    name: record_lit_name(record),
                    fields,
                },
            ),
            op_result_type(op),
        );
    }

    pub(super) fn value_member_field(&self, op: &Op, operand_index: usize) -> Option<String> {
        let index = aggregate_member_index(op)?;
        let record_ty = op_operand_types(op).get(operand_index)?;
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

    pub(super) fn record_name_from_op(&self, op: &Op) -> Option<String> {
        let base_ty = op_operand_types(op).first()?;
        let record_name = cir_ptr_pointee(base_ty)
            .map(|ty| self.parent.expand_alias(ty))
            .and_then(cir_record_name)?;
        Some(sanitize_ident(record_name).into_string())
    }

    pub(super) fn member_field_type(&self, base_ptr: &str, field: &str) -> Option<Type> {
        let record_name = self.member_record_name(base_ptr)?;
        self.record_field_type_by_name(&record_name, field)
    }

    pub(super) fn member_field_type_from_op(&self, op: &Op, field: &str) -> Option<Type> {
        let record_name = self.record_name_from_op(op)?;
        self.record_field_type_by_name(&record_name, field)
    }

    pub(super) fn record_field_type_by_name(&self, record_name: &str, field: &str) -> Option<Type> {
        let record = self.parent.records.get(record_name)?;
        record
            .fields
            .iter()
            .find(|candidate| sanitize_ident(&candidate.name).as_str() == field)
            .map(|candidate| self.parent.record_field_type(&candidate.ty))
    }

    pub(super) fn member_field_is_trailing(&self, base_ptr: &str, op: &Op, field: &str) -> bool {
        let Some(record_name) = self
            .member_record_name(base_ptr)
            .or_else(|| self.record_name_from_op(op))
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

    // shift up then arithmetic-shift down masks to `size` bits, sign-extending signed types.
    pub(super) fn truncate_bitfield_expr(&self, op: &Op, expr: Expr, ty: Option<&CirType>) -> Expr {
        let rust_ty = ty.map(|ty| self.parent.rust_type(ty));
        let bits = rust_ty.as_ref().and_then(|t| int_bits(&t.render()));
        match (self.bitfield_size(op), bits) {
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

    pub(super) fn bitfield_size(&self, op: &Op) -> Option<u32> {
        self.bitfield_size_offset(op).map(|(size, _)| size)
    }

    pub(super) fn bitfield_size_offset(&self, op: &Op) -> Option<(u32, u32)> {
        let attr = self.parent.resolve_attr(op.attr("bitfield_info")?);
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

    pub(super) fn op_base_is_union(&self, op: &Op) -> bool {
        op_operand_types(op)
            .first()
            .and_then(cir_ptr_inner)
            .is_some_and(|ty| self.parent.cir_type_is_union(ty))
    }
    pub(super) fn ptr_stride_method_and_args(&self, op: &Op, index: Expr) -> (String, Vec<Expr>) {
        if let Some(index_operand) = op.operands.get(1) {
            if let Some(value) = self.const_int_values.get(index_operand)
                && *value >= 0
            {
                return ("add".into(), vec![int_value_expr(*value)]);
            }
            if op_operand_types(op)
                .get(1)
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
        parse_cir_int_type(resolved).is_some_and(|(signed, _)| !signed)
    }
}
