use super::*;

/// Constructor/destructor call order for one translation unit.
pub(super) struct LifecycleHooks {
    /// `__attribute__((constructor))` functions, ascending priority (ties in
    /// declaration order) — matches `.init_array` execution order.
    pub(super) ctors: Vec<String>,
    /// `__attribute__((destructor))` functions, in the reverse of their own
    /// ascending-priority/declaration-order build list — matches how
    /// `.fini_array` is built like `.init_array` but run back to front.
    pub(super) dtors: Vec<String>,
}

/// Scans top-level `cir.func` ops for `global_ctor_priority`/`global_dtor_priority`
/// (present whenever the source had `__attribute__((constructor))`/`(destructor)`,
/// defaulting to priority 65535 when none was given) and orders the hooks the way
/// glibc's `.init_array`/`.fini_array` would run them. Hooks this TU can't wire up
/// (no `main` to splice into, or a non-`void(void)` signature) are diagnosed and
/// dropped rather than silently ignored.
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
        let is_ctor = op.attrs.contains_key("global_ctor_priority");
        let is_dtor = op.attrs.contains_key("global_dtor_priority");
        if !is_ctor && !is_dtor {
            continue;
        }
        let Some(name) = attr_str(op, "sym_name") else {
            continue;
        };
        if function_type_has_params(attr_str(op, "function_type").unwrap_or("")) {
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

/// A no-arg call to a locally defined function, wrapped in `unsafe {}` when the
/// callee requires it (mirrors the wrapping `lower_call` applies at call sites).
pub(super) fn hook_call_stmt(name: &str, unsafe_functions: &BTreeSet<String>) -> Stmt {
    let call = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(name.to_string().into())),
        args: Vec::new(),
    };
    let call = if unsafe_functions.contains(name) {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
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
            for result in &op.results {
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
        let Some(init) = attr_str(op, "initial_value") else {
            continue;
        };
        for symbol in parse_cir_global_view_array(init) {
            let name = sanitize_ident(symbol).into_string();
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
    declarations: &[crate::c_ast::LocalEnumDecl],
    enums: &BTreeMap<String, crate::c_ast::Enum>,
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
            let result = op.results.first()?;
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
            let result = op.results.first()?;
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
        .filter_map(|op| attr_str(op, "initial_value"))
    {
        targets.extend(parse_cir_global_views(init).into_iter().map(str::to_string));
    }
    targets
}

pub(super) fn declared_function_param_types(
    op: &Op,
    aliases: &BTreeMap<String, String>,
) -> BTreeMap<String, Vec<Type>> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    ops.iter()
        .filter(|op| op.kind() == CirOpKind::Func)
        .filter_map(|op| {
            let name = attr_str(op, "sym_name")?;
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (param_tys, _) = parse_function_type(function_type);
            let params = param_tys
                .iter()
                .map(|ty| rust_type_with_aliases(ty, aliases))
                .collect();
            Some((name.to_string(), params))
        })
        .collect()
}

pub(super) fn declared_function_return_types(
    op: &Op,
    aliases: &BTreeMap<String, String>,
) -> BTreeMap<String, Type> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    ops.iter()
        .filter(|op| op.kind() == CirOpKind::Func)
        .filter_map(|op| {
            let name = attr_str(op, "sym_name")?;
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (_, ret_ty) = parse_function_type(function_type);
            let ty = rust_type_with_aliases(ret_ty.as_deref().unwrap_or("()"), aliases);
            Some((name.to_string(), ty))
        })
        .collect()
}

pub(super) fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}
