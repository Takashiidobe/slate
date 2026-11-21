use super::*;

pub(super) const VA_ARGS: &str = "__va_args";

pub(super) fn static_va_list(list: Expr, target: rust::Type) -> Expr {
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::GenericPath {
                path: rust::Path::new(["core", "mem", "transmute"].map(rust::Ident::from)),
                type_args: vec![rust::Type::Custom("core::ffi::VaList<'_>".into()), target],
            }),
            args: vec![clone_va_list(list)],
            binding: CallBinding::Generated,
        })),
    }))
}

pub(super) fn through_raw_pointer(place: Expr) -> Expr {
    Expr::Unary {
        op: rust::UnaryOp::Deref,
        expr: Box::new(Expr::AddrOf {
            mutable: true,
            expr: Box::new(place),
        }),
    }
}

pub(super) fn clone_va_list(list: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(list),
        method: "clone".into(),
        args: Vec::new(),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_call(
        &mut self,
        callee: &ir::Callee,
        arguments: &[ir::Value],
        signature: &ir::Type,
    ) -> Result<Expr> {
        if let ir::Callee::Indirect(pointer) = callee
            && matches!(
                self.tables.resolve_type(&pointer.ty),
                ir::Type::BlockPointer { .. }
            )
        {
            return self.lower_block_call(pointer, arguments, signature);
        }
        let names = &self.tables.names;
        let func = match callee {
            ir::Callee::Direct(id) if self.tables.stdio.functions.contains_key(id) => {
                stdio::support_path(self.tables.stdio.functions[id])
            }
            ir::Callee::Direct(id) => {
                let name = names.get(id).ok_or(Invariant::UnknownCallee(*id))?;
                if name.is_extern
                    && (name.codegen_builtin || name.rust.starts_with("__builtin_"))
                    && name.library.is_none()
                    && !self.tables.support_functions.contains_key(id)
                {
                    return Err(Construct::Function {
                        name: name.rust.clone(),
                        detail: "builtin without a lowering".into(),
                    }
                    .into());
                }
                if name.weak_extern {
                    Expr::MethodCall {
                        recv: Box::new(Expr::Var(name.rust.as_str().into())),
                        method: "unwrap".into(),
                        args: Vec::new(),
                    }
                } else {
                    Expr::Var(name.rust.as_str().into())
                }
            }
            ir::Callee::Indirect(pointer) => {
                let mut recv = self.lower_value(pointer)?;
                if let ir::Type::Function {
                    prototyped: false,
                    return_type,
                    ..
                } = self.tables.resolve_type(signature)
                {
                    let from = self.lower_type(&pointer.ty)?;
                    let rust::Type::FnPtr { abi, .. } = from else {
                        return Err(unsupported_type(&pointer.ty));
                    };
                    let params = arguments
                        .iter()
                        .map(|argument| self.lower_type(&argument.ty))
                        .collect::<Result<Vec<_>>>()?;
                    let ret = match return_type {
                        Some(ret) => self.lower_type(ret)?,
                        None => rust::Type::Unit,
                    };
                    let to = rust::Type::FnPtr {
                        abi,
                        params,
                        ret: Box::new(ret),
                    };
                    if to != from {
                        recv = Expr::Transmute {
                            from,
                            to,
                            expr: Box::new(recv),
                        };
                    }
                }
                Expr::MethodCall {
                    recv: Box::new(recv),
                    method: "unwrap".into(),
                    args: Vec::new(),
                }
            }
        };
        let variadic_start = match self.tables.resolve_type(signature) {
            ir::Type::Function {
                parameters,
                variadic: true,
                ..
            } => Some(parameters.len()),
            _ => None,
        };
        let mut args = match callee {
            ir::Callee::Direct(id) => self.nested_capture_arguments(*id)?,
            ir::Callee::Indirect(_) => Vec::new(),
        };
        let mut borrowed = Vec::new();
        let mut kept: Vec<usize> = (0..args.len()).collect();
        for (index, argument) in arguments.iter().enumerate() {
            if matches!(self.tables.resolve_type(&argument.ty), ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }))
            {
                kept.push(args.len());
            }
            let variadic = variadic_start.is_some_and(|start| index >= start);
            let borrow = match callee {
                ir::Callee::Direct(id) if !variadic => self.tables.parameter_borrow(*id, index),
                ir::Callee::Direct(_) | ir::Callee::Indirect(_) => None,
            };
            if let Some(borrow) = borrow {
                args.push(self.lower_borrow(argument, borrow)?);
                borrowed.push((args.len() - 1, borrow.mutable));
                continue;
            }
            let mut lowered = self.lower_call_argument(argument, variadic)?;
            if !variadic
                && matches!(callee, ir::Callee::Direct(id) if self.tables.support_functions.contains_key(id))
            {
                let ir::Callee::Direct(id) = callee else {
                    unreachable!()
                };
                let expected =
                    crate::libc_catalog::parameters(&self.tables.support_functions[id].shape);
                let expected = expected[index].to_string();
                lowered = self.support_argument(argument, lowered, &expected)?;
            }
            args.push(lowered);
        }
        let mut evaluated = Vec::new();
        if !borrowed.is_empty() {
            let exclusive = borrowed.iter().any(|&(_, mutable)| mutable);
            for (index, arg) in args.iter_mut().enumerate() {
                let trivial = match arg {
                    Expr::Value(_) => true,
                    Expr::Var(_) => !exclusive,
                    Expr::Cast { expr, .. } => {
                        matches!(**expr, Expr::Value(_))
                            || (!exclusive && matches!(**expr, Expr::Var(_)))
                    }
                    #[expect(
                        slate::sensitive_wildcard,
                        reason = "any other argument may read an object a sibling argument borrows"
                    )]
                    _ => false,
                };
                if borrowed.iter().any(|&(borrow, _)| borrow == index)
                    || kept.contains(&index)
                    || trivial
                {
                    continue;
                }
                let temp = self.next_temp();
                evaluated.push(Stmt::Let {
                    name: temp.clone(),
                    mutable: false,
                    ty: None,
                    init: Some(std::mem::replace(arg, Expr::Var(temp.as_str().into()))),
                });
            }
        }
        let names = &self.tables.names;
        let call = Expr::Call {
            func: Box::new(func),
            args,
            binding: match callee {
                ir::Callee::Direct(id) => CallBinding::Direct {
                    identity: names[id].identity,
                    canonical_type: None,
                    trusted_declaration: None,
                    trusted_headers: Default::default(),
                },
                ir::Callee::Indirect(_) => CallBinding::Indirect,
            },
        };
        let call = if matches!(callee, ir::Callee::Direct(id) if self.tables.support_functions.contains_key(id))
        {
            let ir::Callee::Direct(id) = callee else {
                unreachable!()
            };
            let vector_return = self.tables.support_functions[id]
                .shape
                .rsplit_once(" -> ")
                .map(|(_, ret)| ret.to_string())
                .filter(|ret| crate::libc_catalog::is_vector_path(ret));
            if let Some(vector) = vector_return
                && let ir::Type::Function {
                    return_type: Some(ret),
                    ..
                } = self.tables.resolve_type(signature)
            {
                Expr::Transmute {
                    from: rust::Type::Custom(vector),
                    to: self.lower_type(ret)?,
                    expr: Box::new(call),
                }
            } else if let ir::Type::Function {
                return_type: Some(ret),
                ..
            } = self.tables.resolve_type(signature)
                && support_result_needs_cast(self.tables.resolve_type(ret))
                && !matches!(callee, ir::Callee::Direct(id) if self.tables.support_functions[id].shape.ends_with(" -> !"))
            {
                Expr::Cast {
                    expr: Box::new(call),
                    ty: {
                        let c = if let ir::Callee::Direct(id) = callee {
                            self.tables.c_types.bindings.get(id).and_then(|c| {
                                match self.tables.c_types.types.canonical_kind(*c) {
                                    slate_parser::sema::ctype::CTypeKind::Function {
                                        ret, ..
                                    } => Some(*ret),
                                    _ => None,
                                }
                            })
                        } else {
                            None
                        };
                        self.lower_c_type(ret, c)?
                    },
                }
            } else {
                call
            }
        } else {
            call
        };
        let unsafe_call = match callee {
            ir::Callee::Direct(id) => names.get(id).is_some_and(|name| name.is_unsafe),
            ir::Callee::Indirect(_) => true,
        };
        let call = if unsafe_call {
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(call)),
            }))
        } else {
            call
        };
        Ok(if evaluated.is_empty() {
            call
        } else {
            Expr::Block(Box::new(rust::Block {
                stmts: evaluated,
                tail: Some(Box::new(call)),
            }))
        })
    }

    pub(super) fn lower_call_argument(
        &mut self,
        argument: &ir::Value,
        variadic: bool,
    ) -> Result<Expr> {
        let lowered = self.lower_value(argument)?;
        if !variadic {
            return Ok(lowered);
        }
        let ty = self.lower_type(&argument.ty)?;
        Ok(match ty {
            rust::Type::FnPtr { .. } => {
                convert_function_pointer(ty, byte_pointer_type(), Box::new(lowered))
            }
            rust::Type::Prim(Prim::I8 | Prim::I16) => Expr::Cast {
                expr: Box::new(lowered),
                ty: rust::Type::Prim(Prim::I32),
            },
            rust::Type::Prim(Prim::U8 | Prim::U16) => Expr::Cast {
                expr: Box::new(lowered),
                ty: rust::Type::Prim(Prim::U32),
            },
            _ => lowered,
        })
    }

    pub(super) fn call_c_bridge(
        &mut self,
        name: String,
        params: Vec<rust::Type>,
        ret: rust::Type,
        args: Vec<Expr>,
        identity: FunctionIdentity,
    ) -> Expr {
        self.declare_c_bridge(name.clone(), params, ret, identity);
        Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(Expr::Var(name.into())),
                args,
                binding: match identity {
                    FunctionIdentity::Known(_) => CallBinding::Direct {
                        identity,
                        canonical_type: None,
                        trusted_declaration: None,
                        trusted_headers: Default::default(),
                    },
                    FunctionIdentity::HeaderInline | FunctionIdentity::Unknown => {
                        CallBinding::Generated
                    }
                },
            })),
        }))
    }

    pub(super) fn declare_c_bridge(
        &mut self,
        name: String,
        params: Vec<rust::Type>,
        ret: rust::Type,
        identity: FunctionIdentity,
    ) {
        self.dependencies
            .bridges
            .entry(name.clone())
            .or_insert_with(|| rust::ExternFnDecl {
                attrs: Vec::new(),
                name: name.clone(),
                identity,
                declared_type: None,
                trusted_headers: Default::default(),
                params: params
                    .into_iter()
                    .enumerate()
                    .map(|(index, ty)| FnParam {
                        comments: Vec::new(),
                        name: format!("_{index}"),
                        mutable: false,
                        ty,
                    })
                    .collect(),
                variadic: false,
                ret: (!matches!(ret, rust::Type::Unit)).then_some(ret),
                noreturn: false,
                safe: false,
            });
    }
}

#[expect(
    slate::sensitive_wildcard,
    reason = "support functions return their Rust spelling of a number or pointer, cast to the C type; any other result type is returned as is"
)]
fn support_result_needs_cast(ty: &ir::Type) -> bool {
    matches!(ty, ir::Type::Numeric(_) | ir::Type::Pointer { .. })
}
