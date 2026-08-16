use super::types::{self, CType, RecordInfo};
use super::{Ctx, Env, LResult, LowerError, VarInfo};
use crate::backend::rust_ast::{
    Abi, Block, ExternDecl, ExternFnDecl, FnDef, FnParam, IndentStmt, Item, RecordDef, RecordField,
    Stmt, Type, Visibility,
};
use crate::function_identity::FunctionIdentity;
use crate::parse::clang_ast::{Clang, Decl, Node, SourceLocation};
use clang_ast::Id;
use std::collections::HashSet;
use std::path::Path;

fn qual_type_str(qt: &Option<crate::parse::clang_ast::QualType>) -> &str {
    qt.as_ref().map(|t| t.canonical()).unwrap_or("int")
}

fn same_file(a: &Path, b: &Path) -> bool {
    match (std::fs::canonicalize(a), std::fs::canonicalize(b)) {
        (Ok(a), Ok(b)) => a == b,
        _ => a == b,
    }
}

fn is_primary_loc(loc: &Option<SourceLocation>, primary: &Path) -> bool {
    let Some(bare) = loc
        .as_ref()
        .and_then(|loc| loc.spelling_loc.as_ref().or(loc.expansion_loc.as_ref()))
    else {
        return false;
    };
    same_file(Path::new(bare.file.as_ref()), primary)
}

fn has_body(node: &Node) -> bool {
    node.inner
        .iter()
        .any(|c| matches!(c.kind, Clang::CompoundStmt(_)))
}

fn collect_used_ids(node: &Node, used: &mut HashSet<Id>) {
    if let Clang::DeclRefExpr(r) = &node.kind {
        used.insert(r.referenced_decl.id);
    }
    for child in &node.inner {
        collect_used_ids(child, used);
    }
}

pub(crate) fn collect_address_taken_fns(node: &Node, names: &mut HashSet<String>) {
    if let Clang::ImplicitCastExpr(c) = &node.kind
        && c.cast_kind == "FunctionToPointerDecay"
        && let Some(child) = node.inner.first()
        && let Clang::DeclRefExpr(r) = &child.kind
        && r.referenced_decl.kind == "FunctionDecl"
        && let Some(name) = &r.referenced_decl.name
    {
        names.insert(name.clone());
    }
    for child in &node.inner {
        collect_address_taken_fns(child, names);
    }
}

fn used_ids(tu: &Node, primary: &Path) -> HashSet<Id> {
    let mut used = HashSet::new();
    for node in &tu.inner {
        let primary_here = match &node.kind {
            Clang::FunctionDecl(d) | Clang::VarDecl(d) => is_primary_loc(&d.loc, primary),
            Clang::RecordDecl(r) => is_primary_loc(&r.loc, primary),
            _ => false,
        };
        if primary_here {
            collect_used_ids(node, &mut used);
        }
    }
    used
}

fn find_record_tag(ty: &CType) -> Option<String> {
    match ty {
        CType::Record { tag, .. } => Some(tag.clone()),
        CType::Array { base, .. } | CType::Ptr(base) => find_record_tag(base),
        _ => None,
    }
}

fn record_tag(r: &crate::parse::clang_ast::Record, next: Option<&Node>) -> Option<String> {
    if let Some(name) = &r.name {
        return Some(format!(
            "{} {}",
            r.tag_used.as_deref().unwrap_or("struct"),
            name
        ));
    }
    let d = match &next?.kind {
        Clang::TypedefDecl(d) | Clang::VarDecl(d) | Clang::FieldDecl(d) => d,
        _ => return None,
    };
    find_record_tag(&CType::parse(qual_type_str(&d.qual_type)))
}

fn collect_records(siblings: &[Node], ctx: &mut Ctx) {
    for (i, item) in siblings.iter().enumerate() {
        if let Clang::RecordDecl(r) = &item.kind
            && r.complete_definition
            && !r.is_implicit
            && let Some(tag) = record_tag(r, siblings.get(i + 1))
            && !ctx.records.contains_key(&tag)
        {
            let fields = item
                .inner
                .iter()
                .filter_map(|field| match &field.kind {
                    Clang::FieldDecl(d) => Some((
                        d.name.clone().unwrap_or_default(),
                        CType::parse(qual_type_str(&d.qual_type)),
                    )),
                    _ => None,
                })
                .collect();
            ctx.records.insert(
                tag,
                RecordInfo {
                    is_union: r.tag_used.as_deref() == Some("union"),
                    fields,
                    packed: false,
                    align_override: None,
                },
            );
        }
        collect_records(&item.inner, ctx);
    }
}

fn attr_priority(attr_node: &Node) -> i64 {
    attr_node
        .inner
        .first()
        .and_then(enum_const_value)
        .map(|v| v as i64)
        .unwrap_or(65535)
}

fn collect_lifecycle_hooks(tu: &Node, ctx: &mut Ctx) {
    let mut ctors: Vec<(i64, String)> = Vec::new();
    let mut dtors: Vec<(i64, String)> = Vec::new();
    for item in &tu.inner {
        let Clang::FunctionDecl(d) = &item.kind else {
            continue;
        };
        if !has_body(item) {
            continue;
        }
        let Some(name) = &d.name else { continue };
        for attr in &item.inner {
            let Clang::Other(o) = &attr.kind else {
                continue;
            };
            match o.kind.as_deref() {
                Some("ConstructorAttr") => ctors.push((attr_priority(attr), name.clone())),
                Some("DestructorAttr") => dtors.push((attr_priority(attr), name.clone())),
                _ => {}
            }
        }
    }
    ctors.sort_by_key(|(prio, _)| *prio);
    dtors.sort_by_key(|(prio, _)| *prio);
    dtors.reverse();
    ctx.ctor_calls = ctors.into_iter().map(|(_, name)| name).collect();
    ctx.dtor_calls = dtors.into_iter().map(|(_, name)| name).collect();
}

pub(crate) fn collect_top_level(tu: &Node, ctx: &mut Ctx) {
    collect_records(&tu.inner, ctx);
    collect_lifecycle_hooks(tu, ctx);
    for item in &tu.inner {
        match &item.kind {
            Clang::RecordDecl(_) => {
                for field in &item.inner {
                    if let Clang::EnumDecl(_) = &field.kind {
                        register_enum_constants(field, ctx);
                    }
                }
            }
            Clang::VarDecl(d) if !d.is_implicit => {
                ctx.vars.insert(
                    item.id,
                    VarInfo {
                        name: d.name.clone().unwrap_or_default(),
                    },
                );
            }
            Clang::FunctionDecl(d) if !d.is_implicit => {
                ctx.vars.insert(
                    item.id,
                    VarInfo {
                        name: d.name.clone().unwrap_or_default(),
                    },
                );
            }
            Clang::EnumDecl(_) => register_enum_constants(item, ctx),
            _ => {}
        }
    }
}

fn register_enum_constants(enum_decl: &Node, ctx: &mut Ctx) {
    let mut next_value: i128 = 0;
    for child in &enum_decl.inner {
        let Clang::EnumConstantDecl(_) = &child.kind else {
            continue;
        };
        let value = child
            .inner
            .first()
            .and_then(enum_const_value)
            .unwrap_or(next_value);
        ctx.enum_values.insert(child.id, value);
        next_value = value + 1;
    }
}

fn enum_const_value(node: &Node) -> Option<i128> {
    match &node.kind {
        Clang::IntegerLiteral(l) => l.value.trim().parse().ok(),
        Clang::Other(o) => match o.value.as_ref().and_then(serde_json::Value::as_str) {
            Some(v) => v.trim().parse().ok(),
            None => node.inner.first().and_then(enum_const_value),
        },
        _ => node.inner.first().and_then(enum_const_value),
    }
}

pub(crate) fn program_uses_long_double(items: &[Item]) -> bool {
    items.iter().any(|item| match item {
        Item::Fn(f) => {
            f.ret.as_ref().is_some_and(type_contains_long_double)
                || f.params.iter().any(|p| type_contains_long_double(&p.ty))
                || body_has_long_double(&f.body)
        }
        Item::Static { ty, .. } => type_contains_long_double(ty),
        Item::Record(r) => r.fields.iter().any(|f| type_contains_long_double(&f.ty)),
        Item::ExternBlock { decls, .. } => decls.iter().any(|d| match d {
            ExternDecl::Fn(f) => {
                f.ret.as_ref().is_some_and(type_contains_long_double)
                    || f.params.iter().any(|p| type_contains_long_double(&p.ty))
            }
            ExternDecl::Static { ty, .. } => type_contains_long_double(ty),
        }),
        _ => false,
    })
}

fn type_contains_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Ref { inner, .. } | Type::Ptr { inner, .. } | Type::Complex(inner) => {
            type_contains_long_double(inner)
        }
        Type::Slice(inner) => type_contains_long_double(inner),
        Type::Array { elem, .. } => type_contains_long_double(elem),
        Type::FnPtr { params, ret, .. } => {
            type_contains_long_double(ret) || params.iter().any(type_contains_long_double)
        }
        Type::Generic { args, .. } => args.iter().any(type_contains_long_double),
        _ => false,
    }
}

fn body_has_long_double(body: &[IndentStmt]) -> bool {
    body.iter().any(|s| stmt_has_long_double(&s.stmt))
}

fn block_has_long_double(block: &Block) -> bool {
    body_has_long_double(&block.stmts)
}

fn stmt_has_long_double(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Let {
            ty: Some(ty), init, ..
        } => type_contains_long_double(ty) || init.as_ref().is_some_and(expr_has_long_double),
        Stmt::Let { init, .. } => init.as_ref().is_some_and(expr_has_long_double),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_long_double(cond)
                || body_has_long_double(then_body)
                || expr_has_long_double(then_value)
                || body_has_long_double(else_body)
                || expr_has_long_double(else_value)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_long_double(target) || expr_has_long_double(value)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_has_long_double(expr),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_long_double(cond)
                || body_has_long_double(then_body)
                || body_has_long_double(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_long_double(body)
        }
        Stmt::For { iter, body, .. } => expr_has_long_double(iter) || body_has_long_double(body),
        Stmt::Unsafe { body } | Stmt::Block(body) => block_has_long_double(body),
        Stmt::While { cond, body } => expr_has_long_double(cond) || block_has_long_double(body),
        Stmt::Match { expr, arms } => {
            expr_has_long_double(expr) || arms.iter().any(|arm| body_has_long_double(&arm.body))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::InlineAsm(_) => false,
    }
}

fn expr_has_long_double(expr: &crate::backend::rust_ast::Expr) -> bool {
    use crate::backend::rust_ast::Expr;
    match expr {
        Expr::Cast { expr, ty } => type_contains_long_double(ty) || expr_has_long_double(expr),
        Expr::TupleStructLit { name, fields } => {
            name == "LongDouble" || fields.iter().any(expr_has_long_double)
        }
        Expr::Unary { expr, .. } | Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } => {
            expr_has_long_double(expr)
        }
        Expr::Binary { lhs, rhs, .. } => expr_has_long_double(lhs) || expr_has_long_double(rhs),
        Expr::Call { func, args, .. } => {
            expr_has_long_double(func) || args.iter().any(expr_has_long_double)
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_long_double(recv) || args.iter().any(expr_has_long_double)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_has_long_double(base),
        Expr::Index { base, index } => expr_has_long_double(base) || expr_has_long_double(index),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_long_double(cond)
                || expr_has_long_double(then_expr)
                || expr_has_long_double(else_expr)
        }
        _ => false,
    }
}

fn is_shim_loc(loc: &Option<SourceLocation>) -> bool {
    let Some(bare) = loc
        .as_ref()
        .and_then(|loc| loc.spelling_loc.as_ref().or(loc.expansion_loc.as_ref()))
    else {
        return false;
    };
    let Some(shim_dir) = crate::cir::emit::libc_shim_dir() else {
        return false;
    };
    Path::new(bare.file.as_ref()).starts_with(Path::new(&shim_dir))
}

fn emit_records(
    siblings: &[Node],
    ctx: &Ctx,
    items: &mut Vec<Item>,
    emitted: &mut HashSet<String>,
) {
    for (i, item) in siblings.iter().enumerate() {
        if let Clang::RecordDecl(r) = &item.kind
            && r.complete_definition
            && !r.is_implicit
            && !is_shim_loc(&r.loc)
            && let Some(tag) = record_tag(r, siblings.get(i + 1))
            && emitted.insert(tag.clone())
            && let Some(info) = ctx.records.get(&tag)
        {
            items.push(lower_record_def(&tag, info, ctx));
        }
        emit_records(&item.inner, ctx, items, emitted);
    }
}

pub(crate) fn lower_items(tu: &Node, ctx: &mut Ctx, primary: &Path) -> LResult<Vec<Item>> {
    let used = used_ids(tu, primary);

    let mut defined_fn_names: HashSet<String> = HashSet::new();
    for node in &tu.inner {
        if let Clang::FunctionDecl(d) = &node.kind
            && has_body(node)
            && (is_primary_loc(&d.loc, primary) || used.contains(&node.id))
        {
            defined_fn_names.insert(d.name.clone().unwrap_or_default());
        }
    }

    let mut items = Vec::new();
    let mut emitted_records: HashSet<String> = HashSet::new();
    let mut emitted_fns: HashSet<String> = HashSet::new();
    let mut emitted_externs: HashSet<String> = HashSet::new();

    emit_records(&tu.inner, ctx, &mut items, &mut emitted_records);

    for node in &tu.inner {
        match &node.kind {
            Clang::FunctionDecl(d) if !d.is_implicit => {
                if ctx.intrinsic_passthroughs.contains_key(&node.id) {
                    continue;
                }
                let name = d.name.clone().unwrap_or_default();
                if has_body(node)
                    && defined_fn_names.contains(&name)
                    && emitted_fns.insert(name.clone())
                {
                    let mut extra = Vec::new();
                    let fn_item = lower_function(node, d, ctx, &mut extra)?;
                    items.extend(extra);
                    items.push(fn_item);
                } else if !defined_fn_names.contains(&name)
                    && used.contains(&node.id)
                    && emitted_externs.insert(name)
                {
                    items.push(lower_function(node, d, ctx, &mut Vec::new())?);
                }
            }
            Clang::VarDecl(d) if !d.is_implicit => {
                let primary_here = is_primary_loc(&d.loc, primary);
                if primary_here {
                    items.push(lower_global(node, d, ctx, false)?);
                } else if used.contains(&node.id) {
                    items.push(lower_global(node, d, ctx, true)?);
                }
            }
            _ => {}
        }
    }
    Ok(items)
}

fn lower_function(
    node: &Node,
    d: &Decl,
    ctx: &mut Ctx,
    extra_items: &mut Vec<Item>,
) -> LResult<Item> {
    let CType::Func {
        return_ty,
        params: proto_params,
        is_variadic,
    } = CType::parse(qual_type_str(&d.qual_type))
    else {
        return Err(LowerError::NonFunctionType(d.name.clone()));
    };
    let name = d.name.clone().unwrap_or_default();
    let body_node = node
        .inner
        .iter()
        .find(|c| matches!(c.kind, Clang::CompoundStmt(_)));
    let param_decls: Vec<&Node> = node
        .inner
        .iter()
        .filter(|c| matches!(c.kind, Clang::ParmVarDecl(_)))
        .collect();

    let fn_params: Vec<FnParam> = if !param_decls.is_empty() {
        param_decls
            .iter()
            .enumerate()
            .map(|(i, p)| {
                let Clang::ParmVarDecl(pd) = &p.kind else {
                    unreachable!()
                };
                let ty = CType::parse(qual_type_str(&pd.qual_type));
                let name = match &pd.name {
                    Some(name) if !name.is_empty() => name.clone(),
                    _ => format!("arg{i}"),
                };
                FnParam {
                    name,
                    mutable: false,
                    ty: ty.lower(&ctx.records),
                }
            })
            .collect()
    } else {
        proto_params
            .iter()
            .enumerate()
            .map(|(i, ty)| FnParam {
                name: format!("arg{i}"),
                mutable: false,
                ty: ty.lower(&ctx.records),
            })
            .collect()
    };

    let ret = if name == "main" {
        None
    } else {
        match return_ty.lower(&ctx.records) {
            Type::Unit => None,
            ty => Some(ty),
        }
    };

    let Some(body) = body_node else {
        return Ok(Item::ExternBlock {
            abi: Abi::C.spelling().to_string(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name,
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                params: fn_params,
                variadic: is_variadic,
                ret,
                safe: false,
            })],
        });
    };

    collect_locals(node, &name, ctx, extra_items)?;

    let env = Env {
        vars: &ctx.vars,
        records: &ctx.records,
        enum_values: &ctx.enum_values,
        is_main: name == "main",
        continue_label: None,
        break_label: None,
        goto: None,
        dtor_calls: &ctx.dtor_calls,
        ret_ty: return_ty.as_ref(),
        intrinsic_passthroughs: &ctx.intrinsic_passthroughs,
        uses_complex_runtime: &ctx.uses_complex_runtime,
    };
    let mut body_stmts = super::stmts::lower_function_body(body, env, ret.is_some())?;
    if env.is_main {
        super::stmts::splice_ctor_prelude(&mut body_stmts, &ctx.ctor_calls);
    }

    let abi = ctx.address_taken_fns.contains(&name).then_some(Abi::C);
    Ok(Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: name != "main",
        abi,
        name,
        params: fn_params,
        ret,
        body: body_stmts,
    }))
}

fn collect_locals(
    node: &Node,
    fn_name: &str,
    ctx: &mut Ctx,
    extra_items: &mut Vec<Item>,
) -> LResult<()> {
    match &node.kind {
        Clang::ParmVarDecl(d) => {
            ctx.vars.insert(
                node.id,
                VarInfo {
                    name: d.name.clone().unwrap_or_default(),
                },
            );
        }
        Clang::VarDecl(d) if !d.is_implicit => {
            let ty = CType::parse(qual_type_str(&d.qual_type));
            let is_static = d.storage_class.as_deref() == Some("static");
            let base_name = d.name.clone().unwrap_or_default();
            if is_static {
                let mangled = format!("{fn_name}__{base_name}");
                let env = Env {
                    vars: &ctx.vars,
                    records: &ctx.records,
                    enum_values: &ctx.enum_values,
                    is_main: false,
                    continue_label: None,
                    break_label: None,
                    goto: None,
                    dtor_calls: &[],
                    ret_ty: &super::VOID_RET,
                    intrinsic_passthroughs: &ctx.intrinsic_passthroughs,
                    uses_complex_runtime: &ctx.uses_complex_runtime,
                };
                let init = match node.inner.first() {
                    Some(c) => super::globals::lower_init(c, &ty, env)?,
                    None => super::globals::zero_value(&ty, &ctx.records),
                };
                extra_items.push(Item::Static {
                    attrs: Vec::new(),
                    vis: Visibility::Private,
                    mutable: true,
                    name: mangled.clone(),
                    ty: ty.lower(&ctx.records),
                    init,
                });
                ctx.vars.insert(node.id, VarInfo { name: mangled });
            } else {
                ctx.vars.insert(node.id, VarInfo { name: base_name });
            }
        }
        _ => {}
    }
    for child in &node.inner {
        collect_locals(child, fn_name, ctx, extra_items)?;
    }
    Ok(())
}

fn lower_global(node: &Node, d: &Decl, ctx: &Ctx, force_extern: bool) -> LResult<Item> {
    let ty = CType::parse(qual_type_str(&d.qual_type));
    let rust_ty = ty.lower(&ctx.records);
    let name = d.name.clone().unwrap_or_default();
    let is_extern =
        force_extern || (d.storage_class.as_deref() == Some("extern") && node.inner.is_empty());
    if is_extern {
        return Ok(Item::ExternBlock {
            abi: Abi::C.spelling().to_string(),
            decls: vec![ExternDecl::Static {
                attrs: Vec::new(),
                mutable: true,
                name,
                ty: rust_ty,
            }],
        });
    }
    let env = Env {
        vars: &ctx.vars,
        records: &ctx.records,
        enum_values: &ctx.enum_values,
        is_main: false,
        continue_label: None,
        break_label: None,
        goto: None,
        dtor_calls: &[],
        ret_ty: &super::VOID_RET,
        intrinsic_passthroughs: &ctx.intrinsic_passthroughs,
        uses_complex_runtime: &ctx.uses_complex_runtime,
    };
    let init = match node.inner.first() {
        Some(c) => super::globals::lower_init(c, &ty, env)?,
        None => super::globals::zero_value(&ty, &ctx.records),
    };
    Ok(Item::Static {
        attrs: Vec::new(),
        vis: Visibility::Private,
        mutable: true,
        name,
        ty: rust_ty,
        init,
    })
}

fn lower_record_def(tag: &str, info: &RecordInfo, ctx: &Ctx) -> Item {
    Item::Record(RecordDef {
        comments: Vec::new(),
        vis: Visibility::Private,
        field_vis: Visibility::Private,
        is_union: info.is_union,
        allow_non_camel_case: true,
        name: types::rust_record_name(tag),
        fields: info
            .fields
            .iter()
            .map(|(name, ty)| RecordField {
                comments: Vec::new(),
                name: name.as_str().into(),
                ty: ty.lower(&ctx.records),
            })
            .collect(),
        packed: info.packed.then_some(1),
        align: info.align_override,
    })
}
