# Instructions for AI Agents

<!-- BEGIN BEADS INTEGRATION v:1 profile:minimal hash:970c3bf2 -->

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

## Agent Context Profiles

The managed Beads block is task-tracking guidance, not permission to override repository, user, or orchestrator instructions.

- **Conservative (default)**: Use `bd` for task tracking. Do not run git commits, git pushes, or Dolt remote sync unless explicitly asked. At handoff, report changed files, validation, and suggested next commands.
- **Minimal**: Keep tool instruction files as pointers to `bd prime`; use the same conservative git policy unless active instructions say otherwise.
- **Team-maintainer**: Only when the repository explicitly opts in, agents may close beads, run quality gates, commit, and push as part of session close. A current "do not commit" or "do not push" instruction still wins.

## Session Completion

This protocol applies when ending a Beads implementation workflow. It is subordinate to explicit user, repository, and orchestrator instructions.

1. **File issues for remaining work** - Create beads for anything that needs follow-up
2. **Run quality gates** (if code changed) - Tests, linters, builds
3. **Update issue status** - Close finished work, update in-progress items
4. **Handle git/sync by active profile**:

```bash
# Conservative/minimal/default: report status and proposed commands; wait for approval.
git status
```

5. **Hand off** - Summarize changes, validation, issue status, and any blocked sync/commit/push step

**Critical rules:**

- Explicit user or orchestrator instructions override this Beads block.
- Do not commit or push without clear authority from the active profile or the current user request.
- If a required sync or push is blocked, stop and report the exact command and error.
<!-- END BEADS INTEGRATION -->

## Slate

Slate translates C to Rust by lowering **ClangIR (CIR)** Clang's MLIR-based IR
rather than LLVM IR, so it keeps structured control flow, integer signedness,
and named locals. Correctness is the only
bar and is checked by **differential testing**: compile and run both the C and
the generated Rust, then require identical stdout and exit code.

For the current supported C subset (and what is _not_ handled yet), see
[docs/README.md](docs/README.md); `c.bnf` is a one-screen reference grammar of
that subset.

## Toolchain (prerequisite)

Nothing works without a **CIR-enabled Clang** (`CLANG_ENABLE_CIR=ON`). Tool paths
default to a local build and are overridable via environment variables:

| Var                                 | Default                                     | Role                                                                                                                                                                                                                                                                                                         |
| ----------------------------------- | ------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `SLATE_CLANG`                       | `~/llvm-project/build-cir/bin/clang`        | emit CIR + Clang AST JSON                                                                                                                                                                                                                                                                                    |
| `SLATE_CIR_OPT`                     | `~/llvm-project/build-cir/bin/cir-opt`      | CIR → MLIR generic form                                                                                                                                                                                                                                                                                      |
| `SLATE_CC`                          | `clang` (from `PATH`)                       | compile the C side of differential tests                                                                                                                                                                                                                                                                     |
| `SLATE_CARGO`                       | `cargo`                                     | compile the generated Rust                                                                                                                                                                                                                                                                                   |
| `SLATE_ALIVE_TV`                    | `~/alive2/build/alive-tv`                   | translation-validate a fixup pass's before/after IR                                                                                                                                                                                                                                                          |
| `SLATE_TARGET` / `SLATE_CLANG_ARGS` | —                                           | shared target triple / extra clang flags                                                                                                                                                                                                                                                                     |
| `SLATE_MACRO_DUMP_PLUGIN`           | `<SLATE_CLANG build>/lib/SlateMacroDump.so` | macro invocations plus include/function provenance, keyed by physical source offset                                                                                                                                                                                                                          |
| `SLATE_LIBC_SHIM`                   | `libc-shim/include`                         | directory SLATE_CLANG parses with `-nostdlibinc -isystem <dir>` instead of the host's system libc headers (clang's own builtin freestanding headers — stddef.h, stdint.h, stdatomic.h, etc. — stay available); set to a different directory to override, or to an empty value to fall back to system headers |

`c_ast.rs` always loads `SLATE_CLANG` with `-fplugin=$SLATE_MACRO_DUMP_PLUGIN`, so
that plugin must be built against the same clang tree `SLATE_CLANG` points at
before anything that parses C will run:

```bash
SLATE_CLANG=~/llvm-project/build-cir/bin/clang ./tools/macro-dump-plugin/build.sh
```

Rerun this after rebuilding `SLATE_CLANG` from source - the plugin links
against that tree's headers and must be rebuilt in lockstep.

## Build & Test

> **Always use `cargo nextest r --release` to test** (not `cargo test`).

```bash
cargo nextest r --release                   # unit + fixture differential
cargo fmt                                   # required before finishing

cargo run -- translate tests/fixtures/add.c # C -> Rust on stdout
```

During feature development, isolate the new differential fixture:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --test differential -E 'test(generated_differential)' --nocapture
```

Run the unfiltered full release suite only as the completion gate.

Generated fixture trees are ignored inspection artifacts. Do not run
`emit-fixtures` or `emit-lowered-fixtures` as part of implementation or session
completion. The user regenerates them manually when desired.

## Architecture Overview

```
C ──emit -> CIR -> parse -> Op-tree -> lower -> Rust source ->
│  clang|cir-opt                      ▲
└──ast-dump=json──────► Clang AST ────┘
verified:  run(C).{stdout,exit} == run(Rust).{stdout,exit}
```

CIR is the primary lowering input; the Clang AST is the source-fact oracle, and
the two are joined by **source location**. `src/lower.rs` holds the `cir.*`
handlers; `src/c_ast.rs` extracts source facts from Clang JSON; `src/cir/`
parses the generic-form CIR op-tree.

**Read before making changes**

- [docs/writing-a-fixup.md](docs/writing-a-fixup.md) — start here for any new
  or migrated rewrite: how to pick a rewrite shape and wire a new pass into
  the pipeline end to end.
- [docs/fixups.md](docs/fixups.md) — the
  preferred query-driven interface for supported expression and definition
  rewrites: matcher/`EditSet` mechanics in depth.
- [docs/facts.md](docs/facts.md) — the read-only analysis layer fixups
  consume instead of re-deriving facts by hand.
- [docs/architecture.md](docs/architecture.md) — sources, the two IRs, the
  pipeline, and why CIR over LLVM IR.
- [docs/passes.md](docs/passes.md) — the pass catalog: what runs, in what order.
- [docs/README.md](docs/README.md) — the supported-subset surface.

## Conventions & Patterns

- **Never comment.**
- **Every feature and fixup starts with a C fixture** in `tests/fixtures/` (C-only), driven
  by `cargo nextest r --test differential -E 'test(generated_differential)'`.
- **Testing**: Feature testing is done with e2e fixture differential tests, **never unit tests**.
- **Transliterate first, idiomatize later.** Baseline Rust may be ugly:
  `#[repr(C)]`, raw pointers, explicit temps, `libc`, and `unsafe` are all
  acceptable. Make it correct first; recover idiom in separate, verified fixups.
- **Keep the reference grammar current.** When you extend the supported subset,
  update `c.bnf`. The diagnostic structured generator does not need to grow with
  new features.
- Run `cargo fmt`, `cargo clippy` and `cargo nextest r --release` before finishing.
