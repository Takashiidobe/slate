# Unsafe Placement

Lowering marks the operations that need `unsafe`, such as a raw pointer
dereference or an unsafe call. Rewrite rules carry those marks when moving or
replacing expressions. Placement then adds Rust blocks around the operations
that still need them.

## Place the block around the operation

A raw pointer read on the right-hand side can be scoped to the expression:

```rust
let x = unsafe { *p };
use_value(x);
```

A raw pointer write needs the assignment target inside the unsafe region:

```rust
unsafe { *p = value; }
```

Placement can enclose the common expression containing several unsafe
operations. It can also merge adjacent whole-statement unsafe blocks when doing
so preserves scopes. A `let` between operations can require a separate block so
the binding stays visible.

This ordering keeps rewrite matches independent of the final block placement. It
also lets project linking resolve a call to a safe Rust path before placement
decides whether the call still needs a block.
