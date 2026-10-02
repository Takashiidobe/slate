# Intrinsic lowering

<!-- toc -->
- [Catalog](#catalog)
- [Frontend](#frontend)
- [Legacy port](#legacy-port)
- [Limits](#limits)
<!-- /toc -->

## Catalog

| Input | Generated facts |
| --- | --- |
| LLVM C++ intrinsic API | LLVM names, signatures, immediate parameters, overload positions |
| `llvm-tblgen --dump-json` over `Intrinsics.td` | C builtin aliases and overloaded immediate parameters |
| Optional stdarch sources | Internal x86/x86_64 LLVM link-name signature overrides |

- Generator: [`slate-intrinsic-gen`](../../crates/slate-intrinsic-gen/README.md).
- Checked-in catalog: `crates/slate/src/frontend/lowerer/intrinsics_table.rs`.
- General, x86, AArch64, ARM, and RISC-V sections; LLVM commit stored in the catalog.
- LLVM and C++ dependencies apply only to regeneration.
- Supported IIT templates retain overload constraints and references:
  `overload:index:vector-constraint:element-constraint`, `match:index`.
- Unsupported IIT shapes keep absent signatures and produce barriers.

## Frontend

- `frontend/lowerer/intrinsics.rs` matches declarations carrying `c_builtin`
  metadata, with no body, against general and current-target entries.
- Calls emit `extern "llvm-intrinsic"` declarations with LLVM `link_name` and
  the `link_llvm_intrinsics` feature gate. No ordinary C extern is emitted
  for a mapped builtin.
- Uniquely matching stdarch signatures take precedence over LLVM signatures.
- Overload positions select call-site types; mangling ignores integer signedness,
  uses vector lane counts, and uses `p0` for ordinary pointers.
- Declarations deduplicate by LLVM link name and complete Rust signature.
- Immediate arguments must have constant expression trees; Rust const blocks
  preserve that requirement through code generation.
- Explicit C adapters cover byte swaps, fabs/copysign, and clear-cache calls.
- x86 f80 fabs/copysign reuse the existing long-double runtime shims.

## Legacy port

- Source: `intrinsics.rs` and `intrinsics_table.rs` before removal commit
  `c8ad97815`; old entry point: `lower_call_llvm_intrinsic`.
- Reused overload mangling, unique stdarch matching, signature-based declarations,
  pointer handling, `clear_cache` override, and long-double shim selection.
- Old CIR supplied LLVM-shaped calls. Parser IR supplies C builtin signatures;
  mismatched arity or type shapes require explicit C adapters.
- Parser `Type::Vector` uses `std::simd::Simd` for layout and intrinsic ABI.
  The old array-to-SIMD conversions become vector construction and bit casts.
- Vector constructors, splats, lane reads/writes, bit casts, and integer arithmetic
  support ordinary SIMD operands around intrinsic calls.
- Fixtures: `x86_scalar_intrinsics.c`, `x86_vector_intrinsics.c`,
  `intrinsic_{integer,float,pointer}_overloads.c`, `intrinsic_immediate_args.c`.

## Limits

- Aggregate returns, packed one-bit vectors, and complex dependent IIT types
  require further adaptation.
- SIMD elements: 8/16/32/64-bit integers, f32/f64; 1/2/4/8/16/32/64 lanes.
- Vector comparison masks, lane-wise numeric conversions, and ordinary C vector
  function ABI remain barriers; function addresses cannot expose LLVM intrinsics.
- Only default-address-space pointers participate in intrinsic signatures.
- TableGen aliases cover direct builtin mappings; builtins Clang expands by
  hand need separate lowering.
