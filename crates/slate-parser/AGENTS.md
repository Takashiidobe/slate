# Slate Parser

Workspace workflow, test profiles, and routing are in the
[root instructions](../../AGENTS.md).

## Scope

This crate preprocesses, parses, analyzes, and lowers C into typed IR for one
configuration (flavor, standard, target). Fix missing or incorrect IR facts
here; `crates/slate` must not work around them.

```
src/pp, lexer.rs -> src/parser (ast.rs) -> src/sema (ctype/, passes) -> src/ir
```

Read [parser-architecture](../../wiki/concepts/parser-architecture.md) first:
checker vs lowering, what belongs in the IR, strictness policy.

## Read before changing

`llog search` ranks concept pages above logs and `wiki/historical/` pages
last; historical pages are superseded, so do not follow them.

Specs and grammars are updated in the same change as the code they describe.

| Change | Read |
| --- | --- |
| Any IR change | [ir-spec](../../wiki/concepts/ir-spec.md) and the matching `ir/` subpage; update it, and [ir-grammar](../../wiki/concepts/ir-grammar.md) if printed output changes |
| Any AST change | [ast-spec](../../wiki/concepts/ast-spec.md), [ast-grammar](../../wiki/concepts/ast-grammar.md) |
| New `Stmt`, `Expr`, `ConstExpr`, `ArraySize` variant | [ast-enum-touchpoints](../../wiki/concepts/ast-enum-touchpoints.md) |
| New type family, `ir::Type` or `CTypeKind` variant | [type-family-touchpoints](../../wiki/concepts/type-family-touchpoints.md): lists the match sites that accept a new type silently |
| Anything that differs per flavor, standard, or target | [configuration-threading](../../wiki/concepts/configuration-threading.md) |
| New compiler argument | [compiler-arg-rules](../../wiki/concepts/compiler-arg-rules.md), [compiler-flags](../../wiki/concepts/compiler-flags.md) |
| New target triple | [adding-a-target](../../wiki/concepts/adding-a-target.md) |
| Warnings and `-W` | [diagnostic-severity](../../wiki/concepts/diagnostic-severity.md) |
| Sema pass order, C types, entities | [sema-passes](../../wiki/concepts/sema-passes.md), [c-type-layer](../../wiki/concepts/c-type-layer.md), [entity-model](../../wiki/concepts/entity-model.md) |
| Preprocessor, attributes, asm | [preprocessor](../../wiki/concepts/preprocessor.md), [attributes](../../wiki/concepts/attributes.md), [ir/asm](../../wiki/concepts/ir/asm.md), [msvc-asm](../../wiki/concepts/msvc-asm.md) |
| Generated or captured sources | [generated-sources](../../wiki/concepts/generated-sources.md) |
| Fixtures, MSVC oracle, corpus | [fixture-layout](../../wiki/concepts/fixture-layout.md), [msvc-oracle](../../wiki/concepts/msvc-oracle.md), [c-corpus](../../wiki/concepts/c-corpus.md) |

After adding or renaming wiki headings, run
`python3 crates/slate-parser/tools/wiki_toc.py` (`--check` reports stale pages).

## Cross-crate changes

Land the IR change, its spec update, and its parser fixtures here first, then
change the lowering in `crates/slate`, then run both crates' profiles.

## Code and fixtures

Report failures through `Result` and `thiserror`; never use `assert!` and
friends in parser paths, and refactor any you find. Never add unit tests: test
with FileCheck fixtures.

A fixture's directory sets its flavor and target; see
[fixture-layout](../../wiki/concepts/fixture-layout.md). FileCheck output is
generated. Add the `SLATE-FILECHECK-*` directives first (error fixtures need
`// SLATE-FILECHECK-ERROR PARSE`; IR fixtures need `DEFINES` and `ARGS`); the
generator silently skips a fixture without them. Then run:

```bash
python3 crates/slate-parser/tools/update_filecheck.py --in-place <fixture>
```

Never write `CHECK` lines by hand. Oracles: native `clang` and `gcc`, and
`crates/slate-parser/tools/cl.exe` for MSVC.

`SLATE_CLANG_ORACLE=1` also compares the AST with clang. It is a debugging aid,
not part of the gate.
