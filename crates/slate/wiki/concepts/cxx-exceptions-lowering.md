# C++ exceptions lowering

> Scoping doc, not implemented. Companion to
> [cxx-translation-pain-points.md](cxx-translation-pain-points.md) (the "RAII
>
> - exceptions have no direct Rust control-flow shape" section originates
>   this doc) and [setjmp-longjmp-lowering.md](setjmp-longjmp-lowering.md),
>   which already treats a related unwind-shaped C construct the same way.

## Correction to prior scoping: CIR already models this in detail

`cxx-translation-pain-points.md`'s header claims "CIR doesn't support C++
yet." For exceptions specifically that's stale — verified empirically on the
current build (`~/llvm-project/build-cir/bin/clang -std=c++17 -emit-cir`, no
`-Xclang -fclangir` needed) with a `throw std::runtime_error(...)` inside an
`if`, caught by a `try`/`catch (const std::runtime_error&)` in `main`. CIR
emits full structured EH, not a punt to opaque LLVM landingpad tables.

## What `-emit-cir` produces

Throw site:

```
%9 = cir.alloc.exception 16 -> !cir.ptr<!rec_std3A3Aruntime_error>
%10 = cir.get_global @".str" : ...
%11 = cir.cast array_to_ptrdecay %10 : ... -> !cir.ptr<!s8i>
cir.call @_ZNSt13runtime_errorC1EPKc(%9, %11) : (...) -> ()
cir.throw %9 : !cir.ptr<!rec_std3A3Aruntime_error>, @_ZTISt13runtime_error, @_ZNSt13runtime_errorD1Ev
cir.unreachable
```

- `cir.alloc.exception` is `__cxa_allocate_exception` — heap allocation sized
  to the thrown type.
- The ctor call constructs the object in the allocated storage, same as any
  ordinary `cir.call` to a constructor.
- `cir.throw` carries **three** things: the object pointer, the RTTI
  type-info global (`@_ZTISt13runtime_error`), and the destructor to run once
  the exception is caught (`@_ZNSt13runtime_errorD1Ev`). Both the type
  identity and the cleanup obligation are explicit operands, not something
  that has to be re-derived from the type later.

Try/catch site:

```
cir.try {
  <body, e.g. the risky() call and its cir.return>
^bb1:  // no predecessors
  cir.yield
} catch [type #cir.global_view<@_ZTISt13runtime_error> : !cir.ptr<!u8i>] (%arg0: !cir.eh_token) {
  %catch_token, %exn_ptr = cir.begin_catch %arg0 : !cir.eh_token -> (!cir.catch_token, !cir.ptr<!void>)
  cir.cleanup.scope {
    cir.init_catch_param reference %exn_ptr to %3 : ...
    <catch body>
  } cleanup all {
    cir.end_catch %catch_token : !cir.catch_token
    cir.yield
  }
  cir.yield
} unwind (%arg0: !cir.eh_token) {
  cir.resume %arg0 : !cir.eh_token
}
```

- Catch clauses are a list, each tagged with an RTTI `#cir.global_view`,
  matched in source order — multiple `catch` blocks on one `try` show up as
  multiple `catch (type name) { ... }`-shaped regions on the same `cir.try`.
- `unwind` is the "no clause matched" landing pad: exactly a rethrow,
  `cir.resume` on the same `!cir.eh_token` the catch regions took as an
  argument.
- Any function containing a `try` gets `personality(@__gxx_personality_v0)`
  attached to its `cir.func`.

Everything needed for translation — allocation, RTTI-keyed dispatch order,
per-catch destructor, rethrow — is already a structured MLIR region, not
something that has to be reconstructed from raw unwind tables.

## Raw-lowering target: `panic_any` / `catch_unwind`, not signals or a manual state machine

Same design choice already made for `setjmp`/`longjmp`
([setjmp-longjmp-lowering.md](setjmp-longjmp-lowering.md)): map the
unwind-shaped source construct onto Rust's own unwind mechanism, not a
hand-rolled reimplementation.

| CIR op                                                                                         | Rust raw-lowering target                                                                                                                                                                                                      |
| ---------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `cir.alloc.exception` + ctor call + `cir.throw`                                                | construct the payload value, `std::panic::panic_any(payload)`                                                                                                                                                                 |
| `cir.try { A } catch T1(...) {B1} catch T2(...) {B2} unwind {resume}`                          | `match std::panic::catch_unwind(AssertUnwindSafe(\|\| A)) { Ok(v) => v, Err(p) => if let Some(e) = p.downcast_ref::<T1>() { B1 } else if let Some(e) = p.downcast_ref::<T2>() { B2 } else { std::panic::resume_unwind(p) } }` |
| `cir.resume` (unwind clause, no match)                                                         | `std::panic::resume_unwind(payload)` — direct 1:1                                                                                                                                                                             |
| destructor named in `cir.throw`'s third operand                                                | free: the payload is a real Rust value, `Drop` fires on unwind automatically, no fixup needed                                                                                                                                 |
| locals live across the `try` body at an unwind point (`cir.scope`/`cir.cleanup.scope` nesting) | ordinary Rust `Drop` on those locals, same as any other scope exit — no special-cased cleanup codegen needed as long as they're real owned Rust values                                                                        |

Catch clause order matters and is preserved directly: CIR's clause list is
already in source order, so the `downcast_ref` chain just walks it in the
same order.

**Known gap**: catching by base class needs the C++ class hierarchy (from
AST, not CIR — CIR only gives you the concrete thrown type's RTTI symbol) to
expand into the set of concrete leaf types that could match, since Rust's
`Any::downcast_ref` is exact-type-only with no upcast-matching equivalent.
Same structural gap as the multiple-inheritance section of
[cxx-translation-pain-points.md](cxx-translation-pain-points.md) — no
fixup-pass trick recovers RTTI-style hierarchy matching from a `downcast_ref`
chain; it has to be expanded at translation time from the known hierarchy.

## Planned follow-up: rewrite to `Result`, gated on raw lowering compiling first

The `panic_any`/`catch_unwind` mapping above is scoped as the **raw
lowering** target — mirrors the project convention (transliterate first,
idiomatize later; compare `translate-lowered` vs `SetjmpRecovery` for
setjmp/longjmp). It is not the end state.

The intended idiomization fixup is a whole-program rewrite from panics to
typed `Result` propagation: functions on the throw path get an `-> Result<T,
E>` signature, call sites get `?`, and `catch` arms become `match`/`if let`
on the `Result`'s `Err` variant instead of a panic downcast. This is a real
restructuring pass (signature changes ripple through every caller on the
path, same shape as `SetjmpRecovery`'s whole-program ABI-flip walk, but
bigger — return type changes instead of an ABI attribute flip), not a
per-function lowering handler.

**Sequencing constraint, stated explicitly by design**: do not start the
`Result` rewrite until the raw `panic_any`/`catch_unwind` lowering above is
implemented and differentially passing its own fixtures. Reasons:

1. The `Result` rewrite is a fixup pass over already-lowered Rust, the same
   relationship every other fixup in this codebase has to raw lowering (see
   [fixups.md](../historical/fixups.md), [writing-a-query-driven-fixup.md](../historical/writing-a-query-driven-fixup.md))
   — it needs a correct, compiling raw form to rewrite _from_. Without that,
   there's no differential baseline to prove the rewrite preserves behavior
   against.
2. `SetjmpRecovery` is the direct precedent for exactly this ordering: raw
   lowering (plain `extern "C"` calls) landed and was verified
   optimization-unsafe-but-correct at `-O0` before the `catch_unwind` rewrite
   was built on top of it. Exceptions should follow the same order — get
   `panic_any`/`catch_unwind` fixtures green first, then rewrite.
3. Catch-by-base-class (the known gap above) and multi-clause ordering are
   easier to get right once there's a working panic-based reference
   translation to diff the `Result`-based one against for the same fixture.

## Open questions (not yet resolved)

- Whether `panic_any`'s `Box<dyn Any + Send>` payload type is precise enough,
  or whether a project-specific `enum ExceptionPayload { .. }` per
  translation unit (one variant per distinct thrown type) gives better
  `downcast_ref` ergonomics and lets the `Result` rewrite reuse the same enum
  as its `Err` type — this would make the two passes share a payload
  representation instead of needing a second one invented later.
- Whether `noexcept` functions should get any special treatment (C++
  `noexcept` violation calls `std::terminate`; Rust panicking across a
  `-C panic=abort` boundary already aborts, so this may be a free mapping,
  unverified).
- Stack-unwind-across-multiple-frames destructor ordering hasn't been tested
  end-to-end yet (only the single-frame `try`/`catch` in the same function
  shown above) — needs a fixture where the `throw` and `catch` are in
  different functions with locals to destruct in between.
