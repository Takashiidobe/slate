# Writing a Query-Driven Fixup

Use `src/fixups/query/` when a rewrite can replace a selected expression, delete
a definition, or replace a function body. The query engine owns AST traversal,
fact lookup, stable node locations, conflict detection, mutation, and tracing.
The rule should contain only candidate selection, ordered preconditions, and a
typed recipe.

Read [writing-a-fixup.md](writing-a-fixup.md) first. Use a legacy pass under
`src/fixups/rewrite/` only when the query planner cannot represent the change,
such as a multi-statement or control-flow rewrite.

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
rules. Add several rules to one builder only when they intentionally share a
snapshot; overlapping edits are rejected.

Recompute facts before the next fact-dependent query. Place definition deletion
after the final pass that can remove a user.

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
