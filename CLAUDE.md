# Project Instructions for AI Agents

This file provides instructions and context for AI coding agents working on this project.

<!-- BEGIN BEADS INTEGRATION v:1 profile:minimal hash:7510c1e2 -->

## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full workflow context and commands.

### Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

### Rules

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files

**Architecture in one line:** issues live in a local Dolt DB; sync uses `refs/dolt/data` on your git remote; `.beads/issues.jsonl` is a passive export. See https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md for details and anti-patterns.

## Session Completion

**This project has no git remote** — beads data and code both live locally.
"Done" means the branch is merged into `main` in the coordination checkout, not
pushed anywhere. Do NOT run `git push` / `git pull --rebase`.

**When ending a work session**, complete ALL steps below. Work is NOT complete
until the branch is merged into `main`.

1. **File issues for remaining work** - Create issues for anything that needs follow-up
2. **Run quality gates** (if code changed) - `cargo fmt`, `cargo test`
3. **Update issue status** - Close finished work, update in-progress items
4. **INTEGRATE INTO MAIN** - from the coordination checkout, not the worktree:
   ```bash
   cd /home/takashi/Projects/slate
   git merge --no-ff work/<id>
   cargo fmt && cargo test
   bd close <id> --reason "..."
   ```
5. **Clean up** - `git worktree remove ../slate-<id>`, clear stashes
6. **Verify** - All changes committed and merged; `git status` clean
7. **Hand off** - Provide context for next session

**CRITICAL RULES:**

- Work is NOT complete until the branch is merged into `main`
- NEVER leave work stranded on an unmerged worktree branch
- There is no remote: do NOT `git push` or `git pull --rebase`
- Resolve merge conflicts in the coordination checkout and rerun tests
<!-- END BEADS INTEGRATION -->

## What Slate Is

Slate translates C to Rust by lowering **ClangIR (CIR)** — Clang's MLIR-based IR
— rather than LLVM IR, so it keeps structured control flow, integer signedness,
and named locals. It is transpilation, not decompilation. Correctness is the only
bar and is checked by **differential testing**: compile and run both the C and
the generated Rust, then require identical stdout and exit code.

For the current supported C subset (and what is _not_ handled yet), see
[docs/README.md](docs/README.md); `c.bnf` is a one-screen reference grammar of
that subset.

## Toolchain (prerequisite)

Nothing works without a **CIR-enabled Clang** (`CLANG_ENABLE_CIR=ON`). Tool paths
default to a local build and are overridable via environment variables:

| Var                                 | Default                                | Role                                     |
| ----------------------------------- | -------------------------------------- | ---------------------------------------- |
| `SLATE_CLANG`                       | `~/llvm-project/build-cir/bin/clang`   | emit CIR + Clang AST JSON                |
| `SLATE_CIR_OPT`                     | `~/llvm-project/build-cir/bin/cir-opt` | CIR → MLIR generic form                  |
| `SLATE_CC`                          | `clang` (from `PATH`)                  | compile the C side of differential tests |
| `SLATE_CARGO`                       | `cargo`                                | compile the generated Rust               |
| `SLATE_ALIVE_TV`                    | `~/alive2/build/alive-tv`              | translation-validate a fixup pass's before/after IR |
| `SLATE_TARGET` / `SLATE_CLANG_ARGS` | —                                      | shared target triple / extra clang flags |

## Build & Test

> **Always use `cargo nextest r --release` to test** (not `cargo test`).

```bash
cargo nextest r --release                   # unit + differential + fuzz
cargo fmt                                   # required before finishing

cargo run -- translate tests/fixtures/add.c # C -> Rust on stdout
cargo run -- emit-cir   tests/fixtures/add.c # inspect the CIR the lowerer sees
cargo run -- emit-fixtures                   # regenerate tests/fixtures.generated/ (git-ignored)

# fuzzer: random seeds each run; SLATE_FUZZ_SEED=<n> to replay, SLATE_FUZZ_CASES=<n> for count
cargo nextest r --release --test bnf_fuzz generator_differential --nocapture
```

## Architecture Overview

```
C ──emit──► CIR ──parse──► Op-tree ──lower──► Rust source
│  clang|cir-opt                    ▲
└──ast-dump=json──────► Clang AST ──┘
verified:  run(C).{stdout,exit} == run(Rust).{stdout,exit}
```

CIR is the primary lowering input; the Clang AST is the source-fact oracle, and
the two are joined by **source location**. `src/lower.rs` holds the `cir.*`
handlers; `src/c_ast.rs` extracts source facts from Clang JSON; `src/cir/`
parses the generic-form CIR op-tree. Do not add pass-scheduling machinery until a
feature needs it.

Read these before making changes — they are the real playbook:

- **[WORKFLOW.md](WORKFLOW.md)** — multi-agent ticket selection, linked-bead
  checks, branch/worktree setup, and integration rules. Start here before
  claiming work when other agents may be active.
- **[docs/adding-features.md](docs/adding-features.md)** — the workflow for
  adding coverage (baseline language feature vs. Rust fixup). Start here when
  adding a feature.
- **[docs/writing-a-fixup.md](docs/writing-a-fixup.md)** — the AST-to-AST recipe
  for a fixup pass (fixups are AST-to-AST only, never string rewrites). Read this
  before writing or modifying anything under `src/fixups/`.
- **[docs/effects.md](docs/effects.md)** — the Rust-to-Rust effects interpreter
  workflow for validating that raw lowered Rust and fixuped Rust preserve the
  same semantic effects. Read this before changing anything under `src/effects/`.
- [docs/architecture.md](docs/architecture.md) — sources, the two IRs, the
  pipeline, and why CIR over LLVM IR.
- [docs/passes.md](docs/passes.md) — the pass catalog: what runs, in what order.
- [docs/idiomatization.md](docs/idiomatization.md) — the `unsafe`/`libc` →
  idiomatic ladder.
- [docs/fuzzing.md](docs/fuzzing.md) — the stateful C-subset generator behind
  differential fuzzing (`tests/support/cgen.rs`).
- [docs/cfg-portability.md](docs/cfg-portability.md) — single-config vs.
  multi-config (`translate-cfg`) translation and the supported preprocessor
  predicate → Rust `cfg` mappings.
- [docs/README.md](docs/README.md) — the supported-subset surface.

## Conventions & Patterns

- **Every feature starts with a C fixture** in `tests/fixtures/` (C-only), driven
  by `cargo test --test differential generated_differential`.
- **Transliterate first, idiomatize later.** Baseline Rust may be ugly:
  `#[repr(C)]`, raw pointers, explicit temps, `libc`, and `unsafe` are all
  acceptable. Make it correct first; recover idiom in separate, verified fixups.
- **The lowerer emits structured AST, never rendered strings.** Everything in
  `src/lower.rs` builds `src/rust_ast.rs` nodes (`Item`/`Stmt`/`Expr`); `format!`
  into Rust source text is not allowed. Keep it as strongly typed as possible —
  favor a new enum variant over a `String` bridge, so the compiler enforces
  exhaustiveness and fixups can pattern-match the shape. If the AST cannot express
  something, add the node to `rust_ast.rs` rather than emitting text. Same rule
  for fixups — see [docs/writing-a-fixup.md](docs/writing-a-fixup.md).
- **Correctness lives in baseline lowering, never in a fixup.** A fixup must be
  optional in spirit — disabling it still leaves correct Rust.
- **Effects validation compares Rust to Rust.** `compare-effects-rust-rust`
  interprets raw lowered Rust and fixuped Rust; it is for checking fixup
  preservation, not C/CIR semantics. See [docs/effects.md](docs/effects.md).
- **Use shared fixup walkers.** Fact collectors should use
  `src/fixups/facts/walk.rs`; rewrite passes should use
  `src/fixups/support/walk.rs`. Do not add pass-local recursive `exprs`,
  `stmt_exprs`, or body walkers when the shared helper can cover the traversal;
  extend the shared helper if a common traversal shape is missing.
- **The generator only emits what Slate can translate.** When you extend the
  supported subset, extend `tests/support/cgen.rs` (and `c.bnf`) to match; keep
  every generated program well-defined (no UB) so C and Rust agree.
- **Comments explain why, not what.** One line at most. Only justify why the
  code is the way it is (a non-obvious constraint, a magic constant); never
  narrate what the code already says. If a name already makes the intent clear,
  don't comment. When in doubt, no comment.
- Run `cargo fmt` and `cargo test` before finishing.

See [AGENTS.md](AGENTS.md) for shell/tooling notes.
