use super::places::without_macro_holes;
use super::*;

const SUPPORT: &str = "__slate_blocks";

fn void_pointer(mutable: bool) -> rust::Type {
    rust::Type::Ptr {
        mutable,
        inner: Box::new(rust::Type::Custom("std::ffi::c_void".into())),
    }
}

fn byref_type(value: rust::Type) -> rust::Type {
    rust::Type::Generic {
        name: format!("{SUPPORT}::Byref"),
        args: vec![value],
    }
}

pub(super) fn byref_value(binding: Expr) -> Expr {
    Expr::Unary {
        op: rust::UnaryOp::Deref,
        expr: Box::new(support_call("value", vec![binding])),
    }
}

fn support_call(function: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(format!("{SUPPORT}::{function}").into())),
        args,
        binding: CallBinding::Generated,
    }
}

impl<'a, 'm> FunctionLowerer<'a, 'm> {
    pub(super) fn new(tables: &'a Tables<'m>, dependencies: &'a mut Dependencies) -> Self {
        FunctionLowerer {
            tables,
            dependencies,
            temps: 0,
            locals: Default::default(),
            owner: String::new(),
            hoisted: Vec::new(),
            dispatch_bindings: HashSet::new(),
            dispatch_vlas: HashSet::new(),
            aligned_locals: HashSet::new(),
            register_locals: HashSet::new(),
            counted_loops: HashSet::new(),
            non_index_names: HashSet::new(),
            macro_sites: Vec::new(),
            negated_macro_conditions: HashSet::new(),
            macro_template_check: false,
            old_value: None,
            declaring: std::cell::Cell::new(None),
            after_declaration: None,
            uses_alloca: false,
            in_static_initializer: false,
            target_features: Vec::new(),
            shared_pointers: HashMap::new(),
            captured: HashSet::new(),
            byref: HashSet::new(),
            byref_slots: HashSet::new(),
            pending: Vec::new(),
            dispatching: false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        self.tables.place_is_unsafe(place) || self.rooted_in_byref(place)
    }

    fn reserve(&mut self, storage: Stmt) {
        if self.dispatching {
            self.hoisted.push(storage);
        } else {
            self.pending.push(storage);
        }
    }

    pub(super) fn byref_pointer(&self, id: BindingId) -> Expr {
        let name = Expr::Var(self.binding_name(id).as_str().into());
        if self.byref_slots.contains(&id) {
            support_call(
                "slot",
                vec![Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(name),
                }],
            )
        } else {
            name
        }
    }

    pub(super) fn declare_byref_slot(&mut self, id: BindingId, ty: &ir::Type) -> Result<Stmt> {
        let value = self.lower_binding_type(ty, id)?;
        self.byref.insert(id);
        self.byref_slots.insert(id);
        Ok(Stmt::Let {
            name: self.binding_name(id),
            mutable: true,
            ty: Some(rust::Type::Generic {
                name: format!("{SUPPORT}::ByrefSlot"),
                args: vec![value],
            }),
            init: Some(Expr::Var(
                format!("{SUPPORT}::ByrefSlot::new()").as_str().into(),
            )),
        })
    }

    pub(super) fn init_byref_slot(&mut self, variable: &ir::Variable) -> Result<Stmt> {
        let header = support_call(
            "init",
            vec![Expr::AddrOf {
                mutable: true,
                expr: Box::new(Expr::Var(self.binding_name(variable.id).as_str().into())),
            }],
        );
        self.store_byref(header, variable)
    }

    fn store_byref(&mut self, byref: Expr, variable: &ir::Variable) -> Result<Stmt> {
        let initial = match &variable.initializer {
            Some(value) => self.lower_value(value)?,
            None => zeroed(),
        };
        Ok(Stmt::Expr(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(support_call("store", vec![byref, initial]))),
        }))))
    }

    fn rooted_in_byref(&self, place: &ir::Place) -> bool {
        if self.byref.is_empty() && self.captured.is_empty() {
            return false;
        }
        if let Some((array, _)) = self.tables.array_subscript(place) {
            return self.rooted_in_byref(array);
        }
        match &without_macro_holes(place).kind {
            PlaceKind::Binding(id) => self.byref.contains(id) || self.captured.contains(id),
            PlaceKind::Field { base, .. }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::ComplexPart { base, .. } => self.rooted_in_byref(base),
            _ => false,
        }
    }

    pub(super) fn lower_byref_let(&mut self, variable: &ir::Variable) -> Result<Stmt> {
        let ty = byref_type(self.lower_binding_type(&variable.ty, variable.id)?);
        self.byref.insert(variable.id);
        let name = self.binding_name(variable.id);
        let slot = format!("{name}_byref");
        self.reserve(Stmt::Let {
            name: slot.as_str().into(),
            mutable: true,
            ty: Some(rust::Type::Generic {
                name: "std::mem::MaybeUninit".into(),
                args: vec![ty.clone()],
            }),
            init: Some(Expr::Var("std::mem::MaybeUninit::uninit()".into())),
        });
        self.after_declaration = Some(Stmt::Let {
            name: format!("{name}_scope").as_str().into(),
            mutable: false,
            ty: None,
            init: Some(support_call("scope", vec![Expr::Var(name.as_str().into())])),
        });
        self.pending.push(Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: Some(rust::Type::Ptr {
                mutable: true,
                inner: Box::new(ty),
            }),
            init: Some(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(support_call(
                    "byref",
                    vec![Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var(slot.as_str().into())),
                    }],
                ))),
            }))),
        });
        self.store_byref(Expr::Var(name.as_str().into()), variable)
    }

    pub(super) fn lower_block_literal(&mut self, block: &ir::BlockLiteral) -> Result<Expr> {
        self.dependencies.blocks = true;
        let serial = self.dependencies.block_serial;
        self.dependencies.block_serial += 1;
        let function_name = format!("__slate_block_{serial}");
        let captures_name = format!("{function_name}_captures");
        let mut fields = Vec::new();
        let mut inits = Vec::new();
        let mut loads = Vec::new();
        let mut retained = Vec::new();
        for (index, capture) in block.captures.iter().enumerate() {
            let field = format!("c{index}");
            let kind = if capture.by_reference {
                Some("byref_field")
            } else if matches!(
                self.tables.resolve_type(&capture.ty),
                ir::Type::BlockPointer { .. }
            ) {
                Some("block_field")
            } else {
                None
            };
            if let Some(kind) = kind {
                retained.push(support_call(
                    kind,
                    vec![Expr::Macro {
                        name: "std::mem::offset_of".into(),
                        args: vec![
                            Expr::Var(captures_name.as_str().into()),
                            Expr::Var(field.as_str().into()),
                        ],
                    }],
                ));
            }
            let value_ty = self.lower_binding_type(&capture.ty, capture.id)?;
            let (ty, init) = if capture.by_reference {
                (
                    rust::Type::Ptr {
                        mutable: true,
                        inner: Box::new(byref_type(value_ty)),
                    },
                    self.byref_pointer(capture.id),
                )
            } else {
                (value_ty, self.lower_binding(capture.id))
            };
            loads.push(Stmt::Let {
                name: self.binding_name(capture.id),
                mutable: true,
                ty: Some(ty.clone()),
                init: Some(Expr::Field {
                    base: Box::new(Expr::Var("__captures".into())),
                    field: field.clone(),
                }),
            });
            fields.push(rust::RecordField {
                cfg: None,
                comments: Vec::new(),
                name: field.as_str().into(),
                ty,
            });
            inits.push((field, init));
        }
        let captures_ty = if fields.is_empty() {
            rust::Type::Unit
        } else {
            self.dependencies
                .block_items
                .push(Item::Record(rust::RecordDef {
                    comments: Vec::new(),
                    trailing_comments: Vec::new(),
                    vis: rust::Visibility::Private,
                    field_vis: rust::Visibility::Private,
                    is_union: false,
                    name: captures_name.clone(),
                    fields,
                    packed: None,
                    align: None,
                    opaque: false,
                }));
            rust::Type::Custom(captures_name.clone())
        };
        let item = {
            let mut child = FunctionLowerer::new(self.tables, &mut *self.dependencies);
            child.owner = self.owner.clone();
            child.locals = std::cell::RefCell::new(self.locals.borrow().clone());
            child.byref = self.byref.clone();
            child.target_features = self.target_features.clone();
            child.lower_block_function(&block.function, &function_name, &captures_ty, loads)?
        };
        self.dependencies.block_items.push(item);
        let invoke = Expr::Cast {
            expr: Box::new(Expr::Var(function_name.as_str().into())),
            ty: void_pointer(false),
        };
        if block.captures.is_empty() {
            let literal = format!("{function_name}_literal");
            self.dependencies.block_items.push(Item::Static {
                comments: Vec::new(),
                attrs: Vec::new(),
                vis: rust::Visibility::Private,
                mutable: false,
                name: literal.clone(),
                ty: rust::Type::Custom(format!("{SUPPORT}::Block<()>")),
                init: support_call("global", vec![invoke]),
            });
            return Ok(Expr::Cast {
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: false,
                        expr: Box::new(Expr::Var(literal.into())),
                    }),
                    ty: void_pointer(false),
                }),
                ty: void_pointer(true),
            });
        }
        let descriptor = format!("{function_name}_descriptor");
        self.dependencies.block_items.push(Item::Static {
            comments: Vec::new(),
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            mutable: false,
            name: descriptor.clone(),
            ty: rust::Type::Custom(format!("{SUPPORT}::Descriptor")),
            init: Expr::Call {
                func: Box::new(Expr::GenericPath {
                    path: rust::Path::new([SUPPORT, "descriptor"].map(rust::Ident::from)),
                    type_args: vec![captures_ty.clone()],
                }),
                args: vec![Expr::Ref {
                    mutable: false,
                    expr: Box::new(Expr::ArrayLit(retained)),
                }],
                binding: CallBinding::Generated,
            },
        });
        let slot = format!("{function_name}_slot");
        self.reserve(Stmt::Let {
            name: slot.as_str().into(),
            mutable: true,
            ty: Some(rust::Type::Custom(format!(
                "std::mem::MaybeUninit<{SUPPORT}::Block<{captures_name}>>"
            ))),
            init: Some(Expr::Var("std::mem::MaybeUninit::uninit()".into())),
        });
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(support_call(
                "stack",
                vec![
                    Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var(slot.as_str().into())),
                    },
                    invoke,
                    Expr::Ref {
                        mutable: false,
                        expr: Box::new(Expr::Var(descriptor.as_str().into())),
                    },
                    Expr::StructLit {
                        name: captures_name,
                        fields: inits,
                    },
                ],
            ))),
        })))
    }

    fn lower_block_function(
        &mut self,
        function: &ir::Function,
        name: &str,
        captures_ty: &rust::Type,
        loads: Vec<Stmt>,
    ) -> Result<Item> {
        let body = function
            .body
            .as_deref()
            .ok_or(Invariant::BlockWithoutBody)?;
        let fixed = types::fixed_parameters(&function.parameters);
        let mut params = vec![FnParam {
            comments: Vec::new(),
            name: "__block".into(),
            mutable: false,
            ty: void_pointer(true),
        }];
        for param in fixed {
            params.push(FnParam {
                comments: Vec::new(),
                name: self.binding_name(param.value.id),
                mutable: true,
                ty: self.lower_type(&param.ty)?,
            });
        }
        let variadic = matches!(
            function.parameters,
            ir::Parameters::Prototype { variadic: true, .. }
        );
        if variadic {
            params.push(FnParam {
                comments: Vec::new(),
                name: calls::VA_ARGS.into(),
                mutable: true,
                ty: rust::Type::Variadic,
            });
        }
        let ret = function
            .return_type
            .as_ref()
            .map(|ty| self.lower_type(ty))
            .transpose()?;
        self.non_index_names = self.non_index_names(&params, body);
        let mut statements = vec![Stmt::Let {
            name: "__captures".into(),
            mutable: false,
            ty: Some(captures_ty.clone()),
            init: Some(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::GenericPath {
                        path: rust::Path::new([SUPPORT, "captures"].map(rust::Ident::from)),
                        type_args: vec![captures_ty.clone()],
                    }),
                    args: vec![Expr::Var("__block".into())],
                    binding: CallBinding::Generated,
                })),
            }))),
        }];
        statements.extend(loads);
        let lowered = if control_flow::needs_dispatch(body) {
            self.lower_dispatch(body)?
        } else {
            self.lower_statement_list(body)?
        };
        statements.extend(std::mem::take(&mut self.hoisted));
        statements.extend(lowered);
        let ends_in_return = matches!(
            statements
                .iter()
                .rev()
                .find(|statement| !matches!(statement, Stmt::Comment(_))),
            Some(Stmt::Return(_))
        );
        if ret.is_some() && !ends_in_return {
            statements.push(Stmt::Return(Some(zeroed())));
        }
        Ok(Item::Fn(FnDef {
            comments: Vec::new(),
            unsafe_: variadic,
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            abi: Some(rust::Abi::CUnwind),
            name: name.into(),
            params,
            ret,
            body: statements,
        }))
    }

    pub(super) fn lower_block_call(
        &mut self,
        block: &ir::Value,
        arguments: &[ir::Value],
        signature: &ir::Type,
    ) -> Result<Expr> {
        let ir::Type::Function {
            parameters,
            return_type,
            variadic,
            ..
        } = self.tables.resolve_type(signature)
        else {
            return Err(unsupported_type(signature));
        };
        let temp = self.next_temp();
        let mut params = vec![void_pointer(true)];
        for parameter in parameters {
            params.push(self.lower_type(parameter)?);
        }
        if *variadic {
            params.push(rust::Type::Variadic);
        }
        let ret = match return_type {
            Some(ret) => self.lower_type(ret)?,
            None => rust::Type::Unit,
        };
        let mut args = vec![Expr::Var(temp.as_str().into())];
        for (index, argument) in arguments.iter().enumerate() {
            args.push(self.lower_call_argument(argument, index >= parameters.len())?);
        }
        let callee = self.lower_value(block)?;
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: vec![Stmt::Let {
                name: temp.clone(),
                mutable: false,
                ty: None,
                init: Some(callee),
            }],
            tail: Some(Box::new(Expr::Call {
                func: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Transmute {
                        from: void_pointer(false),
                        to: rust::Type::FnPtr {
                            abi: rust::Abi::CUnwind,
                            params,
                            ret: Box::new(ret),
                        },
                        expr: Box::new(support_call(
                            "invoke",
                            vec![Expr::Var(temp.as_str().into())],
                        )),
                    }),
                    method: "unwrap".into(),
                    args: Vec::new(),
                }),
                args,
                binding: CallBinding::Generated,
            })),
        })))
    }
}
