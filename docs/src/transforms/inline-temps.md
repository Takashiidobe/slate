# Inline Temps

<div class="transform-pass">

## Purpose

Use `inline-temps` to select this pass. Move a synthetic temporary’s initializer
into its only read so lowering can emit the expression directly.

## Example

**C input**

```c
int decrement(int n) {
    n--;
    return n;
}
```

**Emitted Rust**

```rust
fn decrement(mut n: i32) -> i32 {
    n -= 1;
    n
}
```

## Rewrite rules

```text
temp t = E; S; use(read(t))  →  S; use(E)
temp a = read(n); temp b = a - 1; write(n, b)
                            →  write(n, read(n) - 1)
```

The pass substitutes the initializer once, keeping its type and node ID.

## Caveats

The read must be in one of the next 32 sibling statements or the list's tail
value. It must be a direct value use outside macro arguments, address operands,
and statement expressions. Captures, a second read, or an unsequenced temporary
keep the binding.

Moving `E` reorders it with `S` and any evaluation before the read in the
consumer. For ordinary reads and writes, the conflicts to check are
`reads(E) ∩ writes(S)`, `writes(E) ∩ reads(S)`, and `writes(E) ∩ writes(S)`. For
example, sinking `t = read(n)` past `write(n, 7)` would change which value
reaches the consumer. The pass also blocks opaque statements and control flow.
An effectful initializer must execute on the same paths, so moving a call under
the right operand of `&&` would be invalid when the left operand is false.

</div>
