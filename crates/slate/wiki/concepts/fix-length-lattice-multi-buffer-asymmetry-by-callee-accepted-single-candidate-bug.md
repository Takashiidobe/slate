# Fix length-lattice multi-buffer asymmetry (by_callee/accepted single-candidate bug)

_created 2026-08-29_

`length_lattice.rs` (slate-y0qs.4.8, ptr+len -> buffer lifting) silently
lifted only ONE buffer param per function. A function with two `(ptr, len)`
pairs — e.g. `cmp_texts(const char *a, int alen, const char *b, int blen)` —
lifted `b`/`blen` to `&str` but left `a`/`alen` as a raw `*mut i8`.

Root cause: two `BTreeMap<&str, usize>` maps keyed by function name only:

- `by_callee` in `signature_call_sites`: `.collect()` kept only the
  last-indexed candidate per function, so call sites only ever resolved that
  one candidate. The others got zero call sites and were disabled by the
  `count == 0` guard.
- `accepted` in the driver: same `.collect()` collapse, so the apply side
  also only rewrote one candidate per function.

Fix: both become per-function `Vec<usize>` of candidate ids.
`signature_call_sites` emits one proof per candidate per call (ascending id).
`apply_signature_fn` retypes every accepted ptr param and removes their len
params in descending index order (so earlier removals don't shift later
indices); `rewrite_signature_expr` bridges each ptr arg and removes each len
arg per call, descending. Proof-deque alignment holds because an active
candidate has no `None`-proof sites (any would disable it), so every call
contributes exactly one proof per active candidate, and both producer and
consumer iterate candidates in ascending-id order.

The seed gate (`active &= seeded || forwarding_only`) already rejected the
spurious cross-pairings `(a, blen)`/`(b, alen)`, so no extra guard was needed.

Verified: `cmp_texts` now lifts both params to `&str`; regression-clean across
the rewrites and libc profiles.
