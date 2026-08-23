# setjmp/longjmp lowering

> See [fixups.md](fixups.md) for the general query-driven rewrite interface
> this rule implements, and [lowerer-internals.md](lowerer-internals.md) for
> where the plain-call side of `setjmp`/`longjmp` gets recognized during
> lowering (`intrinsics.rs`).

## Design: `catch_unwind`, not signals or a state machine

C's `setjmp`/`longjmp` is modeled as Rust's unwind mechanism, not reimplemented
with OS signals or a hand-rolled continuation/state machine:

- `setjmp(buf)` lowers to a `std::panic::catch_unwind(|| { .. })` wrapping the
  rest of the function body, returning `0` on the "called `setjmp`" path.
- `longjmp(buf, val)` lowers to a `panic!` carrying a payload struct unique to
  that jump-buffer (`payload_struct_item`), which `catch_unwind`'s `Err` arm
  unpacks back into the return value `setjmp` would have produced.

This is a whole-program rewrite rule (`recover_setjmp_catch_unwind`,
`src/backend/query/rules/setjmp_recovery.rs`, `Pass::SetjmpRecovery`), not a
per-function lowering handler, because it needs to see every call site that
can reach a `setjmp`-tagged function — including indirect calls through
function pointers — to correctly propagate the unwind ABI.

## `C-unwind` ABI propagation

Because `longjmp` becomes a Rust panic, any function on the call path between
the `setjmp` frame and the `longjmp` call must be able to unwind through it.
The rule:

1. Finds every function whose body contains a `setjmp` guard and rewrites it.
2. Finds every function whose body calls `longjmp` against one of those
   buffers (`rewrite_longjmp_calls_in_body`), including indirect calls through
   function pointers, and flips its ABI to `"C-unwind"`
   (`flip_unwind_abi_single_program`) — both on the Rust `fn` definition and
   on the generated `extern` declaration. An ordinary `extern "C"` boundary
   (the default) would `abort()` instead of unwinding through it, which is
   why the ABI flip is necessary rather than optional.
3. Follows calls through renamed/rewritten files (cross-TU renaming can move
   a `longjmp`-calling function into a different generated module path than
   its original name suggests), not just the original source file names.

## Known design tension

`longjmp`'s C semantics allow the jump buffer to be reused across multiple
`setjmp`/`longjmp` round-trips, and to be stored in a `malloc`'d array shared
across calls. The `catch_unwind`-based model handles the common single-shot
case correctly; a `malloc`'d-array-of-`jmp_buf` pattern combined with real
`vprintf`-style variadic forwarding was scoped as a known conflict area
rather than solved outright — check open beads before assuming full coverage
of unusual jump-buffer storage patterns.

## History

Built out over 2026-08-10 (see `wiki/log/2026-08-10-00-00.md`): started as a
single-function pattern, then extended to whole-program scope, indirect-call
visiting, and rewritten-file following.
