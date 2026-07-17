use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::support::walk;
use crate::rust_ast::{
    BinOp, Expr, FnDef, Ident, IndentStmt, Item, Program, RecordDef, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(program: &mut Program) {
    let records = record_layouts(program);
    let comparators = comparator_plans(program);
    if comparators.is_empty() {
        return;
    }

    for item in &mut program.items {
        if let Item::Fn(f) = item {
            rewrite_body(&mut f.body, &records, &comparators);
        }
    }
}

pub(in crate::fixups) fn prune_unused_externs(program: &mut Program) {
    let used = direct_calls(program);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                crate::rust_ast::ExternDecl::Fn(f)
                    if matches!(f.name.as_str(), "qsort" | "bsearch") =>
                {
                    used.contains(&f.name)
                }
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

#[derive(Clone, PartialEq, Eq)]
enum CompareKey {
    Whole,
    Field(String),
}

#[derive(Clone)]
struct ComparatorPlan {
    elem_ty: Type,
    key: CompareKey,
}

#[derive(Clone)]
struct ArrayBinding {
    elem_ty: Type,
    len: u64,
}

fn rewrite_body(
    body: &mut [IndentStmt],
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) {
    let mut arrays = BTreeMap::new();
    let mut ints = BTreeMap::new();

    for indent in body.iter_mut() {
        if let Some((name, array)) = array_decl(&indent.stmt) {
            arrays.insert(name, array);
        }
        if let Some((name, value)) = integer_decl(&indent.stmt, &ints) {
            ints.insert(name, value);
        }

        if let Some(replacement) =
            replacement_stmt(&indent.stmt, &arrays, &ints, records, comparators)
        {
            indent.stmt = replacement;
            continue;
        }

        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            rewrite_body(body, records, comparators);
        });
    }
}

fn replacement_stmt(
    stmt: &Stmt,
    arrays: &BTreeMap<String, ArrayBinding>,
    ints: &BTreeMap<String, i128>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Stmt> {
    match stmt {
        Stmt::Expr(expr) => {
            qsort_replacement(expr, arrays, ints, records, comparators).map(Stmt::Expr)
        }
        Stmt::Unsafe { body } => body
            .tail
            .as_deref()
            .and_then(|tail| qsort_replacement(tail, arrays, ints, records, comparators))
            .map(Stmt::Expr),
        Stmt::Let {
            name,
            mutable,
            ty,
            init: Some(init),
        } => bsearch_replacement(init, arrays, ints, records, comparators).map(|expr| Stmt::Let {
            name: name.clone(),
            mutable: *mutable,
            ty: ty.clone(),
            init: Some(expr),
        }),
        _ => None,
    }
}

fn qsort_replacement(
    expr: &Expr,
    arrays: &BTreeMap<String, ArrayBinding>,
    ints: &BTreeMap<String, i128>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Expr> {
    let call = unsafe_call(expr)?;
    let (name, args) = direct_call(call)?;
    if name != "qsort" || args.len() != 4 {
        return None;
    }
    let array_name = array_pointer_arg(&args[0], true)?;
    let array = arrays.get(&array_name)?;
    let len = integer_value(&args[1], ints)?;
    if len != i128::from(array.len) {
        return None;
    }
    let elem_size = integer_value(&args[2], ints)?;
    if elem_size != i128::from(type_size(&array.elem_ty, records)?) {
        return None;
    }
    let comparator = comparators.get(comparator_arg(&args[3])?)?;
    if !same_type(&array.elem_ty, &comparator.elem_ty) {
        return None;
    }

    Some(Expr::MethodCall {
        recv: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::Var(array_name.into())),
            method: "as_mut_slice".into(),
            args: Vec::new(),
        }),
        method: "sort_by".into(),
        args: vec![sort_closure(&comparator.key)],
    })
}

fn bsearch_replacement(
    expr: &Expr,
    arrays: &BTreeMap<String, ArrayBinding>,
    ints: &BTreeMap<String, i128>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Expr> {
    let call = unsafe_call(expr)?;
    let (name, args) = direct_call(call)?;
    if name != "bsearch" || args.len() != 5 {
        return None;
    }
    let key_name = key_pointer_arg(&args[0])?;
    let array_name = array_pointer_arg(&args[1], true)?;
    let array = arrays.get(&array_name)?;
    let len = integer_value(&args[2], ints)?;
    if len != i128::from(array.len) {
        return None;
    }
    let elem_size = integer_value(&args[3], ints)?;
    if elem_size != i128::from(type_size(&array.elem_ty, records)?) {
        return None;
    }
    let comparator = comparators.get(comparator_arg(&args[4])?)?;
    if !same_type(&array.elem_ty, &comparator.elem_ty) {
        return None;
    }

    Some(Expr::Cast {
        expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(array_name.clone().into())),
                    method: "as_slice".into(),
                    args: Vec::new(),
                }),
                method: "binary_search_by".into(),
                args: vec![search_closure(&comparator.key, &key_name)],
            }),
            method: "map_or".into(),
            args: vec![
                Expr::Value(RustValue::NullPtr),
                Expr::Closure {
                    params: vec![Ident::from("__slate_i")],
                    body: Box::new(Expr::Cast {
                        expr: Box::new(Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(Expr::Index {
                                base: Box::new(Expr::Var(array_name.into())),
                                index: Box::new(Expr::Var("__slate_i".into())),
                            }),
                        }),
                        ty: Type::parse("*mut core::ffi::c_void"),
                    }),
                },
            ],
        }),
        ty: Type::parse("*mut core::ffi::c_void"),
    })
}

fn sort_closure(key: &CompareKey) -> Expr {
    Expr::Closure {
        params: vec![Ident::from("__slate_a"), Ident::from("__slate_b")],
        body: Box::new(cmp_expr(
            projected_var("__slate_a", key),
            projected_var("__slate_b", key),
            matches!(key, CompareKey::Field(_)),
        )),
    }
}

fn search_closure(key: &CompareKey, key_name: &str) -> Expr {
    Expr::Closure {
        params: vec![Ident::from("__slate_probe")],
        body: Box::new(cmp_expr(
            projected_var("__slate_probe", key),
            projected_var(key_name, key),
            true,
        )),
    }
}

fn cmp_expr(lhs: Expr, rhs: Expr, rhs_ref: bool) -> Expr {
    Expr::MethodCall {
        recv: Box::new(lhs),
        method: "cmp".into(),
        args: vec![if rhs_ref {
            Expr::Ref {
                mutable: false,
                expr: Box::new(rhs),
            }
        } else {
            rhs
        }],
    }
}

fn projected_var(name: &str, key: &CompareKey) -> Expr {
    let base = Expr::Var(name.into());
    match key {
        CompareKey::Whole => base,
        CompareKey::Field(field) => Expr::Field {
            base: Box::new(base),
            field: field.clone(),
        },
    }
}

fn comparator_plans(program: &Program) -> BTreeMap<String, ComparatorPlan> {
    program
        .items
        .iter()
        .filter_map(|item| {
            let Item::Fn(f) = item else {
                return None;
            };
            comparator_plan(f).map(|plan| (f.name.clone(), plan))
        })
        .collect()
}

fn comparator_plan(f: &FnDef) -> Option<ComparatorPlan> {
    if f.params.len() != 2 {
        return None;
    }
    let left_param = f.params[0].name.as_str();
    let right_param = f.params[1].name.as_str();
    let mut aliases = BTreeMap::new();
    let mut values = BTreeMap::new();
    for indent in &f.body {
        match &indent.stmt {
            Stmt::Assign { target, value } => {
                let Expr::Var(name) = target else {
                    continue;
                };
                if let Some(ptr) = pointer_source(value, &aliases) {
                    aliases.insert(name.as_str().to_string(), ptr);
                }
            }
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                if let Some(ptr) = pointer_source(init, &aliases) {
                    aliases.insert(name.as_str().to_string(), ptr);
                } else if let Some(source) = compare_source(init, &aliases) {
                    values.insert(name.clone(), source);
                }
            }
            Stmt::Return(Some(Expr::Binary {
                op: BinOp::Sub,
                lhs,
                rhs,
            })) => {
                let Expr::Var(lhs) = &**lhs else {
                    return None;
                };
                let Expr::Var(rhs) = &**rhs else {
                    return None;
                };
                let left = values.get(lhs.as_str())?;
                let right = values.get(rhs.as_str())?;
                if left.param != left_param || right.param != right_param {
                    return None;
                }
                if !same_type(&left.elem_ty, &right.elem_ty) || left.key != right.key {
                    return None;
                }
                return Some(ComparatorPlan {
                    elem_ty: left.elem_ty.clone(),
                    key: left.key.clone(),
                });
            }
            _ => {}
        }
    }
    None
}

#[derive(Clone)]
struct CompareSource {
    param: String,
    elem_ty: Type,
    key: CompareKey,
}

#[derive(Clone)]
struct PointerSource {
    param: String,
    elem_ty: Type,
}

fn compare_source(expr: &Expr, aliases: &BTreeMap<String, PointerSource>) -> Option<CompareSource> {
    let expr = unsafe_tail(expr).unwrap_or(expr);
    match expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => {
            let ptr = pointer_source(expr, aliases)?;
            Some(CompareSource {
                param: ptr.param,
                elem_ty: ptr.elem_ty,
                key: CompareKey::Whole,
            })
        }
        Expr::Field { base, field } => {
            let Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } = strip_casts(base)
            else {
                return None;
            };
            let ptr = pointer_source(expr, aliases)?;
            Some(CompareSource {
                param: ptr.param,
                elem_ty: ptr.elem_ty,
                key: CompareKey::Field(field.clone()),
            })
        }
        _ => None,
    }
}

fn pointer_source(expr: &Expr, aliases: &BTreeMap<String, PointerSource>) -> Option<PointerSource> {
    match expr {
        Expr::Cast { expr, ty } => {
            let Type::Ptr { inner, .. } = ty else {
                return pointer_source(expr, aliases);
            };
            let Expr::Var(name) = strip_casts(expr) else {
                return None;
            };
            Some(PointerSource {
                param: name.as_str().to_string(),
                elem_ty: (**inner).clone(),
            })
        }
        _ => match strip_casts(expr) {
            Expr::Var(name) => aliases.get(name.as_str()).cloned(),
            _ => None,
        },
    }
}

fn unsafe_call(expr: &Expr) -> Option<&Expr> {
    Some(unsafe_tail(expr).unwrap_or(expr))
}

fn unsafe_tail(expr: &Expr) -> Option<&Expr> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    block.tail.as_deref()
}

fn direct_call(expr: &Expr) -> Option<(&str, &[Expr])> {
    let Expr::Call { func, args } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    Some((name.as_str(), args.as_slice()))
}

fn array_pointer_arg(expr: &Expr, require_mut: bool) -> Option<String> {
    let expr = strip_casts(expr);
    match expr {
        Expr::ArrayPtr { array, mutable } if *mutable || !require_mut => {
            let Expr::Var(name) = &**array else {
                return None;
            };
            Some(name.as_str().to_string())
        }
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && method == if require_mut { "as_mut_ptr" } else { "as_ptr" } =>
        {
            let Expr::Var(name) = &**recv else {
                return None;
            };
            Some(name.as_str().to_string())
        }
        _ => None,
    }
}

fn key_pointer_arg(expr: &Expr) -> Option<String> {
    match strip_casts(expr) {
        Expr::AddrOf { expr, .. } => {
            let Expr::Var(name) = &**expr else {
                return None;
            };
            Some(name.as_str().to_string())
        }
        Expr::Ref { expr, .. } => {
            let Expr::Var(name) = &**expr else {
                return None;
            };
            Some(name.as_str().to_string())
        }
        _ => None,
    }
}

fn comparator_arg(expr: &Expr) -> Option<&str> {
    let Expr::Call { func, args } = strip_casts(expr) else {
        return None;
    };
    let Expr::Var(some) = &**func else {
        return None;
    };
    if some.as_str() != "Some" || args.len() != 1 {
        return None;
    }
    let Expr::Var(name) = &args[0] else {
        return None;
    };
    Some(name.as_str())
}

fn array_decl(stmt: &Stmt) -> Option<(String, ArrayBinding)> {
    let Stmt::Let {
        name,
        ty: Some(Type::Array { elem, len }),
        ..
    } = stmt
    else {
        return None;
    };
    Some((
        name.clone(),
        ArrayBinding {
            elem_ty: (**elem).clone(),
            len: *len,
        },
    ))
}

fn integer_decl(stmt: &Stmt, ints: &BTreeMap<String, i128>) -> Option<(String, i128)> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    integer_value(init, ints).map(|value| (name.clone(), value))
}

fn integer_value(expr: &Expr, ints: &BTreeMap<String, i128>) -> Option<i128> {
    match strip_casts(expr) {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Var(name) => ints.get(name.as_str()).copied(),
        _ => None,
    }
}

fn strip_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => strip_casts(expr),
        _ => expr,
    }
}

#[derive(Clone)]
struct Layout {
    size: u64,
    align: u64,
}

fn record_layouts(program: &Program) -> BTreeMap<String, Layout> {
    let records = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some((record.name.clone(), record)),
            _ => None,
        })
        .collect::<BTreeMap<_, _>>();
    records
        .keys()
        .filter_map(|name| record_layout(name, &records).map(|layout| (name.clone(), layout)))
        .collect()
}

fn record_layout(name: &str, records: &BTreeMap<String, &RecordDef>) -> Option<Layout> {
    let record = records.get(name)?;
    if record.is_union {
        let mut size = 0;
        let mut align = 1;
        for field in &record.fields {
            let layout = type_layout(&field.ty, &BTreeMap::new(), records)?;
            size = size.max(layout.size);
            align = align.max(layout.align);
        }
        return Some(Layout {
            size: align_to(size, align),
            align,
        });
    }
    let mut size = 0;
    let mut align = 1;
    for field in &record.fields {
        let layout = type_layout(&field.ty, &BTreeMap::new(), records)?;
        size = align_to(size, layout.align) + layout.size;
        align = align.max(layout.align);
    }
    Some(Layout {
        size: align_to(size, align),
        align,
    })
}

fn type_size(ty: &Type, records: &BTreeMap<String, Layout>) -> Option<u64> {
    type_layout(ty, records, &BTreeMap::new()).map(|layout| layout.size)
}

fn type_layout(
    ty: &Type,
    layouts: &BTreeMap<String, Layout>,
    records: &BTreeMap<String, &RecordDef>,
) -> Option<Layout> {
    match ty {
        Type::Prim(p) => prim_layout(*p),
        Type::Custom(name) => layouts
            .get(name)
            .cloned()
            .or_else(|| record_layout(name, records)),
        Type::Array { elem, len } => {
            let elem = type_layout(elem, layouts, records)?;
            Some(Layout {
                size: elem.size * *len,
                align: elem.align,
            })
        }
        _ => None,
    }
}

fn prim_layout(p: crate::rust_ast::Prim) -> Option<Layout> {
    let size = match p {
        crate::rust_ast::Prim::Bool | crate::rust_ast::Prim::I8 | crate::rust_ast::Prim::U8 => 1,
        crate::rust_ast::Prim::I16 | crate::rust_ast::Prim::U16 => 2,
        crate::rust_ast::Prim::I32 | crate::rust_ast::Prim::U32 | crate::rust_ast::Prim::F32 => 4,
        crate::rust_ast::Prim::I64 | crate::rust_ast::Prim::U64 | crate::rust_ast::Prim::F64 => 8,
        crate::rust_ast::Prim::I128 | crate::rust_ast::Prim::U128 => 16,
        crate::rust_ast::Prim::Isize | crate::rust_ast::Prim::Usize => return None,
    };
    Some(Layout { size, align: size })
}

fn align_to(value: u64, align: u64) -> u64 {
    if align == 0 {
        return value;
    }
    value.div_ceil(align) * align
}

fn same_type(a: &Type, b: &Type) -> bool {
    crate::codegen::type_to_string(a) == crate::codegen::type_to_string(b)
}

fn direct_calls(program: &Program) -> BTreeSet<String> {
    let mut used = BTreeSet::new();
    for item in &program.items {
        if let Item::Fn(f) = item {
            collect_direct_calls(&f.body, &mut used);
        }
    }
    used
}

fn collect_direct_calls(body: &[IndentStmt], used: &mut BTreeSet<String>) {
    for indent in body {
        walk::stmt_expr_any(&indent.stmt, &mut |expr| {
            if let Some((name, _)) = direct_call(expr) {
                used.insert(name.to_string());
            }
            false
        });
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, ExternDecl, ExternFnDecl, Prim, Visibility};

    fn void_ptr() -> Type {
        Type::parse("*mut core::ffi::c_void")
    }

    fn qsort_decl() -> Item {
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name: "qsort".into(),
                params: Vec::new(),
                variadic: false,
                ret: None,
            })],
        }
    }

    fn cmp_int() -> Item {
        Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "cmp".into(),
            params: vec![
                param("a", "*mut core::ffi::c_void"),
                param("b", "*mut core::ffi::c_void"),
            ],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                IndentStmt {
                    depth: 1,
                    stmt: temp(
                        "lhs",
                        "i32",
                        Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(Expr::Unary {
                                op: UnaryOp::Deref,
                                expr: Box::new(Expr::Cast {
                                    expr: Box::new(var("a")),
                                    ty: Type::parse("*mut i32"),
                                }),
                            })),
                        })),
                    ),
                },
                IndentStmt {
                    depth: 1,
                    stmt: temp(
                        "rhs",
                        "i32",
                        Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(Expr::Unary {
                                op: UnaryOp::Deref,
                                expr: Box::new(Expr::Cast {
                                    expr: Box::new(var("b")),
                                    ty: Type::parse("*mut i32"),
                                }),
                            })),
                        })),
                    ),
                },
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Return(Some(bin(BinOp::Sub, var("lhs"), var("rhs")))),
                },
            ],
        })
    }

    #[test]
    fn rewrites_qsort_call_to_sort_by() {
        let mut program = Program {
            items: vec![
                qsort_decl(),
                cmp_int(),
                Item::Fn(func(
                    Vec::new(),
                    None,
                    vec![
                        let_mut(
                            "items",
                            "[i32; 3]",
                            Expr::ArrayRepeat {
                                elem: Box::new(int(0)),
                                len: 3,
                            },
                        ),
                        temp("n", "u64", int(3)),
                        temp("sz", "u64", int(4)),
                        Stmt::Expr(Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(call(
                                "qsort",
                                vec![
                                    Expr::Cast {
                                        expr: Box::new(Expr::MethodCall {
                                            recv: Box::new(var("items")),
                                            method: "as_mut_ptr".into(),
                                            args: Vec::new(),
                                        }),
                                        ty: void_ptr(),
                                    },
                                    var("n"),
                                    var("sz"),
                                    call("Some", vec![var("cmp")]),
                                ],
                            ))),
                        }))),
                    ],
                )),
            ],
        };
        fixup(&mut program);
        prune_unused_externs(&mut program);
        let rust = program.emit();

        assert!(rust.contains(
            "items.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.cmp(__slate_b));"
        ));
        assert!(!rust.contains("fn qsort("));
    }

    #[test]
    fn leaves_size_mismatch_raw() {
        let mut program = Program {
            items: vec![
                qsort_decl(),
                cmp_int(),
                Item::Fn(func(
                    Vec::new(),
                    None,
                    vec![
                        let_mut(
                            "items",
                            "[i32; 3]",
                            Expr::ArrayRepeat {
                                elem: Box::new(int(0)),
                                len: 3,
                            },
                        ),
                        temp("n", "u64", int(3)),
                        temp("sz", "u64", int(8)),
                        Stmt::Expr(Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(call(
                                "qsort",
                                vec![
                                    Expr::Cast {
                                        expr: Box::new(Expr::MethodCall {
                                            recv: Box::new(var("items")),
                                            method: "as_mut_ptr".into(),
                                            args: Vec::new(),
                                        }),
                                        ty: void_ptr(),
                                    },
                                    var("n"),
                                    var("sz"),
                                    call("Some", vec![var("cmp")]),
                                ],
                            ))),
                        }))),
                    ],
                )),
            ],
        };
        fixup(&mut program);
        let rust = program.emit();

        assert!(rust.contains("qsort("));
        assert!(!rust.contains("sort_by("));
    }
}
