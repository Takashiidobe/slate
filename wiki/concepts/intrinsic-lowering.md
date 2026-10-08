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
| `llvm-tblgen --dump-json` over `Intrinsics.td` | C builtin aliases and overloaded immediate parameters |
| Optional stdarch sources | Internal x86/x86_64 LLVM link-name signature overrides |

- Generator: [`slate-intrinsic-gen`](../../crates/slate-intrinsic-gen/README.md).
- Checked-in catalog: `crates/slate/src/frontend/lowerer/intrinsics_table.rs`.
- General, x86, AArch64, ARM, and RISC-V sections; LLVM commit stored in the catalog.
- LLVM and C++ dependencies apply only to regeneration.
- Supported IIT templates retain overload constraints and references:
  `overload:index:vector-constraint:element-constraint`, `match:index`,
  `element:index` (`LLVMVectorElementType`, the lane type of an overload;
  `llvm.vector.reduce.*` results).
- The checked-in catalog regenerates byte-for-byte from the system LLVM
  (`--llvm-build /usr`, 23.1.1) with `--llvm-src` at the recorded commit,
  except for 4 RISC-V `pssh*` entries that the system LLVM lacks.
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
- Parser `intrinsic<T, llvm.name>(..)` values, produced by sema's
  [vector builtin expansion](ir/builtins.md#vector-builtin-expansion), find
  their entry by LLVM name and take the same path.
- Scalar `__builtin_rotateleft/right{8,16,32,64}` calls use Rust integer
  `rotate_left`/`rotate_right`; counts convert to `u32` and wrap modulo width.
- `__builtin_flt_rounds()` calls `__slate_fenv_flt_rounds` in
  `frontend/shims/fenv.c`: `fegetround()` maps target `FE_TOWARDZERO`,
  `FE_TONEAREST`, `FE_UPWARD`, `FE_DOWNWARD` to 0, 1, 2, 3; unknown modes to -1.
  Each call reads the current environment; no host rounding constants are used.
- C bridge declarations emit independently of f80 support; the shim collector
  includes the fenv runtime whenever a `__slate_fenv_*` bridge is requested.
- x86 f80 fabs/copysign reuse the existing long-double runtime shims.

- Parser IR supplies C builtin signatures;
  mismatched arity or type shapes require explicit C adapters.
- Parser `Type::Vector` uses `std::simd::Simd` for layout and intrinsic ABI.
- Vector constructors, splats, lane reads/writes, bit casts, and integer arithmetic
  support ordinary SIMD operands around intrinsic calls.

## Limits

- Aggregate returns, packed one-bit vectors, and complex dependent IIT types
  require further adaptation.
- SIMD elements: 8/16/32/64-bit integers, f32/f64; 1/2/4/8/16/32/64 lanes.
- Constant-mask `shuffle` (any result lane count, undef lanes read lane 0) is
  `std::simd::simd_swizzle!`; vector `~` / `-` are `!` / `-` (unsigned negation
  is `splat(0) - v`); lane-wise conversions are
  `std::simd::num::Simd{Int,Uint,Float}::cast::<T>(v)`, chosen by the source
  element (`Expr::GenericPath` carries the turbofish).
- Vector comparison masks, dynamic shuffle masks, and ordinary C vector
  function ABI remain barriers; function addresses cannot expose LLVM intrinsics.
- Only default-address-space pointers participate in intrinsic signatures.
- TableGen aliases cover direct builtin mappings; builtins Clang expands by
  hand expand in sema. Unexpanded ones stay `builtin without a lowering`
  barriers.
