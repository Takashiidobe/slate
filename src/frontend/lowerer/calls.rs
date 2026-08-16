use super::*;
use crate::function_identity::CallBinding;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    fn coerce_group_alloca(&self, load_result: &str) -> Option<(&str, &str, &str)> {
        let ptr = self.load_ptr_operand.get(load_result)?;
        let base = self.member_base_operand.get(ptr)?;
        let (address, real_type) = self.coerce_alloca_real_type.get(base)?;
        Some((base.as_str(), address.as_str(), real_type.as_str()))
    }

    pub(super) fn lower_call(&mut self, op: &Op) {
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let direct_callee =
            attr_str(op, "callee").map(|callee| callee.trim_start_matches('@').to_string());
        let weak_ref_target = direct_callee
            .as_ref()
            .and_then(|callee| self.parent.weak_refs.get(callee))
            .cloned();
        let external_weak_call = weak_ref_target
            .as_ref()
            .is_some_and(|target| self.parent.external_weak_targets.contains(target));
        let direct_callee = weak_ref_target.clone().or(direct_callee);
        let mut binding = if weak_ref_target.is_some() {
            CallBinding::Generated
        } else {
            self.parent.call_binding(op, direct_callee.is_some())
        };
        if binding.known().is_none()
            && let Some(callee) = direct_callee.as_deref()
            && let Some(identity @ FunctionIdentity::Known(_)) =
                self.parent.known_functions.get(callee)
        {
            binding = CallBinding::Direct {
                identity: *identity,
                canonical_type: match binding {
                    CallBinding::Direct { canonical_type, .. } => canonical_type,
                    CallBinding::Indirect | CallBinding::Generated => None,
                },
            };
        }
        let (callee_name, callee_expr, arg_operands, arg_types, indirect_callee_operand) =
            if let Some(callee) = direct_callee {
                let callee_expr = if external_weak_call {
                    Expr::MethodCall {
                        recv: Box::new(Expr::Var(callee.clone().into())),
                        method: "unwrap".into(),
                        args: vec![],
                    }
                } else {
                    Expr::Var(callee.clone().into())
                };
                (
                    callee.clone(),
                    callee_expr,
                    op.operands.as_slice(),
                    operand_types.as_slice(),
                    None,
                )
            } else {
                let Some((callee_operand, arg_operands)) = op.operands.split_first() else {
                    return;
                };
                (
                    String::new(),
                    Expr::MethodCall {
                        recv: Box::new(self.operand_expr(callee_operand)),
                        method: "unwrap".into(),
                        args: vec![],
                    },
                    arg_operands,
                    operand_types.get(1..).unwrap_or(&[]),
                    Some(callee_operand.clone()),
                )
            };
        let arg_attrs_offset = op.operands.len() - arg_operands.len();
        let mut args = arg_operands
            .iter()
            .zip(arg_types.iter().copied())
            .map(|(operand, ty)| self.call_arg_expr(operand, ty))
            .collect::<Vec<_>>();
        if self.try_long_double_call_shim(op, &callee_name, &args, arg_types)
            || self.try_format_call_shims(op, &callee_name, &args, arg_types)
        {
            return;
        }
        let indirect_param_types = indirect_callee_operand
            .as_deref()
            .and_then(|operand| self.loaded_field_types.get(operand))
            .and_then(|ty| match ty {
                Type::FnPtr { params, .. } => Some(params.clone()),
                _ => None,
            });
        let indirect_call = indirect_callee_operand.is_some();
        let cast_runtime_result = op_result_type(op).is_some_and(|result_ty| {
            let result_ty = self.parent.rust_type(result_ty);
            self.parent
                .extern_returns
                .get(&callee_name)
                .and_then(|ret| ret.as_deref())
                .is_some_and(|ret| matches!(ret, "usize" | "isize") && ret != result_ty.render())
        });
        if self.parent.boxed_variadic_defs.contains(&callee_name) {
            let fixed = self
                .parent
                .function_param_types
                .get(&callee_name)
                .map_or(0, Vec::len)
                .min(args.len());
            let variadic = args.split_off(fixed);
            let variadic_operands = &arg_operands[fixed..];
            let mut boxed = Vec::new();
            let mut i = 0;
            while i < variadic.len() {
                let group = variadic_operands
                    .get(i)
                    .and_then(|id| self.coerce_group_alloca(id));
                let arg = if let Some((group_key, address, real_record)) = group {
                    let group_key = group_key.to_string();
                    let address = address.to_string();
                    let mut j = i + 1;
                    while variadic_operands
                        .get(j)
                        .and_then(|id| self.coerce_group_alloca(id))
                        .is_some_and(|(key, _, _)| key == group_key)
                    {
                        j += 1;
                    }
                    let real_ty = Type::Custom(rust_record_name(real_record));
                    let read = Self::unsafe_expr(Expr::Call {
                        binding: CallBinding::Generated,
                        func: Box::new(Expr::Path(Path::new(
                            ["std", "ptr", "read_unaligned"].map(Ident::from),
                        ))),
                        args: vec![Expr::Cast {
                            expr: Box::new(self.pointer_operand_expr(&address)),
                            ty: Type::Ptr {
                                mutable: false,
                                inner: Box::new(real_ty),
                            },
                        }],
                    });
                    i = j;
                    read
                } else {
                    let byval = call_arg_byval_type(op, arg_attrs_offset + fixed + i).is_some();
                    let arg = variadic[i].clone();
                    let arg = if byval {
                        Self::unsafe_expr(Expr::Call {
                            binding: CallBinding::Generated,
                            func: Box::new(Expr::Path(Path::new(
                                ["std", "ptr", "read_unaligned"].map(Ident::from),
                            ))),
                            args: vec![arg],
                        })
                    } else {
                        arg
                    };
                    i += 1;
                    arg
                };
                boxed.push(Expr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(Expr::Var("__SlateVaArg::new".into())),
                    args: vec![arg],
                });
            }
            args.push(Expr::Call {
                binding: CallBinding::Generated,
                func: Box::new(Expr::Var("__SlateVaArgs::new".into())),
                args: vec![Expr::VecLit(boxed)],
            });
        }
        let call = Expr::Call {
            binding,
            func: Box::new(callee_expr),
            args: if let Some(param_types) = self.parent.externs.get(&callee_name).cloned() {
                args.into_iter()
                    .enumerate()
                    .map(|(i, arg)| match param_types.get(i) {
                        Some(_)
                            if arg_types
                                .get(i)
                                .is_some_and(|t| is_cir_function_pointer_type(t)) =>
                        {
                            arg
                        }
                        Some(Type::VaList) => Expr::MethodCall {
                            recv: Box::new(arg),
                            method: "clone".into(),
                            args: vec![],
                        },
                        Some(ty) => Expr::Cast {
                            expr: Box::new(arg),
                            ty: ty.clone(),
                        },
                        None => arg,
                    })
                    .collect()
            } else if let Some(param_types) = self.parent.function_param_types.get(&callee_name) {
                cast_void_ptr_call_args(args, arg_types, param_types)
            } else if let Some(param_types) = &indirect_param_types {
                cast_void_ptr_call_args(args, arg_types, param_types)
            } else {
                args
            },
        };
        let expr = if is_complex_runtime_call(&callee_name) {
            self.parent.uses_complex.set(true);
            Self::unsafe_expr(call)
        } else if self.parent.externs.contains_key(&callee_name)
            || self.parent.variadic_defs.contains(&callee_name)
            || self.parent.unsafe_functions.contains(&callee_name)
            || indirect_call
        {
            Self::unsafe_expr(call)
        } else {
            call
        };
        let expr = if cast_runtime_result {
            op_result_type(op).map_or(expr.clone(), |ty| Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(ty),
            })
        } else {
            expr
        };

        if let Some(result) = op.results.first() {
            let indirect_ret_ty = indirect_callee_operand
                .and_then(|operand| self.loaded_field_types.get(&operand))
                .and_then(|ty| match ty {
                    Type::FnPtr { ret, .. } => Some(ret.as_ref().clone()),
                    _ => None,
                });
            match indirect_ret_ty {
                Some(ty) => self.materialize_expr_as(result, expr, ty),
                None => self.materialize_expr(result, expr, op_result_type(op)),
            }
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
    }

    pub(super) fn try_format_call_shims(
        &mut self,
        op: &Op,
        callee_name: &str,
        args: &[Expr],
        arg_types: &[&str],
    ) -> bool {
        self.try_long_double_call_shim(op, callee_name, args, arg_types)
    }

    pub(super) fn try_long_double_call_shim(
        &mut self,
        op: &Op,
        callee_name: &str,
        args: &[Expr],
        arg_types: &[&str],
    ) -> bool {
        if crate::cir::emit::uses_f64_long_double_abi() {
            return false;
        }
        let cf80_shim = match callee_name {
            "__mulxc3" => Some("__slate_cf80_mul"),
            "__divxc3" => Some("__slate_cf80_div"),
            _ => None,
        };
        if let Some(shim) = cf80_shim {
            let Some(result) = op.results.first() else {
                return true;
            };
            if args.len() != 4 {
                return false;
            }
            self.parent.uses_complex.set(true);
            let lhs = Expr::StructLit {
                name: COMPLEX_TY.into(),
                fields: vec![
                    ("re".into(), args[0].clone()),
                    ("im".into(), args[1].clone()),
                ],
            };
            let rhs = Expr::StructLit {
                name: COMPLEX_TY.into(),
                fields: vec![
                    ("re".into(), args[2].clone()),
                    ("im".into(), args[3].clone()),
                ],
            };
            let call = Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(shim.into())),
                args: vec![lhs, rhs],
            };
            self.materialize_expr(result, call, op_result_type(op));
            return true;
        }
        if !self.parent.externs.contains_key(callee_name) {
            return false;
        }
        let ret_cir_ty = op_result_type(op);
        let has_long_double_arg = arg_types.iter().any(|ty| {
            self.parent
                .rust_type_has_long_double(&self.parent.rust_type(ty))
        });
        let has_long_double_ret = ret_cir_ty
            .map(|ty| self.parent.rust_type(ty))
            .is_some_and(|ty| self.parent.rust_type_has_long_double(&ty));
        if !has_long_double_arg && !has_long_double_ret {
            return false;
        }
        let mut param_types: Vec<Type> = arg_types
            .iter()
            .map(|ty| {
                if is_quad_long_double(ty) {
                    Type::Prim(Prim::F128)
                } else if is_long_double(ty) {
                    Type::LongDouble
                } else if let Some(inner) = cir_ptr_inner(ty)
                    && is_wrapped_long_double(inner)
                {
                    Type::Ptr {
                        mutable: true,
                        inner: Box::new(Type::LongDouble),
                    }
                } else {
                    self.parent.rust_type(ty)
                }
            })
            .collect();
        let mut ret = ret_cir_ty
            .map(|ty| self.parent.rust_type(ty))
            .filter(|ty| !ty.is_unit());
        let identity = self
            .parent
            .known_functions
            .get(callee_name)
            .copied()
            .unwrap_or(crate::function_identity::FunctionIdentity::Unknown);
        crate::frontend::function_abi::repair_function_signature(
            self.parent
                .function_types
                .get(callee_name)
                .map(String::as_str),
            &mut param_types,
            &mut ret,
        );
        let ret_shim_ty = ret.unwrap_or(Type::Unit);
        let ret_tag = if ret_cir_ty
            .is_some_and(|ty| is_complex_long_double_coercion_type(ty, &self.parent.aliases))
        {
            "cf80".to_string()
        } else {
            long_double_shim_type_tag(&ret_shim_ty)
        };
        let arg_tags: Vec<String> = arg_types
            .iter()
            .zip(param_types.iter())
            .map(|(ty, param_ty)| {
                if is_quad_long_double(ty) {
                    "lq".to_string()
                } else if is_long_double(ty) {
                    "f80".to_string()
                } else if cir_ptr_inner(ty).is_some_and(is_wrapped_long_double) {
                    "pf80".to_string()
                } else {
                    long_double_shim_type_tag(param_ty)
                }
            })
            .collect();
        let mut segments = vec![format!("r{ret_tag}")];
        segments.extend(arg_tags);
        let shim_name = format!("__slate_{callee_name}__{}", segments.join("_"));
        self.parent
            .long_double_shims
            .entry(shim_name.clone())
            .or_insert_with(|| ExternFnDecl {
                identity,
                name: shim_name.clone(),
                declared_type: self.parent.function_types.get(callee_name).cloned(),
                params: param_types
                    .iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("_{i}"),
                        mutable: false,
                        ty: ty.clone(),
                    })
                    .collect(),
                variadic: false,
                ret: (!ret_shim_ty.is_unit()).then_some(ret_shim_ty.clone()),
                safe: false,
            });
        let call_args = args
            .iter()
            .zip(arg_types.iter())
            .enumerate()
            .map(|(i, (arg, ty))| {
                if is_quad_long_double(ty)
                    || is_long_double(ty)
                    || cir_ptr_inner(ty).is_some_and(is_wrapped_long_double)
                    || is_cir_function_pointer_type(ty)
                {
                    arg.clone()
                } else {
                    Expr::Cast {
                        expr: Box::new(arg.clone()),
                        ty: param_types[i].clone(),
                    }
                }
            })
            .collect();
        let expr = Self::unsafe_expr(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var(shim_name.into())),
            args: call_args,
        });
        if let Some(result) = op.results.first() {
            self.materialize_expr(result, expr, op_result_type(op));
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
        true
    }

    pub(super) fn byte_ptr_operand(&self, operand: &str, mutable: bool) -> Expr {
        Expr::Cast {
            expr: Box::new(self.pointer_operand_expr(operand)),
            ty: Type::Ptr {
                mutable,
                inner: Box::new(Type::Prim(Prim::U8)),
            },
        }
    }

    pub(super) fn usize_operand(&self, operand: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(self.operand_expr(operand)),
            ty: Type::Prim(Prim::Usize),
        }
    }

    // cir.libc.memcpy/memmove: (dst, src, len). memmove keeps overlapping copy
    // semantics; both operate byte-wise via *u8 pointers.
    pub(super) fn lower_mem_copy(&mut self, op: &Op, overlapping: bool) {
        let known = if overlapping {
            crate::function_identity::Known::MemMove
        } else {
            crate::function_identity::Known::MemCpy
        };
        if !self.lower_known_libc_op(op, known) {
            return;
        }
        if op.operands.len() < 3 {
            return;
        }
        let dst = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[0], true));
        let src = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[1], false));
        let count = self.usize_operand(&op.operands[2]);
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::PtrCopy {
            src: Box::new(src),
            dst: Box::new(dst),
            count: Box::new(count),
            overlapping,
        })));
    }

    // cir.libc.memset: (dst, val:u8, len); the alignment attr carries no runtime
    // meaning here.
    pub(super) fn lower_mem_set(&mut self, op: &Op) {
        if !self.lower_known_libc_op(op, crate::function_identity::Known::MemSet) {
            return;
        }
        if op.operands.len() < 3 {
            return;
        }
        let dst = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[0], true));
        let val = Expr::Cast {
            expr: Box::new(self.operand_expr(&op.operands[1])),
            ty: Type::Prim(Prim::U8),
        };
        let count = self.usize_operand(&op.operands[2]);
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::WriteBytes {
            dst: Box::new(dst),
            val: Box::new(val),
            count: Box::new(count),
        })));
    }

    // cir.libc.memchr: (src, pattern:i32, len:u64) -> void*. Backed by a prelude
    // helper so the byte scan stays a single structured call site.
    pub(super) fn lower_mem_chr(&mut self, op: &Op) {
        if !self.lower_known_libc_op(op, crate::function_identity::Known::MemChr) {
            return;
        }
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        self.parent.uses_memchr.set(true);
        let src = self.pointer_operand_expr(&op.operands[0]);
        let pattern = Expr::Cast {
            expr: Box::new(self.operand_expr(&op.operands[1])),
            ty: Type::Prim(Prim::I32),
        };
        let len = self.usize_operand(&op.operands[2]);
        let call = Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var("__slate_memchr".into())),
            args: vec![src, pattern, len],
        };
        self.materialize_expr(result, call, op_result_type(op));
    }

    pub(super) fn lower_known_libc_op(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
    ) -> bool {
        let binding = self.parent.call_binding(op, true);
        if binding.known() == Some(known)
            || matches!(
                binding,
                crate::function_identity::CallBinding::Direct {
                    identity: FunctionIdentity::Unknown,
                    canonical_type: None,
                }
            )
            || self.parent.known_functions.get(known.symbol())
                == Some(&FunctionIdentity::Known(known))
        {
            return true;
        }
        let args = op
            .operands
            .iter()
            .map(|operand| self.operand_expr(operand))
            .collect();
        let call = Expr::Call {
            binding,
            func: Box::new(Expr::Var(known.symbol().into())),
            args,
        };
        let expr = if self.parent.externs.contains_key(known.symbol()) {
            Self::unsafe_expr(call)
        } else {
            call
        };
        if let Some(result) = op.results.first() {
            self.materialize_expr(result, expr, op_result_type(op));
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
        false
    }

    // Atomic ops lower to real `std::sync::atomic` operations viewed through
    // `AtomicN::from_ptr(store_address(ptr))`, so the existing integer slot is
    // accessed atomically without changing its storage. Integer/bool types map
    // to an atomic wrapper; float/pointer atomics fall back to a non-atomic RMW
    // (std has no atomic float, and atomic pointers need a different shape).
    pub(super) fn atomic_rust_type(&self, op: &Op) -> Type {
        op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32))
    }

    pub(super) fn lower_atomic_fetch(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let val = self.operand_expr(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        let binop = attr_int(op, "binop").unwrap_or(0);
        let Some(atomic_ty) = atomic_type(&ty) else {
            // float/pointer atomic: non-atomic read-modify-write fallback.
            self.lower_atomic_fetch_nonatomic(op, &result, val, ty, binop);
            return;
        };
        let fetched = Self::unsafe_expr(Expr::AtomicFetch {
            ty: atomic_ty,
            op: atomic_rmw_op(binop),
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
            value: Box::new(val.clone()),
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        });
        if attr_bool(op, "fetch_first") {
            self.materialize_expr(&result, fetched, op_result_type(op));
        } else {
            let old = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: old.clone(),
                mutable: false,
                ty: Some(ty),
                init: Some(fetched),
            });
            let new = atomic_combine(binop, Expr::Var(old.into()), val);
            self.materialize_expr(&result, new, op_result_type(op));
        }
    }

    pub(super) fn lower_atomic_fetch_nonatomic(
        &mut self,
        op: &Op,
        result: &str,
        val: Expr,
        ty: Type,
        binop: i64,
    ) {
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty.clone()),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        let new = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: new.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(atomic_combine(binop, Expr::Var(old.clone().into()), val)),
        });
        self.push_unsafe_assign(
            Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr),
            },
            Expr::Var(new.clone().into()),
        );
        let bound = if attr_bool(op, "fetch_first") {
            old
        } else {
            new
        };
        self.immutable_temps.insert(bound.clone());
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(bound.into())));
    }

    pub(super) fn lower_atomic_xchg(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let val = self.operand_expr(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        if let Some(atomic_ty) = atomic_type(&ty) {
            let expr = Self::unsafe_expr(Expr::AtomicSwap {
                ty: atomic_ty,
                place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
                value: Box::new(val),
                ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
            });
            self.materialize_expr(&result, expr, op_result_type(op));
            return;
        }
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        self.push_unsafe_assign(
            Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr),
            },
            val,
        );
        self.immutable_temps.insert(old.clone());
        self.values.insert(result, Val::Expr(Expr::Var(old.into())));
    }

    pub(super) fn lower_atomic_test_and_set(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let old = Self::unsafe_expr(Expr::AtomicSwap {
            ty: AtomicType::I8,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(Expr::Value(RustValue::I64(1))),
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        });
        self.materialize_expr(
            result,
            Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(old),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_atomic_clear(&mut self, op: &Op) {
        let Some(ptr) = op.operands.first() else {
            return;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::AtomicStore {
            ty: AtomicType::I8,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(Expr::Value(RustValue::I64(0))),
            ordering: store_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        })));
    }

    pub(super) fn lower_atomic_cmpxchg(&mut self, op: &Op) {
        if op.operands.len() < 3 || op.results.len() < 2 {
            return;
        }
        let expected = self.operand_expr(&op.operands[1]);
        let desired = self.operand_expr(&op.operands[2]);
        let ty = op_result_types(op)
            .first()
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        if let Some(atomic_ty) = atomic_type(&ty) {
            let res = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: res.clone(),
                mutable: false,
                ty: Some(Type::Generic {
                    name: "Result".into(),
                    args: vec![ty.clone(), ty.clone()],
                }),
                init: Some(Self::unsafe_expr(Expr::AtomicCompareExchange {
                    ty: atomic_ty,
                    place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
                    expected: Box::new(expected),
                    desired: Box::new(desired),
                    success: rust_ordering(attr_int(op, "succ_order").unwrap_or(5)),
                    failure: load_ordering(attr_int(op, "fail_order").unwrap_or(5)),
                })),
            });
            let old = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: old.clone(),
                mutable: false,
                ty: Some(ty.clone()),
                init: Some(Expr::Match {
                    expr: Box::new(Expr::Var(res.clone().into())),
                    arms: vec![
                        ExprMatchArm {
                            pattern: Pattern::TupleStruct {
                                name: "Ok".into(),
                                fields: vec![Pattern::Binding("v".into())],
                            },
                            value: Expr::Var("v".into()),
                        },
                        ExprMatchArm {
                            pattern: Pattern::TupleStruct {
                                name: "Err".into(),
                                fields: vec![Pattern::Binding("v".into())],
                            },
                            value: Expr::Var("v".into()),
                        },
                    ],
                }),
            });
            let ok = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: ok.clone(),
                mutable: false,
                ty: Some(Type::Prim(Prim::Bool)),
                init: Some(Expr::MethodCall {
                    recv: Box::new(Expr::Var(res.into())),
                    method: "is_ok".into(),
                    args: vec![],
                }),
            });
            self.immutable_temps.insert(old.clone());
            self.immutable_temps.insert(ok.clone());
            self.values
                .insert(op.results[0].clone(), Val::Expr(Expr::Var(old.into())));
            self.values
                .insert(op.results[1].clone(), Val::Expr(Expr::Var(ok.into())));
            return;
        }
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        let ok = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        self.push_stmt(Stmt::Let {
            name: ok.clone(),
            mutable: false,
            ty: Some(Type::Prim(Prim::Bool)),
            init: Some(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Var(old.clone().into())),
                rhs: Box::new(expected),
            }),
        });
        self.push_stmt(Stmt::If {
            cond: Expr::Var(ok.clone().into()),
            then_body: vec![Self::indent_stmt(Self::unsafe_stmt(Self::assign_stmt(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(addr),
                },
                desired,
            )))],
            else_body: Vec::new(),
        });
        self.immutable_temps.insert(old.clone());
        self.immutable_temps.insert(ok.clone());
        self.values
            .insert(op.results[0].clone(), Val::Expr(Expr::Var(old.into())));
        self.values
            .insert(op.results[1].clone(), Val::Expr(Expr::Var(ok.into())));
    }

    pub(super) fn lower_atomic_fence(&mut self, op: &Op) {
        let ordering = attr_int(op, "ordering").unwrap_or(5);
        if ordering == 0 {
            return;
        }
        self.push_stmt(Stmt::Expr(Expr::AtomicFence {
            ordering: rust_ordering(ordering),
        }));
    }

    pub(super) fn atomic_load_expr(&self, op: &Op, ptr: &str) -> Option<Expr> {
        let mem_order = attr_int(op, "mem_order")?;
        let ty = op_result_type(op).map(|ty| self.parent.rust_type(ty))?;
        let atomic_ty = atomic_type(&ty)?;
        Some(Self::unsafe_expr(Expr::AtomicLoad {
            ty: atomic_ty,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            ordering: load_ordering(mem_order),
        }))
    }

    pub(super) fn try_atomic_store(
        &mut self,
        op: &Op,
        ptr: &str,
        value_ty: Option<&str>,
        value: Expr,
    ) -> bool {
        let Some(mem_order) = attr_int(op, "mem_order") else {
            return false;
        };
        let Some(wrapper) = value_ty
            .map(|ty| self.parent.rust_type(ty))
            .as_ref()
            .and_then(atomic_type)
        else {
            return false;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::AtomicStore {
            ty: wrapper,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(value),
            ordering: store_ordering(mem_order),
        })));
        true
    }

    pub(super) fn lower_va_start(&mut self, op: &Op) {
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(place) = self.va_target_place(ptr) else {
            return;
        };
        let args = self
            .va_args_param
            .clone()
            .unwrap_or_else(|| "__slate_va_args".into());
        self.push_unsafe_assign(
            place,
            Expr::MethodCall {
                recv: Box::new(Expr::Var(args.into())),
                method: "clone".into(),
                args: vec![],
            },
        );
    }

    pub(super) fn lower_va_arg(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(place) = self.va_target_place(ptr) else {
            return;
        };
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        self.materialize_expr(
            result,
            Self::unsafe_expr(Expr::MethodCallGeneric {
                recv: Box::new(place),
                method: "next_arg".into(),
                type_args: vec![ty],
                args: vec![],
            }),
            op_result_type(op),
        );
    }

    pub(super) fn lower_va_copy(&mut self, op: &Op) {
        let [dst, src] = op.operands.as_slice() else {
            return;
        };
        let (Some(dst_place), Some(src_place)) =
            (self.va_target_place(dst), self.va_target_place(src))
        else {
            return;
        };
        self.push_unsafe_assign(
            dst_place,
            Expr::MethodCall {
                recv: Box::new(src_place),
                method: "clone".into(),
                args: vec![],
            },
        );
    }
}
