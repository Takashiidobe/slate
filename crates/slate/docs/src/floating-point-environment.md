# floating-point env

`<fenv.h>` itself works: `libc-shim/include/fenv.h` declares
`fegetround`/`fesetround`/`fegetenv`/`fesetenv`/`feclearexcept`/... with the
right ABI-matching `fenv_t`/`fexcept_t` layouts per target
(`libc-shim/bits/<arch>/fenv.h`), and calls to them lower like any other
extern C call, resolved against the real target libc at link time. A C
program that calls `fesetround(FE_DOWNWARD)` and does math afterward
translates and runs correctly.

What Slate does *not* support is a family of `#pragma STDC` directives that
exist to tell the *compiler* how it's allowed to treat
floating-point-environment-observing code:

```c
#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF
#pragma STDC FENV_ROUND FE_TONEAREST
#pragma STDC FENV_DEC_ROUND FE_DEC_TONEAREST
```

`preprocess.rs`'s `DirectiveRecord::disposition` currently classifies all
four as `UnsupportedSemantic`, so `directive_translate.rs`/`cross_tu.rs`
reject a translation unit outright if any of them is active (see the
`pragma_inventory.c` and `unsupported_directive` fixtures in `tests/`). But
they aren't all unsupported for the same reason - `FP_CONTRACT` turns out to
be mostly a missing allowlist entry, while the other three reflect a real
gap. See below.

## `FP_CONTRACT` is (mostly) not the problem

It's tempting to assume `FP_CONTRACT` needs special lowering support because
it's "the fma pragma" and CIR/Slate already have fma handling
(`cir.fma`/`cir.fmuladd` -> `Op::Fma`/`Op::Fmuladd` -> `.mul_add()` in
`lowerer.rs`). That's true, but it's also basically already handled: Clang's
CIRGen resolves within-statement contraction *before* Slate ever sees CIR.
`CIRGenExprScalar.cpp`'s `tryEmitFMulAdd` checks
`op.fpFeatures.allowFPContractWithinStatement()` (which reflects the
pragma/`-ffp-contract` state at that point in the source) and emits a single
`cir.fmuladd` when contraction is legal, or separate `cir.fmul`+`cir.fadd`
when it isn't. By the time Slate parses the CIR, the pragma has already done
its job, exactly like `CX_LIMITED_RANGE` below - there's no leftover scoped
state to track. So `#pragma STDC FP_CONTRACT` is currently rejected as an
unsupported directive mostly because it isn't allowlisted next to
`CX_LIMITED_RANGE` yet, not because Slate lacks the machinery to honor it.

## `FENV_ACCESS`/`FENV_ROUND`/`FENV_DEC_ROUND` are the real problem

These are a different kind of pragma: rather than baking a single either/or
choice into which CIR op gets emitted, they mark the *individual ops
themselves* as needing dynamic-rounding/exception-observing ("constrained")
semantics. CIR represents this with an optional `cir::FenvAttr` that
`CIRGenExprScalar.cpp` attaches to casts, `cir.fmuladd`, and other
floating-point ops via `getConstrainedFPAttr()` whenever the surrounding
code is under `FENV_ACCESS ON` (or, for C23, a non-default `FENV_ROUND`).
An op with that attribute set can't be freely reordered, constant-folded,
or assumed to round under the "current" static mode, because the dynamic
floating-point environment can change between it and neighboring code (e.g.
via `fesetround`).

Slate's CIR parser (`src/cir/`) never reads a `fenv`/`FenvAttr` field at
all, and the lowerer has no notion of a "constrained" op distinct from a
normal one - every `cir.*` handler translates an operation to Rust the same
way regardless of this attribute. So if `FENV_ACCESS ON` is active and CIR
attaches a real (non-null) `FenvAttr` to an op, Slate would silently drop
it and lower to ordinary Rust arithmetic that doesn't observe rounding-mode
changes the way the source contract promised. Doing that silently would be
exactly the kind of miscompile differential testing exists to catch, so
these three pragmas are hard rejections instead: `preprocess.rs`'s
`DirectiveRecord::disposition` classifies them as `UnsupportedSemantic`,
and `directive_translate.rs`/`cross_tu.rs` reject a translation unit
outright if one is active (see the `pragma_inventory.c` and
`unsupported_directive` fixtures in `tests/`).

## Why `CX_LIMITED_RANGE` is accepted

`#pragma STDC CX_LIMITED_RANGE` (`is_cx_limited_range_pragma` in
`preprocess.rs`) *is* accepted, for the same reason `FP_CONTRACT` mostly
isn't a problem: Clang fully resolves its effect before Slate ever sees
CIR. It only controls whether complex multiply/divide get the naive formula
or the range-safe (Smith's algorithm) expansion, and that choice is baked
into the CIR ops Clang emits. There's no remaining scoped state for Slate to
track - by the time the CIR arrives, the pragma has already done its job.
