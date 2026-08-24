# Model Darwin stdio locale and wide text ABI

_created 2026-08-24_

The AArch64 macOS stdio and locale profile is listed in
`libc-shim/macos-stdio-locale-headers.txt`. Darwin-specific public record
definitions belong in `bits/darwin/`; shared ISO declarations stay in the
top-level headers only where their signatures match.

Darwin exposes `FILE` as `struct __sFILE`, uses a signed 64-bit `fpos_t`, a
128-byte 8-byte-aligned `mbstate_t`, an opaque `struct _xlocale *` locale
handle, 32-bit `wint_t`, `wctype_t`, and `wctrans_t`, and Darwin-specific
locale and `nl_item` numbers. Standard streams resolve through `__stdinp`,
`__stdoutp`, and `__stderrp`; they are not global `FILE` objects.

The structural fixture compiles against the shim and, when configured,
against `SLATE_MACOS_SDK`. Its generated Rust is checked with rustc for
`aarch64-apple-darwin` without linking. The Apple Big Sur open-source snapshot
supports the declarations but does not replace the installed-SDK check.
