use super::*;

pub(super) fn builtin_library_name(builtin: &str) -> Option<&'static str> {
    Some(match builtin {
        "__builtin_abort" => "abort",
        "__builtin_nan" => "nan",
        "__builtin_nanf" => "nanf",
        "__builtin_nanl" => "nanl",
        "__builtin_memcpy" => "memcpy",
        "__builtin_memmove" => "memmove",
        "__builtin_memset" => "memset",
        "__builtin_memcmp" => "memcmp",
        _ => return None,
    })
}

pub(super) fn main_wrapper(arity: usize) -> Item {
    let call = |path: &str, args: Vec<Expr>| Expr::Call {
        func: Box::new(Expr::Var(path.into())),
        args,
        binding: CallBinding::Generated,
    };
    let method = |recv: Expr, name: &str, args: Vec<Expr>| Expr::MethodCall {
        recv: Box::new(recv),
        method: name.into(),
        args,
    };
    let inferred = || rust::Type::Custom("_".into());
    let mut body = Vec::new();
    if arity > 0 {
        body.extend([
            Stmt::Let {
                name: "__slate_argv_storage".into(),
                mutable: false,
                ty: Some(rust::Type::Generic {
                    name: "Vec".into(),
                    args: vec![rust::Type::Custom("std::ffi::CString".into())],
                }),
                init: Some(method(
                    method(
                        call("std::env::args", Vec::new()),
                        "map",
                        vec![Expr::Closure {
                            params: vec!["arg".into()],
                            body: Box::new(method(
                                call("std::ffi::CString::new", vec![Expr::Var("arg".into())]),
                                "unwrap",
                                Vec::new(),
                            )),
                        }],
                    ),
                    "collect",
                    Vec::new(),
                )),
            },
            Stmt::Let {
                name: "__slate_argv".into(),
                mutable: true,
                ty: Some(rust::Type::Generic {
                    name: "Vec".into(),
                    args: vec![inferred()],
                }),
                init: Some(method(
                    method(
                        method(Expr::Var("__slate_argv_storage".into()), "iter", Vec::new()),
                        "map",
                        vec![Expr::Closure {
                            params: vec!["arg".into()],
                            body: Box::new(Expr::Cast {
                                expr: Box::new(method(
                                    Expr::Var("arg".into()),
                                    "as_ptr",
                                    Vec::new(),
                                )),
                                ty: inferred(),
                            }),
                        }],
                    ),
                    "collect",
                    Vec::new(),
                )),
            },
            Stmt::Expr(method(
                Expr::Var("__slate_argv".into()),
                "push",
                vec![call("std::ptr::null_mut", Vec::new())],
            )),
        ]);
    }
    let args = (0..arity)
        .map(|index| match index {
            0 => Expr::Cast {
                expr: Box::new(method(
                    Expr::Var("__slate_argv_storage".into()),
                    "len",
                    Vec::new(),
                )),
                ty: inferred(),
            },
            _ => method(Expr::Var("__slate_argv".into()), "as_mut_ptr", Vec::new()),
        })
        .collect();
    body.push(Stmt::Expr(call(
        "std::process::exit",
        vec![call("__slate_main", args)],
    )));
    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: rust::Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "main".into(),
        params: Vec::new(),
        ret: None,
        body,
    })
}

impl Tables<'_> {
    pub(super) fn builtin_name(&self, id: BindingId) -> Option<&str> {
        self.names.get(&id)?.builtin.as_deref()
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_extern(
        &mut self,
        function: &slate_parser::ast::Span<ir::Function>,
    ) -> Result<rust::ExternDecl> {
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "unprototyped declaration".into(),
            }
            .into());
        };
        if self.tables.function_has_vector(function) {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "vector C ABI".into(),
            }
            .into());
        }
        let ret = self.lower_return(function)?;
        let attrs = self
            .tables
            .builtin_name(function.value.id)
            .and_then(builtin_library_name)
            .map(|name| Attr::LinkName(name.into()))
            .into_iter()
            .collect();
        Ok(rust::ExternDecl::Fn(rust::ExternFnDecl {
            attrs,
            name: function.name.clone(),
            identity: FunctionIdentity::Unknown,
            declared_type: None,
            trusted_headers: Default::default(),
            params: fixed
                .iter()
                .map(|parameter| {
                    Ok(FnParam {
                        name: binding_name(parameter.value.id, &self.tables.bindings),
                        mutable: false,
                        ty: self
                            .lower_type(&parameter.ty)
                            .map_err(|e| e.used_at(Site::of(parameter)))?,
                    })
                })
                .collect::<Result<Vec<_>>>()?,
            variadic: *variadic,
            ret,
            safe: false,
        }))
    }

    pub(super) fn lower_function(
        &mut self,
        function: &slate_parser::ast::Span<ir::Function>,
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Item> {
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "unprototyped definition".into(),
            }
            .into());
        };
        let ret = self.lower_return(function)?;
        let mut params = fixed
            .iter()
            .map(|param| {
                Ok(FnParam {
                    name: binding_name(param.value.id, &self.tables.bindings),
                    mutable: true,
                    ty: self
                        .lower_type(&param.ty)
                        .map_err(|e| e.used_at(Site::of(param)))?,
                })
            })
            .collect::<Result<Vec<_>>>()?;
        if *variadic {
            params.push(FnParam {
                name: VA_ARGS.into(),
                mutable: true,
                ty: rust::Type::Variadic,
            });
        }
        let mut statements = if control_flow::needs_dispatch(body) {
            self.lower_dispatch(body)?
        } else {
            self.lower_statement_list(body)?
        };
        statements.splice(0..0, std::mem::take(&mut self.hoisted));
        if matches!(function.fallthrough, Some(ir::Fallthrough::ReturnZero))
            && !matches!(statements.last(), Some(Stmt::Return(_)))
        {
            statements.push(Stmt::Return(Some(Expr::Value(rust::RustValue::I64(0)))));
        }
        if matches!(function.fallthrough, Some(ir::Fallthrough::UndefinedIfUsed))
            && !matches!(statements.last(), Some(Stmt::Return(_)))
        {
            statements.push(Stmt::Return(Some(zeroed())));
        }
        let target_feature = self.tables.target_feature_attr(function)?;
        Ok(Item::Fn(FnDef {
            unsafe_: *variadic || target_feature.is_some(),
            attrs: target_feature.into_iter().collect(),
            vis: rust::Visibility::Private,
            abi: variadic.then_some(rust::Abi::CUnwind),
            name: self.tables.names[&function.value.id].rust.clone(),
            params,
            ret,
            body: statements,
        }))
    }

    fn lower_return(
        &mut self,
        function: &slate_parser::ast::Span<ir::Function>,
    ) -> Result<Option<rust::Type>> {
        let Some(ty) = &function.return_type else {
            return Ok(None);
        };
        let returns = self.tables.metadata.get(&function.id).and_then(|entries| {
            entries
                .iter()
                .find(|(key, _)| key == "c_return")
                .map(|(_, value)| value.clone())
        });
        self.lower_type(ty)
            .map(Some)
            .map_err(|e| e.returned_by(Site::of(function), returns))
    }
}
