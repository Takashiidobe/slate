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
  -> src/slate_parser_frontend/lowerer.rs -> rust_ast::Program -> Rust source
```

- `crates/slate-parser` produces a typed `ir::Module` (places, bindings,
  explicit conversions, hoisted side effects). Slate consumes it directly as a
  library; never parse its printed IR.
- `src/slate_parser_frontend.rs` drives slate-parser;
  `src/slate_parser_frontend/lowerer.rs` lowers IR to `rust_ast`. Anything it
  cannot lower yet returns `Error::Unsupported`, which is a per-function
  lowering barrier.
- IR semantics are specified in [ir-spec](../../wiki/concepts/ir-spec.md) and
  its `wiki/concepts/ir/` subpages. Read the relevant subpage before lowering
  a new `ValueKind`, `Statement`, `PlaceKind`, or `Type`.
- The migration plan is the `slate-p58o` epic (`bd show slate-p58o`).

**Legacy code:** `src/frontend/` (ClangIR + Clang AST lowering) and
`src/backend/engine/` (rewrites/fixups) belong to the old CIR pipeline. Do not
extend them or cite their wiki pages (`lowerer-internals.md`, `passes.md`,
`rewrite-engine-v2.md`) for slate-frontend work. They are removed in Phase 6.

**Rewrites are not enabled for the slate frontend yet.** Only raw lowering
(`translate-lowered --frontend=slate`) is tested; there is no rewrites
profile.

## Toolchain

| Var              | Default                              | Role                                                          |
| ---------------- | ------------------------------------ | ------------------------------------------------------------- |
| `SLATE_SYSROOTS` | `~/.local/share/slate/sysroots`      | slate-parser reads target headers from `<dir>/<triple>`       |
| `SLATE_CLANG`    | `~/llvm-project/build-cir/bin/clang` | compiles the C side of differential tests (the oracle)        |
| `SLATE_CARGO`    | `cargo`                              | compiles the generated Rust                                   |
| `SLATE_RUSTFMT`  | `rustfmt`                            | formats generated Rust                                        |

Install a sysroot with `cargo run -p slate-sysroots -- install <triple>`.

## Debugging a fixture

```bash
cargo run --release -p slate -- emit-slate-ir <file.c>                         # the IR slate receives
cargo run --release -p slate -- translate-lowered --frontend=slate <file.c>   # raw Rust output
cargo run --release -p slate -- lowering-barriers <file.c>                    # first barrier per function
```

## Testing

```bash
cargo nextest r --release --profile slate
```

The `slate` profile runs `differential` (`tests/fixtures`) plus the
c-testsuite, gcc-torture, and gcc-dg corpus suites, all through the slate
frontend. Other test binaries in this crate exercise the legacy CIR pipeline
and are not gates.

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
cargo nextest r --release -p slate --test differential -E 'test(fixtures_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

Corpus suites use their own selectors: `SLATE_GCC_TORTURE_FIXTURE`,
`SLATE_GCC_DG_FIXTURE`, `SLATE_C_TESTSUITE_FIXTURE`.
`gcc_torture_unsupported_triage_report` and
`c_testsuite_unsupported_triage_report` are the corpus triage reports.

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
`@slate-lowerer` markers or `SLATE-FILECHECK` blocks, and do not run
`tools/update_filecheck.py`.

## Conventions

- **Never comment.**
- **Every feature starts with a C fixture**, and it must fail before you
  write the fix.
- **Feature testing is e2e differential fixtures, never unit tests.**
- **Transliterate first, idiomatize later.** Baseline Rust may be ugly:
  `#[repr(C)]`, raw pointers, explicit temps, `libc`, and `unsafe` are all
  acceptable.
