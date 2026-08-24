# MSVC UCRT manifest audit

_created 2026-08-24_

`libc-shim/msvc-basic-headers.txt` is the supported x86-64 MSVC C header
manifest. Every entry must compile independently against both the shim and the
pinned xwin UCRT tree; `tests/header_compilation.rs` enforces that invariant.

The manifest covers ISO C scalar, integer, variadic, assertion, error, signal,
locale, character, string, allocation, process, stream, time, math, floating
environment, and wide-character families, plus UCRT stat/timeb/utime records.
Focused fixtures in `tests/fixtures/msvc/` compare every exposed aggregate
category and representative divergent signatures and constants with xwin.

The audit removes declarations and macros that xwin does not expose, including
POSIX limits and C23 additions. Macro redirects, inline forwarding helpers, and
imported symbol identities remain owned by `slate-wlpp.4`.

Excluded public families are tracked by `slate-wlpp.8` (filesystem,
descriptors, search, allocation), `slate-wlpp.9` (console and DOS),
`slate-wlpp.10` (multibyte and UTF), and `slate-wlpp.11` (complex, inttypes,
and fpieee). Internal `corecrt*`/`vcruntime*`, C++ `new`/SafeInt, generic-text
`tchar.h`, and Windows SDK headers are not public C declaration-shim headers.
