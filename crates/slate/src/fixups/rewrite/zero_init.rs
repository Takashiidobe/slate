//! Fuse a zero-initialized declaration with the assignment that immediately
//! overwrites it, when the assignment does not read the placeholder.

use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, BindingId, ConstValue, EffectSubject, FixupFacts, FunctionId, PathSegment,
    PlaceAccess, PlaceKind, Purity, ValueSubject,
};
use crate::fixups::idents::expr_ident;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, binding_facts, fact, function_path_location,
    path_fact, stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Stmt, UnaryOp};

pub(in crate::fixups) struct ZeroInit<'a> {
    cross_effects: bool,
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for ZeroInit<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_at(body, self.function, self.facts, &mut Vec::new())
    }
}

impl<'a> ZeroInit<'a> {
    fn fixup_at(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        if self.fixup_nested(body, function, facts, path) {
            return true;
        }
        for i in 0..body.len().saturating_sub(1) {
            let decl_path = stmt_path(path, i);
            let Stmt::Let {
                name,
                mutable: true,
                ty: Some(ty),
                init: Some(_),
            } = &body[i].stmt
            else {
                continue;
            };
            let name = name.clone();
            let ty = ty.clone();
            let Some(binding) =
                facts.binding_by_local_path(function, &name, &AstPath(decl_path.clone()))
            else {
                continue;
            };
            if !binding_is_zero(function, facts, binding, &decl_path) {
                continue;
            }
            let mut move_decl_to_assignment = false;
            let ctx = ZeroInitContext {
                function,
                facts,
                binding,
                cross_effects: self.cross_effects,
                body_path: path,
            };
            let mut assign_index = first_overwriting_assignment(body, i, &name, &ctx);
            if assign_index.is_none() && self.cross_effects {
                assign_index =
                    first_deferred_initialization(body, i, &name, function, facts, binding, path);
                move_decl_to_assignment = assign_index.is_some();
            }
            let Some(assign_index) = assign_index else {
                continue;
            };
            let Stmt::Assign { value, .. } = &body[assign_index].stmt else {
                unreachable!();
            };
            let assign_path = stmt_path(path, assign_index);
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| (body[i].stmt.clone(), body[assign_index].stmt.clone()));
            let value = value.clone();
            if move_decl_to_assignment {
                let depth = body[assign_index].depth;
                body[assign_index] = IndentStmt {
                    depth,
                    stmt: Stmt::Let {
                        name,
                        mutable: true,
                        ty: Some(ty),
                        init: Some(value),
                    },
                };
                if let Some((before_decl, before_assign)) = trace_before {
                    let after_decl = body[assign_index].stmt.clone();
                    self.log_zero_init_event(
                        function,
                        facts,
                        binding,
                        &decl_path,
                        &assign_path,
                        move_decl_to_assignment,
                        &before_decl,
                        &before_assign,
                        &after_decl,
                    );
                }
                body.remove(i);
            } else if let Stmt::Let { init, .. } = &mut body[i].stmt {
                *init = Some(value);
                if let Some((before_decl, before_assign)) = trace_before {
                    let after_decl = body[i].stmt.clone();
                    self.log_zero_init_event(
                        function,
                        facts,
                        binding,
                        &decl_path,
                        &assign_path,
                        move_decl_to_assignment,
                        &before_decl,
                        &before_assign,
                        &after_decl,
                    );
                }
                body.remove(assign_index);
            }
            return true;
        }
        false
    }

    fn fixup_nested(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        for (index, stmt) in body.iter_mut().enumerate() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |body, path| {
                    if !changed && self.fixup_at(body, function, facts, path) {
                        changed = true;
                    }
                });
            });
            if changed {
                return true;
            }
        }
        false
    }

    #[allow(clippy::too_many_arguments)]
    fn log_zero_init_event(
        &mut self,
        function: FunctionId,
        facts: &FixupFacts,
        binding: BindingId,
        decl_path: &[PathSegment],
        assign_path: &[PathSegment],
        moved_decl: bool,
        before_decl: &Stmt,
        before_assign: &Stmt,
        after_decl: &Stmt,
    ) {
        if !self.logger.is_enabled() {
            return;
        }
        let mut event_facts = binding_facts(facts, binding);
        event_facts.extend([
            path_fact("decl_path", decl_path),
            path_fact("assign_path", assign_path),
            fact("binding_is_zero", "true"),
            fact("cross_effects", self.cross_effects.to_string()),
            fact("moved_decl_to_assignment", moved_decl.to_string()),
            fact(
                "assignment_reads_binding",
                assignment_reads_binding(facts, binding, assign_path).to_string(),
            ),
            fact(
                "assignment_writes_binding",
                assignment_writes_binding(function, facts, binding, assign_path).to_string(),
            ),
        ]);
        self.logger.rewrite(RewriteEvent {
            pass: TracePass::ZeroInit,
            kind: "fold_zero_init_assignment".into(),
            location: function_path_location(facts, function, assign_path),
            before: vec![
                stmt_snippet("declaration", before_decl),
                stmt_snippet("assignment", before_assign),
            ],
            after: vec![stmt_snippet("declaration", after_decl)],
            facts: event_facts,
        });
    }

    pub(in crate::fixups) fn new(
        cross_effects: bool,
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            cross_effects,
            function,
            facts,
            logger,
        }
    }
}

struct ZeroInitContext<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    binding: BindingId,
    cross_effects: bool,
    body_path: &'a [PathSegment],
}

fn first_overwriting_assignment(
    body: &[IndentStmt],
    decl_index: usize,
    name: &str,
    ctx: &ZeroInitContext<'_>,
) -> Option<usize> {
    let (assign_index, value) =
        body.iter()
            .enumerate()
            .skip(decl_index + 1)
            .find_map(|(index, indent)| match &indent.stmt {
                Stmt::Assign { target, value } if expr_ident(target) == Some(name) => {
                    Some((index, value))
                }
                _ => None,
            })?;
    let assign_path = stmt_path(ctx.body_path, assign_index);
    if !assignment_writes_binding(ctx.function, ctx.facts, ctx.binding, &assign_path)
        || assignment_reads_binding(ctx.facts, ctx.binding, &assign_path)
        || assignment_reads_intervening_binding(
            body,
            decl_index,
            assign_index,
            ctx.function,
            ctx.facts,
            ctx.body_path,
            &assign_path,
        )
    {
        return None;
    }
    let value_reads_nothing = reads_nothing(value);
    for (index, indent) in body
        .iter()
        .enumerate()
        .take(assign_index)
        .skip(decl_index + 1)
    {
        let path = stmt_path(ctx.body_path, index);
        if can_cross_intervening_stmt(ctx.function, ctx.facts, ctx.binding, &path, &indent.stmt) {
            continue;
        }
        // a constant store reads nothing, so removing it is safe past any effect
        // as long as the crossed statement neither reads nor writes the binding.
        if ctx.cross_effects
            && value_reads_nothing
            && !assignment_reads_binding(ctx.facts, ctx.binding, &path)
            && !binding_written_under(ctx.facts, ctx.binding, &path)
        {
            continue;
        }
        return None;
    }
    Some(assign_index)
}

fn first_deferred_initialization(
    body: &[IndentStmt],
    decl_index: usize,
    name: &str,
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    body_path: &[PathSegment],
) -> Option<usize> {
    let (assign_index, _) =
        body.iter()
            .enumerate()
            .skip(decl_index + 1)
            .find_map(|(index, indent)| match &indent.stmt {
                Stmt::Assign { target, value } if expr_ident(target) == Some(name) => {
                    Some((index, value))
                }
                _ => None,
            })?;
    let assign_path = stmt_path(body_path, assign_index);
    if !assignment_writes_binding(function, facts, binding, &assign_path)
        || !can_move_decl_to_assignment(
            decl_index,
            assign_index,
            facts,
            binding,
            body_path,
            &assign_path,
        )
    {
        return None;
    }
    Some(assign_index)
}

fn can_move_decl_to_assignment(
    decl_index: usize,
    assign_index: usize,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    body_path: &[PathSegment],
    assign_path: &[PathSegment],
) -> bool {
    if assignment_reads_binding(facts, binding, assign_path) {
        return false;
    }
    (decl_index + 1..assign_index).all(|index| {
        let path = stmt_path(body_path, index);
        !assignment_reads_binding(facts, binding, &path)
            && !binding_written_under(facts, binding, &path)
    })
}

fn reads_nothing(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Cast { expr, .. } => reads_nothing(expr),
        Expr::Unary { op, expr } => {
            matches!(op, UnaryOp::Neg | UnaryOp::Not) && reads_nothing(expr)
        }
        Expr::Binary { lhs, rhs, .. } => reads_nothing(lhs) && reads_nothing(rhs),
        _ => false,
    }
}

fn binding_written_under(
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.def_use(binding).is_some_and(|fact| {
        fact.writes
            .iter()
            .any(|write| write.0.as_slice().starts_with(path))
    })
}

fn assignment_reads_intervening_binding(
    body: &[IndentStmt],
    decl_index: usize,
    assign_index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    assign_path: &[PathSegment],
) -> bool {
    body.iter()
        .enumerate()
        .take(assign_index)
        .skip(decl_index + 1)
        .any(|(index, indent)| {
            let Stmt::Let { name, .. } = &indent.stmt else {
                return false;
            };
            let path = stmt_path(body_path, index);
            facts
                .binding_by_local_path(function, name, &AstPath(path))
                .and_then(|binding| facts.def_use(binding))
                .is_some_and(|def_use| {
                    def_use
                        .reads
                        .iter()
                        .any(|read| read.0.as_slice().starts_with(assign_path))
                })
        })
}

fn can_cross_intervening_stmt(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
    stmt: &Stmt,
) -> bool {
    let Stmt::Let { init, .. } = stmt else {
        return false;
    };
    if assignment_reads_binding(facts, binding, path) {
        return false;
    }
    init.is_none()
        || facts
            .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
            .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

fn binding_is_zero(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.has_value(
        function,
        ValueSubject::Binding(binding),
        &AstPath(path.to_vec()),
        &ConstValue::Zero,
    )
}

fn assignment_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    facts
        .place(function, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.access == PlaceAccess::Write
                && fact.ordinary_slot
                && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
        })
        && facts.def_use(binding).is_some_and(|fact| {
            fact.writes
                .iter()
                .any(|write| write.0.as_slice().starts_with(path))
        })
}

fn assignment_reads_binding(
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.def_use(binding).is_some_and(|fact| {
        fact.reads
            .iter()
            .any(|read| read.0.as_slice().starts_with(path))
    })
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}
