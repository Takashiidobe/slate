use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_get_global(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let name = attr_str(op, "name")
            .unwrap_or("")
            .trim_start_matches('@')
            .trim_matches('"')
            .to_string();
        let name = self.parent.weak_refs.get(&name).cloned().unwrap_or(name);
        let name = if self.parent.strings.contains_key(&name)
            || self.parent.const_arrays.contains_key(&name)
            || self.parent.const_aggregates.contains_key(&name)
            || self.parent.const_zero_globals.contains(&name)
        {
            name
        } else {
            self.parent.rust_global_name(&name)
        };
        self.values.insert(result.clone(), Val::Global(name));
        if let Some(ty @ Type::FnPtr { .. }) =
            op_result_type(op).map(|ty| self.parent.rust_type(ty))
        {
            self.loaded_field_types.insert(result.clone(), ty);
        }
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
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        if let Some(outputs) = self.asm_outputs.get(base_ptr)
            && let Some(output) = aggregate_member_index(op).and_then(|index| outputs.get(index))
        {
            self.forward_values.insert(result.clone(), output.clone());
            return;
        }
        let base = self.place_or_deref_expr(base_ptr);
        let raw_field = attr_str(op, "name").unwrap_or(result);
        let logical_field = if raw_field.is_empty() {
            aggregate_member_index(op)
                .map(|index| format!("__slate_anon_{index}"))
                .unwrap_or_else(|| sanitize_ident(result).into_string())
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
            },
        );
    }

    pub(super) fn bitfield_storage_member(&self, op: &Op) -> Option<(String, Type, bool)> {
        let record_name = op
            .ty
            .as_deref()
            .and_then(split_top_level_arrow)
            .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
            .and_then(|inputs| split_top_level(inputs, ',').first().copied())
            .and_then(cir_ptr_pointee)
            .map(|ty| {
                self.parent
                    .aliases
                    .get(ty)
                    .map(String::as_str)
                    .unwrap_or(ty)
            })
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
        let Some(result) = op.results.first() else {
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
        let Some(result) = op.results.first() else {
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
        let record_ty = op_operand_types(op.ty.as_deref()?)
            .get(operand_index)
            .copied()?;
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

    pub(super) fn member_field_type(&self, base_ptr: &str, field: &str) -> Option<Type> {
        if let Some(Type::Custom(record_name)) = self.slot_types.get(base_ptr) {
            return self.record_field_type_by_name(record_name, field);
        }
        if let Some(Type::Custom(record_name)) = self
            .member_ptrs
            .get(base_ptr)
            .and_then(|member| member.field_ty.as_ref())
        {
            return self.record_field_type_by_name(record_name, field);
        }
        if let Some(Type::Custom(record_name)) = self
            .element_ptrs
            .get(base_ptr)
            .and_then(|element| element.elem_ty.as_ref())
        {
            return self.record_field_type_by_name(record_name, field);
        }
        None
    }

    pub(super) fn member_field_type_from_op(&self, op: &Op, field: &str) -> Option<Type> {
        let base_ty = op_operand_types(op.ty.as_deref()?).first().copied()?;
        let record_name = cir_ptr_pointee(base_ty).and_then(cir_record_name)?;
        self.record_field_type_by_name(&sanitize_ident(record_name).into_string(), field)
    }

    pub(super) fn record_field_type_by_name(&self, record_name: &str, field: &str) -> Option<Type> {
        let record = self.parent.records.get(record_name)?;
        record
            .fields
            .iter()
            .find(|candidate| sanitize_ident(&candidate.name).as_str() == field)
            .map(|candidate| self.parent.record_field_type(&candidate.ty))
    }

    pub(super) fn lower_set_bitfield(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let ty = op_result_type(op);
        let value = self.operand_expr(&op.operands[1]);
        let value = ty.map_or(value.clone(), |ty| Expr::Cast {
            expr: Box::new(value),
            ty: self.parent.rust_type(ty),
        });
        let trunc = self.truncate_bitfield_expr(op, value, ty);
        self.materialize_expr(result, trunc, ty);
        let stored = self.operand_expr(result);
        let (place, needs_unsafe) = self.bitfield_place(&op.operands[0]);
        if let Some(field) = self
            .member_ptrs
            .get(&op.operands[0])
            .and_then(|member| member.bitfield_name.as_ref())
            .cloned()
        {
            if needs_unsafe {
                let ptr = self.next_temp();
                self.push_stmt(Stmt::Let {
                    name: ptr.clone(),
                    mutable: false,
                    ty: None,
                    init: Some(Self::unsafe_expr(Expr::Unary {
                        op: UnaryOp::Raw(Raw::Mut),
                        expr: Box::new(place),
                    })),
                });
                let temp = self.next_temp();
                let temp_ty = self
                    .member_ptrs
                    .get(&op.operands[0])
                    .and_then(|member| member.field_ty.clone());
                self.push_stmt(Stmt::Let {
                    name: temp.clone(),
                    mutable: true,
                    ty: temp_ty,
                    init: Some(Self::unsafe_expr(Expr::MethodCall {
                        recv: Box::new(Expr::Var(ptr.clone().into())),
                        method: "read_unaligned".into(),
                        args: Vec::new(),
                    })),
                });
                self.push_stmt(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(temp.clone().into())),
                    method: format!("set_{field}"),
                    args: vec![stored],
                }));
                self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(ptr.into())),
                    method: "write_unaligned".into(),
                    args: vec![Expr::Var(temp.into())],
                })));
                return;
            }
            let call = Expr::MethodCall {
                recv: Box::new(place),
                method: format!("set_{field}"),
                args: vec![stored],
            };
            self.push_stmt(Stmt::Expr(call));
            return;
        }
        if needs_unsafe {
            self.push_unsafe_assign(place, stored);
        } else {
            self.push_assign(place, stored);
        }
    }

    pub(super) fn lower_get_bitfield(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let ty = op_result_type(op);
        let (place, needs_unsafe) = self.bitfield_place(ptr);
        let read = if let Some(field) = self
            .member_ptrs
            .get(ptr)
            .and_then(|member| member.bitfield_name.as_ref())
        {
            Expr::MethodCall {
                recv: Box::new(if needs_unsafe {
                    Self::unsafe_expr(Expr::MethodCall {
                        recv: Box::new(Expr::Unary {
                            op: UnaryOp::Raw(Raw::Const),
                            expr: Box::new(place),
                        }),
                        method: "read_unaligned".into(),
                        args: Vec::new(),
                    })
                } else {
                    place
                }),
                method: field.clone(),
                args: Vec::new(),
            }
        } else {
            if needs_unsafe {
                Self::unsafe_expr(place)
            } else {
                place
            }
        };
        let expr = self.truncate_bitfield_expr(op, read, ty);
        self.materialize_expr(result, expr, ty);
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
    pub(super) fn truncate_bitfield_expr(&self, op: &Op, expr: Expr, ty: Option<&str>) -> Expr {
        let bits = ty
            .map(|ty| self.parent.rust_type(ty))
            .and_then(|t| int_bits(&t.render()));
        match (self.bitfield_size(op), bits) {
            (Some(size), Some(bits)) if size < bits => {
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
        let raw = attr_str(op, "bitfield_info")?;
        let resolved = self.parent.aliases.get(raw).map_or(raw, String::as_str);
        Some((
            bitfield_info_number(resolved, "size = ")?,
            bitfield_info_number(resolved, "offset = ")?,
        ))
    }

    pub(super) fn lower_get_element(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base_ptr = &op.operands[0];
        let index = self.operand_expr(&op.operands[1]);
        let unbounded = self
            .member_ptrs
            .get(base_ptr)
            .is_some_and(|member| matches!(&member.field_ty, Some(Type::Array { len: 0, .. })));
        let array_len = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .and_then(cir_ptr_inner)
            .and_then(parse_cir_array_type)
            .map(|(_, len)| len);
        let out_of_bounds = array_len.is_some_and(|len| {
            self.known_arith_value(&op.operands[1])
                .is_some_and(|value| value >= i128::from(len))
        });
        let elem_ty = op_result_type(op)
            .and_then(cir_ptr_inner)
            .map(|ty| self.parent.rust_type(ty));
        if let Some(Val::Global(name)) = self.values.get(base_ptr).cloned() {
            if let Some(labels) = self.parent.block_addr_globals.get(&name) {
                self.block_addr_element_ptrs.insert(
                    result.clone(),
                    BlockAddrElementPtr {
                        labels: labels.clone(),
                        index: index.clone(),
                    },
                );
            }
            let declared_len = array_len.map(|len| len as usize);
            if let Some(base) = self.global_array_literal_expr(&name, elem_ty.clone(), declared_len)
            {
                self.element_ptrs.insert(
                    result.clone(),
                    ElementPtr {
                        base,
                        index,
                        unsafe_access: false,
                        unbounded: false,
                        out_of_bounds: false,
                        elem_ty,
                    },
                );
                return;
            }
        }
        let base = self.place_or_deref_expr(base_ptr);
        let unsafe_access =
            unbounded || self.place_expr(base_ptr).is_none() || self.ptr_requires_unsafe(base_ptr);
        self.element_ptrs.insert(
            result.clone(),
            ElementPtr {
                base,
                index,
                unsafe_access,
                unbounded,
                out_of_bounds,
                elem_ty,
            },
        );
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
        op.ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next())
            .and_then(cir_ptr_inner)
            .is_some_and(|ty| self.parent.cir_type_is_union(ty))
    }

    pub(super) fn lower_cast(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op).unwrap_or("");
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        if (is_cir_va_list_value_type(result_ty) || is_cir_va_list_value_type(operand_ty))
            && let Some(place) = self.va_target_place(src)
        {
            self.va_places.insert(result.clone(), place.clone());
            self.values.insert(result.clone(), Val::Expr(place));
            return;
        }
        let result_rust_ty = self.parent.rust_type(result_ty);
        let operand_rust_ty = self.parent.rust_type(operand_ty);
        let value = match self.values.get(src).cloned() {
            Some(Val::Global(name))
                if is_cir_function_pointer_type(result_ty)
                    && is_cir_function_pointer_type(operand_ty) =>
            {
                let from = self.parent.rust_type(operand_ty);
                let to = self.parent.rust_type(result_ty);
                if from == to {
                    Val::Global(name)
                } else {
                    let raw_ptr = Type::Ptr {
                        mutable: false,
                        inner: Box::new(Type::Unit),
                    };
                    Val::Expr(Expr::Transmute {
                        from: raw_ptr.clone(),
                        to,
                        expr: Box::new(Expr::Cast {
                            expr: Box::new(Expr::Var(sanitize_ident(&name))),
                            ty: raw_ptr,
                        }),
                    })
                }
            }
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && self.parent.strings.contains_key(&name) =>
            {
                let bytes = self.parent.strings[&name].clone();
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::ByteStr(bytes)),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && self.parent.const_arrays.contains_key(&name) =>
            {
                let elems = &self.parent.const_arrays[&name];
                let (elem_ty, len) = cir_ptr_inner(operand_ty)
                    .and_then(parse_cir_array_type)
                    .map_or((Type::Prim(Prim::I32), elems.len()), |(elem, len)| {
                        (self.parent.rust_type(&elem), len as usize)
                    });
                let default = self.parent.default_value_expr(&elem_ty);
                let mut typed: Vec<Expr> = elems.clone();
                if let Some(first) = typed.first_mut() {
                    *first = Expr::Cast {
                        expr: Box::new(first.clone()),
                        ty: elem_ty,
                    };
                }
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::ArrayPtr {
                        array: Box::new(render_array_literal_expr(&typed, len, default)),
                        mutable: false,
                    }),
                    ty: ptr_ty,
                })
            }
            Some(Val::Global(_)) if !result_ty.starts_with("!cir.ptr<") => {
                let Some(name) = self.global_name(src) else {
                    self.emit_todo("cir.cast (global ptrtoint)");
                    return;
                };
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: false,
                        expr: Box::new(Expr::Var(name.into())),
                    }),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && !is_cir_function_pointer_type(result_ty)
                    && !self.parent.strings.contains_key(&name)
                    && !self.parent.const_arrays.contains_key(&name)
                    && !self
                        .parent
                        .globals
                        .contains_key(&sanitize_ident(&name).into_string())
                    && !self
                        .parent
                        .extern_globals
                        .contains_key(&sanitize_ident(&name).into_string()) =>
            {
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::Var(sanitize_ident(&name))),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            Some(Val::Global(name)) => match self.global_array_decay_expr(&name, result_ty) {
                Some(expr) => Val::Expr(expr),
                None => Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var(sanitize_ident(&name))),
                    }),
                    ty: self.parent.rust_type(result_ty),
                }),
            },
            _ if self
                .slot_types
                .get(src)
                .is_some_and(|ty| matches!(ty, Type::Array { .. })) =>
            {
                let array_ptr = Expr::ArrayPtr {
                    array: Box::new(
                        self.slot_receiver(src)
                            .unwrap_or_else(|| self.operand_expr(src)),
                    ),
                    mutable: true,
                };
                let elem_ty_matches = result_ty
                    .strip_prefix("!cir.ptr<")
                    .and_then(|s| s.strip_suffix('>'))
                    .is_some_and(|pointee| {
                        matches!(
                            self.slot_types.get(src),
                            Some(Type::Array { elem, .. })
                                if **elem == self.parent.rust_type(pointee)
                        )
                    });
                if elem_ty_matches {
                    Val::Expr(array_ptr)
                } else {
                    Val::Expr(Expr::Cast {
                        expr: Box::new(array_ptr),
                        ty: self.parent.rust_type(result_ty),
                    })
                }
            }
            _ if is_wrapped_long_double(result_ty)
                && !is_long_double(operand_ty)
                && bitint_generic_parts(&operand_rust_ty).is_some() =>
            {
                let (wide_expr, signed) =
                    bitint_to_int_expr(&operand_rust_ty, self.operand_expr(src)).unwrap();
                let wide_ty = if signed {
                    Type::Prim(Prim::I128)
                } else {
                    Type::Prim(Prim::U128)
                };
                let shim = f80_cast_from_name(&wide_ty).expect("f80 shim for i128/u128 exists");
                Val::Expr(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![wide_expr],
                })
            }
            _ if is_wrapped_long_double(result_ty) && !is_long_double(operand_ty) => {
                let rust_ty = self.parent.rust_type(operand_ty);
                let Some(shim) = f80_cast_from_name(&rust_ty) else {
                    self.emit_todo("long double cast");
                    return;
                };
                Val::Expr(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![self.operand_expr(src)],
                })
            }
            _ if is_wrapped_long_double(operand_ty)
                && !is_long_double(result_ty)
                && bitint_generic_parts(&result_rust_ty).is_some() =>
            {
                let (name, _, _) = bitint_generic_parts(&result_rust_ty).unwrap();
                let signed = name == "bitint::BInt";
                let wide_ty = if signed {
                    Type::Prim(Prim::I128)
                } else {
                    Type::Prim(Prim::U128)
                };
                let shim = f80_cast_to_name(&wide_ty).expect("f80 shim for i128/u128 exists");
                let wide_expr = Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![self.operand_expr(src)],
                };
                Val::Expr(bitint_from_int_expr(&result_rust_ty, wide_expr, signed).unwrap())
            }
            _ if is_wrapped_long_double(operand_ty) && !is_long_double(result_ty) => {
                let rust_ty = self.parent.rust_type(result_ty);
                let Some(shim) = f80_cast_to_name(&rust_ty) else {
                    self.emit_todo("long double cast");
                    return;
                };
                Val::Expr(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![self.operand_expr(src)],
                })
            }
            _ if is_cir_function_pointer_type(result_ty)
                && is_cir_function_pointer_type(operand_ty)
                && result_ty != operand_ty =>
            {
                let from = self
                    .loaded_field_types
                    .get(src)
                    .cloned()
                    .unwrap_or_else(|| self.parent.rust_type(operand_ty));
                let to = self.parent.rust_type(result_ty);
                if from == to {
                    Val::Expr(self.function_pointer_operand_expr(src))
                } else {
                    Val::Expr(Expr::Transmute {
                        from,
                        to,
                        expr: Box::new(self.function_pointer_operand_expr(src)),
                    })
                }
            }
            _ if result_ty.starts_with("!cir.ptr<")
                && is_cir_function_pointer_type(operand_ty)
                && !is_cir_function_pointer_type(result_ty) =>
            {
                Val::Expr(Expr::Transmute {
                    from: self.parent.rust_type(operand_ty),
                    to: self.parent.rust_type(result_ty),
                    expr: Box::new(self.function_pointer_operand_expr(src)),
                })
            }
            _ if result_ty.starts_with("!cir.ptr<")
                && operand_ty.starts_with("!cir.ptr<")
                && !(is_cir_function_pointer_type(result_ty)
                    && !is_cir_function_pointer_type(operand_ty)) =>
            {
                Val::Expr(Expr::Cast {
                    expr: Box::new(self.pointer_operand_expr(src)),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            _ if operand_ty.starts_with("!cir.ptr<")
                && result_ty != "!cir.bool"
                && !is_cir_function_pointer_type(operand_ty)
                && !is_cir_function_pointer_type(result_ty) =>
            {
                Val::Expr(Expr::Cast {
                    expr: Box::new(self.pointer_operand_expr(src)),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            _ if result_ty.starts_with("!cir.ptr<!cir.func<") => {
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(Expr::Transmute {
                    from: Type::Prim(Prim::Usize),
                    to: ptr_ty,
                    expr: Box::new(Expr::Cast {
                        expr: Box::new(self.operand_expr(src)),
                        ty: Type::Prim(Prim::Usize),
                    }),
                })
            }
            _ if result_ty == "!cir.bool" && is_cir_function_pointer_type(operand_ty) => {
                Val::Expr(Expr::MethodCall {
                    recv: Box::new(self.function_pointer_operand_expr(src)),
                    method: "is_some".into(),
                    args: Vec::new(),
                })
            }
            _ if result_ty == "!cir.bool" && operand_ty != "!cir.bool" => Val::Expr(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(self.operand_expr(src)),
                rhs: Box::new(zero_for_cir_type(operand_ty)),
            }),
            _ if bitint_generic_parts(&result_rust_ty).is_some()
                && bitint_generic_parts(&operand_rust_ty).is_none()
                && parse_cir_int_type(operand_ty).is_some() =>
            {
                let (signed, _) = parse_cir_int_type(operand_ty).unwrap();
                Val::Expr(
                    bitint_from_int_expr(&result_rust_ty, self.operand_expr(src), signed).unwrap(),
                )
            }
            _ if bitint_generic_parts(&operand_rust_ty).is_some()
                && bitint_generic_parts(&result_rust_ty).is_none()
                && parse_cir_int_type(result_ty).is_some() =>
            {
                let (wide, _) =
                    bitint_to_int_expr(&operand_rust_ty, self.operand_expr(src)).unwrap();
                Val::Expr(Expr::Cast {
                    expr: Box::new(wide),
                    ty: result_rust_ty.clone(),
                })
            }
            _ if result_ty == operand_ty => Val::Expr(self.operand_expr(src)),
            _ => Val::Expr(Expr::Cast {
                expr: Box::new(self.operand_expr(src)),
                ty: self.parent.rust_type(result_ty),
            }),
        };
        if is_cir_function_pointer_type(result_ty) {
            let ty = if matches!(value, Val::Global(_)) {
                self.loaded_field_types
                    .get(src)
                    .cloned()
                    .unwrap_or_else(|| self.parent.rust_type(operand_ty))
            } else {
                self.parent.rust_type(result_ty)
            };
            self.loaded_field_types.insert(result.clone(), ty);
        }
        self.values.insert(result.clone(), value);
    }

    pub(super) fn lower_ptr_diff(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let lhs = self.fn_ptr_aware_operand_expr(
            &op.operands[0],
            operand_types.first().copied(),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let rhs = self.fn_ptr_aware_operand_expr(
            &op.operands[1],
            operand_types.get(1).copied(),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I64));
        self.materialize_expr(
            result,
            Self::unsafe_expr(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(lhs),
                    method: "offset_from".into(),
                    args: vec![rhs],
                }),
                ty,
            }),
            op_result_type(op),
        );
    }

    pub(super) fn lower_ptr_stride(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let function_pointer_stride = operand_types
            .first()
            .is_some_and(|ty| is_cir_function_pointer_type(ty))
            && op_result_type(op).is_some_and(is_cir_function_pointer_type);
        let base = self.fn_ptr_aware_operand_expr(
            &op.operands[0],
            function_pointer_stride
                .then(|| operand_types.first().copied())
                .flatten(),
            Self::function_pointer_byte_operand_expr,
            Self::pointer_operand_expr,
        );
        let index = self.operand_expr(&op.operands[1]);
        let (method, args) = self.ptr_stride_method_and_args(op, index);
        let stride = Self::unsafe_expr(Expr::MethodCall {
            recv: Box::new(base),
            method,
            args,
        });
        let value = if function_pointer_stride {
            Expr::Transmute {
                from: Type::Ptr {
                    mutable: false,
                    inner: Box::new(Type::Prim(Prim::U8)),
                },
                to: self
                    .parent
                    .rust_type(op_result_type(op).expect("checked above")),
                expr: Box::new(stride),
            }
        } else {
            stride
        };
        self.values.insert(result.clone(), Val::Expr(value));
    }

    pub(super) fn ptr_stride_method_and_args(&self, op: &Op, index: Expr) -> (String, Vec<Expr>) {
        if let Some(index_operand) = op.operands.get(1) {
            if let Some(value) = self.const_int_values.get(index_operand)
                && *value >= 0
            {
                return ("add".into(), vec![int_value_expr(*value)]);
            }
            if op_operand_types(op.ty.as_deref().unwrap_or(""))
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

    pub(super) fn cir_int_is_unsigned(&self, ty: &str) -> bool {
        let resolved = self.parent.aliases.get(ty).map_or(ty, String::as_str);
        if let Some((signed, _)) = parse_cir_int_type(resolved) {
            return !signed;
        }
        resolved
            .trim()
            .strip_prefix("!cir.int<")
            .and_then(|rest| rest.split(',').next())
            .is_some_and(|sign| sign.trim() == "u")
    }
}
