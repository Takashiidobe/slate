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

Non-obvious project context lives in `wiki/`. Check it before re-deriving
something from scratch. Every concept page opens with a table of contents,
so jump straight to the heading you need. After adding or renaming headings,
run `python3 tools/wiki_toc.py` (`--check` reports stale pages).

Start here:

- [architecture](../../wiki/concepts/parser-architecture.md): read first. Checker vs
  lowering, what belongs in the IR, strictness policy, priorities, oracles.

Specs and grammars (update in the same change as the code they describe):

- [ast-spec](../../wiki/concepts/ast-spec.md): what the AST means. Update with
  any AST change.
- [ast-grammar](../../wiki/concepts/ast-grammar.md): EBNF of `slate-parser parse`
  output, from `src/ast.rs` and `src/const_expr.rs`.
- [ir-spec](../../wiki/concepts/ir-spec.md): what the IR means and what is
  implemented; a map of the `ir/` subpages below. Update the matching
  subpage with any IR change.
  - [ir/pipeline](../../wiki/concepts/ir/pipeline.md): failure kinds, node ids
    and metadata, dump commands, required constant folding, reachability.
  - [ir/types](../../wiki/concepts/ir/types.md): scalar formats, literals,
    `_BitInt`, tags, enums, record layout (pack, `ms_struct`, Microsoft).
  - [ir/type-families](../../wiki/concepts/ir/type-families.md): complex,
    imaginary, vector, fixed-point.
  - [ir/operations](../../wiki/concepts/ir/operations.md): UB and rounding
    policies, floating pragmas, conversions, `?:`.
  - [ir/places-pointers](../../wiki/concepts/ir/places-pointers.md): places,
    bit-fields, pointer arithmetic and conversions, qualified access.
  - [ir/atomics](../../wiki/concepts/ir/atomics.md): `_Atomic` layout and ABI per
    flavor, atomic builtins.
  - [ir/control-flow](../../wiki/concepts/ir/control-flow.md): structured
    control flow, side-effect hoisting, unsequenced order, pragmas.
  - [ir/declarations](../../wiki/concepts/ir/declarations.md): linkage,
    redeclaration conflicts, symbol and object attributes, inline.
  - [ir/initialization](../../wiki/concepts/ir/initialization.md): aggregate
    initializers, compound literals, flexible arrays, VLAs.
  - [ir/calls-abi](../../wiki/concepts/ir/calls-abi.md): call signatures,
    `AbiSignature` and `native_c`, calling conventions, targets.
  - [ir/builtins](../../wiki/concepts/ir/builtins.md): builtin registry, implicit
    declarations, custom lowering, `va_list`.
  - [ir/asm](../../wiki/concepts/ir/asm.md): GNU and MSVC inline asm, operand
    selection, Rust `asm!` options.
  - [ir/open-design](../../wiki/concepts/ir/open-design.md): agreed but
    unimplemented shapes and open questions.
- [ir-grammar](../../wiki/concepts/ir-grammar.md): EBNF of the printed IR, from
  the printers in `src/ir/`. Update with anything that changes IR output.

Before changing these, read:

- [ast-enum-touchpoints](../../wiki/concepts/ast-enum-touchpoints.md): adding a
  variant to `Stmt`, `Expr`, `ConstExpr`, or `ArraySize`.
- [type-family-touchpoints](../../wiki/concepts/type-family-touchpoints.md):
  adding a type family or an `ir::Type`/`CTypeKind` variant, including the
  match sites that accept a new type silently.
- [configuration-threading](../../wiki/concepts/configuration-threading.md):
  anything that differs per flavor, standard, or target; how `Dialect`
  reaches each stage.
- [compiler-arg-rules](../../wiki/concepts/compiler-arg-rules.md): adding a
  compiler argument.
- [compiler-flags](../../wiki/concepts/compiler-flags.md): which flags are
  emulated and their resolved effect.
- [adding-a-target](../../wiki/concepts/adding-a-target.md): wiring a new triple
  through `TargetSpec`.
- [diagnostic-severity](../../wiki/concepts/diagnostic-severity.md): named
  warnings, default severities, `-W` handling.

Subsystems:

- [attributes](../../wiki/concepts/attributes.md): attribute pipeline from
  parsing and registration to applicability and IR consumers.
- [sema-passes](../../wiki/concepts/sema-passes.md): pass order, which
  `src/sema/` file belongs to which pass, `ResolveError` policy.
- [c-type-layer](../../wiki/concepts/c-type-layer.md): interned C types in
  `src/sema/ctype/` and how they erase to `ir::Type`.
- [entity-model](../../wiki/concepts/entity-model.md): per-`BindingId` declared
  entities merged across redeclarations.
- [preprocessor](../../wiki/concepts/preprocessor.md): predefine seeding,
  includes, expansion, token and macro provenance.
- [msvc-asm](../../wiki/concepts/msvc-asm.md): parsing MSVC `__asm` and inferring
  its reads, writes, and clobbers.
- [generated-sources](../../wiki/concepts/generated-sources.md): generated and
  captured files, their generators, and when to rerun them.

Testing:

- [fixture-layout](../../wiki/concepts/fixture-layout.md): how a fixture's
  directory sets its flavor and target.
- [msvc-oracle](../../wiki/concepts/msvc-oracle.md): running `cl.exe` under Wine.
- [c-corpus](../../wiki/concepts/c-corpus.md): real-world projects in
  `~/c-corpus`, per-flavor compile databases, the corpus sweep.

History: `wiki/index.md` and `wiki/log/` hold the chronological log of
changes and decisions; query it with `llog search <keyword>`.

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

Oracle compilers: `clang` and `gcc` are installed natively; MSVC is `tools/cl.exe`.

A fixture's directory sets its compiler and target
(`tests/fixtures/<flavor>/<os>/<arch>/`, with `error/` and `suites/`
variants); see [fixture-layout](../../wiki/concepts/fixture-layout.md).

FileCheck expectations are generated. After changing a fixture or its
renderer, run `python3 tools/update_filecheck.py --in-place <fixture>`;
do not write `CHECK` lines by hand.

Testing is done via filecheck. Standard gate (use release to run tests
~10x faster):

```
cargo nextest run --release --no-fail-fast
```

The AST can additionally be checked against clang-ast as an
oracle by setting `SLATE_CLANG_ORACLE=1`:

```
SLATE_CLANG_ORACLE=1 cargo test
```

This is a debugging aid, not part of the standard gate; there is no need
for it to pass.

No unit tests should **ever** be added.
