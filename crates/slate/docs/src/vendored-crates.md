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

## bitint

`_BitInt(N)` is a C23 feature that allows for arbitrarily sized integers
(up to N bits). This isn't a supported feature in Rust, so Slate has its
own implementation. The crate itself implements numerical traits that
are required by C. There's a mapping in Slate from each operation in C
to what it is in Rust.

## num-complex

`Complex` numbers in C also have to be represented. Thankfully, the
`num-complex` crate handles most complex operations. One wrinkle is that
Complex numbers have to handle `long double` which, when it's an f80,
doesn't have an analogue in rust.

## aligned

Because of Sys-V alignment (requiring functions to be 16-byte aligned)
some functions are wrapped in the `aligned::Aligned` crate in order to
handle pointer arithmetic properly.
