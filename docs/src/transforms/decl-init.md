# Decl Init

<div class="transform-pass">

## Purpose

Use `decl-init` to select this pass. Combine an uninitialized declaration with
its first assignment so lowering can emit an initialized Rust binding.

## Example

**C input**

```c
int choose(int c) {
    int x;
    if (c)
        x = 4;
    else
        x = 9;
    return x;
}
```

**Emitted Rust**

```rust
fn choose(c: i32) -> i32 {
    let x: i32 = if c != 0 {
        4
    } else {
        9
    };
    x
}
```

## Rewrite rules

```text
let x; S; write(x, E) → S; let x = E
let x; if C { A; write(x, E) } else { B; write(x, F) }
    → let x = if C { A; E } else { B; F }
```

The first rule moves the declaration to the write. The second keeps each branch
prefix in its original arm. In both rules the initializer runs where the first
write's value would have run.

## Caveats

The declaration must have no source initializer. In the straight-line rule `S`
and `E` must leave `x` untouched, with no control flow or lifetime boundary that
prevents sinking. The branch rule needs an adjacent conditional with one final
write per branch and no intervening use of `x` in the test or either arm. For
example, `int x; prepare(&x); x = compute();` needs the storage before `prepare`
runs.

</div>
