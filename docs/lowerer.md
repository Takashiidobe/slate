# Lowerer internals

> This is the module-level map of `src/frontend/lowerer.rs` and
> `src/frontend/lowerer/`. See [architecture.md](architecture.md) for why the
> lowerer exists and what it consumes; see [passes.md](passes.md) for where
> `lower` sits in the overall pipeline. Read this before touching any `cir.*`
> handler.

## Two-tier state: `Lowerer` vs `FunctionLowerer`

Lowering runs in two nested passes over one `Lowerer<'a>`:

- **`Lowerer<'a>`** (`lowerer.rs`) holds translation-unit-wide state: collected
  globals (`globals`, `extern_globals`, `strings`, `const_arrays`), records and
  enums (`records`, `enums`), known function signatures
  (`function_return_types`, `function_param_types`, `known_functions`),
  bitfield storage layout (`bitfield_storages`), and misc feature-usage flags
  (`uses_long_double`, `uses_complex`, ...) that decide which prelude items the
  final `Program` needs. `pub fn lower_with_project` constructs one `Lowerer`
  and calls `lower_module`, which:
  1. Walks every top-level op once via `collect_global` (strings first, so
     later globals that reference them by name resolve), `collect_bitfield_storages`,
     `collect_lifecycle_hooks`, `collect_used_symbols`, and builds
     `global_sym_types` (raw CIR `sym_type` per global symbol, needed because a
     global's _initializer_ can reference another global declared later in the
     module — see the `global_view_index_path` note below).
  2. Lowers every function definition by building one `FunctionLowerer` per
     function and running it over the function body.

- **`FunctionLowerer<'a, 'b>`** (`lowerer.rs` struct, methods spread across
  most of `lowerer/*.rs`) holds `parent: &'a mut Lowerer<'b>` plus everything
  scoped to the function currently being lowered: SSA-value bindings
  (`values: BTreeMap<String, Val>`), alloca/slot tracking (`slots`,
  `slot_places`, `slot_types`), place trackers for pointer-typed SSA values
  (`member_ptrs`, `element_ptrs`), the output statement buffer (`body`), loop
  and `goto`-dispatch state (`loop_stack`, `dispatch: Option<DispatchCtx>` —
  the state-machine fallback for CFGs that don't reloop cleanly), and va_list /
  asm / macro-const bookkeeping local to the function.

Free functions outside both structs (`attr_str`, `region_ops`, `parse_cir_*`,
type-string parsing, etc.) are stateless CIR/text parsing helpers — they take
whatever `Op`/`&str`/`&Module` they need as arguments instead of reading
`self`, so they're reusable from both `Lowerer` and `FunctionLowerer` methods
and from the collection pass.

## Op dispatch

Every op inside a function body funnels through `FunctionLowerer::lower_op`
(`lowerer.rs`, ~3780), a single `match op.kind()` that fans out to one
`self.lower_xxx(op)` (or a handful of ops sharing one handler, e.g. every
libm unary op routes through `lower_unary_method`/`lower_known_unary_method`).
The parser (`src/cir/ir.rs`) never grows a new variant for a new lowering
feature — `CirOpKind` is a fixed enum keyed off the CIR op mnemonic, and
`lower_op` is the _only_ place that has to change to route a newly-recognized
op name to its handler. The handler itself almost always lives in a submodule,
picked by category (see the table below).

Global-scope constructs (globals, struct/union/enum definitions, function
signatures) don't go through `lower_op` at all — they're collected directly in
`Lowerer::collect_global` and sibling `collect_*` functions during
`lower_module`, before any function body is visited.

## Submodule map

`lowerer.rs` declares the submodules with `mod x;`. Modules whose content is
free functions (parsing/analysis helpers with no `self`) are glob-imported
(`use x::*;`) so callers can use them unqualified; modules that only add
`impl<'a, 'b> FunctionLowerer<'a, 'b>` blocks are declared but not
glob-imported, since their functions are reached as `self.method()`.

| Module               | Kind                    | Owns                                                                                                                                                                                                                                                                                                                |
| -------------------- | ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `analysis.rs`        | free fns, glob-imported | Whole-op-tree scans that run once before any function is lowered: lifecycle hooks (ctors/dtors), used-symbol collection, cross-block-liveness for the goto-dispatch fallback, enum locals that need integer storage, `attr_str`/`attr_int`-style op-attribute readers.                                              |
| `arithmetic.rs`      | `impl FunctionLowerer`  | Integer/float binary and unary arithmetic ops, overflow-checked arithmetic (`AddOverflow` etc.), shifts, rotates, `select`, constant-folding helpers for arithmetic ops.                                                                                                                                            |
| `asm.rs`             | free fns, glob-imported | Inline-asm template translation, module-level `asm()` blocks, weak-alias `.symver`-style asm, register/dialect parsing.                                                                                                                                                                                             |
| `atomic.rs`          | free fns, glob-imported | Mapping CIR atomic op/ordering attrs to `rust_ast` atomic types and `AtomicOrdering`.                                                                                                                                                                                                                               |
| `bitfields.rs`       | free fns, glob-imported | Bitfield storage-unit layout: which struct fields share a backing integer, offsets/widths within it, generating the wrapper struct items.                                                                                                                                                                           |
| `builtins.rs`        | `impl FunctionLowerer`  | Comparison (`cmp`), `ffs`/`clrsb`, `modf`, signbit, float-classification (`is_fp_class`) and other libm-adjacent builtins that don't fit `arithmetic.rs`'s pure binop/unop shape.                                                                                                                                   |
| `calls.rs`           | `impl FunctionLowerer`  | Call lowering (`lower_call`), known-libc-function special cases (`memcpy`/`memset`/`memchr` as slice ops), atomic RMW/cmpxchg/fence intrinsics, `va_arg`/`va_start`.                                                                                                                                                |
| `cir_ops.rs`         | free fns, glob-imported | Structural CIR shape helpers: switch-case pattern extraction (including Duff's-device detection), "does this region diverge", opaque single-op/region unwrapping, result-type parsing.                                                                                                                              |
| `constants.rs`       | free fns, glob-imported | Parsing CIR constant attribute text (`#cir.int<..>`, `#cir.global_view<..>`, `#cir.const_array<..>`, `#cir.ptr<N>`, float bit patterns) into `Expr`/`RustValue`; the standard-library record shim table.                                                                                                            |
| `control_flow.rs`    | `impl FunctionLowerer`  | `cir.if`/`cir.for`/`cir.while`/`cir.do`/`cir.switch`/`cir.scope`, `break`/`continue`/`goto`/`return`, and the state-machine fallback (`lower_dispatch`) for irreducible control flow.                                                                                                                               |
| `expressions.rs`     | `impl FunctionLowerer`  | `_Complex` arithmetic (add/sub/mul/div/conj) and complex-part extraction.                                                                                                                                                                                                                                           |
| `intrinsics.rs`      | `impl FunctionLowerer`  | `__builtin_*`/LLVM-intrinsic ops that aren't plain arithmetic: trap/unreachable, `__builtin_object_size`, vector shuffle/extract/insert, x86 target intrinsics, extended asm, setjmp.                                                                                                                               |
| `memory.rs`          | `impl FunctionLowerer`  | Everything that computes or dereferences a place: `cir.get_member`/`get_element`/`extract_member`/`insert_member`, casts, pointer-difference/stride, bitfield get/set, and the `place_expr`/`place_or_deref_expr` machinery every other handler calls to turn a CIR SSA pointer value into a Rust place expression. |
| `op_utils.rs`        | free fns, glob-imported | Low-level `Op` attribute accessors (`attr_symbol_ref`, `attr_int`, `attr_bool`, successor-operand parsing, aggregate member index) used throughout every other submodule.                                                                                                                                           |
| `runtime_support.rs` | free fns, glob-imported | The generated `f80`/long-double shim: trait impls, extern decls, cast helpers, emitted once per program when `uses_long_double` is set.                                                                                                                                                                             |
| `storage.rs`         | `impl FunctionLowerer`  | `cir.alloca`/`store`/`load`/`copy`/`const`, local-slot naming and hoisting, and — notably — `render_const_value_expr`-adjacent global-initializer materialization shared with `Lowerer` (macro consts, layout queries, enum consts pulled from the fact queues).                                                    |
| `types.rs`           | free fns, glob-imported | CIR type-string parsing into `rust_ast::Type` (`rust_type`, `rust_type_with_aliases`), `_BitInt` support, va_list detection, anonymous-record synthesis (`anon_local_records`, exported for use outside the lowerer).                                                                                               |
| `values.rs`          | `impl FunctionLowerer`  | Temp/statement emission plumbing every handler uses: `next_temp`, `push_stmt`/`push_assign`, `materialize_expr`, `unsafe_expr`/`unsafe_stmt` wrapping, function-pointer null/byte-representation handling.                                                                                                          |

`lowerer.rs` itself keeps: the `Lowerer`/`FunctionLowerer`/`DispatchCtx`/
`LoopFrame`/`MemberPtr`/`ElementPtr` struct definitions, `lower_module`,
`lower_op`'s dispatch match, global collection (`collect_global` and its
`#cir.*`-initializer branches), record/enum/function-signature setup, and a
handful of helpers not (yet) worth splitting out — e.g. `global_view_init_expr`
/ `global_view_index_path` (constant address-of-global expressions, including
indexed sub-object views: `&arr[i]`, `&record.field`, chained through nested
arrays/records/unions) and `record_field_offset` (byte offset of a named field,
used by fixups that need struct layout math).

## Constant global-initializer resolution (`#cir.global_view`)

Worth calling out on its own because it's a recurring source of gcc-torture
bugs (see [gcc-torture-triage.md](gcc-torture-triage.md)): CIR represents a
constant address-of-global expression — `&g`, `&arr[3]`, `&s.field`, or a chain
of both — as `#cir.global_view<@symbol, [i0, i1, ...]>`. The indices are a
**type-directed path** into `symbol`'s own declared type: each index selects an
array element or the Nth positional record member (for a union, member offset
is always 0, since union members overlay). `constants::parse_cir_global_view`
extracts just `symbol`; `constants::parse_cir_global_view_indices` extracts the
index list. `Lowerer::global_view_index_path` walks those indices against
`global_sym_types[symbol]` (not `self.globals[symbol].ty`, since the
_referencing_ global can be collected before the _referenced_ one) to build a
`Field`/`Index` chain rooted at a raw-pointer deref of the target — needed
because indexing/field access through wrapper types like `aligned::Aligned`
would otherwise call a non-`const` `Deref`/`DerefMut` impl, which `static`
initializers reject.

## Adding a new `cir.*` handler

1. Add or extend a C fixture reproducing the missing op (see
   [writing-a-fixup.md](writing-a-fixup.md) for fixture conventions; the same
   fixture-first rule applies to lowering features).
2. Confirm it's a _lowering_ gap, not a fixup gap: `cargo run -- translate-lowered <file.c>`
   shows raw lowering output with no fixup passes applied.
3. Find the op's CIR name with `cargo run -- emit-cir <file.c>` (or `SLATE_CLANG`'s
   `cir-opt --mlir-print-op-generic` directly for the exact generic-form text
   the parser sees).
4. If `CirOpKind` doesn't have a variant for it yet, add one in `src/cir/ir.rs`.
5. Add the `CirOpKind::X => self.lower_x(op)` arm in `FunctionLowerer::lower_op`
   (`lowerer.rs`).
6. Implement `lower_x` in whichever submodule matches the table above (new
   arithmetic-shaped op -> `arithmetic.rs`; new place/pointer op -> `memory.rs`;
   new control-flow op -> `control_flow.rs`; etc). Reuse `place_expr`/
   `materialize_expr`/`push_stmt` from `memory.rs`/`values.rs` rather than
   building `Expr`/`Stmt` nodes by hand where those helpers already cover the
   shape.
7. `cargo fmt`, then run the fixture through the relevant `cargo nextest r --release --profile lowering`
   differential test.
