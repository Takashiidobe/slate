use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::support::walk;
use crate::rust_ast::{
    Attr, Derive, Expr, FnDef, ImplItem, IndentStmt, Item, Program, Repr, Stmt, StructDef,
    StructFields, Type,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let plans = plans(facts);
    if plans.is_empty() {
        return false;
    }
    let mut changed = false;
    for item in &mut program.items {
        changed |= rewrite_item(item, &plans, facts);
    }
    changed
}

#[derive(Clone)]
struct Plan {
    generated_name: String,
    fields: Vec<FieldPlan>,
}

#[derive(Clone)]
struct FieldPlan {
    name: String,
    ty: Type,
}

fn plans(facts: &FixupFacts) -> BTreeMap<String, Plan> {
    facts
        .anonymous_structs
        .iter()
        .map(|fact| {
            (
                fact.original_name.clone(),
                Plan {
                    generated_name: fact.generated_name.clone(),
                    fields: fact
                        .fields
                        .iter()
                        .map(|field| FieldPlan {
                            name: field.name.clone(),
                            ty: field.ty.clone(),
                        })
                        .collect(),
                },
            )
        })
        .collect()
}

fn rewrite_item(item: &mut Item, plans: &BTreeMap<String, Plan>, facts: &FixupFacts) -> bool {
    match item {
        Item::Record(record) => {
            let Some(plan) = plans.get(&record.name).cloned() else {
                return false;
            };
            *item = Item::Struct(StructDef {
                attrs: vec![
                    Attr::Repr(vec![Repr::C]),
                    Attr::Derive(vec![Derive::Clone, Derive::Copy]),
                ],
                vis: record.vis,
                generics: vec![],
                name: plan.generated_name,
                fields: StructFields::Tuple(
                    plan.fields.into_iter().map(|field| field.ty).collect(),
                ),
            });
            true
        }
        Item::Fn(f) => {
            let Some(function) = facts
                .functions
                .iter()
                .find(|function| function.name == f.name)
                .map(|function| function.id)
            else {
                return rewrite_fn(f, None, plans, facts);
            };
            rewrite_fn(f, Some(function), plans, facts)
        }
        Item::Static { ty, init, .. } => {
            rewrite_type(ty, plans) | rewrite_expr(init, &BTreeMap::new(), plans)
        }
        Item::Struct(s) => rewrite_struct_def(s, plans),
        Item::Impl(im) => {
            let mut changed = rewrite_type(&mut im.self_ty, plans);
            for item in &mut im.items {
                match item {
                    ImplItem::AssocType { ty, .. } => changed |= rewrite_type(ty, plans),
                    ImplItem::Method(method) => {
                        for param in &mut method.params {
                            changed |= rewrite_type(&mut param.ty, plans);
                        }
                        if let Some(ret) = &mut method.ret {
                            changed |= rewrite_type(ret, plans);
                        }
                        changed |= rewrite_expr(&mut method.body, &BTreeMap::new(), plans);
                    }
                }
            }
            changed
        }
        Item::Cfg { item, .. } => rewrite_item(item, plans, facts),
        Item::ExternBlock { decls, .. } => {
            let mut changed = false;
            for decl in decls {
                match decl {
                    crate::rust_ast::ExternDecl::Fn(f) => {
                        for param in &mut f.params {
                            changed |= rewrite_type(&mut param.ty, plans);
                        }
                        if let Some(ret) = &mut f.ret {
                            changed |= rewrite_type(ret, plans);
                        }
                    }
                    crate::rust_ast::ExternDecl::Static { ty, .. } => {
                        changed |= rewrite_type(ty, plans)
                    }
                }
            }
            changed
        }
        Item::Comment(_)
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::Use { .. }
        | Item::Enum(_)
        | Item::Raw(_) => false,
    }
}

fn rewrite_fn(
    f: &mut FnDef,
    function: Option<FunctionId>,
    plans: &BTreeMap<String, Plan>,
    facts: &FixupFacts,
) -> bool {
    let local_types = function.map_or_else(BTreeMap::new, |function| {
        local_anonymous_types(function, facts, plans)
    });
    let mut changed = false;
    for param in &mut f.params {
        changed |= rewrite_type(&mut param.ty, plans);
    }
    if let Some(ret) = &mut f.ret {
        changed |= rewrite_type(ret, plans);
    }
    changed |= rewrite_body(&mut f.body, &local_types, plans);
    changed
}

fn local_anonymous_types(
    function: FunctionId,
    facts: &FixupFacts,
    plans: &BTreeMap<String, Plan>,
) -> BTreeMap<String, String> {
    let original_names: BTreeSet<_> = plans.keys().map(String::as_str).collect();
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter_map(|binding| {
            let rendered = facts.binding_type(binding.id)?;
            original_names
                .contains(rendered)
                .then(|| (binding.name.clone(), rendered.to_string()))
        })
        .collect()
}

fn rewrite_body(
    body: &mut Vec<IndentStmt>,
    local_types: &BTreeMap<String, String>,
    plans: &BTreeMap<String, Plan>,
) -> bool {
    let mut changed = false;
    for indent in body {
        changed |= rewrite_stmt(&mut indent.stmt, local_types, plans);
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= rewrite_body(body, local_types, plans);
        });
    }
    changed
}

fn rewrite_stmt(
    stmt: &mut Stmt,
    local_types: &BTreeMap<String, String>,
    plans: &BTreeMap<String, Plan>,
) -> bool {
    let mut changed = false;
    match stmt {
        Stmt::Let { ty, .. } | Stmt::LetIf { ty, .. } => {
            if let Some(ty) = ty {
                changed |= rewrite_type(ty, plans);
            }
        }
        _ => {}
    }
    walk::stmt_exprs_mut_with(stmt, &mut |expr| {
        changed |= rewrite_expr(expr, local_types, plans);
        true
    });
    changed
}

fn rewrite_expr(
    expr: &mut Expr,
    local_types: &BTreeMap<String, String>,
    plans: &BTreeMap<String, Plan>,
) -> bool {
    match expr {
        Expr::StructLit { name, fields } => {
            let Some(plan) = plans.get(name).cloned() else {
                return false;
            };
            let Some(values) = positional_fields(fields, &plan) else {
                return false;
            };
            *expr = Expr::TupleStructLit {
                name: plan.generated_name,
                fields: values,
            };
            true
        }
        Expr::Field { base, field } => {
            let Some(original) = base_local_type(base, local_types) else {
                return false;
            };
            let Some(plan) = plans.get(original) else {
                return false;
            };
            let Some(index) = plan
                .fields
                .iter()
                .position(|candidate| candidate.name == *field)
            else {
                return false;
            };
            *expr = Expr::TupleField {
                base: Box::new((**base).clone()),
                index,
            };
            true
        }
        Expr::Cast { ty, .. } => rewrite_type(ty, plans),
        Expr::Transmute { from, to, .. } => rewrite_type(from, plans) | rewrite_type(to, plans),
        Expr::MethodCallGeneric { type_args, .. } => rewrite_types(type_args, plans),
        Expr::TupleStructLit { name, .. } => {
            if let Some(plan) = plans.get(name) {
                *name = plan.generated_name.clone();
                true
            } else {
                false
            }
        }
        _ => false,
    }
}

fn positional_fields(fields: &[(String, Expr)], plan: &Plan) -> Option<Vec<Expr>> {
    if fields.len() != plan.fields.len() {
        return None;
    }
    plan.fields
        .iter()
        .map(|field| {
            fields
                .iter()
                .find_map(|(name, value)| (name == &field.name).then(|| value.clone()))
        })
        .collect()
}

fn base_local_type<'a>(expr: &Expr, local_types: &'a BTreeMap<String, String>) -> Option<&'a str> {
    let Expr::Var(name) = expr else {
        return None;
    };
    local_types.get(name.as_str()).map(String::as_str)
}

fn rewrite_struct_def(s: &mut StructDef, plans: &BTreeMap<String, Plan>) -> bool {
    match &mut s.fields {
        StructFields::Tuple(fields) => rewrite_types(fields, plans),
        StructFields::Named(fields) => {
            let mut changed = false;
            for (_, ty) in fields {
                changed |= rewrite_type(ty, plans);
            }
            changed
        }
    }
}

fn rewrite_type(ty: &mut Type, plans: &BTreeMap<String, Plan>) -> bool {
    match ty {
        Type::Custom(name) => {
            let Some(plan) = plans.get(name) else {
                return false;
            };
            *name = plan.generated_name.clone();
            true
        }
        Type::Complex(inner)
        | Type::Ref { inner, .. }
        | Type::Slice(inner)
        | Type::Ptr { inner, .. } => rewrite_type(inner, plans),
        Type::Generic { args, .. } => rewrite_types(args, plans),
        Type::Array { elem, .. } => rewrite_type(elem, plans),
        Type::FnPtr { params, ret } => rewrite_types(params, plans) | rewrite_type(ret, plans),
        Type::Prim(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic => false,
    }
}

fn rewrite_types(types: &mut [Type], plans: &BTreeMap<String, Plan>) -> bool {
    let mut changed = false;
    for ty in types {
        changed |= rewrite_type(ty, plans);
    }
    changed
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Ident, Prim, RecordDef, RecordField, Visibility};

    #[test]
    fn rewrites_anonymous_record_to_tuple_struct_and_tuple_fields() {
        let mut program = Program {
            items: vec![
                Item::Record(RecordDef {
                    comments: vec![],
                    vis: Visibility::Private,
                    field_vis: Visibility::Private,
                    is_union: false,
                    allow_non_camel_case: false,
                    name: "anon_0".into(),
                    packed: false,
                    align: None,
                    fields: vec![
                        RecordField {
                            comments: vec![],
                            name: Ident::from("x"),
                            ty: Type::Prim(Prim::I32),
                        },
                        RecordField {
                            comments: vec![],
                            name: Ident::from("y"),
                            ty: Type::Prim(Prim::I32),
                        },
                    ],
                }),
                Item::Fn(func(
                    vec![],
                    None,
                    vec![
                        Stmt::Let {
                            name: "point".into(),
                            mutable: true,
                            ty: Some(Type::Custom("anon_0".into())),
                            init: Some(Expr::StructLit {
                                name: "anon_0".into(),
                                fields: vec![("x".into(), int(3)), ("y".into(), int(4))],
                            }),
                        },
                        temp(
                            "sum",
                            "i32",
                            Expr::Field {
                                base: Box::new(var("point")),
                                field: "x".into(),
                            },
                        ),
                    ],
                )),
            ],
        };
        let analyzed = facts::analyze(program.clone());

        assert!(fixup(&mut program, &analyzed.facts));

        assert_eq!(
            program.emit(),
            "\
#[repr(C)]
#[derive(Clone, Copy)]
struct __slate_anonymous_struct_0(i32, i32);

fn f() {
    let mut point: __slate_anonymous_struct_0 = __slate_anonymous_struct_0(3, 4);
    let sum: i32 = point.0;
}
"
        );
    }
}
