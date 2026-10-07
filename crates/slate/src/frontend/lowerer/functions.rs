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

pub(super) fn codegen_builtin(metadata: &[(String, String)]) -> bool {
    let entry = |name: &str| {
        metadata
            .iter()
            .find_map(|(key, value)| (key == name).then_some(value.as_str()))
    };
    entry("c_builtin_kind").is_some_and(|kind| kind != "library")
        || entry("c_builtin_header").is_some_and(|header| header.ends_with("intrin.h"))
}

pub(super) fn main_wrapper(arity: usize) -> Item {
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
    Item::Fn(FnDef {
        comments: Vec::new(),
        attrs: vec![Attr::NoMangle],
        vis: rust::Visibility::Private,
        unsafe_: true,
        abi: Some(rust::Abi::CUnwind),
        name: "main".into(),
        params,
        ret: Some(rust::Type::Prim(rust::Prim::I32)),
        body: vec![Stmt::Return(Some(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(Expr::Var("__slate_main".into())),
                args,
                binding: CallBinding::Generated,
            })),
        }))))],
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
                    comments: self.claim_comments(param.id),
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
                comments: Vec::new(),
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
        statements.extend(self.remaining_statement_comments(body));
        statements.splice(0..0, std::mem::take(&mut self.hoisted));
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
        let target_feature = self.tables.target_feature_attr(function)?;
        Ok(Item::Fn(FnDef {
            comments: self.claim_comments(function.id),
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
