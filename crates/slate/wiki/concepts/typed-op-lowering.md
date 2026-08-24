# Typed-op function-body lowering

The function-body migration is complete: lowering consumes
`clang_ir::model::Op` and its typed `Region`/`Block` tree with no runtime
dependency on generic `Operation`. The single traversal and dispatch engine
lives in `lowerer/dispatch.rs`.

## Mechanism

`clang-ir-types`'s generated `Op::for_each_result` and
`FunctionLowerer::value_types` record typed operation results and block
arguments; runtime operand-type queries go through this SSA-keyed lookup
rather than re-deriving types from raw operand strings. `cir.store` and
`cir.get_global` already dispatch through their generated `TypedStore`/
`TypedGetGlobal` structs directly rather than a raw `lower_*(&Op)` handler.

Each generated `Op` variant routes directly to its typed handler. New handlers
must accept the generated struct and use the SSA type lookup when an operand's
type is not stored directly on that struct.

Use the generated fields and the SSA type lookup as-is; don't add a
universal operand-type field to generated operations just to avoid touching
call sites — that reintroduces the untyped escape hatch this migration
exists to remove.

Validate each slice: `cargo fmt`, `cargo clippy --release --all-targets
--all-features -- -D warnings`, `cargo nextest run --release --profile
lowering --no-fail-fast`.
