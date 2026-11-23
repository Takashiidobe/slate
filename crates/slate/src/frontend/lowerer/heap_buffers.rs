use super::*;
use crate::function_identity::Known;
use optional::{method, some_pattern};
use slate_analysis::{GuardedHeapStatement, HeapAction, HeapElement, HeapIndex};

fn peel_pointer(mut value: &ir::Value) -> &ir::Value {
    while let ValueKind::Copy { operand, .. }
    | ValueKind::Convert {
        kind: ir::ConversionKind::PointerCast,
        operand,
        ..
    } = &value.node.value
    {
        value = operand;
    }
    value
}

fn usize_of(expr: Expr) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty: rust::Type::Prim(Prim::Usize),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_heap_statement(
        &mut self,
        statement: &Span<ir::Statement>,
    ) -> Result<Option<Stmt>> {
        let Some(fact) = self.tables.facts.heap_statement(statement.id) else {
            return Ok(None);
        };
        if !self.owned(fact.owner) {
            return Ok(None);
        }
        Ok(Some(match (fact.action, &statement.value) {
            (HeapAction::TryAllocate, ir::Statement::Let(variable))
                if let Some(initializer) = &variable.initializer
                    && let Some((Known::Malloc, [size])) = self.allocator_call(initializer)
                    && let ir::Type::Pointer { pointee, .. } =
                        self.tables.resolve_type(&variable.ty) =>
            {
                self.dependencies.vec_ext = true;
                Stmt::Let {
                    origin: rust::LocalOrigin::Source,
                    name: self.binding_name(variable.id),
                    mutable: false,
                    ty: Some(rust::Type::Generic {
                        name: "Option".into(),
                        args: vec![rust::Type::Generic {
                            name: "Vec".into(),
                            args: vec![self.lower_type(pointee)?],
                        }],
                    }),
                    init: Some(generated_call(
                        "Vec::try_filled",
                        vec![
                            usize_of(self.lower_value(size)?),
                            Expr::Value(rust::RustValue::I64(0)),
                        ],
                    )),
                }
            }
            (HeapAction::AllocationFailed, ir::Statement::If { then_body, .. }) => {
                let owner = self.binding_name(fact.owner);
                let value = Expr::var(owner.as_str().into());
                let present = some_pattern(&owner);
                Stmt::Let {
                    origin: rust::LocalOrigin::Source,
                    name: owner,
                    mutable: true,
                    ty: None,
                    init: Some(Expr::Match {
                        expr: Box::new(value.clone()),
                        arms: vec![
                            rust::ExprMatchArm {
                                pattern: present,
                                value,
                            },
                            rust::ExprMatchArm {
                                pattern: rust::Pattern::Wildcard,
                                value: Expr::Block(Box::new(rust::Block {
                                    stmts: self.lower_statement_list(then_body)?,
                                    tail: None,
                                })),
                            },
                        ],
                    }),
                }
            }
            (HeapAction::TryResize, ir::Statement::Let(result))
                if let Some(initializer) = &result.initializer
                    && let Some((Known::Realloc, [_, size])) = self.allocator_call(initializer) =>
            {
                self.dependencies.vec_ext = true;
                let resized = method(
                    Expr::var(self.binding_name(fact.owner).into()),
                    "try_resize",
                    vec![
                        usize_of(self.lower_value(size)?),
                        Expr::Value(rust::RustValue::I64(0)),
                    ],
                );
                Stmt::Let {
                    origin: rust::LocalOrigin::Synthetic,
                    name: self.binding_name(result.id),
                    mutable: false,
                    ty: Some(rust::Type::Prim(Prim::Bool)),
                    init: Some(method(resized, "is_none", vec![])),
                }
            }
            (HeapAction::ResizeFailed(result), ir::Statement::If { then_body, .. }) => Stmt::If {
                cond: Expr::var(self.binding_name(result).into()),
                then_body: self.lower_statement_list(then_body)?,
                else_body: Vec::new(),
            },
            (HeapAction::RemoveWrite, ir::Statement::Write { .. }) => {
                Stmt::Block(rust::Block::default())
            }
            (
                HeapAction::ClearedCheck(equal),
                ir::Statement::If {
                    then_body,
                    else_body,
                    ..
                },
            ) => self.lower_known_if(equal, then_body, else_body.as_deref())?,
            _ => return Err(Invariant::UnprovenBorrow(format!("{fact:?}")).into()),
        }))
    }

    pub(super) fn lower_guarded_heap(
        &mut self,
        statement: &ir::Statement,
        action: GuardedHeapStatement,
    ) -> Result<Stmt> {
        Ok(match (action, statement) {
            (GuardedHeapStatement::Declare, ir::Statement::Let(variable))
                if let ir::Type::Pointer { pointee, .. } = &variable.ty =>
            {
                let mut element = self.lower_type(pointee)?;
                if self.tables.facts.heap_has_uninitialized_slots(variable.id) {
                    element = rust::Type::Generic {
                        name: "core::mem::MaybeUninit".into(),
                        args: vec![element],
                    };
                }
                Stmt::Let {
                    origin: rust::LocalOrigin::Source,
                    name: self.binding_name(variable.id),
                    mutable: true,
                    ty: Some(rust::Type::Generic {
                        name: "Option".into(),
                        args: vec![rust::Type::Generic {
                            name: "Vec".into(),
                            args: vec![element],
                        }],
                    }),
                    init: Some(Expr::Value(rust::RustValue::None)),
                }
            }
            (
                GuardedHeapStatement::Allocate {
                    binding,
                    count,
                    zeroed,
                },
                ir::Statement::Write { .. },
            ) => {
                self.dependencies.vec_ext = true;
                let uninit = self.tables.facts.heap_has_uninitialized_slots(binding);
                let mut args = vec![heap_count(count)];
                let function = if zeroed {
                    args.push(heap_zero(uninit));
                    "Vec::try_filled"
                } else if uninit {
                    "Vec::try_uninit"
                } else {
                    "Vec::try_alloc"
                };
                Stmt::Assign {
                    target: Expr::var(self.binding_name(binding).into()),
                    value: generated_call(function, args),
                }
            }
            (GuardedHeapStatement::Resize { binding, count }, ir::Statement::Let(result)) => {
                self.dependencies.vec_ext = true;
                let uninit = self.tables.facts.heap_has_uninitialized_slots(binding);
                let mut args = vec![heap_count(count)];
                let name = if uninit {
                    args.push(Expr::var("core::mem::MaybeUninit::uninit".into()));
                    "try_resize_with"
                } else {
                    "try_reserve_len"
                };
                Stmt::Let {
                    origin: rust::LocalOrigin::Synthetic,
                    name: self.binding_name(result.id),
                    mutable: false,
                    ty: Some(rust::Type::Prim(Prim::Bool)),
                    init: Some(method(
                        method(Expr::var(self.binding_name(binding).into()), name, args),
                        "is_none",
                        vec![],
                    )),
                }
            }
            (GuardedHeapStatement::ResizeFailed(result), ir::Statement::If { then_body, .. }) => {
                Stmt::If {
                    cond: Expr::var(self.binding_name(result).into()),
                    then_body: self.lower_statement_list(then_body)?,
                    else_body: Vec::new(),
                }
            }
            (
                GuardedHeapStatement::ZeroFill {
                    binding,
                    count,
                    overwrite,
                },
                ir::Statement::Expression(_),
            ) => {
                let uninit = self.tables.facts.heap_has_uninitialized_slots(binding);
                let buffer = Expr::var(self.binding_name(binding).into());
                let resize = || {
                    Stmt::Expr(method(
                        buffer.clone(),
                        "resize",
                        vec![heap_count(count), heap_zero(false)],
                    ))
                };
                let fill = || Stmt::Expr(method(buffer.clone(), "fill", vec![heap_zero(uninit)]));
                if uninit {
                    fill()
                } else if overwrite {
                    Stmt::Block(rust::Block {
                        stmts: vec![resize(), fill()],
                        tail: None,
                    })
                } else {
                    resize()
                }
            }
            (
                GuardedHeapStatement::Present {
                    binding,
                    elide_release,
                },
                ir::Statement::If {
                    then_body,
                    else_body,
                    ..
                },
            ) => {
                let name = self.binding_name(binding);
                let opt = Expr::var(name.as_str().into());
                let guarded = Stmt::If {
                    cond: Expr::Let {
                        pattern: some_pattern(name.as_str()),
                        expr: Box::new(method(opt.clone(), "as_mut", vec![])),
                    },
                    then_body: self.lower_statement_list(then_body)?,
                    else_body: self
                        .lower_statement_list(else_body.as_deref().unwrap_or_default())?,
                };
                if elide_release {
                    return Ok(guarded);
                }
                Stmt::Block(rust::Block {
                    stmts: vec![guarded, Stmt::Expr(generated_call("drop", vec![opt]))],
                    tail: None,
                })
            }
            (GuardedHeapStatement::Remove, _) => Stmt::Block(rust::Block::default()),
            _ => return Err(Invariant::UnprovenBorrow(format!("{action:?}")).into()),
        })
    }

    pub(super) fn heap_element(&self, place: &ir::Place) -> Option<HeapElement> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return None;
        };
        self.tables.facts.heap_element(pointer.node.id)
    }

    pub(super) fn heap_index(&self, element: HeapElement) -> Expr {
        let index = match element.index {
            HeapIndex::Constant(index) => heap_count(index),
            HeapIndex::Binding(binding) => usize_of(Expr::var(self.binding_name(binding).into())),
        };
        Expr::Index {
            base: Box::new(Expr::var(self.binding_name(element.binding).into())),
            index: Box::new(index),
        }
    }

    pub(super) fn lower_heap_store(&self, place: &ir::Place, value: &Expr) -> Option<Stmt> {
        let element = self.heap_element(place)?;
        if self
            .tables
            .facts
            .heap_has_uninitialized_slots(element.binding)
        {
            let target = self.heap_index(element);
            if value.reads_var(&self.binding_name(element.binding)) {
                return Some(Stmt::Assign {
                    target,
                    value: generated_call("core::mem::MaybeUninit::new", vec![value.clone()]),
                });
            }
            return Some(Stmt::Expr(method(target, "write", vec![value.clone()])));
        }
        element.append.then(|| {
            Stmt::Expr(method(
                Expr::var(self.binding_name(element.binding).into()),
                "push",
                vec![value.clone()],
            ))
        })
    }

    pub(super) fn lower_heap_load(&self, value: &ir::Value) -> Option<Expr> {
        let ValueKind::Read {
            place,
            ordering: None,
        } = &value.node.value
        else {
            return None;
        };
        let element = self.heap_element(place)?;
        self.tables
            .facts
            .heap_has_uninitialized_slots(element.binding)
            .then(|| unsafe_op(method(self.heap_index(element), "assume_init_read", vec![])))
    }

    pub(super) fn owned(&self, binding: BindingId) -> bool {
        !self.unborrowed.contains(&binding)
            && !self.switch_lets.contains(&binding)
            && self.tables.owner_of(binding).is_some()
    }

    pub(super) fn owned_read(&self, value: &ir::Value) -> Option<BindingId> {
        let mut value = value;
        while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
            &value.node.value
        {
            value = operand;
        }
        let ValueKind::Read {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } = &value.node.value
        else {
            return None;
        };
        self.owned(*id).then_some(*id)
    }

    fn allocator_call<'v>(&self, value: &'v ir::Value) -> Option<(Known, &'v [ir::Value])> {
        let ValueKind::Call {
            callee, arguments, ..
        } = &peel_pointer(value).node.value
        else {
            return None;
        };
        Some((self.known_callee(callee)?, arguments.as_slice()))
    }

    pub(super) fn lower_owned_let(&mut self, variable: &ir::Variable) -> Result<Option<Stmt>> {
        if !self.owned(variable.id) {
            return Ok(None);
        }
        let Some(initializer) = &variable.initializer else {
            return Ok(None);
        };
        let size = match self.allocator_call(initializer) {
            Some((Known::Malloc, [size])) => usize_of(self.lower_value(size)?),
            Some((Known::Calloc, [count, size])) => Expr::Binary {
                op: BinOp::Mul,
                lhs: Box::new(usize_of(self.lower_value(count)?)),
                rhs: Box::new(usize_of(self.lower_value(size)?)),
            },
            _ => return Err(Invariant::UnprovenBorrow(initializer.to_string()).into()),
        };
        let ir::Type::Pointer { pointee, .. } = self.tables.resolve_type(&variable.ty) else {
            return Err(Invariant::UnprovenBorrow(variable.ty.to_string()).into());
        };
        let element = self.lower_type(pointee)?;
        let mutable = self
            .borrow_of(variable.id)
            .is_some_and(|borrow| borrow.mutable);
        Ok(Some(Stmt::Let {
            origin: rust::LocalOrigin::Source,
            name: self.binding_name(variable.id),
            mutable,
            ty: Some(rust::Type::Generic {
                name: "Vec".into(),
                args: vec![element],
            }),
            init: Some(Expr::VecRepeat {
                elem: Box::new(Expr::Value(rust::RustValue::I64(0))),
                len: Box::new(size),
            }),
        }))
    }

    pub(super) fn lower_reallocation(
        &mut self,
        binding: BindingId,
        value: &ir::Value,
    ) -> Result<Option<Stmt>> {
        if !self.owned(binding) {
            return Ok(None);
        }
        let Some((Known::Realloc, [pointer, size])) = self.allocator_call(value) else {
            return Err(Invariant::UnprovenBorrow(value.to_string()).into());
        };
        if self.owned_read(pointer) != Some(binding) {
            return Err(Invariant::UnprovenBorrow(value.to_string()).into());
        }
        Ok(Some(Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::var(self.binding_name(binding).as_str().into())),
            method: "resize".into(),
            args: vec![
                usize_of(self.lower_value(size)?),
                Expr::Value(rust::RustValue::I64(0)),
            ],
            unsafe_: rust::Unsafe(false),
        })))
    }

    pub(super) fn lower_release(&self, arguments: &[ir::Value]) -> Option<Expr> {
        let [pointer] = arguments else {
            return None;
        };
        let binding = self.owned_read(pointer)?;
        Some(Expr::Call {
            func: Box::new(Expr::var("drop".into())),
            args: vec![Expr::var(self.binding_name(binding).as_str().into())],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn elided_release(&self, value: &ir::Value) -> bool {
        let ValueKind::Call { arguments, .. } = &value.node.value else {
            return false;
        };
        self.tables.elided_release(value.node.id) && self.lower_release(arguments).is_some()
    }
}

fn heap_count(count: u64) -> Expr {
    Expr::Value(rust::RustValue::U128(count.into()))
}

fn heap_zero(uninit: bool) -> Expr {
    let zero = Expr::Value(rust::RustValue::I64(0));
    if uninit {
        generated_call("core::mem::MaybeUninit::new", vec![zero])
    } else {
        zero
    }
}

fn generated_call(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::var(name.into())),
        args,
        binding: CallBinding::Generated,
        unsafe_: rust::Unsafe(false),
    }
}
