# `_Float16` and FENV-constrained floating point

Two related floating-point correctness areas: scope-limiting the `_FloatN`
family to what Clang actually implements, and shimming ops that need to
honor a dynamic FP environment (`#pragma STDC FENV_ACCESS`).

## `_Float16` scope

Of the full TS 18661-3 `_FloatN`/`_FloatNx` family, only `_Float16` is in
scope. Confirmed directly against the project's CIR-enabled Clang (not
assumed): `_Float32x`/`_Float64x`/`_Float128x` (GCC-only "extended" spellings)
and the plain `_Float32`/`_Float64`/`_Float128` are all rejected with
`unknown type name` — an upstream Clang gap, not a Slate gap. `_FloatN_t`
typedefs (glibc's `bits/floatn-common.h`) resolve for free through the
existing typedef fallback once the base type is supported, but the header
itself isn't in `libc-shim/` — a stdlib-header gap, not a lowering gap.

`_Float16` itself works end-to-end on x86_64: Clang emits `!cir.f16`
(`CirType::Fp16`, already parsed), arithmetic gets promoted to `float` before
`cir.fadd`/etc. so `!cir.f16` only shows up in alloca/load/store/signatures/
casts — a much smaller lowering surface than `long double`. Unlike `float`,
`_Float16` does **not** get promoted under C's default argument promotions
when passed to a variadic function (confirmed via probe) — any va_list/
variadic float-arg lowering path must accept f16 unpromoted. Rust nightly's
`f16` (same unstable track as the already-adopted `f128`) compiles clean as
an `extern "C"` parameter/return type — follow the existing `Feature::F128`
nightly-gate template (`Prim::F16` + `Feature::F16` gating
`#![feature(f16)]`), no wrapper struct needed (`long double` needs one only
because x87 80-bit has no native Rust representation).

**Bug found investigating this, independent of `_Float16` support**:
`CirType::Fp16` already existed as a parser variant (used for ARM `__fp16`
raw-bits contexts) but both `src/frontend/lowerer/types.rs`'s type-lowering
and `cir_type_to_ctype`'s fallback silently mapped it to a 32-bit int. Any
fixture hitting either path today would silently miscompile to `i32` rather
than erroring.

## TS 18661-3 library surface for `_Float16` (`slate-5exy.3.8`)

C23's floating-formatting family (`strfromd`/`strfromf`/`strfroml`) is done —
covered end-to-end by `tests/fixtures.library/c23_strfrom/` and the
`strfromf`/`strfromd`/`strfroml` calls in `tests/fixtures/c23_library.c`. The
remaining question was the wider TS 18661-3 surface: `strfromf16`/`strtof16`
and the ~20 `f16`-suffixed `<math.h>` functions (`sqrtf16`, `fabsf16`, etc.)
that would exist for `_Float16` alongside those.

**Confirmed unavailable, not just undeclared under strict flags.** Probed
directly against both this repo's `libc-shim/include` (nothing in
`stdlib.h`/`math.h` declares any `f16`-suffixed symbol — Slate has never
implemented this surface in its own shim) and the host's real glibc headers,
including with `__STDC_WANT_IEC_60559_TYPES_EXT__`/`_GNU_SOURCE` defined —
`strfromf16`/`strtof16`/`sqrtf16` all fail as undeclared regardless. Root
cause: `/usr/include/bits/floatn-common.h` hardcodes
`#define __HAVE_FLOAT16 0` unconditionally on this glibc — the gate every
one of these declarations sits behind (`__HAVE_FLOAT16 && __GLIBC_USE
(IEC_60559_TYPES_EXT)`) is permanently closed, independent of any feature-
test macro. `nm -D` on the installed `libc.so.6`/`libm.so.6` confirms it goes
deeper than headers: no `f16`-suffixed symbol is exported by either shared
object, so there is nothing to link a reference binary against even if a
declaration were forced. This is a toolchain/platform ceiling, not a Slate
gap — same shape as the `_Float32`/`_Float64`/`_Float128` rejection above,
one layer down (Clang accepts the *type*; glibc has no *library* for it).

**Disposition**: no differential fixture is possible for this surface on the
configured toolchain — there's no working C reference to diff against.
Nothing in the shim needed adding. Revisit only if the configured glibc ever
ships `__HAVE_FLOAT16 1` (tracked upstream as glibc bug 28457).

## Constrained floating point (`FENV_ACCESS`)

`docs/floating-point-environment.md` (or its wiki successor) covers current
state: `<fenv.h>` runtime calls already work; the pragma itself
(`FENV_ACCESS`/`FENV_ROUND`/`FENV_DEC_ROUND`) is hard-rejected at parse time.
`FP_CONTRACT` looks similar but isn't actually part of this problem — Clang's
CIRGen already resolves within-statement contraction into `cir.fmuladd` vs.
plain ops before Slate parses the CIR, and Slate already lowers both; it's
rejected today only because it's missing from the pragma allowlist next to
`CX_LIMITED_RANGE`, a much smaller separate fix.

**Why it's unsupported**: under these pragmas (or `-frounding-math`), Clang's
CIR builder attaches an optional `cir::FenvAttr` to affected ops via
`CIRBaseBuilder::getConstrainedFPAttr()`. Slate's parser never reads this
attribute, and the lowerer has no notion of "constrained" distinct from
ordinary — every handler translates the same way regardless. If the attr
were ever non-null and ignored, that's a silent miscompile (dynamic
rounding-mode/exception-flag semantics dropped), which is why the pragma is
hard-rejected rather than best-effort translated.

**Design: C shim, not soft-float.** The actual hazard `FENV_ACCESS` guards
against isn't rounding correctness (real hardware FP instructions already
consult the live MXCSR/FPU control word regardless of source language) — it's
the _optimizer_ being free to fold/reassociate/hoist float ops across code
that might change the environment, absent a barrier. A same-crate Rust
soft-float call (`arpfloat`, `softfp`) is visible/inlinable to LLVM and
doesn't inherently provide that barrier, and still needs
`fegetround()`/`feraiseexcept()` FFI calls to stay in sync with real libc
state — paying the FFI cost without gaining the barrier property. The chosen
design instead lowers each `FenvAttr`-marked op to a call into a small C
helper (`__slate_fenv_add_f64(a, b)`, mirroring the existing `__slate_f80_*`
long-double shim pattern): real hardware arithmetic, correct environment
handling for free, and the opaque `extern "C"` call is itself the required
compiler barrier.

Confirmed directly (`rustc --emit=llvm-ir` at `-O2`/`-O3`): an unannotated
extern function call blocks LLVM from reordering or folding neighboring float
ops across it in both directions, purely because LLVM infers no
`readnone`/`memory(none)` for an FFI declaration it can't see the body of —
`nounwind` only. No `#[inline(never)]` or other annotation needed.

**Op surface**: Tier 1 (needs a shim) is the five binary arithmetic ops
(`+ - * / %`), float comparisons, and float casts — everything Slate lowers
to a plain Rust operator, which the optimizer can see straight through. Tier
1b, found by compiling probe code and reading the emitted IR rather than
assuming: about 20 `f64`/`f32` methods (`sin`, `cos`, `exp`, `pow`, `ceil`,
`sqrt`, `fma`, ...) that compile to a foldable LLVM intrinsic and need the
same shim treatment; another dozen (`asin`, `atan2`, `cbrt`, `hypot`, ...)
already compile to a real opaque extern call and need no shim. `FNegOp`
never carries the attr (no exception/rounding dependence) — always skip it.
`FMAOp`/`FMulAddOp` are deliberately **excluded**: their fuse-or-not decision
is a static target-triple fact (`isFMAFasterThanFMulAndFAdd`), not runtime
environment state — that's a target-dispatch bug (`slate-kmao`), not a
`FENV_ACCESS` shimming concern, and shimming them anyway would be
unidiomatic overreach.

**Containment is lexical, per-op, and free.** `FenvAttr` isn't a per-function
or per-TU flag — Clang's `CIRGenFPOptionsRAII` stamps it independently on
each op from that specific expression's lexical position in the pragma
stack, matching the C standard's own lexical (not reachability-based)
contract for `FENV_ACCESS`. So the lowerer needs no `self.constrained`-style
state threaded through `FunctionLowerer` — just check `value.fenv.is_some()`
on each op instance and branch shim-vs-native right there, the same shape
`type_mentions_long_double` dispatch already uses. Threading ambient state
instead would taint more code than the real C reference binary does,
breaking differential parity for no benefit.
