# MSVC secure CRT declaration surface

_created 2026-08-24_

MSVC secure declarations live under `libc-shim/bits/msvc/secure/`, grouped by
the public header that exposes them. Shared ISO header branches only include
the corresponding MSVC entry file.

The pinned UCRT defaults `__STDC_WANT_SECURE_LIB__` to `1`. When enabled, it
defines `rsize_t` and exposes the standard bounds-checked names. Setting it to
`0` hides those names and `rsize_t`, but underscore-prefixed UCRT extensions
and Microsoft conversion functions such as `mbstowcs_s` remain available.

The surface includes errno accessors, invalid-parameter handler types and
entry points, narrow and wide stdio, string and memory operations, search and
conversion functions, environment buffer APIs, locale variants, and narrow
and wide time functions. It does not model inline forwarding bodies, common
stdio helpers, macro redirects, or imported symbol identities; those belong to
`slate-wlpp.4`.
