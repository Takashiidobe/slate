# Slate-Parser

Workspace rules (beads, testing from the root, session completion, commits)
live in [the root AGENTS.md](../../AGENTS.md).

## Wiki

Non-obvious project context lives in the workspace-root `wiki/`. Check it before re-deriving
something from scratch. Every concept page opens with a table of contents,
so jump straight to the heading you need. After adding or renaming headings,
run `python3 crates/slate-parser/tools/wiki_toc.py` (`--check` reports stale pages).

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

## Goal

Slate-Parser is Slate's C front end. It lets Slate ingest real-world C
headers and sources and convert them to Rust. The pipeline, end to end:

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
3. **Semantic analysis.** `src/sema/` resolves types, scopes, and
   declarations over the AST, matching clang's semantics
   closely enough that output can be diffed against clang as an oracle.
4. **Lower to a typed IR.** `Sema::lower` (`src/sema/module.rs`) produces
   an `ir::Module` (`src/ir/`), modeled loosely on Clang IR (CIR). Types
   are fully resolved, places and bindings are explicit, conversions and
   arithmetic semantics are spelled out, and side effects are hoisted
   into sequenced statements, so the IR is easy to translate
   mechanically. [ir-spec](../../wiki/concepts/ir-spec.md) records what is
   implemented.
5. **Rust conversion.** `crates/slate` consumes `ir::Module` directly as a
   library (`crates/slate/src/slate_parser_frontend/`) and lowers it to
   Rust, so it never re-derives type or control-flow facts resolved in
   steps 2-4. That lowering is being built out under the `slate-p58o`
   epic.

When Slate lowering exposes a missing or wrong IR fact, fix it here rather
than working around it in `crates/slate`.

## Coding

Never use `assert!` and friends. Everything should be an explicit Result
type, using `thiserror`. If you see a stray `assert!` or `assert_eq!`,
think about refactoring it.

## Testing

Oracle compilers: `clang` and `gcc` are installed natively; MSVC is `crates/slate-parser/tools/cl.exe`.

A fixture's directory sets its compiler and target
(`crates/slate-parser/tests/fixtures/<flavor>/<os>/<arch>/`, with `error/` and `suites/`
variants); see [fixture-layout](../../wiki/concepts/fixture-layout.md).

FileCheck expectations are generated. After changing a fixture or its
renderer, run `python3 crates/slate-parser/tools/update_filecheck.py --in-place <fixture>`;
do not write `CHECK` lines by hand.

Testing is done via filecheck. Standard gate (use release to run tests
~10x faster):

```
cargo nextest run --release --profile parser --no-fail-fast
```

The AST can additionally be checked against clang-ast as an
oracle by setting `SLATE_CLANG_ORACLE=1`:

```
SLATE_CLANG_ORACLE=1 cargo nextest run --release --profile parser
```

This is a debugging aid, not part of the standard gate; there is no need
for it to pass.

No unit tests should **ever** be added.
