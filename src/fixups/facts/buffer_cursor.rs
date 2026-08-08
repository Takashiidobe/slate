use std::collections::BTreeMap;

use crate::fixups::facts::{
    self, AstPath, BindingFact, BindingId, BindingTypeFact, BufferPointerFieldFact, FunctionId,
    PathSegment, Site,
};
use crate::rust_ast::{Expr, Ident, IndentStmt, RustValue, Stmt, Type};
#[derive(Clone)]
struct ArrayLocal {
    binding: BindingId<'db>,
}

#[derive(Clone)]
struct BufferLocal {
    binding: BindingId<'db>,
}

#[derive(Clone)]
struct PointerSource {
    array: String,
    index: usize,
}

pub(in crate::fixups) fn collect_for_function(
    function: FunctionId<'db>,
    body: &[IndentStmt],
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
) -> Vec<BufferPointerFieldFact<'db>> {
    let arrays = array_locals(function, bindings, binding_types);
    let buffers = buffer_locals(function, bindings, binding_types);
    if arrays.is_empty() || buffers.is_empty() {
        return Vec::new();
    }

    let mut out = Vec::new();
    for (index, indent) in body.iter().enumerate() {
        let Stmt::Assign { target, value } = &indent.stmt else {
            continue;
        };
        let Some((buffer, field)) = buffer_field(target) else {
            continue;
        };
        let Some(buffer_local) = buffers.get(buffer.as_str()) else {
            continue;
        };
        let Some(source) = pointer_source(value) else {
            continue;
        };
        let Some(array) = arrays.get(source.array.as_str()) else {
            continue;
        };
        out.push(BufferPointerFieldFact {
            site: Site {
                function,
                path: AstPath(vec![PathSegment::Stmt(index)]),
            },
            buffer: buffer_local.binding,
            array: array.binding,
            field: field.to_string(),
            index: source.index,
        });
    }
    out
}

fn array_locals(
    function: FunctionId<'db>,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
) -> BTreeMap<String, ArrayLocal> {
    bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter_map(|binding| {
            let ty = facts::binding_type(binding_types, binding.id)?;
            array_len_from_rendered_type(ty)?;
            Some((
                binding.name.clone(),
                ArrayLocal {
                    binding: binding.id,
                },
            ))
        })
        .collect()
}

fn buffer_locals(
    function: FunctionId<'db>,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
) -> BTreeMap<String, BufferLocal> {
    bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter(|binding| {
            facts::binding_type(binding_types, binding.id)
                .is_some_and(|ty| !ty.starts_with('[') && !ty.starts_with('*'))
        })
        .map(|binding| {
            (
                binding.name.clone(),
                BufferLocal {
                    binding: binding.id,
                },
            )
        })
        .collect()
}

fn array_len_from_rendered_type(ty: &str) -> Option<usize> {
    let (_, len) = ty.rsplit_once(';')?;
    len.trim_end_matches(']').trim().parse().ok()
}

fn buffer_field(expr: &Expr) -> Option<(&Ident, &str)> {
    let Expr::Field { base, field } = expr else {
        return None;
    };
    let Expr::Var(buffer) = &**base else {
        return None;
    };
    Some((buffer, field.as_str()))
}

fn pointer_source(expr: &Expr) -> Option<PointerSource> {
    match peel_unsafe(expr) {
        Expr::ArrayPtr {
            array,
            mutable: true,
        } => {
            let Expr::Var(array) = &**array else {
                return None;
            };
            Some(PointerSource {
                array: array.to_string(),
                index: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "as_mut_ptr" && args.is_empty() => {
            let Expr::Var(array) = &**recv else {
                return None;
            };
            Some(PointerSource {
                array: array.to_string(),
                index: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            let mut source = pointer_source(recv)?;
            source.index += usize_index(&args[0])?;
            Some(source)
        }
        _ => None,
    }
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().map_or(expr, peel_unsafe)
        }
        _ => expr,
    }
}

fn usize_index(expr: &Expr) -> Option<usize> {
    match expr {
        Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => Some(*n),
        Expr::Cast {
            expr,
            ty: Type::Prim(crate::rust_ast::Prim::Usize),
        } => usize_index(expr),
        _ => None,
    }
}
