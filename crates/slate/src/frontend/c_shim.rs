use crate::backend::rust_ast::{ExternDecl, ExternFnDecl, Item, Prim, Program, RecordDef, Type};
use crate::frontend::function_abi::repair_function_signature;
use crate::function_identity::FunctionIdentity;
use std::collections::BTreeSet;

fn c_type_for_tag(tag: &str) -> String {
    if let Some(inner) = tag.strip_prefix('p') {
        return match inner {
            "i8" | "u8" => "char *".to_string(),
            "x" => "void *".to_string(),
            "f80" => "long double *".to_string(),
            other => format!("{} *", c_type_for_tag(other)),
        };
    }
    match tag {
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
        _ => "void *",
    }
    .to_string()
}

fn c_type_for_rust_type(ty: &Type, bridge: bool) -> String {
    match ty {
        Type::Prim(Prim::I8) => "signed char".into(),
        Type::Prim(Prim::U8) => "unsigned char".into(),
        Type::Prim(Prim::I16) => "short".into(),
        Type::Prim(Prim::U16) => "unsigned short".into(),
        Type::Prim(Prim::I32) => "int".into(),
        Type::Prim(Prim::U32) => "unsigned int".into(),
        Type::Prim(Prim::I64) => "long long".into(),
        Type::Prim(Prim::U64) => "unsigned long long".into(),
        Type::Prim(Prim::Isize) => "long".into(),
        Type::Prim(Prim::Usize) => "size_t".into(),
        Type::Prim(Prim::F32) => "float".into(),
        Type::Prim(Prim::F64) => "double".into(),
        Type::Prim(Prim::Bool) => "_Bool".into(),
        Type::CLib(clib) => clib.c_name().into(),
        Type::LongDouble if bridge => "__slate_f80".into(),
        Type::LongDouble => "long double".into(),
        Type::Complex(inner) if matches!(inner.as_ref(), Type::LongDouble) && bridge => {
            "__slate_cf80".into()
        }
        Type::Complex(inner) if matches!(inner.as_ref(), Type::LongDouble) => {
            "long double _Complex".into()
        }
        Type::Complex(inner) if matches!(inner.as_ref(), Type::Prim(Prim::F32)) => {
            "float _Complex".into()
        }
        Type::Complex(inner) if matches!(inner.as_ref(), Type::Prim(Prim::F64)) => {
            "double _Complex".into()
        }
        Type::Ptr { mutable, inner } => {
            let pointee = match inner.as_ref() {
                Type::CLib(clib) if *clib == crate::backend::rust_ast::CLibType::VOID => "void",
                Type::Prim(Prim::I8) => "char",
                Type::Prim(Prim::U8) => "unsigned char",
                Type::LongDouble => "long double",
                other => return format!("{} *", c_type_for_rust_type(other, false)),
            };
            if *mutable {
                format!("{pointee} *")
            } else {
                format!("const {pointee} *")
            }
        }
        Type::FnPtr { params, ret, .. } => {
            let ret = c_type_for_rust_type(ret, false);
            let params = params
                .iter()
                .map(|param| c_type_for_rust_type(param, false))
                .collect::<Vec<_>>()
                .join(", ");
            format!("{ret} (*)({params})")
        }
        Type::Custom(name) => format!("struct {name}"),
        Type::Unit => "void".into(),
        _ => "void *".into(),
    }
}

fn render_typed_shim(shim: &ExternFnDecl) -> Option<String> {
    let callback = shim.name.strip_prefix("__slate_ld_").map(str::to_string);
    let (callee, signature_bridge) = if let Some(original) = &callback {
        (original.clone(), false)
    } else {
        let rest = shim.name.strip_prefix("__slate_")?;
        (rest.split("__").next()?.to_string(), true)
    };
    let ret_ty = shim.ret.as_ref();
    let ret_c = ret_ty
        .map(|ty| c_type_for_rust_type(ty, signature_bridge))
        .unwrap_or_else(|| "void".into());
    let params = shim
        .params
        .iter()
        .enumerate()
        .map(|(i, param)| match &param.ty {
            Type::FnPtr { params, ret, .. } => {
                let ret = c_type_for_rust_type(ret, false);
                let params = params
                    .iter()
                    .map(|param| c_type_for_rust_type(param, false))
                    .collect::<Vec<_>>()
                    .join(", ");
                format!("{ret} (*_{i})({params})")
            }
            ty => format!("{} _{i}", c_type_for_rust_type(ty, signature_bridge)),
        })
        .collect::<Vec<_>>()
        .join(", ");
    let args = shim
        .params
        .iter()
        .enumerate()
        .map(|(i, param)| match &param.ty {
            Type::LongDouble if callback.is_some() => format!("__slate_f80_store(_{i})"),
            Type::LongDouble => format!("__slate_f80_load(_{i})"),
            Type::Complex(inner) if matches!(inner.as_ref(), Type::LongDouble) => {
                format!("__slate_cf80_load(_{i})")
            }
            _ => format!("_{i}"),
        })
        .collect::<Vec<_>>()
        .join(", ");
    let call = format!("{callee}({args})");
    let body = if ret_ty.is_none() || ret_ty.is_some_and(Type::is_unit) {
        format!("{call};")
    } else if matches!(
        ret_ty,
        Some(Type::Complex(inner)) if matches!(inner.as_ref(), Type::LongDouble)
    ) {
        format!("return __slate_cf80_store({call});")
    } else if callback.is_some() && matches!(ret_ty, Some(Type::LongDouble)) {
        format!("return __slate_f80_load({call});")
    } else if matches!(ret_ty, Some(Type::LongDouble)) {
        format!("return __slate_f80_store({call});")
    } else {
        format!("return {call};")
    };
    let prototype = if let Some(original) = &callback {
        let rust_ret = ret_ty
            .map(|ty| c_type_for_rust_type(ty, true))
            .unwrap_or_else(|| "void".into());
        let rust_params = shim
            .params
            .iter()
            .map(|param| c_type_for_rust_type(&param.ty, true))
            .collect::<Vec<_>>()
            .join(", ");
        format!("{rust_ret} {original}({rust_params});\n")
    } else {
        if shim.identity != FunctionIdentity::Unknown {
            String::new()
        } else if let Some(declared_type) = shim.declared_type.as_deref() {
            render_declared_prototype(&callee, declared_type).unwrap_or_default()
        } else {
            let mut prototype_params = shim
                .params
                .iter()
                .map(|param| param.ty.clone())
                .collect::<Vec<_>>();
            let mut prototype_ret = shim.ret.clone();
            repair_function_signature(
                shim.declared_type.as_deref(),
                &mut prototype_params,
                &mut prototype_ret,
            );
            let c_ret = prototype_ret
                .as_ref()
                .map(|ty| c_type_for_rust_type(ty, false))
                .unwrap_or_else(|| "void".into());
            let c_params = prototype_params
                .iter()
                .map(|param| c_type_for_rust_type(param, false))
                .collect::<Vec<_>>()
                .join(", ");
            format!("{c_ret} {callee}({c_params});\n")
        }
    };
    Some(format!(
        "{prototype}{ret_c} {}({params}) {{\n    {body}\n}}\n",
        shim.name
    ))
}

fn render_declared_prototype(name: &str, declared_type: &str) -> Option<String> {
    let open = declared_type.find('(')?;
    Some(format!(
        "{} {name}{};\n",
        declared_type[..open].trim(),
        &declared_type[open..]
    ))
}

fn render_trampoline(name: &str) -> Option<String> {
    let rest = name.strip_prefix("__slate_")?;
    let sep = rest.find("__")?;
    let callee = &rest[..sep];
    let callback_callee = callee.strip_prefix("cb_");
    let tags: Vec<&str> = rest[sep + 2..].split('_').collect();
    let (ret_tag, arg_tags) = match tags.first() {
        Some(tag) if tag.starts_with('r') && tag.len() > 1 => (&tag[1..], &tags[1..]),
        _ => ("i32", &tags[..]),
    };
    let ret_c_type = if callback_callee.is_some() && ret_tag == "f80" {
        "long double".to_string()
    } else {
        c_type_for_tag(ret_tag)
    };
    let params = arg_tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            let ty = if callback_callee.is_some() && *tag == "f80" {
                "long double".to_string()
            } else {
                c_type_for_tag(tag)
            };
            format!("{ty} _{i}")
        })
        .collect::<Vec<_>>()
        .join(", ");
    let args = arg_tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            if callback_callee.is_some() && *tag == "f80" {
                format!("__slate_f80_store(_{i})")
            } else if *tag == "cf80" {
                format!("__slate_cf80_load(_{i})")
            } else if *tag == "pcf80" {
                format!("__slate_cf80_load(*_{i})")
            } else if *tag == "f80" {
                format!("__slate_f80_load(_{i})")
            } else if *tag == "pf80" {
                format!("_{i}")
            } else if *tag == "ld" {
                format!("(long double)_{i}")
            } else {
                format!("_{i}")
            }
        })
        .collect::<Vec<_>>()
        .join(", ");
    let call = format!("{}({args})", callback_callee.unwrap_or(callee));
    let body = if ret_tag == "v" {
        format!("{call};")
    } else if ret_tag == "cf80" {
        format!("return __slate_cf80_store({call});")
    } else if callback_callee.is_some() && ret_tag == "f80" {
        format!("return __slate_f80_load({call});")
    } else if ret_tag == "f80" {
        format!("return __slate_f80_store({call});")
    } else {
        format!("return {call};")
    };
    let prototype = if callback_callee.is_some() {
        let rust_ret = if ret_tag == "f80" {
            "__slate_f80".to_string()
        } else {
            c_type_for_tag(ret_tag)
        };
        let rust_params = arg_tags
            .iter()
            .map(|tag| {
                if *tag == "f80" {
                    "__slate_f80".to_string()
                } else {
                    c_type_for_tag(tag)
                }
            })
            .collect::<Vec<_>>()
            .join(", ");
        format!(
            "{rust_ret} {}({rust_params});\n",
            callback_callee.unwrap_or(callee)
        )
    } else {
        String::new()
    };
    Some(format!(
        "{prototype}{ret_c_type} {name}({params}) {{\n    {body}\n}}\n"
    ))
}

const F80_SHIMS: &str = include_str!("./shims/long_double.c");
const FENV_SHIMS: &str = include_str!("./shims/fenv.c");

fn shim_preamble(shims: &[ExternFnDecl]) -> String {
    let headers = shims
        .iter()
        .filter_map(|shim| match shim.identity {
            FunctionIdentity::Known(known) => Some(known.header()),
            FunctionIdentity::Unknown => None,
        })
        .collect::<BTreeSet<_>>();
    std::iter::once("#define _GNU_SOURCE".to_string())
        .chain(
            headers
                .into_iter()
                .map(|header| format!("#include <{header}>")),
        )
        .collect::<Vec<_>>()
        .join("\n")
}

pub fn render_shim_c_source(shims: &[ExternFnDecl]) -> String {
    let mut blocks = vec![shim_preamble(shims)];
    let has_f80 = shims
        .iter()
        .any(|shim| shim.name.contains("f80") || shim.name.starts_with("__slate_ld_"));
    if has_f80 {
        blocks.push(F80_SHIMS.to_string());
    }
    if shims
        .iter()
        .any(|shim| shim.name.starts_with("__slate_fenv_"))
    {
        blocks.push(FENV_SHIMS.to_string());
    }
    for shim in shims {
        if shim.name.starts_with("__slate_f80_")
            || shim.name.starts_with("__slate_cf80_")
            || shim.name.starts_with("__slate_fenv_")
        {
            continue;
        } else if let Some(source) = render_typed_shim(shim) {
            blocks.push(source);
        }
    }
    blocks.join("\n")
}

pub fn render_shim_c_source_for_program(program: &Program) -> String {
    let shims: Vec<ExternFnDecl> = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls),
            _ => None,
        })
        .flatten()
        .filter_map(|decl| match decl {
            ExternDecl::Fn(shim) => Some(shim.clone()),
            ExternDecl::Static { .. } => None,
        })
        .collect();
    let mut blocks = vec![shim_preamble(&shims)];
    let has_f80 = shims
        .iter()
        .any(|shim| shim.name.contains("f80") || shim.name.starts_with("__slate_ld_"));
    if has_f80 {
        blocks.push(F80_SHIMS.to_string());
    }
    if shims
        .iter()
        .any(|shim| shim.name.starts_with("__slate_fenv_"))
    {
        blocks.push(FENV_SHIMS.to_string());
    }
    for record in long_double_records(program) {
        blocks.push(render_record_def(record));
    }
    for shim in &shims {
        if shim.name.starts_with("__slate_f80_")
            || shim.name.starts_with("__slate_cf80_")
            || shim.name.starts_with("__slate_fenv_")
        {
            continue;
        } else if let Some(source) = render_typed_shim(shim) {
            blocks.push(source);
        }
    }
    blocks.join("\n")
}

fn long_double_records(program: &Program) -> Vec<&RecordDef> {
    let mut remaining: Vec<&RecordDef> = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some(record),
            _ => None,
        })
        .filter(|record| {
            record
                .fields
                .iter()
                .any(|field| rust_type_has_long_double(&field.ty))
        })
        .collect();
    let record_names: BTreeSet<String> =
        remaining.iter().map(|record| record.name.clone()).collect();
    let mut emitted = BTreeSet::new();
    let mut ordered = Vec::new();
    while !remaining.is_empty() {
        let index = remaining
            .iter()
            .position(|record| {
                record.fields.iter().all(|field| {
                    !rust_type_has_unemitted_record(&field.ty, &record_names, &emitted)
                })
            })
            .unwrap_or(0);
        let record = remaining.remove(index);
        emitted.insert(record.name.clone());
        ordered.push(record);
    }
    ordered
}

fn render_record_def(record: &RecordDef) -> String {
    let keyword = if record.is_union { "union" } else { "struct" };
    let fields = record
        .fields
        .iter()
        .map(|field| {
            format!(
                "    {};",
                render_c_declaration(&field.ty, field.name.as_str(), false)
            )
        })
        .collect::<Vec<_>>()
        .join("\n");
    format!("{keyword} {} {{\n{fields}\n}};\n", record.name)
}

fn render_c_declaration(ty: &Type, name: &str, bridge: bool) -> String {
    match ty {
        Type::Array { elem, len } => render_c_declaration(elem, &format!("{name}[{len}]"), bridge),
        Type::FnPtr { params, ret, .. } => {
            let ret = c_type_for_rust_type(ret, false);
            let params = params
                .iter()
                .map(|param| c_type_for_rust_type(param, false))
                .collect::<Vec<_>>()
                .join(", ");
            format!("{ret} (*{name})({params})")
        }
        _ => format!("{} {name}", c_type_for_rust_type(ty, bridge)),
    }
}

fn rust_type_has_unemitted_record(
    ty: &Type,
    record_names: &BTreeSet<String>,
    emitted: &BTreeSet<String>,
) -> bool {
    match ty {
        Type::Custom(name) => record_names.contains(name) && !emitted.contains(name),
        Type::Array { elem, .. } => rust_type_has_unemitted_record(elem, record_names, emitted),
        _ => false,
    }
}

fn rust_type_has_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Complex(inner)
        | Type::Ref { inner, .. }
        | Type::Slice(inner)
        | Type::Ptr { inner, .. }
        | Type::Array { elem: inner, .. } => rust_type_has_long_double(inner),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(rust_type_has_long_double) || rust_type_has_long_double(ret)
        }
        Type::Generic { args, .. } => args.iter().any(rust_type_has_long_double),
        _ => false,
    }
}

pub fn render_shim_c_source_for_names(names: &BTreeSet<String>) -> String {
    let mut blocks = vec!["#define _GNU_SOURCE".to_string()];
    if names.iter().any(|name| name.contains("f80")) {
        blocks.push(F80_SHIMS.to_string());
    }
    if names.iter().any(|name| name.starts_with("__slate_fenv_")) {
        blocks.push(FENV_SHIMS.to_string());
    }
    for name in names {
        if name.starts_with("__slate_fenv_") {
            continue;
        } else if let Some(trampoline) = render_trampoline(name) {
            blocks.push(trampoline);
        }
    }
    blocks.join("\n")
}
