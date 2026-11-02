# Blocks lowering

<!-- toc -->
- [Representation](#representation)
- [Block literals](#block-literals)
- [Calls](#calls)
- [\_\_block variables](#__block-variables)
- [Global blocks](#global-blocks)
- [Runtime](#runtime)
- [Testing](#testing)
- [Not yet lowered](#not-yet-lowered)
<!-- /toc -->

How slate lowers clang `-fblocks` IR to Rust. The parser side is in
[ir/control-flow](ir/control-flow.md#block-literals). Tracked under
`slate-3amj.42`. There is no `block2`/`objc2` dependency: that route needs
`Encode` impls for every struct in a block signature and a cargo dependency
the single-file `rustc` harness cannot resolve. Slate writes the Blocks ABI
itself.

## Representation

- A block pointer type lowers to `*mut c_void`.
- `frontend/support/blocks.rs` (module `__slate_blocks`, emitted only when a
  block type appears) defines `Block<C>`, a `#[repr(C)]` header (`isa`,
  `flags`, `reserved`, `invoke`, `descriptor`) followed by a capture struct
  `C`. `new` mallocs a block with `_NSConcreteMallocBlock`,
  `BLOCK_NEEDS_FREE` and refcount 1; `invoke` and `captures` read it back.
- Blocks never get copy/dispose helpers. Captures are plain bits, a heap
  block is never a stack block, and `Block_copy`/`Block_release` only touch
  the refcount. Nothing frees the creating reference, so a created block
  leaks.

## Block literals

`lowerer/blocks.rs`. Each `ValueKind::Block` becomes:

- a `#[repr(C)] struct __slate_block_N_captures` with one field per capture
  (the pointer to the shared cell for `by_reference`, the value otherwise);
- an `extern "C-unwind" fn __slate_block_N(__block, params..)` item lowered
  by a child `FunctionLowerer` (it inherits the parent's binding numbering so
  captured names agree). Its prologue reads the capture struct into locals
  named like the originals;
- an expression `__slate_blocks::new(__slate_block_N as *const c_void,
  __slate_block_N_captures { .. })`.

Captures are taken by value at creation, matching C.

## Calls

A call whose indirect callee has block pointer type evaluates the block once
into a temp, transmutes `__slate_blocks::invoke(temp)` to an
`extern "C-unwind" fn(*mut c_void, params..) -> ret`, and calls it with the
block as the first argument. Variadic block types are not lowered.

## __block variables

`ir::Variable::byref` (printed `[byref]`) marks an automatic `__block`
variable. The backend declares it as `*mut T` from `__slate_blocks::cell`
(leaked `Box`) and `lower_binding` dereferences it everywhere, so blocks,
nested blocks and the declaring function share one cell, including after the
declaring frame returns. `FunctionLowerer::place_is_unsafe` treats places
rooted in such a binding as unsafe.

## Global blocks

A block literal with no captures is a constant initializer. In a static
initializer it becomes `static __slate_block_N_literal: Block<()> =
__slate_blocks::global(..)` (isa `_NSConcreteGlobalBlock`, `BLOCK_IS_GLOBAL`)
and the value is its address.

## Runtime

Non-Apple targets link compiler-rt's BlocksRuntime through the
`slate-blocks` crate (`crates/slate/vendor/slate-blocks`; `build.rs` compiles
`runtime.c`/`data.c`). `translate-project` writes the crate and the manifest
dependency only when a unit emitted `__slate_blocks`; the support module has
`use slate_blocks as _` so rustc links it. Darwin uses libSystem. MSVC
targets are skipped. The differential harness adds the same two C files to
the oracle compile when `dg-options` has `-fblocks`.

## Testing

`tests/fixtures/block_basic.c`, `block_more.c`, `block_globals.c` and the
two-unit `tests/fixtures.release/block_units`. Fixtures declare
`_Block_copy`/`_Block_release` themselves instead of including `<Block.h>`.

## Not yet lowered

- Captured blocks are not retained, so a stack block from C captured by an
  escaping block dangles.
- Referencing a `__block` variable inside its own initializer.
- Variadic blocks, `__block` on MSVC targets, blocks in aggregates inside
  non-constant global initializers.
