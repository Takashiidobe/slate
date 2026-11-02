use super::*;

pub(super) fn collect_nested_functions<'m>(
    statements: &'m [Span<ir::Statement>],
    found: &mut Vec<&'m ir::NestedFunction>,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::NestedFunction(nested) => {
                found.push(nested);
                if let Some(body) = &nested.function.body {
                    collect_nested_functions(body, found);
                }
            }
            ir::Statement::Block(body)
            | ir::Statement::MacroExpansion { body, .. }
            | ir::Statement::While { body, .. }
            | ir::Statement::DoWhile { body, .. }
            | ir::Statement::Switch { body, .. }
            | ir::Statement::Label { body, .. }
            | ir::Statement::Case { body, .. }
            | ir::Statement::Default { body, .. } => collect_nested_functions(body, found),
            ir::Statement::OmpParallel(region) => collect_nested_functions(&region.body, found),
            ir::Statement::OmpFor(omp) => collect_nested_functions(&omp.body, found),
            ir::Statement::SehTry { body, handler, .. } => {
                collect_nested_functions(body, found);
                match handler {
                    ir::SehHandler::Except { body, .. } | ir::SehHandler::Finally(body) => {
                        collect_nested_functions(body, found)
                    }
                }
            }
            ir::Statement::For { init, body, .. } => {
                collect_nested_functions(init, found);
                collect_nested_functions(body, found);
            }
            ir::Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_nested_functions(then_body, found);
                if let Some(else_body) = else_body {
                    collect_nested_functions(else_body, found);
                }
            }
            _ => {}
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_nested_function(
        &mut self,
        statement: &Span<ir::Statement>,
        nested: &ir::NestedFunction,
    ) -> Result<()> {
        let body = nested
            .function
            .body
            .as_deref()
            .ok_or(Invariant::NestedFunctionWithoutBody)?;
        let function = Span::new(
            nested.function.clone(),
            statement.spelling,
            statement.expansion,
        );
        let name = self
            .tables
            .names
            .get(&nested.function.id)
            .ok_or(Invariant::UnknownCallee(nested.function.id))?
            .rust
            .clone();
        let item = {
            let mut child = FunctionLowerer::new(self.tables, &mut *self.dependencies);
            child.owner = owner_name(&name);
            child.locals = std::cell::RefCell::new(self.locals.borrow().clone());
            let mut captures = Vec::new();
            for capture in &nested.captures {
                let pointee = if child.tables.variably_modified(&capture.ty) {
                    child.lower_type(&capture.ty)?
                } else {
                    child.lower_binding_type(&capture.ty, capture.id)?
                };
                let pointer = format!("__slate_capture_{}", child.local_index(capture.id));
                captures.push(FnParam {
                    comments: Vec::new(),
                    name: pointer.as_str().into(),
                    mutable: false,
                    ty: rust::Type::Ptr {
                        mutable: true,
                        inner: Box::new(pointee),
                    },
                });
                child.shared_pointers.insert(capture.id, pointer);
                child.captured.insert(capture.id);
            }
            let Item::Fn(mut definition) = child.lower_function(&function, body)? else {
                return Err(Invariant::NestedFunctionWithoutBody.into());
            };
            definition.params.splice(0..0, captures);
            definition
        };
        self.dependencies.nested_functions.push(Item::Fn(item));
        Ok(())
    }

    pub(super) fn nested_capture_arguments(&mut self, id: BindingId) -> Result<Vec<Expr>> {
        let Some(nested) = self.tables.nested.get(&id) else {
            return Ok(Vec::new());
        };
        nested
            .captures
            .iter()
            .map(|capture| match self.shared_pointers.get(&capture.id) {
                Some(pointer) => Ok(Expr::Var(pointer.as_str().into())),
                None => self.lower_address(
                    &ir::Place {
                        ty: capture.ty.clone(),
                        kind: PlaceKind::Binding(capture.id),
                        access: Default::default(),
                    },
                    true,
                ),
            })
            .collect()
    }
}
