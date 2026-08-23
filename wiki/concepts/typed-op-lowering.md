# Typed-op function-body lowering

Tracked by `slate-cevu.6`, the deepest phase of the broader clang-ir typed
migration (see [clang-ir-typed-migration.md](clang-ir-typed-migration.md)).
Goal: lower function bodies entirely from `clang_ir::model::Op` and its typed
`Region`/`Block` tree, with **no runtime dependency on generic `Operation`**
— today's `lower_block`/`lower_region_ops` still walk the untyped tree.

## Mechanism

`clang-ir-types`'s generated `Op::for_each_result` and
`FunctionLowerer::value_types` record typed operation results and block
arguments; runtime operand-type queries go through this SSA-keyed lookup
rather than re-deriving types from raw operand strings. `cir.store` and
`cir.get_global` already dispatch through their generated `TypedStore`/
`TypedGetGlobal` structs directly rather than a raw `lower_*(&Op)` handler.

Convert one op family at a time: change the handler to accept the generated
typed struct, route the matching `TypedOp` arm directly to it, and only
remove the corresponding `CirOpKind` fallback arm once the typed replacement
passes fixtures. `cir.const`/`cir.load`/`cir.copy` are next in line — `const`
additionally needs its macro/enum source-location helpers changed to accept
a typed `SourceLocation` instead of `&Op`.

Use the generated fields and the SSA type lookup as-is; don't add a
universal operand-type field to generated operations just to avoid touching
call sites — that reintroduces the untyped escape hatch this migration
exists to remove.

Once the invoked families for a traversal path are fully converted, replace
`lower_block`/`lower_region_ops`'s raw traversal with typed
`model::Block`/`model::Region` for that path.

Validate each slice: `cargo fmt`, `cargo clippy --release --all-targets
--all-features -- -D warnings`, `cargo nextest run --release --profile
lowering --no-fail-fast`.
