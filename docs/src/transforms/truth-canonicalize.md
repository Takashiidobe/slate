# Truth Canonicalize

<div class="transform-pass">

## Purpose

Use `truth-canonicalize` to select this pass. Remove redundant truth tests and
recover short-circuit expressions from boolean IR.

## Example

**C input**

```c
int inverted(int a, int b) {
    return !(a < b);
}
```

**Emitted Rust**

```rust
fn inverted(a: i32, b: i32) -> i32 {
    (a >= b) as i32
}
```

## Rewrite rules

```text
ne(from_bool(b), 0)            → b
eq(from_bool(b), 0)            → not(b)
not(not(b))                    → b
not(lt_int(a, b))              → ge_int(a, b)
if c { 1 } else { 0 }          → from_bool(c)
if c { 1 } else { from_bool(y) } → from_bool(c || y)
```

`from_bool` maps false to 0 and true to 1. The first two identities follow from
those two values. The last identity evaluates `y` only when `c` is false. The
pass can also recover a short circuit from branches writing the same synthetic
boolean temporary. A source variable written by both arms keeps its branch
writes.

## Caveats

Ordered floating point comparisons need a separate rule. With `a = NaN`,
`!(a < b)` is true and `a >= b` is false, so the pass keeps the negation.
Negating equality can still flip `==` and `!=`. Rewrites that would remove an
explicit conversion or comparison, or a protected macro value, are blocked.

</div>
