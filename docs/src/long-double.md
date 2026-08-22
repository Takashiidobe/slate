# long double

`long double` has existed since C89, but the standard says almost nothing
about its representation, so every platform picked differently:

- `f64` MSVC on x86_64 just aliases `long double` to `double`.
- `f80` GCC and Clang on x86/x86_64 use the 80-bit x87 extended format,
  stored padded to 12 or 16 bytes depending on ABI.
- `double-double` two `f64`s summed together, used on older PowerPC/AIX.
- `f128` IEEE quad precision, used on ARM64, RISC-V, and modern PowerPC
  (`ppc64le`).

`f64` and `f128` cases need no special casing, because they're
implemented as types in Rust. `f80` however, is special.

## Why f80 needs a shim

The obvious approach is a 10-byte (or 16-byte padded) struct:

```rust
#[repr(C, align(16))]
#[derive(Clone, Copy)]
pub struct LongDouble(pub [u8; 10]);
```

That's a fine _storage_ representation, but it's wrong the moment a `long
double` crosses a function boundary or needs arithmetic. The x86
ABI passes `long double` in an x87 register, not in a general-purpose register
or SSE register the way a `[u8; 10]` struct would be. A Rust function with that struct in its
signature and real x87 arithmetic on the bits would use the wrong calling
convention and the wrong instructions.

Implementing f80 arithmetic correctly in Rust means either reimplementing
x87 semantics (rounding, the 80-bit mantissa/exponent split, math fns) or writing
inline assembly for every operation. Both are really difficult, when our
C compiler takes care of it anyway.

## Using shims

Slate keeps `long double` values in an opaque byte-array type on the Rust
side and does all _actual_ f80 arithmetic in small C helper functions
compiled alongside the generated Rust and linked in via `extern "C"`. The
byte array is only ever a transport format between calls into the shim;
nothing on the Rust side inspects or computes on the bytes directly.

The C side (`__slate_f80`) is a struct wrapping the raw bytes:

```c
typedef struct {
  unsigned char bytes[10];
} __attribute__((aligned(16))) __slate_f80;

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
```

Every arithmetic op, comparison, cast, and libm function
is shimmed as `load(a) op load(b)` then `store`d, so the actual computation
always happens in real `long double` C code where x87 semantics are free.
`_Complex long double` gets the same treatment, as a pair of
`__slate_f80` fields (`__slate_cf80`).

Generated Rust never sees `long double` itself; it sees `Type::LongDouble`,
which lowers to the opaque 10-byte struct, and every operation on it is
rewritten into a call to the matching `__slate_f80_*`/`__slate_cf80_*`
extern function instead of a native Rust operator.

## Picking the right long double

Lowering asks two questions about a CIR `LongDouble`/`Fp80` type, in order:

1. Is it actually quad precision on this target (`is_quad_long_double`)? If
   so, lower to `Prim::F128`, and use `f128` bit for bit.
2. Otherwise, is the target ABI one where `long double` is just `double`
   (`uses_f64_long_double_abi`, like MSVC)? If so, use `f64`.
3. Otherwise it's x87 style long double: lower to
   `Type::LongDouble`, the shimmed representation.
4. We don't support double-double targets, so no need for that.

## Handling Function Pointers

A C function that takes or returns `long double` by value can't be called
directly from Rust with the shimmed byte-array type: the ABI still expects
a real x87 value in the argument/return slot, and a `[u8; 10]`
param wouldn't match it. So any `extern "C"` declaration or function
pointer whose signature mentions `long double` gets a matching trampoline:
a small C wrapper (name-prefixed `__slate_ld_`) with the true `long double`
signature that unpacks its shimmed arguments, calls the real function, and
packs the shimmed return. Lowering tracks the mapping from original function
name to trampoline name, so the C sees the right code and the Rust only
sees the `[u8; 10]`.
