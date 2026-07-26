use std::collections::BTreeMap;

use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{
    BinOp, Expr, FnDef, Ident, IndentStmt, Item, Program, RecordDef, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(program: &mut Program) {
    let mut logger = crate::fixups::trace::NoopLogger;
    SortSearch::new(&mut logger).fixup(program);
}

pub(in crate::fixups) struct SortSearch<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> SortSearch<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) {
        let before = self.logger.is_enabled().then(|| program.emit());
        fixup_impl(program);
        if let Some(before) = before {
            let after = program.emit();
            if before != after {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::SortSearch,
                    kind: "rewrite_sort_search_calls".into(),
                    location: TraceLocation::default(),
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", after.trim_end())],
                    facts: Vec::new(),
                });
            }
        }
    }
}

fn fixup_impl(program: &mut Program) {
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
    let mut layout_sizes = BTreeMap::new();

    for indent in body.iter_mut() {
        if let Some((name, array)) = array_decl(&indent.stmt) {
            arrays.insert(name, array);
        }
        if let Some((name, value)) = integer_decl(&indent.stmt, &ints) {
            ints.insert(name, value);
        }
        if let Some((name, ty)) = layout_size_decl(&indent.stmt) {
            layout_sizes.insert(name, ty);
        }

        if let Some(replacement) = replacement_stmt(
            &indent.stmt,
            &arrays,
            &ints,
            &layout_sizes,
            records,
            comparators,
        ) {
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
    layout_sizes: &BTreeMap<String, Type>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Stmt> {
    match stmt {
        Stmt::Expr(expr) => {
            qsort_replacement(expr, arrays, ints, layout_sizes, records, comparators)
                .map(Stmt::Expr)
        }
        Stmt::Unsafe { body } => body
            .tail
            .as_deref()
            .and_then(|tail| {
                qsort_replacement(tail, arrays, ints, layout_sizes, records, comparators)
            })
            .map(Stmt::Expr),
        Stmt::Let {
            name,
            mutable,
            ty,
            init: Some(init),
        } => bsearch_replacement(init, arrays, ints, layout_sizes, records, comparators).map(
            |expr| Stmt::Let {
                name: name.clone(),
                mutable: *mutable,
                ty: ty.clone(),
                init: Some(expr),
            },
        ),
        _ => None,
    }
}

fn qsort_replacement(
    expr: &Expr,
    arrays: &BTreeMap<String, ArrayBinding>,
    ints: &BTreeMap<String, i128>,
    layout_sizes: &BTreeMap<String, Type>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Expr> {
    let call = unsafe_call(expr)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::Qsort) || args.len() != 4 {
        return None;
    }
    let array_name = array_pointer_arg(&args[0], true)?;
    let array = arrays.get(&array_name)?;
    let len = integer_value(&args[1], ints)?;
    if len != i128::from(array.len) {
        return None;
    }
    if !elem_size_matches(&args[2], &array.elem_ty, ints, layout_sizes, records) {
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
    layout_sizes: &BTreeMap<String, Type>,
    records: &BTreeMap<String, Layout>,
    comparators: &BTreeMap<String, ComparatorPlan>,
) -> Option<Expr> {
    let call = unsafe_call(expr)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::Bsearch) || args.len() != 5 {
        return None;
    }
    let key_name = key_pointer_arg(&args[0])?;
    let array_name = array_pointer_arg(&args[1], true)?;
    let array = arrays.get(&array_name)?;
    let len = integer_value(&args[2], ints)?;
    if len != i128::from(array.len) {
        return None;
    }
    if !elem_size_matches(&args[3], &array.elem_ty, ints, layout_sizes, records) {
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
                let left = returned_compare_source(lhs, &aliases, &values)?;
                let right = returned_compare_source(rhs, &aliases, &values)?;
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

fn returned_compare_source(
    expr: &Expr,
    aliases: &BTreeMap<String, PointerSource>,
    values: &BTreeMap<String, CompareSource>,
) -> Option<CompareSource> {
    match expr {
        Expr::Var(name) => values.get(name.as_str()).cloned(),
        _ => compare_source(expr, aliases),
    }
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
    let Expr::Call { func, args, .. } = expr else {
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
    let Expr::Call { func, args, .. } = strip_casts(expr) else {
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
        name, ty: Some(ty), ..
    } = stmt
    else {
        return None;
    };
    let Type::Array { elem, len } = ty.peel_aligned() else {
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

fn layout_size_decl(stmt: &Stmt) -> Option<(String, Type)> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    layout_size_type(init).map(|ty| (name.clone(), ty))
}

fn integer_value(expr: &Expr, ints: &BTreeMap<String, i128>) -> Option<i128> {
    match strip_casts(expr) {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Var(name) => ints.get(name.as_str()).copied(),
        _ => None,
    }
}

fn elem_size_matches(
    expr: &Expr,
    elem_ty: &Type,
    ints: &BTreeMap<String, i128>,
    layout_sizes: &BTreeMap<String, Type>,
    records: &BTreeMap<String, Layout>,
) -> bool {
    if let Some(size) = integer_value(expr, ints) {
        return Some(size) == type_size(elem_ty, records).map(i128::from);
    }
    layout_size_value(expr, layout_sizes).is_some_and(|ty| same_type(&ty, elem_ty))
}

fn layout_size_value(expr: &Expr, layout_sizes: &BTreeMap<String, Type>) -> Option<Type> {
    match strip_casts(expr) {
        Expr::Var(name) => layout_sizes.get(name.as_str()).cloned(),
        expr => layout_size_type(expr),
    }
}

fn layout_size_type(expr: &Expr) -> Option<Type> {
    let (name, args) = direct_call(strip_casts(expr))?;
    if !args.is_empty() {
        return None;
    }
    let ty = name
        .strip_prefix("std::mem::size_of::<")?
        .strip_suffix('>')?;
    Some(Type::parse(ty))
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
        crate::rust_ast::Prim::I128 | crate::rust_ast::Prim::U128 | crate::rust_ast::Prim::F128 => {
            16
        }
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
                identity: crate::function_identity::FunctionIdentity::Unknown,
                name: "qsort".into(),
                params: Vec::new(),
                variadic: false,
                ret: None,
                returns_nonnull: false,
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
            returns_nonnull: false,
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
        let analyzed = crate::fixups::facts::analyze(program.clone());
        program = analyzed.program;
        crate::fixups::rewrite::prune_unused_externs::fixup(&mut program, &analyzed.facts);
        let rust = program.emit();

        assert!(rust.contains(
            "items.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.cmp(__slate_b));"
        ));
        assert!(!rust.contains("fn qsort("));
    }

    #[test]
    fn rewrites_qsort_call_with_size_of_type() {
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
                        temp(
                            "sz",
                            "u64",
                            Expr::Cast {
                                expr: Box::new(call("std::mem::size_of::<i32>", Vec::new())),
                                ty: Type::Prim(Prim::U64),
                            },
                        ),
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
        let analyzed = crate::fixups::facts::analyze(program.clone());
        program = analyzed.program;
        crate::fixups::rewrite::prune_unused_externs::fixup(&mut program, &analyzed.facts);
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
