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

**Also considered and rejected: the `softfp` crate**
(docs.rs/softfp — pure-Rust IEEE 754 with explicit `RoundingMode` and
callback-based exception-flag reporting, built for RISC-V emulation).
Same conclusion as `arpfloat`, sharpened by `softfp`'s actual API: it
reports exceptions and reads the rounding mode via a
`softfp_set_exception_flags`-style *callback* the embedder supplies, not
a per-call return — so to keep its output visible to the rest of the real
C program (`fetestexcept()` etc.), the callback itself would still have
to call into libc's `fegetround()`/`feraiseexcept()`. That's not fewer
FFI calls than the C shim, it's more (a read call and a write call per
op, wrapping a slower software arithmetic routine, vs. one opaque call
doing real hardware arithmetic with exception flags set as a natural side
effect). `softfp` is the right tool for a different sub-problem:
`FENV_ROUND` pinning a *static*, compile-time-known mode (no
`fesetround()` involved at all) — there, a Rust constant threads straight
into `softfp`'s API with zero FFI and zero C-toolchain dependency, which
matters for freestanding/`no_std` targets that don't have a paired cross
C compiler the way every target Slate's differential testing already
requires one for. Not in scope for `slate-0ar5` (that's the dynamic
`FENV_ACCESS` case, where the shim wins outright), but worth remembering
if `FENV_ROUND`'s static-mode case ever gets its own subtask.

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

That's the five binary arithmetic ops, comparisons, and casts × {f32, f64}
as the *minimum* — see Tier 1b below for the much larger transcendental/
rounding-function surface that also turned out to need shimming. f80/f128
already route through the existing long-double/quad shim paths for
unrelated ABI reasons — threading `FenvAttr` through those is a much
smaller lift than building them was originally.

**Tier 1b — verified via `rustc --emit=llvm-ir`, not just "probably
opaque."** `CIRGenBuiltin.cpp:544-903` shows ~30 more constrained ops
(`acos`, `sin`, `cos`, `exp`, `log`, `pow`, `ceil`, `floor`, `fmod`,
`lround`, ...). The earlier version of this doc guessed most of these were
already opaque libm calls and only flagged `sqrt`/`fabs` as likely
exceptions — checked directly instead of assuming, and the real split is
much wider. Compiled a probe crate with each `f64` method at
`-O`/`--emit=llvm-ir` and read what call each one actually lowers to:

*Compiles to an LLVM intrinsic (foldable/reorderable, same hazard as
`+`/`*`, shimmed in `fenv.c`):* `.sin()` → `llvm.sin`, `.cos()` →
`llvm.cos`, `.exp()`/`.exp2()` → `llvm.exp`/`llvm.exp2`,
`.ln()`/`.log2()`/`.log10()` → `llvm.log*`, `.powf()` → `llvm.pow`,
`.ceil()`/`.floor()`/`.round()`/`.round_ties_even()`/`.trunc()` →
`llvm.ceil`/`llvm.floor`/`llvm.round`/`llvm.rint`/`llvm.trunc`,
`.sqrt()`/`.abs()` → `llvm.sqrt`/`llvm.fabs`, `.max()`/`.min()` →
`llvm.maximumnum`/`llvm.minimumnum`, `.copysign()` → `llvm.copysign`.
`Op::Fma` (`.mul_add()` → `llvm.fma`) is in this category too when
`FenvAttr` is set — same intrinsic, same foldability, distinct from the
target-dispatch fix in `slate-kmao` (which only concerns `Op::Fmuladd`'s
fuse-or-not decision, not this).

*Already a real opaque extern call, no shim needed:* `.asin()`, `.atan()`,
`.atan2()`, `.acos()`, `.acosh()`, `.asinh()`, `.atanh()`, `.cosh()`,
`.sinh()`, `.tanh()`, `.cbrt()`, `.exp_m1()`, `.ln_1p()`, `.hypot()`,
`.tan()`.

All of `fenv.c`'s Tier 1b additions (`sin`/`cos`/`exp`/`exp2`/`log`/`log2`/
`log10`/`pow`/`ceil`/`floor`/`round`/`rint`/`trunc`/`sqrt`/`fabs`/`fmax`/
`fmin`/`copysign`/`fma`, × {f32, f64}) are already written — see
`slate-0ar5.3`'s closure. Don't assume the "already opaque" list above is
complete either without spot-checking IR for anything new added later;
libm-calling behavior can vary by target/libc, and this list was itself
wrong once already.

## Containment strategy — dispatch per-op, never per-function/TU

A real risk with this design: if implemented carelessly, it could shim
*every* arithmetic op in a function just because `FENV_ACCESS ON` appears
somewhere in it — tainting ordinary, unrelated code with shim calls for a
pragma almost nothing uses, and actively hurting readability of the
generated Rust.

That risk doesn't require any cleverness on Slate's side to avoid, because
Clang already solves it upstream. Checked `CIRGenFPOptionsRAII`
(`CIRGenFunction.cpp:1376`): it constructs each op's `FenvAttr` from
`e->getFPFeaturesInEffect(langOpts)` — the FP-pragma state at that
*specific expression's* lexical position, as tracked by Sema's pragma
stack. So `FenvAttr` isn't a per-function or per-TU flag Clang hands
Slate — it's stamped independently on each individual `cir.fadd`/`cir.cast`/etc.
instruction, present only if that particular expression was lexically
inside an active `#pragma STDC FENV_ACCESS ON` region when Clang emitted
it. An add two lines outside the pragma's block gets no attr at all, same
op kind, same function.

This also happens to be the *correct* boundary, not just a convenient
one: `FENV_ACCESS`'s contract in the C standard is lexical, not
reachability-based. Code outside an active `FENV_ACCESS ON` block that
happens to run after an earlier `fesetround()` call at runtime gets no
promises from the standard either — relying on that is undefined
behavior, on the programmer, not the compiler. So matching Clang's
lexical decision exactly is both the minimal-taint choice and the
differential-correct one; they're the same thing here.

**Implementation rule for `slate-0ar5.4`:** each `Op::Fadd`/`Op::Fmul`/etc.
match arm must read `value.fenv.is_some()` off that specific op instance
and branch to shim-vs-native right there — the same shape
`Op::Fmuladd`/`type_mentions_long_double` dispatch already uses. Do
**not** thread a `self.constrained`-style bool through `FunctionLowerer`
for "we're inside a FENV_ACCESS region right now" — that would taint more
code than the real C reference binary does, breaking differential parity,
on top of reintroducing the exact readability regression this design
exists to avoid.

**Regression test to add in `slate-0ar5.4`/`.5`:** a fixture/probe with
`FENV_ACCESS ON` around exactly one `+` (or other Tier 1 op), with
ordinary arithmetic immediately before and after it in the same function,
outside the pragma's block. Inspect the generated Rust and confirm only
that one op became a shim call — the sibling ops stay plain native
operators. Direct, cheap proof the taint doesn't leak.

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
