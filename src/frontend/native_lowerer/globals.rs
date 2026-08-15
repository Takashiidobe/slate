use super::types::{aggregate_type_name, lower_type, resolve_aggregate};
use crate::backend::rust_ast::{Expr, Prim, Raw, RustValue, Type, UnaryOp};
use crate::parse::ast::{NodeId, Obj, Relocation, Type as CType};
use std::collections::HashMap;

pub(crate) fn lower_global_init(ty: &CType, obj: &Obj, types: &HashMap<NodeId, CType>) -> Expr {
    let data = obj.init_data.as_deref().unwrap_or(&[]);
    decode(ty, data, 0, &obj.relocations, types)
}

fn decode(
    ty: &CType,
    data: &[u8],
    offset: usize,
    relocs: &[Relocation],
    types: &HashMap<NodeId, CType>,
) -> Expr {
    let resolved = resolve_aggregate(ty, types);
    match resolved {
        CType::Struct { members, .. } => Expr::StructLit {
            name: aggregate_type_name(resolved),
            fields: members
                .iter()
                .map(|member| {
                    (
                        member.name.clone(),
                        decode(
                            &member.ty,
                            data,
                            offset + member.offset as usize,
                            relocs,
                            types,
                        ),
                    )
                })
                .collect(),
        },
        CType::Union { members, .. } => match members.first() {
            Some(member) => Expr::StructLit {
                name: aggregate_type_name(resolved),
                fields: vec![(
                    member.name.clone(),
                    decode(
                        &member.ty,
                        data,
                        offset + member.offset as usize,
                        relocs,
                        types,
                    ),
                )],
            },
            None => Expr::Value(RustValue::I64(0)),
        },
        CType::Array { base, len } | CType::Vector { base, len } => {
            let elem_size = base.size().max(1) as usize;
            Expr::ArrayLit(
                (0..(*len).max(0))
                    .map(|i| decode(base, data, offset + i as usize * elem_size, relocs, types))
                    .collect(),
            )
        }
        CType::Ptr(_) | CType::Func { .. } => decode_pointer(resolved, offset, relocs, data, types),
        _ if resolved.is_flonum() => decode_float(resolved, data, offset),
        _ => decode_int(resolved, data, offset),
    }
}

fn decode_pointer(
    ty: &CType,
    offset: usize,
    relocs: &[Relocation],
    data: &[u8],
    types: &HashMap<NodeId, CType>,
) -> Expr {
    let rust_ty = lower_type(ty, types);
    if let Some(reloc) = relocs.iter().find(|reloc| reloc.offset == offset) {
        let addr = Expr::Unary {
            op: UnaryOp::Raw(Raw::Const),
            expr: Box::new(Expr::Var(reloc.label.as_str().into())),
        };
        let addr = if reloc.addend != 0 {
            Expr::MethodCall {
                recv: Box::new(Expr::Cast {
                    expr: Box::new(addr),
                    ty: Type::Ptr {
                        mutable: false,
                        inner: Box::new(Type::Prim(Prim::U8)),
                    },
                }),
                method: "wrapping_offset".into(),
                args: vec![Expr::Value(RustValue::I64(reloc.addend))],
            }
        } else {
            addr
        };
        return Expr::Cast {
            expr: Box::new(addr),
            ty: rust_ty,
        };
    }
    let raw = read_uint(data, offset, 8);
    if raw == 0 {
        Expr::Value(RustValue::NullPtr)
    } else {
        Expr::Cast {
            expr: Box::new(Expr::Value(RustValue::Usize(raw as usize))),
            ty: rust_ty,
        }
    }
}

fn decode_float(ty: &CType, data: &[u8], offset: usize) -> Expr {
    let value = match ty.size() {
        4 => f32::from_le_bytes(read_bytes::<4>(data, offset)) as f64,
        _ => f64::from_le_bytes(read_bytes::<8>(data, offset)),
    };
    Expr::Value(RustValue::Float(value.into()))
}

fn decode_int(ty: &CType, data: &[u8], offset: usize) -> Expr {
    let size = ty.size().clamp(1, 16) as usize;
    let unsigned = ty.is_unsigned();
    if unsigned {
        Expr::Value(RustValue::TypedUInt(
            read_uint(data, offset, size) as u128,
            int_prim(ty),
        ))
    } else {
        Expr::Value(RustValue::TypedInt(
            read_int(data, offset, size) as i128,
            int_prim(ty),
        ))
    }
}

fn int_prim(ty: &CType) -> Prim {
    match ty.size() {
        1 => {
            if ty.is_unsigned() {
                Prim::U8
            } else {
                Prim::I8
            }
        }
        2 => {
            if ty.is_unsigned() {
                Prim::U16
            } else {
                Prim::I16
            }
        }
        4 => {
            if ty.is_unsigned() {
                Prim::U32
            } else {
                Prim::I32
            }
        }
        16 => {
            if ty.is_unsigned() {
                Prim::U128
            } else {
                Prim::I128
            }
        }
        _ => {
            if ty.is_unsigned() {
                Prim::U64
            } else {
                Prim::I64
            }
        }
    }
}

fn read_bytes<const N: usize>(data: &[u8], offset: usize) -> [u8; N] {
    let mut out = [0u8; N];
    let available = data.len().saturating_sub(offset).min(N);
    if available > 0 {
        out[..available].copy_from_slice(&data[offset..offset + available]);
    }
    out
}

fn read_uint(data: &[u8], offset: usize, size: usize) -> u64 {
    let mut out = [0u8; 8];
    let available = data.len().saturating_sub(offset).min(size).min(8);
    if available > 0 {
        out[..available].copy_from_slice(&data[offset..offset + available]);
    }
    u64::from_le_bytes(out)
}

fn read_int(data: &[u8], offset: usize, size: usize) -> i64 {
    let raw = read_uint(data, offset, size);
    let shift = (8 - size.min(8)) * 8;
    ((raw << shift) as i64) >> shift
}
