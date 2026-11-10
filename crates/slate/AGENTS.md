# Slate

Workspace workflow, test profiles, and routing are in the
[root instructions](../../AGENTS.md).

## Boundary

Slate lowers the typed IR from `slate-parser` to Rust. The frontend consumes
`ir::Module` directly; do not parse printed IR. An unsupported construct is a
`Barrier`; a broken IR invariant is an `InvalidIr` error and panics any test
that hits it. A missing or wrong IR fact is fixed in `slate-parser`, not worked
around here.

```
ir::Module -> src/frontend/lowerer.rs -> backend::rust_ast -> src/backend rewrites -> Rust
```

## Where things live

- `src/frontend/lowerer.rs`, `src/frontend/lowerer/`: IR to `rust_ast`.
- `src/backend/engine/`, `src/backend/engine/rules/`: per-function rewrites.
- `src/backend/interproc/`: whole-project analyses (pointer, length, string
  params).
- `src/backend/link/`: cross-unit signature lifting, shared types, export calls.
- `src/backend/codegen.rs`, `rust_ast.rs`, `format.rs`: output.
- `src/project/`: `translate-project`, library and executable projects.

Rewrites are always on. Project rewrites run once over all units, then the
per-function rules run per unit.

## Read before changing

`llog search` ranks concept pages above logs and `wiki/historical/` pages
last; historical pages are superseded, so do not follow them.

| Change | Read |
| --- | --- |
| Lowering a new `ValueKind`, `Statement`, `PlaceKind`, or `Type` | [ir-spec](../../wiki/concepts/ir-spec.md) and its `ir/` subpage, [ir-to-rust-lowering](../../wiki/concepts/ir-to-rust-lowering.md) |
| Frontend/backend boundary | [slate-architecture](../../wiki/concepts/slate-architecture.md) |
| New or changed rewrite | [rewrite-engine-v2](../../wiki/concepts/rewrite-engine-v2.md), [pass-porting-workflow](../../wiki/concepts/pass-porting-workflow.md) |
| Pointer, length, or string-param analysis | [pointer-capability-lattice](../../wiki/concepts/pointer-capability-lattice.md) |
| Intrinsics, blocks, OpenMP | [intrinsic-lowering](../../wiki/concepts/intrinsic-lowering.md), [blocks-lowering](../../wiki/concepts/blocks-lowering.md), [openmp-lowering](../../wiki/concepts/openmp-lowering.md) |
| Fixture formats, FileCheck directives, per-target lists | [differential-fixtures](../../wiki/concepts/differential-fixtures.md) |
| Cross-target toolchains | [cross-target-toolchains](../../wiki/concepts/cross-target-toolchains.md) |

## Diagnose first

Decide whether a failure is parser, lowering, or rewrite before editing:

```bash
cargo run --release -p slate-c2rust -- emit-slate-ir <file.c>
cargo run --release -p slate-c2rust -- translate <file.c>
cargo run --release -p slate-c2rust -- lowering-barriers <file.c>
```

`emit-slate-ir` is what slate receives: a wrong fact there is a parser bug.
`translate` always runs the rewrites.

## Fixtures and tests

Every feature starts with a C fixture that fails first. Tests are e2e
differential fixtures, never unit tests. Runtime fixtures go in
`tests/fixtures/`; cases that must still fail go in
`tests/fixtures.unsupported/`. Project, library, release, and dylink fixtures
use their own `tests/fixtures.*` directories; see the
[fixture guide](../../wiki/concepts/differential-fixtures.md).

Lowering change:

1. Put the fixture in `tests/fixtures.unsupported/` and confirm it fails with
   the expected barrier.
2. Implement until it passes.
3. `git mv` it into `tests/fixtures/` (the ratchet prints the command).
4. Run the full `slate` profile and promote any other fixture that now passes.

Rewrite change:

1. Add or extend a `tests/fixtures/` fixture and confirm the output lacks the
   rewrite.
2. Implement, then regenerate the block and check that it shows the rewrite
   firing.
3. Run the full `slate` profile; regenerate each `FileCheck failed` fixture and
   review its diff.

FileCheck blocks are generated, never hand-edited:

```bash
python3 crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/<fixture>
```

Isolate one fixture:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest run --release --profile slate --test differential -E 'test(generated_differential)' --nocapture
```

Corpus suites use `SLATE_GCC_TORTURE_FIXTURE`, `SLATE_GCC_DG_FIXTURE`, and
`SLATE_C_TESTSUITE_FIXTURE`. A `could not parse/generate dep info` error means
one `target-*` cache subdirectory is stale: remove only that one.

## Conventions

- Never comment.
- Transliterate first, idiomatize later: baseline Rust may use raw pointers,
  explicit temporaries, `libc`, and `unsafe`. Matching C behavior is the bar.
