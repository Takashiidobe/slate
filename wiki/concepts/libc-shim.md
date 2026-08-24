# libc-shim

Slate has a dir that implements a libc surface of declarations only that
we call while compiling (clang is compiled with -nostdlibinc and points
to our include dir). We used to compile with clang's provided libc
(generally the one on the system, glibc) but this ran into two problems:

- It's not safe across libcs. If you transpile a C program to rust that uses a
  "glibc" only interface, like `strerror_r` with a `char*` return, and
  then try to run it on a rust target without glibc, this will succeed
  but give you the wrong result.
- It's not safe across targets: If you use a struct that's different
  across targets, say `fenv_t` which controls the floating point
  environment and link to your current stdlib:

## Target feature contract

Slate parses `SLATE_TARGET`, or its own Cargo build target when that variable is
unset, and defines exactly one macro in each family before Clang reads the shim:

| Family              | Examples                                                |
| ------------------- | ------------------------------------------------------- |
| `__SLATE_ARCH_`     | `X86`, `X86_64`, `ARM`, `AARCH64`, `RISCV32`, `RISCV64` |
| `__SLATE_VENDOR_`   | `UNKNOWN`, `PC`, `APPLE`                                |
| `__SLATE_KERNEL_`   | `LINUX`, `WINDOWS`, `DARWIN`                            |
| `__SLATE_LIBC_`     | `GLIBC`, `MUSL`, `MINGW`, `MSVC`, `BIONIC`, `DARWIN`, `GENERIC` |
| `__SLATE_PLATFORM_` | `ANDROID`, `MACOS`                                      |
| `__SLATE_OBJ_`      | `ELF`, `COFF`, `MACHO`                                  |
| `__SLATE_WORDSIZE_` | `32`, `64`                                              |
| `__SLATE_ENDIAN_`   | `LITTLE`, `BIG`                                         |

`features.h` rejects missing, conflicting, and unsupported selections. The
target environment and libc are not interchangeable: a GNU Linux target maps
to `__SLATE_LIBC_GLIBC`, while a GNU Windows target maps to
`__SLATE_LIBC_MINGW`.

## Basic AArch64 macOS profile

`SLATE_TARGET=aarch64-apple-darwin` selects Apple, macOS, the Darwin kernel and
libc, Mach-O, LP64, and little endian. Slate passes
`arm64-apple-macos11.0` to Clang so the initial deployment baseline is stable.
The tracked header manifest is `libc-shim/macos-basic-headers.txt`.

The profile covers the compiler-provided freestanding headers and the basic
hosted ISO C surface in `assert.h`, `ctype.h`, `errno.h`, `math.h`, `stdio.h`,
`stdlib.h`, `string.h`, and `time.h`. It models signed 32-bit `wchar_t`,
Darwin's exact and fast integer typedefs, signed 64-bit `time_t`, unsigned
64-bit `clock_t`, pointer-shaped AArch64 `va_list`, and 64-bit binary64
`long double`. Generated Rust uses `f64` for Darwin `long double` and checks
for `aarch64-apple-darwin` without linking when that Rust target is installed.

The basic manifest is extended by
`libc-shim/macos-stdio-locale-headers.txt`, which models Darwin's public
`struct __sFILE`, `fpos_t`, 128-byte `mbstate_t`, `_xlocale` handle, `lconv`,
locale category and `langinfo` values, wide-character types, and standard
stream accessors. POSIX signals, pthreads, filesystem and directory
structures, sockets, and process APIs remain outside the supported profile
until their dedicated ABI tickets are complete. Carrying Darwin
assembler-label identities through generated Rust also remains pending. The
shim does not search Linux host include directories for a Darwin target.

Darwin defaults to `__DARWIN_C_FULL`. `_POSIX_SOURCE`, `_POSIX_C_SOURCE`, and
`_XOPEN_SOURCE` select the matching Darwin namespace, while
`_DARWIN_C_SOURCE` restores the full namespace alongside a POSIX request.
The AArch64 profile fixes UNIX 2003 and 64-bit inode selection and defines the
Darwin assembler-label families so Clang provenance can retain the selected
foreign symbol.

## Basic 64-bit Android Bionic profiles

`SLATE_TARGET=aarch64-linux-android` and
`SLATE_TARGET=x86_64-linux-android` select Android platform, Linux kernel,
Bionic libc, ELF, LP64, and little-endian profiles. Android targets also require
`SLATE_ANDROID_API`; the initial supported baseline is API 21. The API is kept
separate from the Rust target triple and Slate passes the combined Android
compiler target to Clang.

The tracked public-header manifest is
`libc-shim/bionic-basic-headers.txt`. It covers the compiler-provided and basic
freestanding header surface. Fixtures in `tests/fixtures/bionic/` compile
against the shim for both 64-bit architectures and, when the pinned NDK oracle
is present under `target/android-ndk-oracle`, against the NDK headers too.

Both profiles use 64-bit pointers and C `long`, 32-bit `int`, 64-bit
`intmax_t`, unsigned 32-bit `wint_t`, and 16-byte-aligned IEEE binary128
`long double`. AArch64 uses unsigned 32-bit `wchar_t` and a 32-byte `va_list`
record. x86-64 uses signed 32-bit `wchar_t` and a 24-byte array-backed
`va_list`. Slate lowers the binary128 `long double` representation to Rust
`f128` and preserves the architecture-specific variadic representation exposed
by CIR.

Android shim compilation does not search host system include directories, so a
missing Bionic declaration cannot silently resolve to glibc or musl.

## Basic x86-64 MSVC profile

`SLATE_TARGET=x86_64-pc-windows-msvc` selects the MSVC profile. Its
tracked public-header manifest is `libc-shim/msvc-basic-headers.txt` and covers
the supported ISO C assertion, character, error, floating environment, float,
limits, locale, math, signal, variadic, scalar, integer, stream, allocation,
string, time, and wide-character surfaces. It also admits UCRT `process.h`,
`sys/stat.h`, `sys/timeb.h`, `sys/types.h`, and `sys/utime.h` extensions.

The profile models LLP64: pointers, `size_t`, `ptrdiff_t`, `intptr_t`, and
`uintptr_t` are 64-bit; `long` is 32-bit; `long long` is 64-bit; `wchar_t` is
unsigned 16-bit; and `long double` has the same 64-bit representation and
alignment as `double`. It also models `errno_t`, `rsize_t`, the UCRT errno
code numbering, `div_t`/`ldiv_t`/`lldiv_t`, the 9-`int`-field `struct tm`
(without the glibc/BSD `tm_gmtoff`/`tm_zone` extensions), `CLOCKS_PER_SEC`,
and the small MSVC `signal.h` surface (`sig_atomic_t`, `SIGINT`/`SIGILL`/
`SIGFPE`/`SIGSEGV`/`SIGTERM`/`SIGBREAK`/`SIGABRT`/`SIGABRT_COMPAT`,
`SIG_DFL`/`SIG_IGN`/`SIG_GET`/`SIG_SGE`/`SIG_ACK`/`SIG_ERR`). MSVC-only
declarations that don't fit as a target branch inline in a shared header live
under `libc-shim/bits/msvc/` (`errno.h`, `signal.h`, `stddef.h`, `time.h`),
included from the corresponding top-level header.

The time and file-status extensions model `__time32_t` and `__time64_t`, the
three UCRT timespec records, all four `_stat` layouts, `_dev_t`/`_ino_t`/
`_off_t`, the `_S_*` mode constants, and the explicit 32/64-bit time, stat,
utime, and timeb function families. They deliberately omit POSIX stat fields,
mode constants, and timestamp APIs. Secure variants and macro-selected aliases
are separate surfaces.

Secure CRT declarations are grouped under `libc-shim/bits/msvc/secure/`.
The UCRT default of `__STDC_WANT_SECURE_LIB__ == 1` exposes `rsize_t` and the
standard bounds-checked names. Defining it as zero hides those names while
retaining underscore-prefixed Microsoft extensions and the UCRT conversion
functions that are not Annex K-gated. Inline forwarding and imported symbol
selection remain outside the declaration shim.

`process.h` exposes the UCRT process identifiers, `_cwait`, narrow and wide
exec/spawn families, and begin/end thread callback signatures. The MSVC
`stdlib.h`/`wchar.h` branches also expose program-path, startup argument, and
narrow/wide environment accessors. Windows SDK process APIs and obsolete DLL
loader helpers are not part of the declaration shim.

Fixtures under `tests/fixtures/msvc/` cross-compile against `libc-shim`, and
(when `target/msvc-sysroot` is bootstrapped, see [msvc reference sysroot](msvc-reference-sysroot.md))
against the pinned xwin CRT/UCRT trees as a declaration/layout oracle. The
`tests/differential.rs` fixture runner picks the target and compiler by the
fixture's directory, so this is Clang-only verification — it does not run
Slate's own translate pipeline. That's because the current CIR build cannot
emit record or enum tag definitions for its MSVC ABI backend
(`HandleTagDeclDefinition: MSABI`, tracked by `slate-wlpp.7`): any translation
unit that defines a `struct`/`union`/non-anonymous-macro `enum` for this
target — which now includes `<stdlib.h>` (`div_t` family) and `<time.h>`
(`struct tm`) — fails to lower today regardless of whether the fixture itself
uses the type. `errno.h`'s error codes are plain `#define`s (matching the real
UCRT header) specifically to keep that near-universally-included header
translatable. `tests/fixtures/msvc/llp64.c` is the one fixture Slate does
translate end to end, and is kept free of headers that declare aggregates for
that reason.

Every admitted header compiles independently against the shim and, when the
pinned sysroot is present, xwin. Excluded UCRT families are tracked by
`slate-wlpp.8` through `slate-wlpp.11`: filesystem/descriptors/search/allocation,
console/DOS, multibyte/UTF, and remaining numeric extensions. Internal CRT,
C++/SafeInt, generic-text, and Windows SDK headers are outside the C declaration
manifest. Macro redirects and inline/import identities remain in
`slate-wlpp.4`. Including `pthread.h`, `unistd.h`, `sys/ioctl.h`, or
`sys/socket.h` fails explicitly.

For safety across libcs:

`strerror_r` is defined as such for our libc:

```c
#if defined(_GNU_SOURCE)
char *strerror_r(int, char *, size_t);
#elif defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L ||                \
    defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600 ||                          \
    defined(_POSIX_SOURCE) || defined(_BSD_SOURCE)
int __xpg_strerror_r(int, char *, size_t);
#define strerror_r __xpg_strerror_r
#endif
```

This is because there are two different versions of `strerror_r`. One is the
posix standard (in this case, `__xpg_sterror_r` which is defined to
`strerror_r` if `_GNU_SOURCE` isn't set), and the "glibc" version, which
returns a `char*`.

If there's C code that uses `strerror_r`, this is fine because whichever
library you link to (musl or glibc) will give you the right definition
based on compiler flags. However, if we use the system libc (glibc for
me) and transpile to rust, and someone on a musl based system tries to
compile their rust code, they'll get different behavior since the
signature is different.

In rust we can specify this like so:

```rust
#[cfg(target_env = "gnu")]
extern "C" {
    pub fn strerror_r(
        errnum: c_int,
        buf: *mut c_char,
        buflen: usize
    ) -> *mut c_char;
}

#[cfg(target_env = "musl")]
extern "C" {
    pub fn strerror_r(
        errnum: c_int,
        buf: *mut c_char,
        buflen: usize
    ) -> c_int;
}
```

However if we link to the system libc, we can only see one definition of
`strerror_r` (whichever one is enabled based on how it was compiled) and
so we will fail at our goal of transpiling once and being able to run
the rust code on another computer, even if the C code is perfectly
portable.

For the target part, take a look at this program:

```c
#include <fenv.h>
#include <stdio.h>

int main(void) {
  fenv_t original_environment;

  printf("fegetenv original %d\n", fegetenv(&original_environment));
}
```

And compile with my system's libc (I'm on x86_64 linux, using glibc):

```rust
#![allow(dead_code, unused, non_snake_case, non_upper_case_globals, arithmetic_overflow)]

#[repr(C)]
#[derive(Clone, Copy)]
struct fenv_t {
    __bitfield_0: u16,
    __bitfield_1: u16,
    __bitfield_2: u16,
    __bitfield_3: u16,
    __bitfield_4: u16,
    __bitfield_5: u16,
    __bitfield_6: u32,
    __bitfield_7: u16,
    __bitfield_8: u16,
    __bitfield_9: u32,
    __bitfield_10: u16,
    __bitfield_11: u16,
    __bitfield_12: u32,
}


unsafe extern "C" {
    fn fegetenv(_0: *mut fenv_t) -> i32;
}

fn main() {
    let mut original_environment: fenv_t = fenv_t { __bitfield_0: 0, __bitfield_1: 0, __bitfield_2: 0, __bitfield_3: 0, __bitfield_4: 0, __bitfield_5: 0, __bitfield_6: 0, __bitfield_7: 0, __bitfield_8: 0, __bitfield_9: 0, __bitfield_10: 0, __bitfield_11: 0, __bitfield_12: 0 };
    println!("fegetenv original {}", unsafe { fegetenv(std::ptr::addr_of_mut!(original_environment) as *mut fenv_t) });
}
```

Note that you'll get some definition of fenv_t (it depends based on what
your libc defines it as) but if I try to run this on a different
architecture, say aarch64, this code will be incorrect, because it
assumes I'm using the fenv_t from `x86_64`. This is because the libc on
your environment is set to be conditionally compiled to only be for your
host target. We therefore cannot accomplish our goal of compiling
"portable" C to "portable Rust".

However, if you compile the same example with slate:

You'll get the proper definitions for both x86_64 and aarch64, and the
unsafe extern will even be merged for us. Since the original C code was
meant to be portable, the output rust should be as well:

```rust
#[cfg(target_arch = "x86_64")]
#[repr(C)]
#[derive(Clone, Copy)]
struct fenv_t {
    __bitfield_0: u16,
    __bitfield_1: u16,
    __bitfield_2: u16,
    __bitfield_3: u16,
    __bitfield_4: u16,
    __bitfield_5: u16,
    __bitfield_6: u32,
    __bitfield_7: u16,
    __bitfield_8: u16,
    __bitfield_9: u32,
    __bitfield_10: u16,
    __bitfield_11: u16,
    __bitfield_12: u32,
}


#[cfg(target_arch = "aarch64")]
#[repr(C)]
#[derive(Clone, Copy)]
struct fenv_t {
    __fpcr: u32,
    __fpsr: u32,
}


#[cfg(any(target_arch = "x86_64", target_arch = "aarch64"))]
unsafe extern "C" {
    fn fegetenv(_0: *mut fenv_t) -> i32;
}

#[cfg(target_arch = "x86_64")]
fn main() {
    let mut original_environment: fenv_t = fenv_t { __bitfield_0: 0, __bitfield_1: 0, __bitfield_2: 0, __bitfield_3: 0, __bitfield_4: 0, __bitfield_5: 0, __bitfield_6: 0, __bitfield_7: 0, __bitfield_8: 0, __bitfield_9: 0, __bitfield_10: 0, __bitfield_11: 0, __bitfield_12: 0 };
    println!("fegetenv original {}", unsafe { fegetenv(std::ptr::addr_of_mut!(original_environment) as *mut fenv_t) });
}

#[cfg(target_arch = "aarch64")]
fn main() {
    let mut original_environment: fenv_t = fenv_t { __fpcr: 0, __fpsr: 0 };
    println!("fegetenv original {}", unsafe { fegetenv(std::ptr::addr_of_mut!(original_environment) as *mut fenv_t) });
}
```
