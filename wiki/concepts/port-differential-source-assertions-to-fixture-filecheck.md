# Port differential source assertions to fixture FileCheck

_created 2026-08-24_

Generated-Rust shape expectations belong beside their C fixture. Use the raw
snapshot to understand the lowering, then encode only the intended rewrite or
lowering property rather than copying the old `contains` assertions.

Global and function-scoped `DAG`/`NOT` assertions run independently, making
item and statement order irrelevant. A function block is bounded by its
column-zero closing brace. Plain checks inside that block form an ordered
FileCheck sequence; reserve them for behavior such as close-before-remove.

Profile prefixes keep raw lowering expectations separate from rewrite output.
Cross-target checks combine the profile and target in one prefix. Special ABI
projects are target-check cases, not platform fixture flavors.
Rust harness tests remain appropriate for timing, diagnostics, target setup,
and `fixup-debug` output because generated-source FileCheck cannot replace
those assertions.
