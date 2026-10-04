# Slate

Workspace rules (beads, testing from the root, session completion, commits)
live in [the root AGENTS.md](../../AGENTS.md). Run every command below from
the workspace root.

Slate translates C to Rust. Correctness is the only bar, and it is checked by
**differential testing**: compile and run both the C and the generated Rust,
then require identical stdout and exit code.

## Architecture

```
C -> slate-parser (preprocess, parse, sema) -> ir::Module
  -> src/frontend/lowerer.rs -> rust_ast::Program -> Rust source
```

- `crates/slate-parser` produces a typed `ir::Module` (places, bindings,
  explicit conversions, hoisted side effects). Slate consumes it directly as a
  library; never parse its printed IR.
- `src/frontend.rs` drives slate-parser;
  `src/frontend/lowerer.rs` lowers IR to `rust_ast`.
  `lower()` collects a `Barrier` (an unsupported `Construct` plus the IR
  node's `Site`) per function or global it cannot lower yet. A broken IR
  invariant aborts lowering with `InvalidIr`; it is never a barrier, and any
  test run that hits it panics.
- IR semantics are specified in [ir-spec](../../wiki/concepts/ir-spec.md) and
  its `wiki/concepts/ir/` subpages. Read the relevant subpage before lowering
  a new `ValueKind`, `Statement`, `PlaceKind`, or `Type`.
- [Slate architecture](../../wiki/concepts/slate-architecture.md) defines the
  single frontend and backend boundary. [Historical records](../../wiki/historical/index.md)
  preserve prior designs.

The frontend consumes slate-parser IR exclusively. `src/backend/` holds
rewrites and code generation. Rewrites are always on: differential suites run
`translate`, and `translate-project` always applies its rewrites.
`tests/fixtures.release/` fixtures go through `translate-project` and a
`cargo build --release` (`release_build_differential`), for behavior that only
shows up after LLVM optimization or across translation units: a `.c` file is a
one-unit project, a directory of `.c` files is a multi-unit project.
`tests/fixtures.library/<name>/src/` projects have no `main`: `library_differential`
translates them as a `staticlib`, links each `tests/*.c` driver against the
archive, and compares it with an all-native build.

## Toolchain

| Var              | Default                              | Role                                                          |
| ---------------- | ------------------------------------ | ------------------------------------------------------------- |
| `SLATE_SYSROOTS` | `~/.local/share/slate/sysroots`      | slate-parser reads target headers from `<dir>/<triple>`       |
| `SLATE_CARGO`    | `cargo`                              | compiles the generated Rust                                   |
| `SLATE_RUSTFMT`  | `rustfmt`                            | fallback formatter when syn rejects generated Rust |

Differential tests use `clang` on PATH as the C oracle. Slate frontend translation does not
invoke Clang.

Install a sysroot with `cargo run -p slate-c2rust -- sysroot install <triple>`.

## Debugging a fixture

```bash
cargo run --release -p slate-c2rust -- emit-slate-ir <file.c>                         # the IR slate receives
cargo run --release -p slate-c2rust -- translate <file.c>                            # Rust output
cargo run --release -p slate-c2rust -- lowering-barriers <file.c>                    # first barrier per function
```

## Testing

```bash
cargo nextest r --release --profile slate
```

The `slate` profile runs `differential` (`tests/fixtures`) plus the
c-testsuite, gcc-torture, gcc-dg, and chibicc corpus suites, all through the
slate frontend. `chibicc_suite` translates each two-TU fixture with
`translate-project`; external definitions become
`#[unsafe(no_mangle)]` items and the linker resolves symbols across modules.
Additional suites are tracked for restoration under `slate-p58o.6`; their
fixture directories remain.

Each suite is a ratchet: `tests/fixtures/` must pass, and
`tests/fixtures.unsupported/` must still fail. When an unsupported fixture
starts passing, `fixtures_unsupported_tests_still_fail` fails and prints the
`git mv` that promotes it. Corpus suites work the same way with their own
`*.unsupported` directories.

Isolate one fixture while developing:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --profile slate --test differential -E 'test(generated_differential)' --nocapture
```

Find the first barrier for every unsupported fixture:

```bash
cargo nextest r --release -p slate-c2rust --test differential -E 'test(fixtures_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

Corpus suites use their own selectors: `SLATE_GCC_TORTURE_FIXTURE`,
`SLATE_GCC_DG_FIXTURE`, `SLATE_C_TESTSUITE_FIXTURE`.
`gcc_torture_unsupported_triage_report`,
`c_testsuite_unsupported_triage_report`, and
`chibicc_unsupported_triage_report` are the corpus triage reports.

Batch suites write translated Rust under `crates/slate/target/*-suite/` and
build it under `crates/slate/target/test-cache/`. A `could not parse/generate
dep info` error usually means one cache subdirectory is stale; remove only
that `target-*` directory and rerun.

## Workflow for a lowering change

1. Put the fixture in `tests/fixtures.unsupported/` (or find the existing one)
   and confirm it fails with the expected barrier: run the triage report with
   `SLATE_DIFF_FIXTURE=<name>`.
2. Implement the lowering until the triage report prints `PASS <name>`.
3. `git mv` it into `tests/fixtures/`.
4. Run the full `slate` profile. It must be green; promote any other fixtures
   the change makes pass.

**FileCheck is suspended** until lowering covers enough that shape checks are
signal rather than noise. Do not add `@lowering`, `@rewrite`, or
`@slate-lowerer` markers or `SLATE-FILECHECK` blocks.

## Conventions

- **Never comment.**
- **Every feature starts with a C fixture**, and it must fail before you
  write the fix.
- **Feature testing is e2e differential fixtures, never unit tests.**
- **Transliterate first, idiomatize later.** Baseline Rust may be ugly:
  `#[repr(C)]`, raw pointers, explicit temps, `libc`, and `unsafe` are all
  acceptable.
