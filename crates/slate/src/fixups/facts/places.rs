use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, AtomicPlaceAccess, FixupFacts, FunctionId, PathSegment, PlaceAccess, PlaceFact,
    PlaceKind, PlaceProjection, PlaceRoot, VolatileAccess,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, Stmt, UnaryOp};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.places.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector {
            function,
            places: Vec::new(),
        };
        collector.body(&f.body, &mut Vec::new());
        all.extend(collector.places);
    }
    facts.places = all;
}

pub(super) fn is_simple_local_slot(expr: &Expr) -> bool {
    matches!(classify_place(expr), PlaceKind::Local { .. })
}

struct Collector {
    function: FunctionId,
    places: Vec<PlaceFact>,
}

impl Collector {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.body(&block.stmts, path);
        if let Some(tail) = &block.tail {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| {
                self.expr(tail, path);
            });
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
            }
            Stmt::LetIf {
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path);
                    self.expr(then_value, path);
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path);
                    self.expr(else_value, path);
                });
            }
            Stmt::Assign { target, value } => {
                self.place(target, PlaceAccess::Write, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path);
                });
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.place(target, PlaceAccess::ReadWrite, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path);
                });
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| self.body(then_body, path));
                walk::with_path_segment(path, PathSegment::Else, |path| self.body(else_body, path));
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| self.body(body, path));
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| self.body(body, path));
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path)
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::While { cond, body } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr, path);
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.body(&arm.body, path)
                    });
                }
            }
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        if is_place_like(expr) {
            self.record(expr, PlaceAccess::Read, path);
        }
        match expr {
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::Var(_)
            | Expr::Path(_)
            | Expr::Todo(_)
            | Expr::AtomicFence { .. } => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
            }
            Expr::Binary { lhs, rhs, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(lhs, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(rhs, path));
            }
            Expr::Call { func, args } => {
                if let Some(access) = volatile_access(expr) {
                    self.record_volatile(access, path);
                }
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(func, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(recv, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
            }
            Expr::Index { base, index } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::ArrayLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
            }
            Expr::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(&arm.value, path)
                    });
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(cond, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(then_expr, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(else_expr, path)
                });
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
                self.record_atomic(AtomicPlaceAccess::Read, path);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
            }
            Expr::AtomicStore { ptr, value, .. } => {
                self.record_atomic(AtomicPlaceAccess::Write, path);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicFetch { ptr, value, .. } | Expr::AtomicSwap { ptr, value, .. } => {
                self.record_atomic(AtomicPlaceAccess::ReadWrite, path);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                self.record_atomic(AtomicPlaceAccess::ReadWrite, path);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(expected, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(desired, path)
                });
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
            Expr::WriteBytes { dst, val, count } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(val, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
        }
    }

    fn place(&mut self, expr: &Expr, access: PlaceAccess, path: &mut Vec<PathSegment>) {
        self.record(expr, access, path);
        match expr {
            Expr::Index { base, index } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.place(base, access, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.place(base, access, path)
                });
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
            }
            Expr::Cast { expr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.place(expr, access, path)
                });
            }
            _ => {}
        }
    }

    fn record(&mut self, expr: &Expr, access: PlaceAccess, path: &[PathSegment]) {
        let kind = classify_place(expr);
        let ordinary_slot = ordinary_slot(&kind);
        self.places.push(PlaceFact {
            function: self.function,
            path: AstPath(path.to_vec()),
            access,
            readable: readable(access, &kind),
            assignable: assignable(access, &kind),
            ordinary_slot,
            kind,
        });
    }

    fn record_volatile(&mut self, access: VolatileAccess, path: &[PathSegment]) {
        self.places.push(PlaceFact {
            function: self.function,
            path: AstPath(path.to_vec()),
            access: match access {
                VolatileAccess::Read => PlaceAccess::Read,
                VolatileAccess::Write => PlaceAccess::Write,
            },
            kind: PlaceKind::Volatile { access },
            readable: matches!(access, VolatileAccess::Read),
            assignable: matches!(access, VolatileAccess::Write),
            ordinary_slot: false,
        });
    }

    fn record_atomic(&mut self, access: AtomicPlaceAccess, path: &[PathSegment]) {
        self.places.push(PlaceFact {
            function: self.function,
            path: AstPath(path.to_vec()),
            access: match access {
                AtomicPlaceAccess::Read => PlaceAccess::Read,
                AtomicPlaceAccess::Write => PlaceAccess::Write,
                AtomicPlaceAccess::ReadWrite => PlaceAccess::ReadWrite,
            },
            kind: PlaceKind::Atomic { access },
            readable: !matches!(access, AtomicPlaceAccess::Write),
            assignable: !matches!(access, AtomicPlaceAccess::Read),
            ordinary_slot: false,
        });
    }
}

fn is_place_like(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Var(_)
            | Expr::Field { .. }
            | Expr::TupleField { .. }
            | Expr::Index { .. }
            | Expr::Unary {
                op: UnaryOp::Deref,
                ..
            }
            | Expr::Cast { .. }
    )
}

fn classify_place(expr: &Expr) -> PlaceKind {
    let mut projections = Vec::new();
    let root = collect_place(expr, &mut projections);
    projections.reverse();
    match (root, projections.is_empty()) {
        (PlaceRoot::Local { name }, true) => PlaceKind::Local { name },
        (root, false) => PlaceKind::Projection { root, projections },
        (PlaceRoot::Unsupported, true) => PlaceKind::Unsupported,
    }
}

fn collect_place(expr: &Expr, projections: &mut Vec<PlaceProjection>) -> PlaceRoot {
    match expr {
        Expr::Var(name) => PlaceRoot::Local {
            name: name.as_str().to_string(),
        },
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => {
            projections.push(PlaceProjection::Deref);
            collect_place(expr, projections)
        }
        Expr::Field { base, field } => {
            projections.push(PlaceProjection::Field(field.clone()));
            collect_place(base, projections)
        }
        Expr::TupleField { base, index } => {
            projections.push(PlaceProjection::TupleField(*index));
            collect_place(base, projections)
        }
        Expr::Index { base, .. } => {
            projections.push(PlaceProjection::Index);
            collect_place(base, projections)
        }
        Expr::Cast { expr, .. } => collect_place(expr, projections),
        _ => PlaceRoot::Unsupported,
    }
}

fn volatile_access(expr: &Expr) -> Option<VolatileAccess> {
    let Expr::Call { func, .. } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    match name.as_str() {
        "std::ptr::read_volatile" | "core::ptr::read_volatile" => Some(VolatileAccess::Read),
        "std::ptr::write_volatile" | "core::ptr::write_volatile" => Some(VolatileAccess::Write),
        _ => None,
    }
}

fn ordinary_slot(kind: &PlaceKind) -> bool {
    matches!(kind, PlaceKind::Local { .. })
}

fn readable(access: PlaceAccess, kind: &PlaceKind) -> bool {
    !matches!(kind, PlaceKind::Unsupported) && !matches!(access, PlaceAccess::Write)
}

fn assignable(access: PlaceAccess, kind: &PlaceKind) -> bool {
    !matches!(kind, PlaceKind::Unsupported) && !matches!(access, PlaceAccess::Read)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{AtomicOrdering, AtomicType, Expr, Item, Program, Stmt, Type};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(
                vec![param("p", "*mut i32"), param("items", "&mut [i32]")],
                None,
                stmts,
            ))],
        })
        .facts
    }

    fn place_at(facts: &facts::FixupFacts, path: AstPath) -> &PlaceFact {
        facts.places.iter().find(|fact| fact.path == path).unwrap()
    }

    #[test]
    fn records_plain_local_read_and_write_slots() {
        let facts = analyzed(vec![let_mut("x", "i32", int(0)), assign("x", var("x"))]);

        let write = place_at(&facts, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(
            write.kind,
            PlaceKind::Local {
                name: "x".to_string()
            }
        );
        assert_eq!(write.access, PlaceAccess::Write);
        assert!(!write.readable);
        assert!(write.assignable);
        assert!(write.ordinary_slot);

        let read = place_at(
            &facts,
            AstPath(vec![PathSegment::Stmt(1), PathSegment::Expr(1)]),
        );
        assert_eq!(read.access, PlaceAccess::Read);
        assert!(read.readable);
        assert!(!read.assignable);
        assert!(is_simple_local_slot(&var("x")));
    }

    #[test]
    fn records_field_tuple_and_index_projection_chains() {
        let facts = analyzed(vec![Stmt::Assign {
            target: Expr::TupleField {
                base: Box::new(Expr::Field {
                    base: Box::new(Expr::Index {
                        base: Box::new(var("items")),
                        index: Box::new(var("i")),
                    }),
                    field: "entry".into(),
                }),
                index: 0,
            },
            value: int(1),
        }]);

        let place = place_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(place.access, PlaceAccess::Write);
        assert!(!place.ordinary_slot);
        assert_eq!(
            place.kind,
            PlaceKind::Projection {
                root: PlaceRoot::Local {
                    name: "items".to_string()
                },
                projections: vec![
                    PlaceProjection::Index,
                    PlaceProjection::Field("entry".to_string()),
                    PlaceProjection::TupleField(0),
                ],
            }
        );

        let index_read = place_at(
            &facts,
            AstPath(vec![
                PathSegment::Stmt(0),
                PathSegment::Expr(0),
                PathSegment::Expr(0),
                PathSegment::Expr(1),
            ]),
        );
        assert_eq!(
            index_read.kind,
            PlaceKind::Local {
                name: "i".to_string()
            }
        );
        assert_eq!(index_read.access, PlaceAccess::Read);
    }

    #[test]
    fn records_deref_projection_as_complex_lvalue() {
        let facts = analyzed(vec![Stmt::Assign {
            target: Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(var("p")),
            },
            value: int(1),
        }]);

        let place = place_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(
            place.kind,
            PlaceKind::Projection {
                root: PlaceRoot::Local {
                    name: "p".to_string()
                },
                projections: vec![PlaceProjection::Deref],
            }
        );
        assert!(place.assignable);
        assert!(!place.ordinary_slot);
    }

    #[test]
    fn records_compound_assignment_as_read_write_place() {
        let facts = analyzed(vec![Stmt::CompoundAssign {
            target: var("x"),
            op: crate::rust_ast::BinOp::Add,
            value: int(1),
        }]);

        let place = place_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(place.access, PlaceAccess::ReadWrite);
        assert!(place.readable);
        assert!(place.assignable);
    }

    #[test]
    fn volatile_and_atomic_places_are_not_ordinary_slots() {
        let facts = analyzed(vec![
            temp("v", "i32", call("std::ptr::read_volatile", vec![var("p")])),
            Stmt::Expr(call("std::ptr::write_volatile", vec![var("p"), int(1)])),
            Stmt::Expr(Expr::AtomicLoad {
                ty: AtomicType::I32,
                ptr: Box::new(var("p")),
                ordering: AtomicOrdering::SeqCst,
            }),
            Stmt::Expr(Expr::AtomicStore {
                ty: AtomicType::I32,
                ptr: Box::new(var("p")),
                value: Box::new(int(2)),
                ordering: AtomicOrdering::SeqCst,
            }),
        ]);

        let volatile_read = place_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(
            volatile_read.kind,
            PlaceKind::Volatile {
                access: VolatileAccess::Read
            }
        );
        assert!(!volatile_read.ordinary_slot);

        let volatile_write = place_at(&facts, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(
            volatile_write.kind,
            PlaceKind::Volatile {
                access: VolatileAccess::Write
            }
        );
        assert!(!volatile_write.ordinary_slot);

        let atomic_read = place_at(&facts, AstPath(vec![PathSegment::Stmt(2)]));
        assert_eq!(
            atomic_read.kind,
            PlaceKind::Atomic {
                access: AtomicPlaceAccess::Read
            }
        );
        assert!(!atomic_read.ordinary_slot);

        let atomic_write = place_at(&facts, AstPath(vec![PathSegment::Stmt(3)]));
        assert_eq!(
            atomic_write.kind,
            PlaceKind::Atomic {
                access: AtomicPlaceAccess::Write
            }
        );
        assert!(!atomic_write.ordinary_slot);
    }

    #[test]
    fn unsupported_complex_places_stay_non_assignable() {
        let expr = Expr::Call {
            func: Box::new(var("get_ptr")),
            args: vec![],
        };
        let kind = classify_place(&expr);

        assert_eq!(kind, PlaceKind::Unsupported);
        assert!(!ordinary_slot(&kind));
    }

    #[test]
    fn casts_do_not_create_a_projection_layer() {
        let expr = Expr::Cast {
            expr: Box::new(var("x")),
            ty: Type::parse("*mut i32"),
        };

        assert_eq!(
            classify_place(&expr),
            PlaceKind::Local {
                name: "x".to_string()
            }
        );
    }
}
