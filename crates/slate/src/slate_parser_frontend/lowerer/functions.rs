use super::*;

pub(super) fn lower_extern(function: &ir::Function, cx: &Context) -> Result<rust::ExternDecl> {
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return Err(super::Error::Unsupported(format!(
            "unprototyped declaration {}",
            function.name
        )));
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
                    name: binding_name(parameter.value.id, &cx.bindings),
                    mutable: false,
                    ty: lower_type(cx, &parameter.ty)?,
                })
            })
            .collect::<Result<Vec<_>>>()?,
        variadic: *variadic,
        ret: function
            .return_type
            .as_ref()
            .map(|ty| lower_type(cx, ty))
            .transpose()?,
        safe: false,
    }))
}

pub(super) fn lower_function(
    function: &ir::Function,
    body: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
) -> Result<Item> {
    cx.temps.set(0);
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return Err(super::Error::Unsupported(format!(
            "unprototyped function {}",
            function.name
        )));
    };
    let mut params = fixed
        .iter()
        .map(|param| {
            Ok(FnParam {
                name: binding_name(param.value.id, &cx.bindings),
                mutable: true,
                ty: lower_type(cx, &param.ty)?,
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
    let mut statements = body
        .iter()
        .map(|statement| lower_statement(statement, cx))
        .collect::<Result<Vec<_>>>()?;
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
        name: cx.names[&function.id].rust.clone(),
        params,
        ret: function
            .return_type
            .as_ref()
            .map(|ty| lower_type(cx, ty))
            .transpose()?,
        body: statements,
    }))
}
