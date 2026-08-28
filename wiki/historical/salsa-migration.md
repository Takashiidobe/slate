# Salsa Migration

Tracked by the `slate-kby1` epic, **complete**. This doc is a historical
record of the migration's motivation and plan, kept for context on why the
architecture looks the way it does; for the current design see
[facts.md](../concepts/facts.md) (facts layer) and [fixups.md](../concepts/fixups.md) (rule-authoring
contract). Scope, per the epic: only `src/backend/facts/` (`FixupFacts` and its
collectors) and `query::QueryContext`'s internals moved to salsa.
`EditSets`/`Plan<E: EditTarget>`/matchers/recipes — the whole rewriting layer
described in [fixups.md](../concepts/fixups.md) — stayed as they were. Rule-authoring
ergonomics (`case.fact(|query| query.method(handle))`) didn't change either;
only what runs behind `QueryContext`'s methods did.

The plan below (target architecture, phases) is what was proposed going in;
see [Where it landed](#where-it-landed) for what was actually built, which
diverged from this plan in one significant way — the final design dropped the
`TouchedItems`-driven diffing this doc describes in favor of treating the
whole `Program` as one salsa input and leaning entirely on backdating.

## Why (motivation, before the migration)

`facts::analyze(&Program)` (`src/backend/facts/mod.rs`, since deleted) walked
the whole program through 26 collectors, in order, producing `FixupFacts` — a
flat bag of ~45 `Vec<...Fact>` fields, each scanned/filtered linearly by
`FunctionId`/`BindingId` per query. It was called at roughly 20 sites in
`backend::apply_with_logger`, plus internally by every `to_fixpoint_*` loop.

`slate-04q.75.56.9` had already pushed back against full reanalysis with
`IncrementalFacts`/`Dirty` (`backend/mod.rs`, since deleted): `Clean` /
`Touched(TouchedItems)` / `Everything`, where `TouchedItems` (`query/plan.rs`,
since deleted) came from what a `plan.apply()` call actually edited.
`resolve()` either no-op'd, spliced touched functions via
`FixupFacts::splice_function`/`remove_items`, or fell back to full
`facts::analyze`.

The splice path's ceiling was narrow: `splice_function` only re-derived 7 of
26 collector families for a touched function (bindings/loops, `borrow_alias`,
`def_use`, `effects`, `values`, `strings` buffers/views/uses, `counted_loop`),
then unconditionally reran `casts` and `lazy_singleton` whole-program on top
(`splice_incremental_facts`, `backend/mod.rs`). Every other family — the
interprocedural ones (`heap_ownership`, `string_params`, `ptr_len`,
`calls`/`callsites`, `printf`, `file_ownership`, `anonymous_structs`, ...) and
several local ones not yet ported (`places`, `control_flow`, `atomic_locals`,
`buffer_cursor`, `array_element_pointer_origin`, `loop_shapes`,
`null_check_dominance`, ...) — was invisible to `Touched`. Any pass that
touched those, and every legacy (non-query-engine) `Fixup`-trait pass
regardless of what it touched, called `mark_everything_dirty()` and forced a
full reanalyze at the next `resolve()`.

Extending this by hand would have meant auditing each of the remaining ~19
families for whether splicing was sound for it — exactly the scaling problem
`slate-04q.75.56.9`'s own epic description names as the reason it stopped at
two hand-picked collectors. Salsa replaces per-family manual soundness
auditing with automatic dependency tracking: a tracked function's dependencies
are whatever it actually read, not a hand-maintained list.

## What salsa buys

- **No per-family audit to add incrementality.** A tracked fn's dependency
  edges come from what it reads at run time, not from someone reasoning
  through `splice_function`'s 7-collector allowlist for an 8th.
- **Backdating.** If a touched function's derived fact value is unchanged
  (same `Eq` value, even though the function's source AST changed), salsa
  stops propagating invalidation to that query's dependents. Splice-then-
  reanalyze has no equivalent — a touched function's facts are always treated
  as changed downstream.
- **Real map/reduce for interprocedural facts.** `heap_ownership`-shaped
  families become "recompute this function's local contribution; the
  program-wide reduction only re-touches functions whose local contribution's
  _value_ changed" — not "rerun over everything," which was the honest
  description of what `Everything`/full `facts::analyze` did.
- **No `mark_everything_dirty` cliff at legacy-pass boundaries.** Because
  salsa resolves lazily at read time, a full-`Program` re-set only costs a
  cheap per-function recheck plus backdating, instead of forcing every
  collector to rerun over the whole program from scratch.

## Architecture this replaced

- `FixupFacts` (`facts/mod.rs:45`, since deleted): one `Vec<XFact>` field per
  collector output; every fact carried a `Site { function: FunctionId, path:
AstPath }` or bare `FunctionId`/`BindingId`, scanned linearly by callers.
- `FunctionId` (`facts/mod.rs:93`) was already the right stable key: assigned
  once, in traversal order, the first time a function is seen
  (`Collector::push_function`), and never reused or renumbered — only
  `item_index` (the function's position in `Program::items`) shifted when
  items were removed (`FixupFacts::remove_item`). That made `FunctionId`
  durable across `Program` revisions, which is exactly the property a
  `#[salsa::input]` key needs — and remains true today.
- `QueryContext<'snapshot>` (`query/context.rs:134`) was rebuilt fresh from
  `&Program` + `&FixupFacts` per snapshot. The `query_cache!` macro (since
  deleted) added a hand-written `RefCell<HashMap<key, value>>` per memoized
  semantic query (`byte_source`, `pure`, `first_nul`, ...) — but nothing
  survived past one `QueryContext` instance, i.e. one snapshot.
- `IncrementalFacts`/`Dirty` (`backend/mod.rs`, since deleted) was the
  best-effort incremental layer described above.

## What doesn't change

- `EditSets`, `Plan<E: EditTarget>`, `ItemPlanBuilder`, conflict detection,
  matchers (`patterns.rs`, `item.rs`), recipes (`recipe.rs`) — untouched.
- Pass ordering and `fixup-debug` tracing (`docs/passes.md`) — unaffected.
- The rule-authoring contract in `docs/fixups.md` — unaffected; only
  `QueryContext`'s internals change, not its public method surface.

## Target architecture (original plan — see [Where it landed](#where-it-landed) for what shipped)

### Identity and granularity

Represent each function as one `#[salsa::input] struct FunctionInput { #[returns(ref)] body: FnDef }`,
keyed outside salsa by the function's existing `FunctionId`. A plain Rust
registry — `HashMap<FunctionId, FunctionInput>` — bridges `FunctionId` to the
live salsa handle; this lives in a new struct (`SalsaFacts`, replacing
`IncrementalFacts`), not inside salsa itself. Program-relative facts a
function needs beyond its own body (e.g. `def_use` reading bindings) come from
composing tracked fns, not from reaching into a global flat `Vec`.

Salsa has no built-in "enumerate all live inputs of a kind" (confirmed against
the vendored `salsa-0.28.2` source, `src/input.rs`). The standard workaround —
used by every real salsa consumer, rust-analyzer included — is a second
`#[salsa::input(singleton)] struct AllFunctions { #[returns(ref)] ids: Vec<FunctionId> }`
that the bridge layer updates whenever the registry gains or loses an entry.
Program-wide reductions read `all_functions(db)` first, then map/reduce over
each id's `local_*` tracked fn.

### Bridging `Program` <-> salsa inputs (superseded — see [Where it landed](#where-it-landed))

Replaces `IncrementalFacts::resolve`. Given the just-applied `TouchedItems`:

1. **`in_place`**: look up each `item_index`'s `FunctionId` in the registry
   (already known — no scan needed, unlike today's
   `function_by_item_index`) and call `.set_body(&mut db).to(new_body)`.
2. **`removed`**: drop the entry from the registry. Salsa has no explicit
   "delete an input"; an unreferenced handle just stops being read. This
   matches `function_by_item_index`'s existing `None`-on-miss semantics for
   any stale caller.
3. **New functions** (inserted items with no known `FunctionId`): mint the
   next `FunctionId` from the existing monotonic counter and
   `FunctionInput::new(&mut db, ...)`.
4. **`unbounded`** (legacy pass, no reported touched set): diff every item in
   the new `Program` against the registry's known content, driving the same
   three operations above for whatever changed. This is strictly better than
   today's `Everything`/full `facts::analyze`, because functions the legacy
   pass didn't actually touch keep their existing `FunctionInput` untouched —
   their downstream tracked fns are memo hits, not recomputed. No pass needs
   to change to get this; it's purely a `resolve()`-side improvement, and
   worth landing in Phase 1 rather than waiting on further pass migrations.

### Fact families: two buckets

Matches the "bucket-2 shaped" language `slate-kby1.1` already uses.

**Bucket 1 — pure per-function, no cross-function reasoning.** `def_use`,
`effects`, `control_flow`, `casts`, `places`, `values`, `atomic_locals`/
`atomic_globals`, `c_strings`, `va_list` alias, `buffer_cursor`,
`array_element_pointer_origin`, `counted_loop`, `loop_shapes`,
`null_check_dominance`, `pointer_option_safety`/`pointer_comparisons`,
`struct_field_ownership`, `option_box_locals`/comparisons, `borrow_alias`.
Each becomes:

```rust
#[salsa::tracked]
fn def_use_for_function(db: &dyn FixupDb, function: FunctionInput) -> Arc<Vec<DefUseFact>> {
    def_use::collect_for_function(function.function(db), &function.body(db), /* ... */)
}
```

`def_use` and `effects` already have a `collect_for_function` split from
their whole-program `collect_facts` loop (used today by `splice_function`) —
port those two first, per `slate-kby1.1`. The rest need the same split
extracted from their `collect_facts` loop before they can become per-function
tracked fns; `splice_function`'s existing 7-family allowlist is the field
guide for which extractions are already known-safe.

**Bucket 2 — program-wide reduction over local contributions.**
`calls`/`call_signatures`/`callsites` (callsite resolution needs the full
signature table), `heap_ownership` + `callee_alloc_summary` +
`interprocedural_alloc_eligibility`/`callers`, `string_params` (fixpoint over
the call graph), `ptr_len` (fixpoint), `file_ownership`, `lazy_singleton`
(program-wide write-once/read-once check), `anonymous_structs` (cross-function
shape dedup — inherently whole-program), `printf` (re-verify; likely local).
Shape:

```rust
#[salsa::tracked]
fn local_heap_ownership(db: &dyn FixupDb, function: FunctionInput) -> Arc<LocalHeapContribution> { /* ... */ }

#[salsa::tracked]
fn heap_ownership_program(db: &dyn FixupDb) -> Arc<Vec<HeapOwnershipFact>> {
    all_functions(db).iter().map(|&f| local_heap_ownership(db, registry_lookup(f))).collect(/* reduce */)
}
```

Editing one function only recomputes that function's `local_*`; the reduce
step only re-touches functions whose `local_*` _value_ changed — provided the
per-function contribution type implements `Eq` so salsa can backdate it. Most
`FixupFacts` structs already derive `PartialEq, Eq` (spot-checked
`facts/mod.rs`); a few don't (e.g. `BindingTypeFact` at `facts/mod.rs:129`,
which holds a `rust_ast::Type` — add `Eq` where missing and cheap. Note
`Program`/`Item`/`FnDef` themselves (`rust_ast.rs`) don't derive `PartialEq`
today; that's fine as designed here, since no salsa input field is typed as
`Program`/`Item` directly — only cloned `FnDef` bodies are — but it does mean
`FunctionInput::set_body` itself can't backdate (every edit signals "changed"
even if semantically identical). Bucket-1/2 tracked fns downstream still
backdate on their own `Eq` output types, so this is a minor, acceptable gap,
not a blocker.

**Fixpoint families** (`string_params`, `ptr_len`): keep as an explicit outer
Rust loop in the bridge layer that calls the memoized `*_program` tracked fn
repeatedly until its result stabilizes, rather than adopting salsa's native
`CycleRecoveryStrategy::Fixpoint` (`cycle_fn`/`cycle_initial` on
`#[salsa::tracked]`, confirmed present in the vendored `salsa-0.28.2`,
`src/function.rs`) up front. The native mechanism is a sharper edge than
"call a memoized query until equal, which is ~free on repeat calls once
nothing changed." Revisit only if the outer-loop call count is a measured
bottleneck.

### `QueryContext` after migration

Keeps its existing public method surface (`byte_source`, `pure`, `first_nul`,
...) so no rule under `query/rules/` changes. Internally it stops holding
`&FixupFacts`'s flat `Vec`s and instead holds `&dyn FixupDb` (or a concrete
`&SalsaFacts`) plus `&Program`; each method body calls the matching tracked fn
instead of linear-filtering a `Vec`. The `query_cache!` macro
(`context.rs:42`) is deleted once every cached query is salsa-backed — salsa's
memoization subsumes it, and today's cache is strictly worse (doesn't survive
past one snapshot anyway).

## Salsa API note (blocks Phase 0)

`src/backend/salsa.rs` today is 100% commented-out scratch code, and it mixes
API generations: `#[salsa::database(ProgramInput, PrecomputedFactsInput)]` is
old-style salsa — that macro shape doesn't exist in `salsa = "0.28"` (already
in `Cargo.toml`, unused). Confirmed against the vendored source
(`~/.cargo/registry/.../salsa-0.28.2`) and its `examples/lazy-input`: the
actual 0.28 shape is

```rust
#[salsa::db]
trait FixupDb: salsa::Database {}

#[salsa::db]
struct Database {
    storage: salsa::Storage<Self>,
}

#[salsa::db]
impl salsa::Database for Database {}
#[salsa::db]
impl FixupDb for Database {}
```

with `#[salsa::input(singleton)]` (that part of the scratch file is already
correct — `singleton` is real, confirmed in `src/input/singleton.rs`) and
`#[salsa::tracked(returns(copy) | returns(clone))]` for query functions.
Rewrite `salsa.rs` against this real API before anything else in Phase 0.

## Migration phases

All phases below are complete (`bd show slate-kby1` for the authoritative
child-issue breakdown, which diverged in numbering from the "suggested
children" sketch at the end of this doc). Phase 4 grew a "Phase 4b" beyond
what's described here — see [Where it landed](#where-it-landed).

**Phase 0 — `slate-kby1.1`** (filed): stand up a real, compiling
`salsa::Database` per the API note above. Migrate `def_use` + `effects` only.
Wire one pass to read them through salsa. Benchmark fact-build cost
before/after on a representative fixture. Land as coexistence: `QueryContext`
gets a salsa-backed path for exactly these two families; everything else
still reads legacy `FixupFacts` `Vec`s — same coexistence pattern `04q.75`
used between the query engine and legacy `Fixup`-trait passes.

**Phase 1 — bridge layer.** Build `SalsaFacts` (registry + `AllFunctions`
singleton + `set_body` wiring, including the improved `unbounded`/diff case
above) and have it own the two Phase-0 families end to end, replacing
`IncrementalFacts`'s role for just those two. Don't delete
`IncrementalFacts` yet — it still drives every other family.

**Phase 2 — remaining bucket-1 families.** One family per beads child,
mirroring `04q.75`'s per-pass migration children: extract a
`collect_for_function` split (following the `def_use`/`effects` template)
where one doesn't exist yet, port to a tracked fn, migrate the consuming
pass(es), benchmark, full `nextest` gate per child.

**Phase 3 — bucket-2 (interprocedural/map-reduce) families.**
`AllFunctions` + `local_*`/`*_program` tracked-fn pairs per family; fixpoint
families use the outer-loop-over-memoized-query pattern.

**Phase 4 — retirement.** Once every `FixupFacts` field has a salsa-backed
source: delete `facts::analyze`, `FixupFacts`'s flat `Vec` fields,
`splice_function`/`remove_items`/`purge_function_facts`,
`IncrementalFacts`/`Dirty`, and the `query_cache!` macro. Update
`QueryContext`'s constructor and all ~20 call sites in `backend/mod.rs`.
Update `docs/fixups.md`, `docs/facts.md`, `docs/passes.md` to describe the
salsa-backed flow — mirroring `04q.75`'s own closing acceptance criterion
("closes only after ... docs ... describe the new workflow").

## Open questions / risks

- **Backdating payoff needs `Eq` on fact structs.** Mostly already true; audit
  and patch the handful that aren't (see `BindingTypeFact` above) during
  Phase 2/3 as each family migrates, not all at once up front.
- **`#[salsa::input]` setters take `&mut db`** (`file.set_contents(&mut db).to(...)`
  in the vendored example), which serializes cleanly with today's
  single-threaded `apply_with_logger` loop. Not a real constraint now; note it
  so a future parallel-pass idea doesn't silently assume otherwise.
- **Registry entries for removed functions are dropped, not tombstoned.**
  Matches `function_by_item_index`'s existing miss-returns-`None` behavior, so
  no caller needs new handling for "function existed, now gone."
  Reconsider only if something needs to distinguish "never existed" from
  "existed, then removed."
- **`printf` and `file_ownership`'s bucket assignment above is a first read**,
  not a verified audit — confirm each collector's actual cross-function
  dependencies (or lack of them) when its migration child is picked up, the
  same way `slate-kby1.1` already flags `def_use`/`effects` specifically for
  being _provably_ bucket-1.

## Suggested beads children under `slate-kby1` (original sketch — see `bd show slate-kby1` for actual)

This was the pre-migration sketch, kept for historical comparison; the real
child issues filed and closed under `slate-kby1` don't match this numbering
one-to-one:

1. Fix `src/backend/salsa.rs` against the real 0.28 API (blocks `kby1.1`).
2. `slate-kby1.1` as already scoped (def_use + effects prototype + benchmark).
3. Bridge layer (`SalsaFacts`, registry, `AllFunctions`, `TouchedItems`-driven
   `set_body` wiring, improved `unbounded` diff case).
4. One child per remaining bucket-1 family (~17 families).
5. One child per bucket-2 family, fixpoint families called out separately
   (~10 families, `string_params`/`ptr_len` as their own children given the
   fixpoint-loop design needs its own review).
6. Retirement: delete `facts::analyze`/`FixupFacts`/`IncrementalFacts`/
   `query_cache!`, update docs.

## Where it landed

The bucket-1/bucket-2 split, the fixpoint-as-outer-loop design, and the
Phase 0-3 per-family migrations happened close to this plan. Phase 4
(retirement) went one step further than scoped here:

- **`Program` itself became the single `#[salsa::input(singleton)]`**
  (`ProgramInput` in `src/backend/salsa.rs`), not a per-function
  `HashMap<FunctionId, FunctionInput>` registry with hand-maintained
  `AllFunctions`. `FunctionInput` is a `#[salsa::interned]` `(ProgramInput,
FunctionId)` key instead of its own `#[salsa::input]`; its body, base-walk
  bindings, binding types, and loops are all tracked fns derived from
  `ProgramInput`, not separately-set fields.
- **`TouchedItems`-driven diffing (the "Bridging" section above) was never
  wired up as the invalidation mechanism, and was deleted outright** rather
  than kept as a fast path. `backend/mod.rs` calls
  `SalsaFacts::set_program(&program)` unconditionally after every edit — no
  `in_place`/`removed`/`unbounded` case analysis. Every function's tracked fns
  get a cheap rerun-and-compare against the previous memo on every edit;
  salsa's backdating (not a hand-maintained touched set) is what stops that
  from cascading into every dependent query. `TouchedItems` the struct lived
  on for a while as dead weight after this landed — computed, threaded
  through `EditSet`/`ItemPlan::apply`, and never read outside its own tests —
  until it was deleted for good (`slate-kby1.6.11`).
- This trades a theoretically tighter "only touched functions rerun" design
  for a much simpler one ("every function reruns a cheap check, salsa
  backdates the rest") at the cost of that cheap check scaling with program
  size on every edit. `slate-kby1.6.10` is the sanity check that this
  tradeoff doesn't regress on a realistic-sized fixture.

## Query-performance patterns

Recurring hotspot shape found while chasing rewrite/compile-time performance
(2026-08-09, `wiki/log/2026-08-09-00-00.md`): a fact lookup implemented as
`some_facts(db).iter().find(|f| f.key == target)` over a salsa-tracked `Vec`
looks cheap in isolation but is O(n) per call, and gets called from inside
another loop over items — the actual complexity is quadratic or worse. The
fix is consistently the same shape, not a one-off: add a second
`#[salsa::tracked(returns(ref))]` query that returns a `BTreeMap<Key, Fact>`
built from the first (`places_by_path`, `control_flow_by_key`,
`all_binding_refs` in `src/backend/salsa.rs`/`src/backend/query/context.rs`
are examples), and have callers key into the map instead of scanning the
`Vec`. Salsa memoizes the map build itself, so this only costs a rebuild when
the underlying facts actually change, not per lookup.

Two more hotspots worth knowing about if compile-time regresses again:
- **Clone-in-a-loop**: `expr_sites().into_iter()` (an owned, cloned
  iteration) inside a per-binding loop was polynomial; switching to
  `.iter()` and only cloning the one matched result fixed it (`65ab6bf3`).
  When a query result only needs to be read, not owned, prefer borrowing
  over `.into_iter()`/`.clone()` on a salsa-tracked collection.
- **Unconditional salsa DB construction**: some fixup passes don't touch the
  query layer at all; `apply_with_logger` (`src/backend/mod.rs`) skips
  constructing a fresh `FixupDb`/`ProgramInput` for those passes rather than
  paying setup cost every pass regardless of whether it's needed.

Also switched the process allocator to `mimalloc` (`#[global_allocator]` in
`src/main.rs`) alongside the def-use indexing work — reported as part of a
~25% overall compile-time improvement combined with the indexing changes.
