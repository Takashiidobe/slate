# Do While Zero

<div class="transform-pass">

## Purpose

Use `do-while-zero` to select this pass. Turn a do-while loop with a constant
false test into a block that runs once.

## Example

**C input**

```c
int once(int stop, int x) {
    do {
        if (stop)
            continue;
        x += 1;
    } while (0);
    return x;
}
```

**Emitted Rust**

```rust
fn once(stop: i32, mut x: i32) -> i32 {
    if stop == 0 {
        x += 1;
    }
    x
}
```

## Rewrite rules

```text
do_L { A } while false  →  block { A }                 if A has no jumps to L
do_L { A } while false  →  block L { A[continue L ↦ break L] } otherwise
```

A continue targeting `L` reaches the false test, so it has the same destination
as a break targeting `L`. Inner loop jumps keep their targets. The later
`labeled-blocks` pass can turn the exit in this example into a plain `if`.

## Caveats

The test must consist of a false boolean constant or a recognized false integer
comparison, with no evaluation statements. For example, `while (tick(), 0)`
still calls `tick()` whenever execution reaches its test. Kept expansions retain
the loop shape for their macro template. The same loop becomes eligible when
Slate emits the expansion inline.

</div>
