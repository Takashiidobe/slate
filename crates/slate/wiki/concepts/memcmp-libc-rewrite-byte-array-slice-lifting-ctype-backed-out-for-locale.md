# memcmp libc rewrite + byte-array slice lifting; ctype backed out for locale

_created 2026-08-29_

Extends the local libc-rewrite table
([[local-libc-table-strcmp-via-ordering-mem-via-intrinsic-dead-let-drop]]) with
`memcmp`, and fixes a length-lattice gap so byte slices actually lift.

**memcmp -> `a[0..n].cmp(&b[0..n]) as i32`.** Mirrors strcmp but slices to the
explicit count `n`. Restricted to genuine byte slices via `is_u8_slice`
(excludes `&str`, so no UTF-8 boundary panic). Uses an explicit `0..n` range
because `Expr::Range` has no empty-endpoint sentinel and is otherwise never
constructed.

**Byte-slice lifting gap (length_lattice).** `&[u8]` params never lifted:
`exact_allocation_pair` needs a `len * elem_size` Mul in the malloc size, which
degenerates for size-1 elements (`malloc(n)` has no `* 1`), and
`exact_utf8_array_pair` is str-only. Added `exact_byte_array_pair`: for
size-1 elem candidates, resolve the backing array via `resolved_byte_array`
and accept `len <= array_len` (sound for the `&arr[..len]` bridge, mirrors the
str `<=`). Wired into the non-Str/StringOwned proof arm. Unblocks any
`fn(u8ptr, len)` over a stack byte array, not just memcmp.

**ctype/tolower/toupper NOT landed.** Implemented then backed out: the flat
table has no proof the C locale is unchanged, so `is_ascii_*`/`to_ascii_*`
would miscompile under `setlocale(LC_ALL,"")`. Guard fixtures
`ctype_*_setlocale_non_c` encode the must-not-fire cases. Follow-up bead filed
for a locale-guarded version. abs/isnan/isinf/isfinite need no table row —
already lowered at CIR level.

Verified: local_libc_memcmp fires end-to-end; rewrites 110/110, libc 19/19.
