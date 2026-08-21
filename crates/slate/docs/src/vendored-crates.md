# Vendored Crates

Slate vendors some crates with fixes that are included in code that has
been translated to Rust.

## triplers

Since of the goals of Slate is to translate C to Rust but keep cross
compilation, we need a way to find the (architecture, OS, libc, endian)
of each target we compile to, and pass it to `libc-shim` in order to
conditionally compile code. `triplers` does most of this for us from the
passed in target. For a given target, say x86_64-unknown-linux-gnu,
slate can compile in `libc-shim` with the given characteristics:

- architecture: x86_64
- vendor: unknown
- os: linux
- libc: glibc
- endianness: little

These are exposed as `__SLATE_*` macros in `<features.h>`.

```c
#if defined(__SLATE_ARCH_X86_64)
/* code here */
#endif
```

So the standard library can conditionally include code per target. For
example, for Mac targets, the definitions for stdio.h are different than
for a linux target, so libc-shim can include these in differently:

```c
#if defined(__SLATE_LIBC_DARWIN)
typedef __fpos_t fpos_t;

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

#define stdin  __stdinp
#define stdout __stdoutp
#define stderr __stderrp

#undef FOPEN_MAX
#undef FILENAME_MAX
#undef TMP_MAX
#undef L_tmpnam
#define FOPEN_MAX    20
#define FILENAME_MAX 1024
#define TMP_MAX      308915776
#define L_tmpnam     1024
#else
typedef union _G_fpos64_t {
  char      __opaque[16];
  long long __lldata;
  double    __align;
} fpos_t;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin  (stdin)
#define stdout (stdout)
#define stderr (stderr)
#endif
```

Some other `__SLATE*` macros used:

- `__SLATE_OBJ_*` (`ELF`, `COFF`, `MACHO`): object file format, for code
  that has to emit or parse symbol tables (e.g. `dladdr`-style lookups).
- `__SLATE_WORDSIZE_*` (`64`, `32`): necessary for certain types that
  change based on word size.
- `__SLATE_ENDIAN_*` (`LITTLE`, `BIG`): used for endian independent
  code.
- `__SLATE_PLATFORM_ANDROID` / `__SLATE_PLATFORM_MACOS`
- `__SLATE_ANDROID_API__`: currently required to be greater than 21.

`target_features` in `src/cir/emit.rs` derives and uses `-D__SLATE_*`
flags from the parsed target triple before invoking Clang, so we have a
guarantee they're correct.

## bitint

`_BitInt(N)` is a C23 feature that allows for arbitrarily sized integers
(up to N bits). This isn't a supported feature in Rust, so Slate has its
own implementation. The crate itself implements numerical traits that
are required by C. There's a mapping in Slate from each operation in C
to what it is in Rust.

The bitint crate has two types, `BUint` and `BInt` (corresponding
to `unsigned _BitInt` and `_Bitint`). storing the value as a
fixed-size `[u8; BYTES]` byte array and reinterpreting it as `[u64; LIMBS]`
limbs for arithmetic `BITS` is the C-declared width, `LIMBS`/`BYTES`
are derived from it.

`src/frontend/lowerer/types.rs` (`bitint_type`, `bitint_storage_bytes`,
`bitint_generic_parts`) picks the concrete `BInt`/`BUint` instantiation for
a given `_BitInt(N)`, except for the two widths Rust already has native
integers for: 128-bit signed/unsigned `_BitInt` lowers straight to `i128`/
`u128` instead of going through the crate. Conversions to/from decimal
literals and native ints (`bitint_from_decimal_str_expr`,
`bitint_from_int_expr`, `bitint_to_int_expr`) are also generated per call
site, since the target width isn't known until lowering sees the C type.

## num-complex

`Complex` numbers in C also have to be represented. Thankfully, the
`num-complex` crate handles most complex operations. One wrinkle is that
Complex numbers have to handle `long double` which, when it's an f80,
doesn't have an analogue in rust.

For `float`/`double`, C `_Complex` lowers straight to
`num_complex::Complex<f32>`/`Complex<f64>` (`COMPLEX_TY` in
`src/frontend/lowerer/runtime_support.rs`), and Slate keeps `*`/`/` on those
bit-identical to Clang's libgcc lowering by routing them through `extern
"C"` runtime calls (`__muldc3`, `__divdc3`, `__mulsc3`, `__divsc3`) rather
than `num-complex`'s own generic multiply/divide.

`Complex<F80>` implements basic ops (`Add`, `Sub`, `Mul`, `Div`, `Neg`,
`PartialEq`, `PartialOrd`) required by `Complex<T>: Num`, each forwarding to
a `__slate_f80_*` shim (see [long double](./long-double.md)) for basic
arithmetic. Everything else C's `<complex.h>` needs for `long double`
is provided by the long double shim (`src/frontend/shims/long_double.c`).

## aligned

The `aligned` crate provides `aligned::Aligned<A, T>` with `A`, a marker type like
`aligned::A16` and `T`, the wrapped value, with `#[repr(align(N))]`
enforced.

The main reason slate uses this is for arrays: the x86-64 System V ABI aligns
large arrays (e.g. `char buf[16]`) to 16 bytes even with no
`_Alignas`/`__attribute__((aligned))` in the source, so the compiler can use
wider SSE loads/stores on them. Clang honors this and CIR reports the
resulting alignment on the declaration, but Rust's arrays only get their
element's natural alignment. There's no way to ask for a stricter array
alignment except by wrapping it, so we have to wrap arrays in aligned so
pointer arithmetic works on them.
