# Writing a Query-Driven Fixup

Use `src/fixups/query/` when a rewrite can replace a selected expression,
delete a definition, replace a function body, splice a run of adjacent
statements, or replace the whole program. The query engine owns AST
traversal, fact lookup, stable node locations, conflict detection, mutation,
and tracing. The rule should contain only candidate selection, ordered
preconditions, and anchored typed edits.

## Expression and call rules

Expressions are `QueryItem`s with stable AST and fact paths. `FnCall` selects
calls from that common domain and captures a `CallRecord`. The first accepted
case wins; if every case rejects, the original expression remains unchanged.

```rust
pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::MemchrPreludeFixupCalls,
        "rewrite_memchr_call",
        FnCall {
            target: Field::eq(CallTarget::Generated("__slate_memchr".into())),
            arity: Field::eq(3),
            ..Default::default()
        },
    )
    .case("known_nul", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        let value = case.fact(|query| query.const_u8(&needle))?;
        case.require_at(value == 0, Predicate::ConstantU8, &needle)?;
        case.fact(|query| query.pure(&needle))?;
        let nul = case.fact(|query| query.first_nul(&source))?;
        case.fact(|query| query.prefix_contains(&count, nul))?;
        let replacement = case.lower_expr(
            pointer_at_or_null(source, known_index(nul)),
            &call.site,
        )?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
    .case("byte_position", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        case.fact(|query| query.full_byte_view(&source, &count))?;
        let needle = case.fact(|query| query.pure(&needle))?;
        let replacement = case.lower_expr(
            pointer_at_or_null(source, byte_position(needle)),
            &call.site,
        )?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
}
```

`ItemCaseContext::fact` accepts any `QueryContext` query and accumulates its
proof evidence. Common orthogonal facts include:

- `call_args(call)` binds fixed-arity argument sites.
- `byte_source(arg)` proves and captures a byte-oriented source.
- `u8_eq(arg, value)` proves a constant byte value.
- `pure(arg)` proves an expression can be moved and returns `StableExpr`.
- `full_byte_view(source, count)` proves the count covers the full byte view.
- `first_nul(source)` proves NUL termination and captures its position.
- `prefix_contains(count, nul)` proves the searched prefix reaches that NUL.
- `extern_fn(pattern)` proves a matching extern declaration.
- `lower_expr(recipe, site)` lowers a typed recipe using facts at the selected
  expression.

Use `?` for every precondition. The context automatically records accepted proof
evidence and earlier case rejections for `fixup-debug`.

## Recipes

A case returns an `EditSet`, never rendered Rust. Keep detailed AST construction
in `src/fixups/query/recipe.rs` so the rule stays short, lower the recipe against
the matched site, and anchor the result explicitly:

```rust
let replacement = case.lower_expr(
    pointer_at_or_null(source, byte_position(needle)),
    &call.site,
)?;
Ok(EditSet::replace_expression(call.site.clone(), replacement))
```

A recipe should accept proof-backed values such as `ByteSource`, `StableExpr`,
or `NulPosition`, then build only `rust_ast` nodes. Moving or cloning an
expression requires a purity or equivalent stability proof.

## Adding a helper

When a rule needs a new precondition or capture:

1. Add the semantic fact under `src/fixups/facts/` if it does not exist.
2. Add a `QueryContext` method returning `QueryResult<T>`.
3. Add its `Predicate` and stable `EvidenceDetail`.
4. Invoke the query through `case.fact(|query| query.method(handle))` so proof
   accumulation remains generic.
5. Add trace formatting in `query/rewrite.rs`.
6. Use the helper as one short precondition in the rule.

Missing or ambiguous evidence must reject the case. Claims such as “all users”
or “all callers” need an explicitly complete domain; an empty observed set is
not enough by itself.

For a rewrite that needs to know whether a definition safely reaches a later
statement in the same block, compose the shared navigation and dependency
queries. `statement_in_container` maps an exact expression use to the direct
statement in a selected container, `statements_between` returns stable handles
for the intervening range, and `binding_uses_in_statement` restricts exact
def-use observations to one statement. `expression_dependencies` and
`expression_effects` separate value dependencies from movement safety, while
`statement_is_movable_declaration` handles declaration initializers. Keep only
the pass's crossing policy in the rule instead of adding a pass-local walker or
candidate planner.

For fixed-array cursor recovery, `buffer_pointer_fields` returns each proven
buffer-field origin with stable buffer and array bindings, the exact assignment
statement, field name, constant index, and array bound. Compose it with
`function_bindings`, `binding_uses`, `statement`, and `statement_expression` so
alias and unresolved-use rejection stays complete while the rule owns the
indexing and pointer-difference policy.

For owned heap lifetimes, `heap_ownership_facts` exposes owner, allocation,
size, alias, free, and reallocation bindings together with exact lifecycle and
use statements. Allocation kind, extent, initialization, read safety, and
resize direction remain independent observations. The rule decides Box/Vec
eligibility and constructs the private recipe plan; the query never returns a
rewrite plan or rewritten body.

For signature rewrites, `parameter_function` relates a stable parameter handle
to its function and `parameter_uses` exposes its binding uses.
`function_reachability` reports external reachability and address exposure,
while `direct_calls` returns stable call handles without claiming completeness.
Use `function_call_domain` when every caller must be known; it rejects
incomplete, address-exposed, and externally reachable domains with the evidence
collected so far. `call_argument` resolves a stable argument expression by
position. The rule owns signature eligibility, argument policy, and the atomic
function-and-call edit set.

## Definition items

Definitions are `QueryItem`s selected by the reusable `Definition` matcher. The
memchr lifecycle deletes an unused helper and otherwise installs its idiomatic
fallback body:

```rust
pub(in crate::fixups) fn helper() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::MemchrPrelude,
        "manage_memchr_helper",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            name: Field::eq("__slate_memchr".into()),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        let uses = case.fact(|query| query.definition_users(definition))?;
        case.require_at(uses.users == 0, Predicate::ZeroUsers, &uses.site)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
    .case("retained", |_, definition| {
        Ok(EditSet::replace_function_body(
            definition.clone(),
            memchr_fallback_body(),
        ))
    })
}
```

The matcher composes `kind`, `name`, and `group` fields. Edit actions are
`EditSet::delete_definition(...)` and
`EditSet::replace_function_body(...)`. Preconditions are:

- `zero_users()` for every symbol exported by one definition;
- `zero_group_users()` for every definition in a header or support group.

These proofs remain conservative when an opaque `Item::Raw` exists, a local
definition is externally reachable, or the use domain is otherwise incomplete.
Generated support must use `Item::SupportModule` and list every exported
qualified path so its lifetime is tied to real AST users.

## Item rules

`QueryRule<M>` runs one matcher over every query item in the program. A
`StatementSequence` matcher selects adjacent statements inside function bodies
and nested blocks, loops, arms, and branches. The rule case can query facts from
any captured handle and returns an `EditSet`; selection does not constrain what
the case may inspect or edit.

```rust
pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence> {
    QueryRule::new(
        Pass::RangeLoop,
        "rewrite_counted_loop_to_range",
        StatementSequence::new(2),
    )
    .case("zero_step_one", |case, matched| {
        let [index_stmt, loop_stmt] = case.statements(matched)?;
        let fact = case.fact(|query| query.counted_loop(&matched.statement(1)))?;
        case.require(fact.start == CountedLoopStart::Zero)?;
        case.require(fact.step == CountedLoopStep::One)?;
        Ok(EditSet::replace_statements(
            matched.target().clone(),
            replacement(index_stmt, loop_stmt, fact),
        ))
    })
}
```

Matchers live in `patterns.rs` and implement the common `Matcher` interface.
`StatementSequence::starting_with(Local { ... })` composes local binding fields
with adjacency, while structural captures such as `LetStmtPattern` and
`LoopStmtPattern` can refine the selected statements in a case. `case.reject()`
and `case.require(condition)` reject unsupported shapes while retaining prior
proof evidence.

Variable-width regions start from an ordinary statement match.
`following_statements` returns stable handles for the rest of the same
container, and `statement_range` validates an inclusive start/end pair before
constructing the replacement anchor. Compose those navigation queries with
`binding_uses_in_expression` and `expression_effects` for dataflow-gated region
rewrites. The rule decides where the structural run ends; path arithmetic and
same-container validation stay in the query layer.

Overlap detection generalizes path-prefix overlap (as for expressions) with a
statement-range check. Two edit sets conflict when any of their anchors overlap,
including when one statement edit removes the container of another edit.

## Scheduling

Build a plan from one immutable `Program` and fresh `FixupFacts` snapshot, then
apply it after the query context is dropped:

```rust
let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
let plan = {
    let query = query::QueryContext::new(&program, &facts);
    let mut builder = query::ItemPlanBuilder::new();
    builder.add_rule(&query, &query::rules::memchr::calls());
    builder.finish()
};
plan.apply(&mut program, &facts, logger).changed
```

Use `ItemPlanBuilder` and `plan.apply(&mut program, &facts, logger)` for every
matcher-driven rule, including expression, binding, definition, function, and
statement matches. Add several rules to one builder only when they intentionally
share a snapshot; overlapping edit sets are rejected across item kinds.

One accepted `EditSet` is transactional. Every anchor is validated against the
unchanged input before mutation, and a missing or stale anchor rejects that
complete set while independent valid sets may still apply. Duplicate or
overlapping anchors inside a set are rejected during planning. Statement ranges
use snapshot coordinates and are applied from higher indexes to lower indexes;
item removals follow the same rule, so earlier removals cannot shift a later
target. Reports count applied edit sets and deduplicate touched items.

The fundamental edits replace a typed expression, statement range, complete
function, or complete definition. Signature, parameter, and body changes use a
lightweight `FunctionRef` to look up the snapshot `FnDef`, clone it only while
constructing the accepted edit, and replace that function.
Argument-list changes replace the anchored call expression. Binding declarations,
initializers, nested blocks, and match arms use statement-range replacement;
record fields and enum variants replace their enclosing definition. Definition
deletion is definition replacement with no successor. Convenience constructors
compose these operations and do not add pass-specific planner actions.
Top-level insertion uses `EditSet::insert_items` with the immutable snapshot's
item count, applies in reverse source order after removals, and marks touched
items unbounded because insertion renumbers every later item.

Recompute facts before the next fact-dependent query. Place definition deletion
after the final pass that can remove a user.

## Lowering-like rules

Control-flow recovery and other lowering-like rewrites use the same query and
edit protocol. Expose a semantic region view with stable statement anchors,
then pass that view to a typed recipe that constructs replacement AST. Keep CFG
analysis and structural emission in the recipe; the query rule only selects the
region, requests the lowering, and returns one transactional `EditSet` for all
affected anchors.

Use multiple statement edits when a region is not contiguous. For example, a
dispatch rewrite can replace its loop and delete an earlier state declaration
in one edit set. Do not widen the replacement range across unrelated statements
or move the lowering algorithm into matcher predicates.

## Program rules

Use `QueryRule<WholeProgram>` only for a transformation that must update
definitions, types, and expressions together. It produces the same `EditSet` as
every other query rule, so program replacement participates in the shared
conflict checks, snapshot validation, tracing, and atomic application:

```rust
QueryRule::new(
    Pass::AnonymousStructs,
    "rewrite_anonymous_structs",
    WholeProgram::when(|query| query.has_anonymous_structs()),
)
    .case("complete_domain", |case, program| {
        let structs = case.fact(|query| query.anonymous_structs())?;
        let rewrite = case.fact(|query| rewrite_anonymous_structs(query, structs))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.touched,
        ))
    })
```

Keep traversal and coordinated AST construction in a typed helper. The rule
should only select the program domain, prove it complete, construct the
replacement, and return the shared anchored edit.

## Files and tests

- Put concise rules in `src/fixups/query/rules/`.
- Put fact adapters and proofs in `context.rs`, `proof.rs`, and `views.rs`.
- Put typed AST construction in `recipe.rs`.
- Register the plan and fresh-fact boundary in `src/fixups/mod.rs`.

Test query rewrites through C fixtures, not unit tests under
`src/fixups/query/`. Cover accepted cases and important rejected fallbacks by
asserting the translated source in `tests/differential.rs`; diagnostic metadata
is not part of rewrite correctness. Then run:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release \
  --test differential -E 'test(generated_differential)' --nocapture
cargo fmt
cargo clippy --all-targets
cargo nextest r --release
```
