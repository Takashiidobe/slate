use super::*;

pub(super) fn is_long_double(cx: &Context, ty: &ir::Type) -> bool {
    matches!(
        resolve_type(cx, ty),
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80))
    )
}

pub(super) fn holds_long_double(cx: &Context, ty: &ir::Type) -> bool {
    is_long_double(cx, ty)
        || match resolve_type(cx, ty) {
            ir::Type::Array { element, .. } => holds_long_double(cx, element),
            ty => record_fields(cx, ty)
                .is_some_and(|fields| fields.iter().any(|field| holds_long_double(cx, &field.ty))),
        }
}

pub(super) fn passes_long_double(function: &ir::Function, cx: &Context) -> bool {
    let ir::Parameters::Prototype { fixed, .. } = &function.parameters else {
        return false;
    };
    fixed
        .iter()
        .map(|parameter| &parameter.ty)
        .chain(&function.return_type)
        .any(|ty| holds_long_double(cx, ty))
}

pub(super) fn long_double_literal(bits: u128) -> Expr {
    Expr::TupleStructLit {
        name: long_double::LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayLit(
            bits.to_le_bytes()[..10]
                .iter()
                .map(|byte| Expr::Value(rust::RustValue::I64(i64::from(*byte))))
                .collect(),
        )],
    }
}

pub(super) fn long_double_shim(name: &str, operand: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args: vec![operand],
        binding: CallBinding::Generated,
    }
}

pub(super) fn lower_long_double_conversion(
    cx: &Context,
    operand: &ir::Value,
    ty: &ir::Type,
) -> Result<Expr> {
    let lowered = lower_value(operand, cx)?;
    let (from, to) = (is_long_double(cx, &operand.ty), is_long_double(cx, ty));
    let shim = match (from, to) {
        (true, true) => return Ok(lowered),
        (false, true) => long_double::f80_cast_from_name(&lower_type(cx, &operand.ty)?),
        _ => long_double::f80_cast_to_name(&lower_type(cx, ty)?),
    };
    let shim = shim.ok_or_else(|| {
        super::Error::Unsupported(format!("long double conversion {} -> {ty}", operand.ty))
    })?;
    Ok(long_double_shim(shim, lowered))
}

pub(super) fn lower_long_double_bridge(
    cx: &Context,
    function: &FunctionName,
    arguments: &[ir::Value],
    ret: &ir::Type,
) -> Result<Expr> {
    let callee = function.rust.as_str();
    if (function.is_variadic && crate::function_identity::Known::from_symbol(callee).is_none())
        || callee.contains("__")
    {
        return Err(super::Error::Unsupported(format!(
            "long double call to {callee}"
        )));
    }
    let params = arguments
        .iter()
        .map(|argument| lower_type(cx, &argument.ty))
        .collect::<Result<Vec<_>>>()?;
    let ret = lower_type(cx, ret)?;
    let tags = long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params))?;
    let name = format!("__slate_{callee}__r{}", tags.join("_"));
    let args = arguments
        .iter()
        .map(|argument| lower_value(argument, cx))
        .collect::<Result<Vec<_>>>()?;
    Ok(call_long_double_bridge(cx, name, params, ret, args))
}

pub(super) fn lower_long_double_variadic_trampoline(
    cx: &Context,
    function: &FunctionName,
    arguments: &[ir::Value],
    fixed: usize,
    ret: &ir::Type,
) -> Result<Expr> {
    let callee = function.rust.as_str();
    let params = arguments
        .iter()
        .map(|argument| lower_type(cx, &argument.ty))
        .collect::<Result<Vec<_>>>()?;
    let ret = lower_type(cx, ret)?;
    let fixed_tags =
        long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params[..fixed]))?;
    let variadic_tags = long_double_bridge_tags(callee, &params[fixed..])?;
    let name = format!(
        "__slate_vcall__r{}__{}",
        fixed_tags.join("_"),
        variadic_tags.join("_")
    );
    let code_pointer = rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Unit),
    };
    let args = std::iter::once(Ok(Expr::Cast {
        expr: Box::new(Expr::Var(callee.into())),
        ty: code_pointer.clone(),
    }))
    .chain(arguments.iter().map(|argument| lower_value(argument, cx)))
    .collect::<Result<Vec<_>>>()?;
    Ok(call_long_double_bridge(
        cx,
        name,
        std::iter::once(code_pointer).chain(params).collect(),
        ret,
        args,
    ))
}

pub(super) fn long_double_bridge_tags<'a>(
    callee: &str,
    types: impl IntoIterator<Item = &'a rust::Type>,
) -> Result<Vec<String>> {
    types
        .into_iter()
        .map(|ty| match long_double::long_double_shim_type_tag(ty) {
            tag if tag == "x" => Err(super::Error::Unsupported(format!(
                "long double call to {callee} passing {}",
                crate::backend::codegen::type_to_string(ty)
            ))),
            tag => Ok(tag),
        })
        .collect()
}

pub(super) fn call_long_double_bridge(
    cx: &Context,
    name: String,
    params: Vec<rust::Type>,
    ret: rust::Type,
    args: Vec<Expr>,
) -> Expr {
    cx.bridges
        .borrow_mut()
        .entry(name.clone())
        .or_insert_with(|| rust::ExternFnDecl {
            attrs: Vec::new(),
            name: name.clone(),
            identity: FunctionIdentity::Unknown,
            declared_type: None,
            trusted_headers: Default::default(),
            params: params
                .into_iter()
                .enumerate()
                .map(|(index, ty)| FnParam {
                    name: format!("_{index}"),
                    mutable: false,
                    ty,
                })
                .collect(),
            variadic: false,
            ret: (!matches!(ret, rust::Type::Unit)).then_some(ret),
            safe: false,
        });
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var(name.into())),
            args,
            binding: CallBinding::Generated,
        })),
    }))
}
