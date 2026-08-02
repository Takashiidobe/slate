# Matcher-driven query predicates

Rule selection used to be one hardcoded constructor per shape:
`CallRule::generated`/`CallRule::known`, `DefinitionRule::function`/
`extern_function`/`header`/`known_extern_functions`/`support_module`. Every
new selection shape needed a new method, and case-level predicates like
`never_returning_extern()`/`dead_local()`/`sole_use()` each bundled several
independent facts into one opaque call nobody else could reuse a piece of.

## Matcher structs, not new mechanism

`Field<T, Cx>` (`field.rs`: `Any`/`Eq`/`Predicate`) and hand-written
matchers built from it (`NullaryMethodCall`, `LetStmtPattern`,
`LoopStmtPattern` in `patterns.rs`) already existed, scoped to pure AST
shape only. Extended the same pattern to facts-backed candidate selection:

- **`FnCall<Cx>`** (`target`, `arity`, `arg_types`) replaces
  `CallRule::generated`/`known`. `target: Field<CallTarget, Cx>` reuses the
  existing enum, so one field subsumes both old constructors.
- **`Definition<Cx>`** (`kind`, `name`, `group`) replaces all five
  `DefinitionRule` constructors the same way.
- Both rules now take one `::matches(pass, rule, matcher)` constructor;
  `candidates()` changed from an exact index lookup to a full scan filtered
  by `Field::matches` per field. Unset fields default to `Field::Any`.

## `Value`/`Local`: one resolver for "what do we know about this data"

A local variable, a call argument, any expression position — all reduce to
the same question. One shared resolver (`QueryContext::local_value`,
`views::ResolvedValue { ty, usage, purity }`) backs a `Value<Cx>` matcher:

```rust
pub(in crate::fixups) struct Usage { reads: usize, writes: usize }
pub(in crate::fixups) struct Value<Cx = ()> {
    ty: Field<Option<Type>, Cx>,
    usage: Field<Option<Usage>, Cx>,
    purity: Field<Option<Purity>, Cx>,
}
```

`Local<Cx> { name, mutable, value: Value<Cx> }` composes with
`StatementSequence::starting_with(matcher)`. Candidate selection requires the
first statement to be a `Stmt::Let` matching it before any case runs.
`dead_locals`'s `usage: eq(Usage{0,0})` and
`final_return_temps`'s `usage: eq(Usage{reads:1,writes:0})` moved out of
case-level predicate calls into the matcher this way.

Not everything folds into the matcher — keep it there only when it's a
*structural or fact property of the candidate itself*, not a relationship
between captures or an either/or with a pure-AST fallback:
`final_return_temps` still resolves the sole read's exact path
through `case.def_use` and compares it against `matched.statement(1).path`,
since "the read is at *this* location" is a relationship between matched
items, not a binding property. `dead_locals` still
branches on `discardable_known_method(init)` OR `case.no_effects()` in the
case body, since collapsing a structural-check/fact-fallback OR into one
`Field` would just re-bundle what got taken apart.

`FnCall` doesn't use `Value` for arguments yet (`args: Vec<Value<Cx>>` is
the natural extension — same resolver, per-arg-position matcher) — no rule
currently needs argument-level usage/purity matching, so it's not built
until one does.

## Shared item engine

`QueryRule<M: Matcher>` now iterates `QueryItem`s, gives the matcher's typed
capture to each ordered case, and accepts a shared anchored `EditSet` result.
`ItemPlanBuilder` handles conflicts and application independently of the
matcher. Statement adjacency is one matcher rather than a separate rule and
planning API. Other item kinds can join the same enumeration and matcher trait
without adding another rule engine.
