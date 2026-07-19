//! Rust cleanup passes that run after faithful CIR lowering.

pub(crate) mod facts;
mod idents;
mod rewrite;
mod runtime;
mod support;

#[cfg(test)]
mod test_support;

use crate::rust_ast::{Block, IndentStmt, Item, Program, Stmt};
use std::fmt::Write;

/// A fixup pass that can be individually disabled via [`SkipSet`], for
/// translation-validation regression testing (compare a fixture's output
/// with and without one pass active).
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Pass {
    RangeLoop,
}

impl Pass {
    pub fn name(self) -> &'static str {
        match self {
            Pass::RangeLoop => "range_loop",
        }
    }

    pub fn parse(name: &str) -> Option<Self> {
        match name {
            "range_loop" => Some(Pass::RangeLoop),
            _ => None,
        }
    }
}

#[derive(Debug, Clone, Default)]
pub struct SkipSet(std::collections::HashSet<Pass>);

impl SkipSet {
    pub fn none() -> Self {
        Self::default()
    }

    pub fn skip(pass: Pass) -> Self {
        let mut set = Self::default();
        set.0.insert(pass);
        set
    }

    fn contains(&self, pass: Pass) -> bool {
        self.0.contains(&pass)
    }
}

pub fn apply(program: Program) -> Program {
    apply_with(program, &SkipSet::none())
}

pub fn apply_with(program: Program, skip: &SkipSet) -> Program {
    apply_with_trace(program, skip, None)
}

pub fn debug(program: Program) -> String {
    let mut trace = Vec::new();
    let final_program = apply_with_trace(program, &SkipSet::none(), Some(&mut trace));
    let mut out = String::new();
    for step in trace {
        writeln!(out, "{:<34} {}", step.name, step.summary()).unwrap();
    }
    let final_summary = ProgramSummary::from_program(&final_program);
    writeln!(
        out,
        "final: items={}, stmts={}, temp_lets={}",
        final_summary.items, final_summary.stmts, final_summary.temp_lets
    )
    .unwrap();
    out
}

#[derive(Clone)]
struct DebugStep {
    name: &'static str,
    before: ProgramSummary,
    after: ProgramSummary,
    changed: bool,
}

impl DebugStep {
    fn summary(&self) -> String {
        let changed = if self.changed { "changed" } else { "unchanged" };
        format!(
            "{changed}; stmts {:+}, temp_lets {:+}, items {:+}",
            self.after.stmts as isize - self.before.stmts as isize,
            self.after.temp_lets as isize - self.before.temp_lets as isize,
            self.after.items as isize - self.before.items as isize
        )
    }
}

#[derive(Clone)]
struct ProgramSummary {
    items: usize,
    stmts: usize,
    temp_lets: usize,
}

impl ProgramSummary {
    fn from_program(program: &Program) -> Self {
        Self {
            items: program.items.len(),
            stmts: program_stmt_count(program),
            temp_lets: program_temp_let_count(program),
        }
    }
}

fn apply_with_trace(
    input: Program,
    skip: &SkipSet,
    mut trace: Option<&mut Vec<DebugStep>>,
) -> Program {
    macro_rules! step {
        ($program:ident, $name:literal, $body:block) => {{
            let before_summary = trace
                .as_ref()
                .map(|_| ProgramSummary::from_program(&$program));
            let before_emit = trace.as_ref().map(|_| $program.emit());
            let result = $body;
            if let Some(trace) = trace.as_mut() {
                let after_summary = ProgramSummary::from_program(&$program);
                let changed = before_emit
                    .as_ref()
                    .is_some_and(|before| *before != $program.emit());
                trace.push(DebugStep {
                    name: $name,
                    before: before_summary.unwrap(),
                    after: after_summary,
                    changed,
                });
            }
            result
        }};
    }

    let facts::AnalyzedProgram { program, .. } = facts::analyze(input);
    let mut program = program;
    step!(program, "goto", {
        structure_goto(&mut program);
    });
    step!(program, "early_inline_temps", {
        inline_temps_to_fixpoint(&mut program, InlinePass::Early);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "anonymous_structs", {
        rewrite::anonymous_structs::fixup(&mut program, &facts);
    });
    step!(program, "param_spills", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::param_spills::fixup(f, function, &facts);
            }
        }
    });
    step!(program, "zero_init", {
        zero_init_to_fixpoint(&mut program, false);
    });
    step!(program, "struct_field_init", {
        struct_field_init_to_fixpoint(&mut program);
    });
    step!(program, "singleton_scopes", {
        singleton_scopes_to_fixpoint(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "compound_assign", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::compound_assign::fixup(&mut f.body, function, &facts);
            }
        }
    });
    step!(program, "for_continue", {
        cleanup_for_continues(&mut program);
    });
    step!(program, "singleton_scopes", {
        singleton_scopes_to_fixpoint(&mut program);
    });
    step!(program, "constant_index_casts", {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::constant_index_casts::fixup(&mut f.body);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "unnecessary_casts", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::unnecessary_casts::fixup(&mut f.body, function, &facts);
            }
        }
    });
    step!(program, "call_args", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::call_args::fixup(&mut f.body, function, &facts)
                {
                    changed = true;
                }
            }
            if !changed {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "retval", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::retval::fixup(f, function, &facts);
            }
        }
    });
    step!(program, "final_return_temps", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::final_return_temps::fixup(&mut f.body, function, &facts)
                {
                    changed = true;
                }
            }
            if !changed {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "lazy_singleton", {
        rewrite::lazy_singleton::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "drop_call_results", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::drop_call_results::fixup(&mut f.body, function, &facts);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "string_lift", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::string_lift::fixup(&mut f.body, function, &facts);
            }
        }
    });
    step!(program, "string_params", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::fixup(&mut program, &facts) {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "ptr_len", {
        rewrite::ptr_len::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "slice_index", {
        rewrite::slice_index::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "slice_loop", {
        if rewrite::slice_loop::fixup(&mut program, &facts) {
            late_loop_cleanup(&mut program);
        }
    });
    step!(program, "slice_reduce", {
        if rewrite::slice_reduce::fixup(&mut program) {
            late_loop_cleanup(&mut program);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "range_loop", {
        if !skip.contains(Pass::RangeLoop) && rewrite::range_loop::fixup(&mut program, &facts) {
            late_loop_cleanup(&mut program);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "va_list", {
        rewrite::va_list::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "remove_mut", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::remove_mut::fixup(f, function, &facts);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "string_copy", {
        rewrite::string_copy::fixup(&mut program, &facts);
    });
    step!(program, "string_params", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::fixup(&mut program, &facts) {
                break;
            }
        }
    });
    step!(program, "remove_mut", {
        remove_mut(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "string_libc", {
        rewrite::string_libc::fixup(&mut program, &facts);
    });
    step!(program, "sort_search", {
        rewrite::sort_search::fixup(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "heap_ownership", {
        rewrite::heap_ownership::fixup(&mut program, &facts);
    });
    step!(program, "dead_locals", {
        dead_locals_to_fixpoint(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "remove_mut", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::remove_mut::fixup(f, function, &facts);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "printf_format", {
        rewrite::printf_format::fixup(&mut program, &facts);
    });
    step!(program, "string_params", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::fixup(&mut program, &facts) {
                break;
            }
        }
    });
    step!(program, "remove_mut", {
        remove_mut(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "string_libc", {
        rewrite::string_libc::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "c_strings", {
        rewrite::c_strings::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "stdio", {
        rewrite::stdio::fixup(&mut program, &facts);
    });
    step!(program, "memchr_prelude::fixup_calls", {
        rewrite::memchr_prelude::fixup_calls(&mut program, &facts);
    });
    step!(program, "nullable_pointer", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::nullable_pointer::fixup(&mut program, &facts) {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "string_lift::fixup_c_strings", {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::string_lift::fixup_c_strings(&mut f.body, function, &facts);
            }
        }
    });
    step!(program, "memchr_prelude", {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::memchr_prelude::fixup(f);
            }
        }
    });
    step!(program, "memchr_prelude::prune_unused_helper", {
        rewrite::memchr_prelude::prune_unused_helper(&mut program);
    });
    step!(program, "late_inline_temps", {
        inline_temps_to_fixpoint(&mut program, InlinePass::Late);
    });
    step!(program, "dead_locals", {
        dead_locals_to_fixpoint(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "array_element_pointer_origin", {
        rewrite::array_element_pointer_origin::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "buffer_cursor", {
        rewrite::buffer_cursor::fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "atomic_locals", {
        rewrite::atomic_locals::fixup(&mut program, &facts);
    });
    step!(program, "late_inline_temps", {
        inline_temps_to_fixpoint(&mut program, InlinePass::Late);
    });
    step!(program, "zero_init", {
        zero_init_to_fixpoint(&mut program, true);
    });
    step!(program, "atomic_compare_exchange", {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::atomic_compare_exchange::fixup(&mut f.body, function, &facts)
                {
                    changed = true;
                }
            }
            if !changed {
                break;
            }
        }
    });
    step!(program, "remove_mut", {
        remove_mut(&mut program);
    });
    step!(program, "var_aliases", {
        inline_var_aliases_to_fixpoint(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, "prune_unused_externs", {
        rewrite::prune_unused_externs::fixup(&mut program, &facts);
    });
    step!(program, "unused_items", {
        rewrite::unused_items::fixup(&mut program);
    });
    step!(program, "unused_params", {
        rewrite::unused_params::fixup(&mut program);
    });
    step!(program, "main_zero_exit", {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::main_zero_exit::fixup(f);
            }
        }
    });
    program
}

fn structure_goto(program: &mut Program) {
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            while rewrite::goto::fixup(&mut f.body) {}
        }
    }
}

fn cleanup_for_continues(program: &mut Program) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::for_continue::fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn singleton_scopes_to_fixpoint(program: &mut Program) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::singleton_scopes::fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn zero_init_to_fixpoint(program: &mut Program, cross_effects: bool) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && rewrite::zero_init::fixup(&mut f.body, function, &facts, cross_effects)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn struct_field_init_to_fixpoint(program: &mut Program) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::struct_field_init::fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn late_loop_cleanup(program: &mut Program) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item {
                if rewrite::singleton_scopes::fixup(&mut f.body) {
                    changed = true;
                }
                if let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::dead_locals::fixup(&mut f.body, function, &facts)
                {
                    changed = true;
                }
            }
        }
        if !changed {
            break;
        }
    }
}

fn dead_locals_to_fixpoint(program: &mut Program) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && rewrite::dead_locals::fixup(&mut f.body, function, &facts)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

#[derive(Clone, Copy)]
enum InlinePass {
    Early,
    Late,
}

fn inline_temps_to_fixpoint(program: &mut Program, pass: InlinePass) {
    let inline_round_limit = if program_stmt_count(program) > 2_000 {
        5
    } else {
        usize::MAX
    };
    let mut rounds = 0;
    while rounds < inline_round_limit {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        rounds += 1;
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && match pass {
                    InlinePass::Early => {
                        rewrite::early_inline_temps::fixup(&mut f.body, function, &facts)
                    }
                    InlinePass::Late => {
                        rewrite::late_inline_temps::fixup(&mut f.body, function, &facts)
                    }
                }
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn remove_mut(program: &mut Program) {
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::remove_mut::fixup(f, function, &facts);
        }
    }
}

fn inline_var_aliases_to_fixpoint(program: &mut Program) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::var_aliases::fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn program_stmt_count(program: &Program) -> usize {
    program
        .items
        .iter()
        .map(|item| match item {
            Item::Fn(f) => stmt_count(&f.body),
            _ => 0,
        })
        .sum()
}

fn program_temp_let_count(program: &Program) -> usize {
    program
        .items
        .iter()
        .map(|item| match item {
            Item::Fn(f) => temp_let_count(&f.body),
            _ => 0,
        })
        .sum()
}

fn temp_let_count(stmts: &[IndentStmt]) -> usize {
    stmts
        .iter()
        .map(|stmt| {
            let here = match &stmt.stmt {
                Stmt::Let { name, .. } if is_temp_name(name) => 1,
                _ => 0,
            };
            here + match &stmt.stmt {
                Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::If {
                    then_body,
                    else_body,
                    ..
                } => temp_let_count(then_body) + temp_let_count(else_body),
                Stmt::Loop { body, .. }
                | Stmt::For { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => temp_let_count(body),
                Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
                    block_temp_let_count(body)
                }
                Stmt::Match { arms, .. } => arms.iter().map(|arm| temp_let_count(&arm.body)).sum(),
                _ => 0,
            }
        })
        .sum()
}

fn block_temp_let_count(block: &Block) -> usize {
    temp_let_count(&block.stmts)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn stmt_count(stmts: &[IndentStmt]) -> usize {
    stmts
        .iter()
        .map(|stmt| {
            1 + match &stmt.stmt {
                Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::If {
                    then_body,
                    else_body,
                    ..
                } => stmt_count(then_body) + stmt_count(else_body),
                Stmt::Loop { body, .. }
                | Stmt::For { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => stmt_count(body),
                Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
                    block_stmt_count(body)
                }
                Stmt::Match { arms, .. } => arms.iter().map(|arm| stmt_count(&arm.body)).sum(),
                _ => 0,
            }
        })
        .sum()
}

fn block_stmt_count(block: &Block) -> usize {
    stmt_count(&block.stmts)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Expr, Prim, RustValue, Stmt, Type, UnaryOp};

    #[test]
    fn apply_keeps_migrated_functions_structured() {
        let program = Program {
            items: vec![Item::Fn(migrated_fn(vec![
                Stmt::Let {
                    name: "a".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "b".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "__retval".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "c".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Assign {
                    target: Expr::Var("a".into()),
                    value: Expr::Var("arg0".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("b".into()),
                    value: Expr::Var("arg1".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("c".into()),
                    value: bin(BinOp::Add, Expr::Var("a".into()), Expr::Var("b".into())),
                },
                Stmt::Assign {
                    target: Expr::Var("__retval".into()),
                    value: Expr::Var("c".into()),
                },
                Stmt::Return(Some(Expr::Var("__retval".into()))),
            ]))],
        };

        let out = apply(program);
        let Item::Fn(f) = &out.items[0] else {
            panic!("migrated functions must remain structured");
        };
        assert_eq!(f.params[0].name, "a");
        assert!(!f.params[0].mutable);
        assert_eq!(
            out.emit(),
            "\
fn add(a: i32, b: i32) -> i32 {
    let c: i32 = a + b;
    return c;
}
"
        );
    }

    fn counted_loop_program() -> Program {
        let counted_loop = Stmt::Scope {
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
                                        expr: Box::new(bin(BinOp::Lt, var("i"), var("n"))),
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
                                stmt: Stmt::CompoundAssign {
                                    target: var("total"),
                                    op: BinOp::Add,
                                    value: var("i"),
                                },
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
        };
        Program {
            items: vec![Item::Fn(func(
                vec![param("n", "i32")],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    counted_loop,
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        }
    }

    #[test]
    fn apply_with_none_matches_apply() {
        assert_eq!(
            apply(counted_loop_program()).emit(),
            apply_with(counted_loop_program(), &SkipSet::none()).emit()
        );
    }

    #[test]
    fn apply_rewrites_counted_loop_to_range_for_by_default() {
        let out = apply(counted_loop_program()).emit();
        assert!(out.contains("for i in 0..n"));
        assert!(!out.contains("loop {"));
    }

    #[test]
    fn skip_set_disables_range_loop_pass() {
        let out = apply_with(counted_loop_program(), &SkipSet::skip(Pass::RangeLoop)).emit();
        assert!(!out.contains("for i in 0..n"));
        assert!(out.contains("loop {"));
    }

    #[test]
    fn pass_name_round_trips_through_parse() {
        assert_eq!(Pass::parse(Pass::RangeLoop.name()), Some(Pass::RangeLoop));
        assert_eq!(Pass::parse("not_a_pass"), None);
    }
}
