# Rewrite engine v2: ground-up replacement for src/backend/query + salsa

**Status: in progress, ~6/65 passes ported. This is a handoff spec for
whichever agent picks up `slate-y0qs.3` next.** Read this whole document
before touching code. It records a real decision made after measuring the
incremental approach and finding it insufficient -- don't re-litigate the
"should we rewrite from scratch" question without re-reading the "Rejected:
retrofit the existing engine" section below; that ground has already been
covered.

**Porting one more pass? Read
[pass-porting-workflow.md](pass-porting-workflow.md) first** -- it's the
per-session fast path (which pass to pick, how to port, how to verify, how to
benchmark) layered on top of the architecture this document specifies. This
document is the *what*/*why*; that one is the *how*, session to session.

## The mandate

libexpat (21 TUs) currently takes ~65-67s to translate with rewrite passes
enabled. Bare clang compiles it in a few seconds. Target: **under 10s**, and
the architecture must scale to 60+ rewrite passes *without* runtime growing
linearly (or worse) with pass count -- that's the actual epic goal
(`slate-y0qs`), not just "make libexpat faster once."

This is a full replacement of:

- `src/backend/query/` (the whole directory: `QueryRule`, `Matcher`,
  `EditSet`, `Plan`/`PlanBuilder`, `ItemPlanBuilder`, `query_items`, all of
  `rules/`) -- the batch-oriented matcher/edit engine.
- `src/backend/salsa.rs` (`SalsaFacts`/`ProgramInput`) -- the whole-program
  snapshot-diff incrementality plumbing.
- `src/backend/mod.rs`'s `apply_with_logger` -- the 1175-line, 65-stage
  straight-line pass sequence.
- Likely `src/backend/rust_ast.rs` too, since a tree that's cloned wholesale
  per fixpoint round doesn't fit an arena/stable-ID model. Confirm this once
  the arena design is nailed down; don't assume it survives unchanged.

**What survives:** the *algorithms*, not the *plumbing*.

- `src/backend/facts/*.rs` -- what each fact means and how to derive it from
  AST shape (binding uses, expression purity/effects, control-flow shape,
  cast reasoning, loop shapes, pointer/string/heap provenance). Port the
  logic; the caching/invalidation wrapper around it is being replaced.
- `wiki/concepts/passes.md`'s pass catalog -- the *semantic* description of
  what each of the 65 passes does and why, including the prose next to each
  hand-placed re-run explaining why it's there (e.g. "`string_params`
  re-run after `string_copy` and `printf_format` since each can expose a new
  liftable parameter"). That prose is the actual ordering-dependency
  knowledge this system has accumulated; it becomes rule *priority* in the
  new engine (see below), not a literal restatement of stage boundaries.
- Each `src/backend/query/rules/*.rs` file's *precondition reasoning* -- the
  "what makes this rewrite provably safe" logic inside each case body is
  real engineering (conservative-when-uncertain, evidence-gated). Port the
  reasoning into the new rule bodies; the `QueryRule`/`Matcher`/`EditSet`
  scaffolding it's written against does not survive.
- `tests/fixtures/`, `tests/fixtures.gcc-torture/`, chibicc/c-testsuite
  corpora, and the differential-testing contract itself (see "Validation
  contract" below) -- these are the ground truth the new engine must satisfy;
  nothing about them changes.

## Rejected: retrofit the existing engine

Recorded so nobody re-derives this and re-proposes it. Two retrofit ideas
were tried/analyzed this session, in order:

1. **Function-level touched-item rescanning.** Implemented as `TouchedItems`
   (`src/backend/query/item.rs`), `ItemPlanBuilder::add_rule_scoped`, and
   `to_fixpoint_program_with_facts_scoped` (`src/backend/mod.rs`): after a
   fixpoint round's edits land, only re-walk the item indices (functions)
   those edits touched, instead of the whole program, on the next round.
   Piloted on `ZeroInit`, `EarlyInlineTemps`, `LateInlineTemps`, `CallArgs`,
   `VarAliases`. **Measured result: 67.4s -> ~65.6s on libexpat, ~3-4%.**
   Root cause of the ceiling: this only cuts round-2+ cost. Round 1 -- one
   full walk per rule, unavoidable in this model since a rule has to look at
   a candidate at least once to know if it matches -- still dominates, and
   fresh per-pass timing data confirmed it: the *already-scoped* passes
   (`EarlyInlineTemps`, `ZeroInit`, `LateInlineTemps`, `CallArgs`) were still
   near the top of the per-pass cost list after scoping. Diminishing
   returns; not the right lever for a 6-7x target.

2. **Shared per-domain walk across rules (still inside the old engine).**
   Idea: several rules share a `QueryDomain` (e.g. `SingletonScopes` and
   `DeadLocals` are both `QueryDomain::Statement`) -- walk the domain once,
   try both rules per candidate, instead of two separate full walks. This
   is real and would help, but it surfaces a correctness landmine: today,
   each pass fully applies its edits and refreshes facts (`incremental.
   set_program`) before the next pass's `step!` block runs. Sharing the
   *candidate walk* across rules from different passes means the later
   rule evaluates against a **pre-mutation** snapshot instead of the
   post-earlier-rule-edit state it sees today -- a real behavior change,
   not just a perf one (`SingletonScopes` deletes/unwraps scopes, which
   changes the statement structure `DeadLocals` reasons about). Sharing the
   walk *and* keeping facts consistent requires also merging edit
   *application* into the same round, which requires making
   `PlanBuilder::finish` (`src/backend/query/plan.rs`) priority-aware:
   today, any two overlapping proposals get symmetrically dropped (fine
   when only one pass's proposals ever coexist, which is the case today);
   merging proposals from multiple differently-prioritized passes into one
   round needs "earliest-priority proposal wins the overlap," not "drop
   both." That's an invasive change to the transactional-apply core all 65
   existing passes' correctness depends on.

**Conclusion:** the retrofit cost (making the batch-conflict-resolution core
priority-aware, correctly, and re-validating all 65 passes against it) is
comparable to or greater than building a new engine that avoids the problem
structurally. A worklist that applies **one edit at a time** (pop a
candidate, find the first matching rule by priority, apply immediately,
requeue neighbors -- classic LLVM InstCombine shape) never has a batch of
simultaneous proposals to reconcile in the first place. That's the actual
argument for rewriting instead of retrofitting; it's not "the old code is
old," it's "the old code's batch-transactional shape is the wrong shape for
this problem, and changing that shape safely costs as much as a rewrite."

## Facts: keep the algorithms, replace the plumbing

This was the other decision point worth recording. `SalsaFacts` (`src/
backend/salsa.rs`) is a **batch snapshot-diff** model: `set_program` clones
the whole `Program`, salsa diffs it against the last snapshot, and backdates
per-function `#[salsa::tracked]` results that didn't change. That's a
reasonable fit for "apply a batch of edits, then refresh" (what the old
engine and even the `TouchedItems` retrofit both do), but it is **not** a
good fit for genuine one-edit-at-a-time application: every single edit would
pay a snapshot-rebuild tax (the ~9.5% clone+diff cost measured at today's
large per-round batch size -- multiplying that by "once per edit" instead of
"once per big batch" could plausibly make single-edit application *slower*
than today's batching, not faster).

Decision: **drop salsa for this engine.** Replace it with hand-rolled,
node-ID-keyed fact caches (e.g. `HashMap<NodeId, Fact>` per fact kind,
or a small struct-of-maps if that's cleaner), invalidated **explicitly and
locally**: editing binding B's initializer invalidates exactly B's own
cached facts (and whatever's structurally/def-use downstream of B), nothing
else recomputed, no whole-program refresh step at all. This is the same
idea `wiki/concepts/rewrite-worklist-engine.md`'s original design sketched
for "requeue: structural parent + def-use-linked neighbors" -- it now also
governs fact invalidation, not just worklist requeueing, since there's no
salsa layer doing that job anymore.

Port `src/backend/facts/*.rs`'s algorithms onto this new cache shape. Don't
try to preserve `#[salsa::tracked]` signatures or salsa's `Database`/
`Db`/`Update` trait machinery -- those are salsa-specific plumbing this
decision explicitly discards.

## Target architecture

1. **Arena-based AST with stable node IDs.** Not the current
   clone-per-round `rust_ast::Program` + `AstPath`-based traversal. Mutation
   should be O(1) in place. Every statement/expression/binding/definition
   gets a stable ID usable as a cache key and worklist entry. (A slotmap-
   style arena -- generational indices -- is a reasonable starting point so
   deleted nodes' IDs can't be silently reused and confuse a stale cache
   entry or a stale worklist entry.)

2. **One worklist**, seeded by exactly one traversal of the arena at start.
   Ordered/deduplicated (a `BTreeSet<NodeId>` or equivalent) -- output must
   stay reproducible for differential testing, so pop order needs to be
   deterministic, not just "whatever a `HashSet` iterates in this run."

3. **Two-tier dispatch**, built once from a static rule registry:
   - Tier 1: cheap node-kind discriminant (is this a call? a loop? a cast?
     a binding declaration?) -- free, exhaustiveness-checked by the
     compiler, no data structure beyond a `match`.
   - Tier 2: within a kind, a small map keyed by whatever concrete runtime
     value a rule's top-level precondition anchors on where that's
     extractable -- call target name is the big one (most of the 65 passes'
     call-rewriting rules key off a specific libc function name: `memchr`,
     `strcpy`, `sprintf`, `printf`, `qsort`, `malloc`, ...). A call to
     `printf` should only be tried against the handful of rules that care
     about `printf`, not every call-rewriting rule in the registry. Not
     every domain has an obvious tier-2 key (e.g. plain binding-declaration
     rules mostly don't) -- that's fine, tier 1 alone still collapses N
     separate walks into 1.

4. **Rule registry entries carry, at minimum:** node kind(s)/domain, an
   optional tier-2 anchor key, a **priority**, and a match+apply function.
   Priority should start as each rule's position in today's documented
   `passes.md` sequence (flatten the 65-step list, including the repeated
   re-runs, into a total order) -- that captures real "earlier enables
   later" knowledge distilled from real bugs, and using it as a tie-breaker
   preserves today's observable behavior as the default rather than
   re-deriving ordering from nothing. It is *not* required to stay a
   literal copy of today's stage boundaries forever -- once the new engine
   is validated against the existing corpora, priorities can be tuned -- but
   start there, don't start from an unordered pile of 65 rules and hope
   confluence just works out.

5. **Core loop:** pop lowest node ID (or whatever the worklist's
   determinism policy is) -> tier-1/tier-2 dispatch to a small candidate
   rule list -> try candidates in priority order -> first whose
   preconditions hold applies **immediately** (single in-place mutation) ->
   invalidate exactly that node's own cached facts (and downstream
   dependents per the def-use/structural graph) -> requeue: the edited
   node's structural parent + its def-use neighbors -> continue until the
   worklist drains.

## Known risks to carry forward, not rediscover

- **Termination isn't automatic.** Priority-ordered rule interaction plus
  incremental facts doesn't hand you confluence for free; nothing here
  currently proves the worklist can't oscillate (rule A's edit re-enables
  rule B, whose edit re-enables rule A, forever). `wiki/concepts/
  rewrite-worklist-engine.md`'s "Open question" section flagged this
  already for the local worklist; it applies at least as much here. At
  minimum, carry forward something like today's `FixpointLimit::Rounds`
  safety valve (a hard cap, used today for programs over ~2000 statements)
  scoped to "total edits applied" rather than "rounds," and treat hitting
  it as a bug to investigate, not a silently-accepted timeout.
- **Interprocedural rules are a different problem, not a smaller instance
  of this one.** `string_params`, `ptr_len`, and anything using
  `function_call_domain`/`direct_calls`-style whole-call-graph reasoning
  need an SCC-ordered call-graph fixed point (`slate-y0qs.4`,
  `rewrite-worklist-engine.md`'s "Representation decisions are unification,
  not local rewrites" section, including the c2rust-derived pointer
  capability lattice for `write`/`unique`/`free`/`offset`). Don't try to
  shoehorn these into the local per-node worklist above; they need their
  own phase, run before the local worklist emits final AST, and that phase
  is still open work independent of this rewrite.

## What's currently in the working tree (uncommitted)

As of this handoff, `src/backend/mod.rs`, `src/backend/query/item.rs`, and
`src/backend/query/mod.rs` have **uncommitted** changes implementing the
rejected retrofit (`TouchedItems` etc., see "Rejected" section above and
`wiki/log/2026-08-27-13-23.md` / `wiki/log/2026-08-27-13-38.md` for the
detailed history and measurements). Nothing from this session has been
committed. Since `src/backend/query/` is being deleted wholesale by this
rewrite, those changes are moot -- either `git checkout` them away before
starting, or just let them get deleted along with the rest of the directory.
Don't try to preserve or build on top of `add_rule_scoped`/
`to_fixpoint_program_with_facts_scoped`; they're superseded, not a
foundation.

## Validation contract (non-negotiable regardless of internal architecture)

Per `CLAUDE.md`: correctness is differential testing -- compile and run both
the C and the generated Rust, require identical stdout and exit code. That
bar does not move. Concretely, before considering any slice of this rewrite
done:

```
cargo nextest r --release --profile lowering   # if touching lowering/AST shape
cargo nextest r --release --profile rewrites   # the real target for this work
cargo nextest r --release --profile libc       # if touching libc-shim-adjacent facts
cargo fmt && cargo clippy --all-targets
```

Also holds, and needs explicit attention, not just "run the suite and see":

- Some fixtures carry FileCheck-style structural assertions on the
  generated Rust itself (`support::filecheck::check_generated_rust`,
  referenced from `tests/differential.rs`), not just runtime-behavior
  equivalence -- e.g. asserting a specific idiom like `.sum()` or
  `OnceLock::get_or_init` actually appears. These are a real regression
  net for "did pass X still fire the way it's supposed to," useful signal
  during the rewrite, not just a hurdle to clear at the end.
- `tests/differential.rs` has several tests asserting on `fixup-debug`
  trace output by rule/case *identity* (e.g. `stdout.contains
  ("query_rule=rewrite_anonymous_structs")`, count assertions on
  `query_case=known_origin` occurrences). These are tied to the *old*
  `Pass` enum and rule-naming scheme and will need rewriting to match
  whatever identity scheme the new engine uses -- expect to update these
  tests' expected strings, not to preserve the old naming as a hard
  constraint.
- `slate-mmiy` (anonymous-struct field-name mismatch after re-enabling
  rewrite passes) and the other pre-existing failures tracked under
  `slate-90t8` are bugs in the *old* engine. The new engine isn't expected
  to inherit or specifically fix them -- they may simply not recur if the
  new engine handles the underlying case differently, or may need their
  own new investigation if they do. Don't treat "still 9 tests failing"
  as a pass/fail gate for this rewrite; treat "no *new* failures beyond
  whatever `slate-90t8` already tracks" as the gate, and update `slate-90t8`
  if the set changes shape.

## Baseline numbers to beat

Measured this session, libexpat (21 TUs,
`~/c-corpus/libexpat/expat/build/compile_commands.json`), same machine,
`SLATE_FIXUP_TIMING=1 translate-project --lib` (that env var was old-engine-only
instrumentation and is a no-op against the new engine -- confirmed absent from
`src/` entirely as of 2026-08-28; wrap the binary invocation in wall-clock
timing instead, e.g. `date +%s.%N` before/after, and note the actual CLI shape
is `translate-project --lib --compile-commands <file> <project_dir> <crate_dir>`,
not a bare `--lib <dir>`):

| state | wall time |
| --- | --- |
| unscoped baseline (`4281c7e2`, before any `y0qs.3` work) | 67.4s |
| + function-level touched-item scoping (rejected retrofit, for reference) | ~65.1-66.1s |
| **target for this rewrite** | **< 10s** |

Per-pass cost breakdown at the "rejected retrofit" state (top offenders,
`step pass=` totals summed across all 21 TUs) is in `wiki/log/
2026-08-27-13-38.md` if useful for picking which passes to port first for
an early real measurement -- `SingletonScopes` (5.8s), `EarlyInlineTemps`
(4.6s), `ZeroInit` (3.1s), `PtrLen` (2.7s, interprocedural -- see risks
above), `LateInlineTemps` (2.0s), `RemoveMut` (1.7s), `DeadLocals` (1.5s)
were the largest single contributors on that corpus. Not a mandate to port
in that exact order, just a reasonable place to look for an early, real,
measurable proof-of-architecture number rather than porting all 65 blind
before the first measurement.

## Suggested first slice

Don't port all 65 passes before getting a number. A reasonable first
checkpoint: arena + worklist + two-tier dispatch core, hand-rolled fact
cache for whatever facts `ZeroInit` and `SingletonScopes` actually need
(binding declaration/uses/values for the former, statement-sequence/scope
shape for the latter -- deliberately two structurally different rule shapes
so the dispatch table gets exercised on more than one node kind), validated
against their existing fixtures (`tests/fixtures/zero_init.c`, whatever
`SingletonScopes` fixtures exist) plus a full `rewrites` profile run to
confirm no regression beyond the already-tracked `slate-90t8` set. Re-measure
libexpat at that checkpoint even though only 2 of 65 rules are ported --
it won't show the full 10s target yet, but round-1 cost for those two rules
should already look qualitatively different (one shared walk instead of two
separate ones, tier-2 dispatch narrowing candidates) and that's the signal
that the architecture is actually working before sinking time into porting
the remaining 63.

## Related

- [rewrite-worklist-engine.md](rewrite-worklist-engine.md) -- the original
  (less radical) design this supersedes for the *scheduling* layer; its
  data-structure reasoning (`BTreeSet<Site>` for determinism, tier-1/tier-2
  dispatch shape, def-use neighbor requeue) still applies here almost
  unchanged. What's different in v2 is facts (salsa -> hand-rolled) and the
  scope of what's being replaced (originally scoped as "reuse `QueryRule`/
  `EditSet`/`Plan` unchanged"; that scoping is what this document reverses,
  with the reasoning above for why).
- [passes.md](passes.md) -- the pass catalog to port semantics from.
- [facts.md](facts.md) -- the fact catalog to port algorithms from.
- [pass-porting-workflow.md](pass-porting-workflow.md) -- the per-session
  fast path for picking, porting, verifying, and benchmarking one more pass;
  read that instead of re-deriving the same steps from scratch each time.
- [salsa-migration.md](../historical/salsa-migration.md) -- history of why
  salsa was adopted in the first place; useful background for why this is a
  real reversal and not just churn, but its "Query-performance patterns"
  hotspot section (BTreeMap-indexed lookups, not `iter().find()` scans) is
  still good advice for the hand-rolled fact cache too.
- `slate-y0qs` -- epic tracking this; `slate-y0qs.3` is the in-progress
  child this document is the handoff for. `slate-y0qs.4` (SCC-ordered
  call-graph worklist) is the separate interprocedural phase referenced
  above.
