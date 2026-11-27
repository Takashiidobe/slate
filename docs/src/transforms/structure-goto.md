# Structure Goto

<div class="transform-pass">

## Purpose

Use `structure-goto` to select this pass. Replace direct gotos with labeled
blocks and loops where the jump targets fit structured control flow.

## Example

**C input**

```c
int run(int skip, int x) {
    if (skip)
        goto done;
    x += 1;
done:
    return x * 2;
}
```

**Emitted Rust**

```rust
fn run(skip: i32, mut x: i32) -> i32 {
    if skip == 0 {
        x += 1;
    }
    x * 2
}
```

## Rewrite rules

```text
if C { goto L }; A; L: B  →  block K { if C { break K }; A }; B
L: A; if C { goto L }; B  →  do { A } while C; B
```

For a forward edge, `K` is fresh and the block ends exactly where `L` began. For
a backward edge, the second rule requires a single trailing conditional jump to
`L`. The general backward rule wraps the range in a loop, turns its back edges
into continues, and adds an exit where the original range fell through. In the
example, later passes turn the forward block into an `if`.

## Caveats

Account for every edge to the label before replacing it. A jump into the middle
from outside the candidate range can bypass the new structured entry. The pass
also checks for VLA or cleanup declarations and case labels owned by an outer
switch. An address such as `&&L` makes the function ineligible because the label
is also a value. Residual jumps go to dispatch during lowering.

</div>
