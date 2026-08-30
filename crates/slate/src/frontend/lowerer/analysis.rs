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
        let is_ctor = function.ctor_priority.is_some();
        let is_dtor = function.dtor_priority.is_some();
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
            ctors.push((function.ctor_priority.unwrap_or(65535), name.to_string()));
        }
        if is_dtor {
            dtors.push((function.dtor_priority.unwrap_or(65535), name.to_string()));
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

pub(super) fn cross_block_live_values(body: &inst::Region) -> BTreeSet<String> {
    let mut def_block: BTreeMap<String, usize> = BTreeMap::new();
    let mut used_in: BTreeMap<String, BTreeSet<usize>> = BTreeMap::new();
    let mut forward_bases: BTreeMap<String, Vec<String>> = BTreeMap::new();
    for (block_index, block) in body.blocks.iter().enumerate() {
        walk_block_ops(&block.ops, &mut |op| {
            op.for_each_result(|result, _| {
                def_block.entry(result.clone()).or_insert(block_index);
            });
            op.for_each_operand(|operand| {
                used_in
                    .entry(operand.clone())
                    .or_default()
                    .insert(block_index);
            });
            if let Some((result, bases)) = inlined_address_bases(op) {
                forward_bases.insert(result, bases);
            }
            true
        });
    }
    let mut changed = true;
    while changed {
        changed = false;
        for (result, bases) in &forward_bases {
            let Some(result_uses) = used_in.get(result).cloned() else {
                continue;
            };
            for base in bases {
                let base_uses = used_in.entry(base.clone()).or_default();
                for block in &result_uses {
                    changed |= base_uses.insert(*block);
                }
            }
        }
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

fn inlined_address_bases(op: &Op) -> Option<(String, Vec<String>)> {
    match op {
        Op::GetMember(op) => Some((op.result.clone(), vec![op.addr.clone()])),
        Op::GetElement(op) => Some((op.result.clone(), vec![op.base.clone(), op.index.clone()])),
        Op::ComplexRealPtr(op) => Some((op.result.clone(), vec![op.operand.clone()])),
        Op::ComplexImagPtr(op) => Some((op.result.clone(), vec![op.operand.clone()])),
        _ => None,
    }
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

pub(super) fn enum_locals_requiring_integer_storage(
    declarations: &[crate::frontend::c_ast::LocalEnumDecl],
    enums: &BTreeMap<String, crate::frontend::c_ast::Enum>,
    body: &inst::Region,
) -> BTreeSet<String> {
    let declared: BTreeMap<_, _> = declarations
        .iter()
        .map(|declaration| (declaration.name.as_str(), declaration.enum_name.as_str()))
        .collect();
    let mut slots = BTreeMap::new();
    let mut constants = BTreeMap::new();
    let mut stores = Vec::new();
    walk_region_ops(body, &mut |op| {
        match op {
            Op::Alloca(op) if declared.contains_key(op.name.as_str()) => {
                slots.insert(op.addr.clone(), op.name.clone());
            }
            Op::Const(op) => {
                if let Some(value) = op.value.as_int() {
                    constants.insert(op.res.clone(), value);
                }
            }
            Op::Store(op) => stores.push((op.value.clone(), op.addr.clone())),
            _ => {}
        }
        true
    });
    stores
        .iter()
        .filter_map(|(value, slot)| {
            let name = slots.get(slot)?;
            let enum_name = declared.get(name.as_str())?;
            let value = constants.get(value)?;
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
    let mut targets = BTreeSet::new();
    for function in &module.functions {
        if let Some(body) = &function.body {
            walk_region_ops(body, &mut |op| {
                if let Op::GetGlobal(op) = op
                    && is_cir_function_pointer_type(&op.addr_ty)
                    && let Attr::SymbolRef(name) = &op.name
                {
                    targets.insert(name.trim_start_matches('@').trim_matches('"').to_string());
                }
                true
            });
        }
    }
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
                    || (emit_pub && typed_function_is_exported(function)))
        })
        || module.functions.iter().any(|function| {
            let mut requires_native = false;
            if let Some(body) = &function.body {
                walk_region_ops(body, &mut |op| {
                    if let Op::Call(call) = op
                        && let Some(Attr::SymbolRef(callee)) = call.callee.as_ref()
                        && !defined_functions
                            .contains(callee.trim_start_matches('@').trim_matches('"'))
                        && call
                            .arg_types
                            .iter()
                            .any(|ty| is_cir_va_list_type(ty, aliases))
                    {
                        requires_native = true;
                    }
                    !requires_native
                });
            }
            requires_native
        })
}
