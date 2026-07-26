use std::collections::BTreeMap;

use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{
    Attr, Derive, Expr, FnDef, ImplItem, IndentStmt, Item, Program, Repr, Stmt, StructDef,
    StructFields, Type,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    AnonymousStructs::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct AnonymousStructs<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> AnonymousStructs<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::AnonymousStructs,
                kind: "rewrite_anonymous_structs".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "anonymous_structs",
                    facts.anonymous_structs.len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let plans = plans(facts);
    if plans.is_empty() {
        return false;
    }
    let record_fields = record_field_types(program);
    let global_types = global_types(program);
    let mut changed = false;
    for item in &mut program.items {
        changed |= rewrite_item(item, &plans, &record_fields, &global_types, facts);
    }
    changed
}

fn record_field_types(program: &Program) -> BTreeMap<String, BTreeMap<String, Type>> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some((
                record.name.clone(),
                record
                    .fields
                    .iter()
                    .map(|field| (field.name.to_string(), field.ty.clone()))
                    .collect(),
            )),
            _ => None,
        })
        .collect()
}

fn global_types(program: &Program) -> BTreeMap<String, Type> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Static { name, ty, .. } => Some((name.clone(), ty.clone())),
            _ => None,
        })
        .collect()
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

fn rewrite_item(
    item: &mut Item,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
    global_types: &BTreeMap<String, Type>,
    facts: &FixupFacts,
) -> bool {
    match item {
        Item::Record(record) => {
            let mut changed = false;
            for field in &mut record.fields {
                changed |= rewrite_type(&mut field.ty, plans);
            }
            let Some(mut plan) = plans.get(&record.name).cloned() else {
                return changed;
            };
            for field in &mut plan.fields {
                rewrite_type(&mut field.ty, plans);
            }
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
                return rewrite_fn(f, None, plans, record_fields, global_types, facts);
            };
            rewrite_fn(f, Some(function), plans, record_fields, global_types, facts)
        }
        Item::Static { ty, init, .. } => {
            rewrite_type(ty, plans) | rewrite_expr(init, global_types, plans, record_fields)
        }
        Item::Const { ty, init, .. } => {
            rewrite_type(ty, plans) | rewrite_expr(init, global_types, plans, record_fields)
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
                        changed |=
                            rewrite_expr(&mut method.body, global_types, plans, record_fields);
                    }
                }
            }
            changed
        }
        Item::Cfg { item, .. } => rewrite_item(item, plans, record_fields, global_types, facts),
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
        | Item::Macro { .. }
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
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
    global_types: &BTreeMap<String, Type>,
    facts: &FixupFacts,
) -> bool {
    let mut local_types = global_types.clone();
    if let Some(function) = function {
        local_types.extend(local_anonymous_types(function, facts));
    }
    let mut changed = false;
    for param in &mut f.params {
        changed |= rewrite_type(&mut param.ty, plans);
    }
    if let Some(ret) = &mut f.ret {
        changed |= rewrite_type(ret, plans);
    }
    changed |= rewrite_body(&mut f.body, &local_types, plans, record_fields);
    changed
}

fn local_anonymous_types(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, Type> {
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter_map(|binding| {
            let rendered = facts.binding_type(binding.id)?;
            Some((binding.name.clone(), Type::parse(rendered)))
        })
        .collect()
}

fn rewrite_body(
    body: &mut Vec<IndentStmt>,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> bool {
    let mut changed = false;
    for indent in body {
        changed |= rewrite_stmt(&mut indent.stmt, local_types, plans, record_fields);
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= rewrite_body(body, local_types, plans, record_fields);
        });
    }
    changed
}

fn rewrite_stmt(
    stmt: &mut Stmt,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
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
        changed |= rewrite_expr(expr, local_types, plans, record_fields);
        true
    });
    changed
}

fn rewrite_expr(
    expr: &mut Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
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
            let Some(original) = base_original_type(base, local_types, plans, record_fields) else {
                return false;
            };
            let Some(plan) = plans.get(&original) else {
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

fn base_original_type(
    expr: &Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> Option<String> {
    let ty = expr_type(expr, local_types, plans, record_fields)?;
    let Type::Custom(name) = ty.peel_aligned() else {
        return None;
    };
    Some(name.clone())
}

fn expr_type(
    expr: &Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, Plan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> Option<Type> {
    match expr {
        Expr::Var(name) => local_types
            .get(name.as_str())
            .map(|ty| ty.peel_aligned().clone()),
        Expr::Unary {
            op: crate::rust_ast::UnaryOp::Deref,
            expr,
        } => expr_type(expr, local_types, plans, record_fields),
        Expr::Index { base, .. } => {
            let base_ty = expr_type(base, local_types, plans, record_fields)?;
            match base_ty.peel_aligned() {
                Type::Array { elem, .. } | Type::Slice(elem) => Some((**elem).clone()),
                _ => None,
            }
        }
        Expr::Field { base, field } => {
            let base_ty = base_original_type(base, local_types, plans, record_fields)?;
            record_fields.get(&base_ty)?.get(field).cloned()
        }
        Expr::TupleField { base, index } => {
            let base_ty = base_original_type(base, local_types, plans, record_fields)?;
            Some(plans.get(&base_ty)?.fields.get(*index)?.ty.clone())
        }
        _ => None,
    }
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
        | Type::Variadic
        | Type::Never => false,
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
