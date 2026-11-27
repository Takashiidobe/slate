# Loop and Iterator Recovery

Slate can turn a C counted loop into a Rust range. It can also replace an
eligible range over a fixed array with direct iteration over the elements.

## Recover a range

```c
int sum_to(int n) {
    int total = 0;
    for (int i = 0; i < n; ++i)
        total += i;
    return total;
}
```

```rust
fn sum_to(n: i32) -> i32 {
    let mut total = 0;
    for i in 0..n {
        total += i;
    }
    total
}
```

Rust evaluates the range end once. If the C loop changes `n` in its body or
calls a function to compute the end on every iteration, a fixed range could
change which iterations run. An index that is reassigned, borrowed by a call, or
needed after the loop can also prevent range recovery. Slate can choose a range
from [C IR facts](../analysis/counted-loops.md), and the Rust AST rule
`for_range::recover` recognizes eligible lowered `while` loops.
[Range fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/for_range_recover.c)

## Visit array elements directly

```c
int a[5] = {1, 2, 3, 4, 5};
int total = 0;
for (int i = 0; i < 5; ++i)
    total += a[i];
```

```rust
let a = __SlateAlign16([1, 2, 3, 4, 5]);
let mut total = 0;
for value in a.0.iter().copied() {
    total += value;
}
```

The wrapper preserves the array's alignment on the example target. The
`array_iter::recover` rule checks that the range starts at zero and ends at the
fixed array length, and that the body uses the index once to read that array. If
the body computes `total += i * a[i]`, the index is still needed. A write
through `a[i]` or a mutable borrow of the array also blocks this shared
iterator.
[Array iteration fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/range_loop_local_array.c)
