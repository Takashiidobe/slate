# Writing a Query-Driven Fixup

Use `src/fixups/query/` when a rewrite can replace a selected expression,
delete a definition, replace a function body, splice a run of adjacent
statements, or replace the whole program. The query engine owns AST
traversal, fact lookup, stable node locations, conflict detection, mutation,
and tracing. The rule should contain only candidate selection, ordered
preconditions, and a typed recipe.

Read [writing-a-fixup.md](writing-a-fixup.md) first. Use a legacy pass under
`src/fixups/rewrite/` only when the query planner still cannot represent the
change - for example a rewrite whose window width isn't fixed in advance, or
one that needs to inspect mutated state left behind by an earlier statement
in the same pass.

## Call rules

`CallRule` selects a call shape and tries its cases in order. The first accepted
case wins; if every case rejects, the original call remains unchanged.

```rust
pub(in crate::fixups) fn calls() -> CallRule {
    CallRule::generated(
        Pass::MemchrPreludeFixupCalls,
        "rewrite_memchr_call",
        "__slate_memchr",
        3,
    )
    .case("known_nul", |case| {
        let [source, needle, count] = case.args();
        let source = case.byte_source(source)?;
        case.u8_eq(needle, 0)?;
        case.pure(needle)?;
        let nul = case.first_nul(&source)?;
        case.prefix_contains(count, nul)?;
        Ok(pointer_at_or_null(source, known_index(nul)))
    })
    .case("byte_position", |case| {
        let [source, needle, count] = case.args();
        let source = case.byte_source(source)?;
        case.full_byte_view(&source, count)?;
        let needle = case.pure(needle)?;
        Ok(pointer_at_or_null(source, byte_position(needle)))
    })
}
```

The current `CallCaseContext` helpers are:

- `args()` binds fixed-arity arguments.
- `byte_source(arg)` proves and captures a byte-oriented source.
- `u8_eq(arg, value)` proves a constant byte value.
- `pure(arg)` proves an expression can be moved and returns `StableExpr`.
- `full_byte_view(source, count)` proves the count covers the full byte view.
- `first_nul(source)` proves NUL termination and captures its position.
- `prefix_contains(count, nul)` proves the searched prefix reaches that NUL.
- `never_returning_extern()` proves the selected known call has a matching
  extern declaration with the same arity and a `!` return type.

Use `?` for every precondition. The context automatically records accepted proof
evidence and earlier case rejections for `fixup-debug`.

## Recipes

A case returns a typed recipe, never rendered Rust. Keep detailed AST
construction in `src/fixups/query/recipe.rs` so the rule stays short:

```rust
Ok(pointer_at_or_null(source, byte_position(needle)))
```

A recipe should accept proof-backed values such as `ByteSource`, `StableExpr`,
or `NulPosition`, then build only `rust_ast` nodes. Moving or cloning an
expression requires a purity or equivalent stability proof.

## Adding a helper

When a rule needs a new precondition or capture:

1. Add the semantic fact under `src/fixups/facts/` if it does not exist.
2. Add a `QueryContext` method returning `QueryResult<T>`.
3. Add its `Predicate` and stable `EvidenceDetail`.
4. Add a small method to `CallCaseContext` or `DefinitionCaseContext` that
   accumulates the proof.
5. Add trace formatting in `query/rewrite.rs`.
6. Use the helper as one short precondition in the rule.

Missing or ambiguous evidence must reject the case. Claims such as “all users”
or “all callers” need an explicitly complete domain; an empty observed set is
not enough by itself.

For a rewrite that needs to know whether a definition safely reaches some
later statement in the same block - "is everything between these two points
safe to cross" - use `FixupFacts::binding_touches_in_body(binding, body_path)`
rather than writing a new per-pass fact collector to re-derive read/write
positions. It returns every direct-child statement index of a body that reads
or writes a binding, ordered, so a rule can find the next write after a point
and fold over the statements in between with its own crossability rule (e.g.
`zero_init`'s `direct`/`relaxed`/`moved` cases in `src/fixups/query/context.rs`)
instead of re-walking the AST for it. This is shared, engine-level
infrastructure - extend it in place rather than adding a parallel one-off.

## Definition rules

Definition rules also use ordered cases. The memchr lifecycle deletes an unused
helper and otherwise installs its idiomatic fallback body:

```rust
pub(in crate::fixups) fn helper() -> DefinitionRule {
    DefinitionRule::function(
        Pass::MemchrPrelude,
        "manage_memchr_helper",
        "__slate_memchr",
    )
    .case("unused", |case| {
        case.zero_users()?;
        Ok(delete_definition())
    })
    .case("retained", |_| Ok(replace_body(memchr_fallback_body())))
}
```

Selectors include `function`, `extern_function`, `known_extern_functions`,
`header`, and `support_module`. Actions are `delete_definition()` and
`replace_body(...)`. Preconditions are:

- `zero_users()` for every symbol exported by one definition;
- `zero_group_users()` for every definition in a header or support group.

These proofs remain conservative when an opaque `Item::Raw` exists, a local
definition is externally reachable, or the use domain is otherwise incomplete.
Generated support must use `Item::SupportModule` and list every exported
qualified path so its lifetime is tied to real AST users.

## Statement window rules

`StmtWindowRule` matches a fixed-width run of adjacent statements anywhere in
the program - inside a function body or any nested block, loop, arm, or
branch - and its recipe splices in a replacement `Vec<IndentStmt>` (empty to
delete, one or more to replace). This is the rule kind for rewrites that
depend on more than one statement at a time, such as folding a canonical
`let i = 0; loop { ... }` pair into a Rust `for` loop:

```rust
pub(in crate::fixups) fn rewrite() -> StmtWindowRule {
    StmtWindowRule::new(Pass::RangeLoop, "rewrite_counted_loop_to_range", 2).case(
        "zero_step_one",
        |case| {
            let [index_stmt, loop_stmt] = case.stmts();
            let Stmt::Let { name: index_name, .. } = &index_stmt.stmt else {
                return Err(case.reject());
            };
            let Stmt::Loop { body: loop_body, .. } = &loop_stmt.stmt else {
                return Err(case.reject());
            };
            case.require(loop_body.len() >= 2)?;
            let fact = case.counted_loop()?;
            case.require(fact.start == CountedLoopStart::Zero)?;
            case.require(fact.step == CountedLoopStep::One)?;
            // ...build and return the replacement Vec<IndentStmt>...
        },
    )
}
```

Structural shape (is this a `Let` followed by a `Loop`?) is matched directly
in the rule with ordinary `let else` - a window can hold any `Stmt`, unlike a
call's fixed argument list, so there is no one structural helper to reuse the
way `CallCaseContext::args()` covers every call. `case.reject()` and
`case.require(condition)` turn a failed structural or boolean check into a
`Rejection`, carrying forward whatever proof evidence the case already
accumulated. Semantic guards work exactly like other rule kinds:
`case.counted_loop()` proves and captures the fact backing this window.

Overlap detection generalizes path-prefix overlap (as for expressions) with a
statement-range check: two windows conflict when they select overlapping
indices in the same container, or when one window's statement range would
delete the container the other window lives inside. As with call and
expression rules, conflicting edits are dropped rather than applied in some
implicit order - a rewrite that depends on an enclosing statement's fixup
already having happened in the same pass (e.g. a nested counted loop inside
another counted loop) may need another pass round rather than firing in one
shot; this mirrors how overlapping call rewrites already behave today.

## Scheduling

Build a plan from one immutable `Program` and fresh `FixupFacts` snapshot, then
apply it after the query context is dropped:

```rust
let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
let plan = {
    let query = query::QueryContext::new(&program, &facts);
    let mut builder = query::ExprPlanBuilder::new();
    builder.add_rule(&query, &query::rules::memchr::calls());
    builder.finish()
};
plan.apply(&mut program, &facts, logger).changed
```

Use `DefinitionPlanBuilder` and `plan.apply(&mut program, logger)` for definition
rules, or `StmtWindowPlanBuilder` and `plan.apply(&mut program, &facts, logger)`
for statement-window rules. Add several rules to one builder only when they
intentionally share a snapshot; overlapping edits are rejected.

Recompute facts before the next fact-dependent query. Place definition deletion
after the final pass that can remove a user.

## Program rules

Use `ProgramRule` for one proof-gated transformation that must update definitions,
types, and expressions together. `ProgramPlanBuilder` prepares the entire result
from one immutable AST and facts snapshot, validates its definition anchors, and
applies it atomically:

```rust
ProgramRule::anonymous_structs(Pass::AnonymousStructs, "rewrite_anonymous_structs")
    .case("complete_domain", |case| {
        let structs = case.anonymous_structs()?;
        Ok(rewrite_anonymous_structs(structs))
    })
```

Keep traversal and coordinated AST construction in a typed program recipe. The
rule should only select the program domain, prove it complete, and choose that
recipe.

## Files and tests

- Put concise rules in `src/fixups/query/rules/`.
- Put fact adapters and proofs in `context.rs`, `proof.rs`, and `views.rs`.
- Put typed AST construction in `recipe.rs`.
- Register the plan and fresh-fact boundary in `src/fixups/mod.rs`.

Test query rewrites through C fixtures, not unit tests under
`src/fixups/query/`. Cover accepted cases, important rejected fallbacks, and
debug evidence in `tests/differential.rs`, then run:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release \
  --test differential -E 'test(generated_differential)' --nocapture
cargo fmt
cargo clippy --all-targets
cargo nextest r --release
```
