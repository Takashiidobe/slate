# Porting a pass to the worklist engine (slate-y0qs.3 fast path)

Read this before picking up "port another pass" work under `slate-y0qs.3`.
It exists so that loop doesn't re-derive the same context (and the same
dead ends) every session. For the target architecture itself, read
[rewrite-engine-v2.md](rewrite-engine-v2.md) — this doc is only the
per-session workflow on top of that.

## Picking which pass to port next

Don't re-derive this from the epic + all its children + both long design
docs. Two files are enough:

1. `src/backend/engine/rules/mod.rs`'s `registry()` — what's already ported.
2. The per-pass cost table in `wiki/log/2026-08-27-13-38.md` (`SingletonScopes`
   5.8s, `EarlyInlineTemps` 4.6s, `ZeroInit` 3.1s, `PtrLen` 2.7s
   interprocedural — see `rewrite-engine-v2.md`'s risks section, it's a
   separate phase — `LateInlineTemps` 2.0s, `RemoveMut` 1.7s, `DeadLocals`
   1.5s).

Pick whichever's heaviest and not yet in the registry, or the natural
sibling of whatever was just ported (e.g. `LateInlineTemps` right after
`EarlyInlineTemps` — the old query engine implemented both as one shared
`apply(case, binding, phase)` function, so the pairing is already known).

`wiki/historical/salsa-migration.md` is **not** needed for this — it
documents a different, already-superseded migration (query engine -> salsa,
not salsa -> worklist engine) and is pure historical record now.

## Porting the logic

- Read the nearest already-ported sibling rule file in full first (e.g.
  `inline_temps.rs`'s `EarlyInlineTemps`) — it's a working template for the
  old-engine -> new-engine translation: `Stmt`-matching becomes
  `NodeKind`-matching, `&str` name comparisons become `Ident` equality,
  `expr_ident_count` becomes `e_ident_count`, etc.
- Only fall back to the old query-engine source for logic the sibling
  doesn't cover. `src/backend/query/` was deleted from `main` wholesale when
  the rewrite started, but a pre-deletion copy survives under
  `.claude/worktrees/<hash>/src/backend/query/rules/` — check for such a
  worktree before assuming the old logic is gone for good.
- Register the new rule in `rules/mod.rs`'s `registry()` with a `priority()`
  matching its position in `passes.md`'s numbered pass list (not necessarily
  its exact number — existing entries are approximate, just keep relative
  order right against neighbors already registered).

## Verifying it

- `cargo nextest r --release --profile rewrites` is the real gate. A ported
  pass that's a verified 1:1 port of an already-shipped algorithm (same
  matcher, same edit shape, reusing the sibling's helpers) does **not** need
  a bespoke new fixture under `tests/fixtures/` — CLAUDE.md's "every feature
  starts with a fixture" targets *new* behavior; the existing differential
  corpus already exercises the ported logic broadly.
- Don't hand-construct a minimal C repro to prove one specific guard/branch
  fired. Tracing conservative safety checks (e.g. a `type_anchored`/
  `is_top_level_use`-style guard) by hand to build a case that exercises
  exactly one path is a real time sink for a thin payoff — trust the
  broader suite instead.
- A failing test isn't necessarily your change: `target/test-cache/` is a
  shared, reused build/test cache (`.cargo/config.toml` sets
  `target-dir = "target/test-cache"`, so `cargo build --release` never
  produces `target/release/<bin>` — the binary is always at
  `target/test-cache/release/slate`). A `could not parse/generate dep info`
  failure means a stale artifact in that shared cache, not a regression —
  `rm -rf` the specific `target/test-cache/target-*` subdir the failing test
  names and rerun before concluding your change broke it.

## Benchmarking

- `SLATE_FIXUP_TIMING` is dead. It was old salsa/query-engine
  instrumentation; the new engine has no equivalent env var. Wrap the binary
  invocation in wall-clock timing yourself (`date +%s.%N` before/after,
  3 runs, eyeball the spread).
- `translate-project --lib` needs `--compile-commands <file>
  <project_dir> <crate_dir>` — check `slate translate-project --help`
  rather than guessing flags (`--out` is not one of them).
- To isolate one pass's marginal cost: build twice (with/without the new
  rule registered), keep both binaries, run each 2-3x on the same corpus
  (libexpat: `~/c-corpus/libexpat/expat/build/compile_commands.json`, 21
  TUs). `git stash`/`stash pop` around a rebuild works for a single
  before/after but is wasteful across more than one comparison in a
  session — prefer a `git worktree add` for the baseline build if
  benchmarking more than one change.
- Session shell is fish, not bash: `time (cmd)` and `/usr/bin/time` both
  fail outright.

## Related

- [rewrite-engine-v2.md](rewrite-engine-v2.md) — the target architecture and
  handoff spec this workflow operates under.
- [passes.md](passes.md) — the pass catalog and ordering rationale.
- `wiki/log/2026-08-27-13-38.md` — the per-pass cost table.
- `wiki/historical/salsa-migration.md` — background only, not needed to port
  a pass; read it solely if you need history on *why* salsa was adopted and
  then dropped.
