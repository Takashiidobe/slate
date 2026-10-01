use super::*;

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
        let ret = self.lower_return(function)?;
        Ok(rust::ExternDecl::Fn(rust::ExternFnDecl {
            attrs: Vec::new(),
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
        let mut statements = self.lower_statement_list(body)?;
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
        Ok(Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            unsafe_: *variadic,
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
