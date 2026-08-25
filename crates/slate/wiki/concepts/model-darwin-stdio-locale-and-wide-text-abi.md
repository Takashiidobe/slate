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

## Feature-mode normalization and symbol-alias provenance

Darwin feature-mode normalization and symbol-alias provenance flow from the
macro plugin through `c_ast`, alongside mode and redirect fixtures — so
Darwin's macro-driven symbol redirection (e.g. `__DARWIN_ALIAS`-style
renaming) is tracked with the same provenance machinery already used
elsewhere, not a Darwin-specific bolt-on.

A multi-mode macOS SDK probe runner exercises this against a real SDK when
one is configured (`SLATE_MACOS_SDK`), and skips explicitly rather than
silently passing when it isn't. The libc suite, provenance suite, and
focused stdio/locale differential fixtures pass.

## Known gaps

This is implemented but partial: there is no SDK installed on the current
dev host for oracle comparison (the probe runner's skip path is what's
actually exercised in CI today, not the real-SDK path), redirected linker
labels still need the downstream Rust-identity work, and the remaining
ABI-area dependencies are open. Full rewrites also has two unrelated
pre-existing failures (`gnu_empty_struct`, `integer_bit_builtin_ops`) — not
caused by the Darwin work.
