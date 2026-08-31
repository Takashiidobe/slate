use super::*;

impl<'a> Lowerer<'a> {
    pub(super) fn lower_func_alias(
        &mut self,
        function: &CirFunction,
        functions: &[CirFunction],
    ) -> Option<Item> {
        let name = &function.name;
        let target = function.aliasee.as_deref()?;
        if function.linkage == GlobalLinkageKind::WeakAny {
            return None;
        }
        let target_op = functions
            .iter()
            .find(|candidate| candidate.name == target && !candidate.is_declaration());
        if target_op.is_none() {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported function alias `{name}` to external target `{target}`"
            ));
            return None;
        }

        let (decl, _, _) = self.extern_fn_signature(function);
        if decl.variadic {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported variadic function alias `{name}` to `{target}`"
            ));
            return None;
        }

        let external_def = self.project.emit_pub
            && (typed_function_is_exported(function)
                || self.project.cross_referenced_functions.contains(name));
        self.warn_protected_visibility(function.visibility, name);
        let attrs = symbol_attrs(
            external_def,
            function.linkage == GlobalLinkageKind::WeakAny,
            function.section.as_deref(),
            &[],
        );
        if function.linkage == GlobalLinkageKind::WeakAny {
            self.uses_linkage.set(true);
        }
        let args = decl
            .params
            .iter()
            .map(|param| Expr::Var(param.name.clone().into()))
            .collect();
        let mut call = Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(target.into())),
            args,
        };
        let unsafe_ =
            self.unsafe_functions.contains(name) || self.unsafe_functions.contains(target);
        if unsafe_ {
            call = FunctionLowerer::unsafe_expr(call);
        }
        let stmt = if decl.ret.is_some() {
            Stmt::Return(Some(call))
        } else {
            Stmt::Expr(call)
        };

        Some(Item::Fn(FnDef {
            attrs,
            vis: if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            },
            unsafe_,
            abi: if external_def || self.c_abi_functions.contains(name) {
                Some(Abi::C)
            } else {
                None
            },
            name: name.to_string(),
            params: decl.params,
            ret: decl.ret,
            body: vec![IndentStmt { depth: 1, stmt }],
        }))
    }

    pub(super) fn warn_unsupported_function_attributes(&mut self, function: &CirFunction) {
        let name = &function.name;
        let mut kinds: Vec<&str> = Vec::new();
        match function.side_effect {
            Some(clang_ir::enums::SideEffect::Pure) => kinds.push("pure"),
            Some(clang_ir::enums::SideEffect::Const) => kinds.push("const"),
            _ => {}
        }
        if function.hot {
            kinds.push("hot");
        }
        if let Some(extra) = self.unsupported_attribute_functions.get(name) {
            kinds.extend(extra.iter().copied());
        }
        if kinds.is_empty() {
            return;
        }
        kinds.sort_unstable();
        kinds.dedup();
        self.ctx.diagnostics.warn(format!(
            "lower: `{}` on `{name}` has no faithful Rust equivalent; attribute dropped",
            kinds.join(", ")
        ));
    }

    pub(super) fn warn_protected_visibility(
        &mut self,
        visibility: Option<VisibilityKind>,
        name: &str,
    ) {
        if visibility == Some(VisibilityKind::Protected) {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: protected visibility on `{name}` has no faithful Rust representation; falling back to default exported visibility"
                ));
        }
    }

    pub(super) fn lower_func(&mut self, function: &CirFunction) -> Option<Item> {
        let name = &function.name;
        let weak_alias_target = self.weak_aliases.values().any(|target| target == name);
        let param_types: Vec<&CirType> = function.params.iter().map(|(_, ty)| ty).collect();
        let body = function.body.as_ref()?;
        let entry = body.blocks.first()?;
        let mut needs_alloca_layout_preservation = false;
        walk_region_ops(body, &mut |op| {
            if matches!(op, Op::PtrDiff(_)) {
                needs_alloca_layout_preservation = true;
            }
            true
        });
        let is_main = name == "main";
        let is_variadic = !is_main && function.varargs;
        let boxed_variadic = self.boxed_variadic_defs.contains(name);

        let mut declared_param_names = BTreeSet::new();
        let mut entry_arg_names = BTreeMap::new();
        let mut params = entry
            .args
            .iter()
            .enumerate()
            .map(|(i, (arg, ty))| {
                let ty = param_types.get(i).copied().unwrap_or(ty);
                let base = sanitize_ident(arg).into_string();
                let rust_name = if is_main {
                    declared_param_names.insert(base.clone());
                    base
                } else {
                    let mut suffix = 1;
                    loop {
                        let candidate = if suffix == 1 {
                            base.clone()
                        } else {
                            format!("{base}{suffix}")
                        };
                        if !self.globals.contains_key(&candidate)
                            && !self.extern_globals.contains_key(&candidate)
                            && declared_param_names.insert(candidate.clone())
                        {
                            break candidate;
                        }
                        suffix += 1;
                    }
                };
                entry_arg_names.insert(arg.clone(), rust_name.clone());
                let rust_ty = self.rust_type(ty);
                FnParam {
                    name: rust_name,
                    mutable: is_boxed_va_args_type(&rust_ty),
                    ty: rust_ty,
                }
            })
            .collect::<Vec<_>>();

        let va_args_param = if is_variadic {
            let param = "__slate_va_args".to_string();
            params.push(FnParam {
                name: param.clone(),
                mutable: true,
                ty: if boxed_variadic {
                    Type::Custom("__SlateVaArgs".into())
                } else {
                    Type::Variadic
                },
            });
            Some(param)
        } else {
            None
        };

        let (vis, abi, ret, prelude) = if is_main {
            params.clear();
            let mut prelude = self.main_arg_bindings(entry);
            prelude.extend(
                self.ctor_calls
                    .iter()
                    .map(|name| hook_call_stmt(name, &self.unsafe_functions)),
            );
            (Visibility::Private, None, None, prelude)
        } else {
            let external_def = self.project.emit_pub
                && (typed_function_is_exported(function)
                    || self.project.cross_referenced_functions.contains(name))
                || weak_alias_target;
            let vis = if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            let abi = if !boxed_variadic
                && (external_def || is_variadic || self.c_abi_functions.contains(name))
            {
                Some(Abi::C)
            } else {
                None
            };
            if is_variadic {
                if !boxed_variadic {
                    self.uses_c_variadic.set(true);
                }
                self.variadic_defs.insert(name.to_string());
            }
            let ret = Some(self.rust_type(&function.return_ty))
                .filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID));
            (vis, abi, ret, Vec::<Stmt>::new())
        };

        let diverges = !is_main && function.noreturn && region_ends_in_noreturn_call(body);
        if !is_main && function.noreturn && !diverges {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: __attribute__((noreturn)) on `{name}` does not structurally prove divergence; keeping its declared return type"
                ));
        }
        let ret = if diverges { Some(Type::Never) } else { ret };

        if self.c_abi_functions.contains(name)
            && (params
                .iter()
                .any(|param| type_mentions_long_double(&param.ty))
                || ret.as_ref().is_some_and(type_mentions_long_double))
        {
            let ret_shim_ty = ret.clone().unwrap_or(Type::Unit);
            let trampoline = format!("__slate_ld_{}", sanitize_ident(name));
            self.long_double_shims
                .entry(trampoline.clone())
                .or_insert_with(|| ExternFnDecl {
                    attrs: Vec::new(),
                    identity: FunctionIdentity::Unknown,
                    name: trampoline.clone(),
                    declared_type: None,
                    params: params
                        .iter()
                        .enumerate()
                        .map(|(i, param)| FnParam {
                            name: format!("_{i}"),
                            mutable: false,
                            ty: param.ty.clone(),
                        })
                        .collect(),
                    variadic: false,
                    ret: (!ret_shim_ty.is_unit()).then_some(ret_shim_ty),
                    safe: true,
                });
            self.long_double_callback_trampolines
                .insert(name.to_string(), trampoline);
        }

        let mut attrs = symbol_attrs(
            !is_main
                && (self.project.emit_pub
                    && (typed_function_is_exported(function)
                        || self.project.cross_referenced_functions.contains(name))
                    || weak_alias_target
                    || self.long_double_callback_trampolines.contains_key(name)),
            function.linkage == GlobalLinkageKind::WeakAny,
            function.section.as_deref(),
            &[],
        );
        let rust_fn_name = sanitize_ident(name);
        if (rust_fn_name.as_str() != name || !rust_fn_name.as_str().is_ascii())
            && let Some(no_mangle) = attrs
                .iter()
                .position(|attr| matches!(attr, RustAttr::NoMangle))
        {
            attrs[no_mangle] = RustAttr::ExportName(name.to_string());
        }
        if let Some(features) = self.target_feature_functions.get(name) {
            attrs.push(RustAttr::TargetFeature(features.join(",")));
        }
        if function.cold {
            attrs.push(RustAttr::Cold);
        }
        if self.always_inline_functions.contains(name)
            && !self.target_feature_functions.contains_key(name)
        {
            attrs.push(RustAttr::Inline(InlineHint::Always));
        }
        if self.noinline_functions.contains(name) {
            attrs.push(RustAttr::Inline(InlineHint::Never));
        }
        if self.must_use_functions.contains(name) {
            attrs.push(RustAttr::MustUse);
        }
        if let Some(message) = self.deprecated_functions.get(name) {
            attrs.push(RustAttr::Deprecated(message.clone()));
        }
        self.warn_unsupported_function_attributes(function);
        self.warn_protected_visibility(function.visibility, name);
        if function.linkage == GlobalLinkageKind::WeakAny {
            self.uses_linkage.set(true);
        }
        let unsafe_ = is_variadic || self.unsafe_functions.contains(name);
        let layout_queries: VecDeque<_> = self
            .layout_queries
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let macro_consts: VecDeque<_> = self
            .macro_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let enum_consts: VecDeque<_> = self
            .enum_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let asm_gotos: VecDeque<_> = self.asm_gotos.get(name).cloned().unwrap_or_default().into();
        if self.naked_functions.contains(name) {
            return self.lower_naked_func(name, body, attrs, vis, params, ret);
        }
        let local_enum_decls = self
            .local_enum_decls
            .get(name)
            .map_or(&[][..], Vec::as_slice);
        let integer_enum_locals =
            enum_locals_requiring_integer_storage(local_enum_decls, &self.enums, body);
        let local_enum_types: BTreeMap<String, String> = local_enum_decls
            .iter()
            .filter(|decl| self.enums.contains_key(&decl.enum_name))
            .filter(|decl| !integer_enum_locals.contains(&decl.name))
            .map(|decl| (decl.name.clone(), decl.enum_name.clone()))
            .collect();
        let mut va_allocas = BTreeSet::new();
        walk_region_ops(body, &mut |op| {
            match op {
                Op::VaStart(op) => {
                    va_allocas.insert(op.arg_list.clone());
                }
                Op::VaArg(op) => {
                    va_allocas.insert(op.arg_list.clone());
                }
                Op::VaCopy(op) => {
                    va_allocas.insert(op.src_list.clone());
                    va_allocas.insert(op.dst_list.clone());
                }
                _ => {}
            }
            true
        });
        let mut f = FunctionLowerer {
            parent: self,
            values: BTreeMap::new(),
            value_types: BTreeMap::new(),
            const_int_values: BTreeMap::new(),
            function_pointer_null_values: BTreeSet::new(),
            slots: BTreeMap::new(),
            slot_places: BTreeMap::new(),
            aligned_slots: BTreeSet::new(),
            slot_types: BTreeMap::new(),
            needs_alloca_layout_preservation,
            member_ptrs: BTreeMap::new(),
            element_ptrs: BTreeMap::new(),
            block_addr_element_ptrs: BTreeMap::new(),
            local_block_addr_arrays: BTreeMap::new(),
            indirect_target_values: BTreeMap::new(),
            temp_counter: 0,
            indent: 1,
            body: Vec::new(),
            is_main,
            loop_stack: Vec::new(),
            label_counter: 0,
            dispatch: None,
            hoisting_allocas: false,
            hoisted: BTreeSet::new(),
            resolved_bi_allocas: BTreeSet::new(),
            declared_local_names: declared_param_names,
            forward_allocas: forwardable_temp_allocas(body),
            forward_values: BTreeMap::new(),
            immutable_temps: BTreeSet::new(),
            va_allocas,
            va_places: BTreeMap::new(),
            va_args_param,
            layout_queries,
            macro_consts,
            enum_consts,
            macro_arith_values: BTreeMap::new(),
            asm_outputs: BTreeMap::new(),
            asm_gotos,
            asm_output_places: BTreeMap::new(),
            local_enum_types,
            loaded_field_types: BTreeMap::new(),
            load_ptr_operand: BTreeMap::new(),
            member_base_operand: BTreeMap::new(),
            coerce_alloca_real_type: BTreeMap::new(),
        };

        for stmt in prelude {
            f.push_stmt(stmt);
        }
        for (arg, arg_ty) in &entry.args {
            let rust_name = entry_arg_names
                .get(arg)
                .cloned()
                .unwrap_or_else(|| arg.clone());
            let boxed_va_args = is_boxed_va_args_type(&f.parent.rust_type(arg_ty));
            if boxed_va_args {
                f.va_places
                    .insert(arg.clone(), Expr::Var(rust_name.clone().into()));
            } else {
                f.immutable_temps.insert(rust_name.clone());
            }
            f.values
                .insert(arg.clone(), Val::Expr(Expr::Var(rust_name.into())));
            f.value_types.insert(arg.clone(), arg_ty.clone());
            if let fn_ptr_ty @ Type::FnPtr { .. } = f.parent.rust_type(arg_ty) {
                f.loaded_field_types.insert(arg.clone(), fn_ptr_ty);
            }
        }
        let entry = body.blocks.first().unwrap();
        if body.blocks.len() > 1 {
            let returns_value = !matches!(ret, None | Some(Type::Unit));
            f.lower_dispatch(body, returns_value);
        } else {
            f.lower_constant_size_bi_alloca_group(entry);
            f.lower_block(entry);
        }
        if diverges && matches!(f.body.last().map(|s| &s.stmt), Some(Stmt::Return(None))) {
            f.body.pop();
        }
        Some(Item::Fn(FnDef {
            attrs,
            vis,
            unsafe_,
            abi,
            name: sanitize_ident(name).into_string(),
            params,
            ret,
            body: f.body,
        }))
    }

    pub(super) fn lower_naked_func(
        &mut self,
        name: &str,
        body: &inst::Region,
        mut attrs: Vec<RustAttr>,
        vis: Visibility,
        params: Vec<FnParam>,
        ret: Option<Type>,
    ) -> Option<Item> {
        let mut asm_ops = Vec::new();
        walk_region_ops(body, &mut |op| {
            if let Op::Asm(asm) = op {
                asm_ops.push(asm.clone());
            }
            true
        });
        let Some(dialect) = asm_ops.first().map(|op| cir_asm_dialect(op.asm_flavor)) else {
            self.ctx.diagnostics.error(format!(
                "lower: __attribute__((naked)) function `{name}` has no inline assembly body"
            ));
            return None;
        };
        let mut lines = Vec::with_capacity(asm_ops.len());
        for asm_op in asm_ops {
            let Ok(template) = String::from_utf8(decode_cir_string(&asm_op.asm_string)) else {
                self.ctx
                    .diagnostics
                    .error("lower: inline assembly template is not valid UTF-8");
                return None;
            };
            lines.push(template.replace("$$", "$"));
        }
        attrs.push(RustAttr::Naked);
        let stmt = Stmt::Expr(Expr::Macro {
            name: "core::arch::naked_asm".into(),
            args: asm_macro_args(lines.join("\n\t"), dialect),
        });
        Some(Item::Fn(FnDef {
            attrs,
            vis,
            unsafe_: false,
            abi: Some(Abi::C),
            name: name.to_string(),
            params,
            ret,
            body: vec![IndentStmt { depth: 1, stmt }],
        }))
    }

    pub(super) fn main_arg_bindings(&self, entry: &inst::Block) -> Vec<Stmt> {
        if entry.args.is_empty() {
            return Vec::new();
        }

        let call = |path: &str, args: Vec<Expr>| Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(path.into())),
            args,
        };
        let method = |recv: Expr, name: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: name.into(),
            args,
        };
        let char_ptr = Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Prim(Prim::I8)),
        };

        let storage_init = method(
            method(
                call("std::env::args", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(method(
                        call("std::ffi::CString::new", vec![Expr::Var("arg".into())]),
                        "unwrap",
                        vec![],
                    )),
                }],
            ),
            "collect",
            vec![],
        );
        let ptrs_init = method(
            method(
                method(Expr::Var("__slate_argv_storage".into()), "iter", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(Expr::Cast {
                        expr: Box::new(method(Expr::Var("arg".into()), "as_ptr", vec![])),
                        ty: char_ptr.clone(),
                    }),
                }],
            ),
            "collect",
            vec![],
        );

        let mut stmts = vec![
            Stmt::Let {
                name: "__slate_argv_storage".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![Type::Custom("std::ffi::CString".into())],
                }),
                init: Some(storage_init),
            },
            Stmt::Let {
                name: "__slate_argv_ptrs".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![char_ptr],
                }),
                init: Some(ptrs_init),
            },
            Stmt::Expr(method(
                Expr::Var("__slate_argv_ptrs".into()),
                "push",
                vec![Expr::Value(RustValue::NullPtr)],
            )),
        ];

        for (i, (arg, ty)) in entry.args.iter().enumerate() {
            let value = match i {
                0 => Expr::Cast {
                    expr: Box::new(method(
                        Expr::Var("__slate_argv_storage".into()),
                        "len",
                        vec![],
                    )),
                    ty: Type::Prim(Prim::I32),
                },
                1 => method(Expr::Var("__slate_argv_ptrs".into()), "as_mut_ptr", vec![]),
                _ => Expr::Value(RustValue::NullPtr),
            };
            stmts.push(Stmt::Let {
                name: sanitize_ident(arg).into_string(),
                mutable: false,
                ty: Some(self.rust_type(ty)),
                init: Some(value),
            });
        }
        stmts
    }

    pub(super) fn extern_fn_signature(
        &self,
        function: &CirFunction,
    ) -> (ExternFnDecl, Vec<Type>, Option<String>) {
        self.extern_fn_signature_as(&function.name, function)
    }

    pub(super) fn extern_fn_signature_as(
        &self,
        name: &str,
        function: &CirFunction,
    ) -> (ExternFnDecl, Vec<Type>, Option<String>) {
        let params = function
            .params
            .iter()
            .enumerate()
            .map(|(i, (_, ty))| FnParam {
                name: format!("_{i}"),
                mutable: false,
                ty: self.rust_type(ty),
            })
            .collect::<Vec<_>>();
        let ret_ast = Some(self.rust_type(&function.return_ty))
            .filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID));
        let identity = *self
            .known_functions
            .get(name)
            .unwrap_or(&FunctionIdentity::Unknown);
        let mut decl = ExternFnDecl {
            attrs: Vec::new(),
            name: name.into(),
            identity,
            declared_type: self.function_types.get(name).cloned(),
            params,
            variadic: function.varargs,
            ret: ret_ast,
            safe: false,
        };
        repair_extern_function_signature(
            &mut decl,
            self.function_types.get(name).map(String::as_str),
        );
        let param_types = decl.params.iter().map(|param| param.ty.clone()).collect();
        let ret_ty = decl.ret.as_ref().map(Type::render);
        if decl.variadic || decl.params.iter().any(|param| param.ty == Type::VaList) {
            self.uses_c_variadic.set(true);
        }
        (decl, param_types, ret_ty)
    }
}
