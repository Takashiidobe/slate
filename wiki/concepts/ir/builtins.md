# Builtins

<!-- toc -->
- [Compile-time queries and types](#compile-time-queries-and-types)
- [Source location and function
  names](#source-location-and-function-names)
- [Integer arithmetic and bits](#integer-arithmetic-and-bits)
- [Floating point](#floating-point)
- [Control flow and hints](#control-flow-and-hints)
- [Variadic arguments](#variadic-arguments)
- [Vector builtin expansion](#vector-builtin-expansion)
- [Atomic operations](#atomic-operations)
- [Library and target intrinsic
  calls](#library-and-target-intrinsic-calls)
<!-- /toc -->

Supported by slate-parser, grouped by category. Retained calls reach Slate's
[scalar lowering](../../../crates/slate/src/frontend/lowerer/values.rs) or
[intrinsic lowering](../intrinsic-lowering.md); parser support alone does not
imply Rust lowering for every call.

## Compile-time queries and types

- `__builtin_constant_p`
- `__builtin_types_compatible_p`
- `__builtin_choose_expr`
- `__builtin_classify_type`
- `__builtin_bit_cast`
- `__builtin_addressof`
- `__builtin_counted_by_ref`

Lowered in `slate-parser/src/sema/expression.rs`; constant and type queries
resolve through `sema/types.rs` and `sema/ctype/`.

## Source location and function names

- `__builtin_LINE`, `__builtin_COLUMN`
- `__builtin_FILE`, `__builtin_FILE_NAME`, `__builtin_FUNCTION`
- `__func__`, `__FUNCTION__`, `__PRETTY_FUNCTION__`

`sema/expression.rs`: `source_location` emits constants or string globals;
`Lowerer::place` handles predefined function names.

## Integer arithmetic and bits

- `__builtin_add_overflow`, `__builtin_sub_overflow`, `__builtin_mul_overflow`
- `__builtin_umul_overflow`, `__builtin_umull_overflow`, `__builtin_umulll_overflow`
- `__builtin_rotateleft{8,16,32,64}`, `__builtin_rotateright{8,16,32,64}`
- `__builtin_clz`, `__builtin_ctz`, `__builtin_popcount`, `__builtin_parity`,
  `__builtin_ffs`, `__builtin_clrsb`, with their registered width suffixes
- `__builtin_clzg`, `__builtin_ctzg`, `__builtin_popcountg`
- `__builtin_bswap{16,32,64}`, `__builtin_bitreverse{8,16,32,64}`

Parser: `sema/expression.rs` and `sema/vector_builtins.rs` emit overflow IR;
`sema/fold.rs` folds constant bit queries. Slate: `lowerer/arithmetic.rs` handles
overflow, `lowerer/values.rs` handles rotations and scalar bit counts, and
`lowerer/intrinsics.rs` handles mapped bit operations.

## Floating point

- `__builtin_isnan`, `__builtin_isinf`, `__builtin_isfinite`, `__builtin_isnormal`
- `__builtin_issubnormal`, `__builtin_iszero`, `__builtin_issignaling`
- `__builtin_signbit`, `__builtin_signbitf`, `__builtin_signbitl`
- `__builtin_isinf_sign`, `__builtin_fpclassify`
- `__builtin_isgreater`, `__builtin_isgreaterequal`, `__builtin_isless`,
  `__builtin_islessequal`, `__builtin_isunordered`, `__builtin_islessgreater`
- `__builtin_complex`
- `__builtin_inf`, `__builtin_huge_val`, with `f` and `l` forms
- `__builtin_flt_rounds`

Classification, quiet comparisons, and complex construction expand in
`sema/expression.rs::custom_builtin`. Slate handles scalar classification and
infinities in `lowerer/values.rs`. `flt_rounds` calls the C helper in
`frontend/shims/fenv.c`, mapping `fegetround()` to `FLT_ROUNDS` values.

## Control flow and hints

- `__builtin_unreachable`, `__builtin_assume`, `__assume`
- `__builtin_expect`, `__builtin_expect_with_probability`, `__builtin_unpredictable`
- `__builtin_prefetch`, `__builtin_assume_aligned`
- `__builtin_cpu_init`, `__builtin_cpu_supports`

Parser: `sema/expression.rs::function_like_builtin`. Slate: `lowerer/values.rs`
for hints and `lowerer/target_features.rs` for CPU feature queries.

## Variadic arguments

- `__builtin_va_list`
- `__builtin_va_arg`, `__builtin_va_start`, `__builtin_va_end`, `__builtin_va_copy`

`slate-parser/src/target_info.rs` selects the target's `va_list` layout;
`sema/expression.rs` emits the variadic IR operations. Slate lowers them in
`lowerer/values.rs` and `lowerer/statements.rs`.

## Vector builtin expansion

- `__builtin_shufflevector`, `__builtin_convertvector`
- `__builtin_reduce_{add,mul,and,or,xor,max,min,maximum,minimum}`
- `__builtin_elementwise_{popcount,max,min,fma}`
- `__builtin_ia32_extract*`, `__builtin_ia32_vextractf128_*`
- `__builtin_ia32_pternlog{d,q}{128,256,512}_mask[z]`
- `__builtin_ia32_reduce_f{add,mul}_p{s,d}512`

`sema/expression.rs` handles shuffle and conversion;
`sema/vector_builtins.rs` expands reductions and target operations into vector
IR or LLVM intrinsics. Masked extract and ternary-logic forms require all-ones
masks. Slate consumes these in `lowerer/vectors.rs` and `lowerer/intrinsics.rs`.
See [vector types](type-families.md#vector) for supported shapes.

## Atomic operations

- `__c11_atomic_*`
- `__atomic_*`
- `__scoped_atomic_*`
- `__sync_*`

Lowered by `slate-parser/src/sema/atomic.rs` into reads, writes, updates,
compare-exchanges, and fences; Rust lowering is in `slate/src/frontend/lowerer/atomics.rs`.
Supported operations and ordering rules: [atomic builtins](atomics.md#atomic-builtins).

## Library and target intrinsic calls

- `__rdtsc`
- `__builtin_memcpy`, `__builtin_memmove`, `__builtin_memset`, `__builtin_memcmp`
- `__builtin_strlen`, `__builtin_strcmp`, `__builtin_strncmp`, `__builtin_strchr`
- Registered allocation, math, and process-control library builtins
- Target builtin aliases in the generated intrinsic catalog, including
  supported `__builtin_ia32_*` calls

`sema/builtins.rs` resolves signatures from `sema/clang_builtins.rs` and
`sema/expression.rs` emits calls with `c_builtin` metadata. Slate maps library
symbols through `lowerer/names.rs` and intrinsic aliases through
`lowerer/intrinsics.rs` plus `lowerer/intrinsics_table.rs`.
`__rdtsc` expands in `sema/vector_builtins.rs`. See
[intrinsic lowering](../intrinsic-lowering.md) for catalog and signature limits.
