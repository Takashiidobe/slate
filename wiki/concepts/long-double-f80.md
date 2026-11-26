# How Slate represents C long double

Slate represents C `long double` as `f64`, `f80`, or `f128`, depending on the
target ABI. For `f80`, Slate generates a Rust type named `LongDouble`.

## Formats by target

| Format                         | Representative targets                              |
| ------------------------------ | --------------------------------------------------- |
| `f64` (IEEE binary64)          | Windows MSVC, Apple silicon macOS, 32-bit ARM Linux |
| `f80` (x87 extended precision) | x86-64 Linux, i686 Linux, Intel macOS               |
| `f128` (IEEE binary128)        | AArch64 Linux and Android, x86-64 Android           |

These are default formats from Slate's
[target definitions](../../crates/slate-parser/src/target_info.rs). Runtime
coverage for a target and call shape comes from the
[differential fixtures](../../crates/slate/tests/).

## How Slate stores f80

Rust has no `f80` primitive. Converting to `f64` would narrow the value and can
change arithmetic and comparisons. On x86-64 Linux, Slate emits this type.

```rust
#[repr(C, align(16))]
#[derive(Clone, Copy)]
struct LongDouble([u8; 10]);
```

The array holds the 80-bit payload. With padding, the struct occupies 16 bytes
and has 16-byte alignment, matching C `long double` on this target. Slate uses C
helpers for arithmetic, comparisons, and casts.

## Why f80 calls need a C bridge

The x86-64 System V ABI passes scalar C `long double` arguments in stack memory
and returns scalar results in the x87 register `st(0)`. The byte wrapper is an
integer-class struct. Its arguments can go in general-purpose registers, and its
result comes back in `rax` and `rdx`. The sizes match, but the caller and callee
would look in different places. See the
[x86-64 psABI, §3.2.3](https://gitlab.com/x86-psABIs/x86-64-ABI/-/blob/master/x86-64-ABI/low-level-sys-info.tex).

For a call to `sqrtl`, the generated Rust declares the shim with the wrapper
type at both ends.

```rust
unsafe extern "C" {
    fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
}
```

The C shim accepts the matching `__slate_f80` byte struct. It copies the payload
into a native `long double` before the call and back into the byte struct
afterward. The C compiler handles the stack argument and x87 return because it
sees the native signature.

```c
typedef struct {
  unsigned char bytes[10];
} __attribute__((aligned(__alignof__(long double)))) __slate_f80;

static long double __slate_f80_load(__slate_f80 value) {
  long double out = 0.0L;
  memcpy(&out, value.bytes, sizeof(value.bytes));
  return out;
}

static __slate_f80 __slate_f80_store(long double value) {
  __slate_f80 out;
  memcpy(out.bytes, &value, sizeof(out.bytes));
  return out;
}

extern long double __slate_extern_sqrtl(long double) __asm__("sqrtl");

__slate_f80 __slate_sqrtl__rf80_f80(__slate_f80 _0) {
  return __slate_f80_store(__slate_extern_sqrtl(__slate_f80_load(_0)));
}
```

Project translation writes the required bridge code to `src/slate_long_double.c`
and builds it from `build.rs`. If you compile standalone Rust output that uses
x87 helpers or bridges, compile and link the C shim as part of your build.

See the [Rust wrapper](../../crates/slate/src/frontend/long_double.rs),
[C helpers](../../crates/slate/src/frontend/shims/long_double.c),
[C bridge generator](../../crates/slate/src/frontend/c_shim.rs), and
[native ABI fixture](../../crates/slate/tests/fixtures.dylink/long_double_abi/)
for implementation details and tested call shapes.
