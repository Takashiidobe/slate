use super::*;

pub(super) struct LifecycleHooks {
    pub(super) ctors: Vec<String>,
    pub(super) dtors: Vec<String>,
}

pub(super) fn collect_lifecycle_hooks(
    functions: &[clang_ir::model::Function],
    has_main: bool,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> LifecycleHooks {
    let mut ctors: Vec<(i64, String)> = Vec::new();
    let mut dtors: Vec<(i64, String)> = Vec::new();
    for function in functions {
        if function.is_declaration() {
            continue;
        }
        let Some(op) = function.raw.as_ref() else {
            continue;
        };
        let is_ctor = op.attr("global_ctor_priority").is_some();
        let is_dtor = op.attr("global_dtor_priority").is_some();
        if !is_ctor && !is_dtor {
            continue;
        }
        let name = &function.name;
        if !function.params.is_empty() {
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

pub(super) fn region_ops(op: &Operation) -> Vec<&Operation> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
}

pub(super) fn cross_block_live_values(body: &inst::Region) -> BTreeSet<String> {
    fn walk(
        ops: &[Op],
        block_index: usize,
        def_block: &mut BTreeMap<String, usize>,
        used_in: &mut BTreeMap<String, BTreeSet<usize>>,
    ) {
        for op in ops {
            op.for_each_result(|result, _| {
                def_block.entry(result.clone()).or_insert(block_index);
            });
            op.for_each_operand(|operand| {
                used_in
                    .entry(operand.clone())
                    .or_default()
                    .insert(block_index);
            });
            op.for_each_region(|region| {
                for block in &region.blocks {
                    walk(&block.ops, block_index, def_block, used_in);
                }
            });
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

pub(super) fn collect_used_symbols(
    globals: &[clang_ir::model::Global],
) -> BTreeMap<String, Vec<UsedKind>> {
    let mut flags = BTreeMap::<String, (bool, bool)>::new();
    for global in globals {
        let Some(kind) = (match global.name.as_str() {
            "llvm.compiler.used" => Some(UsedKind::Compiler),
            "llvm.used" => Some(UsedKind::Linker),
            _ => None,
        }) else {
            continue;
        };
        let Some(init) = global.initial_value.as_ref() else {
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

pub(super) fn collect_region_ops_recursive<'a>(op: &'a Operation, out: &mut Vec<&'a Operation>) {
    for child in region_ops(op) {
        out.push(child);
        collect_region_ops_recursive(child, out);
    }
}

pub(super) fn enum_locals_requiring_integer_storage(
    declarations: &[crate::frontend::c_ast::LocalEnumDecl],
    enums: &BTreeMap<String, crate::frontend::c_ast::Enum>,
    ops: &[&Operation],
) -> BTreeSet<String> {
    let declared: BTreeMap<_, _> = declarations
        .iter()
        .map(|declaration| (declaration.name.as_str(), declaration.enum_name.as_str()))
        .collect();
    let slots: BTreeMap<_, _> = ops
        .iter()
        .filter(|op| op.mnemonic() == "alloca")
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
        .filter(|op| op.mnemonic() == "const")
        .filter_map(|op| {
            let (result, _) = op.results.first()?;
            let value = op.attr("value").and_then(Attr::as_int)?;
            Some((result.as_str(), value))
        })
        .collect();
    ops.iter()
        .filter(|op| op.mnemonic() == "store")
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
        Attr::GlobalView { symbol, .. } => out.push(symbol.clone()),
        Attr::ConstArray { elts, .. }
        | Attr::ConstRecord { members: elts, .. }
        | Attr::ConstVector { elts, .. } => collect_global_view_symbols(elts, out),
        Attr::Array(elements) => {
            for element in elements {
                collect_global_view_symbols(element, out);
            }
        }
        _ => {}
    }
}

pub(super) fn c_abi_function_targets(module: &Module) -> BTreeSet<String> {
    let mut ops = Vec::new();
    for function in &module.functions {
        if let Some(op) = function.raw.as_ref() {
            collect_region_ops_recursive(op, &mut ops);
        }
    }
    let mut targets: BTreeSet<String> = ops
        .iter()
        .filter(|op| op.mnemonic() == "get_global")
        .filter(|op| op_result_type(op).is_some_and(is_cir_function_pointer_type))
        .filter_map(|op| attr_symbol_ref(op, "name").map(str::to_string))
        .collect();
    for init in module
        .globals
        .iter()
        .filter_map(|global| global.initial_value.as_ref())
    {
        let mut symbols = Vec::new();
        collect_global_view_symbols(init, &mut symbols);
        targets.extend(symbols);
    }
    targets
}

pub(super) fn module_requires_native_va_list(
    module: &Module,
    c_abi_functions: &BTreeSet<String>,
    emit_pub: bool,
    aliases: &BTreeMap<String, CirType>,
) -> bool {
    let mut ops = Vec::new();
    for function in &module.functions {
        if let Some(op) = function.raw.as_ref() {
            collect_region_ops_recursive(op, &mut ops);
        }
    }
    let defined_functions: BTreeSet<&str> = module
        .functions
        .iter()
        .filter(|function| !function.is_declaration())
        .map(|function| function.name.as_str())
        .collect();
    module
        .functions
        .iter()
        .filter(|function| !function.is_declaration())
        .any(|function| {
            let has_va_list = function.varargs
                || function
                    .params
                    .iter()
                    .any(|(_, ty)| is_cir_va_list_type(ty, aliases));
            has_va_list
                && (c_abi_functions.contains(&function.name)
                    || (emit_pub && function.raw.as_ref().is_some_and(externally_exported)))
        })
        || ops.iter().any(|op| {
            if op.mnemonic() == "call" {
                attr_symbol_ref(op, "callee")
                    .is_some_and(|callee| !defined_functions.contains(callee))
                    && op_operand_types(op)
                        .iter()
                        .any(|ty| is_cir_va_list_type(ty, aliases))
            } else {
                false
            }
        })
}

pub(super) fn attr_str<'a>(op: &'a Operation, key: &str) -> Option<&'a str> {
    op.attr(key).and_then(Attr::as_str)
}
