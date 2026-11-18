use crate::backend::build_script::Step;
use crate::backend::c_emit::{
    self as c, Expr, Field, Function, Param, Prototype, Stmt, Type, Unit,
};
use crate::backend::rust_ast::{Attr, Cfg, ExternDecl, Item, Program};
use std::collections::BTreeSet;

fn c_type_for_tag(tag: &str) -> Type {
    if let Some(inner) = tag.strip_prefix('p') {
        return match inner {
            "i8" | "u8" => Type::Named("char").pointer(),
            "x" => Type::Named("void").pointer(),
            "f80" => Type::Named("long double").pointer(),
            other => c_type_for_tag(other).pointer(),
        };
    }
    Type::Named(match tag {
        "i8" => "signed char",
        "u8" => "unsigned char",
        "i16" => "short",
        "u16" => "unsigned short",
        "i32" => "int",
        "u32" => "unsigned int",
        "i64" => "long long",
        "u64" => "unsigned long long",
        "isize" => "long",
        "usize" => "size_t",
        "f32" => "float",
        "f64" => "double",
        "bool" => "_Bool",
        "c" => "char",
        "v" => "void",
        "ld" => "double",
        "f80" => "__slate_f80",
        "cf80" => "__slate_cf80",
        "lq" => "long double",
        tag if memory_aggregate(tag).is_some() => return Type::Struct(format!("__slate_{tag}")),
        _ => return Type::Named("void").pointer(),
    })
}

fn split_tags(tags: &str) -> impl Iterator<Item = &str> {
    tags.split('_').map(|tag| match tag {
        "s80" => "f80",
        "rs80" => "rf80",
        tag => tag,
    })
}

fn memory_aggregate(tag: &str) -> Option<(u64, u64)> {
    let rest = tag.strip_prefix('m')?;
    let (size, align) = match rest.split_once('a') {
        Some((size, align)) => (size.parse().ok()?, align.parse().ok()?),
        None => (rest.parse().ok()?, 16),
    };
    (size > 0 && u64::is_power_of_two(align) && size % align == 0).then_some((size, align))
}

fn native_c_type(tag: &str) -> Type {
    match tag {
        "f80" | "ld" => Type::Named("long double"),
        "pf80" => Type::Named("long double").pointer(),
        "cf80" => Type::Named("_Complex long double"),
        "pcf80" => Type::Named("_Complex long double").pointer(),
        _ => c_type_for_tag(tag),
    }
}

fn arg(index: usize) -> Expr {
    Expr::name(format!("_{index}"))
}

fn named_params(types: impl IntoIterator<Item = Type>) -> impl Iterator<Item = Param> {
    types
        .into_iter()
        .enumerate()
        .map(|(index, ty)| Param::new(ty, format!("_{index}")))
}

fn function_pointer_param() -> Param {
    Param::new(Type::Named("void").pointer(), "_f")
}

fn call_through_f(ret: Type, params: Vec<Type>, variadic: bool, args: Vec<Expr>) -> Expr {
    let ty = Type::FnPtr {
        ret: Box::new(ret),
        params,
        variadic,
    };
    Expr::call_expr(Expr::cast(ty, Expr::name("_f")), args)
}

fn result(ret: &Type, call: Expr) -> Stmt {
    if ret.is_void() {
        Stmt::Expr(call)
    } else {
        Stmt::Return(call)
    }
}

fn render_variadic_trampoline(name: &str) -> Option<Vec<c::Item>> {
    let (fixed, variadic) = name.strip_prefix("__slate_vcall__")?.split_once("__")?;
    let mut fixed = split_tags(fixed);
    let ret = c_type_for_tag(fixed.next()?.strip_prefix('r')?);
    let fixed = fixed.map(c_type_for_tag).collect::<Vec<_>>();
    let variadic = split_tags(variadic).collect::<Vec<_>>();
    let params = std::iter::once(function_pointer_param())
        .chain(named_params(
            fixed
                .iter()
                .cloned()
                .chain(variadic.iter().map(|tag| c_type_for_tag(tag))),
        ))
        .collect();
    let args = (0..fixed.len())
        .map(arg)
        .chain(variadic.iter().enumerate().map(|(j, tag)| {
            let i = fixed.len() + j;
            match *tag {
                "f80" => Expr::call("__slate_f80_load", vec![arg(i)]),
                _ => arg(i),
            }
        }))
        .collect();
    let call = call_through_f(ret.clone(), fixed, true, args);
    Some(vec![c::Item::Function(Function {
        body: vec![result(&ret, call)],
        ret,
        name: name.to_string(),
        params,
        variadic: false,
    })])
}

fn render_indirect_trampoline(name: &str) -> Option<Vec<c::Item>> {
    let rest = name.strip_prefix("__slate_icall__")?;
    let (fixed, variadic) = match rest.split_once("__") {
        Some((fixed, variadic)) => (fixed, Some(variadic)),
        None => (rest, None),
    };
    let mut fixed = split_tags(fixed);
    let ret_tag = fixed.next()?.strip_prefix('r')?;
    let fixed = fixed.collect::<Vec<_>>();
    let native_params = fixed.iter().map(|tag| native_c_type(tag)).collect();
    let tags = fixed
        .into_iter()
        .chain(
            variadic
                .into_iter()
                .flat_map(split_tags)
                .filter(|tag| !tag.is_empty()),
        )
        .collect::<Vec<_>>();
    let params = std::iter::once(function_pointer_param())
        .chain(named_params(tags.iter().map(|tag| c_type_for_tag(tag))))
        .collect();
    let args = tags
        .iter()
        .enumerate()
        .map(|(i, tag)| match *tag {
            "f80" => Expr::call("__slate_f80_load", vec![arg(i)]),
            "cf80" => Expr::call("__slate_cf80_load", vec![arg(i)]),
            _ => arg(i),
        })
        .collect();
    let call = call_through_f(
        native_c_type(ret_tag),
        native_params,
        variadic.is_some(),
        args,
    );
    let body = match ret_tag {
        "v" => Stmt::Expr(call),
        "f80" => Stmt::Return(Expr::call("__slate_f80_store", vec![call])),
        "cf80" => Stmt::Return(Expr::call("__slate_cf80_store", vec![call])),
        _ => Stmt::Return(call),
    };
    Some(vec![c::Item::Function(Function {
        ret: c_type_for_tag(ret_tag),
        name: name.to_string(),
        params,
        variadic: false,
        body: vec![body],
    })])
}

fn render_variadic_export(name: &str) -> Option<Vec<c::Item>> {
    let (entry, tags) = name.strip_prefix("__slate_ldv__")?.rsplit_once("__")?;
    let mut tags = split_tags(tags);
    let ret_tag = tags.next()?.strip_prefix('r')?;
    let fixed = tags.collect::<Vec<_>>();
    let last = fixed.len().checked_sub(1)?;
    let rust_ret = c_type_for_tag(ret_tag);
    let prototype = Prototype {
        ret: rust_ret.clone(),
        name: name.to_string(),
        params: fixed
            .iter()
            .map(|tag| c_type_for_tag(tag))
            .chain([Type::Named("va_list")])
            .collect(),
        variadic: false,
        asm_label: None,
    };
    let args = fixed
        .iter()
        .enumerate()
        .map(|(i, tag)| match *tag {
            "f80" => Expr::call("__slate_f80_store", vec![arg(i)]),
            _ => arg(i),
        })
        .chain([Expr::name("_ap")])
        .collect();
    let call = Expr::call(name, args);
    let mut body = vec![
        Stmt::decl(Type::Named("va_list"), "_ap", None),
        Stmt::Expr(Expr::call("va_start", vec![Expr::name("_ap"), arg(last)])),
    ];
    let returned = match ret_tag {
        "v" => {
            body.push(Stmt::Expr(call));
            None
        }
        _ => {
            body.push(Stmt::decl(rust_ret, "_r", Some(call)));
            Some(match ret_tag {
                "f80" => Expr::call("__slate_f80_load", vec![Expr::name("_r")]),
                _ => Expr::name("_r"),
            })
        }
    };
    body.push(Stmt::Expr(Expr::call("va_end", vec![Expr::name("_ap")])));
    body.extend(returned.map(Stmt::Return));
    Some(vec![
        c::Item::Prototype(prototype),
        c::Item::Function(Function {
            ret: native_c_type(ret_tag),
            name: entry.to_string(),
            params: named_params(fixed.iter().map(|tag| native_c_type(tag))).collect(),
            variadic: true,
            body,
        }),
    ])
}

fn render_trampoline(name: &str) -> Option<Vec<c::Item>> {
    if name.starts_with("__slate_ldv__") {
        return render_variadic_export(name);
    }
    if name.starts_with("__slate_vcall__") {
        return render_variadic_trampoline(name);
    }
    if name.starts_with("__slate_icall__") {
        return render_indirect_trampoline(name);
    }
    if name.starts_with("__slate_ldfp_") {
        return None;
    }
    let exported = name
        .strip_prefix("__slate_ld__")
        .and_then(|rest| rest.rsplit_once("__"));
    let (callee, tags) = match exported {
        Some(split) => split,
        None => name.strip_prefix("__slate_")?.rsplit_once("__")?,
    };
    let exported_callee = exported.map(|(callee, _)| callee);
    let callback_callee = callee.strip_prefix("cb_").filter(|_| exported.is_none());
    let native_entry = callback_callee.is_some() || exported_callee.is_some();
    let tags: Vec<&str> = split_tags(tags).collect();
    let (ret_tag, arg_tags) = match tags.first() {
        Some(tag) if tag.starts_with('r') && tag.len() > 1 => (&tag[1..], &tags[1..]),
        _ => ("i32", &tags[..]),
    };
    let entry_type = |tag: &str| {
        if native_entry && matches!(tag, "f80" | "cf80") {
            native_c_type(tag)
        } else {
            c_type_for_tag(tag)
        }
    };
    let args = arg_tags
        .iter()
        .enumerate()
        .map(|(i, tag)| match *tag {
            "f80" if native_entry => Expr::call("__slate_f80_store", vec![arg(i)]),
            "cf80" if native_entry => Expr::call("__slate_cf80_store", vec![arg(i)]),
            "cf80" => Expr::call("__slate_cf80_load", vec![arg(i)]),
            "pcf80" => Expr::call("__slate_cf80_load", vec![Expr::deref(arg(i))]),
            "f80" => Expr::call("__slate_f80_load", vec![arg(i)]),
            "ld" => Expr::cast(Type::Named("long double"), arg(i)),
            _ => arg(i),
        })
        .collect();
    let undeclared = !native_entry && header_for_shim_name(name).is_none();
    let (entry, target) = match (exported_callee, callback_callee) {
        (Some(exported), _) => (exported.to_string(), name.to_string()),
        (None, Some(callback)) => (name.to_string(), callback.to_string()),
        (None, None) if undeclared => (name.to_string(), format!("__slate_extern_{callee}")),
        (None, None) => (name.to_string(), callee.to_string()),
    };
    let call = Expr::call(target.clone(), args);
    let body = match ret_tag {
        "v" => Stmt::Expr(call),
        "cf80" if native_entry => Stmt::Return(Expr::call("__slate_cf80_load", vec![call])),
        "cf80" => Stmt::Return(Expr::call("__slate_cf80_store", vec![call])),
        "f80" if native_entry => Stmt::Return(Expr::call("__slate_f80_load", vec![call])),
        "f80" => Stmt::Return(Expr::call("__slate_f80_store", vec![call])),
        _ => Stmt::Return(call),
    };
    let prototype = if native_entry {
        Some(Prototype {
            ret: c_type_for_tag(ret_tag),
            name: target,
            params: arg_tags.iter().map(|tag| c_type_for_tag(tag)).collect(),
            variadic: false,
            asm_label: None,
        })
    } else if undeclared {
        Some(Prototype {
            ret: native_c_type(ret_tag),
            name: target,
            params: arg_tags.iter().map(|tag| native_c_type(tag)).collect(),
            variadic: false,
            asm_label: Some(callee.to_string()),
        })
    } else {
        None
    };
    let function = Function {
        ret: entry_type(ret_tag),
        name: entry,
        params: named_params(arg_tags.iter().map(|tag| entry_type(tag))).collect(),
        variadic: false,
        body: vec![body],
    };
    Some(
        prototype
            .map(c::Item::Prototype)
            .into_iter()
            .chain([c::Item::Function(function)])
            .collect(),
    )
}

pub(crate) const F80_SHIMS: &str = include_str!("./shims/long_double.c");
pub(crate) const FENV_SHIMS: &str = include_str!("./shims/fenv.c");

pub fn collect_program_shim_names(program: &Program) -> Vec<(String, Option<Cfg>)> {
    let mut names = Vec::new();
    collect_item_shim_names(&program.items, None, &mut names);
    names
}

fn collect_item_shim_names(
    items: &[Item],
    gate: Option<&Cfg>,
    names: &mut Vec<(String, Option<Cfg>)>,
) {
    for item in items {
        match item {
            Item::Cfg { cfg, item } => {
                let nested = match gate {
                    Some(outer) => Cfg::All(vec![outer.clone(), cfg.clone()]),
                    None => cfg.clone(),
                };
                collect_item_shim_names(std::slice::from_ref(item), Some(&nested), names);
            }
            Item::InlineMod { items, .. } => collect_item_shim_names(items, gate, names),
            Item::ExternBlock { abi, .. } if abi == "llvm-intrinsic" => {}
            Item::ExternBlock { decls, .. } => {
                names.extend(decls.iter().filter_map(|decl| match decl {
                    ExternDecl::Fn(shim) => Some((shim.name.clone(), gate.cloned())),
                    ExternDecl::Static { .. } => None,
                }))
            }
            Item::Fn(definition) => {
                names.extend(definition.attrs.iter().filter_map(|attr| match attr {
                    Attr::ExportName(name)
                        if name.starts_with("__slate_ld__")
                            || name.starts_with("__slate_ldv__") =>
                    {
                        Some((name.clone(), gate.cloned()))
                    }
                    _ => None,
                }))
            }
            _ => {}
        }
    }
}

pub fn shim_build_steps(gates: &[Option<Cfg>]) -> Vec<Step> {
    let rerun = Step::RerunIfChanged("src/slate_long_double.c".into());
    let compile = Step::CompileC {
        file: "src/slate_long_double.c".into(),
        library: "slate_long_double".into(),
    };
    if gates.iter().any(Option::is_none) {
        return vec![rerun, compile];
    }
    let mut unique: Vec<Cfg> = Vec::new();
    for gate in gates.iter().flatten() {
        if !unique.contains(gate) {
            unique.push(gate.clone());
        }
    }
    let cfg = match <[Cfg; 1]>::try_from(unique) {
        Ok([only]) => only,
        Err(unique) => Cfg::Any(unique),
    };
    vec![
        rerun,
        Step::When {
            cfg,
            steps: vec![compile],
        },
    ]
}

fn header_for_shim_name(name: &str) -> Option<&'static str> {
    let rest = name.strip_prefix("__slate_")?;
    let callee = rest.rsplit_once("__").map_or(rest, |(callee, _)| callee);
    let callee = callee.strip_prefix("cb_").unwrap_or(callee);
    crate::function_identity::Known::from_symbol(callee)
        .map(crate::function_identity::Known::header)
}

pub fn render_shim_c_source_for_names(names: &BTreeSet<String>) -> String {
    let headers = names
        .iter()
        .filter_map(|name| header_for_shim_name(name))
        .filter(|header| !header.starts_with("bits/"))
        .collect::<BTreeSet<_>>();
    let mut unit = Unit::default();
    unit.push(c::Item::Define("_GNU_SOURCE"));
    unit.extend(headers.into_iter().map(c::Item::Include));
    if names.iter().any(|name| {
        !name.starts_with("__slate_intrinsic_") && name.contains("f80")
            || name.contains("s80")
            || name.starts_with("__slate_f128_")
            || name.starts_with("__slate_ldv__")
    }) {
        unit.push(c::Item::Raw(F80_SHIMS));
    }
    if names.iter().any(|name| name.starts_with("__slate_fenv_")) {
        unit.push(c::Item::Raw(FENV_SHIMS));
    }
    let aggregates = names
        .iter()
        .flat_map(|name| name.split('_'))
        .map(|tag| tag.strip_prefix('r').unwrap_or(tag))
        .filter_map(|tag| memory_aggregate(tag).map(|layout| (tag, layout)))
        .collect::<BTreeSet<_>>();
    unit.extend(
        aggregates
            .into_iter()
            .map(|(tag, (size, align))| c::Item::Struct {
                tag: format!("__slate_{tag}"),
                fields: vec![Field {
                    align: Some(align),
                    ty: Type::Named("unsigned char"),
                    name: "v".into(),
                    len: Some(size),
                }],
            }),
    );
    unit.extend(
        names
            .iter()
            .filter(|name| !name.starts_with("__slate_fenv_"))
            .filter_map(|name| render_trampoline(name))
            .flatten(),
    );
    unit.to_string()
}
