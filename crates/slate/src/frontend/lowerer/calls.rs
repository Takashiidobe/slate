use super::*;

pub(super) const VA_ARGS: &str = "__va_args";

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
                    && builtin_library_name(&name.rust).is_none()
                {
                    return Err(Construct::Function {
                        name: name.rust.clone(),
                        detail: "builtin without a lowering".into(),
                    }
                    .into());
                }
                Expr::Var(name.rust.as_str().into())
            }
            ir::Callee::Indirect(pointer) => Expr::MethodCall {
                recv: Box::new(self.lower_value(pointer)?),
                method: "unwrap".into(),
                args: Vec::new(),
            },
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
                    Ok(
                        if variadic_start.is_some_and(|start| index >= start)
                            && matches!(ty, rust::Type::FnPtr { .. })
                        {
                            convert_function_pointer(ty, byte_pointer_type(), Box::new(lowered))
                        } else {
                            lowered
                        },
                    )
                })
                .collect::<Result<Vec<_>>>()?,
            binding: CallBinding::unknown(),
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
}
