use std::collections::HashMap;

use std::collections::HashSet;

use crate::backend::rust_ast::{Block, Expr, Ident, IndentStmt, Label, Prim, Stmt, Type};

pub(super) fn simplify(stmts: Vec<Stmt>) -> Vec<Stmt> {
    let mut current = stmts;
    for _ in 0..16 {
        let before = current.clone();
        let integral = collect_integral_locals(&current);
        current = retarget_jumps(current);
        current = elide_unused_labels(current);
        current = map_bodies(current, &|body| clean_branches(body, &integral));
        current = map_bodies(current, &|body| loops_to_while(body, &integral));
        if current == before {
            break;
        }
    }
    current
}

#[derive(Clone, PartialEq, Eq)]
enum Cont {
    Continue(Label),
    Opaque(usize),
}

struct Frame {
    label: Option<Label>,
    is_loop: bool,
    exit: Cont,
    entry: Option<Cont>,
}

struct Retarget {
    next: usize,
    frames: Vec<Frame>,
}

fn to_indent(stmts: Vec<Stmt>) -> Vec<IndentStmt> {
    stmts
        .into_iter()
        .map(|stmt| IndentStmt { depth: 0, stmt })
        .collect()
}

fn without_last(body: &[IndentStmt]) -> Vec<IndentStmt> {
    body[..body.len() - 1].to_vec()
}

fn from_indent(stmts: &[IndentStmt]) -> Vec<Stmt> {
    stmts.iter().map(|s| s.stmt.clone()).collect()
}

impl Retarget {
    fn fresh(&mut self) -> Cont {
        self.next += 1;
        Cont::Opaque(self.next)
    }

    fn frame_named(&self, label: &Label) -> Option<&Frame> {
        self.frames.iter().find(|f| f.label.as_ref() == Some(label))
    }

    fn innermost_loop(&self) -> Option<usize> {
        self.frames.iter().rposition(|f| f.is_loop)
    }

    fn jump_to(&self, target: &Cont) -> Option<Stmt> {
        let innermost = self.innermost_loop();
        for (i, f) in self.frames.iter().enumerate().rev() {
            let unlabeled = Some(i) == innermost && !self.crosses_labeled_block(i);
            if f.is_loop && f.entry.as_ref() == Some(target) {
                if unlabeled {
                    return Some(Stmt::Continue(None));
                }
                if let Some(label) = &f.label {
                    return Some(Stmt::Continue(Some(label.clone())));
                }
            }
            if f.exit == *target {
                if unlabeled {
                    return Some(Stmt::Break(None));
                }
                if let Some(label) = &f.label {
                    return Some(Stmt::Break(Some(label.clone())));
                }
            }
        }
        None
    }

    fn crosses_labeled_block(&self, from: usize) -> bool {
        self.frames[from + 1..]
            .iter()
            .any(|f| !f.is_loop && f.label.is_some())
    }

    fn target_of(&self, stmt: &Stmt) -> Option<Cont> {
        match stmt {
            Stmt::Break(Some(l)) => self.frame_named(l).map(|f| f.exit.clone()),
            Stmt::Continue(Some(l)) => self.frame_named(l).and_then(|f| f.entry.clone()),
            Stmt::Break(None) => self.innermost_loop().map(|i| self.frames[i].exit.clone()),
            Stmt::Continue(None) => self
                .innermost_loop()
                .and_then(|i| self.frames[i].entry.clone()),
            _ => None,
        }
    }

    fn exits_list(&self, body: &[IndentStmt], cont: &Cont) -> bool {
        match body.last().map(|s| &s.stmt) {
            Some(stmt) => self.target_of(stmt).as_ref() == Some(cont),
            None => false,
        }
    }

    fn sink_tail(&self, body: &[Stmt], cont: &Cont) -> Option<Vec<Stmt>> {
        for (i, stmt) in body.iter().enumerate() {
            let tail = &body[i + 1..];
            if tail.is_empty() {
                break;
            }
            let sunk = match stmt {
                Stmt::If {
                    cond,
                    then_body,
                    else_body,
                } => {
                    let exits_then = self.exits_list(then_body, cont);
                    let exits_else = self.exits_list(else_body, cont);
                    if exits_then == exits_else {
                        continue;
                    }
                    let mut kept = if exits_then {
                        else_body.clone()
                    } else {
                        then_body.clone()
                    };
                    kept.extend(to_indent(tail.to_vec()));
                    let (then_body, else_body) = if exits_then {
                        (without_last(then_body), kept)
                    } else {
                        (kept, without_last(else_body))
                    };
                    Stmt::If {
                        cond: cond.clone(),
                        then_body,
                        else_body,
                    }
                }
                Stmt::Match { expr, arms } => {
                    let open: Vec<usize> = (0..arms.len())
                        .filter(|&a| !self.exits_list(&arms[a].body, cont))
                        .collect();
                    let [open] = open[..] else {
                        continue;
                    };
                    let arms = arms
                        .iter()
                        .enumerate()
                        .map(|(a, arm)| {
                            let arm_body = if a == open {
                                let mut body = arm.body.clone();
                                body.extend(to_indent(tail.to_vec()));
                                body
                            } else {
                                without_last(&arm.body)
                            };
                            crate::backend::rust_ast::MatchArm {
                                pattern: arm.pattern.clone(),
                                body: arm_body,
                            }
                        })
                        .collect();
                    Stmt::Match {
                        expr: expr.clone(),
                        arms,
                    }
                }
                _ => continue,
            };
            let mut out = body[..i].to_vec();
            out.push(sunk);
            return Some(out);
        }
        None
    }

    fn body(&mut self, stmts: Vec<Stmt>, cont: Cont) -> Vec<Stmt> {
        let mut stmts = stmts;
        while let Some(next) = self.sink_tail(&stmts, &cont) {
            stmts = next;
        }
        let len = stmts.len();
        let mut out = Vec::with_capacity(len);
        for (i, stmt) in stmts.into_iter().enumerate() {
            let after = if i + 1 == len {
                cont.clone()
            } else {
                self.fresh()
            };
            if let Some(target) = self.target_of(&stmt) {
                if target == after {
                    continue;
                }
                out.push(self.jump_to(&target).unwrap_or(stmt));
                continue;
            }
            out.push(self.stmt(stmt, after));
        }
        out
    }

    fn stmt(&mut self, stmt: Stmt, after: Cont) -> Stmt {
        match stmt {
            Stmt::LabeledBlock { label, body } => {
                self.frames.push(Frame {
                    label: Some(label.clone()),
                    is_loop: false,
                    exit: after.clone(),
                    entry: None,
                });
                let body = self.body(from_indent(&body), after);
                self.frames.pop();
                Stmt::LabeledBlock {
                    label,
                    body: to_indent(body),
                }
            }
            Stmt::Loop { label, body } => {
                let entry = match &label {
                    Some(l) => Cont::Continue(l.clone()),
                    None => self.fresh(),
                };
                self.frames.push(Frame {
                    label: label.clone(),
                    is_loop: true,
                    exit: after,
                    entry: Some(entry.clone()),
                });
                let body = self.body(from_indent(&body), entry);
                self.frames.pop();
                Stmt::Loop {
                    label,
                    body: to_indent(body),
                }
            }
            Stmt::While { cond, body } => {
                let entry = self.fresh();
                self.frames.push(Frame {
                    label: None,
                    is_loop: true,
                    exit: after,
                    entry: Some(entry.clone()),
                });
                let body = self.block(body, entry);
                self.frames.pop();
                Stmt::While { cond, body }
            }
            Stmt::For { pat, iter, body } => {
                let entry = self.fresh();
                self.frames.push(Frame {
                    label: None,
                    is_loop: true,
                    exit: after,
                    entry: Some(entry.clone()),
                });
                let body = self.body(from_indent(&body), entry);
                self.frames.pop();
                Stmt::For {
                    pat,
                    iter,
                    body: to_indent(body),
                }
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => Stmt::If {
                cond,
                then_body: to_indent(self.body(from_indent(&then_body), after.clone())),
                else_body: to_indent(self.body(from_indent(&else_body), after)),
            },
            Stmt::Match { expr, arms } => Stmt::Match {
                expr,
                arms: arms
                    .into_iter()
                    .map(|arm| crate::backend::rust_ast::MatchArm {
                        pattern: arm.pattern,
                        body: to_indent(self.body(from_indent(&arm.body), after.clone())),
                    })
                    .collect(),
            },
            Stmt::Scope { body } => Stmt::Scope {
                body: to_indent(self.body(from_indent(&body), after)),
            },
            Stmt::Unsafe { body } => Stmt::Unsafe {
                body: self.block(body, after),
            },
            Stmt::Block(body) => Stmt::Block(self.block(body, after)),
            other => other,
        }
    }

    fn block(&mut self, block: Block, after: Cont) -> Block {
        let cont = if block.tail.is_some() {
            self.fresh()
        } else {
            after
        };
        Block {
            stmts: to_indent(self.body(from_indent(&block.stmts), cont)),
            tail: block.tail,
        }
    }
}

fn retarget_jumps(stmts: Vec<Stmt>) -> Vec<Stmt> {
    let mut rt = Retarget {
        next: 0,
        frames: Vec::new(),
    };
    let root = rt.fresh();
    rt.body(stmts, root)
}

fn count_label_uses(stmts: &[Stmt], uses: &mut HashMap<Label, usize>) {
    for stmt in stmts {
        match stmt {
            Stmt::Break(Some(l)) | Stmt::Continue(Some(l)) => {
                *uses.entry(l.clone()).or_default() += 1;
            }
            _ => {}
        }
        for body in child_bodies(stmt) {
            count_label_uses(&body, uses);
        }
    }
}

fn child_bodies(stmt: &Stmt) -> Vec<Vec<Stmt>> {
    match stmt {
        Stmt::LabeledBlock { body, .. }
        | Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body } => vec![from_indent(body)],
        Stmt::If {
            then_body,
            else_body,
            ..
        } => vec![from_indent(then_body), from_indent(else_body)],
        Stmt::Match { arms, .. } => arms.iter().map(|arm| from_indent(&arm.body)).collect(),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            vec![from_indent(&body.stmts)]
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => vec![from_indent(then_body), from_indent(else_body)],
        _ => Vec::new(),
    }
}

fn elide_unused_labels(stmts: Vec<Stmt>) -> Vec<Stmt> {
    let mut uses = HashMap::new();
    count_label_uses(&stmts, &mut uses);
    strip_labels(stmts, &uses)
}

fn strip_labels(stmts: Vec<Stmt>, uses: &HashMap<Label, usize>) -> Vec<Stmt> {
    let mut out = Vec::with_capacity(stmts.len());
    for stmt in stmts {
        match stmt {
            Stmt::LabeledBlock { label, body } if !uses.contains_key(&label) => {
                out.extend(strip_labels(from_indent(&body), uses));
            }
            Stmt::Loop {
                label: Some(label),
                body,
            } if !uses.contains_key(&label) => out.push(Stmt::Loop {
                label: None,
                body: to_indent(strip_labels(from_indent(&body), uses)),
            }),
            other => out.push(map_children(other, &mut |body| strip_labels(body, uses))),
        }
    }
    out
}

fn map_children(stmt: Stmt, f: &mut dyn FnMut(Vec<Stmt>) -> Vec<Stmt>) -> Stmt {
    let mut go = |body: &[IndentStmt]| to_indent(f(from_indent(body)));
    match stmt {
        Stmt::LabeledBlock { label, body } => Stmt::LabeledBlock {
            label,
            body: go(&body),
        },
        Stmt::Loop { label, body } => Stmt::Loop {
            label,
            body: go(&body),
        },
        Stmt::For { pat, iter, body } => Stmt::For {
            pat,
            iter,
            body: go(&body),
        },
        Stmt::Scope { body } => Stmt::Scope { body: go(&body) },
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => Stmt::If {
            cond,
            then_body: go(&then_body),
            else_body: go(&else_body),
        },
        Stmt::LetIf {
            name,
            mutable,
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        } => Stmt::LetIf {
            name,
            mutable,
            ty,
            cond,
            then_body: go(&then_body),
            then_value,
            else_body: go(&else_body),
            else_value,
        },
        Stmt::Match { expr, arms } => Stmt::Match {
            expr,
            arms: arms
                .into_iter()
                .map(|arm| crate::backend::rust_ast::MatchArm {
                    body: go(&arm.body),
                    pattern: arm.pattern,
                })
                .collect(),
        },
        Stmt::Unsafe { body } => Stmt::Unsafe {
            body: Block {
                stmts: go(&body.stmts),
                tail: body.tail,
            },
        },
        Stmt::While { cond, body } => Stmt::While {
            cond,
            body: Block {
                stmts: go(&body.stmts),
                tail: body.tail,
            },
        },
        Stmt::Block(body) => Stmt::Block(Block {
            stmts: go(&body.stmts),
            tail: body.tail,
        }),
        other => other,
    }
}

fn map_bodies(stmts: Vec<Stmt>, f: &dyn Fn(Vec<Stmt>) -> Vec<Stmt>) -> Vec<Stmt> {
    let stmts = stmts
        .into_iter()
        .map(|stmt| map_children(stmt, &mut |body| map_bodies(body, f)))
        .collect();
    f(stmts)
}

fn collect_integral_locals(stmts: &[Stmt]) -> HashSet<Ident> {
    let mut out = HashSet::new();
    walk_integral_locals(stmts, &mut out);
    out
}

fn walk_integral_locals(stmts: &[Stmt], out: &mut HashSet<Ident>) {
    for stmt in stmts {
        if let Stmt::Let {
            name,
            ty: Some(Type::Prim(prim)),
            ..
        } = stmt
            && is_integral_prim(*prim)
        {
            out.insert(Ident::new(name.clone()));
        }
        for body in child_bodies(stmt) {
            walk_integral_locals(&body, out);
        }
    }
}

fn is_integral_prim(prim: Prim) -> bool {
    matches!(
        prim,
        Prim::Bool
            | Prim::I8
            | Prim::I16
            | Prim::I32
            | Prim::I64
            | Prim::I128
            | Prim::Isize
            | Prim::U8
            | Prim::U16
            | Prim::U32
            | Prim::U64
            | Prim::U128
            | Prim::Usize
    )
}

fn clean_branches(stmts: Vec<Stmt>, integral: &HashSet<Ident>) -> Vec<Stmt> {
    stmts
        .into_iter()
        .map(|stmt| match stmt {
            Stmt::If {
                cond,
                then_body,
                else_body,
            } if then_body.is_empty() && !else_body.is_empty() => Stmt::If {
                cond: negate(cond, integral),
                then_body: else_body,
                else_body: Vec::new(),
            },
            other => other,
        })
        .collect()
}

fn loops_to_while(stmts: Vec<Stmt>, integral: &HashSet<Ident>) -> Vec<Stmt> {
    stmts
        .into_iter()
        .map(|stmt| match stmt {
            Stmt::Loop { label: None, body } => match head_exit_test(&body) {
                Some(cond) => Stmt::While {
                    cond: negate(cond, integral),
                    body: Block {
                        stmts: body[1..].to_vec(),
                        tail: None,
                    },
                },
                None => Stmt::Loop { label: None, body },
            },
            other => other,
        })
        .collect()
}

fn head_exit_test(body: &[IndentStmt]) -> Option<Expr> {
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = &body.first()?.stmt
    else {
        return None;
    };
    if !else_body.is_empty() {
        return None;
    }
    let [only] = &then_body[..] else {
        return None;
    };
    matches!(only.stmt, Stmt::Break(None)).then(|| cond.clone())
}

fn negate(expr: Expr, integral: &HashSet<Ident>) -> Expr {
    let is_integral = |e: &Expr| match e {
        Expr::Var(v) => integral.contains(v),
        other => super::super::negate::is_integral_literal(other),
    };
    super::super::negate::negate(expr, &is_integral)
}
