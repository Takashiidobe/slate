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
        let names = &self.tables.names;
        let func = match callee {
            ir::Callee::Direct(id) => {
                let name = names.get(id).ok_or(Invariant::UnknownCallee(*id))?;
                if name.is_extern
                    && (name.codegen_builtin || name.rust.starts_with("__builtin_"))
                    && name.library.is_none()
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
        let call = Expr::Call {
            func: Box::new(func),
            args: arguments
                .iter()
                .enumerate()
                .map(|(index, argument)| {
                    let lowered = self.lower_value(argument)?;
                    let ty = self.lower_type(&argument.ty)?;
                    let variadic = variadic_start.is_some_and(|start| index >= start);
                    Ok(match ty {
                        rust::Type::FnPtr { .. } if variadic => {
                            convert_function_pointer(ty, byte_pointer_type(), Box::new(lowered))
                        }
                        rust::Type::Prim(Prim::I8 | Prim::I16) if variadic => Expr::Cast {
                            expr: Box::new(lowered),
                            ty: rust::Type::Prim(Prim::I32),
                        },
                        rust::Type::Prim(Prim::U8 | Prim::U16) if variadic => Expr::Cast {
                            expr: Box::new(lowered),
                            ty: rust::Type::Prim(Prim::U32),
                        },
                        _ => lowered,
                    })
                })
                .collect::<Result<Vec<_>>>()?,
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
        let unsafe_call = match callee {
            ir::Callee::Direct(id) => names.get(id).is_some_and(|name| name.is_unsafe),
            ir::Callee::Indirect(_) => true,
        };
        Ok(if unsafe_call {
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(call)),
            }))
        } else {
            call
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
                    FunctionIdentity::Unknown => CallBinding::Generated,
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
                safe: false,
            });
    }
}
