# Dispatch Slot Promotion

A [goto dispatcher](../goto.md#cycles-with-multiple-entry-points) can assign a
local in one `match` arm and use it in another. When a local needs to outlive an
arm, lowering can allocate the storage outside the dispatcher as
`MaybeUninit<T>` and access it through a synthetic pointer.

## Replace the pointer with a binding

The abbreviated Rust AST below shows a 32-bit integer written on entry and
updated by a dispatcher arm. The dispatcher itself is omitted.

```rust
let mut total: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
let __slate_slot_total: *mut i32 = (&raw mut total) as *mut i32;
unsafe { core::ptr::write(__slate_slot_total, 0) };
unsafe { *__slate_slot_total += 2 };
```

`slot_promote` changes the storage to a typed local, initially zeroed. It
replaces the writes and dereferences through the synthetic pointer with direct
assignments and uses of the local. The initial C assignment still runs at its
original point. Other cleanup can simplify the two initializations where the
control flow permits it.

```rust
let mut total: i32 = 0;
total += 2;
```

The rule checks that the storage is used only by the synthetic pointer, that
zero is a valid representation for the supported type, and that the pointer uses
can be expressed as places or direct writes. Passing the synthetic pointer
itself to an opaque call blocks this rule. A separate declaration-sinking rule
can move a binding to its sole assignment when every read follows it in that
scope and none of those uses takes its address. A value shared across arms can
remain a typed local outside the dispatcher.
[Goto dispatcher fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/goto_basic_block_entries.c)
