# Labeled Blocks

<div class="transform-pass">

## Purpose

Use `labeled-blocks` to select this pass. Recover if/else branches and switch
arms from labeled blocks and their exits.

## Example

**C input**

```c
int choose(int c, int x) {
    if (c)
        goto other;
    x += 1;
    goto done;
other:
    x += 2;
done:
    return x;
}
```

**Emitted Rust**

```rust
fn choose(c: i32, mut x: i32) -> i32 {
    if c == 0 {
        x += 1;
    } else {
        x += 2;
    }
    x
}
```

## Rewrite rules

```text
block L { if C { A; break L }; B; break L }
    → if C { A } else { B }
```

Both branches reach the end of `L`, and each executes the same body as before.
This pass can also sink a common tail into the sole break that reaches it, and
can move case siblings into switch arms.

## Caveats

If two breaks share the tail, sinking would duplicate it. If the true branch
ends in `continue`, the two branches have different destinations. A tail move
also needs to preserve scope and name resolution. For example, moving a call to
`helper()` into a block that declares a local named `helper` would change what
the emitted Rust call resolves to. Cleanup, VLA, and SEH scopes can also block a
move.

</div>
