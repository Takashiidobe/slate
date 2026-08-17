use super::*;

pub(super) struct LifecycleHooks {
    pub(super) ctors: Vec<String>,
    pub(super) dtors: Vec<String>,
}

pub(super) fn collect_lifecycle_hooks(
    ops: &[&Op],
    has_main: bool,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> LifecycleHooks {
    let mut ctors: Vec<(i64, String)> = Vec::new();
    let mut dtors: Vec<(i64, String)> = Vec::new();
    for op in ops {
        if op.kind() != CirOpKind::Func || region_ops(op).is_empty() {
            continue;
        }
        let is_ctor = op.attr("global_ctor_priority").is_some();
        let is_dtor = op.attr("global_dtor_priority").is_some();
        if !is_ctor && !is_dtor {
            continue;
        }
        let Some(name) = attr_str(op, "sym_name") else {
            continue;
        };
        if attr_type(op, "function_type").is_some_and(function_type_has_params) {
            diagnostics.warn(
                format!(
                    "lower: __attribute__((constructor/destructor)) on `{name}` with a non-void(void) signature is not supported; hook dropped"
                ));
            continue;
        }
        if !has_main {
            diagnostics.warn(
                format!(
                    "lower: __attribute__((constructor/destructor)) on `{name}` needs a `main` in this translation unit to splice into; hook dropped"
                ));
            continue;
        }
        if is_ctor {
            ctors.push((
                attr_int(op, "global_ctor_priority").unwrap_or(65535),
                name.to_string(),
            ));
        }
        if is_dtor {
            dtors.push((
                attr_int(op, "global_dtor_priority").unwrap_or(65535),
                name.to_string(),
            ));
        }
    }
    ctors.sort_by_key(|(prio, _)| *prio);
    dtors.sort_by_key(|(prio, _)| *prio);
    let mut dtor_order: Vec<String> = dtors.into_iter().map(|(_, name)| name).collect();
    dtor_order.reverse();
    LifecycleHooks {
        ctors: ctors.into_iter().map(|(_, name)| name).collect(),
        dtors: dtor_order,
    }
}

pub(super) fn hook_call_stmt(name: &str, unsafe_functions: &BTreeSet<String>) -> Stmt {
    let call = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(name.to_string().into())),
        args: Vec::new(),
    };
    let call = if unsafe_functions.contains(name) {
        Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(call)),
        }))
    } else {
        call
    };
    Stmt::Expr(call)
}

pub(super) fn region_ops(op: &Op) -> Vec<&Op> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
}

pub(super) fn cross_block_live_values(body: &Region) -> BTreeSet<String> {
    fn walk(
        ops: &[Op],
        block_index: usize,
        def_block: &mut BTreeMap<String, usize>,
        used_in: &mut BTreeMap<String, BTreeSet<usize>>,
    ) {
        for op in ops {
            for (result, _) in &op.results {
                def_block.entry(result.clone()).or_insert(block_index);
            }
            for operand in &op.operands {
                used_in
                    .entry(operand.clone())
                    .or_default()
                    .insert(block_index);
            }
            for region in &op.regions {
                for block in &region.blocks {
                    walk(&block.ops, block_index, def_block, used_in);
                }
            }
        }
    }
    let mut def_block: BTreeMap<String, usize> = BTreeMap::new();
    let mut used_in: BTreeMap<String, BTreeSet<usize>> = BTreeMap::new();
    for (i, block) in body.blocks.iter().enumerate() {
        walk(&block.ops, i, &mut def_block, &mut used_in);
    }
    def_block
        .into_iter()
        .filter(|(ssa, def_i)| {
            used_in
                .get(ssa)
                .is_some_and(|uses| uses.iter().any(|used_block| used_block != def_i))
        })
        .map(|(ssa, _)| ssa)
        .collect()
}

pub(super) fn collect_used_symbols(ops: &[&Op]) -> BTreeMap<String, Vec<UsedKind>> {
    let mut flags = BTreeMap::<String, (bool, bool)>::new();
    for op in ops {
        if op.kind() != CirOpKind::Global {
            continue;
        }
        let Some(kind) = (match attr_str(op, "sym_name") {
            Some("llvm.compiler.used") => Some(UsedKind::Compiler),
            Some("llvm.used") => Some(UsedKind::Linker),
            _ => None,
        }) else {
            continue;
        };
        let Some(init) = op.attr("initial_value") else {
            continue;
        };
        let mut symbols = Vec::new();
        collect_global_view_symbols(init, &mut symbols);
        for symbol in symbols {
            let name = sanitize_ident(&symbol).into_string();
            let entry = flags.entry(name).or_default();
            match kind {
                UsedKind::Compiler => entry.0 = true,
                UsedKind::Linker => entry.1 = true,
                UsedKind::Plain => {}
            }
        }
    }

    flags
        .into_iter()
        .map(|(name, (compiler, linker))| {
            let mut kinds = Vec::new();
            if compiler {
                kinds.push(UsedKind::Plain);
            }
            if linker {
                kinds.push(UsedKind::Linker);
            }
            (name, kinds)
        })
        .collect()
}

pub(super) fn collect_region_ops_recursive<'a>(op: &'a Op, out: &mut Vec<&'a Op>) {
    for child in region_ops(op) {
        out.push(child);
        collect_region_ops_recursive(child, out);
    }
}

pub(super) fn enum_locals_requiring_integer_storage(
    declarations: &[crate::frontend::c_ast::LocalEnumDecl],
    enums: &BTreeMap<String, crate::frontend::c_ast::Enum>,
    ops: &[&Op],
) -> BTreeSet<String> {
    let declared: BTreeMap<_, _> = declarations
        .iter()
        .map(|declaration| (declaration.name.as_str(), declaration.enum_name.as_str()))
        .collect();
    let slots: BTreeMap<_, _> = ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::Alloca)
        .filter_map(|op| {
            let (result, _) = op.results.first()?;
            let name = attr_str(op, "name")?;
            declared
                .contains_key(name)
                .then_some((result.as_str(), name))
        })
        .collect();
    let constants: BTreeMap<_, _> = ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::Const)
        .filter_map(|op| {
            let (result, _) = op.results.first()?;
            let value = attr_str(op, "value").and_then(parse_cir_int)?;
            Some((result.as_str(), value))
        })
        .collect();
    ops.iter()
        .filter(|op| op.kind() == CirOpKind::Store)
        .filter_map(|op| {
            let [value, slot, ..] = op.operands.as_slice() else {
                return None;
            };
            let name = slots.get(slot.as_str())?;
            let enum_name = declared.get(name)?;
            let value = constants.get(value.as_str())?;
            let enm = enums.get(*enum_name)?;
            (!enm
                .variants
                .iter()
                .any(|variant| i128::from(variant.value) == *value))
            .then(|| (*name).to_string())
        })
        .collect()
}

fn collect_global_view_symbols(attr: &Attr, out: &mut Vec<String>) {
    match attr {
        Attr::GlobalView { symbol, .. } => out.push(symbol.trim_start_matches('@').to_string()),
        Attr::ConstArray {
            data: clang_ir::ast::ConstArrayData::Elements(elements),
            ..
        } => {
            for element in elements {
                collect_global_view_symbols(element, out);
            }
        }
        Attr::ConstRecord { elements, .. } | Attr::Array(elements) => {
            for element in elements {
                collect_global_view_symbols(element, out);
            }
        }
        _ => {}
    }
}

pub(super) fn c_abi_function_targets(op: &Op) -> BTreeSet<String> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    let mut targets: BTreeSet<String> = ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::GetGlobal)
        .filter(|op| op_result_type(op).is_some_and(is_cir_function_pointer_type))
        .filter_map(|op| attr_symbol_ref(op, "name").map(str::to_string))
        .collect();
    for init in ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::Global)
        .filter_map(|op| op.attr("initial_value"))
    {
        let mut symbols = Vec::new();
        collect_global_view_symbols(init, &mut symbols);
        targets.extend(symbols);
    }
    targets
}

pub(super) fn module_requires_native_va_list(
    module_op: &Op,
    c_abi_functions: &BTreeSet<String>,
    emit_pub: bool,
    aliases: &BTreeMap<String, CirType>,
) -> bool {
    let mut ops = Vec::new();
    collect_region_ops_recursive(module_op, &mut ops);
    let defined_functions: BTreeSet<&str> = ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::Func && !region_ops(op).is_empty())
        .filter_map(|op| attr_str(op, "sym_name"))
        .collect();
    ops.iter().any(|op| match op.kind() {
        CirOpKind::Func if !region_ops(op).is_empty() => {
            let name = attr_str(op, "sym_name").unwrap_or_default();
            let has_va_list = attr_type(op, "function_type").is_some_and(|function_type| {
                function_type_is_variadic(function_type)
                    || parse_function_type(function_type)
                        .0
                        .iter()
                        .any(|ty| is_cir_va_list_type(ty, aliases))
            });
            has_va_list && (c_abi_functions.contains(name) || (emit_pub && externally_exported(op)))
        }
        CirOpKind::Call => {
            attr_str(op, "callee")
                .map(|callee| callee.trim_start_matches('@'))
                .is_some_and(|callee| !defined_functions.contains(callee))
                && op_operand_types(op)
                    .iter()
                    .any(|ty| is_cir_va_list_type(ty, aliases))
        }
        _ => false,
    })
}

pub(super) fn declared_function_param_types(
    op: &Op,
    aliases: &BTreeMap<String, CirType>,
    va_list_boxed: bool,
) -> BTreeMap<String, Vec<Type>> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    ops.iter()
        .filter(|op| op.kind() == CirOpKind::Func)
        .filter_map(|op| {
            let name = attr_str(op, "sym_name")?;
            let function_type = attr_type(op, "function_type")?;
            let (param_tys, _) = parse_function_type(function_type);
            let params = param_tys
                .iter()
                .map(|ty| rust_type_with_aliases(ty, aliases, va_list_boxed))
                .collect();
            Some((name.to_string(), params))
        })
        .collect()
}

pub(super) fn declared_function_return_types(
    op: &Op,
    aliases: &BTreeMap<String, CirType>,
    va_list_boxed: bool,
) -> BTreeMap<String, Type> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    ops.iter()
        .filter(|op| op.kind() == CirOpKind::Func)
        .filter_map(|op| {
            let name = attr_str(op, "sym_name")?;
            let function_type = attr_type(op, "function_type")?;
            let (_, ret_ty) = parse_function_type(function_type);
            let ty = ret_ty
                .as_ref()
                .map(|ty| rust_type_with_aliases(ty, aliases, va_list_boxed))
                .unwrap_or(Type::Unit);
            Some((name.to_string(), ty))
        })
        .collect()
}

pub(super) fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attr(key).and_then(Attr::as_str)
}
