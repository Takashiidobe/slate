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

## Session Completion

This protocol applies when ending a Beads implementation workflow. It is subordinate to explicit user, repository, and orchestrator instructions.

1. **File issues for remaining work** - Create beads for anything that needs follow-up
2. **Run quality gates** (if code changed) - Tests, linters, builds
3. **Update issue status** - Close finished work, update in-progress items
4. **Handle git/sync by active profile**:
5. **Every change must have a corresponding log**: - create a new log
   with `llog new` for every change made, summarizing the change, no
   more than 30 lines.
6. **Hand off** - Summarize changes, validation, issue status, and commit.

**Critical rules:**

- Explicit user or orchestrator instructions override this Beads block.
- Do not push without clear authority from the user.
- If a required sync or push is blocked, stop and report the exact command and error.
<!-- END BEADS INTEGRATION -->

## Slate-Parser

FileCheck expectations are generated. After changing a fixture or its
renderer, run `python3 tools/update_filecheck.py --in-place <fixture>`;
do not write `CHECK` lines by hand.

### Wiki

Non-obvious project context lives in `wiki/` (see `llog`). Check it before
re-deriving something from scratch:

- `wiki/concepts/ast-enum-touchpoints.md` — before adding a variant to
  `Stmt`, `Expr`, `ConstExpr`, or `ArraySize`: every file that matches it
  exhaustively, so you don't have to grep the whole crate to find out
  where a new variant needs handling.
- `wiki/index.md` and `wiki/log/` — chronological log of past changes and
  decisions; `llog search <keyword>` to query it.

### Goal

Slate-Parser is the C front end for Slate. It exists to let Slate ingest
real-world C headers and sources and eventually convert them to Rust.
The pipeline, end to end:

1. **Preprocess + parse together, polyvariantly.** Slate-Parser parses C
   like clang would, but instead of resolving `#ifdef`/macro branches
   against one fixed configuration up front, it keeps the preprocessed
   output and the parsed AST together as it goes. Conditional regions
   are preserved as `Conditional<T>` nodes rather than eagerly resolved
   (see `[[architecture_polyvariant_ast]]` in memory) so the same parse
   can later be "evaluated" against different `-D` defines/target macro
   sets without reparsing.
2. **Apply `-D` defines to get a concrete AST.** Given a set of
   command-line defines and target-specific builtin macros
   (`compiler_args.rs`), the polyvariant PP+AST is evaluated down to a
   single concrete AST for that configuration.
3. **Semantic analysis.** `sema.rs` resolves types, scopes, and
   declarations over the concrete AST, matching clang's semantics
   closely enough that output can be diffed against clang as an oracle.
4. **Lower to a Clang-IR-like bytecode.** The concrete, semantically
   analyzed AST is lowered to a small bytecode/IR modeled loosely on
   Clang IR (CIR). This is the layer where full type resolution is
   finalized and the representation is normalized into a form that's
   easy to mechanically translate.
5. **Rust conversion.** The bytecode/IR is the intended handoff point
   for Slate to generate Rust — it's deliberately kept close to Clang
   IR's shape so lowering C semantics to it (and then to Rust) doesn't
   require re-deriving type/control-flow information already resolved
   in steps 2-4.

Steps 4-5 are not fully implemented yet; `src/` currently covers
preprocessing, parsing, the polyvariant AST, and sema. Check `bd ready`
/ `bd list` for the current state of the bytecode-lowering and
Rust-conversion work.

### Testing

Testing is done via filecheck. Standard gate:

```
cargo nextest
```

The evaluated AST can additionally be checked against clang-ast as an
oracle by setting `SLATE_CLANG_ORACLE=1`:

```
SLATE_CLANG_ORACLE=1 cargo test
```

This is a debugging aid, not part of the standard gate; there is no need
for it to pass.

No unit tests should ever be added.
