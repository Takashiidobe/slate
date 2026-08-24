# MSVC process and environment surface

_created 2026-08-24_

The admitted C-facing process surface is `process.h` plus the environment
declarations shared by `stdlib.h` and `wchar.h`.

It includes `_getpid`, `_cwait`, all narrow and wide `_exec*` and `_spawn*`
variants, begin/end thread entry points and callback typedefs, startup argument
and environment accessors, program-path accessors, narrow and wide environment
mutation/search functions, and the UCRT process constants.

The top-level `process.h` dispatches only for MSVC and is empty for other libc
profiles. Windows SDK process creation APIs, obsolete DLL loader helpers,
nonstandard aliases, macro redirects, and final imported symbol selection are
excluded. The latter three belong to `slate-wlpp.4`.
