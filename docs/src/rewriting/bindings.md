# Bindings and Temporary Storage

Lowering can introduce locals for a Rust expression, a pointer alias, or a slot
shared by several branches. The rewrite engine can remove that scaffolding when
the resulting Rust scope and access order permit it.

## Inline a temporary

**Before**

```rust
let tmp = compute();
consume(tmp);
```

**After**

```rust
consume(compute());
```

A single read makes substitution possible. The move still needs to preserve when
the initializer runs and what it reads. For example, a temporary holding
`compute()` cannot move into a short circuit branch if that would skip the call.
An intervening assignment to a value the initializer reads can also block it.

A synthetic pointer alias has the same problem. If `q` holds a copied value of
`p`, replacing a later use of `q` with `p` would read a different pointer after
`p` is reassigned. See the
[alias regression fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/raw_ptr_alias_source_reassigned.c).

## Recover a binding

- [Branch Initialization](branch-initialization.md) turns eligible assignments
  into the arms of an `if` initializer.
- [Dispatch Slot Promotion](dispatch-slots.md) replaces eligible `MaybeUninit`
  slots and synthetic pointers with direct uses of a typed local.

For initialization decided on C IR and mutability chosen during lowering, see
[Decl Init](../transforms/decl-init.md) and
[Binding Mutability](../lowering/remove_mut.md).
