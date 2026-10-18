use super::*;
use slate_parser::target_info::TargetFamily;

pub(super) fn codegen_builtin(metadata: &[(String, String)]) -> bool {
    let entry = |name: &str| {
        metadata
            .iter()
            .find_map(|(key, value)| (key == name).then_some(value.as_str()))
    };
    entry("c_builtin_kind").is_some_and(|kind| kind != "library")
        || entry("c_builtin_header").is_some_and(|header| header.ends_with("intrin.h"))
}

pub(super) fn main_wrapper(arity: usize, huge_stack: bool) -> Item {
    let c_strings = || rust::Type::Ptr {
        mutable: true,
        inner: Box::new(rust::Type::Ptr {
            mutable: true,
            inner: Box::new(rust::Type::Custom("std::ffi::c_char".into())),
        }),
    };
    let params = [
        ("argc", rust::Type::Prim(rust::Prim::I32)),
        ("argv", c_strings()),
        ("envp", c_strings()),
    ]
    .into_iter()
    .map(|(name, ty)| FnParam {
        comments: Vec::new(),
        name: name.into(),
        mutable: false,
        ty,
    })
    .collect::<Vec<_>>();
    let args = params[..arity]
        .iter()
        .map(|param| Expr::Cast {
            expr: Box::new(Expr::Var(param.name.as_str().into())),
            ty: rust::Type::Custom("_".into()),
        })
        .collect();
    let call = Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var("__slate_main".into())),
            args,
            binding: CallBinding::Generated,
        })),
    }));
    let mut body = Vec::new();
    let call = if huge_stack {
        for name in &params[1..arity.max(1)] {
            body.push(Stmt::Let {
                name: name.name.clone(),
                mutable: false,
                ty: None,
                init: Some(Expr::Cast {
                    expr: Box::new(Expr::Var(name.name.as_str().into())),
                    ty: rust::Type::Prim(rust::Prim::Usize),
                }),
            });
        }
        let method = |recv: Expr, method: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: method.into(),
            args,
        };
        let builder = method(
            Expr::Call {
                func: Box::new(Expr::Var("std::thread::Builder::new".into())),
                args: Vec::new(),
                binding: CallBinding::Generated,
            },
            "stack_size",
            vec![Expr::Value(rust::RustValue::Usize(1 << 30))],
        );
        let spawned = method(
            builder,
            "spawn_scoped",
            vec![
                Expr::Var("s".into()),
                Expr::Closure {
                    params: Vec::new(),
                    body: Box::new(call),
                },
            ],
        );
        Expr::Call {
            func: Box::new(Expr::Var("std::thread::scope".into())),
            args: vec![Expr::Closure {
                params: vec!["s".into()],
                body: Box::new(method(
                    method(method(spawned, "unwrap", Vec::new()), "join", Vec::new()),
                    "unwrap",
                    Vec::new(),
                )),
            }],
            binding: CallBinding::Generated,
        }
    } else {
        call
    };
    body.push(Stmt::Return(Some(call)));
    Item::Fn(FnDef {
        comments: Vec::new(),
        attrs: vec![Attr::NoMangle],
        vis: rust::Visibility::Private,
        unsafe_: true,
        abi: Some(rust::Abi::CUnwind),
        name: "main".into(),
        params,
        ret: Some(rust::Type::Prim(rust::Prim::I32)),
        body,
    })
}

impl Tables<'_> {
    pub(super) fn builtin_name(&self, id: BindingId) -> Option<&str> {
        self.names.get(&id)?.builtin.as_deref()
    }

    pub(super) fn builtin_library(&self, id: BindingId) -> Option<&str> {
        self.names.get(&id)?.library.as_deref()
    }

    pub(super) fn convention_abi(&self, convention: ir::CallConv) -> Option<rust::Abi> {
        match (convention, self.target.family) {
            (ir::CallConv::C, _) => Some(rust::Abi::CUnwind),
            (ir::CallConv::Win64, TargetFamily::X86_64) => Some(rust::Abi::Win64Unwind),
            (ir::CallConv::SysV64, TargetFamily::X86_64) => Some(rust::Abi::SysV64Unwind),
            _ => None,
        }
    }

    pub(super) fn function_abi(&self, function: &ir::Function) -> Result<rust::Abi> {
        let convention = function
            .abi
            .as_ref()
            .map_or(ir::CallConv::C, |abi| abi.calling);
        let unsupported = |detail: String| -> Result<rust::Abi> {
            Err(Construct::Function {
                name: function.name.clone(),
                detail,
            }
            .into())
        };
        let Some(abi) = self.convention_abi(convention) else {
            return unsupported(format!("{convention} calling convention"));
        };
        if abi == rust::Abi::CUnwind {
            return Ok(abi);
        }
        if self.passes_long_double(function) {
            return unsupported(format!(
                "long double under the {convention} calling convention"
            ));
        }
        if function.body.is_some()
            && matches!(
                function.parameters,
                ir::Parameters::Prototype { variadic: true, .. }
            )
        {
            return unsupported(format!("variadic {convention} definition"));
        }
        Ok(abi)
    }
}

pub(super) fn ifunc_symbol(function: &ir::Function) -> Option<String> {
    function.symbol.ifunc.as_ref()?;
    Some(match function.linkage {
        ir::Linkage::External => function.name.clone(),
        _ => format!("__slate_ifunc_{}", function.name),
    })
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_extern(
        &mut self,
        function: &slate_parser::ast::Span<ir::Function>,
    ) -> Result<(rust::Abi, rust::ExternDecl)> {
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "unprototyped declaration".into(),
            }
            .into());
        };
        let abi = self.tables.function_abi(function)?;
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
            .builtin_library(function.value.id)
            .map(str::to_owned)
            .or_else(|| ifunc_symbol(function))
            .or_else(|| {
                (function_rust_name(function) != function.name).then(|| function.name.clone())
            })
            .map(Attr::LinkName)
            .into_iter()
            .collect();
        if function.symbol.weak {
            let mut params = fixed
                .iter()
                .map(|parameter| self.lower_type(&parameter.ty))
                .collect::<Result<Vec<_>>>()?;
            if *variadic {
                params.push(rust::Type::Variadic);
            }
            return Ok((
                abi,
                rust::ExternDecl::Static {
                    attrs: vec![Attr::ExternWeakLinkage],
                    mutable: false,
                    name: function.name.clone(),
                    ty: rust::Type::FnPtr {
                        abi,
                        params,
                        ret: Box::new(ret.unwrap_or(rust::Type::Unit)),
                    },
                },
            ));
        }
        let decl = rust::ExternDecl::Fn(rust::ExternFnDecl {
            attrs,
            name: function.name.clone(),
            identity: self
                .tables
                .names
                .get(&function.value.id)
                .map_or(FunctionIdentity::Unknown, |name| name.identity),
            declared_type: None,
            trusted_headers: Default::default(),
            params: fixed
                .iter()
                .map(|parameter| {
                    Ok(FnParam {
                        comments: Vec::new(),
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
        });
        Ok((abi, decl))
    }

    pub(super) fn lower_function(
        &mut self,
        function: &slate_parser::ast::Span<ir::Function>,
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Item> {
        let fixed = types::fixed_parameters(&function.parameters);
        let variadic = matches!(
            function.parameters,
            ir::Parameters::Prototype { variadic: true, .. }
        );
        let interrupt = function.semantics.interrupt;
        let abi = if interrupt {
            self.dependencies.x86_interrupt = true;
            rust::Abi::X86Interrupt
        } else {
            self.tables.function_abi(function)?
        };
        self.target_features = self.tables.enabled_target_features(function)?;
        let ret = self.lower_return(function)?;
        let mut params = fixed
            .iter()
            .map(|param| {
                Ok(FnParam {
                    comments: self.claim_comments(param.id),
                    name: binding_name(param.value.id, &self.tables.bindings),
                    mutable: true,
                    ty: self
                        .lower_type(&param.ty)
                        .map_err(|e| e.used_at(Site::of(param)))?,
                })
            })
            .collect::<Result<Vec<_>>>()?;
        if variadic {
            params.push(FnParam {
                comments: Vec::new(),
                name: VA_ARGS.into(),
                mutable: true,
                ty: rust::Type::Variadic,
            });
        }
        if function.semantics.naked {
            for param in &mut params {
                param.mutable = false;
            }
            return Ok(Item::Fn(FnDef {
                comments: self.claim_comments(function.id),
                unsafe_: variadic,
                attrs: vec![rust::Attr::Naked],
                vis: rust::Visibility::Private,
                abi: Some(abi),
                name: self.tables.names[&function.value.id].rust.clone(),
                params,
                ret,
                body: vec![self.lower_naked_body(body)?],
            }));
        }
        let mut statements = if control_flow::needs_dispatch(body) {
            self.lower_dispatch(body)?
        } else {
            self.lower_statement_list(body)?
        };
        statements.extend(self.remaining_statement_comments(body));
        statements.splice(0..0, std::mem::take(&mut self.hoisted));
        if self.uses_alloca {
            statements.insert(
                0,
                Stmt::Let {
                    name: "__slate_alloca_mark".into(),
                    mutable: false,
                    ty: None,
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Var("__slate_alloca::Mark::new".into())),
                        args: Vec::new(),
                        binding: CallBinding::Generated,
                    }),
                },
            );
        }
        if interrupt
            && let Some(frame) = params.first_mut()
            && let rust::Type::Ptr { inner, .. } = &frame.ty
        {
            let pointee = (**inner).clone();
            let original = std::mem::replace(&mut frame.name, "__slate_interrupt_frame".into());
            frame.ty = pointee.clone();
            statements.insert(
                0,
                Stmt::Let {
                    name: original,
                    mutable: false,
                    ty: Some(rust::Type::Ptr {
                        mutable: true,
                        inner: Box::new(pointee),
                    }),
                    init: Some(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var("__slate_interrupt_frame".into())),
                    }),
                },
            );
        }
        if matches!(function.fallthrough, Some(ir::Fallthrough::ReturnZero))
            && !matches!(
                statements
                    .iter()
                    .rev()
                    .find(|statement| !matches!(statement, Stmt::Comment(_))),
                Some(Stmt::Return(_))
            )
        {
            statements.push(Stmt::Return(Some(Expr::Value(rust::RustValue::I64(0)))));
        }
        if matches!(function.fallthrough, Some(ir::Fallthrough::UndefinedIfUsed))
            && !matches!(
                statements
                    .iter()
                    .rev()
                    .find(|statement| !matches!(statement, Stmt::Comment(_))),
                Some(Stmt::Return(_))
            )
        {
            statements.push(Stmt::Return(Some(zeroed())));
        }
        if let Some(ir::Fallthrough::Return(value)) = &function.fallthrough
            && !matches!(
                statements
                    .iter()
                    .rev()
                    .find(|statement| !matches!(statement, Stmt::Comment(_))),
                Some(Stmt::Return(_))
            )
        {
            let returned = Stmt::Return(Some(self.lower_value(value)?));
            statements.push(if control_flow::needs_dispatch(body) {
                Stmt::Unsafe {
                    body: rust::Block {
                        stmts: vec![returned],
                        tail: None,
                    },
                }
            } else {
                returned
            });
        }
        let target_feature = self.tables.target_feature_attr(function)?;
        Ok(Item::Fn(FnDef {
            comments: self.claim_comments(function.id),
            unsafe_: variadic || target_feature.is_some(),
            attrs: target_feature.into_iter().collect(),
            vis: rust::Visibility::Private,
            abi: (variadic || abi != rust::Abi::CUnwind).then_some(abi),
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
