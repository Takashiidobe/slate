# Const Fold

<div class="transform-pass">

## Purpose

Use `const-fold` to select this pass. Fold implicit integer conversions, signed
negations, and comparisons of constants into typed literals.

## Example

**C input**

```c
signed char narrow(void) {
    signed char c = 200;
    return c;
}
```

**Emitted Rust**

```rust
fn narrow() -> i8 {
    let c: i8 = -56;
    c
}
```

## Rewrite rules

```text
implicit_int_convert<T>(const(v)) → const<T>(convert_T(v))
r = v mod 2^width(T), with 0 <= r < 2^width(T)
convert_T(v) = r - 2^width(T) if T is signed and r >= 2^(width(T)-1)
convert_T(v) = r              otherwise
```

For the eight-bit signed destination, `r = 200` and `200 - 256 = -56`. The pass
also folds signed literal negation and eligible integer constant comparisons,
such as `ne(1, 0) → true`.

## Caveats

The match is limited to these IR operations. An explicit cast such as
`(signed char)200`, an arithmetic expression such as `60 * 60`, or a conversion
of a wrapped macro constant stays in the IR.

</div>
