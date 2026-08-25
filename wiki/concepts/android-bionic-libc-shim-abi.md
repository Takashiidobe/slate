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
together (translate-only — `tests/differential.rs` is disabled repo-wide
right now, unrelated to Android).

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

## Known test-isolation flake (pre-existing, unrelated)

`gcc_torture_suite` and the snapshot tests race on a shared `target/` scratch
dir when run together under the `lowering` nextest profile. Reproduces
identically on a clean `HEAD` with no Android-related changes — not caused
by, or specific to, this work.
