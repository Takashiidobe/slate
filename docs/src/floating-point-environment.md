# Floating-point environment

`#pragma STDC FENV_ACCESS ON` makes the active floating-point environment part
of the behavior of C arithmetic. With Clang settings on x86-64 Linux, the middle
division below uses the rounding mode set by `fesetround(FE_DOWNWARD)`. The
first and last divisions use the surrounding nearest-even policy.

```c
#include <fenv.h>

void compute(volatile double x, volatile double y, double out[3]) {
    out[0] = x / y;
    fesetround(FE_DOWNWARD);
    {
#pragma STDC FENV_ACCESS ON
        out[1] = x / y;
    }
    fesetround(FE_TONEAREST);
    out[2] = x / y;
}
```

## How the IR carries the policy

Semantic analysis starts with the compiler settings and tracks the
floating-point policy for each C scope. The pragma sets the policy inside the
block. When semantic analysis leaves the block, it resumes the enclosing policy.
The block itself has no effect on the machine's rounding mode. The two
`fesetround` calls change that mode at runtime.

The IR attaches the policy directly to each division. Run
`slate emit-slate-ir input.c` to see it. These are the three division
expressions, in source order, with the surrounding stores omitted.

```ir
div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(
    read<f64, volatile>(%3), read<f64, volatile>(%4))
div<f64, rounding=environment, exceptions=observable, contract=on>(
    read<f64, volatile>(%3), read<f64, volatile>(%4))
div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(
    read<f64, volatile>(%3), read<f64, volatile>(%4))
```

`rounding=environment` means the division consults the rounding mode in effect
when it runs. `exceptions=observable` tells IR consumers that the program can
observe floating-point exceptions, for example by reading the flags with
`fetestexcept`. The IR keeps `fesetround` as a call in program order, so the
division carries a policy that still works when the mode is chosen dynamically.
`contract=on` records the permission to fuse eligible operations within an
expression. It is unchanged across these three divisions.

## How lowering uses it

Lowering reads the division's policy from the node. It emits ordinary Rust `/`
for the first and last divisions and calls `__slate_fenv_div_f64` for the middle
division. The C shim evaluates the operation with the active environment. Here
is the middle assignment from the generated Rust.

```rust
out[1] = unsafe {
    __slate_fenv_div_f64(
        core::ptr::read_volatile(core::ptr::addr_of!(x)),
        core::ptr::read_volatile(core::ptr::addr_of!(y)),
    )
};
```

Use `lowering-barriers` to check support for the pragma, operation, and target.
[Rounding-mode fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/stdlib_fenv_access.c)
