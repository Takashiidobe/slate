use super::types::{CType, RecordRegistry};
use super::{Env, LResult};
use crate::backend::rust_ast::{Expr, RustValue};
use crate::parse::clang_ast::{Clang, Node};

pub(crate) fn zero_value(ty: &CType, records: &RecordRegistry) -> Expr {
    match ty {
        CType::Record { tag, is_union } => {
            let Some(info) = records.get(tag) else {
                return Expr::Value(RustValue::I64(0));
            };
            let fields = if *is_union {
                info.fields
                    .first()
                    .map(|(name, ty)| vec![(name.clone(), zero_value(ty, records))])
                    .unwrap_or_default()
            } else {
                info.fields
                    .iter()
                    .map(|(name, ty)| (name.clone(), zero_value(ty, records)))
                    .collect()
            };
            Expr::StructLit {
                name: super::types::rust_record_name(tag),
                fields,
            }
        }
        CType::Array { base, len } => Expr::ArrayRepeat {
            elem: Box::new(zero_value(base, records)),
            len: len.unwrap_or(0).max(0) as usize,
        },
        CType::LDouble => crate::frontend::lowerer::runtime_support::long_double_zero_expr(),
        _ if ty.is_flonum() => Expr::Value(RustValue::Float(0.0.into())),
        CType::Ptr(_) | CType::Func { .. } => Expr::Value(RustValue::NullPtr),
        _ => Expr::Value(ty.int_value(0)),
    }
}

pub(crate) fn lower_init(node: &Node, ty: &CType, env: Env) -> LResult<Expr> {
    match (&node.kind, ty) {
        (Clang::InitListExpr(_), CType::Record { tag, is_union }) => {
            let Some(info) = env.records.get(tag) else {
                return Ok(zero_value(ty, env.records));
            };
            if *is_union {
                let Some((name, field_ty)) = info.fields.first() else {
                    return Ok(Expr::Value(RustValue::I64(0)));
                };
                let value = match node.inner.first() {
                    Some(c) => lower_init(c, field_ty, env)?,
                    None => zero_value(field_ty, env.records),
                };
                return Ok(Expr::StructLit {
                    name: super::types::rust_record_name(tag),
                    fields: vec![(name.clone(), value)],
                });
            }
            let mut children = node.inner.iter();
            let fields = info
                .fields
                .iter()
                .map(|(name, field_ty)| {
                    let value = match children.next() {
                        Some(c) => lower_init(c, field_ty, env)?,
                        None => zero_value(field_ty, env.records),
                    };
                    Ok((name.clone(), value))
                })
                .collect::<LResult<Vec<_>>>()?;
            Ok(Expr::StructLit {
                name: super::types::rust_record_name(tag),
                fields,
            })
        }
        (Clang::InitListExpr(_), CType::Array { base, len }) => {
            let elems = node
                .inner
                .iter()
                .map(|c| lower_init(c, base, env))
                .collect::<LResult<Vec<_>>>()?;
            let pad = len
                .map(|len| (len as usize).saturating_sub(elems.len()))
                .unwrap_or(0);
            let mut all = elems;
            all.extend((0..pad).map(|_| zero_value(base, env.records)));
            Ok(Expr::ArrayLit(all))
        }
        (Clang::InitListExpr(_), _) => match node.inner.first() {
            Some(c) => lower_init(c, ty, env),
            None => Ok(zero_value(ty, env.records)),
        },
        (Clang::StringLiteral(l), CType::Array { base, len })
            if matches!(**base, CType::Char | CType::UChar) =>
        {
            let signed = matches!(**base, CType::Char);
            let mut bytes = super::exprs::unescape_c_string(&l.value);
            bytes.push(0);
            let target_len = len.map(|n| n.max(0) as usize).unwrap_or(bytes.len());
            bytes.truncate(target_len);
            let mut elems: Vec<Expr> = bytes
                .iter()
                .map(|b| {
                    let value = if signed {
                        i128::from(i8::from_ne_bytes([*b]))
                    } else {
                        i128::from(*b)
                    };
                    Expr::Value(base.int_value(value))
                })
                .collect();
            elems.resize_with(target_len, || zero_value(base, env.records));
            Ok(Expr::ArrayLit(elems))
        }
        (Clang::Other(o), _) if o.kind.as_deref() == Some("ImplicitValueInitExpr") => {
            Ok(zero_value(ty, env.records))
        }
        _ => super::exprs::lower_expr(node, env),
    }
}
