# Query/rewrite engine: gap analysis and redesign plan

This replaces the earlier generic tutorial draft. That draft described a
matcher design in the abstract (a `Rule<Node, Cx>`, a `Field<T, Cx>`
predicate combinator, a `Matchable` derive). Since it was written, real code
landed at `src/fixups/query/` implementing a good chunk of the same idea
against this codebase's actual `rust_ast` and `facts` types — `CallRule`,
`DefinitionRule`, `ProgramRule`, `QueryContext`, `Proof`/`Evidence`/
`Rejection`. It is not a stub; `memchr`, `libc_exit`, `anonymous_structs`,
and two `PruneUnusedDefinitions` rules run through it today.

`src/fixups/query/matcher.rs` (uncommitted) is a second, unrelated attempt at
the "declarative struct pattern" piece of the original plan. It doesn't
compile to anything useful — `matches!(fn_def, match_fn)` treats `match_fn`
as an irrefutable binding pattern, not a value to compare against, so it
matches every `Item::Fn` unconditionally. This is exactly the trap the
original plan called out in its first paragraph ("Rust patterns... cannot be
constructed dynamically and passed around as values"). Delete it; the real
starting point is `rule.rs`/`context.rs`, not `matcher.rs`.

The real gap isn't "we have nothing." It's:

1. The engine only has three rule kinds, each hand-built around one shape
   (call expressions, named definitions, whole-program anonymous-struct
   rewrite). There's no generic way to match an arbitrary `Stmt`/`Expr`
   shape or a *window* of statements, so anything that isn't "rewrite this
   one call expression" or "delete/replace this one named item" can't use
   the engine at all.
2. Facts are computed eagerly and in bulk (`facts::analyze(&program)`, ~40
   sub-analyses, called ~20 times across the pipeline in `fixups/mod.rs`),
   and `QueryContext`'s own per-query methods (`byte_source`, `const_u8`,
   `pure`, `first_nul`, ...) recompute from scratch on every call with no
   memoization. Nothing here is lazy.
3. There's no structural-pattern combinator layer (`Field<T, Cx>`,
   `Matchable` derive). Structural matching is hand-written per rule
   (`CallRule::candidates` special-cases `CallTarget`; `DefinitionRule`
   special-cases four selector kinds). That's fine at 5 rules; it will not
   scale to ~1000.
4. The 46 pre-existing rewrites in `src/fixups/rewrite/*.rs` (the ones the
   query engine hasn't absorbed yet) are exactly the "rewrite hidden away
   from the match" pattern you're trying to get away from: each implements
   `Fixup::fixup`, hand-walks the body with `src/fixups/support/walk.rs`,
   and open-codes its own structural test as a chain of `let ... else`.
   `src/fixups/rewrite/range_loop.rs` is a representative case — 148 lines,
   matches a `(Let, Loop)` statement pair against a precomputed
   `CountedLoopFact`, and splices in a `Stmt::For`. The query engine has no
   concept of "match N adjacent statements, replace with M statements," so
   this class of rewrite is structurally excluded today, not just
   unmigrated.

The rest of this document is a concrete plan to close these four gaps
without blowing the 10k-line budget, and without regressing the parts that
already work well (typed captures, `Proof`/`Rejection`/`Evidence`, the
if-a-case-doesn't-match-nothing-happens default).

## Layer boundaries (keep these separate)

The current code already draws one distinction correctly and should keep
doing so: **structural shape** vs. **semantic proof** are different kinds of
predicate and want different mechanisms.

- Structural shape ("is this a 3-arg call", "is this a `Let` immediately
  followed by a `Loop`", "does this struct have exactly these params") is a
  pure function of the AST node itself. This is what `Field<T, Cx>` /
  `Matchable` is for.
- Semantic proof ("is this pointer readable for `count` bytes", "is this
  extern declaration `noreturn`", "does this binding have zero other
  users") needs the fact base and can fail three ways (true / false /
  unknown-so-reject), which is exactly what `Proof`/`Rejection` already
  model. This should **not** become field-predicate soup — cross-argument
  relationships (`prefix_contains(count, nul)`) aren't expressible as
  independent per-field predicates anyway, as the original draft already
  noted under "cross-field ranges."

Concretely: keep `CallCaseContext`/`DefinitionCaseContext`-style imperative
`?`-chained case bodies for semantic guards. Add a structural pattern layer
in front of them so `candidates()` and the "does this shape even qualify"
checks stop being hand-written per rule. Don't try to fold proof queries
into `Field<T, Cx>::Predicate` — a `Cx`-parameterized field predicate can't
see a sibling field's captured value without smuggling it in some other way,
which is precisely the problem the draft's "Cross-field ranges" section
punts on.

## 1. Generalize the rule/plan machinery over node kind

`rewrite.rs` (`ExprPlanBuilder`/`ExprPlan`), `program.rs`
(`ProgramPlanBuilder`/`ProgramPlan`), and `definition.rs`
(`DefinitionPlanBuilder`/`DefinitionPlan`) are ~85% identical: collect
proposals, run cases in order, take the first `Replace`, group by target
site, flag `AmbiguousTarget`/`OverlappingTargets`/`MissingTarget`/
`CandidateRejected`, apply, log. That's three copies of the same conflict
resolution algorithm (~250 loc apiece) parameterized only by what "target"
and "apply an edit" mean for that node kind.

Collapse this into one generic engine:

```rust
trait Site: Ord + Clone {
    fn overlaps(&self, other: &Self) -> bool;
}

trait EditTarget {
    type Site: Site;
    fn site(&self) -> Self::Site;
    fn apply(self, program: &mut Program) -> ApplyOutcome;
}

struct Plan<E: EditTarget> {
    edits: Vec<PlannedEdit<E>>,
    diagnostics: Vec<PlanDiagnostic<E::Site>>,
}
```

`ExprSite::overlaps` becomes the existing `sites_overlap` (path-prefix
check); `DefinitionLocation::overlaps` becomes the existing item/extern-decl
overlap check; a new `StmtWindowSite` (below) gets a range-overlap check.
`RuleIdentity`/`RuleCaseIdentity`/`CaseRejection`/`PlanDiagnostic` stay as
they are — they're already node-kind-agnostic. This is pure deduplication:
expect it to *shrink* total LOC (three ~300-line builders collapse to one
~350-line generic one plus three ~30-line `Site`/`EditTarget` impls), not
grow it, while adding the new node kind below for free.

`ProgramRule`'s `ProgramRuleSelector` enum (currently one variant,
`AnonymousStructs`) is the weakest part of the existing design — it's an
enum that exists to be matched exhaustively for a single case. Once
`DefinitionRule`-style candidate collection is unified, program-level rules
that need "all of the program, once" as their candidate set can just use a
unit candidate (`()`) rather than a bespoke selector enum.

## 2. Add statement-window matching (closes the range_loop-class gap)

This is the highest-value structural addition, because it's what's missing
from *most* of the 46 unmigrated rewrites (range loops, slice loops, goto
cleanup, singleton scopes, dead-locals, ptr-copy — anything that looks at
more than one statement at a time or splices statements in/out).

Add a candidate/site kind alongside `ExprSite`:

```rust
struct StmtWindowSite {
    item_index: usize,
    path: AstPath,       // path to the containing Vec<IndentStmt>
    range: Range<usize>, // [start, end) index into that Vec
}
```

`StmtWindowRule` mirrors `CallRule`'s shape:

```rust
pub(in crate::fixups) struct StmtWindowRule {
    identity: RuleIdentity,
    width: usize, // e.g. 2 for a Let+Loop pair; matches every width-sized window
    cases: Vec<DeclarativeStmtWindowCase>,
}
```

`candidates()` slides a `width`-sized window over every `Vec<IndentStmt>` in
the program (functions, and recursively into nested bodies via the existing
`src/fixups/support/walk.rs` body-vec walker — no new traversal code
needed). Case functions get a `StmtWindowCaseContext` with:

```rust
fn stmts<const N: usize>(&self) -> [&IndentStmt; N];   // structural access
fn counted_loop(&mut self) -> Result<CountedLoopFact, Rejection>; // fact query
```

and a recipe returns `Vec<IndentStmt>` (replacement statements — 0 for
delete, 1+ for splice, same shape as today's `Stmt::splice` call in
`range_loop.rs`). `range_loop.rs`'s 148 lines (hand-rolled recursive walk +
hand-rolled pair match + hand-rolled splice + hand-rolled trace event)
collapse to something close to:

```rust
pub(in crate::fixups) fn rewrite() -> StmtWindowRule {
    StmtWindowRule::new(Pass::RangeLoop, "rewrite_counted_loop_to_range", 2)
        .case("zero_step_one", |case| {
            let [let_stmt, loop_stmt] = case.stmts();
            let index = case.let_name(let_stmt)?;
            let body = case.loop_body(loop_stmt)?;
            let fact = case.counted_loop()?;
            case.require(fact.start == CountedLoopStart::Zero)?;
            case.require(fact.step == CountedLoopStep::One)?;
            case.require(fact.index_use != CountedLoopIndexUse::SliceIndexOnly)?;
            Ok(for_range(index, fact.bound, body))
        })
}
```

roughly 15 lines, with the window-sliding, splicing, conflict detection, and
tracing all coming from the shared engine instead of being hand-rolled per
rewrite. This is the mechanism that gets the bulk of the 46 existing
`Fixup`-trait passes under the 50-loc/rewrite target — most of them are
"match a small fixed-size run of statements against a fact, replace it,"
same shape as `range_loop`, just with different fact types.

Single-`Expr`-shape rewrites that aren't calls (e.g. eventually "rewrite
this `Cast` chain," independent of the call-target machinery `CallRule`
hardcodes) want the same treatment: an `ExprShapeRule` with `width` replaced
by a structural `Field`-based pattern (see §4) instead of `CallTarget`
lookup. Build `StmtWindowRule` first since it unblocks strictly more of the
backlog; generalize `CallRule` into `ExprShapeRule` once the pattern layer
exists, since `CallRule` is really `ExprShapeRule` specialized to "is a call
to X."

## 3. Lazy, memoized facts inside `QueryContext`

Two different things are called "facts" here and they should get different
treatment:

**Program-shape indexes** (`calls`, `definitions`, `symbol_uses` in
`context.rs`) are already computed once per `QueryContext::new` in a single
linear pass. That's fine — they're cheap, always needed, and every rule
built on this engine wants them. Leave them eager.

**Derived semantic facts** (`byte_source`, `const_u8`, `const_usize`,
`pure`, `first_nul`, `full_byte_view`, `prefix_contains`,
`never_returning_extern`, `zero_users`, `zero_group_users`) are each
recomputed from `self.facts`/`self.program` from scratch on every call, with
no caching, even though many share subcomputations (`pure` and
`full_byte_view` both call into effect/value lookups keyed by the same
`ExprSite`; a rewrite with 3 cases against the same call site, like
`memchr`, calls `byte_source` twice from two different cases). Memoize
these:

```rust
struct QueryContext<'snapshot> {
    // ...existing eager indexes...
    cache: RefCell<QueryCache<'snapshot>>,
}

#[derive(Default)]
struct QueryCache<'snapshot> {
    byte_source: HashMap<ExprSite, QueryResult<ByteSource<'snapshot>>>,
    const_u8: HashMap<ExprSite, QueryResult<u8>>,
    const_usize: HashMap<ExprSite, QueryResult<usize>>,
    pure: HashMap<ExprSite, QueryResult<StableExpr>>,
    first_nul: HashMap<BindingId, QueryResult<NulPosition>>,
    // ...
}
```

Each `QueryContext` method becomes "check cache, else compute and insert."
`QueryResult<T>` already derives `Clone`, so this is a mechanical wrapper —
no change to case-function call sites, no change to `Proof`/`Rejection`
semantics. This is *within-snapshot* memoization: a `QueryContext` is built
fresh per `Program` snapshot (as it is today), so there's no staleness to
track within its lifetime — the AST it was built from is immutable for as
long as the context lives, so a cached result can never go stale before the
context itself is discarded. "Stale" only becomes a real concept at the
*pipeline* level (§5, below), not inside one `QueryContext`. Don't build
dependency-edge invalidation inside `QueryContext` — there's nothing to
invalidate against.

This closes the "same case re-derives the same fact" waste but is a modest
win by itself (memchr has 2 cases; even at 1000 rewrites, most call sites
are touched by O(1) cases). The bigger win is §5.

## 4. `Field<T, Cx>` and a `Matchable` derive — scope this down

The original draft's `Field<T, Cx>`/`FnDefPattern`/`Matchable` derive is the
right shape for the *structural* half of matching (§ "Layer boundaries"),
but the derive macro is the most expensive piece of this plan relative to
its payoff, and this crate isn't a workspace yet — a `proc-macro = true`
crate can't share a `Cargo.toml` with the binary/lib, so this means adding
`slate-macros` as a workspace member plus `syn`+`quote`+`proc-macro2` as
build-time dependencies. That's a real compile-time and structural cost,
not just a LOC cost.

Recommendation: build `Field<T, Cx>` by hand first (it's ~120 loc, no
macros needed — see the original draft's implementation, it's
codebase-agnostic and can be dropped in near-verbatim into
`src/fixups/query/field.rs`), and hand-write pattern structs only for the
node shapes that actually recur across many rewrites: `Stmt` (for
`StmtWindowRule` cases matching e.g. "a `Let` with no init" or "a `Loop`
whose body ends in `Break`") and the handful of `Expr` variants call/cast
rewrites repeatedly gate on (`Cast`, `MethodCall`, `ArrayPtr`). Don't
hand-write a pattern struct for `Item` or `Type` until a rewrite actually
needs one — right now nothing does; `DefinitionRule` matches items by
name/kind, which is a string lookup, not a structural pattern.

Only reach for the `Matchable` derive once hand-written pattern structs
exist for 3+ node types and the boilerplate is visibly repetitive (same
`Field::any()`-per-field constructor, same `matches()` conjunction). At that
point the derive is mechanical — expand each field into `Field<FieldTy,
Cx>`, generate a `Default`-via-`Field::any()` constructor, and generate
`.matches(&self, node: &Node, cx: &Cx) -> bool` — and the workspace-split
cost is easier to justify because it's amortized over real duplication
instead of paid up front speculatively. Treat this as Phase 3, not Phase 1.

## 5. Pipeline-level laziness (optional, separate from the query engine)

The user-visible "facts get re-analyzed constantly" pain is mostly
`fixups/mod.rs` calling `facts::analyze(&program)` ~20 times end to end,
each one recomputing all ~40 sub-analyses in `FixupFacts` regardless of
which passes actually run between one `analyze()` call and the next. That's
a pipeline-scheduling problem, not a query-engine problem — it's not inside
`src/fixups/query/` at all, it's the imperative sequence of `step!` blocks
in `fixups/mod.rs`. Fixing it for real means each of the ~40 sub-analyses in
`facts::analyze` becoming independently lazy/cached and invalidated by
*which AST paths actually changed* between passes — a proper incremental
dependency graph across the whole fixup pipeline, not just within one
`QueryContext`.

That's out of scope for "scaffold the query module": it touches all 46
non-query fixups and the `facts::analyze` entry point, is a much bigger
project than the matcher/rewrite ergonomics this doc is about, and the
value only shows up once most rewrites have migrated onto the query engine
(so there's a single scheduler to make lazy, instead of 46 independently
imperative `step!` blocks). Track it separately once migration is far
enough along that `fixups/mod.rs` is mostly `query::Plan` calls; don't block
this redesign on it. §3 gets the query engine itself off the "recompute
every call" pattern, which is the part actually inside its scope.

## memchr: current vs. what's actually worth changing

Current `rules/memchr.rs` (44 lines total, 2 cases, ~12–15 lines each) is
already close to the target shape:

```rust
.case("known_nul", |case| {
    let [source, needle, count] = case.args();
    let source = case.byte_source(source)?;
    case.u8_eq(needle, 0)?;
    case.pure(needle)?;
    let nul = case.first_nul(&source)?;
    case.prefix_contains(count, nul)?;
    Ok(pointer_at_or_null(source, known_index(nul)))
})
```

A `Field`/pattern-matching layer wouldn't shorten this case much — every
line here is a semantic proof query (`byte_source`, `u8_eq`... `pure`), not
a structural check, and §"Layer boundaries" above is explicit that proof
queries stay imperative. The place `Field<T, Cx>` earns its keep is
`CallRule::candidates`, which today hardcodes "look up by `(CallTarget,
arity)`" — fine for matching a specific known function, useless for a rule
that wants to match, say, "any call with a pointer-typed first argument and
a `usize`-typed last argument" without naming the callee. That's the case
where a `Field::predicate` over the candidate's argument types earns its
keep, and it's exactly the shape `CallRule` needs to grow into
`ExprShapeRule` (§2) to support C-stdlib rewrites that aren't 1:1 with a
single known symbol.

So: don't spend effort re-deriving `memchr` through `Field`/`Matchable` —
it's already near the 20-line target and gains nothing from the pattern
layer. Spend the `Field`/`Matchable` effort on `StmtWindowRule` cases and on
generalizing `CallRule`'s candidate selection, where it actually removes
hand-written structural matching.

## Rough LOC budget

| Piece | Current | Delta | Notes |
|---|---|---|---|
| `rewrite.rs` + `program.rs` + `definition.rs` | ~1080 | **−400** | collapse into one generic `Plan<E: EditTarget>` (§1) |
| new: `plan.rs` (generic engine) | 0 | +400 | replaces the three above |
| new: `stmt_window.rs` (rule + case ctx + apply) | 0 | +350 | §2 |
| `context.rs` cache wrapper | 1209 | +150 | §3, mechanical |
| new: `field.rs` (`Field<T, Cx>`) | 0 | +150 | §4, hand-written, no macros |
| new: hand-written `Stmt`/select `Expr` pattern structs | 0 | +250 | §4 |
| `matcher.rs` (delete) | 103 | −103 | broken scratch file, superseded by the above |
| existing `rule.rs`/`definition.rs` (kept, generalized to `EditTarget`) | ~440 | +50 | trait impls |
| `Matchable` derive (deferred, Phase 3 only) | 0 | +0 for now | separate proc-macro crate if/when triggered |
| **Total engine** | ~4078 | **~4775** | well under the 10k ceiling, room to spare for Phase 3 |

Per-rewrite cost after this: call-target rewrites stay at today's ~15–20
loc/case (already met); statement-window rewrites (the bulk of the backlog)
land at ~15–25 loc/case once `StmtWindowRule` exists, down from the
100–150 loc of a hand-rolled `Fixup` impl like `range_loop.rs` today.

## Suggested order

1. **§1 (generic `Plan<E>`)** first — it's a net LOC reduction and touches
   nothing about matching semantics, so it's low-risk and de-risks
   everything after it by giving §2 a plan/apply engine to plug into
   instead of writing a fourth bespoke one.
2. **§3 (memoized `QueryContext`)** — mechanical, no API change for
   existing rules, immediately safe to land alongside current rules.
3. **§2 (`StmtWindowRule`)** — the actual unlock. Prove it by migrating
   `range_loop.rs` (self-contained, well-isolated fact dependency) before
   touching anything with more moving parts like `slice_loop.rs` or
   `goto.rs`.
4. **§4, hand-written `Field`/pattern structs** — only once 2–3
   `StmtWindowRule` cases exist and the structural boilerplate is visibly
   repeated between them.
5. **`Matchable` derive** — only once §4's hand-written structs are
   repetitive across 3+ node types. Treat the workspace split as a real
   decision point, not a foregone conclusion.
6. **§5 (pipeline-level incremental facts)** — separate initiative, revisit
   once most of `src/fixups/rewrite/*.rs` has migrated onto the query
   engine.
