use super::types::{aggregate_type_name, lower_type};
use crate::backend::rust_ast::{
    Abi, ExternDecl, ExternFnDecl, FnDef, FnParam, Item, RecordDef, RecordField, Type, Visibility,
};
use crate::function_identity::FunctionIdentity;
use crate::parse::ast::{DeclKind, GlobalKind, NodeId, Obj, Program, Type as CType};
use std::collections::{HashMap, HashSet};

pub(crate) fn lower_items(program: &Program) -> Vec<Item> {
    let mut items = Vec::new();
    let mut emitted_aggregates: HashSet<NodeId> = HashSet::new();
    for decl in &program.decls {
        match &decl.kind {
            DeclKind::Function(func) => {
                let Some(obj) = program.globals.iter().find(|obj| obj.id == decl.id) else {
                    continue;
                };
                if !obj.is_live {
                    continue;
                }
                items.push(lower_function(obj, &func.ty, program));
            }
            DeclKind::Var(var) => {
                let Some(obj) = program.globals.iter().find(|obj| obj.id == decl.id) else {
                    continue;
                };
                if obj.global_kind == GlobalKind::Literal {
                    continue;
                }
                items.push(lower_global(obj, &var.ty, &program.types));
            }
            DeclKind::Record(occurrence) => {
                if !emitted_aggregates.insert(occurrence.type_id) {
                    continue;
                }
                let Some(ty) = program.types.get(&occurrence.type_id) else {
                    continue;
                };
                items.push(lower_record_def(ty, &program.types));
            }
            DeclKind::Enum(_) | DeclKind::Typedef { .. } => {}
        }
    }
    items
}

fn lower_function(obj: &Obj, ty: &CType, program: &Program) -> Item {
    let types = &program.types;
    let CType::Func {
        return_ty,
        params,
        is_variadic,
    } = ty
    else {
        unreachable!("function Decl with non-Func type")
    };
    let ret = if obj.name == "main" {
        None
    } else {
        match lower_type(return_ty, types) {
            Type::Unit => None,
            ret => Some(ret),
        }
    };
    let fn_params: Vec<FnParam> = if obj.is_definition {
        obj.params
            .iter()
            .map(|param| FnParam {
                name: param.name.clone(),
                mutable: false,
                ty: lower_type(&param.ty, types),
            })
            .collect()
    } else {
        params
            .iter()
            .enumerate()
            .map(|(i, (name, ty))| FnParam {
                name: if name.is_empty() {
                    format!("arg{i}")
                } else {
                    name.clone()
                },
                mutable: false,
                ty: lower_type(ty, types),
            })
            .collect()
    };

    if !obj.is_definition {
        return Item::ExternBlock {
            abi: Abi::C.spelling().to_string(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name: obj.name.clone(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                params: fn_params,
                variadic: *is_variadic,
                ret,
                safe: false,
            })],
        };
    }

    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: obj.name != "main",
        abi: None,
        name: obj.name.clone(),
        params: fn_params,
        ret,
        body: super::stmts::lower_function_body(obj, program),
    })
}

fn lower_global(obj: &Obj, ty: &CType, types: &HashMap<NodeId, CType>) -> Item {
    let rust_ty = lower_type(ty, types);
    if !obj.is_definition {
        return Item::ExternBlock {
            abi: Abi::C.spelling().to_string(),
            decls: vec![ExternDecl::Static {
                attrs: Vec::new(),
                mutable: true,
                name: obj.name.clone(),
                ty: rust_ty,
            }],
        };
    }
    let init = super::globals::lower_global_init(ty, obj, types);
    Item::Static {
        attrs: Vec::new(),
        vis: Visibility::Private,
        mutable: true,
        name: obj.name.clone(),
        ty: rust_ty,
        init,
    }
}

fn lower_record_def(ty: &CType, types: &HashMap<NodeId, CType>) -> Item {
    let (CType::Struct { members, .. } | CType::Union { members, .. }) = ty else {
        unreachable!("lower_record_def called with non-aggregate type")
    };
    Item::Record(RecordDef {
        comments: Vec::new(),
        vis: Visibility::Private,
        field_vis: Visibility::Private,
        is_union: matches!(ty, CType::Union { .. }),
        allow_non_camel_case: true,
        name: aggregate_type_name(ty),
        fields: members
            .iter()
            .map(|member| RecordField {
                comments: Vec::new(),
                name: member.name.as_str().into(),
                ty: lower_type(&member.ty, types),
            })
            .collect(),
        packed: is_packed(ty),
        align: align_override(ty),
    })
}

fn is_packed(ty: &CType) -> Option<u32> {
    match ty {
        CType::Struct { is_packed, .. } | CType::Union { is_packed, .. } if *is_packed => Some(1),
        _ => None,
    }
}

fn align_override(ty: &CType) -> Option<u32> {
    match ty {
        CType::Struct { align_override, .. } | CType::Union { align_override, .. }
            if *align_override > 0 =>
        {
            Some(*align_override as u32)
        }
        _ => None,
    }
}
