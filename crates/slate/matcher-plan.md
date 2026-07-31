# Query-owned incremental facts

`src/fixups/query/` and per-function fact splicing
(`FixupFacts::splice_function`/`remove_items`, `slate-04q.75.56.8.1`–`.8.4`)
are done. What's left: nobody should ever have to decide whether facts need
recomputing.

## Goal

Rule authors already don't decide this — a `.case()` closure just calls
`context.foo(...)` and gets a `QueryResult`. The pipeline author wiring
`fixups/mod.rs` still does: `.8.4` required hand-auditing each step
transition ("does `facts::analyze` immediately follow, does anything mutate
outside the reported `touched` set") and writing bespoke splice code per
case. Doesn't scale — every future migration would repeat that audit.

## Mechanism: dirty-tracked, resolve-on-read

Introduce `IncrementalFacts` — a `FixupFacts` plus a `Dirty` marker
(`Clean`, `Touched(TouchedItems)`, `Everything`) — threaded once through
`apply_with_logger`, replacing today's ~20 manual
`facts::analyze(&program)` rebindings.

- Every migrated step's `plan.apply()` calls
  `incremental.mark_touched(&report.touched)`: cheap, unions item indices
  into the dirty set, never recomputes.
- Every legacy step, and `late_loop_cleanup` (mutates arbitrary functions
  without reporting what), calls `incremental.mark_everything_dirty()` when
  it changed something — same treatment for both, no `RangeLoop`-style
  special-casing.
- Whenever a step needs facts, it calls `incremental.resolve(&program)` —
  the only place recompute happens: `Clean` no-ops, `Touched(set)` splices
  each function via `splice_function`/`remove_items`, `Everything` falls
  back to a full `facts::analyze`. Resolving clears the marker.

This subsumes and deletes `.8.4`'s hand-picked `LazySingleton`/`RangeLoop`
wiring. The win now applies to every future migration automatically:
whenever two migrated steps end up adjacent, `resolve()` skips the
reanalyze for free, no per-pair auditing added. Legacy passes still force a
full reanalyze at the next `resolve()`, same as today — just uniform now.

## Why not finer-grained

`QueryContext` is already rebuilt fresh once per step, so resolving once
per construction is already as fine-grained as visibly matters. Per-field
self-healing inside `FixupFacts` (~25 accessors, each interior-mutable)
would buy nothing more under that lifetime.

## Verification

Reuse `.8.3`'s precedent — `FixupFacts::diff_incremental` and
`slate verify-incremental-facts` already exist; generalize the check to run
after every `resolve()`, not just the two sites `.8.4` covered.

## Scope boundary

Legacy `Fixup`-trait passes don't report touched functions, so one between
two migrated steps still forces `Everything`. Extending self-healing across
legacy boundaries is out of scope — only makes sense as each legacy pass is
itself migrated.
