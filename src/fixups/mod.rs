//! Rust cleanup passes that run after faithful CIR lowering.

pub(crate) mod facts;
mod idents;
mod rewrite;
mod runtime;
mod support;

#[cfg(test)]
mod test_support;

use crate::rust_ast::{Block, IndentStmt, Item, Program, Stmt};

pub fn apply(program: Program) -> Program {
    let facts::AnalyzedProgram { program, .. } = facts::analyze(program);
    let mut program = program;
    structure_goto(&mut program);
    inline_temps_to_fixpoint(&mut program, InlinePass::Early);
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    rewrite::anonymous_structs::fixup(&mut program, &facts);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::param_spills::fixup(f, function, &facts);
        }
    }
    zero_init_to_fixpoint(&mut program, false);
    struct_field_init_to_fixpoint(&mut program);
    singleton_scopes_to_fixpoint(&mut program);
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::compound_assign::fixup(&mut f.body, function, &facts);
        }
    }
    cleanup_for_continues(&mut program);
    singleton_scopes_to_fixpoint(&mut program);
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            rewrite::constant_index_casts::fixup(&mut f.body);
        }
    }
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::unnecessary_casts::fixup(&mut f.body, function, &facts);
        }
    }
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
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::retval::fixup(f, function, &facts);
        }
    }
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
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::lazy_singleton::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::drop_call_results::fixup(&mut f.body, function, &facts);
        }
    }
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::string_lift::fixup(&mut f.body, function, &facts);
        }
    }
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        if !rewrite::string_params::fixup(&mut program, &facts) {
            break;
        }
    }
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::ptr_len::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::slice_index::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    if rewrite::slice_loop::fixup(&mut program, &facts) {
        late_loop_cleanup(&mut program);
    }
    if rewrite::slice_reduce::fixup(&mut program) {
        late_loop_cleanup(&mut program);
    }
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    if rewrite::range_loop::fixup(&mut program, &facts) {
        late_loop_cleanup(&mut program);
    }
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::va_list::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::remove_mut::fixup(f, function, &facts);
        }
    }
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::string_copy::fixup(&mut program, &facts);
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        if !rewrite::string_params::fixup(&mut program, &facts) {
            break;
        }
    }
    remove_mut(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::string_libc::fixup(&mut program, &facts);
    let facts::AnalyzedProgram {
        mut program,
        facts: _,
    } = facts::analyze(program);
    rewrite::sort_search::fixup(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::heap_ownership::fixup(&mut program, &facts);
    dead_locals_to_fixpoint(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::remove_mut::fixup(f, function, &facts);
        }
    }
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::printf_format::fixup(&mut program, &facts);
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        if !rewrite::string_params::fixup(&mut program, &facts) {
            break;
        }
    }
    remove_mut(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::string_libc::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::c_strings::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::stdio::fixup(&mut program, &facts);
    rewrite::memchr_prelude::fixup_calls(&mut program, &facts);
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        if !rewrite::nullable_pointer::fixup(&mut program, &facts) {
            break;
        }
    }
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::string_lift::fixup_c_strings(&mut f.body, function, &facts);
        }
    }
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            rewrite::memchr_prelude::fixup(f);
        }
    }
    rewrite::memchr_prelude::prune_unused_helper(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::array_element_pointer_origin::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::buffer_cursor::fixup(&mut program, &facts);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::atomic_locals::fixup(&mut program, &facts);
    inline_temps_to_fixpoint(&mut program, InlinePass::Late);
    zero_init_to_fixpoint(&mut program, true);
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
    remove_mut(&mut program);
    inline_var_aliases_to_fixpoint(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    rewrite::prune_unused_externs::fixup(&mut program, &facts);
    rewrite::unused_items::fixup(&mut program);
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            rewrite::main_zero_exit::fixup(f);
        }
    }
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
    use crate::rust_ast::{BinOp, Expr, Prim, RustValue, Stmt, Type};

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
}
