# Bionic libc-shim ABI facts

> Oracle-verified against the NDK r27d sysroot (see
> [android-ndk-oracle.md](android-ndk-oracle.md) for how that oracle is
> materialized/probed). Covers `libc-shim`'s stdio/locale/wide-text layer for
> both 64-bit Android targets.

`libc-shim/bionic-stdio-locale-headers.txt` (stdio.h, locale.h, wchar.h,
wctype.h, uchar.h, langinfo.h) compiles clean for `aarch64-linux-android21`
and `x86_64-linux-android21` with `-nostdlibinc`, mirroring the
`bionic-basic-headers.txt` manifest pattern. This only proves compile-time
scoping (`bionic_stdio_locale_header_manifest_compiles_for_64_bit_targets` in
`tests/header_compilation.rs`); full ABI probes (`FILE` identity, `fpos_t`,
`mbstate_t` layout, `lconv` fields) against the NDK oracle are the separate
fixes below.

## Real ABI divergences from glibc, fixed in the shim

Bionic differs from the shim's glibc-shaped defaults in several places that
matter because they cross the FFI boundary to the real `libc.so`:

- `fpos_t` is a **plain `off_t` scalar**, not glibc's 16-byte union.
- `__mbstate_t` is `{__seq: [u8; 4], __reserved: [u8; 4]}`, align-1 — not
  glibc's `{int __count; union}`, align-4.
- `wctype_t` is `signed long`; `wctrans_t` is `const void*` — not glibc's
  `unsigned long` / `const int*`.
- `locale.h`/`wchar.h`/`wctype.h` gate the `newlocale`/`uselocale`/`_l`
  family and `LC_PAPER..LC_IDENTIFICATION` **unconditionally** for Bionic —
  real Bionic headers don't feature-test-macro-gate them the way glibc does.
- `langinfo.h`'s `nl_item` constants are Bionic's plain `1..55` ordinals
  (`CODESET=1`), not glibc's packed encoding (`CODESET=14`). This one is a
  real correctness bug if gotten wrong, not just a layout mismatch — the
  value is passed to the real `nl_langinfo()` in `libc.so`.

Verified via NDK-sysroot `clang -fsyntax-only` for both 64-bit targets, plus
`cargo check` against real `aarch64`/`x86_64-linux-android` rustc targets.
Structural fixture: `tests/fixtures/bionic/stdio_locale_conversion.c`
exercises `FILE`/`fpos_t`/`mbstate_t`/`locale_t`/`wctype_t`/`nl_langinfo`
together, with full `@lowering`/`@rewrite` FileCheck coverage now that
`tests/differential.rs` is back online.

`struct lconv` (glibc-shaped, shared by every non-Darwin/FreeBSD libc) was
cross-checked field-by-field against the NDK r27d sysroot's `locale.h`:
order and types match exactly, no Bionic-specific branch needed.

## The Rust `libc` crate has no Android `mbstate_t` binding

`libc` 0.2.189 doesn't bind `mbstate_t` for Android at all, so lowering
`mbstate_t` through `CLIB_RECORD_TYPES` (`src/backend/rust_ast.rs`) to
`libc::mbstate_t` doesn't compile for Android targets. Fixed by removing
`mbstate_t` from `CLIB_RECORD_TYPES` entirely rather than special-casing
Android: the generic record-lowering path already emits a correct
`#[repr(C)]` struct straight from the shim's own (now oracle-accurate)
definition, for every libc — there was no need for a `libc`-crate binding at
all. 3 insta snapshots (`c11`, `c23_library`, `uchar_conversions`) now show
real struct literals instead of `std::mem::zeroed::<libc::mbstate_t>()`.

## Filesystem/dirent ABI (slate-sfzn.7.4)

`libc-shim/bionic-filesystem-headers.txt` (sys/types.h, sys/stat.h,
sys/statvfs.h, dirent.h, fcntl.h, utime.h, unistd.h) compiles clean for both
64-bit Android targets. Layout facts are locked in by
`tests/fixtures/bionic/filesystem_layout.c`
(`bionic_filesystem_layout_fixture_compiles_for_64_bit_targets` in
`tests/header_compilation.rs`), compiled for both architectures from one file
via `#if defined(__SLATE_ARCH_X86_64)`.

Real Bionic keeps `struct stat` byte-compatible with the Linux kernel struct
per architecture, which happens to diverge from the shim's shared
`bits/generic/stat.h` / `bits/x86_64/stat.h` in exactly two fields — added as
`bits/bionic/stat.h`, dispatched from `bits/stat.h`:

- **aarch64**: `st_blksize` is a raw `int` (4 bytes) and `st_blocks` a raw
  `long`, not the `blksize_t`/`blkcnt_t` typedefs — `struct stat` is 128
  bytes.
- **x86_64**: `st_nlink` is a raw `unsigned long` (8 bytes, not `nlink_t`),
  and `st_blksize`/`st_blocks` are raw `long` — `struct stat` is 144 bytes.
- The public `blksize_t`/`blkcnt_t` typedefs (as opposed to the raw struct
  fields above) are **unsigned long** on Bionic, unlike glibc/musl's signed
  `long`. `nlink_t` itself stays `uint32_t` on every Bionic architecture,
  including x86_64 — it's only the `struct stat` field that bypasses the
  typedef there.
- `struct statvfs` has no public `f_type` field; Bionic reserves that word as
  part of an anonymous `uint32_t __f_reserved[6]` tail.

Confirmed by reading `sys/stat.h`'s `__STAT64_BODY` macro and `sys/types.h`
directly out of the NDK r27d sysroot, not just by probing generated layouts.

Bionic also does not feature-test-macro-gate the `*64` LFS family, `mknod`/
`mknodat`, `S_IREAD`/`S_IWRITE`/`S_IEXEC`, `seekdir`/`telldir`, or most
`fcntl.h` `AT_*`/`F_*`/`splice`/`fallocate`-family declarations behind
`_GNU_SOURCE`/`_XOPEN_SOURCE`/`_LARGEFILE64_SOURCE` the way glibc does — real
Bionic headers expose them unconditionally (`<linux/fcntl.h>` is included
unconditionally by Bionic's `<fcntl.h>`). The shim now exposes the same set
unconditionally under `__SLATE_LIBC_BIONIC`, following the same pattern
already used for locale.h/wchar.h/wctype.h in 7.3. Conversely, real Bionic
has **no** `getdents`, `posix_getdents`, `versionsort`, `name_to_handle_at`,
`open_by_handle_at`, `struct file_handle`, or `struct f_owner_ex` at all — the
shim now excludes those for Bionic instead of leaking the Linux-libc surface
(`bionic_filesystem_layout_fixture_compiles_for_64_bit_targets` asserts
`getdents`/`posix_getdents` fail to compile).

API-level gating (`lockf`/`lockf64` need API ≥24, `statx` needs API ≥30,
`seekdir`/`telldir` need API ≥23) is explicitly left to slate-sfzn.7.9 — this
ticket only fixes feature-test-macro visibility and struct/typedef shape at
the API 21 baseline, matching 7.3's carve-out.

### `O_*` flag bits: arm/aarch64 swap several values relative to x86

The Linux kernel UAPI (and so Bionic, which ships generated kernel headers
verbatim) assigns different bit patterns to `O_DIRECTORY`/`O_NOFOLLOW`/
`O_DIRECT`/`O_LARGEFILE`/`O_TMPFILE` on arm/aarch64 than on x86 — this is a
kernel-arch divergence, not a libc one, so it applies identically to any
libc built for those architectures:

| macro         | x86 (generic) | arm/aarch64 |
| ------------- | -------------:| -----------:|
| `O_DIRECTORY` |      `0200000`|    `040000` |
| `O_NOFOLLOW`  |      `0400000`|   `0100000` |
| `O_DIRECT`    |       `040000`|   `0200000` |
| `O_LARGEFILE` |      `0100000`|   `0400000` |
| `O_TMPFILE`   |    `020200000`| `020040000` |

`bits/aarch64/fcntl.h`/`bits/arm/fcntl.h` already had the correct values,
but they were dead: the top-level `libc-shim/include/fcntl.h` unconditionally
redefined the same macro names with the x86 values right after including
`bits/fcntl.h`, silently overwriting them on every architecture (invisible
on x86_64 because the values happened to coincide there). Fixed by giving
the generic values their own `bits/generic/fcntl.h` (dispatched from
`bits/fcntl.h`'s `#else`, following the same convention as `bits/stat.h`)
and deleting the clobbering block from the top-level header — each arch's
open-flag section now lives entirely in its own `bits/<arch>/fcntl.h`.
Caught by `filesystem_layout`'s Bionic NDK-oracle cross-compile in
`tests/differential.rs`, which failed on the real aarch64 sysroot's
`O_DIRECTORY` static-assert once `tests/differential.rs` came back online.

### Pre-existing, out-of-scope bug found along the way

While cross-checking `__nlink_t`/`__blksize_t` against the real ABI, probing
glibc/aarch64 (unrelated to Bionic) showed `sizeof(struct stat)` is
**already wrong today** for that target (144 bytes instead of 128): the
shared `bits/types.h` gives `__nlink_t` = `unsigned long` for
`__SLATE_LIBC_GLIBC && __SLATE_WORDSIZE_64` with no architecture check, but
real glibc/Linux only widens `nlink_t` to 8 bytes on x86_64 — aarch64 (and
riscv64) need the 4-byte `unsigned int`. This predates and is unrelated to
the Bionic work here, so it was left alone and reported separately rather
than folded into this change (see `slate-jxmx`).

## Known test-isolation flake (pre-existing, unrelated)

`gcc_torture_suite` and the snapshot tests race on a shared `target/` scratch
dir when run together under the `lowering` nextest profile. Reproduces
identically on a clean `HEAD` with no Android-related changes — not caused
by, or specific to, this work.
