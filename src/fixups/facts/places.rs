use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, AtomicPlaceAccess, FixupFacts, FunctionId, PathSegment, PlaceAccess, PlaceFact,
    PlaceKind, PlaceProjection, PlaceRoot, Site, VolatileAccess,
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
            Stmt::For { iter, body, .. } => {
                self.expr(iter, path);
                walk::with_path_segment(path, PathSegment::ForBody, |path| self.body(body, path));
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
            Stmt::InlineAsm(_) => {}
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
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(start, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(end, path));
            }
            Expr::Call { func, args, .. } => {
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
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
            }
            Expr::VecRepeat { elem, len } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(len, path));
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
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                self.record_atomic(AtomicPlaceAccess::Read, path);
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
            }
            Expr::AtomicStore { place, value, .. } => {
                self.record_atomic(AtomicPlaceAccess::Write, path);
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicFetch { place, value, .. } | Expr::AtomicSwap { place, value, .. } => {
                self.record_atomic(AtomicPlaceAccess::ReadWrite, path);
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicNew { value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(value, path));
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.record_atomic(AtomicPlaceAccess::ReadWrite, path);
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
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
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
            access,
            readable: readable(access, &kind),
            assignable: assignable(access, &kind),
            ordinary_slot,
            kind,
        });
    }

    fn record_volatile(&mut self, access: VolatileAccess, path: &[PathSegment]) {
        self.places.push(PlaceFact {
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
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
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
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
