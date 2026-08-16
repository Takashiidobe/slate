use super::exprs::{lower_expr, truthy};
use super::types::CType;
use super::{Env, GotoCtx, LResult, LowerError, NodeExt, is_present};
use crate::backend::rust_ast::{
    Block, Expr as RExpr, ExprMatchArm, IndentStmt, Label, MatchArm, Path, Pattern, Prim,
    RustValue, Stmt as RStmt, Type as RType, UnaryOp as RUnaryOp,
};
use crate::function_identity::CallBinding;
use crate::parse::clang_ast::{Clang, Decl, Node};
use clang_ast::Id;
use std::collections::{HashMap, HashSet};
use std::sync::atomic::{AtomicUsize, Ordering};

static LOOP_LABEL_COUNTER: AtomicUsize = AtomicUsize::new(0);
static GOTO_LABEL_COUNTER: AtomicUsize = AtomicUsize::new(0);

fn body_has_direct_continue(node: &Node) -> bool {
    match &node.kind {
        Clang::ContinueStmt(_) => true,
        Clang::ForStmt(_) | Clang::WhileStmt(_) | Clang::DoStmt(_) => false,
        _ => node.inner.iter().any(body_has_direct_continue),
    }
}

fn loop_labels_if_needed(body: &Node) -> Option<(Label, Label)> {
    if !body_has_direct_continue(body) {
        return None;
    }
    let n = LOOP_LABEL_COUNTER.fetch_add(1, Ordering::Relaxed);
    Some((
        Label::new(format!("__loop{n}")),
        Label::new(format!("__continue{n}")),
    ))
}

fn lower_loop_body(
    body: &Node,
    labels: Option<&(Label, Label)>,
    env: Env,
    out: &mut Vec<IndentStmt>,
) -> LResult<()> {
    let base_env = Env {
        continue_label: None,
        break_label: None,
        ..env
    };
    match labels {
        Some((loop_label, continue_label)) => {
            let mut inner = Vec::new();
            lower_stmt(
                body,
                Env {
                    continue_label: Some(continue_label),
                    break_label: Some(loop_label),
                    ..base_env
                },
                &mut inner,
            )?;
            out.push(mk(RStmt::LabeledBlock {
                label: continue_label.clone(),
                body: inner,
            }));
        }
        None => lower_stmt(body, base_env, out)?,
    }
    Ok(())
}

fn decl_type(d: &Decl) -> CType {
    CType::parse(d.qual_type.as_ref().map(|t| t.canonical()).unwrap_or("int"))
}

fn body_has_goto(node: &Node) -> bool {
    if let Clang::Other(o) = &node.kind
        && matches!(
            o.kind.as_deref(),
            Some("GotoStmt") | Some("IndirectGotoStmt")
        )
    {
        return true;
    }
    node.inner.iter().any(body_has_goto)
}

pub(crate) fn lower_function_body(
    body: &Node,
    env: Env,
    returns_value: bool,
) -> LResult<Vec<IndentStmt>> {
    let mut out = Vec::new();
    if body.inner.iter().any(body_has_goto) {
        lower_goto_function_body(body, env, returns_value, &mut out)?;
    } else {
        for stmt in &body.inner {
            lower_stmt(stmt, env, &mut out)?;
        }
    }
    Ok(vec![mk(RStmt::Unsafe {
        body: Block {
            stmts: out,
            tail: None,
        },
    })])
}

// Lowers a function body containing top-level goto/label statements as a
// state-machine dispatch loop: each label starts a new numbered segment, and
// each goto becomes an assignment to the state variable plus a labeled
// `continue`. Locals declared across segment boundaries are hoisted above the
// loop (as plain mutable lets) since each match arm is otherwise its own
// Rust scope.
fn lower_goto_function_body(
    body: &Node,
    env: Env,
    returns_value: bool,
    out: &mut Vec<IndentStmt>,
) -> LResult<()> {
    let mut flat: Vec<&Node> = Vec::new();
    let mut segment_starts: Vec<usize> = vec![0];
    let mut label_ids: Vec<(usize, Id)> = Vec::new();
    for stmt in &body.inner {
        if let Clang::Other(o) = &stmt.kind
            && o.kind.as_deref() == Some("LabelStmt")
        {
            let decl_id = o.decl_id.ok_or(LowerError::UnsupportedGoto)?;
            let idx = flat.len();
            segment_starts.push(idx);
            label_ids.push((idx, decl_id));
            let inner = stmt.inner.first().ok_or(LowerError::UnsupportedGoto)?;
            flat.push(inner);
        } else {
            flat.push(stmt);
        }
    }
    segment_starts.sort_unstable();
    segment_starts.dedup();

    let mut label_to_state: HashMap<Id, usize> = HashMap::new();
    for (idx, id) in &label_ids {
        let state = segment_starts
            .iter()
            .position(|s| s == idx)
            .ok_or(LowerError::UnsupportedGoto)?;
        label_to_state.insert(*id, state);
    }

    let hoisted_vars: HashSet<Id> = flat
        .iter()
        .filter(|stmt| matches!(stmt.kind, Clang::DeclStmt(_)))
        .flat_map(|stmt| {
            stmt.inner.iter().filter_map(|c| match &c.kind {
                Clang::VarDecl(_) => Some(c.id),
                _ => None,
            })
        })
        .collect();

    for stmt in &flat {
        let Clang::DeclStmt(_) = &stmt.kind else {
            continue;
        };
        for child in &stmt.inner {
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
            out.push(mk(RStmt::Let {
                name: info.name.clone(),
                mutable: true,
                ty: Some(ty.lower(env.records)),
                init: Some(super::globals::zero_value(&ty, env.records)),
            }));
        }
    }

    let n = GOTO_LABEL_COUNTER.fetch_add(1, Ordering::Relaxed);
    let dispatch_label = Label::new(format!("__goto_dispatch{n}"));
    let state_var = format!("__goto_state{n}");
    let goto_env = Env {
        goto: Some(GotoCtx {
            state_var: state_var.as_str(),
            dispatch_label: &dispatch_label,
            label_to_state: &label_to_state,
            hoisted_vars: &hoisted_vars,
        }),
        ..env
    };

    let mut arms = Vec::new();
    for i in 0..segment_starts.len() {
        let start = segment_starts[i];
        let end = segment_starts.get(i + 1).copied().unwrap_or(flat.len());
        let mut seg_out = Vec::new();
        for stmt in &flat[start..end] {
            lower_stmt(stmt, goto_env, &mut seg_out)?;
        }
        seg_out.push(mk(RStmt::Assign {
            target: RExpr::Var(state_var.as_str().into()),
            value: RExpr::Value(RustValue::I64((i + 1) as i64)),
        }));
        seg_out.push(mk(RStmt::Continue(Some(dispatch_label.clone()))));
        arms.push(MatchArm {
            pattern: Pattern::I64(i as i64),
            body: seg_out,
        });
    }
    let fallthrough = if returns_value {
        RStmt::Expr(RExpr::Macro {
            name: "unreachable".into(),
            args: Vec::new(),
        })
    } else {
        RStmt::Break(Some(dispatch_label.clone()))
    };
    arms.push(MatchArm {
        pattern: Pattern::Wildcard,
        body: vec![mk(fallthrough)],
    });

    out.push(mk(RStmt::Let {
        name: state_var.clone(),
        mutable: true,
        ty: Some(RType::Prim(Prim::I32)),
        init: Some(RExpr::Value(RustValue::I64(0))),
    }));
    out.push(mk(RStmt::Loop {
        label: Some(dispatch_label),
        body: vec![mk(RStmt::Match {
            expr: RExpr::Var(state_var.as_str().into()),
            arms,
        })],
    }));
    Ok(())
}

fn mk(stmt: RStmt) -> IndentStmt {
    IndentStmt { depth: 0, stmt }
}

fn hook_call_stmt(name: &str) -> RStmt {
    RStmt::Expr(RExpr::Call {
        binding: CallBinding::Generated,
        func: Box::new(RExpr::Var(name.into())),
        args: Vec::new(),
    })
}

pub(crate) fn splice_ctor_prelude(body_stmts: &mut [IndentStmt], ctors: &[String]) {
    if ctors.is_empty() {
        return;
    }
    let Some(IndentStmt {
        stmt: RStmt::Unsafe { body },
        ..
    }) = body_stmts.first_mut()
    else {
        return;
    };
    let prelude: Vec<IndentStmt> = ctors.iter().map(|name| mk(hook_call_stmt(name))).collect();
    body.stmts.splice(0..0, prelude);
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
                Some(e) => {
                    let lowered = lower_expr(e, env)?;
                    Some(
                        if super::exprs::node_type(e) == CType::Bool && *env.ret_ty != CType::Bool {
                            RExpr::Cast {
                                expr: Box::new(lowered),
                                ty: env.ret_ty.lower(env.records),
                            }
                        } else {
                            lowered
                        },
                    )
                }
                None => None,
            };
            if env.is_main {
                let code = value.unwrap_or(RExpr::Value(RustValue::I64(0)));
                for name in env.dtor_calls {
                    out.push(mk(hook_call_stmt(name)));
                }
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
                let info = env
                    .vars
                    .get(&child.id)
                    .ok_or(LowerError::UnregisteredLocal)?;
                if env.goto.is_some_and(|g| g.hoisted_vars.contains(&child.id)) {
                    if let Some(c) = child.inner.first() {
                        let ty = decl_type(d);
                        let value = super::globals::lower_init(c, &ty, env)?;
                        out.push(mk(RStmt::Assign {
                            target: RExpr::Var(info.name.as_str().into()),
                            value,
                        }));
                    }
                    continue;
                }
                let ty = decl_type(d);
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
            let labels = loop_labels_if_needed(body);
            let mut loop_body = Vec::new();
            if is_present(cond) {
                loop_body.push(break_unless(cond, env)?);
            }
            lower_loop_body(body, labels.as_ref(), env, &mut loop_body)?;
            if is_present(inc) {
                loop_body.push(mk(RStmt::Expr(lower_expr(inc, env)?)));
            }
            out.push(mk(RStmt::Loop {
                label: labels.map(|(loop_label, _)| loop_label),
                body: loop_body,
            }));
        }
        Clang::WhileStmt(_) => {
            let cond = node.child(0)?;
            let body = node.child(1)?;
            let labels = loop_labels_if_needed(body);
            let mut loop_body = vec![break_unless(cond, env)?];
            lower_loop_body(body, labels.as_ref(), env, &mut loop_body)?;
            out.push(mk(RStmt::Loop {
                label: labels.map(|(loop_label, _)| loop_label),
                body: loop_body,
            }));
        }
        Clang::DoStmt(_) => {
            let body = node.child(0)?;
            let cond = node.child(1)?;
            let labels = loop_labels_if_needed(body);
            let mut loop_body = Vec::new();
            lower_loop_body(body, labels.as_ref(), env, &mut loop_body)?;
            loop_body.push(break_unless(cond, env)?);
            out.push(mk(RStmt::Loop {
                label: labels.map(|(loop_label, _)| loop_label),
                body: loop_body,
            }));
        }
        Clang::SwitchStmt(_) => {
            let cond = node.child(0)?;
            let body = node.child(1)?;
            lower_switch(cond, body, env, out)?;
        }
        Clang::CaseStmt(_) | Clang::DefaultStmt(_) => {
            let mut n = node;
            loop {
                let next = n.inner.last().ok_or(LowerError::MissingCaseBody)?;
                match &next.kind {
                    Clang::CaseStmt(_) | Clang::DefaultStmt(_) => n = next,
                    _ => {
                        lower_stmt(next, env, out)?;
                        break;
                    }
                }
            }
        }
        Clang::BreakStmt(_) => out.push(mk(match env.break_label {
            Some(label) => RStmt::Break(Some(label.clone())),
            None => RStmt::Break(None),
        })),
        Clang::ContinueStmt(_) => out.push(mk(match env.continue_label {
            Some(label) => RStmt::Break(Some(label.clone())),
            None => RStmt::Continue(None),
        })),
        Clang::NullStmt(_) => {}
        Clang::BinaryOperator(b) if b.opcode == "=" => {
            out.push(mk(RStmt::Assign {
                target: lower_expr(node.child(0)?, env)?,
                value: lower_expr(node.child(1)?, env)?,
            }));
        }
        Clang::Other(o) if o.kind.as_deref() == Some("GotoStmt") => {
            let goto = env.goto.ok_or(LowerError::UnsupportedGoto)?;
            let target = o.target_label_decl_id.ok_or(LowerError::UnsupportedGoto)?;
            let state = *goto
                .label_to_state
                .get(&target)
                .ok_or(LowerError::UnsupportedGoto)?;
            out.push(mk(RStmt::Assign {
                target: RExpr::Var(goto.state_var.into()),
                value: RExpr::Value(RustValue::I64(state as i64)),
            }));
            out.push(mk(RStmt::Continue(Some(goto.dispatch_label.clone()))));
        }
        Clang::Other(o) if o.kind.as_deref() == Some("LabelStmt") => {
            let inner = node.inner.first().ok_or(LowerError::UnsupportedGoto)?;
            lower_stmt(inner, env, out)?;
        }
        Clang::Other(o) if o.kind.as_deref() == Some("IndirectGotoStmt") => {
            return Err(LowerError::UnsupportedGoto);
        }
        _ => out.push(mk(RStmt::Expr(lower_expr(node, env)?))),
    }
    Ok(())
}

static SWITCH_LABEL_COUNTER: AtomicUsize = AtomicUsize::new(0);

struct SwitchCase<'a> {
    patterns: Vec<i64>,
    is_default: bool,
    stmts: Vec<&'a Node>,
}

/// Lower a switch as an index-dispatch loop rather than a bare `match`, so
/// break can target the switch itself (a bare Rust `break` inside a `match`
/// binds to the nearest enclosing *loop*, not the switch) and real C
/// fallthrough (no break, execution continues into the next case's body) is
/// representable at all -- a `match` arm can't fall into the next arm.
fn lower_switch(cond: &Node, body: &Node, env: Env, out: &mut Vec<IndentStmt>) -> LResult<()> {
    let cases = collect_switch_cases(body);
    if cases.is_empty() {
        return Ok(());
    }

    let n = SWITCH_LABEL_COUNTER.fetch_add(1, Ordering::Relaxed);
    let switch_label = Label::new(format!("__switch{n}"));
    let selector_name = format!("__switch_value{n}");
    let case_name = format!("__switch_case{n}");
    let default_index = cases.iter().position(|case| case.is_default);
    let fallback = default_index.map(|index| index as i64).unwrap_or(-1);

    let mut selector_arms = Vec::new();
    for (index, case) in cases.iter().enumerate() {
        for &value in &case.patterns {
            selector_arms.push(ExprMatchArm {
                pattern: Pattern::I64(value),
                value: RExpr::Value(RustValue::I64(index as i64)),
            });
        }
    }
    selector_arms.push(ExprMatchArm {
        pattern: Pattern::Wildcard,
        value: RExpr::Value(RustValue::I64(fallback)),
    });

    let switch_env = Env {
        break_label: Some(&switch_label),
        ..env
    };
    let mut case_arms = Vec::new();
    for (index, case) in cases.iter().enumerate() {
        let mut case_body = Vec::new();
        for stmt in &case.stmts {
            lower_stmt(stmt, switch_env, &mut case_body)?;
        }
        if !ends_in_control_flow(&case_body) {
            if index + 1 < cases.len() {
                case_body.push(mk(RStmt::Assign {
                    target: RExpr::Var(case_name.clone().into()),
                    value: RExpr::Value(RustValue::I64((index + 1) as i64)),
                }));
                case_body.push(mk(RStmt::Continue(Some(switch_label.clone()))));
            } else {
                case_body.push(mk(RStmt::Break(Some(switch_label.clone()))));
            }
        }
        case_arms.push(MatchArm {
            pattern: Pattern::I64(index as i64),
            body: case_body,
        });
    }
    case_arms.push(MatchArm {
        pattern: Pattern::Wildcard,
        body: vec![mk(RStmt::Break(Some(switch_label.clone())))],
    });

    let selector = lower_expr(cond, env)?;
    out.push(mk(RStmt::Scope {
        body: vec![
            mk(RStmt::Let {
                name: selector_name.clone(),
                mutable: false,
                ty: None,
                init: Some(selector),
            }),
            mk(RStmt::Let {
                name: case_name.clone(),
                mutable: true,
                ty: Some(RType::Prim(Prim::I32)),
                init: Some(RExpr::Match {
                    expr: Box::new(RExpr::Var(selector_name.into())),
                    arms: selector_arms,
                }),
            }),
            mk(RStmt::Loop {
                label: Some(switch_label),
                body: vec![mk(RStmt::Match {
                    expr: RExpr::Var(case_name.into()),
                    arms: case_arms,
                })],
            }),
        ],
    }));
    Ok(())
}

fn ends_in_control_flow(body: &[IndentStmt]) -> bool {
    matches!(
        body.last().map(|stmt| &stmt.stmt),
        Some(RStmt::Break(_) | RStmt::Continue(_) | RStmt::Return(_))
    )
}

fn collect_switch_cases(body: &Node) -> Vec<SwitchCase<'_>> {
    let stmts: &[Node] = match &body.kind {
        Clang::CompoundStmt(_) => &body.inner,
        _ => std::slice::from_ref(body),
    };
    let mut cases: Vec<SwitchCase<'_>> = Vec::new();
    for stmt in stmts {
        match &stmt.kind {
            Clang::CaseStmt(_) | Clang::DefaultStmt(_) => {
                let mut patterns = Vec::new();
                let mut is_default = false;
                let mut node = stmt;
                let first_body_stmt = loop {
                    match &node.kind {
                        Clang::CaseStmt(_) => {
                            if let Ok(value_node) = node.child(0) {
                                patterns.push(case_label_value(value_node));
                            }
                        }
                        _ => is_default = true,
                    }
                    let Some(next) = node.inner.last() else {
                        break None;
                    };
                    match &next.kind {
                        Clang::CaseStmt(_) | Clang::DefaultStmt(_) => node = next,
                        _ => break Some(next),
                    }
                };
                cases.push(SwitchCase {
                    patterns,
                    is_default,
                    stmts: first_body_stmt.into_iter().collect(),
                });
            }
            _ => {
                if let Some(case) = cases.last_mut() {
                    case.stmts.push(stmt);
                }
            }
        }
    }
    cases
}

fn case_label_value(node: &Node) -> i64 {
    match &node.kind {
        Clang::IntegerLiteral(l) => l.value.trim().parse::<i64>().unwrap_or(0),
        Clang::UnaryOperator(u) if u.opcode == "-" => node
            .inner
            .first()
            .map(case_label_value)
            .unwrap_or(0)
            .wrapping_neg(),
        Clang::Other(_) => node.inner.first().map(case_label_value).unwrap_or(0),
        _ => 0,
    }
}
