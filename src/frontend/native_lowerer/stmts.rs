use super::exprs::{lower_expr, truthy};
use super::types::CType;
use super::{Env, LResult, LowerError, NodeExt, is_present};
use crate::backend::rust_ast::{
    Block, Expr as RExpr, IndentStmt, MatchArm, Path, Pattern, Prim, RustValue, Stmt as RStmt,
    Type as RType, UnaryOp as RUnaryOp,
};
use crate::function_identity::CallBinding;
use crate::parse::clang_ast::{Clang, Decl, Node};

fn decl_type(d: &Decl) -> CType {
    CType::parse(d.qual_type.as_ref().map(|t| t.canonical()).unwrap_or("int"))
}

pub(crate) fn lower_function_body(body: &Node, env: Env) -> LResult<Vec<IndentStmt>> {
    let mut out = Vec::new();
    for stmt in &body.inner {
        lower_stmt(stmt, env, &mut out)?;
    }
    Ok(vec![mk(RStmt::Unsafe {
        body: Block {
            stmts: out,
            tail: None,
        },
    })])
}

fn mk(stmt: RStmt) -> IndentStmt {
    IndentStmt { depth: 0, stmt }
}

fn block_of(node: &Node, env: Env) -> LResult<Vec<IndentStmt>> {
    let mut out = Vec::new();
    lower_stmt(node, env, &mut out)?;
    Ok(out)
}

fn break_unless(cond: &Node, env: Env) -> LResult<IndentStmt> {
    Ok(mk(RStmt::If {
        cond: RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(truthy(cond, env)?),
        },
        then_body: vec![mk(RStmt::Break(None))],
        else_body: Vec::new(),
    }))
}

pub(crate) fn lower_stmt(node: &Node, env: Env, out: &mut Vec<IndentStmt>) -> LResult<()> {
    match &node.kind {
        Clang::CompoundStmt(_) => {
            for stmt in &node.inner {
                lower_stmt(stmt, env, out)?;
            }
        }
        Clang::ReturnStmt(_) => {
            let value = match node.inner.first() {
                Some(e) => Some(lower_expr(e, env)?),
                None => None,
            };
            if env.is_main {
                let code = value.unwrap_or(RExpr::Value(RustValue::I64(0)));
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
                out.push(mk(RStmt::Return(value)));
            }
        }
        Clang::DeclStmt(_) => {
            for child in &node.inner {
                let Clang::VarDecl(d) = &child.kind else {
                    continue;
                };
                if d.is_implicit || d.storage_class.as_deref() == Some("static") {
                    continue;
                }
                let ty = decl_type(d);
                let info = env
                    .vars
                    .get(&child.id)
                    .ok_or(LowerError::UnregisteredLocal)?;
                let init = match child.inner.first() {
                    Some(c) => super::globals::lower_init(c, &ty, env)?,
                    None => super::globals::zero_value(&ty, env.records),
                };
                out.push(mk(RStmt::Let {
                    name: info.name.clone(),
                    mutable: true,
                    ty: Some(ty.lower(env.records)),
                    init: Some(init),
                }));
            }
        }
        Clang::IfStmt(_) => {
            let cond = node.child(0)?;
            let then_branch = node.child(1)?;
            let else_branch = node.inner.get(2);
            out.push(mk(RStmt::If {
                cond: truthy(cond, env)?,
                then_body: block_of(then_branch, env)?,
                else_body: match else_branch {
                    Some(e) => block_of(e, env)?,
                    None => Vec::new(),
                },
            }));
        }
        Clang::ForStmt(_) => {
            let init = node.child(0)?;
            let cond = node.child(2)?;
            let inc = node.child(3)?;
            let body = node.child(4)?;
            if is_present(init) {
                lower_stmt(init, env, out)?;
            }
            let mut loop_body = Vec::new();
            if is_present(cond) {
                loop_body.push(break_unless(cond, env)?);
            }
            lower_stmt(body, env, &mut loop_body)?;
            if is_present(inc) {
                loop_body.push(mk(RStmt::Expr(lower_expr(inc, env)?)));
            }
            out.push(mk(RStmt::Loop {
                label: None,
                body: loop_body,
            }));
        }
        Clang::WhileStmt(_) => {
            let cond = node.child(0)?;
            let body = node.child(1)?;
            let mut loop_body = vec![break_unless(cond, env)?];
            lower_stmt(body, env, &mut loop_body)?;
            out.push(mk(RStmt::Loop {
                label: None,
                body: loop_body,
            }));
        }
        Clang::DoStmt(_) => {
            let body = node.child(0)?;
            let cond = node.child(1)?;
            let mut loop_body = block_of(body, env)?;
            loop_body.push(break_unless(cond, env)?);
            out.push(mk(RStmt::Loop {
                label: None,
                body: loop_body,
            }));
        }
        Clang::SwitchStmt(_) => {
            let cond = node.child(0)?;
            let body = node.child(1)?;
            out.push(mk(RStmt::Match {
                expr: lower_expr(cond, env)?,
                arms: lower_switch_arms(body, env)?,
            }));
        }
        Clang::CaseStmt(_) | Clang::DefaultStmt(_) => {
            let mut patterns = Vec::new();
            push_case_chain(node, &mut patterns, out, env)?;
        }
        Clang::BreakStmt(_) => out.push(mk(RStmt::Break(None))),
        Clang::ContinueStmt(_) => out.push(mk(RStmt::Continue(None))),
        Clang::NullStmt(_) => {}
        Clang::BinaryOperator(b) if b.opcode == "=" => {
            out.push(mk(RStmt::Assign {
                target: lower_expr(node.child(0)?, env)?,
                value: lower_expr(node.child(1)?, env)?,
            }));
        }
        Clang::Other(o) if is_goto_or_label(o.kind.as_deref()) => {
            out.push(mk(RStmt::Expr(RExpr::Macro {
                name: "unimplemented".into(),
                args: vec![RExpr::Str(
                    "slate: goto/label unsupported in native lowering".into(),
                )],
            })));
        }
        _ => out.push(mk(RStmt::Expr(lower_expr(node, env)?))),
    }
    Ok(())
}

fn is_goto_or_label(kind: Option<&str>) -> bool {
    matches!(
        kind,
        Some("GotoStmt") | Some("LabelStmt") | Some("IndirectGotoStmt")
    )
}

fn lower_switch_arms(body: &Node, env: Env) -> LResult<Vec<MatchArm>> {
    let stmts: &[Node] = match &body.kind {
        Clang::CompoundStmt(_) => &body.inner,
        _ => std::slice::from_ref(body),
    };
    let mut arms: Vec<MatchArm> = Vec::new();
    let mut patterns: Vec<Pattern> = Vec::new();
    let mut current: Vec<IndentStmt> = Vec::new();

    for stmt in stmts {
        match &stmt.kind {
            Clang::CaseStmt(_) | Clang::DefaultStmt(_) => {
                if !patterns.is_empty() {
                    push_arms(&mut arms, &mut patterns, &mut current);
                }
                push_case_chain(stmt, &mut patterns, &mut current, env)?;
            }
            _ => {
                if patterns.is_empty() {
                    patterns.push(Pattern::Wildcard);
                }
                lower_stmt(stmt, env, &mut current)?;
            }
        }
    }
    push_arms(&mut arms, &mut patterns, &mut current);

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
    Ok(arms)
}

fn push_case_chain(
    mut node: &Node,
    patterns: &mut Vec<Pattern>,
    body: &mut Vec<IndentStmt>,
    env: Env,
) -> LResult<()> {
    loop {
        patterns.push(match &node.kind {
            Clang::CaseStmt(_) => Pattern::I64(case_label_value(node.child(0)?)),
            _ => Pattern::Wildcard,
        });
        let next = node.inner.last().ok_or(LowerError::MissingCaseBody)?;
        match &next.kind {
            Clang::CaseStmt(_) | Clang::DefaultStmt(_) => node = next,
            _ => {
                lower_stmt(next, env, body)?;
                return Ok(());
            }
        }
    }
}

fn push_arms(arms: &mut Vec<MatchArm>, patterns: &mut Vec<Pattern>, body: &mut Vec<IndentStmt>) {
    let n = patterns.len();
    for (i, pattern) in patterns.drain(..).enumerate() {
        let arm_body = if i + 1 == n {
            std::mem::take(body)
        } else {
            body.clone()
        };
        arms.push(MatchArm {
            pattern,
            body: arm_body,
        });
    }
}

fn case_label_value(node: &Node) -> i64 {
    match &node.kind {
        Clang::IntegerLiteral(l) => l.value.trim().parse::<i64>().unwrap_or(0),
        Clang::Other(_) => node.inner.first().map(case_label_value).unwrap_or(0),
        _ => 0,
    }
}
