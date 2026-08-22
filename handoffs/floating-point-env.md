# Handoff: shimming FENV_ACCESS-constrained float ops to C

Tracked by `slate-0ar5`.

## Prerequisite reading

`docs/src/floating-point-environment.md` — the doc this session wrote,
covering the current state (runtime `<fenv.h>` works, the four pragmas are
hard-rejected) and why `FP_CONTRACT`/`CX_LIMITED_RANGE` are *not* part of
this problem (Clang already resolves them into plain CIR ops before Slate
sees them). `docs/src/long-double.md` — the shim pattern this handoff wants
to reuse (`__slate_f80_*` C helpers, opaque `extern "C"` calls carrying real
hardware semantics that a `[u8; 10]` Rust struct can't).

## Current state

Nothing has been implemented yet. `src/frontend/preprocess.rs`'s
`DirectiveRecord::disposition` classifies `#pragma STDC FENV_ACCESS`,
`FP_CONTRACT`, `FENV_ROUND`, and `FENV_DEC_ROUND` all as
`UnsupportedSemantic` (only `CX_LIMITED_RANGE` is allowlisted via
`is_cx_limited_range_pragma`), and `directive_translate.rs`/`cross_tu.rs`
reject any translation unit where one is active
(`tests/fixtures.cfg/reject/pragma_inventory.c`,
`tests/fixtures.multi.reject/unsupported_directive/`,
`tests/directive_translate.rs:480`, `tests/cross_tu.rs:79`).

This session determined `FP_CONTRACT` doesn't actually belong in that list
architecturally — Clang's CIRGen (`CIRGenExprScalar.cpp`'s `tryEmitFMulAdd`)
already resolves within-statement contraction into `cir.fmuladd` vs. plain
`cir.fmul`+`cir.fadd` before Slate ever parses the CIR, and Slate already
lowers both (`Op::Fma`/`Op::Fmuladd` → `.mul_add()`,
`lowerer.rs:4808-4823`). It's rejected today only because it isn't
allowlisted next to `CX_LIMITED_RANGE` — a much smaller, separate fix from
what's described below, and not what `slate-0ar5` is about.

`FENV_ACCESS`/`FENV_ROUND`/`FENV_DEC_ROUND` are the real gap this handoff
covers.

## Why this is unsupported: the CIR mechanism

Grepped the local `llvm-project` checkout
(`~/llvm-project/clang/lib/CIR/CodeGen/`). Under these pragmas (or
`-frounding-math`), Clang's CIR builder attaches an optional
`cir::FenvAttr` ("constrained floating point") to the ops it emits, via
`CIRBaseBuilder::getConstrainedFPAttr()`
(`clang/include/clang/CIR/Dialect/Builder/CIRBaseBuilder.h:252`). That
function returns a null attr when constrained FP isn't active — the
existing default — and a real attr (carrying the assumed rounding mode)
when it is.

Slate's `src/cir/` parser never reads a `fenv`/`FenvAttr` field at all, and
the lowerer has no notion of a "constrained" op distinct from an ordinary
one — every `cir.*` handler translates the same way regardless. So if this
attribute were ever non-null, Slate would silently drop it and lower to
plain Rust arithmetic that doesn't honor dynamic rounding-mode changes
(`fesetround`) or exception-flag semantics — a silent miscompile, which is
why the pragma is hard-rejected today rather than best-effort translated.

## The design: shim, not softfloat

Two options were discussed and compared:

1. **Soft-float in Rust** (e.g. the `arpfloat` crate, which has explicit
   `RoundingMode` support). Rejected: it doesn't solve the actual hazard.
   Real hardware FP instructions already consult the live MXCSR/FPU control
   word at execution time regardless of source language — an unoptimized
   `a + b` in Rust already rounds correctly under a `fesetround()` change.
   The actual thing `FENV_ACCESS` guards against is the *optimizer*:
   without a barrier, LLVM is free to constant-fold, reassociate, CSE, or
   hoist float ops across code that might change the environment. A
   same-crate Rust function call (`arpfloat::add(...)`) is visible/inlinable
   to LLVM and doesn't inherently prevent that unless force-`#[inline(never)]`'d
   and separately audited. It also doesn't avoid FFI anyway — you'd still
   need `fegetround()` before each op and `feraiseexcept()` after to keep
   real libc-visible exception state in sync, so it pays the FFI cost
   without gaining the barrier property.

2. **C shim, long-double pattern** (recommended). For every op CIR marks
   with a non-null `FenvAttr`, lower to a call into a small C helper
   (`__slate_fenv_add_f64(a, b)`, etc.) compiled alongside the generated
   Rust and linked via `extern "C"`, mirroring `__slate_f80_*`. The helper
   is just `return a + b;` in real C: real hardware arithmetic, so it
   inherits the ambient rounding mode and exception flags correctly for
   free. The opaque call is *also* exactly the compiler barrier
   `FENV_ACCESS` requires — rustc/LLVM can't fold or reorder across a call
   it can't see into. One nice simplification: `FenvAttr`'s rounding-mode
   field only matters when `FENV_ROUND` pins a static mode; under plain
   `FENV_ACCESS ON` it's left dynamic, so the shim doesn't need a mode
   parameter at all — it just needs to exist as an opaque call.

## Scoped op surface

Grepped every call site of `getConstrainedFPAttr()` in
`CIRGenExprScalar.cpp`, `CIRGenBuiltin.cpp`, and `CIRGenBuilder.h`.

**Tier 1 — needs a new shim.** Ops Slate currently lowers to native Rust
operators or an LLVM intrinsic the optimizer can see straight through:

| CIR op | Current Rust lowering | Why exposed |
|---|---|---|
| `FAddOp`/`FSubOp`/`FMulOp`/`FDivOp`/`FRemOp` | `+ - * / %` | plain operators |
| `CmpOp`/`VecCmpOp` (float operands) | `< > <= >= == !=` | no exception notion in Rust compares |
| `CastOp` (`bool_to_float`, `int_to_float`, `float_to_int`, `float_to_bool`, float-extend/trunc) | `as` | same |

**`FMAOp`/`FMulAddOp` deliberately excluded from this shim.** They do
carry `FenvAttr` under `FENV_ACCESS`, but shimming them isn't the fix —
during `slate-0ar5.2`'s verification a *separate, static-target* bug was
found (`slate-kmao`): `Op::Fmuladd` (implicit contraction, from
`cir.fmuladd`/`llvm.fmuladd`, "may fuse") always lowers to `.mul_add()`
(`llvm.fma`, "must fuse"), which is only correct on targets that
unconditionally have hardware FMA. Whether `llvm.fmuladd` legalizes to a
fused instruction is a static fact of the target triple/features
(`isFMAFasterThanFMulAndFAdd` per backend), not a dynamic runtime fact the
way `FENV_ACCESS`'s rounding-mode state is — so the fix is target dispatch
in the lowerer (same shape as `is_quad_long_double` in the long-double
path), not a C shim. Shimming every f64 arithmetic op out to C for what's
really a compile-time-resolvable target question is unidiomatic and costs
real readability for consumers of the generated Rust — it also fights
future idiomization/rewrite passes that would otherwise recover plain
Rust operators. Reserve the shim for ops whose correctness genuinely
depends on runtime environment state that Rust has no way to express
natively (add/sub/mul/div/rem/casts/compares under `FENV_ACCESS`) — that's
the actual dividing line, not "which ops CIR happens to attach `FenvAttr`
to." See `slate-kmao` for the target-dispatch fix and the per-arch
legality table. Once that lands, `FMAOp`/`FMulAddOp` under `FENV_ACCESS`
specifically (both contraction *and* a dynamic environment active) is a
compounding edge case worth a follow-up look, not solved by either fix
alone.

`FNegOp` explicitly never carries the attr
(`CIRBaseBuilder.h`: "fneg does not raise FP exceptions or depend on the
rounding mode") — skip it.

That's **7 op-shapes × {f32, f64}** as the true minimum (~14 functions plus
comparison predicates), since f80/f128 already route through the existing
long-double/quad shim paths for unrelated ABI reasons — threading `FenvAttr`
through those is a much smaller lift than building them was originally.

**Tier 2 — probably already opaque, verify before adding work.**
`CIRGenBuiltin.cpp:544-903` shows ~30 more constrained ops (`acos`, `sin`,
`cos`, `exp`, `log`, `pow`, `ceil`, `floor`, `fmod`, `lround`, ...). Slate
already lowers most to Rust `f32`/`f64` std methods
(`lowerer.rs:4476-4565`: `Op::Sin` → `.sin()`, etc.), and on most targets
those std methods call into the real system libm — already an opaque
extern call with the same barrier property, no shim needed. Exceptions to
check: `sqrt` and `fabs` are typically compiled as direct hardware
intrinsics (`llvm.sqrt.f64`/`llvm.fabs.f64`) rather than libm calls, so LLVM
can constant-fold them the same way it folds native operators — these two
probably need to move to Tier 1. Don't assume the rest of Tier 2 is fine
without spot-checking generated IR; libm-calling behavior can vary by
target/libc.

## Open question — resolved (`slate-0ar5.1`)

The whole design rested on: **does an unannotated `extern "C"` call in the
generated Rust actually block rustc/LLVM from reordering or folding
neighboring float ops across it, in both directions, at `-O2`/`-O3`?**
Confirmed yes.

Reproducer (`rustc --edition 2021 --crate-type lib -C opt-level=3 --emit=llvm-ir`):

```rust
extern "C" {
    fn opaque_add(a: f64, b: f64) -> f64;
}

#[no_mangle]
pub fn probe2(w: f64, x: f64, y: f64, z: f64) -> f64 {
    let indep_before = w + x;
    let called = unsafe { opaque_add(y, z) };
    let indep_after = w * x;          // no data dependency on the call at all
    called + indep_before + indep_after
}
```

At both `opt-level=2` and `opt-level=3`, the emitted IR preserves source
order exactly:

```llvm
%indep_before = fadd double %w, %x
%called = tail call noundef double @opaque_add(double noundef %y, double noundef %z) #1
%indep_after = fmul double %w, %x
...
declare noundef double @opaque_add(double noundef, double noundef) unnamed_addr #0
attributes #1 = { nounwind }
```

The key detail is the attribute set on the declaration: `nounwind` only —
rustc/LLVM never infers `readnone`/`readonly`/`memory(none)` for an FFI
declaration it can't see the body of, so the call is treated as having
unknown side effects and acts as a full ordering barrier. This held even
for `indep_after`, which has zero data dependency on the call's arguments
or result — proving it's not just data-dependency-driven scheduling, LLVM
genuinely won't hoist/sink *anything* across an opaque call by default.

This confirms the shim design is sound: a plain `extern "C"` extern
function declaration, with no special annotations, is enough. No need for
`#[inline(never)]`, no need to worry about LTO seeing through it (Slate
doesn't do cross-language LTO), and no need for anything Rust-crate-local
like the rejected `arpfloat` approach would have required.

## Subtasks (`bd show slate-0ar5` for the full tree)

1. `slate-0ar5.1` — ~~Resolve the open question above~~ **done**, see above.
2. `slate-0ar5.2` — Land the unrelated, much smaller `FP_CONTRACT` allowlist
   fix separately (add `is_fp_contract_pragma` alongside
   `is_cx_limited_range_pragma` in `preprocess.rs`) — no shim work needed,
   just confirm `Op::Fma`/`Op::Fmuladd` already do the right thing (they
   appear to). No dependency on the rest of this chain — ready now.
3. `slate-0ar5.3` — Write the C shim helpers for the Tier 1 op-shapes,
   following `__slate_f80_*`'s structure in the long-double lowering path.
4. `slate-0ar5.4` — Add `FenvAttr` parsing to `src/cir/` so the lowerer can
   detect which ops need the shim instead of the native Rust operator.
5. `slate-0ar5.5` — Allowlist `FENV_ACCESS`/`FENV_ROUND`/`FENV_DEC_ROUND` in
   `preprocess.rs` once the shim path exists, with fixtures exercising a
   `fesetround()` change straddling each Tier 1 op kind.
6. `slate-0ar5.6` — Revisit Tier 2's `sqrt`/`fabs` exception once the rest
   lands.
