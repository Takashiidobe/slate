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
  `C`. `stack` writes a block with `_NSConcreteStackBlock` into a caller
  slot; `invoke` and `captures` read it back.
- `Descriptor` is the runtime's `Block_descriptor_1` plus copy/dispose
  pointers, followed by slate-only fields the runtime never reads:
  `captures_offset` and `fields`, a list of `(offset, BLOCK_FIELD_IS_*)`
  pairs. The shared `copy_helper`/`dispose_helper` walk that list through the
  block's own descriptor and call `_Block_object_assign`/
  `_Block_object_dispose`, so no helper is generated per literal. `stack`
  sets `BLOCK_HAS_COPY_DISPOSE` only when the list is non-empty.
- `Block_copy` of a stack block mallocs a copy and calls the copy helper,
  which `Block_copy`s every captured block pointer. `Block_release` of the
  last reference calls the dispose helper, which releases them. Escaping a
  block without `Block_copy` is undefined behavior, as in C.

## Block literals

`lowerer/blocks.rs`. Each `ValueKind::Block` becomes:

- a `#[repr(C)] struct __slate_block_N_captures` with one field per capture
  (the pointer to the shared cell for `by_reference`, the value otherwise);
- an `extern "C-unwind" fn __slate_block_N(__block, params..)` item lowered
  by a child `FunctionLowerer` (it inherits the parent's binding numbering so
  captured names agree). Its prologue reads the capture struct into locals
  named like the originals;
- a `static __slate_block_N_descriptor` from `__slate_blocks::descriptor`,
  listing each by-value capture of block pointer type with `block_field`;
- a `let mut __slate_block_N_slot: MaybeUninit<Block<..>>` hoisted to the
  top of the enclosing function (or block function). Its lifetime is the
  whole function, a superset of the C scope; a literal in a loop reuses the
  slot each iteration, as clang does;
- an expression `__slate_blocks::stack(&raw mut slot, __slate_block_N as
  *const c_void, &descriptor, __slate_block_N_captures { .. })`.

Captures are taken by value at creation, matching C. A literal with no
captures becomes a global block (below), as clang emits it.

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
rooted in such a binding as unsafe. Blocks capture the cell pointer as plain
bits and the copy helper does not touch it, so cells leak, and `&x` never
moves on `Block_copy` the way C's byref forwarding does.

## Global blocks

A block literal with no captures, in a function or a static initializer,
becomes `static __slate_block_N_literal: Block<()> =
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

`tests/fixtures/block_basic.c`, `block_more.c`, `block_globals.c`,
`block_lifetime.c` (copy identity, a captured block outliving its release,
no heap growth for non-escaping literals) and the two-unit
`tests/fixtures.release/block_units`. Fixtures declare
`_Block_copy`/`_Block_release` themselves instead of including `<Block.h>`.
`tests/fixtures.library/block_capture_retain` has a native C driver pass a
stack block to a translated library that captures it in a copied block; it
crashed before captured blocks were retained.

## Not yet lowered

- `__block` variables as runtime `Block_byref` structs (forwarding, refcount,
  stack until copied, `BLOCK_FIELD_IS_BYREF` in `fields`).
- Referencing a `__block` variable inside its own initializer.
- Variadic blocks, `__block` on MSVC targets, blocks in aggregates inside
  non-constant global initializers.
