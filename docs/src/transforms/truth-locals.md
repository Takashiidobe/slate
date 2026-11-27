# Truth Locals

<div class="transform-pass">

## Purpose

Use `truth-locals` to select this pass. Store an integer local as a boolean when
all its definitions produce 0 or 1 and its uses favor truth tests.

## Example

**C input**

```c
int classify(int a, int b) {
    int lt = a < b;
    if (lt)
        return 4;
    return 5;
}
```

**Emitted Rust**

```rust
fn classify(a: i32, b: i32) -> i32 {
    let lt: bool = a < b;
    if lt {
        return 4;
    }
    5
}
```

## Rewrite rules

```text
all definitions of x ∈ {0, 1}
encode(0) = false, encode(1) = true
ne(read_int(x), 0) → read_bool(x)
eq(read_int(x), 0) → not(read_bool(x))
other read_int(x)  → from_bool<int>(read_bool(x))
```

The encoding preserves every possible stored value. A definition qualifies if it
is 0, 1, a conversion from a boolean, a conditional with two qualifying arms, or
a copy from another proven local. The pass solves the proof across copies. Truth
uses must outnumber integer reads, since each remaining integer read needs a
conversion back.

## Caveats

A write of 2 would collapse two distinct nonzero values into true, changing a
later integer read. A call that writes through `&x` defeats the proof and needs
the original storage type. Updates, captures, volatile or atomic accesses, and
references in macro or OpenMP regions also keep the integer local.

</div>
