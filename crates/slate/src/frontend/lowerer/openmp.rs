use super::*;
use slate_parser::target_info::TargetEnvironment;

impl FunctionLowerer<'_, '_> {
    fn shared_pointer(&self, id: BindingId) -> String {
        format!("__slate_omp_{}", self.local_index(id))
    }

    fn firstprivate_pointer(&self, id: BindingId) -> String {
        format!("__slate_omp_first_{}", self.local_index(id))
    }

    pub(super) fn lower_omp_parallel(
        &mut self,
        statement: &Span<ir::Statement>,
        region: &ir::OmpParallel,
    ) -> Result<Stmt> {
        if self.tables.target.environment == TargetEnvironment::Msvc {
            return Err(Construct::Statement {
                kind: variant_name(&statement.value),
                ir: statement.display().to_string(),
            }
            .into());
        }
        self.dependencies.libgomp = true;
        let mut num_threads = match &region.num_threads {
            Some(value) => self.lower_value(value)?,
            None => Expr::Value(rust::RustValue::TypedUInt(0, Prim::U32)),
        };
        if let Some(condition) = &region.condition {
            num_threads = Expr::If {
                cond: Box::new(self.lower_condition(condition)?),
                then_expr: Box::new(num_threads),
                else_expr: Box::new(Expr::Value(rust::RustValue::TypedUInt(1, Prim::U32))),
            };
        }
        let mut outer = Vec::new();
        let mut shared = Vec::new();
        for variable in &region.shared {
            let place = ir::Place::new(
                variable.ty.clone(),
                PlaceKind::Binding(variable.binding),
                Default::default(),
            );
            let pointer = self.shared_pointer(variable.binding);
            outer.push(Stmt::Let {
                origin: rust::LocalOrigin::Synthetic,
                name: pointer.clone(),
                mutable: false,
                ty: None,
                init: Some(self.lower_address(&place, true)?),
            });
            shared.push((variable.binding, pointer));
        }
        let mut body = Vec::new();
        for private in &region.private {
            let copy = Span::new(
                ir::Statement::Let(private.copy.clone()),
                statement.spelling,
                statement.expansion,
            );
            body.extend(self.lower_statement(&copy)?);
            if private.firstprivate {
                let place = ir::Place::new(
                    private.copy.ty.clone(),
                    PlaceKind::Binding(private.original),
                    Default::default(),
                );
                let pointer = self.firstprivate_pointer(private.copy.id);
                outer.push(Stmt::Let {
                    origin: rust::LocalOrigin::Synthetic,
                    name: pointer.clone(),
                    mutable: false,
                    ty: None,
                    init: Some(self.lower_address(&place, false)?),
                });
                body.push(Stmt::Assign {
                    target: self.lower_binding(private.copy.id),
                    value: Expr::Call {
                        func: Box::new(Expr::var("core::ptr::read".into())),
                        args: vec![Expr::var(pointer.into())],
                        binding: CallBinding::Generated,
                        unsafe_: rust::Unsafe(true),
                    },
                });
            }
        }
        let enclosing = self.shared_pointers.clone();
        self.shared_pointers.extend(shared);
        let params = std::mem::take(&mut self.dispatch_params);
        let lowered = self.lower_statement_list(&region.body);
        self.dispatch_params = params;
        self.shared_pointers = enclosing;
        body.extend(lowered?);
        outer.push(Stmt::Expr(Expr::Call {
            func: Box::new(Expr::var("__slate_libgomp::parallel".into())),
            args: vec![
                num_threads,
                Expr::Closure {
                    params: Vec::new(),
                    body: Box::new(Expr::Unsafe(Box::new(rust::Block {
                        stmts: body,
                        tail: None,
                    }))),
                },
            ],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        }));
        Ok(Stmt::Scope { body: outer })
    }
}
