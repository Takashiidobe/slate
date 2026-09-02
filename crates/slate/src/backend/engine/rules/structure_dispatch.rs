use std::collections::{BTreeSet, HashMap, HashSet};

use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{self, Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{
    Expr, Ident, IndentStmt, Label, MatchArm, Pattern, RustValue, Stmt,
};

struct Flow {
    prefix: Vec<Stmt>,
    transfer: Transfer,
}

enum Transfer {
    Goto(usize),
    Exit,
    Return(Option<Expr>),
    Diverge(Stmt),
    Branch {
        cond: Expr,
        then_: Box<Flow>,
        else_: Box<Flow>,
    },
    Switch {
        expr: Expr,
        arms: Vec<(Pattern, Flow)>,
    },
}

struct Cfg {
    states: Vec<Flow>,
}

impl Cfg {
    fn successors(&self, s: usize) -> Vec<usize> {
        let mut out = Vec::new();
        collect_gotos(&self.states[s], &mut out);
        out
    }
}

fn collect_gotos(flow: &Flow, out: &mut Vec<usize>) {
    match &flow.transfer {
        Transfer::Goto(t) => {
            if !out.contains(t) {
                out.push(*t);
            }
        }
        Transfer::Branch { then_, else_, .. } => {
            collect_gotos(then_, out);
            collect_gotos(else_, out);
        }
        Transfer::Switch { arms, .. } => {
            for (_, f) in arms {
                collect_gotos(f, out);
            }
        }
        Transfer::Exit | Transfer::Return(_) | Transfer::Diverge(_) => {}
    }
}

fn successors_within(cfg: &Cfg, s: usize, region: &BTreeSet<usize>) -> Vec<usize> {
    cfg.successors(s)
        .into_iter()
        .filter(|t| region.contains(t))
        .collect()
}

fn successors_excl(
    cfg: &Cfg,
    s: usize,
    region: &BTreeSet<usize>,
    exclude_into: Option<usize>,
) -> Vec<usize> {
    successors_within(cfg, s, region)
        .into_iter()
        .filter(|&t| Some(t) != exclude_into)
        .collect()
}

fn reach_within(cfg: &Cfg, seed: usize, region: &BTreeSet<usize>) -> BTreeSet<usize> {
    let mut reached = BTreeSet::new();
    let mut stack = vec![seed];
    while let Some(s) = stack.pop() {
        if !region.contains(&s) || !reached.insert(s) {
            continue;
        }
        stack.extend(successors_within(cfg, s, region));
    }
    reached
}

fn reach_excl(
    cfg: &Cfg,
    seed: usize,
    region: &BTreeSet<usize>,
    exclude_into: Option<usize>,
) -> BTreeSet<usize> {
    let mut reached = BTreeSet::new();
    let mut stack = vec![seed];
    while let Some(s) = stack.pop() {
        if !region.contains(&s) || !reached.insert(s) {
            continue;
        }
        stack.extend(successors_excl(cfg, s, region, exclude_into));
    }
    reached
}

fn predecessors_excl(
    cfg: &Cfg,
    region: &BTreeSet<usize>,
    exclude_into: Option<usize>,
) -> HashMap<usize, Vec<usize>> {
    let mut preds: HashMap<usize, Vec<usize>> = HashMap::new();
    for &s in region {
        for t in successors_excl(cfg, s, region, exclude_into) {
            preds.entry(t).or_default().push(s);
        }
    }
    preds
}

fn reverse_reach_within(
    seed: usize,
    region: &BTreeSet<usize>,
    preds: &HashMap<usize, Vec<usize>>,
) -> BTreeSet<usize> {
    let mut reached = BTreeSet::new();
    let mut stack = vec![seed];
    while let Some(s) = stack.pop() {
        if !region.contains(&s) || !reached.insert(s) {
            continue;
        }
        if let Some(p) = preds.get(&s) {
            stack.extend(p.iter().copied());
        }
    }
    reached
}

fn sccs_within(
    cfg: &Cfg,
    region: &BTreeSet<usize>,
    exclude_into: Option<usize>,
) -> Vec<BTreeSet<usize>> {
    let preds = predecessors_excl(cfg, region, exclude_into);
    let mut seen = BTreeSet::new();
    let mut out = Vec::new();
    for &s in region {
        if seen.contains(&s) {
            continue;
        }
        let fwd = reach_excl(cfg, s, region, exclude_into);
        let bwd = reverse_reach_within(s, region, &preds);
        let scc: BTreeSet<usize> = fwd.intersection(&bwd).copied().collect();
        let self_loop = successors_excl(cfg, s, region, exclude_into).contains(&s);
        if scc.len() > 1 || self_loop {
            seen.extend(scc.iter().copied());
            out.push(scc);
        } else {
            seen.insert(s);
        }
    }
    out
}

fn scc_entries(
    cfg: &Cfg,
    scc: &BTreeSet<usize>,
    region: &BTreeSet<usize>,
    entry: usize,
) -> BTreeSet<usize> {
    let mut out = BTreeSet::new();
    for &s in region {
        if scc.contains(&s) {
            continue;
        }
        for t in successors_within(cfg, s, region) {
            if scc.contains(&t) {
                out.insert(t);
            }
        }
    }
    if scc.contains(&entry) {
        out.insert(entry);
    }
    out
}

enum Unit {
    Node(usize),
    Loop { header: usize, body: Vec<Unit> },
    Irreducible { members: BTreeSet<usize> },
}

fn representative(unit: &Unit) -> usize {
    match unit {
        Unit::Node(s) => *s,
        Unit::Loop { header, .. } => *header,
        Unit::Irreducible { members } => *members.iter().next().unwrap(),
    }
}

fn schedule(
    cfg: &Cfg,
    universe: &BTreeSet<usize>,
    entry: usize,
    exclude_into: Option<usize>,
) -> Option<Vec<Unit>> {
    let reachable = reach_within(cfg, entry, universe);
    if reachable.is_empty() {
        return Some(Vec::new());
    }
    let sccs = sccs_within(cfg, &reachable, exclude_into);
    let mut scc_of: HashMap<usize, usize> = HashMap::new();
    for (i, scc) in sccs.iter().enumerate() {
        for &s in scc {
            scc_of.insert(s, i);
        }
    }

    #[derive(Clone, Copy, PartialEq, Eq, Hash)]
    enum Cond {
        Node(usize),
        Scc(usize),
    }
    let node_of = |s: usize| {
        scc_of
            .get(&s)
            .map(|&i| Cond::Scc(i))
            .unwrap_or(Cond::Node(s))
    };
    let cond_successors = |c: Cond| -> Vec<Cond> {
        let members: Vec<usize> = match c {
            Cond::Node(s) => vec![s],
            Cond::Scc(i) => sccs[i].iter().copied().collect(),
        };
        let mut out = Vec::new();
        for &m in &members {
            for t in successors_excl(cfg, m, &reachable, exclude_into) {
                if members.contains(&t) {
                    continue;
                }
                let cn = node_of(t);
                if !out.contains(&cn) {
                    out.push(cn);
                }
            }
        }
        out
    };

    fn visit(
        c: Cond,
        cond_successors: &dyn Fn(Cond) -> Vec<Cond>,
        visited: &mut HashSet<Cond>,
        postorder: &mut Vec<Cond>,
    ) {
        if !visited.insert(c) {
            return;
        }
        for n in cond_successors(c) {
            visit(n, cond_successors, visited, postorder);
        }
        postorder.push(c);
    }
    let mut visited = HashSet::new();
    let mut postorder = Vec::new();
    visit(
        node_of(entry),
        &cond_successors,
        &mut visited,
        &mut postorder,
    );
    postorder.reverse();

    let mut units = Vec::new();
    for c in postorder {
        match c {
            Cond::Node(s) => units.push(Unit::Node(s)),
            Cond::Scc(i) => {
                let scc = &sccs[i];
                let entries = scc_entries(cfg, scc, &reachable, entry);
                if entries.len() == 1 {
                    let header = *entries.iter().next().unwrap();
                    let body = schedule(cfg, scc, header, Some(header))?;
                    units.push(Unit::Loop { header, body });
                } else {
                    units.push(Unit::Irreducible {
                        members: scc.clone(),
                    });
                }
            }
        }
    }
    Some(units)
}

enum LoopCtx {
    Natural {
        header: usize,
        label: Label,
    },
    Irreducible {
        members: BTreeSet<usize>,
        label: Label,
        block_var: Ident,
    },
}

struct RenderCtx {
    base: String,
    counter: usize,
}

impl RenderCtx {
    fn fresh_label(&mut self) -> Label {
        let n = self.counter;
        self.counter += 1;
        Label::new(format!("{}_l{n}", self.base))
    }

    fn fresh_ident(&mut self) -> Ident {
        let n = self.counter;
        self.counter += 1;
        Ident::new(format!("{}_v{n}", self.base))
    }
}

fn predecessors_of(cfg: &Cfg, target: usize) -> Vec<usize> {
    (0..cfg.states.len())
        .filter(|&s| cfg.successors(s).contains(&target))
        .collect()
}

fn needs_label(cfg: &Cfg, units: &[Unit], i: usize) -> bool {
    if i == 0 {
        return false;
    }
    let Unit::Node(s) = &units[i] else {
        return true;
    };
    let prev = representative(&units[i - 1]);
    let preds = predecessors_of(cfg, *s);
    preds != [prev]
}

fn assign_incoming_labels(
    cfg: &Cfg,
    units: &[Unit],
    ctx: &mut RenderCtx,
    state_label: &mut HashMap<usize, Label>,
    irreducible_var: &mut HashMap<usize, Ident>,
) {
    for (i, u) in units.iter().enumerate() {
        if needs_label(cfg, units, i) {
            let label = ctx.fresh_label();
            match u {
                Unit::Node(s) => {
                    state_label.insert(*s, label);
                }
                Unit::Loop { header, .. } => {
                    state_label.insert(*header, label);
                }
                Unit::Irreducible { members } => {
                    for &m in members {
                        state_label.insert(m, label.clone());
                    }
                }
            }
        }
        if let Unit::Irreducible { members } = u {
            let var = ctx.fresh_ident();
            for &m in members {
                irreducible_var.insert(m, var);
            }
        }
        if let Unit::Loop { body, .. } = u {
            assign_incoming_labels(cfg, body, ctx, state_label, irreducible_var);
        }
    }
}

fn to_indent(stmts: Vec<Stmt>) -> Vec<IndentStmt> {
    stmts
        .into_iter()
        .map(|stmt| IndentStmt { depth: 0, stmt })
        .collect()
}

fn int_expr(n: usize) -> Expr {
    Expr::Value(RustValue::I64(n as i64))
}

fn resolve(
    target: usize,
    state_label: &HashMap<usize, Label>,
    irreducible_var: &HashMap<usize, Ident>,
    loop_stack: &[LoopCtx],
) -> Vec<Stmt> {
    for lc in loop_stack.iter().rev() {
        match lc {
            LoopCtx::Natural { header, label } if *header == target => {
                return vec![Stmt::Continue(Some(label.clone()))];
            }
            LoopCtx::Irreducible {
                members,
                label,
                block_var,
            } if members.contains(&target) => {
                return vec![
                    Stmt::Assign {
                        target: Expr::Var(*block_var),
                        value: int_expr(target),
                    },
                    Stmt::Continue(Some(label.clone())),
                ];
            }
            _ => {}
        }
    }
    if let Some(&var) = irreducible_var.get(&target) {
        let label = state_label
            .get(&target)
            .cloned()
            .unwrap_or_else(|| unreachable!("irreducible member without a unit label"));
        return vec![
            Stmt::Assign {
                target: Expr::Var(var),
                value: int_expr(target),
            },
            Stmt::Break(Some(label)),
        ];
    }
    match state_label.get(&target) {
        Some(label) => vec![Stmt::Break(Some(label.clone()))],
        None => Vec::new(),
    }
}

fn render_flow(
    flow: &Flow,
    state_label: &HashMap<usize, Label>,
    irreducible_var: &HashMap<usize, Ident>,
    loop_stack: &[LoopCtx],
    exit_label: &Label,
) -> Vec<Stmt> {
    let mut out = flow.prefix.clone();
    match &flow.transfer {
        Transfer::Goto(t) => out.extend(resolve(*t, state_label, irreducible_var, loop_stack)),
        Transfer::Exit => out.push(Stmt::Break(Some(exit_label.clone()))),
        Transfer::Return(v) => out.push(Stmt::Return(v.clone())),
        Transfer::Diverge(s) => out.push(s.clone()),
        Transfer::Branch { cond, then_, else_ } => {
            let then_body =
                render_flow(then_, state_label, irreducible_var, loop_stack, exit_label);
            let else_body =
                render_flow(else_, state_label, irreducible_var, loop_stack, exit_label);
            out.push(Stmt::If {
                cond: cond.clone(),
                then_body: to_indent(then_body),
                else_body: to_indent(else_body),
            });
        }
        Transfer::Switch { expr, arms } => {
            let arms = arms
                .iter()
                .map(|(pattern, f)| MatchArm {
                    pattern: pattern.clone(),
                    body: to_indent(render_flow(
                        f,
                        state_label,
                        irreducible_var,
                        loop_stack,
                        exit_label,
                    )),
                })
                .collect();
            out.push(Stmt::Match {
                expr: expr.clone(),
                arms,
            });
        }
    }
    out
}

fn render_units(
    cfg: &Cfg,
    units: &[Unit],
    ctx: &mut RenderCtx,
    state_label: &HashMap<usize, Label>,
    irreducible_var: &HashMap<usize, Ident>,
    loop_stack: &mut Vec<LoopCtx>,
    exit_label: &Label,
) -> Option<Vec<Stmt>> {
    let mut rendered: Vec<Vec<Stmt>> = Vec::new();
    for u in units {
        let stmts = match u {
            Unit::Node(s) => render_flow(
                &cfg.states[*s],
                state_label,
                irreducible_var,
                loop_stack,
                exit_label,
            ),
            Unit::Loop { header, body } => {
                let label = ctx.fresh_label();
                loop_stack.push(LoopCtx::Natural {
                    header: *header,
                    label: label.clone(),
                });
                let inner = render_units(
                    cfg,
                    body,
                    ctx,
                    state_label,
                    irreducible_var,
                    loop_stack,
                    exit_label,
                )?;
                loop_stack.pop();
                vec![Stmt::Loop {
                    label: Some(label),
                    body: to_indent(inner),
                }]
            }
            Unit::Irreducible { members } => {
                let label = ctx.fresh_label();
                let var = *irreducible_var.get(members.iter().next().unwrap())?;
                loop_stack.push(LoopCtx::Irreducible {
                    members: members.clone(),
                    label: label.clone(),
                    block_var: var,
                });
                let mut arms = Vec::new();
                for &m in members {
                    let body = render_flow(
                        &cfg.states[m],
                        state_label,
                        irreducible_var,
                        loop_stack,
                        exit_label,
                    );
                    arms.push(MatchArm {
                        pattern: Pattern::I64(m as i64),
                        body: to_indent(body),
                    });
                }
                arms.push(MatchArm {
                    pattern: Pattern::Wildcard,
                    body: to_indent(vec![Stmt::Expr(Expr::Macro {
                        name: "unreachable".into(),
                        args: Vec::new(),
                    })]),
                });
                loop_stack.pop();
                vec![Stmt::Loop {
                    label: Some(label),
                    body: to_indent(vec![Stmt::Match {
                        expr: Expr::Var(var),
                        arms,
                    }]),
                }]
            }
        };
        rendered.push(stmts);
    }
    Some(wrap_all(&rendered, units, state_label))
}

fn wrap_all(
    rendered: &[Vec<Stmt>],
    units: &[Unit],
    state_label: &HashMap<usize, Label>,
) -> Vec<Stmt> {
    if rendered.is_empty() {
        return Vec::new();
    }
    if rendered.len() == 1 {
        return rendered[0].clone();
    }
    let last = rendered[rendered.len() - 1].clone();
    let last_state = representative(&units[units.len() - 1]);
    let inner = wrap_all(
        &rendered[..rendered.len() - 1],
        &units[..units.len() - 1],
        state_label,
    );
    let mut out = match state_label.get(&last_state) {
        Some(label) => vec![Stmt::LabeledBlock {
            label: label.clone(),
            body: to_indent(inner),
        }],
        None => inner,
    };
    out.extend(last);
    out
}

fn prime_entry(units: &[Unit], entry: usize, irreducible_var: &HashMap<usize, Ident>) -> Vec<Stmt> {
    let Some(Unit::Irreducible { members }) = units.first() else {
        return Vec::new();
    };
    if !members.contains(&entry) {
        return Vec::new();
    }
    let Some(&var) = irreducible_var.get(&entry) else {
        return Vec::new();
    };
    vec![Stmt::Assign {
        target: Expr::Var(var),
        value: int_expr(entry),
    }]
}

fn declare_irreducible_vars(irreducible_var: &HashMap<usize, Ident>) -> Vec<Stmt> {
    let mut seen = HashSet::new();
    let mut out = Vec::new();
    for &var in irreducible_var.values() {
        if seen.insert(var) {
            out.push(Stmt::Let {
                name: var.as_str().to_string(),
                mutable: true,
                ty: None,
                init: Some(int_expr(0)),
            });
        }
    }
    out
}

fn structure(cfg: &Cfg, entry: usize, base: &str) -> Option<Vec<Stmt>> {
    let universe: BTreeSet<usize> = (0..cfg.states.len()).collect();
    let units = schedule(cfg, &universe, entry, None)?;
    if units.is_empty() {
        return None;
    }
    let mut ctx = RenderCtx {
        base: base.to_string(),
        counter: 0,
    };
    let mut state_label = HashMap::new();
    let mut irreducible_var = HashMap::new();
    assign_incoming_labels(cfg, &units, &mut ctx, &mut state_label, &mut irreducible_var);
    let exit_label = ctx.fresh_label();
    let mut loop_stack = Vec::new();
    let mut out = declare_irreducible_vars(&irreducible_var);
    out.extend(prime_entry(&units, entry, &irreducible_var));
    let body = render_units(
        cfg,
        &units,
        &mut ctx,
        &state_label,
        &irreducible_var,
        &mut loop_stack,
        &exit_label,
    )?;
    out.push(Stmt::LabeledBlock {
        label: exit_label,
        body: to_indent(body),
    });
    Some(out)
}

fn const_usize(expr: &Expr) -> Option<usize> {
    match expr {
        Expr::Value(RustValue::I64(v)) if *v >= 0 => Some(*v as usize),
        Expr::Value(RustValue::I128(v)) if *v >= 0 => Some(*v as usize),
        Expr::Value(RustValue::Usize(v)) => Some(*v),
        _ => None,
    }
}

fn expr_diverges(expr: &Expr) -> bool {
    match expr {
        Expr::Macro { name, .. } => matches!(
            name.as_str(),
            "unreachable" | "panic" | "todo" | "unimplemented"
        ),
        Expr::Call { func, .. } => callee_name(func)
            .as_deref()
            .is_some_and(|name| name.ends_with("process::exit") || name == "abort"),
        _ => false,
    }
}

fn callee_name(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Var(ident) => Some(ident.as_str().to_string()),
        Expr::Path(path) => Some(
            path.segments
                .iter()
                .map(|s| s.as_str())
                .collect::<Vec<_>>()
                .join("::"),
        ),
        _ => None,
    }
}

fn as_stmts(v: &[IndentStmt]) -> Vec<Stmt> {
    v.iter().map(|s| s.stmt.clone()).collect()
}

fn parse_flow(stmts: &[Stmt], state_var: Ident, loop_label: &Label) -> Option<Flow> {
    let mut prefix = Vec::new();
    let mut i = 0;
    while i < stmts.len() {
        match &stmts[i] {
            Stmt::Assign {
                target: Expr::Var(v),
                value,
            } if *v == state_var => {
                let Some(Stmt::Continue(Some(l))) = stmts.get(i + 1) else {
                    return None;
                };
                if l.as_str() != loop_label.as_str() {
                    return None;
                }
                let target = const_usize(value)?;
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Goto(target),
                });
            }
            Stmt::Return(v) => {
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Return(v.clone()),
                });
            }
            Stmt::Break(Some(l)) if l.as_str() == loop_label.as_str() => {
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Exit,
                });
            }
            Stmt::Break(_) | Stmt::Continue(_) => {
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Diverge(stmts[i].clone()),
                });
            }
            Stmt::Expr(e) if expr_diverges(e) => {
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Diverge(stmts[i].clone()),
                });
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                let rest = &stmts[i + 1..];
                let mut then_stmts = as_stmts(then_body);
                then_stmts.extend_from_slice(rest);
                let mut else_stmts = as_stmts(else_body);
                else_stmts.extend_from_slice(rest);
                let then_ = parse_flow(&then_stmts, state_var, loop_label)?;
                let else_ = parse_flow(&else_stmts, state_var, loop_label)?;
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Branch {
                        cond: cond.clone(),
                        then_: Box::new(then_),
                        else_: Box::new(else_),
                    },
                });
            }
            Stmt::Match { expr, arms } => {
                let rest = &stmts[i + 1..];
                let mut parsed = Vec::new();
                for arm in arms {
                    let mut arm_stmts = as_stmts(&arm.body);
                    arm_stmts.extend_from_slice(rest);
                    let flow = parse_flow(&arm_stmts, state_var, loop_label)?;
                    parsed.push((arm.pattern.clone(), flow));
                }
                return Some(Flow {
                    prefix,
                    transfer: Transfer::Switch {
                        expr: expr.clone(),
                        arms: parsed,
                    },
                });
            }
            Stmt::Scope { body } => {
                let mut combined = as_stmts(body);
                combined.extend(stmts[i + 1..].iter().cloned());
                return parse_flow(&combined, state_var, loop_label);
            }
            other if stmt_contains_asm_target(other, state_var, loop_label) => {
                return None;
            }
            _ => {
                prefix.push(stmts[i].clone());
                i += 1;
            }
        }
    }
    None
}

fn asm_targets_dispatch(
    asm: &crate::backend::rust_ast::InlineAsm,
    state_var: Ident,
    loop_label: &Label,
) -> bool {
    asm.operands.iter().any(|op| {
        matches!(
            op,
            crate::backend::rust_ast::AsmOperand::Label { state, destination, .. }
                if matches!(state, Expr::Var(v) if *v == state_var) && destination.as_str() == loop_label.as_str()
        )
    })
}

fn stmt_contains_asm_target(stmt: &Stmt, state_var: Ident, loop_label: &Label) -> bool {
    match stmt {
        Stmt::InlineAsm(asm) => asm_targets_dispatch(asm, state_var, loop_label),
        Stmt::Unsafe { body } => body
            .stmts
            .iter()
            .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label)),
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => body
            .iter()
            .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label)),
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            then_body
                .iter()
                .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label))
                || else_body
                    .iter()
                    .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label))
        }
        Stmt::Match { arms, .. } => arms.iter().any(|arm| {
            arm.body
                .iter()
                .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label))
        }),
        Stmt::For { body, .. } => body
            .iter()
            .any(|s| stmt_contains_asm_target(&s.stmt, state_var, loop_label)),
        _ => false,
    }
}

fn subtree_ids(arena: &Arena, id: NodeId, out: &mut Vec<NodeId>) {
    out.push(id);
    let Some(kind) = arena.get(id) else { return };
    for &child in kind.child_lists().into_iter().flatten() {
        subtree_ids(arena, child, out);
    }
}

struct Plan {
    remove: Vec<NodeId>,
    loop_id: NodeId,
    entry: usize,
    cfg: Cfg,
    base: String,
}

fn plan_goto(arena: &Arena, loop_id: NodeId) -> Option<Plan> {
    let Some(NodeKind::Loop {
        label: Some(loop_label),
        body,
    }) = arena.get(loop_id)
    else {
        return None;
    };
    let [match_id] = &body[..] else { return None };
    let Some(NodeKind::Match {
        expr: Expr::Var(state_var),
        arms,
    }) = arena.get(*match_id)
    else {
        return None;
    };
    let state_var = *state_var;
    let loop_label = loop_label.clone();

    let mut arm_bodies: Vec<Vec<Stmt>> = Vec::new();
    for arm in arms {
        match &arm.pattern {
            Pattern::Wildcard => {}
            Pattern::I64(v) if *v as usize == arm_bodies.len() => {
                let stmts: Option<Vec<Stmt>> = arm
                    .body
                    .iter()
                    .map(|&id| arena::reify_bare(arena, id))
                    .collect();
                arm_bodies.push(stmts?);
            }
            _ => return None,
        }
    }
    if arm_bodies.len() < 2 {
        return None;
    }

    let def_id = arena.definition(state_var)?;
    let Some(NodeKind::Let {
        init: Some(init_expr),
        ..
    }) = arena.get(def_id)
    else {
        return None;
    };
    let entry = const_usize(init_expr)?;

    let mut subtree = Vec::new();
    subtree_ids(arena, loop_id, &mut subtree);
    if !arena
        .def_use_neighbors(state_var)
        .iter()
        .all(|n| subtree.contains(n))
    {
        return None;
    }

    let mut states = Vec::new();
    for stmts in &arm_bodies {
        states.push(parse_flow(stmts, state_var, &loop_label)?);
    }

    Some(Plan {
        remove: vec![def_id],
        loop_id,
        entry,
        cfg: Cfg { states },
        base: loop_label.as_str().to_string(),
    })
}

fn plan_switch(arena: &Arena, loop_id: NodeId) -> Option<Plan> {
    let Some(NodeKind::Loop {
        label: Some(switch_label),
        body,
    }) = arena.get(loop_id)
    else {
        return None;
    };
    let [match_id] = &body[..] else {
        return None;
    };
    let Some(NodeKind::Match {
        expr: Expr::Var(case_var),
        arms,
    }) = arena.get(*match_id)
    else {
        return None;
    };
    let case_var = *case_var;
    let switch_label = switch_label.clone();

    let parent_id = arena.parent(loop_id)?;
    let parent_kind = arena.get(parent_id)?;
    let siblings = parent_kind
        .child_lists()
        .into_iter()
        .find(|list| list.contains(&loop_id))?;
    let pos = siblings.iter().position(|&x| x == loop_id)?;
    if pos < 2 {
        return None;
    }
    let case_let_id = siblings[pos - 1];
    let selector_let_id = siblings[pos - 2];
    let Some(NodeKind::Let {
        name: case_decl,
        init:
            Some(Expr::Match {
                expr: selector_ref,
                arms: selector_arms,
            }),
        ..
    }) = arena.get(case_let_id)
    else {
        return None;
    };
    if *case_decl != case_var {
        return None;
    }
    let Some(NodeKind::Let {
        name: selector_name,
        init: Some(selector_expr),
        ..
    }) = arena.get(selector_let_id)
    else {
        return None;
    };
    let Expr::Var(sv) = selector_ref.as_ref() else {
        return None;
    };
    if sv != selector_name {
        return None;
    }

    let mut arm_bodies: Vec<Vec<Stmt>> = Vec::new();
    for arm in arms {
        match &arm.pattern {
            Pattern::Wildcard => {}
            Pattern::I64(v) if *v as usize == arm_bodies.len() => {
                let stmts: Option<Vec<Stmt>> = arm
                    .body
                    .iter()
                    .map(|&id| arena::reify_bare(arena, id))
                    .collect();
                arm_bodies.push(stmts?);
            }
            _ => return None,
        }
    }
    if arm_bodies.is_empty() {
        return None;
    }

    let mut states = Vec::new();
    for stmts in &arm_bodies {
        states.push(parse_flow(stmts, case_var, &switch_label)?);
    }
    let entry_index = states.len();

    let mut entry_arms = Vec::new();
    for arm in selector_arms {
        let idx = match const_usize(&arm.value) {
            Some(idx) if idx < states.len() => idx,
            _ => {
                entry_arms.push((
                    arm.pattern.clone(),
                    Flow {
                        prefix: Vec::new(),
                        transfer: Transfer::Exit,
                    },
                ));
                continue;
            }
        };
        entry_arms.push((
            arm.pattern.clone(),
            Flow {
                prefix: Vec::new(),
                transfer: Transfer::Goto(idx),
            },
        ));
    }
    states.push(Flow {
        prefix: Vec::new(),
        transfer: Transfer::Switch {
            expr: selector_expr.clone(),
            arms: entry_arms,
        },
    });

    let mut subtree = Vec::new();
    subtree_ids(arena, loop_id, &mut subtree);
    if !arena
        .def_use_neighbors(case_var)
        .iter()
        .all(|n| subtree.contains(n))
    {
        return None;
    }

    Some(Plan {
        remove: vec![selector_let_id, case_let_id],
        loop_id,
        entry: entry_index,
        cfg: Cfg { states },
        base: switch_label.as_str().to_string(),
    })
}

fn plan(arena: &Arena, loop_id: NodeId) -> Option<Plan> {
    plan_goto(arena, loop_id).or_else(|| plan_switch(arena, loop_id))
}

pub(in crate::backend::engine) struct StructureDispatch;

impl NodeRule for StructureDispatch {
    fn name(&self) -> &'static str {
        "structure_dispatch::structure"
    }

    fn priority(&self) -> u32 {
        1
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        structure(&plan.cfg, plan.entry, &plan.base).is_some()
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        let Some(replacement) = structure(&plan.cfg, plan.entry, &plan.base) else {
            return false;
        };
        let Some(parent_id) = arena.parent(plan.loop_id) else {
            return false;
        };
        let Some(parent_kind) = arena.get(parent_id) else {
            return false;
        };
        let Some((list_index, insert_at)) =
            parent_kind
                .child_lists()
                .iter()
                .enumerate()
                .find_map(|(list_index, list)| {
                    list.iter()
                        .position(|&x| x == plan.loop_id)
                        .map(|loop_pos| {
                            let mut remove_set: BTreeSet<NodeId> =
                                plan.remove.iter().copied().collect();
                            remove_set.insert(plan.loop_id);
                            let insert_at = list[..loop_pos]
                                .iter()
                                .filter(|x| !remove_set.contains(x))
                                .count();
                            (list_index, insert_at)
                        })
                })
        else {
            return false;
        };

        for &r in &plan.remove {
            arena.take(r);
        }
        arena.discard_subtree(plan.loop_id);
        let new_ids = arena::insert_stmts(arena, Some(parent_id), replacement);

        let Some(parent_kind) = arena.get_mut(parent_id) else {
            return false;
        };
        let mut lists = parent_kind.child_lists_mut();
        let Some(list) = lists.get_mut(list_index) else {
            return false;
        };
        let mut remove_set: BTreeSet<NodeId> = plan.remove.iter().copied().collect();
        remove_set.insert(plan.loop_id);
        list.retain(|x| !remove_set.contains(x));
        let insert_at = insert_at.min(list.len());
        list.splice(insert_at..insert_at, new_ids);
        true
    }
}
