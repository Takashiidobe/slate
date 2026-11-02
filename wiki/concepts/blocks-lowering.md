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
  (a `*mut Byref<T>` for `by_reference`, the value otherwise);
- an `extern "C-unwind" fn __slate_block_N(__block, params..)` item lowered
  by a child `FunctionLowerer` (it inherits the parent's binding numbering so
  captured names agree). Its prologue reads the capture struct into locals
  named like the originals;
- a `static __slate_block_N_descriptor` from `__slate_blocks::descriptor`,
  listing each by-value capture of block pointer type with `block_field` and
  each `by_reference` capture with `byref_field`;
- a `let mut __slate_block_N_slot: MaybeUninit<Block<..>>` declared right
  before the statement that contains the literal (`FunctionLowerer::pending`,
  emitted by `lower_statement`; nested statements save and restore it), so
  it lives until the end of the enclosing C block. In a goto-dispatch
  function there are no lexical scopes, so the slot is hoisted to the
  function top instead;
- an expression `__slate_blocks::stack(&raw mut slot, __slate_block_N as
  *const c_void, &descriptor, __slate_block_N_captures { .. })`.

Captures are taken by value at creation, matching C. A literal with no
captures becomes a global block (below), as clang emits it.

## Calls

A call whose indirect callee has block pointer type evaluates the block once
into a temp, transmutes `__slate_blocks::invoke(temp)` to an
`extern "C-unwind" fn(*mut c_void, params..) -> ret`, and calls it with the
block as the first argument. A variadic block type adds `...` to that
pointer type and promotes the variadic arguments like any variadic call
(`lower_call_argument`); a variadic literal's function is an
`unsafe extern "C-unwind" fn(__block, fixed.., mut __va_args: ...)`. The
parser checks `va_start` against the block's own signature, not the
enclosing function's.

## __block variables

`ir::Variable::byref` (printed `[byref]`) marks an automatic `__block`
variable. It is a runtime `Block_byref`: `Byref<T>` is the header (`isa`
null, `forwarding`, `flags` 0, `size`) followed by the value, and every
access is `*__slate_blocks::value(x)`, which goes through `forwarding`.
C never needs byref keep/destroy helpers (the runtime ignores
`BLOCK_BYREF_CALLER` block fields), so the runtime copies the value as bits.
`Block_copy` of a block that captures `x` moves it to the heap with refcount
2 (the copy and the declaring scope) and patches the stack `forwarding`, so
`&x` changes after a copy exactly as in C. `FunctionLowerer::place_is_unsafe`
treats places rooted in such a binding as unsafe.

- Structured functions: `lower_byref_let` emits
  `let mut x_byref: MaybeUninit<Byref<T>>` (pending, before the statement),
  `let x: *mut Byref<T> = byref(&raw mut x_byref, init)`, then
  `let x_scope = scope(x)`, whose `Drop` calls
  `_Block_object_dispose(x, BLOCK_FIELD_IS_BYREF)` at the end of the C scope.
  `dead_store` keeps `__slate_blocks::scope` lets like other drop guards.
- Goto-dispatch functions: `x` is a `ByrefSlot<T>` at the function top (the
  storage plus a `live` flag) and is reached through `slot(&raw mut x)`. The
  declaration becomes `init(&raw mut x, value)`, which releases the previous
  incarnation first when a backward goto re-runs it; the slot's `Drop`
  releases at function exit. Leaving the scope forward without returning
  releases at function exit instead of scope exit, which only delays freeing
  the heap copy. Jumping past a `__block` declaration is a clang error.

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
no heap growth for non-escaping literals), `block_byref.c` (forwarding,
shared and escaping `__block` variables, per-iteration variables, no heap
growth), `block_goto.c` (`__block` under goto, including a backward goto
that re-declares a copied variable) and the two-unit
`tests/fixtures.release/block_units`. Fixtures declare
`_Block_copy`/`_Block_release` themselves instead of including `<Block.h>`.
`tests/fixtures.library/block_capture_retain` has a native C driver pass a
stack block to a translated library that captures it in a copied block; it
crashed before captured blocks were retained. `block_variadic.c` and
`tests/fixtures.library/block_variadic` cover variadic blocks, the latter
calling translated variadic blocks from native C and native ones from
translated code.

## Not yet lowered

- Referencing a `__block` variable inside its own initializer.
- `__block` on MSVC targets, blocks in aggregates inside
  non-constant global initializers.
