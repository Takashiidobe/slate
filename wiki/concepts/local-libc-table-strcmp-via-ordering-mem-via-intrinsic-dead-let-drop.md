# Local libc table: strcmp via Ordering, mem\* via intrinsic, dead-let drop

_created 2026-08-29_

Extends the local libc-rewrite table (`backend/engine/rules/libc_call.rs`, see
[[local-libc-call-rewrite-table-with-multi-hop-lifted-arg-chase]]) with three
shapes beyond the value-as-is `strlen` case.

**strcmp -> `a.cmp(b) as i32`.** `Ordering` is `#[repr(i8)]` (Less=-1,
Equal=0, Greater=1), and `&str`/`&[u8]` `Ord` is lexicographic _unsigned_-byte
order — identical to C strcmp's ordering. So `.cmp() as i32` is sign-faithful
and yields -1/0/1, which also matches glibc's vectorized ±1 in practice. Exact
byte-difference _magnitude_ is not reproduced (impl-defined anyway); the
element must be `u8`/`str` (guard `is_byte_str_or_slice`) since a signed `i8`
slice would flip sign. Follow-up idiomatization of `x <cmp> 0` -> `a <cmp> b`
is deferred to a later pass. The chase now also accepts const string-literal
args (`const_str_literal`), so `strcmp(s, "abc")` works when only `s` lifts.

**mem\* -> intrinsic.** memcpy/memmove -> `PtrCopy` (copy_nonoverlapping/copy),
memset -> `WriteBytes`, args cast to `*u8`. No lifting needed (raw byte ops).
memcpy/memmove/memset fixtures updated to expect the intrinsic form (was raw
`libc::` fallback).

**Return-value / dead-let handling.** C mem* return `dst`; CIR binds that to a
temp even for discarded `memcpy(...);`. `node_discards_result` = bare `Expr`
OR a `Let` whose name has no readers (`def_use_neighbors` empty). When
discarded, mem* emits the bare unit effect and `apply` drops the dead `Let` to
an `Expr` statement via the new `arena.set_kind` (maintains the `defs` index).
When the result is genuinely used, mem\* emits `{ effect; dst }` preserving the
pointer value. Rule now also fires in `Assign`/`CompoundAssign` positions
(`int order = strcmp(...)` lowers to `order = strcmp(...)`), which required
extending `NodeKind::call_anchor` to those kinds.

Verified e2e: `local_libc_strcmp` (multi-buffer lift + strcmp in assign),
`local_libc_mem` (memcpy/memmove/memset), plus the three mem\* fixtures; full
rewrites (110) and libc (19) profiles green.
