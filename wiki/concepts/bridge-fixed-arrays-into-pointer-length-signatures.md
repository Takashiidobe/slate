# Bridge fixed arrays into pointer length signatures

_created 2026-08-29_

`length_lattice` now admits fixed-size local and global arrays as exact roots
for borrowed pointer-plus-length signature lifts.

Array declarations are indexed by name, element type, and declared length.
Aligned wrappers are peeled, and lowered pointer decay is followed through
`as_ptr`, `as_mut_ptr`, `addr_of`, and typed `cast` expressions. Duplicate
names are treated as ambiguous and rejected.

The proof requires the companion length to equal the declared array length
and the element type to match exactly. It is available only to shared and
mutable slice candidates. `Vec` and `String` candidates still require exact
heap-allocation provenance, so stack or static storage can never be adopted by
an owning Rust type.

`ptr_len_array_sources.c` covers global `&[i32]`, stack `&mut [i32]`, stack
`&str`, exact-length mismatch rejection, and owned-stack rejection.
