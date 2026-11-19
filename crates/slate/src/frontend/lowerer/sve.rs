use super::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum SveImmediate {
    I32,
    Pattern,
    Prefetch,
}

#[derive(Debug)]
pub(super) struct SveIntrinsic {
    pub name: &'static str,
    pub immediates: &'static [SveImmediate],
    pub inferred: u8,
}

const ARCH: &str = "core::arch::aarch64";

pub(super) fn builtin_sve(
    module: &ir::Module,
    function: &Span<ir::Function>,
) -> Option<&'static SveIntrinsic> {
    if function.body.is_some()
        || module.target.family != TargetFamily::AArch64
        || !module
            .metadata
            .get(&function.id)
            .is_some_and(|entries| entries.iter().any(|(key, _)| key == "c_builtin"))
    {
        return None;
    }
    let table = sve_table::SVE_INTRINSICS;
    table
        .binary_search_by(|entry| entry.name.cmp(function.name.as_str()))
        .ok()
        .map(|index| &table[index])
}

pub(super) fn sve_type(name: &str) -> Option<rust::Type> {
    sve_table::SVE_TYPES
        .binary_search(&name)
        .ok()
        .map(|_| rust::Type::Custom(format!("{ARCH}::{name}")))
}

pub(super) fn sve_zero(name: &str) -> Option<Expr> {
    sve_type(name)?;
    let call = |function: String, args: Vec<Expr>| Expr::Call {
        func: Box::new(Expr::Path(rust::Path::new(
            ARCH.split("::")
                .chain([function.as_str()])
                .map(rust::Ident::from),
        ))),
        args,
        binding: CallBinding::Generated,
    };
    let base = name.strip_prefix("sv")?.strip_suffix("_t")?;
    let (vector, count) = match base.split_once('x') {
        Some((vector, count)) => (vector, count.parse::<usize>().ok()?),
        None => (base, 1),
    };
    if vector == "bool" {
        return (count == 1).then(|| {
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(call("svpfalse_b".into(), Vec::new()))),
            }))
        });
    }
    let (suffix, literal) = [
        ("uint", "u", "0u"),
        ("int", "s", "0i"),
        ("float", "f", "0.0f"),
    ]
    .into_iter()
    .find_map(|(prefix, short, literal)| {
        let bits = vector.strip_prefix(prefix)?;
        Some((format!("{short}{bits}"), format!("{literal}{bits}")))
    })?;
    let zero = call(format!("svdup_n_{suffix}"), vec![Expr::Var(literal.into())]);
    let value = if count == 1 {
        zero
    } else {
        call(format!("svcreate{count}_{suffix}"), vec![zero; count])
    };
    Some(Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(value)),
    })))
}

fn arch_call(function: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Path(rust::Path::new(
            ARCH.split("::").chain([function]).map(rust::Ident::from),
        ))),
        args,
        binding: CallBinding::Generated,
    }
}

fn data_suffix(name: &str) -> Option<(String, &str)> {
    let base = name.strip_prefix("sv")?.strip_suffix("_t")?;
    [("uint", "u"), ("int", "s"), ("float", "f")]
        .into_iter()
        .find_map(|(prefix, short)| {
            let bits = base.strip_prefix(prefix)?;
            bits.parse::<u32>().ok()?;
            Some((format!("{short}{bits}"), bits))
        })
}

pub(super) fn sve_convert(
    operand: Expr,
    simd: &rust::Type,
    sizeless: &str,
    to_sizeless: bool,
) -> Result<Expr> {
    let Some((suffix, bits)) = data_suffix(sizeless) else {
        return Err(Construct::Function {
            name: sizeless.to_owned(),
            detail: "fixed-length conversion of an SVE predicate".into(),
        }
        .into());
    };
    let (lane, lanes) = intrinsics::simd_parts(simd).ok_or_else(|| Construct::Function {
        name: sizeless.to_owned(),
        detail: "fixed-length SVE conversion without a Simd type".into(),
    })?;
    let lane = lane.clone();
    let ptrue = arch_call(&format!("svptrue_b{bits}"), Vec::new());
    let body = if to_sizeless {
        rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(arch_call(
                &format!("svld1_{suffix}"),
                vec![
                    ptrue,
                    Expr::MethodCall {
                        recv: Box::new(Expr::MethodCall {
                            recv: Box::new(operand),
                            method: "as_array".into(),
                            args: Vec::new(),
                        }),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    },
                ],
            ))),
        }
    } else {
        let lanes_name = "__sve_lanes";
        rust::Block {
            stmts: vec![
                Stmt::Let {
                    name: lanes_name.into(),
                    mutable: true,
                    ty: Some(rust::Type::Array {
                        elem: Box::new(lane.clone()),
                        len: lanes,
                    }),
                    init: Some(Expr::ArrayRepeat {
                        elem: Box::new(Expr::Cast {
                            expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                            ty: lane,
                        }),
                        len: lanes as usize,
                    }),
                },
                Stmt::Expr(arch_call(
                    &format!("svst1_{suffix}"),
                    vec![
                        ptrue,
                        Expr::MethodCall {
                            recv: Box::new(Expr::Var(lanes_name.into())),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        operand,
                    ],
                )),
            ],
            tail: Some(Box::new(Expr::Call {
                func: Box::new(vectors::vector_constructor(simd, "from_array")),
                args: vec![Expr::Var(lanes_name.into())],
                binding: CallBinding::Generated,
            })),
        }
    };
    Ok(Expr::Unsafe(Box::new(body)))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_sve_bit_cast(
        &mut self,
        value: &ir::Value,
        operand: &ir::Value,
    ) -> Result<Expr> {
        let (sizeless, fixed, to_sizeless) = match (
            self.tables.resolve_type(&operand.ty),
            self.tables.resolve_type(&value.ty),
        ) {
            (fixed @ ir::Type::Vector { .. }, ir::Type::Sizeless(name)) => (name, fixed, true),
            (ir::Type::Sizeless(name), fixed @ ir::Type::Vector { .. }) => (name, fixed, false),
            _ => return Err(unsupported_value(value)),
        };
        let simd = self.lower_type(fixed)?;
        self.lower_type(&ir::Type::Sizeless(sizeless.clone()))?;
        let operand = self.lower_value(operand)?;
        sve_convert(operand, &simd, sizeless, to_sizeless)
    }

    pub(super) fn lower_sve_call(
        &mut self,
        label: &str,
        intrinsic: &SveIntrinsic,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let split = arguments
            .len()
            .checked_sub(intrinsic.immediates.len())
            .ok_or_else(|| sve_unsupported(label, intrinsic, "argument count"))?;
        let (operands, immediates) = arguments.split_at(split);
        let mut generics = Vec::new();
        for (kind, argument) in intrinsic.immediates.iter().zip(immediates) {
            if !intrinsics::constant_argument(argument) {
                return Err(sve_unsupported(label, intrinsic, "non-constant immediate").into());
            }
            let value = crate::backend::codegen::expr_to_string(&self.lower_value(argument)?);
            generics.push(rust::Type::Custom(match kind {
                SveImmediate::I32 => format!("{{ ({value}) as i32 }}"),
                SveImmediate::Pattern | SveImmediate::Prefetch => {
                    let ty = if *kind == SveImmediate::Pattern {
                        "svpattern"
                    } else {
                        "svprfop"
                    };
                    format!(
                        "{{ unsafe {{ core::mem::transmute::<i32, {ARCH}::{ty}>(({value}) as i32) }} }}"
                    )
                }
            }));
        }
        generics.extend((0..intrinsic.inferred).map(|_| rust::Type::Custom("_".into())));
        self.dependencies.sve = true;
        let path = rust::Path::new(
            ARCH.split("::")
                .chain([intrinsic.name])
                .map(rust::Ident::from),
        );
        let func = if generics.is_empty() {
            Expr::Path(path)
        } else {
            Expr::GenericPath {
                path,
                type_args: generics,
            }
        };
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(func),
                args: operands
                    .iter()
                    .map(|operand| self.lower_value(operand))
                    .collect::<Result<Vec<_>>>()?,
                binding: CallBinding::Generated,
            })),
        })))
    }
}

fn sve_unsupported(label: &str, intrinsic: &SveIntrinsic, detail: &str) -> Construct {
    Construct::Function {
        name: label.to_owned(),
        detail: format!("SVE intrinsic {}: {detail}", intrinsic.name),
    }
}
