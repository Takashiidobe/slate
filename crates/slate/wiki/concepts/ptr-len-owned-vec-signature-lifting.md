# ptr_len owned Vec signature lifting

_created 2026-08-28_

`ResolvedPtrType::Vec` uses the same canonical pointer/length candidate graph
as borrowed slices, but ownership changes the root bridge and terminal cleanup.

An accepted exact allocation root becomes
`Vec::from_raw_parts(ptr, len, len)`. The equality between allocation count,
logical length, and capacity is required: accepting an allocation such as
`malloc((len + 1) * sizeof(T))` with logical length `len` would make Vec
deallocate with the wrong layout, so that shape fails closed.

Forwarding candidates move their existing `Vec<T>` into the callee. The
terminal function's matching C `free` call is removed because Vec's `Drop`
subsumes it; CIR-hoisted argument temporaries remain. Function bodies retain
raw-pointer locals through `Vec::as_mut_ptr()` until separate local rewrites
recover indexing.

Vec seed discovery requires a local `free` call before performing the more
expensive loop/offset scans. Forwarding-only Vec functions are discovered by
the candidate dependency graph after a terminal seed exists. This keeps the
libexpat fast path cheap when no Vec lift is available.

String representations remain disabled. `slate-y0qs.4.4` must prove UTF-8 or
character validity before `Str` or `StringOwned` can be emitted soundly.
