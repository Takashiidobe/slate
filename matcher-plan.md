# Orthogonal, matcher-driven query predicates

`never_returning_extern()` (used by `libc_exit`) is one opaque predicate that
bundles three separate facts: a matching extern declaration exists, its
arity matches the call, and its return type is `!`. Nobody else can reuse
"does this known target have a matching extern declaration" without also
getting the arity/never-return checks baked in. Same shape problem, smaller
stakes: `dead_local(name)` bundles "no reads" and "no writes" — useful
independently (e.g. "no writes" alone is what a mut-removal predicate would
want) but only queryable as a pair today.

## Two different fixes for two different things

**Binding-level fact predicates** (`no_reads`, `no_writes`, `sole_read`,
`no_effects`) — these prove a property of one already-identified binding.
No "which one matches" search involved, so they stay flat named
`QueryContext` methods, just split one-condition-per-method instead of
bundled. `dead_local(name)` becomes `case.no_reads(name)?;
case.no_writes(name)?;` composed in the rule case.

**Language-item queries** (calls, extern declarations, and future kinds) —
these search for a candidate matching a shape, where the shape has several
independently-optional criteria. These get a matcher struct instead of a
bespoke method per shape combination.

## Matcher structs, not a new mechanism

`Field<T, Cx>` (`field.rs`: `Any`/`Eq`/`Predicate`) and hand-written
matchers built from it (`NullaryMethodCall`, `LetStmtPattern`,
`LoopStmtPattern` in `patterns.rs`) already exist — they're just scoped to
pure AST shape today, no facts, no evidence, no caching. Extend the same
pattern to facts-backed queries:

```rust
pub(in crate::fixups) struct ExternFnMatch<Cx = ()> {
    name: Field<String, Cx>,
    arity: Field<usize, Cx>,
    returns: Field<Option<Type>, Cx>,
}
```

A `QueryContext` method takes the matcher, scans extern declarations,
checks each field via `Field::matches`, and returns the usual
`QueryResult<ExternFnView>` (cached, with `Evidence`/`Rejection` like every
other predicate) instead of a boolean. The rule composes:

```rust
case.extern_fn(&ExternFnMatch {
    name: Field::eq("exit".into()),
    arity: Field::eq(1),
    returns: Field::eq(Some(Type::Never)),
})?
```

Unset fields default to `Field::Any` (same as today's structural matchers),
so one method serves every combination of criteria a future rule needs
instead of a new method per combination.

## Scope boundary

Hand-written matcher structs per item kind, not a generic/derived
mechanism — matches this file's own earlier "scope this down" call on a
`Matchable` derive: revisit only once hand-written matcher structs are
repetitive across 3+ item kinds. Right now there's exactly one candidate
(`ExternFnMatch`, replacing `never_returning_extern`); building a reflection
layer for one caller would be premature.
