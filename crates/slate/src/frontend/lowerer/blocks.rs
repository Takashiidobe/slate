use super::places::without_macro_holes;
use super::*;

const SUPPORT: &str = "__slate_blocks";

fn void_pointer(mutable: bool) -> rust::Type {
    rust::Type::Ptr {
        mutable,
        inner: Box::new(rust::Type::Custom("std::ffi::c_void".into())),
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
            macro_template_check: false,
            old_value: None,
            declaring: std::cell::Cell::new(None),
            after_declaration: None,
            uses_alloca: false,
            in_static_initializer: false,
            target_features: Vec::new(),
            omp_shared: HashMap::new(),
            byref: HashSet::new(),
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn place_is_unsafe(&self, place: &ir::Place) -> bool {
        self.tables.place_is_unsafe(place) || self.rooted_in_byref(place)
    }

    fn rooted_in_byref(&self, place: &ir::Place) -> bool {
        if self.byref.is_empty() {
            return false;
        }
        if let Some((array, _)) = self.tables.array_subscript(place) {
            return self.rooted_in_byref(array);
        }
        match &without_macro_holes(place).kind {
            PlaceKind::Binding(id) => self.byref.contains(id),
            PlaceKind::Field { base, .. }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::ComplexPart { base, .. } => self.rooted_in_byref(base),
            _ => false,
        }
    }

    pub(super) fn lower_byref_let(&mut self, variable: &ir::Variable) -> Result<Stmt> {
        let ty = self.lower_binding_type(&variable.ty, variable.id)?;
        let init = match &variable.initializer {
            Some(value) => self.lower_value(value)?,
            None => zeroed(),
        };
        self.byref.insert(variable.id);
        Ok(Stmt::Let {
            name: self.binding_name(variable.id),
            mutable: false,
            ty: Some(rust::Type::Ptr {
                mutable: true,
                inner: Box::new(ty),
            }),
            init: Some(support_call("cell", vec![init])),
        })
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
        for (index, capture) in block.captures.iter().enumerate() {
            let field = format!("c{index}");
            let value_ty = self.lower_binding_type(&capture.ty, capture.id)?;
            let (ty, init) = if capture.by_reference {
                (
                    rust::Type::Ptr {
                        mutable: true,
                        inner: Box::new(value_ty),
                    },
                    Expr::Var(self.binding_name(capture.id).as_str().into()),
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
        let captures_value = if inits.is_empty() {
            Expr::Var("()".into())
        } else {
            Expr::StructLit {
                name: captures_name,
                fields: inits,
            }
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
        if self.in_static_initializer {
            let literal = format!("{function_name}_literal");
            self.dependencies.block_items.push(Item::Static {
                comments: Vec::new(),
                attrs: Vec::new(),
                vis: rust::Visibility::Private,
                mutable: false,
                name: literal.clone(),
                ty: rust::Type::Custom(format!("{SUPPORT}::Block<()>")),
                init: support_call(
                    "global",
                    vec![Expr::Cast {
                        expr: Box::new(Expr::Var(function_name.as_str().into())),
                        ty: void_pointer(false),
                    }],
                ),
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
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(support_call(
                "new",
                vec![
                    Expr::Cast {
                        expr: Box::new(Expr::Var(function_name.as_str().into())),
                        ty: void_pointer(false),
                    },
                    captures_value,
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
            unsafe_: false,
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
            variadic: false,
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
        let ret = match return_type {
            Some(ret) => self.lower_type(ret)?,
            None => rust::Type::Unit,
        };
        let mut args = vec![Expr::Var(temp.as_str().into())];
        for argument in arguments {
            args.push(self.lower_value(argument)?);
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
