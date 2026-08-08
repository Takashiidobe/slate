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
| `__SLATE_LIBC_`     | `GLIBC`, `MUSL`, `MINGW`, `MSVC`, `GENERIC`             |
| `__SLATE_OBJ_`      | `ELF`, `COFF`, `MACHO`                                  |
| `__SLATE_WORDSIZE_` | `32`, `64`                                              |
| `__SLATE_ENDIAN_`   | `LITTLE`, `BIG`                                         |

`features.h` rejects missing, conflicting, and unsupported selections. The
target environment and libc are not interchangeable: a GNU Linux target maps
to `__SLATE_LIBC_GLIBC`, while a GNU Windows target maps to
`__SLATE_LIBC_MINGW`.

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
