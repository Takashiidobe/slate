# Floating point

- `__builtin_isnan`, `__builtin_isinf`, `__builtin_isfinite`, and
  `__builtin_isnormal` classify values.
- `__builtin_issubnormal`, `__builtin_iszero`, and `__builtin_issignaling` test
  individual floating-point classes.
- `__builtin_signbit`, `__builtin_signbitf`, and `__builtin_signbitl` test the
  sign bit.
- `__builtin_isinf_sign` tests infinity and reports its sign.
- `__builtin_fpclassify` selects the supplied result for the value's class.
- `__builtin_isgreater`, `__builtin_isgreaterequal`, `__builtin_isless`,
  `__builtin_islessequal`, `__builtin_isunordered`, and
  `__builtin_islessgreater` implement quiet comparisons.
- `__builtin_complex` constructs a complex value from its components.
- `__builtin_inf` and `__builtin_huge_val` construct positive infinity in the
  registered binary floating-point widths.
- `__builtin_nan` and `__builtin_nans` construct quiet and signaling NaNs.
  Registered binary-width variants can fold a literal payload in a static
  initializer.
- `__builtin_flt_rounds` reads the current rounding mode through a C shim.
- `__builtin_fabs`, `__builtin_copysign`, `__builtin_fmax`, `__builtin_fmin`,
  `__builtin_fma`, and `__builtin_powi` have scalar lowering paths. The operand
  type and floating-point policy can select a C helper or library call.

Use `f`, `l`, `f16`, or `f128` suffixes on the infinity and NaN families where
registered. GCC also recognizes decimal variants such as `__builtin_infd32` and
`__builtin_nand64`. These stay as calls.

## `__builtin_isnan`

```c
int is_nan(double x) { return __builtin_isnan(x); }
```

The C result is an integer even though the Rust predicate returns a boolean.

```rust,ignore
fn is_nan(x: f64) -> i32 {
    x.is_nan() as i32
}
```

## `__builtin_inf`

```c
double infinity(void) { return __builtin_inf(); }
```

Slate emits a constant positive infinity.

```rust,ignore
fn infinity() -> f64 {
    f64::INFINITY
}
```

## `__builtin_fabs`

```c
double absolute(double x) { return __builtin_fabs(x); }
```

For this `double` argument, Slate declares and calls LLVM's scalar
absolute-value intrinsic.

```rust,ignore
unsafe extern "llvm-intrinsic" {
    #[link_name = "llvm.fabs.f64"]
    fn __slate_intrinsic_999a0f45e9537850(arg0: f64) -> f64;
}

fn absolute(x: f64) -> f64 {
    unsafe { __slate_intrinsic_999a0f45e9537850(x) }
}
```

## `__builtin_fpclassify`

```c
int example(double x) {
    return __builtin_fpclassify(0, 1, 2, 3, 4, x);
}
```

The arguments before `x` supply the return values for NaN, infinity, normal, subnormal, and zero, in that order.

```rust,ignore
fn example(x: f64) -> i32 {
    if x.is_nan() {
        0
    } else if x.is_infinite() {
        1
    } else if x.is_normal() {
        2
    } else if x.is_subnormal() {
        3
    } else {
        4
    }
}
```

## `__builtin_nan`

```c
double example(void) {
    return __builtin_nan("0x1");
}
```

Slate puts the literal payload into the bits of a quiet NaN.

```rust,ignore
fn example() -> f64 {
    f64::from_bits(0x7ff8000000000001)
}
```

## `__builtin_fma`

```c
double example(double x, double y, double z) {
    return __builtin_fma(x, y, z);
}
```

Slate emits a call to the C library's `fma` for this `double` example.

```rust,ignore
unsafe extern "C" {
    #[link_name = "fma"]
    fn __builtin_fma(arg0: f64, arg1: f64, arg2: f64) -> f64;
}

fn example(x: f64, y: f64, z: f64) -> f64 {
    unsafe { __builtin_fma(x, y, z) }
}
```
