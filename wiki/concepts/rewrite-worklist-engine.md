# Rewrite Worklist Engine (proposed)

Tracked by `slate-y0qs` (epic: rewrite pipeline performance). Not implemented
yet — this is the design arrived at during a 2026-08-26/27 architecture
discussion, written down before it's forgotten. Rewrite passes are currently
commented out in `src/backend/mod.rs` while lowering is the focus; this is the
design to land before re-enabling them at scale (~60+ rules).

## Problem

Today (see [fixups.md](../historical/fixups.md)) each `QueryRule` walks the _entire_
`Program` to find its candidates, and `to_fixpoint_program_with_facts`
(`src/backend/mod.rs`) reruns that per pass until the pass stops changing
anything. With N passes that's `O(passes × rounds × program_size)` — doesn't
compose as more rules are added. `SalsaFacts::set_program`
(`src/backend/salsa.rs`) also unconditionally clones the whole `Program` and
diffs every item for changes every round, purely to rediscover what
`Plan::apply` already knew it edited. Interprocedural fact families
(`string_params`, `ptr_len`, see [facts.md](../historical/facts.md)) run as an outer Rust
loop over a whole-program reduction until stable, which is effectively
`O(n × propagation_depth)` on call-graph-shaped programs.

CIR was considered and rejected as an alternative analysis substrate: Slate
doesn't link MLIR's C++ analysis infrastructure (it parses `cir-opt`'s text
dump — see [architecture.md](architecture.md)), so moving analysis there
would mean rebuilding the same tracked/memoized/incremental machinery salsa
already provides, while giving up the query engine's `EditSet`/conflict
detection/tracing and per-fixture `COMMON`/`REWRITES` gating. The fix is
scheduling, not substrate.

## Target shape

Replace "N passes, each a full walk to fixpoint" with one shared worklist,
rules dispatched by AST node shape, only touched-site neighbors requeued —
the same shape LLVM's InstCombine worklist uses. `QueryRule`/`case()` bodies,
`EditSet`, `Plan`/conflict detection, and `SalsaFacts` fact methods are
unchanged; this is a scheduling change around them, not a rewrite of the
matcher DSL.

```
Program ──► single walk ──► seed worklist with every candidate site
                                 │
                    ┌────────────▼────────────┐
              ┌────►│  worklist: BTreeSet<Site> │
              │     └────────────┬────────────┘
              │                  │ pop lowest (deterministic order)
              │     ┌────────────▼────────────────────────┐
              │     │ match site.node_kind()  (tier 1: free)│
              │     │   → BTreeMap<K, &[RuleCase]> (tier 2) │
              │     │     keyed by the rule's runtime-value │
              │     │     anchor (call target, cast kind…)  │
              │     └────────────┬────────────────────────┘
              │                  │ matches → PlanBuilder (unchanged
              │                  │ conflict/overlap detection)
              │     ┌────────────▼────────────┐
              │     │ Plan::apply (EditSet)     │
              │     │ mutate ONLY touched items │
              │     └────────────┬────────────┘
              │                  │
              │     ┌────────────▼─────────────────────────┐
              │     │ SalsaFacts: set_item() for touched     │
              │     │ items only — no full Program clone/diff│
              │     └────────────┬─────────────────────────┘
              │                  │
              │     ┌────────────▼─────────────────────────┐
              └─────┤ requeue: structural parent (AstPath    │
                    │ prefix) + def_use-linked neighbors      │
                    └─────────────────────────────────────┘
```

Complexity moves from `O(passes × rounds × program_size)` to roughly
`O(sites_initially_matching + edits × neighborhood_size)`. A site untouched
by any edit is visited once, ever.

## Data structures

Three separate decisions, not one:

1. **The worklist itself: `BTreeSet<Site>`, not `VecDeque`.** A plain queue
   needs a side set anyway to stop the same site being enqueued twice while
   pending (LLVM InstCombine pairs a `SmallVector` with a
   `DenseMap<Instr*, idx>` for this). `Site`/`AstPath` are already `Copy` and
   `Ord` ([facts.md](../historical/facts.md)), so `BTreeSet` gives dedup and deterministic
   pop order (function, then path) in one structure — needed since output
   must stay byte-identical for differential tests. A `HashSet` would dedup
   but not give reproducible order across runs.

2. **Rule dispatch: two-tier, not one flat table.**
   - Tier 1: the `Expr`/`Stmt` enum discriminant itself via `match` — "is
     this a `Call`? a `Loop`? a `Cast`?" is free and exhaustiveness-checked
     by the compiler, no data structure.
   - Tier 2: within a kind, a small `BTreeMap<K, &'static [RuleCase]>` built
     once at startup from the static rule registry, keyed by whatever
     runtime value the rule's top-level pattern anchors on (`CallTarget`,
     `CastKind`, loop shape, ...). A site's lookup is "match discriminant →
     index into the right small map," not "try all 60 rules."

3. **Requeueing touched neighbors: no new structure.** Structural parent is
   `AstPath` minus its last segment. Data-flow neighbors come from
   `DefUseFact` via `SalsaFacts::def_use`. Prerequisite: these lookups must
   already be `BTreeMap`-indexed, not `iter().find()` linear scans — see the
   "Query-performance patterns" section of
   [salsa-migration.md](../historical/salsa-migration.md), which flags this exact shape as
   a recurring hotspot. The worklist calls this per touched site repeatedly,
   so unindexed lookups here would reintroduce the quadratic cost the
   worklist is meant to remove.

## Open question flagged for implementation

`BTreeSet<Site>` is only cheap if `Site`'s `Ord` stays cheap. If a future
fact type gets folded into `Site`'s ordering and makes comparison expensive,
that needs to be caught before landing this, not after — worth a note in
whichever `slate-y0qs.3` PR does the work.

## Representation decisions are unification, not local rewrites

`slate-y0qs.4` (SCC-ordered call-graph worklist) is scoped today to
`string_params`/`ptr_len` specifically, but it's an instance of a more
general problem: any rewrite that changes a binding's Rust-side
representation (raw pointer → `Box`, `char*` → `&str`, pointer+length →
slice) requires every use of that binding, across function boundaries, to
agree — that is a whole-program constraint-propagation problem, not a
local peephole rewrite. The local worklist above (bounded by parent +
def-use neighbor requeue) has no complexity guarantee for this shape; it
needs its own phase, run to completion _before_ the local worklist emits
final AST:

1. One representation variable per binding/parameter, valued over a small
   fixed lattice (not an open-ended per-pass heuristic).
2. "These bindings must share a representation" becomes a union-find
   merge, not an `EditSet` mutation — near-O(n) amortized, an actual
   complexity bound rather than "fast in practice."
3. Solved once as an SCC-ordered call-graph fixed point (generalizing
   `slate-y0qs.4` beyond its current two fact families): a callee's
   decided representation only re-examines its direct callers.
4. Monotonicity — each variable only moves forward through the lattice,
   never back — bounds total revisits per binding at the lattice height,
   which is small and fixed. This is the canonical-forms guarantee the
   local worklist's "Open question" section flags as a risk (rewrite
   oscillation), made structural here instead of a matcher-authoring rule.

Only after this phase resolves does the local AST-to-AST worklist run, to
emit the decided representations and do peephole cleanup — by that point
no local rule needs another global round.

### Pointer capability lattice (c2rust-derived)

Superseded by [pointer-capability-lattice.md](pointer-capability-lattice.md),
which extends this table (drops `Box<[T]>`, adds `&str`/`String`/
`Option<...>`) and documents the implemented `src/backend/interproc/
pointer_lattice.rs` solve. Kept here for the original proposal history.

For pointer bindings specifically, the representation variable's lattice
is four independent capability facts — `write`, `unique`, `free`,
`offset` (does the pointer get advanced/indexed, not just dereferenced)
— mapping to one resulting Rust pointer type, adapted from c2rust's
analysis:

| write | unique | free | offset | Resulting type |
| :---: | :----: | :--: | :----: | -------------- |
|       |        |      |        | `&T`           |
|   x   |   x    |      |        | `&mut T`       |
|   x   |        |      |        | `&Cell<T>`     |
|       |   x    |  x   |        | `Box<T>`       |
|       |        |      |   x    | `&[T]`         |
|   x   |   x    |      |   x    | `&mut [T]`     |
|       |   x    |  x   |   x    | `Box<[T]>`     |

Each capability is itself a fact collector in the existing sense
(`facts.md`): computed once from `def_use`/`effects`/`heap_ownership`
evidence per binding, looked up by ID, not re-derived per pass. The
lattice gives inference a fixed, small join structure (16 raw
combinations collapse to 7 target types) instead of open-ended
heuristics per rewrite pass, and it composes with existing collectors —
`free` is `heap_ownership`'s free-site evidence, `unique` is
`borrow_alias`'s uniquely-mutated case, `offset` is whatever
`array_element_pointer_origin`/`slice_index` already prove about indexed
use. Escape hatch: if a call site can't accept the inferred pointer type
(e.g. an external/opaque caller), fall back to an accessor returning the
raw pointer at that site rather than blocking the whole binding's
inference or forcing a global re-decision — matches the existing
conservative-when-domain-incomplete policy in `fixups.md`.

Pointers should carry capability facts beyond this table too (nullability,
provenance/aliasing scope, ...) so later passes can make finer
representation choices without re-deriving them from raw AST shape.

## Benchmark target: libexpat

`slate-wcf7` (sqlite amalgamation) is blocked on `slate-wcf7.2`, the CIR
memory blowup on large single-TU macro-heavy files. Use libexpat instead
as the non-trivial-codebase benchmark for this epic — start translating it
now and track wall-clock as passes come back online and the worklist/
unification phases land, rather than waiting on the sqlite epic to
unblock before getting real signal on a real codebase.

## Related

- [fixups.md](../historical/fixups.md) — the matcher/`EditSet` layer this reuses unchanged.
- [facts.md](../historical/facts.md) — `def_use` and the `BTreeMap`-indexing pattern the
  neighbor requeue depends on.
- [salsa-migration.md](../historical/salsa-migration.md) — why facts are already
  incremental, and the "Query-performance patterns" hotspot section.
- `slate-y0qs` — epic tracking this; `slate-y0qs.1`–`.4` are the sequenced
  children (profile first, then `set_program` fix, then this worklist
  engine, then the interprocedural call-graph worklist).
