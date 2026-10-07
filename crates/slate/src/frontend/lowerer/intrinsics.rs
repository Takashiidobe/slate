use super::*;
use intrinsics_table::{IntrinsicSignature, StdarchOverride};
use slate_parser::target_info::TargetFamily;
use std::hash::{Hash, Hasher};

pub(super) fn builtin_intrinsic(
    module: &ir::Module,
    function: &Span<ir::Function>,
) -> Option<&'static IntrinsicSignature> {
    if function.body.is_some()
        || !module
            .metadata
            .get(&function.id)
            .is_some_and(|entries| entries.iter().any(|(key, _)| key == "c_builtin"))
    {
        return None;
    }
    let explicit = match function.name.as_str() {
        "__builtin___clear_cache" => Some("llvm.clear_cache"),
        "__builtin_fabs" | "__builtin_fabsf" | "__builtin_fabsl" => Some("llvm.fabs"),
        "__builtin_copysign" | "__builtin_copysignf" | "__builtin_copysignl" => {
            Some("llvm.copysign")
        }
        "__builtin_bswap16" | "__builtin_bswap32" | "__builtin_bswap64" => Some("llvm.bswap"),
        "__builtin_frame_address" => Some("llvm.frameaddress"),
        "__builtin_return_address" => Some("llvm.returnaddress"),
        _ => None,
    };
    catalog(module.target.family).find(|intrinsic| {
        intrinsic.builtins.contains(&function.name.as_str()) || explicit == Some(intrinsic.name)
    })
}

pub(super) fn named_intrinsic(
    family: TargetFamily,
    name: &str,
) -> Option<&'static IntrinsicSignature> {
    catalog(family).find(|intrinsic| intrinsic.name == name)
}

fn catalog(family: TargetFamily) -> impl Iterator<Item = &'static IntrinsicSignature> {
    let target_intrinsics = match family {
        TargetFamily::X86 | TargetFamily::X86_64 => intrinsics_table::X86_INTRINSICS,
        TargetFamily::AArch64 => intrinsics_table::AARCH64_INTRINSICS,
        TargetFamily::Arm32 => intrinsics_table::ARM_INTRINSICS,
    };
    target_intrinsics
        .iter()
        .chain(intrinsics_table::GENERAL_INTRINSICS)
}

pub(super) fn simd_type(element: rust::Type, lanes: u64) -> Option<rust::Type> {
    if !matches!(
        element,
        rust::Type::Prim(
            Prim::I8
                | Prim::U8
                | Prim::I16
                | Prim::U16
                | Prim::I32
                | Prim::U32
                | Prim::I64
                | Prim::U64
                | Prim::F32
                | Prim::F64
        )
    ) || !matches!(lanes, 1 | 2 | 4 | 8 | 16 | 32 | 64)
    {
        return None;
    }
    Some(rust::Type::Generic {
        name: "std::simd::Simd".into(),
        args: vec![element, rust::Type::Custom(lanes.to_string())],
    })
}

fn simd_parts(ty: &rust::Type) -> Option<(&rust::Type, u64)> {
    let rust::Type::Generic { name, args } = ty else {
        return None;
    };
    let [element, rust::Type::Custom(lanes)] = args.as_slice() else {
        return None;
    };
    (name == "std::simd::Simd").then_some((element, lanes.parse().ok()?))
}

fn scalar_type(name: &str) -> Option<rust::Type> {
    let primitive = match name {
        "void" => return Some(rust::Type::Unit),
        "ptr" => {
            return Some(rust::Type::Ptr {
                mutable: true,
                inner: Box::new(rust::Type::Unit),
            });
        }
        "i1" | "bool" => Prim::Bool,
        "i8" => Prim::I8,
        "u8" => Prim::U8,
        "i16" => Prim::I16,
        "u16" => Prim::U16,
        "i32" => Prim::I32,
        "u32" => Prim::U32,
        "i64" => Prim::I64,
        "u64" => Prim::U64,
        "i128" => Prim::I128,
        "u128" => Prim::U128,
        "float" | "f32" => Prim::F32,
        "double" | "f64" => Prim::F64,
        _ => return None,
    };
    Some(rust::Type::Prim(primitive))
}

fn mangle_llvm_type(ty: &rust::Type) -> Option<String> {
    if let Some((element, lanes)) = simd_parts(ty) {
        return Some(format!("v{lanes}{}", mangle_llvm_type(element)?));
    }
    Some(
        match ty {
            rust::Type::Prim(Prim::Bool) => "i1",
            rust::Type::Prim(Prim::I8 | Prim::U8) => "i8",
            rust::Type::Prim(Prim::I16 | Prim::U16) => "i16",
            rust::Type::Prim(Prim::I32 | Prim::U32) => "i32",
            rust::Type::Prim(Prim::I64 | Prim::U64) => "i64",
            rust::Type::Prim(Prim::I128 | Prim::U128) => "i128",
            rust::Type::Prim(Prim::F32) => "f32",
            rust::Type::Prim(Prim::F64) => "f64",
            rust::Type::Ptr { .. } => "p0",
            _ => return None,
        }
        .into(),
    )
}

fn resolve_llvm_type(name: &str, overloads: &[rust::Type]) -> Option<rust::Type> {
    if let Some(rest) = name.strip_prefix("overload:") {
        let mut parts = rest.split(':');
        let index = parts.next()?.parse::<usize>().ok()?;
        let vector_constraint = parts.next()?;
        let element_constraint = parts.next()?;
        let ty = overloads.get(index)?;
        let vector = simd_parts(ty);
        let element = vector.map_or(ty, |(element, _)| element);
        if (vector_constraint == "vector" && vector.is_none())
            || (vector_constraint == "scalar" && vector.is_some())
            || match element_constraint {
                "any" => false,
                "int" => !mangle_llvm_type(element)?.starts_with('i'),
                "float" => !matches!(element, rust::Type::Prim(Prim::F32 | Prim::F64)),
                "ptr" => !matches!(element, rust::Type::Ptr { .. }),
                _ => true,
            }
        {
            return None;
        }
        return Some(ty.clone());
    }
    if let Some(index) = name.strip_prefix("match:") {
        return overloads.get(index.parse::<usize>().ok()?).cloned();
    }
    if let Some(index) = name.strip_prefix("element:") {
        let (element, _) = simd_parts(overloads.get(index.parse::<usize>().ok()?)?)?;
        return Some(element.clone());
    }
    if let Some(vector) = name.strip_prefix('<').and_then(|s| s.strip_suffix('>')) {
        let (lanes, element) = vector.split_once(" x ")?;
        return simd_type(resolve_llvm_type(element, overloads)?, lanes.parse().ok()?);
    }
    scalar_type(name)
}

fn stdarch_type(name: &str) -> Option<rust::Type> {
    let name = name.trim();
    if name.starts_with("*const ") || name.starts_with("*mut ") {
        return Some(rust::Type::Ptr {
            mutable: name.starts_with("*mut "),
            inner: Box::new(rust::Type::Unit),
        });
    }
    if let Some((element, lanes)) = name.split_once('x') {
        return simd_type(scalar_type(element)?, lanes.parse().ok()?);
    }
    scalar_type(name)
}

fn find_stdarch_override(
    intrinsic: &IntrinsicSignature,
    types: &[rust::Type],
) -> Option<&'static StdarchOverride> {
    let family = format!("{}.", intrinsic.name);
    let mut matches = intrinsics_table::X86_STDARCH_OVERRIDES
        .iter()
        .filter(|entry| {
            (entry.link_name == intrinsic.name || entry.link_name.starts_with(&family))
                && entry.params.len() + 1 == types.len()
                && std::iter::once(entry.ret.unwrap_or("void"))
                    .chain(entry.params.iter().copied())
                    .zip(types)
                    .all(|(mined, ty)| {
                        stdarch_type(mined).is_some_and(|mined| {
                            (mined == rust::Type::Unit && *ty == rust::Type::Unit)
                                || mangle_llvm_type(&mined)
                                    .zip(mangle_llvm_type(ty))
                                    .is_some_and(|(a, b)| a == b)
                        })
                    })
        });
    let only = matches.next()?;
    matches.next().is_none().then_some(only)
}

fn compatible_type(from: &rust::Type, to: &rust::Type) -> bool {
    from == to
        || mangle_llvm_type(from)
            .zip(mangle_llvm_type(to))
            .is_some_and(|(a, b)| a == b)
}

fn adapt_type(expr: Expr, from: &rust::Type, to: &rust::Type) -> Expr {
    if from == to {
        return expr;
    }
    if simd_parts(from).is_some() {
        Expr::Transmute {
            from: from.clone(),
            to: to.clone(),
            expr: Box::new(expr),
        }
    } else {
        Expr::Cast {
            expr: Box::new(expr),
            ty: to.clone(),
        }
    }
}

fn constant_argument(value: &ir::Value) -> bool {
    match &value.node.value {
        ValueKind::Constant(_) => true,
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert { operand, .. }
        | ValueKind::Unary { operand, .. } => constant_argument(operand),
        ValueKind::Arith { left, right, .. }
        | ValueKind::Compare { left, right, .. }
        | ValueKind::Logical { left, right, .. } => {
            constant_argument(left) && constant_argument(right)
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            constant_argument(condition)
                && constant_argument(then_value)
                && constant_argument(else_value)
        }
        _ => false,
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_intrinsic(
        &mut self,
        label: &str,
        intrinsic: &IntrinsicSignature,
        arguments: &[ir::Value],
        return_type: &ir::Type,
    ) -> Result<Expr> {
        let unsupported = || Construct::Function {
            name: label.to_owned(),
            detail: format!(
                "intrinsic {} requires unsupported signature adaptation",
                intrinsic.name
            ),
        };
        let params = intrinsic.params.ok_or_else(unsupported)?;
        let ret = intrinsic.ret.ok_or_else(unsupported)?;
        if params.len() != arguments.len() || std::iter::once(return_type).chain(arguments.iter().map(|arg| &arg.ty))
            .any(|ty| matches!(self.tables.resolve_type(ty), ir::Type::Pointer { space, .. } if *space != ir::PointerSpace::Default))
        { return Err(unsupported().into()); }
        let mut types = vec![self.lower_type(return_type)?];
        for argument in arguments {
            types.push(self.lower_type(&argument.ty)?);
        }
        if let Some(shim) = long_double_intrinsic_shim(
            intrinsic
                .name
                .strip_prefix("llvm.")
                .unwrap_or(intrinsic.name),
            &types[1..],
            (types[0] != rust::Type::Unit).then_some(&types[0]),
        ) {
            return Ok(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var(shim.into())),
                    args: arguments
                        .iter()
                        .map(|argument| self.lower_value(argument))
                        .collect::<Result<Vec<_>>>()?,
                    binding: CallBinding::Generated,
                })),
            })));
        }
        let overloads: Vec<_> = intrinsic
            .overloaded_positions
            .unwrap_or_default()
            .iter()
            .map(|&position| {
                types
                    .get(position as usize)
                    .cloned()
                    .ok_or_else(unsupported)
            })
            .collect::<std::result::Result<_, _>>()?;
        if intrinsic.overloaded && intrinsic.overloaded_positions.is_none() {
            return Err(unsupported().into());
        }
        let stdarch_override = find_stdarch_override(intrinsic, &types);
        let shim_types = if let Some(entry) = stdarch_override {
            std::iter::once(entry.ret.unwrap_or("void"))
                .chain(entry.params.iter().copied())
                .map(|ty| stdarch_type(ty).ok_or_else(unsupported))
                .collect::<std::result::Result<Vec<_>, _>>()?
        } else {
            let mut types = vec![resolve_llvm_type(ret, &overloads).ok_or_else(unsupported)?];
            for param in params {
                types.push(resolve_llvm_type(param.llvm_type, &overloads).ok_or_else(unsupported)?);
            }
            types
        };
        if !types
            .iter()
            .zip(&shim_types)
            .all(|(source, expected)| compatible_type(source, expected))
        {
            return Err(unsupported().into());
        }
        let link_name = if intrinsic.name == "llvm.clear_cache" {
            intrinsic.name.to_owned()
        } else if let Some(entry) = stdarch_override {
            entry.link_name.to_owned()
        } else if overloads.is_empty() {
            intrinsic.name.to_owned()
        } else {
            let suffixes = overloads
                .iter()
                .map(|ty| mangle_llvm_type(ty).ok_or_else(unsupported))
                .collect::<std::result::Result<Vec<_>, _>>()?;
            format!("{}.{}", intrinsic.name, suffixes.join("."))
        };
        let mut args = Vec::new();
        let mut fixed = Vec::new();
        for (index, ((param, argument), ty)) in params
            .iter()
            .zip(arguments)
            .zip(&shim_types[1..])
            .enumerate()
        {
            if param.immarg && !constant_argument(argument) {
                return Err(unsupported().into());
            }
            let expr = self.lower_value(argument)?;
            let expr = adapt_type(expr, &types[index + 1], ty);
            args.push(if param.immarg {
                Expr::ConstBlock(Box::new(expr))
            } else {
                expr
            });
            fixed.push(FnParam {
                comments: Vec::new(),
                name: format!("arg{index}"),
                mutable: false,
                ty: ty.clone(),
            });
        }
        let mut hasher = std::collections::hash_map::DefaultHasher::new();
        (&link_name, &shim_types).hash(&mut hasher);
        let name = format!("__slate_intrinsic_{:x}", hasher.finish());
        self.dependencies
            .intrinsics
            .entry(name.clone())
            .or_insert_with(|| rust::ExternFnDecl {
                attrs: vec![Attr::LinkName(link_name)],
                name: name.clone(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                trusted_headers: Default::default(),
                params: fixed,
                variadic: false,
                ret: (shim_types[0] != rust::Type::Unit).then(|| shim_types[0].clone()),
                safe: false,
            });
        let call = Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(Expr::Var(name.into())),
                args,
                binding: CallBinding::Generated,
            })),
        }));
        Ok(adapt_type(call, &shim_types[0], &types[0]))
    }
}

fn long_double_intrinsic_shim(
    intrinsic_name: &str,
    param_types: &[rust::Type],
    ret_type: Option<&rust::Type>,
) -> Option<&'static str> {
    let all_long_double = |count: usize| {
        param_types.len() == count
            && param_types
                .iter()
                .all(|ty| matches!(ty, rust::Type::LongDouble))
    };
    match (intrinsic_name, ret_type, param_types) {
        (
            "nexttoward",
            Some(rust::Type::Prim(Prim::F128)),
            [rust::Type::Prim(Prim::F128), rust::Type::LongDouble],
        ) => Some("__slate_f128_nexttoward"),
        (_, Some(rust::Type::LongDouble), _) => match intrinsic_name {
            "powi" if param_types == [rust::Type::LongDouble, rust::Type::Prim(Prim::I32)] => {
                Some("__slate_f80_powi")
            }
            "copysign" if all_long_double(2) => Some("__slate_f80_copysign"),
            "fmax" if all_long_double(2) => Some("__slate_f80_fmax"),
            "fmin" if all_long_double(2) => Some("__slate_f80_fmin"),
            "fma" if all_long_double(3) => Some("__slate_f80_fma"),
            "fmod" if all_long_double(2) => Some("__slate_f80_fmod"),
            "remainder" if all_long_double(2) => Some("__slate_f80_remainder"),
            "pow" if all_long_double(2) => Some("__slate_f80_pow"),
            "fdim" if all_long_double(2) => Some("__slate_f80_fdim"),
            "hypot" if all_long_double(2) => Some("__slate_f80_hypot"),
            "abs" | "fabs" if all_long_double(1) => Some("__slate_f80_abs"),
            "ceil" if all_long_double(1) => Some("__slate_f80_ceil"),
            "floor" if all_long_double(1) => Some("__slate_f80_floor"),
            "sqrt" if all_long_double(1) => Some("__slate_f80_sqrt"),
            "cbrt" if all_long_double(1) => Some("__slate_f80_cbrt"),
            "exp" if all_long_double(1) => Some("__slate_f80_exp"),
            "exp2" if all_long_double(1) => Some("__slate_f80_exp2"),
            "expm1" if all_long_double(1) => Some("__slate_f80_expm1"),
            "log" if all_long_double(1) => Some("__slate_f80_log"),
            "log2" if all_long_double(1) => Some("__slate_f80_log2"),
            "log10" if all_long_double(1) => Some("__slate_f80_log10"),
            "log1p" if all_long_double(1) => Some("__slate_f80_log1p"),
            "sin" if all_long_double(1) => Some("__slate_f80_sin"),
            "cos" if all_long_double(1) => Some("__slate_f80_cos"),
            "tan" if all_long_double(1) => Some("__slate_f80_tan"),
            "asin" if all_long_double(1) => Some("__slate_f80_asin"),
            "acos" if all_long_double(1) => Some("__slate_f80_acos"),
            "atan" if all_long_double(1) => Some("__slate_f80_atan"),
            "sinh" if all_long_double(1) => Some("__slate_f80_sinh"),
            "cosh" if all_long_double(1) => Some("__slate_f80_cosh"),
            "tanh" if all_long_double(1) => Some("__slate_f80_tanh"),
            "asinh" if all_long_double(1) => Some("__slate_f80_asinh"),
            "acosh" if all_long_double(1) => Some("__slate_f80_acosh"),
            "atanh" if all_long_double(1) => Some("__slate_f80_atanh"),
            "nearbyint" if all_long_double(1) => Some("__slate_f80_nearbyint"),
            "round" if all_long_double(1) => Some("__slate_f80_round"),
            "trunc" if all_long_double(1) => Some("__slate_f80_trunc"),
            "rint" if all_long_double(1) => Some("__slate_f80_rint"),
            _ => None,
        },
        _ => None,
    }
}
