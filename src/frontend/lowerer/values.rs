use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn materialize_expr(&mut self, result: &str, expr: Expr, cir_ty: Option<&CirType>) {
        let ty = cir_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        self.materialize_expr_as(result, expr, ty);
    }

    pub(super) fn materialize_expr_as(&mut self, result: &str, expr: Expr, ty: Type) {
        if let Some(name) = self
            .dispatch
            .as_ref()
            .and_then(|dispatch| dispatch.cross_block_names.get(result).cloned())
        {
            let default = self.parent.default_value_expr(&ty);
            self.dispatch
                .as_mut()
                .unwrap()
                .pending_hoists
                .push(Self::indent_stmt(Stmt::Let {
                    name: name.clone(),
                    mutable: true,
                    ty: Some(ty),
                    init: Some(default),
                }));
            self.push_stmt(Self::assign_stmt(Expr::Var(name.into()), expr));
            return;
        }
        let name = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(expr),
        });
        self.immutable_temps.insert(name.clone());
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
    }

    pub(super) fn forward_safe_value(&mut self, value: Expr, cir_ty: Option<&CirType>) -> Expr {
        let stable = match &value {
            Expr::Value(_) => true,
            Expr::Var(name) => self.immutable_temps.contains(name.as_str()),
            _ => false,
        };
        if stable {
            return value;
        }
        let name = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: cir_ty.map(|ty| self.parent.rust_type(ty)),
            init: Some(value),
        });
        self.immutable_temps.insert(name.clone());
        Expr::Var(name.into())
    }

    pub(super) fn operand_expr(&self, operand: &str) -> Expr {
        if let Some(val) = self.values.get(operand) {
            return val.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slot_place(operand) {
            return slot;
        }
        Expr::Var(sanitize_ident(operand))
    }

    pub(super) fn typed_operand_expr(&self, operand: &str, ty: &CirType) -> Expr {
        if is_cir_function_pointer_type(ty) {
            self.function_pointer_operand_expr(operand)
        } else if matches!(ty, CirType::Pointer { .. }) {
            self.whole_aggregate_pointer_expr(operand, ty)
                .unwrap_or_else(|| self.pointer_operand_expr(operand))
        } else {
            self.operand_expr(operand)
        }
    }

    pub(super) fn value_or_place_address_expr(&self, operand: &str) -> Expr {
        if self.values.contains_key(operand) {
            return self.operand_expr(operand);
        }
        if self.slot_place(operand).is_some()
            || self.member_ptrs.contains_key(operand)
            || self.element_ptrs.contains_key(operand)
            || self.global_name(operand).is_some()
        {
            return self.pointer_operand_expr(operand);
        }
        self.operand_expr(operand)
    }

    pub(super) fn slot_place(&self, operand: &str) -> Option<Expr> {
        self.slot_places.get(operand).cloned().or_else(|| {
            self.slots
                .get(operand)
                .map(|slot| Expr::Var(slot.clone().into()))
        })
    }

    pub(super) fn slot_receiver(&self, operand: &str) -> Option<Expr> {
        let place = self.slot_place(operand)?;
        if !self.aligned_slots.contains(operand) {
            return Some(place);
        }
        match place {
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => Some(*expr),
            _ => Some(place),
        }
    }

    pub(super) fn element_place_expr(&self, element: &ElementPtr) -> Expr {
        let index = Expr::Cast {
            expr: Box::new(element.index.clone()),
            ty: Type::Prim(Prim::Usize),
        };
        if element.unbounded || element.out_of_bounds {
            let array_ptr = self
                .global_array_ptr_expr(&element.base, element.elem_ty.as_ref())
                .unwrap_or_else(|| Expr::ArrayPtr {
                    array: Box::new(element.base.clone()),
                    mutable: true,
                });
            return Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(array_ptr),
                    method: "add".into(),
                    args: vec![index],
                }),
            };
        }
        Expr::Index {
            base: Box::new(element.base.clone()),
            index: Box::new(index),
        }
    }

    fn place_root_is_global(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Var(name) => {
                let name = name.as_str();
                self.parent.globals.contains_key(name)
                    || self.parent.extern_globals.contains_key(name)
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } | Expr::Index { base, .. } => {
                self.place_root_is_global(base)
            }
            _ => false,
        }
    }

    fn global_array_ptr_expr(&self, base: &Expr, elem_ty: Option<&Type>) -> Option<Expr> {
        if !self.place_root_is_global(base) {
            return None;
        }
        Some(Expr::MethodCallGeneric {
            recv: Box::new(Expr::AddrOf {
                mutable: true,
                expr: Box::new(base.clone()),
            }),
            method: "cast".into(),
            type_args: vec![elem_ty.cloned()?],
            args: Vec::new(),
        })
    }

    pub(super) fn pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.member_ptrs.contains_key(operand) || self.element_ptrs.contains_key(operand) {
            return self.store_address_expr(operand);
        }
        if self.global_name(operand).is_some() {
            return self.store_address_expr(operand);
        }
        if let Some(value) = self.values.get(operand) {
            return value.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slot_place(operand) {
            return if self
                .slot_types
                .get(operand)
                .is_some_and(|ty| matches!(ty, Type::Array { .. }))
            {
                Expr::MethodCall {
                    recv: Box::new(self.slot_receiver(operand).unwrap_or(slot)),
                    method: "as_mut_ptr".into(),
                    args: vec![],
                }
            } else {
                self.store_address_expr(operand)
            };
        }
        Expr::Var(sanitize_ident(operand))
    }

    pub(super) fn store_function_pointer_value(
        &mut self,
        operand: &str,
        ptr: &str,
        source_cir_ty: &CirType,
    ) -> Expr {
        let target_ty = self
            .member_ptrs
            .get(ptr)
            .and_then(|member| member.field_ty.as_ref())
            .or_else(|| self.slot_types.get(ptr))
            .cloned()
            .or_else(|| {
                let name = self.global_name(ptr)?;
                self.parent
                    .globals
                    .get(&name)
                    .map(|global| global.ty.clone())
                    .or_else(|| {
                        self.parent
                            .extern_globals
                            .get(&name)
                            .map(|global| global.ty.clone())
                    })
            });
        let operand_is_named_function = matches!(self.values.get(operand), Some(Val::Global(_)));
        if let Some(target_ty) = target_ty.clone()
            && let Some(Val::Global(fn_name)) = self.values.get(operand).cloned()
            && !self.parent.strings.contains_key(&fn_name)
            && let Some(wrapped) = self.parent.enum_return_mismatch_wrap(&fn_name, &target_ty)
        {
            return wrapped;
        }
        if let Some(target_ty) = target_ty.clone()
            && let Some(Val::Global(fn_name)) = self.values.get(operand).cloned()
            && !self.parent.strings.contains_key(&fn_name)
        {
            let target = self
                .parent
                .long_double_callback_trampolines
                .get(&fn_name)
                .cloned()
                .or_else(|| {
                    self.parent
                        .long_double_extern_pointer_shim(&fn_name, &target_ty)
                })
                .unwrap_or(fn_name);
            let raw_ptr = Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Unit),
            };
            return Expr::Transmute {
                from: raw_ptr.clone(),
                to: target_ty,
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::Var(sanitize_ident(&target))),
                    ty: raw_ptr,
                }),
            };
        }
        let mut value = self.function_pointer_operand_expr(operand);
        if let Some(target_ty) = target_ty.as_ref()
            && let source_ty = self
                .loaded_field_types
                .get(operand)
                .cloned()
                .unwrap_or_else(|| self.parent.rust_type(source_cir_ty))
            && source_ty != *target_ty
        {
            value = Expr::Transmute {
                from: source_ty,
                to: target_ty.clone(),
                expr: Box::new(value),
            };
        }
        if !operand_is_named_function
            && let Some(Type::FnPtr { ret, .. }) = target_ty
            && matches!(ret.as_ref(), Type::Custom(enum_name) if self.parent.enums.contains_key(enum_name))
        {
            return Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "mem", "transmute"].map(Ident::from),
                ))),
                args: vec![value],
            });
        }
        value
    }

    pub(super) fn named_function_coerced_to(
        &self,
        operand: &str,
        target_ty: &Type,
    ) -> Option<Expr> {
        let Some(Val::Global(fn_name)) = self.values.get(operand).cloned() else {
            return None;
        };
        if self.parent.strings.contains_key(&fn_name) {
            return None;
        }
        let target = self
            .parent
            .long_double_callback_trampolines
            .get(&fn_name)
            .cloned()
            .unwrap_or(fn_name);
        let raw_ptr = Type::Ptr {
            mutable: false,
            inner: Box::new(Type::Unit),
        };
        Some(Expr::Transmute {
            from: raw_ptr.clone(),
            to: target_ty.clone(),
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::Var(sanitize_ident(&target))),
                ty: raw_ptr,
            }),
        })
    }

    pub(super) fn function_pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.function_pointer_null_values.contains(operand) {
            return Expr::Value(RustValue::None);
        }
        match self.values.get(operand) {
            Some(Val::Global(name)) if !self.parent.strings.contains_key(name) => {
                if name == "main" {
                    return Expr::Value(RustValue::None);
                }
                let target = self
                    .parent
                    .long_double_callback_trampolines
                    .get(name)
                    .map(String::as_str)
                    .unwrap_or(name);
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("Some".into())),
                    args: vec![Expr::Var(sanitize_ident(target))],
                }
            }
            Some(Val::Expr(expr)) if self.is_function_pointer_none_expr(expr) => {
                Expr::Value(RustValue::None)
            }
            Some(value) => value.to_expr(&self.parent.strings),
            None => self.operand_expr(operand),
        }
    }

    pub(super) fn fn_ptr_aware_operand_expr(
        &self,
        operand: &str,
        ty: Option<&CirType>,
        fn_ptr_expr: fn(&Self, &str) -> Expr,
        plain_expr: fn(&Self, &str) -> Expr,
    ) -> Expr {
        if ty.is_some_and(is_cir_function_pointer_type) {
            fn_ptr_expr(self, operand)
        } else {
            plain_expr(self, operand)
        }
    }

    pub(super) fn function_pointer_byte_operand_expr(&self, operand: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.function_pointer_operand_expr(operand)),
                method: "unwrap".into(),
                args: Vec::new(),
            }),
            ty: Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Prim(Prim::U8)),
            },
        }
    }

    pub(super) fn is_function_pointer_null_operand(&self, operand: &str) -> bool {
        if self.function_pointer_null_values.contains(operand) {
            return true;
        }
        matches!(
            self.values.get(operand),
            Some(Val::Expr(expr)) if self.is_function_pointer_none_expr(expr)
        )
    }

    pub(super) fn is_function_pointer_none_expr(&self, expr: &Expr) -> bool {
        matches!(
            expr,
            Expr::Value(RustValue::None) | Expr::Value(RustValue::NullPtr)
        )
    }

    pub(super) fn whole_aggregate_pointer_expr(&self, operand: &str, ty: &CirType) -> Option<Expr> {
        let is_array_slot = self
            .slot_types
            .get(operand)
            .is_some_and(|slot_ty| matches!(slot_ty, Type::Array { .. }));
        let points_to_whole_aggregate = ty
            .pointee()
            .is_some_and(|inner| inner.as_array().is_some() || inner.as_vector().is_some());
        (is_array_slot && points_to_whole_aggregate).then(|| Expr::AddrOf {
            mutable: true,
            expr: Box::new(self.slot_place(operand).expect("checked slot_types above")),
        })
    }

    pub(super) fn call_arg_expr(&self, operand: &str, ty: &CirType) -> Expr {
        if is_boxed_va_args_type(&self.parent.rust_type(ty))
            && let Some(place) = self.va_target_place(operand)
        {
            Expr::MethodCall {
                recv: Box::new(place),
                method: "clone".into(),
                args: vec![],
            }
        } else if matches!(ty, CirType::Pointer { .. }) {
            let expr = self.typed_operand_expr(operand, ty);
            let operand_field_ty = self
                .member_ptrs
                .get(operand)
                .and_then(|member| member.field_ty.as_ref())
                .or_else(|| {
                    self.element_ptrs
                        .get(operand)
                        .and_then(|element| element.elem_ty.as_ref())
                })
                .or_else(|| self.slot_types.get(operand));
            if operand_field_ty.is_some_and(|field_ty| self.parent.type_is_enum(field_ty))
                && matches!(self.parent.rust_type(ty), Type::Ptr { .. })
            {
                Expr::Cast {
                    expr: Box::new(expr),
                    ty: self.parent.rust_type(ty),
                }
            } else {
                expr
            }
        } else {
            self.typed_operand_expr(operand, ty)
        }
    }

    pub(super) fn next_temp(&mut self) -> String {
        let name = format!("_v{}", self.temp_counter);
        self.temp_counter += 1;
        name
    }

    pub(super) fn emit_todo(&mut self, note: &str) {
        self.push_stmt(Stmt::Expr(Expr::Todo(note.to_string())));
    }

    pub(super) fn push_stmt(&mut self, stmt: Stmt) {
        self.body.push(IndentStmt {
            depth: self.indent,
            stmt,
        });
    }

    pub(super) fn unsafe_expr(value: Expr) -> Expr {
        Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(value)),
        }))
    }

    pub(super) fn unsafe_deref_expr(value: Expr) -> Expr {
        match value {
            Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some() => {
                Self::unsafe_expr(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: block.tail.expect("checked above"),
                })
            }
            value => Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(value),
            }),
        }
    }

    pub(super) fn without_empty_unsafe(value: Expr) -> Expr {
        match value {
            Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some() => {
                Self::without_empty_unsafe(*block.tail.expect("checked above"))
            }
            Expr::Cast { expr, ty } => Expr::Cast {
                expr: Box::new(Self::without_empty_unsafe(*expr)),
                ty,
            },
            value => value,
        }
    }

    pub(super) fn and_expr(lhs: Expr, rhs: Expr) -> Expr {
        Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        }
    }

    pub(super) fn or_exprs(mut exprs: Vec<Expr>) -> Expr {
        let first = exprs.remove(0);
        exprs.into_iter().fold(first, |lhs, rhs| Expr::Binary {
            op: BinOp::Or,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        })
    }

    pub(super) fn unsafe_stmt(stmt: Stmt) -> Stmt {
        Stmt::Unsafe {
            body: crate::backend::rust_ast::Block {
                stmts: vec![IndentStmt { depth: 0, stmt }],
                tail: None,
            },
        }
    }

    pub(super) fn assign_stmt(target: Expr, value: Expr) -> Stmt {
        Stmt::Assign { target, value }
    }

    pub(super) fn push_assign(&mut self, target: Expr, value: Expr) {
        self.push_stmt(Self::assign_stmt(target, value));
    }

    pub(super) fn push_unsafe_assign(&mut self, target: Expr, value: Expr) {
        self.push_stmt(Self::unsafe_stmt(Self::assign_stmt(target, value)));
    }

    pub(super) fn pointee_type(&self, ty: &CirType) -> Option<Type> {
        ty.pointee().map(|ty| self.parent.rust_type(ty))
    }
}
