use slate_parser::ir::{self, Type, TypeDefinitionKind};
use slate_parser::sema::ctype::{CTypeKind, QualType};

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Kind {
    Function,
    Global,
    Type,
    Constant,
}

pub struct Symbol {
    pub name: &'static str,
    pub targets: &'static [&'static str],
    pub kind: Kind,
}

include!("libc_symbols_table.rs");

pub fn lookup(name: &str, target: &str, kind: Kind) -> Option<&'static Symbol> {
    SYMBOLS.iter().find(|symbol| {
        symbol.name == name && symbol.kind == kind && symbol.targets.contains(&target)
    })
}

pub fn shape(
    ty: &Type,
    definitions: &[slate_parser::ast::Span<ir::TypeDefinition>],
    ctypes: &ir::CTypeTable,
    c: Option<QualType>,
    record: &impl Fn(&str) -> Option<String>,
    pointer_width: u32,
) -> Option<String> {
    fn inner(
        ty: &Type,
        definitions: &[slate_parser::ast::Span<ir::TypeDefinition>],
        ctypes: &ir::CTypeTable,
        mut c: Option<QualType>,
        record: &impl Fn(&str) -> Option<String>,
        pointer_width: u32,
        depth: usize,
    ) -> Option<String> {
        if depth > 32 {
            return None;
        }
        let recurse =
            |ty: &Type, c| inner(ty, definitions, ctypes, c, record, pointer_width, depth + 1);
        while let Some(qual) = c {
            match ctypes.types.kind(qual.ty) {
                CTypeKind::Typedef {
                    name, underlying, ..
                } => {
                    let pointer_integer = match name.as_str() {
                        "size_t" | "uintptr_t" => Some("usize"),
                        "ssize_t" | "__ssize_t" | "intptr_t" | "ptrdiff_t" => Some("isize"),
                        _ => None,
                    };
                    if let Some(name) = pointer_integer
                        && matches!(ty, Type::Numeric(ir::NumericType::Integer { width, signed, bit_precise: false })
                            if *width == pointer_width && *signed == (name == "isize"))
                    {
                        return Some(name.into());
                    }
                    c = Some(*underlying);
                }
                CTypeKind::TypeOf { underlying, .. } => c = Some(*underlying),
                CTypeKind::Char => return Some("core::ffi::c_char".into()),
                _ => break,
            }
        }
        Some(match ty {
            Type::Void => "core::ffi::c_void".into(),
            Type::Bool => "bool".into(),
            Type::Numeric(ir::NumericType::Integer {
                width,
                signed,
                bit_precise: false,
            }) if matches!(width, 8 | 16 | 32 | 64 | 128) => {
                format!("{}{width}", if *signed { 'i' } else { 'u' })
            }
            Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => "f32".into(),
            Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => "f64".into(),
            Type::Defined(id) => {
                let definition = definitions
                    .iter()
                    .find(|definition| definition.value.id == *id)?;
                match &definition.kind {
                    TypeDefinitionKind::Alias(ty)
                    | TypeDefinitionKind::Enum {
                        underlying: Some(ty),
                        ..
                    } => recurse(ty, c)?,
                    TypeDefinitionKind::Record { .. } => {
                        format!("libc::{}", record(definition.name.as_deref()?)?)
                    }
                    _ => return None,
                }
            }
            Type::Pointer {
                pointee,
                is_const,
                access,
                space: ir::PointerSpace::Default,
            } if access.is_plain() => {
                let c = c.and_then(|c| match ctypes.types.kind(c.ty) {
                    CTypeKind::Pointer(pointee, _) => Some(*pointee),
                    _ => None,
                });
                if matches!(**pointee, Type::Function { .. }) {
                    format!("Option<{}>", recurse(pointee, c)?)
                } else {
                    format!(
                        "*{} {}",
                        if *is_const { "const" } else { "mut" },
                        recurse(pointee, c)?
                    )
                }
            }
            Type::Array {
                element,
                length: Some(length),
            } => format!("[{}; {length}]", recurse(element, None)?),
            Type::Function {
                parameters,
                return_type,
                variadic,
                prototyped: true,
                convention: ir::CallConv::C,
            } => {
                let (cparams, cret) = c
                    .and_then(|c| match ctypes.types.kind(c.ty) {
                        CTypeKind::Function { params, ret, .. } => Some((params.as_slice(), *ret)),
                        _ => None,
                    })
                    .map_or((&[][..], None), |(params, ret)| (params, Some(ret)));
                let mut params = parameters
                    .iter()
                    .enumerate()
                    .map(|(index, ty)| recurse(ty, cparams.get(index).copied()))
                    .collect::<Option<Vec<_>>>()?;
                if *variadic {
                    params.push("...".into());
                }
                let ret = match return_type {
                    Some(ty) if !matches!(**ty, Type::Void) => recurse(ty, cret)?,
                    _ => "()".into(),
                };
                format!("unsafe extern \"C\" fn({}) -> {ret}", params.join(", "))
            }
            _ => return None,
        })
    }
    inner(ty, definitions, ctypes, c, record, pointer_width, 0)
}

pub fn function_type(function: &ir::Function) -> Option<Type> {
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return None;
    };
    if function.symbol.weak
        || function.symbol.alias.is_some()
        || function.symbol.weakref.is_some()
        || function.symbol.ifunc.is_some()
        || function.symbol.fallback.is_some()
        || !function.semantics.target.is_empty()
    {
        return None;
    }
    Some(Type::Function {
        parameters: fixed.iter().map(|param| param.ty.clone()).collect(),
        return_type: function.return_type.clone().map(Box::new),
        variadic: *variadic,
        prototyped: true,
        convention: function
            .abi
            .as_ref()
            .map_or(ir::CallConv::C, |abi| abi.calling),
    })
}

pub fn link_name<'a>(name: &'a str, symbol: &'a ir::SymbolAttributes) -> &'a str {
    symbol
        .asm_name
        .as_deref()
        .unwrap_or(name)
        .trim_start_matches('\u{1}')
}

pub fn parameters(shape: &str) -> Vec<&str> {
    let Some((_, rest)) = shape.split_once(" fn(") else {
        return Vec::new();
    };
    let mut depth = 0;
    let mut start = 0;
    let mut params = Vec::new();
    for (index, c) in rest.char_indices() {
        match c {
            '(' | '<' | '[' => depth += 1,
            ')' if depth == 0 => {
                if index > start {
                    params.push(rest[start..index].trim());
                }
                break;
            }
            ')' | '>' | ']' if !(c == '>' && rest[..index].ends_with('-')) => depth -= 1,
            ',' if depth == 0 => {
                params.push(rest[start..index].trim());
                start = index + 1;
            }
            _ => {}
        }
    }
    params
}

pub fn layout_key<'a>(
    size: u64,
    align: u64,
    fields: impl IntoIterator<Item = (&'a str, u64, bool)>,
) -> u64 {
    let mut hash = 0xcbf29ce484222325u64;
    let mut feed = |bytes: &[u8]| {
        for byte in bytes {
            hash = (hash ^ u64::from(*byte)).wrapping_mul(0x100000001b3);
        }
    };
    feed(&size.to_le_bytes());
    feed(&align.to_le_bytes());
    for (name, offset, bits) in fields {
        feed(&(name.len() as u64).to_le_bytes());
        feed(name.as_bytes());
        feed(&offset.to_le_bytes());
        feed(&[u8::from(bits)]);
    }
    hash
}

pub fn record_key(definition: &ir::TypeDefinition) -> Option<u64> {
    let TypeDefinitionKind::Record { fields, layout, .. } = &definition.kind else {
        return None;
    };
    match (fields, layout) {
        (Some(fields), Some(layout)) => {
            let fields = fields
                .iter()
                .zip(&layout.offsets)
                .map(|(field, offset)| {
                    (
                        format!(
                            "{}:{}",
                            field.name.as_deref().unwrap_or("<anonymous>"),
                            field_type_key(&field.ty.to_string())
                        ),
                        *offset,
                        field.bit_width.is_some(),
                    )
                })
                .collect::<Vec<_>>();
            Some(layout_key(
                layout.size,
                layout.align,
                fields
                    .iter()
                    .map(|(name, offset, bits)| (name.as_str(), *offset, *bits)),
            ))
        }
        (None, None) => Some(layout_key(0, 1, [])),
        _ => None,
    }
}

pub fn record_path(definition: &ir::TypeDefinition, target: &str) -> Option<&'static str> {
    let name = definition.name.as_deref()?;
    crate::backend::rust_ast::CLIB_RECORD_TYPES
        .iter()
        .find(|record| {
            record
                .aliases
                .iter()
                .any(|alias| alias.names.contains(&name) && alias.targets.contains(&target))
        })
        .map(|record| record.rust_path)
}

pub fn field_type_key(mut ty: &str) -> String {
    let mut out = String::new();
    while let Some(at) = ty.find("@type") {
        out.push_str(&ty[..at + 5]);
        ty = ty[at + 5..].trim_start_matches(|c: char| c.is_ascii_digit());
    }
    out.push_str(ty);
    out.retain(|c| !c.is_whitespace());
    out
}
