# floating-point env

In general, manipulating the FP env should work, e.g.

```c
#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF
#pragma STDC FENV_ROUND FE_TONEAREST
#pragma STDC FENV_DEC_ROUND FE_DEC_TONEAREST
```

## `FP_CONTRACT`/`CX_LIMITED_RANGE`

`FP_CONTRACT` and `CX_LIMITED_RANGE` don't need any special handling;
The CIR will lower to `cir.fma/cir.fmuladd` which the lowerer can then
match and handle.

## `FENV_ACCESS`/`FENV_ROUND`/`FENV_DEC_ROUND`

For these, the attribute is attached for FP ops in the `cir::FenvAttr`.
Slate can read this attribute and choose what to do based on the attr.

## How it's done

The first thought was to use a soft float crate, like `softfp`. However,
the soft float crates need to disable floating point optimizations
globally, which they don't do, so they work for the fenv problem if all
calls to the fenv are static.

Instead, Slate ships another shim (`__slate_fenv_*`) that calls out to
the relevant op in C. That way, calls to manipulate the environment work
as necessary, and then any op that are changed by the environment are
lowered to shim calls, so other calls aren't tainted.
