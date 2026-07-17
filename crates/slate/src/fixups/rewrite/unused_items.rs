use crate::rust_ast::{
    Attr, Block, Expr, ExternDecl, FnDef, FnParam, GenericParam, ImplBlock, ImplItem, IndentStmt,
    Item, MatchArm, Method, Pattern, Program, RecordDef, Stmt, StructDef, StructFields, TraitBound,
    Type,
};
use std::collections::{BTreeMap, BTreeSet};

pub(in crate::fixups) fn fixup(program: &mut Program) -> bool {
    let candidates = candidates(program);
    if candidates.is_empty() {
        return false;
    }

    let mut live = BTreeSet::new();
    for (index, item) in program.items.iter().enumerate() {
        if candidates.contains_key(&index) {
            continue;
        }
        mark_refs(&item_refs(item), &candidates, &mut live);
    }

    let mut pending: Vec<usize> = live.iter().copied().collect();
    while let Some(index) = pending.pop() {
        let before = live.len();
        if let Some(candidate) = candidates.get(&index) {
            mark_refs(&candidate.refs, &candidates, &mut live);
        }
        if live.len() != before {
            pending = live.iter().copied().collect();
        }
    }

    let before = program.items.len();
    let mut index = 0;
    program.items.retain(|_| {
        let keep = !candidates.contains_key(&index) || live.contains(&index);
        index += 1;
        keep
    });
    program.items.len() != before
}

#[derive(Debug)]
struct Candidate {
    name: String,
    variants: BTreeSet<String>,
    refs: BTreeSet<String>,
}

fn candidates(program: &Program) -> BTreeMap<usize, Candidate> {
    program
        .items
        .iter()
        .enumerate()
        .filter_map(|(index, item)| candidate(item).map(|candidate| (index, candidate)))
        .collect()
}

fn candidate(item: &Item) -> Option<Candidate> {
    if has_used_attr(item) {
        return None;
    }
    match item {
        Item::Enum(def) => Some(Candidate {
            name: def.name.clone(),
            variants: def
                .variants
                .iter()
                .map(|variant| variant.name.clone())
                .collect(),
            refs: BTreeSet::new(),
        }),
        Item::Record(def) => Some(Candidate {
            name: def.name.clone(),
            variants: BTreeSet::new(),
            refs: record_refs(def),
        }),
        Item::Struct(def) => Some(Candidate {
            name: def.name.clone(),
            variants: BTreeSet::new(),
            refs: struct_refs(def),
        }),
        Item::Cfg { item, .. } => candidate(item),
        _ => None,
    }
}

fn has_used_attr(item: &Item) -> bool {
    match item {
        Item::Fn(f) => attrs_have_used(&f.attrs),
        Item::Static { attrs, .. } => attrs_have_used(attrs),
        Item::Enum(def) => attrs_have_used(&def.attrs),
        Item::Struct(def) => attrs_have_used(&def.attrs),
        Item::Cfg { item, .. } => has_used_attr(item),
        _ => false,
    }
}

fn attrs_have_used(attrs: &[Attr]) -> bool {
    attrs.iter().any(|attr| matches!(attr, Attr::Used(_)))
}

fn mark_refs(
    refs: &BTreeSet<String>,
    candidates: &BTreeMap<usize, Candidate>,
    live: &mut BTreeSet<usize>,
) {
    for referenced in refs {
        for (index, candidate) in candidates {
            if referenced == &candidate.name || candidate.variants.contains(referenced) {
                live.insert(*index);
            }
        }
    }
}

fn item_refs(item: &Item) -> BTreeSet<String> {
    let mut refs = BTreeSet::new();
    collect_item_refs(item, &mut refs);
    refs
}

fn collect_item_refs(item: &Item, refs: &mut BTreeSet<String>) {
    match item {
        Item::Fn(f) => fn_refs(f, refs),
        Item::Static { ty, init, .. } => {
            collect_type_refs(ty, refs);
            collect_expr_refs(init, refs);
        }
        Item::ExternBlock { decls, .. } => {
            for decl in decls {
                match decl {
                    ExternDecl::Fn(f) => {
                        for param in &f.params {
                            fn_param_refs(param, refs);
                        }
                        if let Some(ret) = &f.ret {
                            collect_type_refs(ret, refs);
                        }
                    }
                    ExternDecl::Static { ty, .. } => collect_type_refs(ty, refs),
                }
            }
        }
        Item::Struct(def) => collect_struct_refs(def, refs),
        Item::Record(def) => collect_record_refs(def, refs),
        Item::Impl(block) => impl_refs(block, refs),
        Item::Cfg { item, .. } => collect_item_refs(item, refs),
        Item::Use { path } => {
            for segment in &path.segments {
                refs.insert(segment.as_str().to_owned());
            }
        }
        Item::Enum(_)
        | Item::Comment(_)
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::Raw(_) => {}
    }
}

fn fn_refs(f: &FnDef, refs: &mut BTreeSet<String>) {
    for param in &f.params {
        fn_param_refs(param, refs);
    }
    if let Some(ret) = &f.ret {
        collect_type_refs(ret, refs);
    }
    collect_body_refs(&f.body, refs);
}

fn fn_param_refs(param: &FnParam, refs: &mut BTreeSet<String>) {
    collect_type_refs(&param.ty, refs);
}

fn impl_refs(block: &ImplBlock, refs: &mut BTreeSet<String>) {
    for param in &block.generics {
        generic_param_refs(param, refs);
    }
    collect_type_refs(&block.self_ty, refs);
    for item in &block.items {
        match item {
            ImplItem::AssocType { ty, .. } => collect_type_refs(ty, refs),
            ImplItem::Method(method) => method_refs(method, refs),
        }
    }
}

fn method_refs(method: &Method, refs: &mut BTreeSet<String>) {
    for param in &method.params {
        fn_param_refs(param, refs);
    }
    if let Some(ret) = &method.ret {
        collect_type_refs(ret, refs);
    }
    collect_expr_refs(&method.body, refs);
}

fn generic_param_refs(param: &GenericParam, refs: &mut BTreeSet<String>) {
    for bound in &param.bounds {
        trait_bound_refs(bound, refs);
    }
}

fn trait_bound_refs(bound: &TraitBound, refs: &mut BTreeSet<String>) {
    for (_, ty) in &bound.assoc {
        collect_type_refs(ty, refs);
    }
}

fn struct_refs(def: &StructDef) -> BTreeSet<String> {
    let mut refs = BTreeSet::new();
    collect_struct_refs(def, &mut refs);
    refs
}

fn collect_struct_refs(def: &StructDef, refs: &mut BTreeSet<String>) {
    for param in &def.generics {
        generic_param_refs(param, refs);
    }
    match &def.fields {
        StructFields::Tuple(fields) => {
            for ty in fields {
                collect_type_refs(ty, refs);
            }
        }
        StructFields::Named(fields) => {
            for (_, ty) in fields {
                collect_type_refs(ty, refs);
            }
        }
    }
}

fn record_refs(def: &RecordDef) -> BTreeSet<String> {
    let mut refs = BTreeSet::new();
    collect_record_refs(def, &mut refs);
    refs
}

fn collect_record_refs(def: &RecordDef, refs: &mut BTreeSet<String>) {
    for field in &def.fields {
        collect_type_refs(&field.ty, refs);
    }
}

fn collect_body_refs(body: &[IndentStmt], refs: &mut BTreeSet<String>) {
    for indent in body {
        collect_stmt_refs(&indent.stmt, refs);
    }
}

fn collect_block_refs(block: &Block, refs: &mut BTreeSet<String>) {
    collect_body_refs(&block.stmts, refs);
    if let Some(tail) = &block.tail {
        collect_expr_refs(tail, refs);
    }
}

fn collect_stmt_refs(stmt: &Stmt, refs: &mut BTreeSet<String>) {
    match stmt {
        Stmt::Let { ty, init, .. } => {
            if let Some(ty) = ty {
                collect_type_refs(ty, refs);
            }
            if let Some(init) = init {
                collect_expr_refs(init, refs);
            }
        }
        Stmt::LetIf {
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            if let Some(ty) = ty {
                collect_type_refs(ty, refs);
            }
            collect_expr_refs(cond, refs);
            collect_body_refs(then_body, refs);
            collect_expr_refs(then_value, refs);
            collect_body_refs(else_body, refs);
            collect_expr_refs(else_value, refs);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            collect_expr_refs(target, refs);
            collect_expr_refs(value, refs);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => collect_expr_refs(expr, refs),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::Unsafe { body } | Stmt::Block(body) => collect_block_refs(body, refs),
        Stmt::While { cond, body } => {
            collect_expr_refs(cond, refs);
            collect_block_refs(body, refs);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            collect_expr_refs(cond, refs);
            collect_body_refs(then_body, refs);
            collect_body_refs(else_body, refs);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_body_refs(body, refs)
        }
        Stmt::For { iter, body, .. } => {
            collect_expr_refs(iter, refs);
            collect_body_refs(body, refs);
        }
        Stmt::Match { expr, arms } => {
            collect_expr_refs(expr, refs);
            for arm in arms {
                collect_match_arm_refs(arm, refs);
            }
        }
    }
}

fn collect_match_arm_refs(arm: &MatchArm, refs: &mut BTreeSet<String>) {
    collect_pattern_refs(&arm.pattern, refs);
    collect_body_refs(&arm.body, refs);
}

fn collect_pattern_refs(pattern: &Pattern, refs: &mut BTreeSet<String>) {
    match pattern {
        Pattern::TupleStruct { name, fields } => {
            refs.insert(name.as_str().to_owned());
            for field in fields {
                collect_pattern_refs(field, refs);
            }
        }
        Pattern::Wildcard | Pattern::Binding(_) | Pattern::I64(_) | Pattern::I128(_) => {}
    }
}

fn collect_expr_refs(expr: &Expr, refs: &mut BTreeSet<String>) {
    match expr {
        Expr::Var(name) => {
            refs.insert(name.as_str().to_owned());
        }
        Expr::Path(path) => {
            for segment in &path.segments {
                refs.insert(segment.as_str().to_owned());
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Closure { body: expr, .. }
        | Expr::AtomicNew { value: expr, .. } => collect_expr_refs(expr, refs),
        Expr::Cast { expr, ty } => {
            collect_expr_refs(expr, refs);
            collect_type_refs(ty, refs);
        }
        Expr::Transmute { from, to, expr } => {
            collect_type_refs(from, refs);
            collect_type_refs(to, refs);
            collect_expr_refs(expr, refs);
        }
        Expr::Block(block) | Expr::Unsafe(block) => collect_block_refs(block, refs),
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                collect_expr_refs(ptr, refs);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                collect_expr_refs(ptr, refs);
            }
            collect_expr_refs(value, refs);
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr() {
                collect_expr_refs(ptr, refs);
            }
            collect_expr_refs(expected, refs);
            collect_expr_refs(desired, refs);
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => {
            collect_expr_refs(lhs, refs);
            collect_expr_refs(rhs, refs);
        }
        Expr::Call { func, args } => {
            collect_expr_refs(func, refs);
            for arg in args {
                collect_expr_refs(arg, refs);
            }
        }
        Expr::MethodCall { recv, args, .. } => {
            collect_expr_refs(recv, refs);
            for arg in args {
                collect_expr_refs(arg, refs);
            }
        }
        Expr::MethodCallGeneric {
            recv,
            type_args,
            args,
            ..
        } => {
            collect_expr_refs(recv, refs);
            for ty in type_args {
                collect_type_refs(ty, refs);
            }
            for arg in args {
                collect_expr_refs(arg, refs);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => collect_expr_refs(base, refs),
        Expr::StructLit { name, fields } => {
            refs.insert(name.clone());
            for (_, value) in fields {
                collect_expr_refs(value, refs);
            }
        }
        Expr::TupleStructLit { name, fields } => {
            refs.insert(name.clone());
            for value in fields {
                collect_expr_refs(value, refs);
            }
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            for elem in elems {
                collect_expr_refs(elem, refs);
            }
        }
        Expr::ArrayRepeat { elem, .. } => collect_expr_refs(elem, refs),
        Expr::VecRepeat { elem, len } => {
            collect_expr_refs(elem, refs);
            collect_expr_refs(len, refs);
        }
        Expr::Macro { name, args } => {
            refs.insert(name.clone());
            for arg in args {
                collect_expr_refs(arg, refs);
            }
        }
        Expr::Match { expr, arms } => {
            collect_expr_refs(expr, refs);
            for arm in arms {
                collect_pattern_refs(&arm.pattern, refs);
                collect_expr_refs(&arm.value, refs);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            collect_expr_refs(cond, refs);
            collect_expr_refs(then_expr, refs);
            collect_expr_refs(else_expr, refs);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            collect_expr_refs(src, refs);
            collect_expr_refs(dst, refs);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            collect_expr_refs(src, refs);
            collect_expr_refs(dst, refs);
            collect_expr_refs(count, refs);
        }
        Expr::WriteBytes { dst, val, count } => {
            collect_expr_refs(dst, refs);
            collect_expr_refs(val, refs);
            collect_expr_refs(count, refs);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
    }
}

fn collect_type_refs(ty: &Type, refs: &mut BTreeSet<String>) {
    match ty {
        Type::Custom(name) => {
            refs.insert(name.clone());
        }
        Type::TyVar(name) => {
            refs.insert(name.as_str().to_owned());
        }
        Type::Generic { name, args } => {
            refs.insert(name.clone());
            for arg in args {
                collect_type_refs(arg, refs);
            }
        }
        Type::Complex(inner)
        | Type::Slice(inner)
        | Type::Ptr { inner, .. }
        | Type::Ref { inner, .. } => collect_type_refs(inner, refs),
        Type::Array { elem, .. } => collect_type_refs(elem, refs),
        Type::FnPtr { params, ret } => {
            for param in params {
                collect_type_refs(param, refs);
            }
            collect_type_refs(ret, refs);
        }
        Type::Prim(_)
        | Type::LongDouble
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => {}
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::{func, int};
    use crate::rust_ast::{
        EnumConst, EnumDef, Ident, Prim, RecordField, RustValue, StructFields, Visibility,
    };

    #[test]
    fn prunes_unreferenced_top_level_types() {
        let mut program = Program {
            items: vec![
                Item::Enum(enum_def("memory_order", &["memory_order_seq_cst"])),
                Item::Struct(StructDef {
                    attrs: Vec::new(),
                    vis: Visibility::Private,
                    generics: Vec::new(),
                    name: "atomic_flag".into(),
                    fields: StructFields::Named(vec![("_Value".into(), Type::Prim(Prim::Bool))]),
                }),
                Item::Fn(func(Vec::new(), None, Vec::new())),
            ],
        };

        assert!(fixup(&mut program));
        let emitted = program.emit();
        assert!(!emitted.contains("enum memory_order"));
        assert!(!emitted.contains("struct atomic_flag"));
        assert!(emitted.contains("fn f()"));
    }

    #[test]
    fn keeps_items_referenced_by_type_or_enum_variant() {
        let mut program = Program {
            items: vec![
                Item::Enum(enum_def("memory_order", &["memory_order_seq_cst"])),
                Item::Record(RecordDef {
                    comments: Vec::new(),
                    vis: Visibility::Private,
                    field_vis: Visibility::Private,
                    is_union: false,
                    allow_non_camel_case: false,
                    name: "node".into(),
                    packed: false,
                    align: None,
                    fields: vec![RecordField {
                        comments: Vec::new(),
                        name: Ident::from("order"),
                        ty: Type::Custom("memory_order".into()),
                    }],
                }),
                Item::Fn(func(
                    Vec::new(),
                    None,
                    vec![
                        Stmt::Let {
                            name: "n".into(),
                            mutable: false,
                            ty: Some(Type::Custom("node".into())),
                            init: None,
                        },
                        Stmt::Expr(Expr::Var(Ident::from("memory_order_seq_cst"))),
                    ],
                )),
            ],
        };

        assert!(!fixup(&mut program));
        let emitted = program.emit();
        assert!(emitted.contains("enum memory_order"));
        assert!(emitted.contains("struct node"));
    }

    #[test]
    fn prunes_unrooted_type_cycles() {
        let mut program = Program {
            items: vec![
                Item::Record(record_with_field("a", "b")),
                Item::Record(record_with_field("b", "a")),
                Item::Fn(func(
                    Vec::new(),
                    None,
                    vec![Stmt::Expr(Expr::Value(RustValue::I64(1)))],
                )),
            ],
        };

        assert!(fixup(&mut program));
        let emitted = program.emit();
        assert!(!emitted.contains("struct a"));
        assert!(!emitted.contains("struct b"));
    }

    #[test]
    fn keeps_dependencies_of_live_items() {
        let mut program = Program {
            items: vec![
                Item::Record(record_with_field("node", "payload")),
                Item::Struct(StructDef {
                    attrs: Vec::new(),
                    vis: Visibility::Private,
                    generics: Vec::new(),
                    name: "payload".into(),
                    fields: StructFields::Tuple(vec![Type::Prim(Prim::I32)]),
                }),
                Item::Fn(func(
                    Vec::new(),
                    None,
                    vec![Stmt::Let {
                        name: "n".into(),
                        mutable: false,
                        ty: Some(Type::Custom("node".into())),
                        init: Some(Expr::TupleStructLit {
                            name: "payload".into(),
                            fields: vec![int(0)],
                        }),
                    }],
                )),
            ],
        };

        assert!(!fixup(&mut program));
        let emitted = program.emit();
        assert!(emitted.contains("struct node"));
        assert!(emitted.contains("struct payload"));
    }

    fn enum_def(name: &str, variants: &[&str]) -> EnumDef {
        EnumDef {
            comments: Vec::new(),
            attrs: Vec::new(),
            vis: Visibility::Private,
            name: name.into(),
            variants: variants
                .iter()
                .enumerate()
                .map(|(index, name)| EnumConst {
                    comments: Vec::new(),
                    name: (*name).into(),
                    value: index as i64,
                })
                .collect(),
        }
    }

    fn record_with_field(name: &str, field_ty: &str) -> RecordDef {
        RecordDef {
            comments: Vec::new(),
            vis: Visibility::Private,
            field_vis: Visibility::Private,
            is_union: false,
            allow_non_camel_case: false,
            name: name.into(),
            packed: false,
            align: None,
            fields: vec![RecordField {
                comments: Vec::new(),
                name: Ident::from("field"),
                ty: Type::Custom(field_ty.into()),
            }],
        }
    }
}
