# Integer arithmetic and bits

- `__builtin_add_overflow` checks addition and stores the converted result
  through the result pointer.
- `__builtin_sub_overflow` does the same for subtraction.
- `__builtin_mul_overflow` does the same for multiplication.
- `__builtin_umul_overflow`, `__builtin_umull_overflow`, and
  `__builtin_umulll_overflow` cover the corresponding unsigned multiplication
  widths.
- `__builtin_rotateleft8` and `__builtin_rotateright8` rotate bits. The
  corresponding `16`, `32`, and `64` suffixes select wider types.
- `__builtin_stdc_rotate_left` and `__builtin_stdc_rotate_right` preserve the
  unsigned operand type, including supported bit-precise integer widths.
- `__builtin_clz` and `__builtin_ctz` count leading and trailing zero bits.
- `__builtin_clrsb` counts redundant sign bits.
- `__builtin_popcount` counts set bits.
- `__builtin_parity` returns the parity of the set bits.
- `__builtin_ffs` returns the one-based index of the lowest set bit, or zero.
  These count and scan families include the registered `l` and `ll` forms.
- `__builtin_clzg`, `__builtin_ctzg`, and `__builtin_popcountg` are type-generic
  bit counts.
- `__builtin_bswap16`, `__builtin_bswap32`, and `__builtin_bswap64` reverse
  bytes. GCC mode also supports `__builtin_bswap128`.
- `__builtin_bitreverse8`, `__builtin_bitreverse16`, `__builtin_bitreverse32`,
  and `__builtin_bitreverse64` reverse bits.

For `__builtin_clz` and `__builtin_ctz`, a zero input has undefined behavior.
The type-generic `__builtin_clzg` and `__builtin_ctzg` take an optional second
argument to return for zero.

## `__builtin_popcount`

```c
int count_bits(unsigned value) { return __builtin_popcount(value); }
```

Rust counts the set bits and converts the result to the C return type.

```rust,ignore
fn count_bits(value: u32) -> i32 {
    value.count_ones() as i32
}
```

## `__builtin_add_overflow`

```c
_Bool checked_add(int a, int b, int *out) {
    return __builtin_add_overflow(a, b, out);
}
```

Slate stores the result and returns the overflow flag.

```rust,ignore
fn checked_add(a: i32, b: i32, out: &mut i32) -> bool {
    {
        let __t0: i128 = a as i128;
        let __t1: i128 = b as i128;
        let __t2: i128 = __t0.wrapping_add(__t1);
        *out = __t2 as i32;
        __t0.checked_add(__t1).is_none() || ((__t2 as i32) as i128) != __t2
    }
}
```

## `__builtin_rotateleft32`

```c
unsigned rotate(unsigned value, unsigned count) {
    return __builtin_rotateleft32(value, count);
}
```

Rust rotates the bits within the same 32-bit width.

```rust,ignore
fn rotate(value: u32, count: u32) -> u32 {
    value.rotate_left(count as u32)
}
```

## `__builtin_clz`

```c
int example(unsigned int x) {
    return __builtin_clz(x);
}
```

Count the leading zero bits in a nonzero 32-bit value. The C builtin has undefined behavior for zero.

```rust,ignore
fn example(x: u32) -> i32 {
    x.leading_zeros() as i32
}
```

## `__builtin_ctzg`

```c
int example(unsigned short x) {
    return __builtin_ctzg(x, 16);
}
```

Count the trailing zero bits at the width of the `unsigned short` operand. The second argument gives the result for zero.

```rust,ignore
fn example(x: u16) -> i32 {
    {
        let __t0 = x;
        let __t1 = 16i32;
        if __t0 == 0 { __t1 } else { __t0.trailing_zeros() as i32 }
    }
}
```

## `__builtin_bswap32`

```c
unsigned example(unsigned x) {
    return __builtin_bswap32(x);
}
```

Reverse the four bytes of the input.

```rust,ignore
fn example(x: u32) -> u32 {
    unsafe { __slate_intrinsic_e94e9cf815168b51(x) }
}
```
