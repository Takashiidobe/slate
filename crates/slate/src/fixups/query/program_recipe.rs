use std::collections::BTreeMap;

use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::support::walk;
use crate::rust_ast::{
    Attr, Derive, Expr, FnDef, ImplItem, IndentStmt, Item, Program, Repr, Stmt, StructDef,
    StructFields, Type,
};

use super::{
    AnonymousStructPlan, AnonymousStructSet, ExprSite, Predicate, QueryContext, Rejection,
    RejectionReason,
};

pub(in crate::fixups) struct ProgramRecipe {
    kind: ProgramRecipeKind,
}

enum ProgramRecipeKind {
    AnonymousStructs(AnonymousStructSet),
}

pub(in crate::fixups) fn rewrite_anonymous_structs(structs: AnonymousStructSet) -> ProgramRecipe {
    ProgramRecipe {
        kind: ProgramRecipeKind::AnonymousStructs(structs),
    }
}

pub(super) struct PreparedProgram {
    pub(super) replacement: Program,
    pub(super) anchors: Vec<(usize, String)>,
}

impl ProgramRecipe {
    pub(super) fn lower(self, query: &QueryContext<'_>) -> Result<PreparedProgram, Rejection> {
        match self.kind {
            ProgramRecipeKind::AnonymousStructs(structs) => {
                let anchors = structs
                    .structs
                    .iter()
                    .map(|plan| (plan.item_index, plan.original_name.clone()))
                    .collect();
                let site = structs.structs.first().map(|plan| ExprSite {
                    item_index: plan.item_index,
                    path: Default::default(),
                    fact_path: Default::default(),
                });
                let mut replacement = query.snapshot_program().clone();
                if !apply_anonymous_structs(
                    &mut replacement,
                    query.snapshot_facts(),
                    structs.structs,
                ) {
                    return Err(Rejection::new(
                        Predicate::AnonymousStructDomain,
                        site,
                        RejectionReason::Contradicted,
                        Vec::new(),
                    ));
                }
                Ok(PreparedProgram {
                    replacement,
                    anchors,
                })
            }
        }
    }
}

fn apply_anonymous_structs(
    program: &mut Program,
    facts: &FixupFacts,
    structs: Vec<AnonymousStructPlan>,
) -> bool {
    let plans = structs
        .into_iter()
        .map(|plan| (plan.original_name.clone(), plan))
        .collect::<BTreeMap<_, _>>();
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

fn rewrite_item(
    item: &mut Item,
    plans: &BTreeMap<String, AnonymousStructPlan>,
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
        Item::Fn(function) => {
            let function_id = facts
                .functions
                .iter()
                .find(|fact| fact.name == function.name)
                .map(|fact| fact.id);
            rewrite_fn(
                function,
                function_id,
                plans,
                record_fields,
                global_types,
                facts,
            )
        }
        Item::Static { ty, init, .. } | Item::Const { ty, init, .. } => {
            rewrite_type(ty, plans) | rewrite_expr(init, global_types, plans, record_fields)
        }
        Item::Struct(record) => rewrite_struct_def(record, plans),
        Item::Impl(block) => {
            let mut changed = rewrite_type(&mut block.self_ty, plans);
            for item in &mut block.items {
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
                    crate::rust_ast::ExternDecl::Fn(function) => {
                        for param in &mut function.params {
                            changed |= rewrite_type(&mut param.ty, plans);
                        }
                        if let Some(ret) = &mut function.ret {
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
        | Item::SupportModule(_)
        | Item::Raw(_) => false,
    }
}

fn rewrite_fn(
    function: &mut FnDef,
    function_id: Option<FunctionId>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
    global_types: &BTreeMap<String, Type>,
    facts: &FixupFacts,
) -> bool {
    let mut local_types = global_types.clone();
    if let Some(function_id) = function_id {
        local_types.extend(local_anonymous_types(function_id, facts));
    }
    let mut changed = false;
    for param in &mut function.params {
        changed |= rewrite_type(&mut param.ty, plans);
    }
    if let Some(ret) = &mut function.ret {
        changed |= rewrite_type(ret, plans);
    }
    changed |= rewrite_body(&mut function.body, &local_types, plans, record_fields);
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
    plans: &BTreeMap<String, AnonymousStructPlan>,
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
    plans: &BTreeMap<String, AnonymousStructPlan>,
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
    plans: &BTreeMap<String, AnonymousStructPlan>,
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

fn positional_fields(fields: &[(String, Expr)], plan: &AnonymousStructPlan) -> Option<Vec<Expr>> {
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
    plans: &BTreeMap<String, AnonymousStructPlan>,
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
    plans: &BTreeMap<String, AnonymousStructPlan>,
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

fn rewrite_struct_def(
    record: &mut StructDef,
    plans: &BTreeMap<String, AnonymousStructPlan>,
) -> bool {
    match &mut record.fields {
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

fn rewrite_type(ty: &mut Type, plans: &BTreeMap<String, AnonymousStructPlan>) -> bool {
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
        Type::FnPtr { params, ret, .. } => rewrite_types(params, plans) | rewrite_type(ret, plans),
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

fn rewrite_types(types: &mut [Type], plans: &BTreeMap<String, AnonymousStructPlan>) -> bool {
    let mut changed = false;
    for ty in types {
        changed |= rewrite_type(ty, plans);
    }
    changed
}
