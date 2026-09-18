# Instructions for AI Agents

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
2. **Run quality gates** (if rust changed) - `clippy, fmt`
   - run `cargo clippy --allow-dirty --fix` to fix what can be fixed
     first.
   - afterwards, run `cargo fmt` to clean up code
   - run `cargo nextest run` to run tests afterwards
3. **Update issue status** - Close finished work, update in-progress items
4. **Every change must have a corresponding log**: - create a new log
   with `llog new` for every change made, summarizing the change, no
   more than 30 lines.
5. **Git commit**: run `git commit -m ...` with a one line message
   summarizing the task.
6. **Hand off** - Summarize changes, validation, issue status, and commit.

**Critical rules:**

- Explicit user or orchestrator instructions override this Beads block.
- Do not push without clear authority from the user.
- If a required sync or push is blocked, stop and report the exact command and error.

## Slate-Parser

### Wiki

Non-obvious project context lives in `wiki/` (see `llog`). Check it before
re-deriving something from scratch:

- `wiki/concepts/ast-spec.md` and `wiki/concepts/ir-spec.md` — evergreen
  specs of what the AST means and what it lowers into. Update them in the
  same change as any AST or IR change.
- `wiki/concepts/ir-grammar.md` — EBNF of the printed IR, derived from
  the printers in `src/ir/`. Update it in the same change as anything that
  alters what the IR printer emits.
- `wiki/concepts/ast-grammar.md` — EBNF of the printed AST, derived from
  the types in `src/ast.rs` and `src/const_expr.rs`. Update it in the same
  change as any AST type change or anything that alters what
  `slate-parser parse` prints.
- `wiki/concepts/ast-enum-touchpoints.md` — before adding a variant to
  `Stmt`, `Expr`, `ConstExpr`, or `ArraySize`: every file that matches it
  exhaustively, so you don't have to grep the whole crate to find out
  where a new variant needs handling.
- `wiki/index.md` and `wiki/log/` — chronological log of past changes and
  decisions; `llog search <keyword>` to query it.
- If working on adding a compiler arg, refer to
  [`wiki/concepts/compiler-arg-rules.md`](wiki/concepts/compiler-arg-rules.md).

### Goal

Slate-Parser is to be the new C front end for Slate. It exists to let Slate ingest
real-world C headers and sources and eventually convert them to Rust.
The pipeline, end to end:

1. **Preprocess for one configuration.** Like a normal compiler,
   command-line `-D` defines and target predefines become real macros
   and `#if` selects a single branch. There are no conditional AST
   nodes; cross-platform output means running once per target. Tokens
   keep provenance to the outermost header they came from (the header
   the main file included directly), which is what lets Slate recognize
   and idiomize libc declarations.
2. **Parse.** The preprocessed token stream is parsed into one AST.
   Owning the parser also leaves room for GNU/Clang/MSVC "personalities"
   where compilers give the same C different meanings.
3. **Semantic analysis.** `sema.rs` resolves types, scopes, and
   declarations over the AST, matching clang's semantics
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
preprocessing, parsing, and sema. Check `bd ready`
/ `bd list` for the current state of the bytecode-lowering and
Rust-conversion work.

### Coding

Never use `assert!` and friends. Everything should be an explicit Result
type, using `thiserror`. If you see a stray `assert!` or `assert_eq!`,
think about refactoring it.

### Testing

FileCheck expectations are generated. After changing a fixture or its
renderer, run `python3 tools/update_filecheck.py --in-place <fixture>`;
do not write `CHECK` lines by hand.

Testing is done via filecheck. Standard gate (use release to run tests
~10x faster):

```
cargo nextest run --release
```

The AST can additionally be checked against clang-ast as an
oracle by setting `SLATE_CLANG_ORACLE=1`:

```
SLATE_CLANG_ORACLE=1 cargo test
```

This is a debugging aid, not part of the standard gate; there is no need
for it to pass.

No unit tests should **ever** be added.
