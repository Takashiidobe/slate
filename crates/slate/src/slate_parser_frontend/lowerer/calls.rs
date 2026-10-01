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
    ) -> Result<Expr> {
        let names = &self.tables.names;
        let func = match callee {
            ir::Callee::Direct(id) => Expr::Var(
                names
                    .get(id)
                    .ok_or_else(|| super::Error::Unsupported(format!("unknown callee %{}", id.0)))?
                    .rust
                    .as_str()
                    .into(),
            ),
            ir::Callee::Indirect(pointer) => Expr::MethodCall {
                recv: Box::new(self.lower_value(pointer)?),
                method: "unwrap".into(),
                args: Vec::new(),
            },
        };
        let call = Expr::Call {
            func: Box::new(func),
            args: arguments
                .iter()
                .map(|argument| self.lower_value(argument))
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
