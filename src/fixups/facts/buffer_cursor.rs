use std::collections::BTreeMap;

use crate::fixups::facts::{BindingId, BufferPointerFieldFact, FixupFacts, FunctionId};
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.buffer_pointer_fields.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        all.extend(collect_function(function, &f.body, facts));
    }
    facts.buffer_pointer_fields = all;
}

#[derive(Clone)]
struct ArrayLocal {
    binding: BindingId,
}

#[derive(Clone)]
struct BufferLocal {
    binding: BindingId,
}

#[derive(Clone)]
struct PointerSource {
    array: String,
    index: usize,
}

fn collect_function(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
) -> Vec<BufferPointerFieldFact> {
    let arrays = array_locals(function, facts);
    let buffers = buffer_locals(function, facts);
    if arrays.is_empty() || buffers.is_empty() {
        return Vec::new();
    }

    let mut out = Vec::new();
    for indent in body {
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
            function,
            buffer: buffer_local.binding,
            array: array.binding,
            field: field.to_string(),
            index: source.index,
        });
    }
    out
}

fn array_locals(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, ArrayLocal> {
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter_map(|binding| {
            let ty = facts.binding_type(binding.id)?;
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

fn buffer_locals(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, BufferLocal> {
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter(|binding| {
            facts
                .binding_type(binding.id)
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program};

    fn analyze(stmts: Vec<Stmt>) -> FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        })
        .facts
    }

    fn as_mut_ptr(array: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(array)),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
        }
    }

    fn ptr_add(array: &str, index: i64) -> Expr {
        Expr::MethodCall {
            recv: Box::new(as_mut_ptr(array)),
            method: "add".into(),
            args: vec![int(index)],
        }
    }

    fn field(buffer: &str, field: &str) -> Expr {
        Expr::Field {
            base: Box::new(var(buffer)),
            field: field.into(),
        }
    }

    #[test]
    fn collects_buffer_pointer_fields_with_arbitrary_names() {
        let facts = analyze(vec![
            let_mut(
                "storage",
                "[i32; 4]",
                Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 4,
                },
            ),
            let_mut("buf", "buffer_t", var("zeroed")),
            Stmt::Assign {
                target: field("buf", "begin"),
                value: as_mut_ptr("storage"),
            },
            Stmt::Assign {
                target: field("buf", "pos"),
                value: as_mut_ptr("storage"),
            },
            Stmt::Assign {
                target: field("buf", "limit"),
                value: ptr_add("storage", 4),
            },
        ]);

        let fields = facts
            .buffer_pointer_fields
            .iter()
            .map(|fact| {
                (
                    facts.binding_name(fact.buffer).unwrap(),
                    facts.binding_name(fact.array).unwrap(),
                    fact.field.as_str(),
                    fact.index,
                )
            })
            .collect::<Vec<_>>();

        assert_eq!(
            fields,
            vec![
                ("buf", "storage", "begin", 0),
                ("buf", "storage", "pos", 0),
                ("buf", "storage", "limit", 4),
            ]
        );
    }

    #[test]
    fn rejects_pointer_fields_that_do_not_come_from_local_arrays() {
        let facts = analyze(vec![
            let_mut("buf", "buffer_t", var("zeroed")),
            Stmt::Assign {
                target: field("buf", "pos"),
                value: as_mut_ptr("unknown"),
            },
        ]);

        assert!(facts.buffer_pointer_fields.is_empty());
    }
}
