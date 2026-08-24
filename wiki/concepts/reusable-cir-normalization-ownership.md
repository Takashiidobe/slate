# Reusable CIR normalization ownership

_created 2026-08-24_

`clang_ir::Toolchain` owns reusable `cir-opt` normalization of CIR text. Its
flag-capable entry point accepts caller-selected CIR passes and always appends
generic-form and debug-location printing.

Slate's `frontend::toolchain` module owns the preceding Clang invocation,
`SLATE_*` path/configuration policy, target and libc-shim arguments,
preprocessing queries, and the decision to request CFG flattening. CIR
emission, AST extraction, and preprocessing share that frontend-owned policy.
It constructs a clang-ir Toolchain with the resolved `SLATE_CIR_OPT` path and
passes only the required CIR transformation flags.

Do not add another `cir-opt` subprocess implementation in Slate.
