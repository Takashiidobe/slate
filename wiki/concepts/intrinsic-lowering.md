# Intrinsic lowering

<!-- toc -->
- [Catalog](#catalog)
- [Frontend](#frontend)
- [Limits](#limits)
<!-- /toc -->

## Catalog

| Input | Generated facts |
| --- | --- |
| LLVM C++ intrinsic API | LLVM names, signatures, immediate parameters, overload positions |
| `llvm-tblgen --dump-json` over `Intrinsics.td` | C builtin aliases |
| Optional stdarch sources | Internal x86/x86_64 LLVM link-name signature overrides |

- Generator: [`slate-intrinsic-gen`](../../crates/slate-intrinsic-gen/README.md).
- Checked-in catalog: `crates/slate/src/frontend/lowerer/intrinsics_table.rs`.
- General, x86, AArch64, ARM, and RISC-V sections; LLVM commit stored in the catalog.
- LLVM and C++ dependencies apply only to regeneration.

## Frontend

- `frontend/lowerer/intrinsics.rs` matches declarations carrying `c_builtin`
  metadata, with no body, against general and current-target entries.
- Calls emit `extern "llvm-intrinsic"` declarations with LLVM `link_name` and
  the `link_llvm_intrinsics` feature gate. No ordinary C extern is emitted
  for a mapped builtin.
- Non-overloaded scalar signatures: exact integer widths, bool, f32/f64, void.
- Integer signedness adapts through Rust casts; integer bits stay unchanged.
- Declaration parameters come from the catalog; call arguments and result
  types must match it before emission.
- Fixture: `x86_scalar_intrinsics.c` covers flags, pause, fences, and timestamp
  reads, including unsigned argument/result conversion and repeated calls.

## Limits

- Overloads, vectors, pointers, aggregates, and immediate arguments require
  further adaptation; mapped calls produce source-located lowering barriers.
- Intrinsic function addresses cannot lower as ordinary C function pointers.
- TableGen aliases cover direct builtin mappings; builtins Clang expands by
  hand need separate lowering.
- Stdarch overrides are retained for later ABI adaptation; the scalar path
  uses LLVM signatures directly.
