# Control Flow and Iteration

Lowering uses Rust blocks, loops, and labels to express exits from C control
flow. A Rust rule can simplify the shape once it can see all the uses of a label
or the way a loop uses its index.

## Remove a redundant exit

**Before**

```rust
'done: {
    work();
    break 'done;
}
finish();
```

**After**

```rust
{
    work();
}
finish();
```

The last break has the same destination as falling out of the block. Once the
break is gone, an unused label can disappear too. An earlier break that skips
work still needs its target. A block may remain to preserve the lifetimes of its
bindings.

## Simplify a loop

Rules in `for_range.rs` and `array_iter.rs` handle eligible ranges and array
iteration in the lowered Rust tree. Replacing indexed access with iteration
needs the same visited elements and order. An index needed for another
computation must remain available, and writes must preserve the borrow the loop
needs.

C gotos are structured earlier. For those rules, see
[Structure Goto](../transforms/structure-goto.md) and
[Labeled Blocks](../transforms/labeled-blocks.md). For the exit cleanup above,
start with `label_elide.rs`.
