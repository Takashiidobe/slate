# Typed Op lowering migration

Active bead: `slate-cevu.6`.

Goal: lower function bodies from `clang_ir::model::Op` and its typed
`Region`/`Block` tree, with no runtime dependency on generic `Operation`.

## Current state

- `clang-ir-types 0.1.4` provides generated `Op::for_each_result`.
- `FunctionLowerer::value_types` records typed operation results and block
  arguments. Runtime operand-type queries now use this SSA lookup.
- `cir.store` and `cir.get_global` already use `TypedStore` and
  `TypedGetGlobal` directly.
- 88 generated dispatch arms still call a raw `lower_*(&Op)` handler.
- 58 `CirOpKind` fallback arms remain. Two `operand_types` reads are only
  module-level analysis/bitfield collection, not runtime lowering.

## Next work

1. Convert storage next: `cir.const`, `cir.load`, and `cir.copy`.
   `const` needs its macro/enum source-location helpers changed to accept a
   typed `SourceLocation` instead of `&Op`.
2. Convert handlers one op family at a time to accept generated structs and
   route the matching `TypedOp` arm directly to them.
3. Replace raw `lower_block`/`lower_region_ops` traversal with typed
   `model::Block`/`model::Region` once their invoked families are converted.
4. Remove a `CirOpKind` fallback only after its typed replacement passes.

Use generated fields and the SSA type lookup; do not add universal operand
type fields to generated operations.

Validate each slice:

```bash
cargo fmt
cargo clippy --release --all-targets --all-features -- -D warnings
cargo nextest run --release --profile lowering --no-fail-fast
```
