use super::types::{self, CType, RecordInfo};
use super::{Ctx, Env, LResult, LowerError, VarInfo};
use crate::backend::rust_ast::{
    Abi, ExternDecl, ExternFnDecl, FnDef, FnParam, Item, RecordDef, RecordField, Type, Visibility,
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

pub(crate) fn collect_top_level(tu: &Node, ctx: &mut Ctx) {
    for item in &tu.inner {
        match &item.kind {
            Clang::RecordDecl(r) if r.complete_definition && !r.is_implicit => {
                let Some(name) = &r.name else { continue };
                let tag = format!("{} {}", r.tag_used.as_deref().unwrap_or("struct"), name);
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
            _ => {}
        }
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

    for node in &tu.inner {
        match &node.kind {
            Clang::FunctionDecl(d) if !d.is_implicit => {
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
            Clang::RecordDecl(r)
                if r.complete_definition && !r.is_implicit && is_primary_loc(&r.loc, primary) =>
            {
                let Some(name) = &r.name else { continue };
                let tag = format!("{} {}", r.tag_used.as_deref().unwrap_or("struct"), name);
                if emitted_records.insert(tag.clone())
                    && let Some(info) = ctx.records.get(&tag)
                {
                    items.push(lower_record_def(&tag, info, ctx));
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
        is_main: name == "main",
    };
    let body_stmts = super::stmts::lower_function_body(body, env)?;

    Ok(Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: name != "main",
        abi: None,
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
                    is_main: false,
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
        is_main: false,
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
