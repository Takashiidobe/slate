use super::exprs::{FnCtx, lower_expr};
use super::types::lower_type;
use crate::backend::rust_ast::{
    Block, Expr as RExpr, IndentStmt, MatchArm, Path, Pattern, Prim, RustValue, Stmt as RStmt,
    Type as RType, UnaryOp as RUnaryOp,
};
use crate::function_identity::CallBinding;
use crate::parse::ast::{ExprKind, Obj, Program, Stmt, StmtKind};

pub(crate) fn lower_function_body(obj: &Obj, program: &Program) -> Vec<IndentStmt> {
    let ctx = FnCtx {
        locals: &obj.locals,
        globals: &program.globals,
        types: &program.types,
        is_main: obj.name == "main",
    };
    let mut body = Vec::new();
    for stmt in &obj.body {
        lower_stmt(stmt, &ctx, &mut body);
    }
    vec![mk(RStmt::Unsafe {
        body: Block {
            stmts: body,
            tail: None,
        },
    })]
}

fn mk(stmt: RStmt) -> IndentStmt {
    IndentStmt { depth: 0, stmt }
}

fn block_of(stmt: &Stmt, ctx: &FnCtx) -> Vec<IndentStmt> {
    let mut out = Vec::new();
    lower_stmt(stmt, ctx, &mut out);
    out
}

pub(crate) fn lower_stmt(stmt: &Stmt, ctx: &FnCtx, out: &mut Vec<IndentStmt>) {
    match &stmt.kind {
        StmtKind::Return(value) => {
            let lowered = value.as_ref().map(|expr| lower_expr(expr, ctx));
            if ctx.is_main {
                let code = lowered.unwrap_or(RExpr::Value(RustValue::I64(0)));
                out.push(mk(RStmt::Expr(RExpr::Call {
                    func: Box::new(RExpr::Path(Path::new(
                        ["std", "process", "exit"].map(Into::into),
                    ))),
                    args: vec![RExpr::Cast {
                        expr: Box::new(code),
                        ty: RType::Prim(Prim::I32),
                    }],
                    binding: CallBinding::Generated,
                })));
            } else {
                out.push(mk(RStmt::Return(lowered)));
            }
        }
        StmtKind::Block(stmts) => {
            for inner in stmts {
                lower_stmt(inner, ctx, out);
            }
        }
        StmtKind::If { cond, then, els } => out.push(mk(RStmt::If {
            cond: lower_expr(cond, ctx),
            then_body: block_of(then, ctx),
            else_body: els
                .as_ref()
                .map(|els| block_of(els, ctx))
                .unwrap_or_default(),
        })),
        StmtKind::For {
            init,
            cond,
            inc,
            body,
        } => {
            if let Some(init) = init {
                lower_stmt(init, ctx, out);
            }
            let mut loop_body = Vec::new();
            if let Some(cond) = cond {
                loop_body.push(mk(RStmt::If {
                    cond: RExpr::Unary {
                        op: RUnaryOp::Not,
                        expr: Box::new(lower_expr(cond, ctx)),
                    },
                    then_body: vec![mk(RStmt::Break(None))],
                    else_body: Vec::new(),
                }));
            }
            lower_stmt(body, ctx, &mut loop_body);
            if let Some(inc) = inc {
                loop_body.push(mk(RStmt::Expr(lower_expr(inc, ctx))));
            }
            out.push(mk(RStmt::Loop {
                label: None,
                body: loop_body,
            }));
        }
        StmtKind::DoWhile { body, cond } => {
            let mut loop_body = block_of(body, ctx);
            loop_body.push(mk(RStmt::If {
                cond: RExpr::Unary {
                    op: RUnaryOp::Not,
                    expr: Box::new(lower_expr(cond, ctx)),
                },
                then_body: vec![mk(RStmt::Break(None))],
                else_body: Vec::new(),
            }));
            out.push(mk(RStmt::Loop {
                label: None,
                body: loop_body,
            }));
        }
        StmtKind::Switch { cond, body, .. } => {
            out.push(mk(RStmt::Match {
                expr: lower_expr(cond, ctx),
                arms: lower_switch_arms(body, ctx),
            }));
        }
        StmtKind::Case { stmt, .. } => lower_stmt(stmt, ctx, out),
        StmtKind::Goto { .. } | StmtKind::GotoExpr { .. } => {
            out.push(mk(RStmt::Expr(RExpr::Macro {
                name: "unimplemented".into(),
                args: vec![RExpr::Str(
                    "slate: goto unsupported in native lowering".into(),
                )],
            })));
        }
        StmtKind::Break => out.push(mk(RStmt::Break(None))),
        StmtKind::Continue => out.push(mk(RStmt::Continue(None))),
        StmtKind::Label { stmt, .. } => lower_stmt(stmt, ctx, out),
        StmtKind::Fallthrough => {}
        StmtKind::Asm(_) => {}
        StmtKind::Expr(expr) => match &expr.kind {
            ExprKind::Assign { lhs, rhs } => out.push(mk(RStmt::Assign {
                target: lower_expr(lhs, ctx),
                value: lower_expr(rhs, ctx),
            })),
            _ => out.push(mk(RStmt::Expr(lower_expr(expr, ctx)))),
        },
        StmtKind::Decl(idx) => {
            let local = &ctx.locals[*idx];
            if local.name == "__alloca_size__" || local.name == "__va_area__" {
                return;
            }
            out.push(mk(RStmt::Let {
                name: super::exprs::local_ident(local),
                mutable: true,
                ty: Some(lower_type(&local.ty, ctx.types)),
                init: None,
            }));
        }
    }
}

fn lower_switch_arms(body: &Stmt, ctx: &FnCtx) -> Vec<MatchArm> {
    let stmts: &[Stmt] = match &body.kind {
        StmtKind::Block(stmts) => stmts,
        _ => std::slice::from_ref(body),
    };
    let mut arms: Vec<MatchArm> = Vec::new();
    let mut current: Option<(Pattern, Vec<IndentStmt>)> = None;
    for stmt in stmts {
        if let StmtKind::Case { range, .. } = &stmt.kind {
            if let Some((pattern, out)) = current.take() {
                arms.push(MatchArm { pattern, body: out });
            }
            let pattern = match range {
                Some(range) if range.begin == range.end => Pattern::I64(range.begin),
                Some(range) => Pattern::InclusiveRange {
                    start: range.begin as i128,
                    end: range.end as i128,
                },
                None => Pattern::Wildcard,
            };
            current = Some((pattern, Vec::new()));
        }
        let target = current.get_or_insert_with(|| (Pattern::Wildcard, Vec::new()));
        lower_stmt(stmt, ctx, &mut target.1);
    }
    if let Some((pattern, out)) = current {
        arms.push(MatchArm { pattern, body: out });
    }
    if arms.is_empty()
        || !arms
            .iter()
            .any(|arm| matches!(arm.pattern, Pattern::Wildcard))
    {
        arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: Vec::new(),
        });
    }
    arms
}
