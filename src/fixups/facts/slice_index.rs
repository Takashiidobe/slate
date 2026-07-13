use std::collections::BTreeMap;

use crate::fixups::facts::{
    AstPath, BindingId, FixupFacts, FunctionId, IndexLowerBound, IndexUpperBound, PathSegment,
    PointerOffsetUnit, SliceIndexRangeFact, SlicePointerIndexFact, SlicePointerViewFact,
};
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.slice_pointer_views.clear();
    facts.slice_index_ranges.clear();
    facts.slice_pointer_indexes.clear();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        for (index, param) in f.params.iter().enumerate() {
            if let Some(elem_ty) = slice_elem_ty(&param.ty)
                && let Some(binding) = collector.facts.binding_by_param_index(function, index)
            {
                collector.slices.insert(
                    param.name.as_str().to_string(),
                    SliceBinding {
                        binding,
                        elem_ty: elem_ty.clone(),
                    },
                );
            }
        }
        collector.body(&f.body, &mut Vec::new());
    }
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a mut FixupFacts,
    slices: BTreeMap<String, SliceBinding>,
    len_aliases: BTreeMap<String, BindingId>,
    index_aliases: BTreeMap<BindingId, BindingId>,
    range_by_index: BTreeMap<BindingId, BindingId>,
    pointer_views: BTreeMap<BindingId, SlicePointerViewFact>,
}

#[derive(Clone)]
struct SliceBinding {
    binding: BindingId,
    elem_ty: Type,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a mut FixupFacts) -> Self {
        Self {
            function,
            facts,
            slices: BTreeMap::new(),
            len_aliases: BTreeMap::new(),
            index_aliases: BTreeMap::new(),
            range_by_index: BTreeMap::new(),
            pointer_views: BTreeMap::new(),
        }
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
                walk::nested_bodies_with_path(&indent.stmt, path, &mut |body, path| {
                    self.body(body, path);
                });
            });
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let {
                name,
                ty,
                init: Some(init),
                ..
            } => self.collect_let(name, ty.as_ref(), init, path),
            Stmt::Loop { .. } => {}
            Stmt::While { cond, .. } => self.collect_expr_offsets(cond, path),
            Stmt::Unsafe { .. } | Stmt::Block(_) => {}
            Stmt::LetIf {
                cond,
                then_value,
                else_value,
                ..
            } => {
                self.collect_expr_offsets(cond, path);
                self.collect_expr_offsets(then_value, path);
                self.collect_expr_offsets(else_value, path);
            }
            Stmt::If { cond, .. } => self.collect_expr_offsets(cond, path),
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.collect_expr_offsets(target, path);
                self.collect_expr_offsets(value, path);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.collect_expr_offsets(expr, path),
            Stmt::Match { expr, .. } => self.collect_expr_offsets(expr, path),
            Stmt::Scope { body } => self.collect_counted_loops(body, path, PathSegment::ScopeBody),
            Stmt::LabeledBlock { body, .. } => {
                self.collect_counted_loops(body, path, PathSegment::LabeledBody)
            }
            Stmt::Let { init: None, .. }
            | Stmt::Return(None)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }

    fn collect_let(
        &mut self,
        name: &str,
        ty: Option<&Type>,
        init: &Expr,
        path: &mut Vec<PathSegment>,
    ) {
        let ast_path = AstPath(path.clone());
        let Some(binding) = self
            .facts
            .binding_by_local_path(self.function, name, &ast_path)
        else {
            self.collect_expr_offsets(init, path);
            return;
        };

        if let Some(elem_ty) = ty.and_then(slice_elem_ty) {
            self.slices.insert(
                name.to_string(),
                SliceBinding {
                    binding,
                    elem_ty: elem_ty.clone(),
                },
            );
        }

        if let Some(slice) = self.slice_len_source(init) {
            self.len_aliases.insert(name.to_string(), slice);
        }

        if let Some((slice_name, mutable)) = slice_pointer_source(init)
            && let Some(slice) = self.slices.get(slice_name.as_str())
        {
            let fact = SlicePointerViewFact {
                function: self.function,
                pointer: binding,
                slice: slice.binding,
                mutable,
                elem_ty: slice.elem_ty.clone(),
                path: ast_path.clone(),
            };
            self.pointer_views.insert(binding, fact.clone());
            self.facts.slice_pointer_views.push(fact);
        }

        if let Some(source) = var_binding(init, self.function, self.facts) {
            self.index_aliases.insert(binding, source);
        }

        self.collect_expr_offsets(init, path);
    }

    fn slice_len_source(&self, expr: &Expr) -> Option<BindingId> {
        let Expr::MethodCall { recv, method, args } = peel_casts(expr) else {
            return None;
        };
        if method != "len" || !args.is_empty() {
            return None;
        }
        let Expr::Var(name) = &**recv else {
            return None;
        };
        self.slices.get(name.as_str()).map(|slice| slice.binding)
    }

    fn collect_counted_loops(
        &mut self,
        body: &[IndentStmt],
        parent_path: &mut [PathSegment],
        body_segment: PathSegment,
    ) {
        for (index, pair) in body.windows(2).enumerate() {
            let Stmt::Let {
                name: index_name,
                init: Some(init),
                ..
            } = &pair[0].stmt
            else {
                continue;
            };
            if !is_zero(init) {
                continue;
            }
            let Stmt::Loop {
                body: loop_body, ..
            } = &pair[1].stmt
            else {
                continue;
            };
            let Some(len) = canonical_loop_range(loop_body, index_name.as_str()) else {
                continue;
            };

            let mut index_path = parent_path.to_owned();
            index_path.push(body_segment.clone());
            index_path.push(PathSegment::Stmt(index));
            let index_path = AstPath(index_path);

            let mut loop_path = parent_path.to_owned();
            loop_path.push(body_segment.clone());
            loop_path.push(PathSegment::Stmt(index + 1));
            let loop_path = AstPath(loop_path);

            if let Some(index) =
                self.facts
                    .binding_by_local_path(self.function, index_name.as_str(), &index_path)
                && let Some(slice) = self.len_aliases.get(len.as_str()).copied()
            {
                self.range_by_index.insert(index, slice);
                self.facts.slice_index_ranges.push(SliceIndexRangeFact {
                    function: self.function,
                    index,
                    slice,
                    lower: IndexLowerBound::Zero,
                    upper: IndexUpperBound::SliceLen,
                    loop_path,
                });
            }
        }
    }

    fn collect_expr_offsets<T: OffsetWalk>(&mut self, node: &T, path: &mut Vec<PathSegment>) {
        node.offset_exprs(&mut |expr| {
            let Some((pointer, offset_index, unit)) =
                pointer_offset(expr, self.function, self.facts)
            else {
                return;
            };
            let Some(view) = self.pointer_views.get(&pointer) else {
                return;
            };
            let ranged_index = self
                .index_aliases
                .get(&offset_index)
                .copied()
                .unwrap_or(offset_index);
            if self.range_by_index.get(&ranged_index) != Some(&view.slice) {
                return;
            }
            self.facts
                .slice_pointer_indexes
                .push(SlicePointerIndexFact {
                    function: self.function,
                    pointer,
                    slice: view.slice,
                    offset_index,
                    ranged_index,
                    unit,
                    path: AstPath(path.clone()),
                });
        });
    }
}

trait OffsetWalk {
    fn offset_exprs(&self, f: &mut impl FnMut(&Expr));
}

impl OffsetWalk for Expr {
    fn offset_exprs(&self, f: &mut impl FnMut(&Expr)) {
        walk::exprs(self, f);
    }
}

impl OffsetWalk for Stmt {
    fn offset_exprs(&self, f: &mut impl FnMut(&Expr)) {
        walk::stmt_exprs(self, f);
    }
}

fn canonical_loop_range<'a>(body: &'a [IndentStmt], index_name: &str) -> Option<&'a Ident> {
    let first = body.first()?;
    let Stmt::If {
        cond, then_body, ..
    } = &first.stmt
    else {
        return None;
    };
    if !is_break_only(then_body) {
        return None;
    }
    let (index, len) = negated_less_than(cond)?;
    if index.as_str() != index_name {
        return None;
    }
    if !body
        .iter()
        .any(|indent| increments_by_one(&indent.stmt, index))
    {
        return None;
    }
    Some(len)
}

fn is_break_only(body: &[IndentStmt]) -> bool {
    matches!(
        body,
        [IndentStmt {
            stmt: Stmt::Break(None),
            ..
        }]
    )
}

fn negated_less_than(expr: &Expr) -> Option<(&Ident, &Ident)> {
    let Expr::Unary {
        op: crate::rust_ast::UnaryOp::Not,
        expr,
    } = expr
    else {
        return None;
    };
    let Expr::Binary {
        op: BinOp::Lt,
        lhs,
        rhs,
    } = &**expr
    else {
        return None;
    };
    let Expr::Var(index) = &**lhs else {
        return None;
    };
    let Expr::Var(len) = &**rhs else {
        return None;
    };
    Some((index, len))
}

fn increments_by_one(stmt: &Stmt, index: &Ident) -> bool {
    match stmt {
        Stmt::CompoundAssign {
            target,
            op: BinOp::Add,
            value,
        } => matches!(target, Expr::Var(name) if name == index) && is_one(value),
        Stmt::Assign { target, value } => {
            matches!(target, Expr::Var(name) if name == index) && adds_one(value, index)
        }
        _ => false,
    }
}

fn adds_one(expr: &Expr, index: &Ident) -> bool {
    let Expr::Binary {
        op: BinOp::Add,
        lhs,
        rhs,
    } = expr
    else {
        return false;
    };
    matches!(&**lhs, Expr::Var(name) if name == index) && is_one(rhs)
}

fn is_zero(expr: &Expr) -> bool {
    integer_value(expr) == Some(0)
}

fn is_one(expr: &Expr) -> bool {
    integer_value(expr) == Some(1)
}

fn integer_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

fn slice_elem_ty(ty: &Type) -> Option<&Type> {
    match ty {
        Type::Ref { inner, .. } => match &**inner {
            Type::Slice(elem) => Some(elem),
            _ => None,
        },
        Type::Slice(elem) => Some(elem),
        _ => None,
    }
}

fn slice_pointer_source(expr: &Expr) -> Option<(&Ident, bool)> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if !args.is_empty() {
        return None;
    }
    let mutable = match method.as_str() {
        "as_ptr" => false,
        "as_mut_ptr" => true,
        _ => return None,
    };
    let Expr::Var(name) = &**recv else {
        return None;
    };
    Some((name, mutable))
}

fn pointer_offset(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
) -> Option<(BindingId, BindingId, PointerOffsetUnit)> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if args.len() != 1 {
        return None;
    }
    let unit = match method.as_str() {
        "offset" => PointerOffsetUnit::Elements,
        "byte_offset" => PointerOffsetUnit::Bytes,
        _ => return None,
    };
    let pointer = var_binding(recv, function, facts)?;
    let offset_index = var_binding(peel_casts(&args[0]), function, facts)?;
    Some((pointer, offset_index, unit))
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn var_binding(expr: &Expr, function: FunctionId, facts: &FixupFacts) -> Option<BindingId> {
    let Expr::Var(name) = expr else {
        return None;
    };
    facts
        .bindings
        .iter()
        .find(|binding| binding.function == function && binding.name == name.as_str())
        .map(|binding| binding.id)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::facts::ptr_len;
    use crate::fixups::rewrite;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program, UnaryOp};

    fn analyze_collect(program: &Program) -> FixupFacts {
        let analyzed = facts::analyze(program.clone());
        let mut facts = analyzed.facts;
        collect_facts(program, &mut facts);
        facts
    }

    fn slice_param(name: &str) -> crate::rust_ast::FnParam {
        crate::rust_ast::FnParam {
            name: name.into(),
            mutable: true,
            ty: Type::parse("&mut [i32]"),
        }
    }

    fn as_mut_ptr(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(name)),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
        }
    }

    fn len_call(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(name)),
            method: "len".into(),
            args: Vec::new(),
        }
    }

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn offset(ptr: &str, index: &str, method: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(ptr)),
            method: method.into(),
            args: vec![cast(cast(var(index), "i64"), "isize")],
        }
    }

    fn counted_scope(offset_method: &str) -> Stmt {
        Stmt::Scope {
            body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: let_mut("i", "i32", int(0)),
                },
                IndentStmt {
                    depth: 2,
                    stmt: Stmt::Loop {
                        label: None,
                        body: vec![
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::If {
                                    cond: Expr::Unary {
                                        op: UnaryOp::Not,
                                        expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                    },
                                    then_body: vec![IndentStmt {
                                        depth: 4,
                                        stmt: Stmt::Break(None),
                                    }],
                                    else_body: Vec::new(),
                                },
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: temp("_idx", "i32", var("i")),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: temp("_ptr", "*mut i32", as_mut_ptr("items")),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::Expr(offset("_ptr", "_idx", offset_method)),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::CompoundAssign {
                                    target: var("i"),
                                    op: BinOp::Add,
                                    value: int(1),
                                },
                            },
                        ],
                    },
                },
            ],
        }
    }

    fn offset_body(ptr_source: Expr, offset_method: &str) -> Vec<Stmt> {
        vec![
            Stmt::Scope {
                body: vec![
                    IndentStmt {
                        depth: 2,
                        stmt: let_mut("i", "i32", int(0)),
                    },
                    IndentStmt {
                        depth: 2,
                        stmt: Stmt::Loop {
                            label: None,
                            body: vec![
                                IndentStmt {
                                    depth: 3,
                                    stmt: Stmt::If {
                                        cond: Expr::Unary {
                                            op: UnaryOp::Not,
                                            expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                        },
                                        then_body: vec![IndentStmt {
                                            depth: 4,
                                            stmt: Stmt::Break(None),
                                        }],
                                        else_body: Vec::new(),
                                    },
                                },
                                IndentStmt {
                                    depth: 3,
                                    stmt: temp("_idx", "i32", var("i")),
                                },
                                IndentStmt {
                                    depth: 3,
                                    stmt: Stmt::Let {
                                        name: "_ptr".into(),
                                        mutable: false,
                                        ty: Some(Type::parse("*mut i32")),
                                        init: Some(ptr_source),
                                    },
                                },
                                IndentStmt {
                                    depth: 3,
                                    stmt: Stmt::Expr(offset("_ptr", "_idx", offset_method)),
                                },
                                IndentStmt {
                                    depth: 3,
                                    stmt: Stmt::CompoundAssign {
                                        target: var("i"),
                                        op: BinOp::Add,
                                        value: int(1),
                                    },
                                },
                            ],
                        },
                    },
                ],
            },
            Stmt::Return(None),
        ]
    }

    #[test]
    fn records_slice_pointer_range_and_element_offset_facts() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![
                temp("len", "i32", cast(len_call("items"), "i32")),
                counted_scope("offset"),
            ],
        );
        f.name = "sum".into();
        let program = Program {
            items: vec![Item::Fn(f)],
        };

        let facts = analyze_collect(&program);

        assert_eq!(facts.slice_pointer_views.len(), 1);
        assert_eq!(facts.slice_index_ranges.len(), 1);
        assert_eq!(facts.slice_pointer_indexes.len(), 1);
        assert_eq!(
            facts.slice_pointer_indexes[0].unit,
            PointerOffsetUnit::Elements
        );
        assert_eq!(
            facts.slice_pointer_indexes[0].slice,
            facts.slice_pointer_views[0].slice
        );
        assert_eq!(
            facts.slice_pointer_indexes[0].ranged_index,
            facts.slice_index_ranges[0].index
        );
    }

    #[test]
    fn records_byte_offsets_separately_from_element_offsets() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![
                temp("len", "i32", cast(len_call("items"), "i32")),
                counted_scope("byte_offset"),
            ],
        );
        f.name = "sum".into();
        let program = Program {
            items: vec![Item::Fn(f)],
        };

        let facts = analyze_collect(&program);

        assert_eq!(facts.slice_pointer_indexes.len(), 1);
        assert_eq!(
            facts.slice_pointer_indexes[0].unit,
            PointerOffsetUnit::Bytes
        );
    }

    #[test]
    fn ignores_offsets_without_matching_slice_length_range() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![temp("n", "i32", int(4)), counted_scope("offset")],
        );
        f.name = "sum".into();
        let program = Program {
            items: vec![Item::Fn(f)],
        };

        let facts = analyze_collect(&program);

        assert_eq!(facts.slice_pointer_views.len(), 1);
        assert!(facts.slice_index_ranges.is_empty());
        assert!(facts.slice_pointer_indexes.is_empty());
    }

    #[test]
    fn records_facts_after_ptr_len_lifts_function_to_slice_param() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                let_mut(
                    "values",
                    "[i32; 4]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 4,
                    },
                ),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("values")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(4),
                    ],
                )),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    offset_body(var("items"), "offset"),
                )),
                Item::Fn(main),
            ],
        };
        let analyzed = facts::analyze(program.clone());
        let mut ptr_facts = analyzed.facts;
        ptr_len::collect_facts(&program, &mut ptr_facts);
        rewrite::ptr_len::fixup(&mut program, &ptr_facts);
        let analyzed = facts::analyze(program);
        let mut facts = analyzed.facts;

        collect_facts(&analyzed.program, &mut facts);

        assert_eq!(facts.slice_pointer_views.len(), 1);
        assert_eq!(facts.slice_index_ranges.len(), 1);
        assert_eq!(facts.slice_pointer_indexes.len(), 1);
        assert_eq!(
            facts.slice_pointer_indexes[0].unit,
            PointerOffsetUnit::Elements
        );
    }
}
