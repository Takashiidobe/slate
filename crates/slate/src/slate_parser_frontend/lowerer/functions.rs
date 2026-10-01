use super::*;

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_extern(&mut self, function: &ir::Function) -> Result<rust::ExternDecl> {
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "unprototyped declaration".into(),
            }
            .into());
        };
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
                        ty: self.lower_type(&parameter.ty)?,
                    })
                })
                .collect::<Result<Vec<_>>>()?,
            variadic: *variadic,
            ret: function
                .return_type
                .as_ref()
                .map(|ty| self.lower_type(ty))
                .transpose()?,
            safe: false,
        }))
    }

    pub(super) fn lower_function(
        &mut self,
        function: &ir::Function,
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Item> {
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return Err(Construct::Function {
                name: function.name.clone(),
                detail: "unprototyped definition".into(),
            }
            .into());
        };
        let mut params = fixed
            .iter()
            .map(|param| {
                Ok(FnParam {
                    name: binding_name(param.value.id, &self.tables.bindings),
                    mutable: true,
                    ty: self.lower_type(&param.ty)?,
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
            name: self.tables.names[&function.id].rust.clone(),
            params,
            ret: function
                .return_type
                .as_ref()
                .map(|ty| self.lower_type(ty))
                .transpose()?,
            body: statements,
        }))
    }
}
