# Long Double

Slate uses the target's C `long double` format so that storage, precision, and
supported calls follow the C ABI. The same source can use binary64 on Windows
MSVC or Apple silicon macOS, x87 extended precision on x86-64 Linux, or
binary128 on AArch64 Linux. Slate represents those formats as `f64`,
`LongDouble`, and `f128`, respectively.

## Calling `sqrtl` on x86-64 Linux

```c
#include <math.h>

long double root(long double x) {
    return sqrtl(x);
}
```

The generated Rust keeps the 80-bit payload in ten bytes. Padding and alignment
give the wrapper the same size and alignment as C `long double` on this target,
both 16 bytes. Slate also uses C helpers for x87 arithmetic, comparisons, and
casts to preserve the extended precision.

```rust
#[repr(C, align(16))]
#[derive(Clone, Copy)]
struct LongDouble([u8; 10]);

unsafe extern "C" {
    fn __slate_sqrtl__rf80_f80(x: LongDouble) -> LongDouble;
}

fn root(x: LongDouble) -> LongDouble {
    unsafe { __slate_sqrtl__rf80_f80(x) }
}
```

The native x86-64 System V signature passes a scalar `long double` argument on
the stack and returns the result in the x87 register `st(0)`. The byte wrapper
is an integer-class struct and can travel through general-purpose registers.
Matching storage layout still leaves the two calling conventions different.

The bridge accepts the byte struct from Rust and copies its payload into a
native `long double`. The C compiler then calls `sqrtl` with the native ABI.
After the call, the bridge copies the returned payload back into the byte struct
and returns that to Rust. In the generated C, `__slate_f80_load` and
`__slate_f80_store` perform the two copies.

```c
typedef struct {
    unsigned char bytes[10];
} __attribute__((aligned(__alignof__(long double)))) __slate_f80;

extern long double __slate_extern_sqrtl(long double) __asm__("sqrtl");

__slate_f80 __slate_sqrtl__rf80_f80(__slate_f80 x) {
    return __slate_f80_store(__slate_extern_sqrtl(__slate_f80_load(x)));
}
```

`translate-project` writes the required C code to `src/slate_long_double.c` and
builds it from `build.rs`. If you build standalone Rust output, compile and link
the corresponding C shim alongside it.
[x87 library-call fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/x86_64/long_double_extern_sin.c)
