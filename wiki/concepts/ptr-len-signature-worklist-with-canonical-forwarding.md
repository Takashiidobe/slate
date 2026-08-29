# ptr_len signature worklist with canonical forwarding

_created 2026-08-28_

`length_lattice` seeds signature candidates from locally proven bounded
pointer/length uses. It also admits a forwarding-only candidate when every
read of its canonical length value is an argument paired with the same
canonical pointer value at another candidate call.

Call sites prove a pair in one of two ways:

- an exact `malloc(n * sizeof(T))` allocation and the same normalized,
  already-hoisted length value;
- a forwarded pointer/length candidate in the caller.

Candidates start active and only move to rejected. A failed source queues
only its dependent candidates, so each candidate changes state at most once.
AST bodies, definitions, and call sites are indexed before this worklist; the
worklist itself never rescans a body.

Accepted functions are rewritten together after convergence. Their pointer
parameter becomes `&[T]` or `&mut [T]`, the companion length parameter is
removed, and its body uses become `slice.len()`. Exact allocation callers use
`from_raw_parts[_mut]`; forwarded callers pass their existing slice. CIR's
hoisted argument temporaries remain, preserving evaluation and side effects.

`tests/fixtures/ptr_len_worklist.c` covers an exact allocation seed followed
by a forwarding hop and a mutable terminal indexed use. The libexpat 21-TU
benchmark measured 5.537s, 5.273s, and 5.439s after this port.
