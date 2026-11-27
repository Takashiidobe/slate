# remove_mut

Slate uses IR def-use facts to decide which Rust bindings need `mut`. The old
`remove_mut` Rust rewrite now happens when Slate emits the binding. See
[Def-Use](../analysis/def-use.md) for how analysis collects the facts.

## Example

The caller owns both arrays. One call reads its array and the other writes
through an exclusive borrow. These excerpts omit the rest of the fixture.

**C input**

```c
static void fill(int *values, unsigned count) {
    for (unsigned i = 0; i < count; i++)
        values[i] = 2;
}

/* In the caller */
int shared[2] = {2, 4};
int exclusive[2] = {3, 5};
fill(exclusive, 2);
sum(shared, 2);
```

**Emitted Rust**

```rust
fn fill(values: &mut [i32]) {
    let count: u32 = values.len() as u32;
    for i in 0..count {
        values[i as usize] = 2;
    }
}

// In the caller
let shared: [i32; 2] = [2, 4];
let mut exclusive: [i32; 2] = [3, 5];
fill(exclusive.as_mut_slice());
sum(shared.as_slice());
```

## Rules

```text
initialize x; read(x)          → let x = ...
initialize x; write(x, E)      → let mut x = ...
owned x; borrow_shared(x)      → let x = ...; use(&x)
owned x; borrow_exclusive(x)   → let mut x = ...; use(&mut x)
p: &mut T; write(*p, E)        → p: &mut T
p: &mut T; reassign(p, q)      → mut p: &mut T
```

A field or index write to an owned value writes the binding's storage. A write
through a reference changes the referent. That is why the caller needs
`mut exclusive` and the callee can use `values: &mut [i32]` without a mutable
binding.

## Caveats

Lowering checks which borrow a call actually takes. Opaque address uses keep
mutability conservative. Assignments introduced during Rust lowering also count,
including dispatcher slots and OpenMP firstprivate copies.

See the
[binding mutability fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/binding_mutability.c).
