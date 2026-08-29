# Writing a Fixup

A fixup is an optional, separately verified cleanup pass that runs after
baseline CIR-to-Rust lowering. Baseline lowering is allowed to be ugly —
`#[repr(C)]`, raw pointers, explicit temps, `unsafe` — because correctness is
the only bar for it. Fixups recover idiom (safe references, `Vec`/`Box`,
`for x in ..`, compound assignment, ...) without changing behavior, and each
one is checked the same way baseline lowering is: differential testing
against a C fixture.

The pipeline has two layers:

- `src/backend/facts/` — read-only analysis. Reads an already-lowered
  `Program` and answers questions ("is this pure", "is this the last use",
  "does every caller prove X") without touching the AST. See
  [facts.md](facts.md).
- `src/backend/query/rules/` — the rewrites themselves, each one a `QueryRule`
  that selects candidates, checks preconditions (mostly by reading facts),
  and returns an `EditSet`. See [fixups.md](fixups.md) for the query engine's
  matcher/`EditSet` mechanics in depth — this doc is the map that gets you to
  the right chapter of that one, plus the parts fixups.md doesn't cover:
  picking a rewrite shape and wiring a new pass into the pipeline end to end.

`src/backend/mod.rs` runs a fixed sequence of passes (`Pass` enum in
`trace.rs`) over the `Program`, documented in order in [passes.md](passes.md).
Nothing here is discovered at runtime — the sequence, and each pass's
position in it, is hand-written.

## Picking a rewrite shape

`QueryRule<M>` is generic over what kind of thing it selects. Pick `M` by
asking what your rewrite needs to change atomically:

| You're changing...                                             | Matcher domain                             | Example pass                                         |
| -------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------- |
| One expression or call, in place                               | `Expression` / `FnCall`                    | `memchr_prelude::fixup_calls`                        |
| Whether a definition exists, or its whole body                 | `Definition`                               | `memchr_prelude`                                     |
| A parameter's type/presence _and_ every call site              | `Parameter`                                | `unused_params`, `array_element_pointer_param_hoist` |
| A contiguous run of statements (loop recovery, guard collapse) | `StatementSequence` / `StatementContainer` | `range_loop`, `retval`                               |
| Definitions, types, and expressions together, program-wide     | `WholeProgram`                             | `anonymous_structs`, `ptr_len`                       |

If what you need doesn't fit one atomic `EditSet` over one of these domains,
the rewrite is probably two passes, not one clever rule — see "One accepted
`EditSet` is transactional" in fixups.md before reaching for a workaround.

## Two ways to prove a case

Within any domain, how much of the soundness argument lives in the rule
itself versus in a dedicated facts module is a judgment call, not a fixed
rule:

- **Compose existing facts inline.** Most rules are this: a `.case(name, |case, item| { ... })`
  that chains `case.fact(|query| query.some_precondition(...))?` calls, each
  one a small, reusable `QueryContext` method. Reach for this whenever the
  check is local to the matched item and the facts you need already exist
  (or are cheap to add as one more `QueryContext` method — see "Adding a
  helper" in fixups.md). `unused_params.rs` and
  `array_element_pointer_param_hoist.rs` are this style even though they
  reason about a whole call domain, because `function_call_domain`,
  `parameter_uses`, and `call_argument` already exist as composable facts —
  no new facts module was needed.
- **A dedicated facts module behind a thin `WholeProgram` gate.** Reach for
  this when proving soundness needs its own fixed-point iteration or
  multi-step traversal that doesn't reduce to a handful of composed
  precondition calls — `ptr_len.rs` is the reference example: it iterates an
  `active` candidate set to a fixed point (`candidate_is_sound`,
  `all_callers_prove`) entirely in `src/backend/facts/ptr_len.rs`, and the
  rule in `query/rules/ptr_len.rs` is just
  `WholeProgram::when(|query| query.has_ptr_len_slices())` handing the result
  to a typed rewrite helper in `program_recipe.rs`. Put the analysis under
  `src/backend/facts/` (see facts.md's "Adding a fact") precisely when you'd
  otherwise be tempted to write that traversal as a pass-local walker inside
  the rule — don't; a fact collector is the only place whole-program
  reasoning is allowed to live.

Whichever style you pick, the rule itself should stay "candidate selection,
ordered preconditions, anchored typed edits" — never a hand-rolled AST walk.
If a rule needs to know something not already answerable through
`QueryContext`, add the fact or the query method first (facts.md /
fixups.md's "Adding a helper"), then consume it with `case.fact(...)`.

## Wiring a new pass end to end

This is the concrete checklist — every file a new pass touches, in order:

1. **Fixture first.** Add a C-only file under `tests/fixtures/` that exercises
   the shape you're recovering, before writing any rewrite code. Confirm
   today's baseline output with `cargo run -- translate tests/fixtures/<name>.c`
   so you know what you're changing. Scaffold the FileCheck blocks with the
   `@rewrite-begin`/`@lowering-begin` region directives plus
   `tools/update_filecheck.py --in-place` rather than by hand — see
   [differential fixtures](differential-fixtures.md)'s region-scoped section.
2. **Facts, if you need new ones.** If the rewrite needs information not
   already exposed by `QueryContext`, add a collector under
   `src/backend/facts/` (facts.md's "Adding a fact"), then a `QueryContext`
   method: a full `case.fact(...)`-compatible method returning
   `QueryResult<T>` with a `Predicate`/`EvidenceDetail` pair if it should
   participate in proof-evidence tracing (fixups.md's "Adding a helper"), or
   a plain passthrough method on `ItemCaseContext` (like `expr`,
   `is_bare_pointer_dereference`) when it's a structural helper with nothing
   to prove.
3. **The rule.** Write `src/backend/query/rules/<name>.rs`:
   `QueryRule::new(Pass::X, "case-family-name", Matcher { .. })` with one or
   more `.case(name, fn)`, each returning `Result<EditSet, Rejection>`. The
   first case that doesn't reject wins; if every case rejects, the matched
   item is left unchanged. Put non-trivial AST construction in `recipe.rs`
   (or a private helper in the rule file for something small and local),
   not inline in the case body.
4. **Register the module** in `src/backend/query/rules/mod.rs`
   (`pub(in crate::backend) mod <name>;`).
5. **Add the pass** to `src/backend/trace.rs`'s `Pass` enum — four places:
   the variant itself, `Pass::ALL`, `name()`, and `parse()`. `parse()`/`name()`
   feed `fixup-debug`'s `--only-pass`/`--debug-only-pass` flags and error
   messages, so a mismatch there is caught immediately by
   `valid_pass_names()`.
6. **Schedule it** in `src/backend/mod.rs` with the `step!` macro, choosing a
   position and a run shape:
   - **Single application** — `plan.apply(&mut program, &incremental, logger); incremental.set_program(&program);`
     once. Use this when one pass over the program is always enough (most
     rules).
   - **`loop { ...; if !report.changed { break } }`** — reapplies the same
     plan until it stops changing anything, reusing the shared `incremental`
     `SalsaFacts` (refreshed via `incremental.set_program(&program)` each
     round). Use this when one application of the rule can create a new,
     identically-shaped opportunity for itself (`dead_locals`,
     `unused_params` — removing one thing can make another removable).
   - **`to_fixpoint_program_with_facts(&mut program, limit, |program, salsa| { .. })`** —
     builds a _fresh_ `SalsaFacts` each round instead of reusing
     `incremental`. Use this when the fixup needs a self-contained facts
     snapshot per round rather than the shared incremental one (for example,
     `EarlyInlineTemps`, `ZeroInit`, `CallArgs`, `NullablePointer`, and
     `LateInlineTemps` all use it — grep `to_fixpoint_program_with_facts` in
     `src/backend/mod.rs` for the full set).

   Placement matters: put the step where the facts it depends on are already
   established and before anything that should observe its output. When in
   doubt, place it next to the pass whose output shape it consumes (mine —
   `array_element_pointer_param_hoist` — sits directly after
   `array_element_pointer_origin`, since it consumes the same pointer-origin
   facts that pass produces but only after crossing a call boundary).

7. **Clean up what you leave behind.** A rewrite that deletes the last use of
   a binding, or makes an `unsafe` block unnecessary, needs a later pass to
   actually remove it — check whether `dead_locals`/`remove_mut` already runs
   again after your new step; if not, add another `step!` call for it (see
   the "repeated passes" note at the bottom of passes.md, and
   `strip_redundant_unsafe` in `query/recipe.rs` for collapsing a
   binding-scoped `unsafe { .. }` once its contents no longer need it).
8. **Document it** as the next numbered entry in passes.md's pass sequence,
   renumbering everything after it.
9. **Test.** Iterate against just your fixture, then run the rewrite gate:

   ```bash
   SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --profile rewrites \
     --test differential -E 'test(generated_differential)' --nocapture
   cargo fmt
   cargo clippy --all-targets
   cargo nextest r --release --profile rewrites
   ```

   Cover both the accepted case and the important rejected fallbacks by
   asserting on translated source in `tests/differential.rs` — fixups are
   tested end to end through fixtures, never with `#[cfg(test)]` unit tests
   inside `src/backend/`.

10. **Use `fixup-debug` while developing**, not just at the end:

    ```bash
    cargo run -- fixup-debug tests/fixtures/<name>.c --debug-only-pass <name>
    cargo run -- fixup-debug tests/fixtures/<name>.c --up-to-pass <name>
    ```

    The trace shows which case matched (`query_case=...`) or why every case
    rejected, with the accumulated proof evidence — this is faster than
    reading generated Rust to find out why a rule didn't fire.

## Principles worth keeping in view

- **Never re-derive a fact by hand-walking the tree in a rule.** If you find
  yourself writing a recursive `Expr`/`Stmt` matcher inside `query/rules/`
  to answer a question a fact could answer, that question belongs in
  `src/backend/facts/` instead, even if only one rule will ever ask it.
- **"All callers" or "all users" needs a proven-complete domain.** Use
  `function_call_domain`/`definition_users` (which reject incomplete,
  address-exposed, or externally-reachable domains) rather than treating an
  empty _observed_ set of callers as proof there are none.
- **One `EditSet` per atomic change.** A signature change and its call sites
  are one edit set from one rule, not two rules chained across pipeline
  steps — the query engine validates every anchor in a set against the same
  unchanged snapshot and applies the whole set or none of it.
- **Cases are ordered and conservative.** The first case that doesn't reject
  wins; order specific cases before general fallbacks, and let ambiguous or
  missing evidence reject rather than guess.

## Where to go next

- [fixups.md](fixups.md) — the query engine in depth: matcher code for every
  domain, recipes, `EditSet` scheduling and transactional semantics.
- [facts.md](facts.md) — the facts layer: what each collector proves, who
  consumes it, and how to add a new one.
- [passes.md](passes.md) — the concrete, ordered pass catalog and
  `fixup-debug` usage.
