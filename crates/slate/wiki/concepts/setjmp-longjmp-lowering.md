# setjmp/longjmp lowering

> See [fixups.md](../historical/fixups.md) for the general query-driven rewrite interface
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

## Raw lowering is optimization-unsafe; `catch_unwind` is what makes it safe

`translate-lowered` (no rewrite pass) emits a literal `unsafe extern "C" {
fn setjmp(...) -> i32; }` and calls it directly. rustc has no way to mark
that declaration `returns_twice` (see below), so LLVM's optimizer is free to
treat the call as ordinary single-return control flow.

Verified empirically (`-O0` vs `--release` on the raw-lowered output of a
`setjmp`/`longjmp` fixture, C reference pinned at `-O0` to match
`tests/support/mod.rs`):

- A non-`volatile` local mutated between `setjmp` and the `longjmp` that
  reenters it: `clang -O0` and Rust debug both print `r=99 x=5`; `clang -O2`
  **and** Rust `--release` both print `r=99 x=0`. This is inherited C UB
  (mutating a non-`volatile` automatic between `setjmp`/`longjmp` is
  unspecified per the standard, independent of `returns_twice`), so it isn't
  a Rust-specific gap — but it means the raw-lowering path silently depends
  on the C source avoiding this extremely common real-world idiom, or on the
  Rust side never being built past `opt-level=0`.
- Same code with `volatile int x`: correct (`r=99 x=5`) in C at any
  optimization level, and slate already lowers `volatile` to
  `write_volatile`/`read_volatile`, which stays correct in `--release` too.
  This isolates the actual boundary: raw lowering is only exposed to the
  missing-`returns_twice` risk for the volatile-less idiom.

Practical upshot: the `SetjmpRecovery` rewrite (`catch_unwind`/`panic!`) isn't
just idiomization here — it's what makes the translation optimization-safe
at all, since unwind-based control flow is a construct LLVM's EH machinery
understands natively, with no `returns_twice` loophole required. The
differential harness can't currently catch this class of regression because
it always compiles both sides unoptimized (`tests/support/mod.rs` pins C at
`-O0`; Rust goes through plain `cargo build`, i.e. debug).

## `returns_twice` cannot be attached to an extern declaration from Rust

`#[ffi_returns_twice]` existed at one point specifically to emit LLVM's
`returns_twice` function attribute on a foreign `setjmp` declaration, and was
removed. There is no way to reach the same effect today:

- `#![feature(link_llvm_intrinsics)]` does not apply: `returns_twice` is a
  function **attribute** (like `noreturn`/`cold`), not a callable LLVM
  **intrinsic**. There is no `llvm.returns_twice` symbol to `#[link_name]`
  against — intrinsic-linking only reaches things with real `llvm.*`
  callable semantics (`llvm.prefetch`, `llvm.memcpy.*`, etc.).
- The general mechanism `ffi_returns_twice` was built on
  (`#[rustc_attrs]`-family internal attributes) is not usable from ordinary
  crates on current nightly either — the `internal_features` lint denies it
  outside the compiler/std's own sysroot.
- The only workaround that actually sidesteps the problem: don't let the
  reentrant call exist in Rust-generated IR at all. Push `setjmp` into a
  small hand-written C shim (compiled by a compiler that emits the attribute
  correctly) that itself resolves the branch and returns a single value; Rust
  calls the shim as an ordinary single-return `extern "C"` function. This
  only works when the code between `setjmp` and `longjmp` can be expressed as
  a callback invoked from inside that same C frame — i.e. it has the same
  "single dynamic extent" restriction the `catch_unwind` design already
  accepts, not a way to expose raw reentrant `setjmp` semantics to arbitrary
  Rust call sites.

## `__builtin_setjmp` matches the same rewrite via shape relaxation, not a new rule

`__builtin_setjmp`/`__builtin_longjmp` raw-lower to plain `setjmp(env)`/
`longjmp(env, 1)` calls (`lower_eh_longjmp` alongside the existing setjmp
stub, both in the lowerer), specifically so the one `SetjmpRecovery` matcher
above recognizes them like ordinary library calls rather than needing a
second rewrite rule. Two matcher gaps had to be closed in
`setjmp_recovery.rs` for the raw-lowered shape to actually match:

- `setjmp_guard_shape` originally required exactly `[let, if]` in the guard
  scope; `__builtin_setjmp`'s lowering can prepend `buf[0]`/`buf[2]`
  frame/stack stores, and can hoist the bool-cast of the setjmp result into
  its own temp instead of inlining it into the `if` condition. Relaxed to
  allow an arbitrary statement prefix plus either 2- or 3-statement tail
  shape.
- Buffer identity was matched by local temp variable name, which differs per
  function — `setjmp` in one function and `longjmp` in another (e.g. a
  `sub2` helper) never resolved to the same buffer. `resolve_buffer_root`
  chases addr-of chains back through let-bindings to the underlying global
  name so both sides agree on buffer identity.

Not covered: `pr64242`/`pr84521` (gcc-torture) pass their jump buffer through
an opaque `void*` function parameter with no source-level identity to
resolve, and specifically exercise raw stack-pointer corruption semantics
that the `catch_unwind` model can't represent at all. Left open rather than
force-fit.

`src/backend/mod.rs::apply_with_logger` had all ~80 fixup passes commented
out since 2026-08-14 (repo-wide no-op, unrelated to setjmp specifically) —
`Pass::SetjmpRecovery` was selectively re-enabled to unblock this fix. Check
current state of that list before assuming any other pass is actually
running.

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
