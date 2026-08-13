use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn materialize_expr(&mut self, result: &str, expr: Expr, cir_ty: Option<&str>) {
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

    pub(super) fn forward_safe_value(&mut self, value: Expr, cir_ty: Option<&str>) -> Expr {
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

    pub(super) fn value_or_place_address_expr(&self, operand: &str) -> Expr {
        if self.values.contains_key(operand) || self.slot_place(operand).is_some() {
            return self.operand_expr(operand);
        }
        if self.member_ptrs.contains_key(operand)
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
            return Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ArrayPtr {
                        array: Box::new(element.base.clone()),
                        mutable: true,
                    }),
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
                Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(slot),
                }
            };
        }
        Expr::Var(sanitize_ident(operand))
    }

    pub(super) fn store_function_pointer_value(
        &mut self,
        operand: &str,
        ptr: &str,
        source_cir_ty: &str,
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

    pub(super) fn function_pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.function_pointer_null_values.contains(operand) {
            return Expr::Value(RustValue::None);
        }
        match self.values.get(operand) {
            Some(Val::Global(name)) if !self.parent.strings.contains_key(name) => {
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
        ty: Option<&str>,
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

    pub(super) fn call_arg_expr(&self, operand: &str, ty: &str) -> Expr {
        if is_cir_function_pointer_type(ty) {
            self.function_pointer_operand_expr(operand)
        } else if ty.starts_with("!cir.ptr<") {
            let expr = self.pointer_operand_expr(operand);
            if self
                .slot_types
                .get(operand)
                .is_some_and(|slot_ty| self.parent.type_is_enum(slot_ty))
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
            self.operand_expr(operand)
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

    pub(super) fn pointee_type(&self, ty: &str) -> Option<Type> {
        let ret = op_type_return(ty)?;
        ret.strip_prefix("!cir.ptr<")
            .and_then(|s| s.strip_suffix('>'))
            .map(|ty| self.parent.rust_type(ty))
    }
}
