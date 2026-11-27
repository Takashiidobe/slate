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

## Replace a slot with a binding

Lowering can use a `MaybeUninit` slot when a value crosses dispatch regions.
Promotion needs a proof that each affected read follows initialization and that
moving the declaration preserves scope and address uses. Likewise, an eligible
pair of branch writes can become the two arms of a Rust initializer when each
path supplies its value and keeps its effects in place.

Look in `inline_temps.rs`, `raw_ptr_alias.rs`, `slot_promote.rs`, and
`branch_init.rs` under the rule directory. For initialization decided on C IR
and mutability chosen during lowering, see
[Decl Init](../transforms/decl-init.md) and
[remove_mut](../lowering/remove_mut.md).
