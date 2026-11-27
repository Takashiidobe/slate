# Builtins

Slate resolves compiler builtins using the selected flavor, target, and ISA
flags. Some produce constants or IR operations. Others lower to Rust intrinsics
or calls to the corresponding C library symbol.

```c
int count_bits(unsigned int value) { return __builtin_popcount(value); }
```

Save this as `bits.c` and translate it with `slate translate bits.c`.

Compiler inventory entries describe recognition and prototypes. `__has_builtin`
queries recognition. To check lowering for a particular call, run
`slate lowering-barriers` with the original compiler flags and source. Lowering
can depend on operand types, enabled ISA features, and whether an immediate
argument folds to a constant. See
[Builtins and Inline Assembly](../parsing/sema/builtins-asm.md) for how these
operations appear in IR.

## Compile-time queries and types

- `__builtin_constant_p` tests whether an expression can be folded. At optimized
  levels, some scalar queries can resolve after inlining.
- `__builtin_types_compatible_p` compares two C types.
- `__builtin_choose_expr` selects an expression at compile time.
- `__builtin_classify_type` folds to the compiler's type category.
- `__builtin_offsetof` computes a member's byte offset from the target layout.
- `__builtin_bit_cast` reinterprets the bits of a supported value as another
  type of the same size.
- `__builtin_addressof` takes an object's address.
- `__builtin_counted_by_ref` retrieves a pointer to a field's associated count.
  See [Attributes](attributes.md#allocation-bounds-and-cleanup).
- `__builtin_object_size` computes remaining bytes in an object or subobject.
  The mode controls which size to report and the fallback for an unknown size.
- `__builtin_dynamic_object_size` also allows a size computed at runtime.
- `__builtin_align_up` rounds an integer or pointer up to an alignment in Clang
  mode.
- `__builtin_align_down` rounds an integer or pointer down in Clang mode.
- `__builtin_is_aligned` tests alignment in Clang mode.
- `__builtin_clear_padding` zeros object padding using the resolved layout.
  Lowering covers little-endian targets.

## Source locations and names

- `__builtin_LINE` and `__builtin_COLUMN` return the source line and column.
- `__builtin_FILE` and `__builtin_FILE_NAME` return the source path or basename.
- `__builtin_FUNCTION` returns the enclosing function name.
- `__func__`, `__FUNCTION__`, and `__PRETTY_FUNCTION__` are predefined function
  names.

## Integer arithmetic and bits

- `__builtin_add_overflow` checks addition and stores the converted result
  through the result pointer.
- `__builtin_sub_overflow` does the same for subtraction.
- `__builtin_mul_overflow` does the same for multiplication.
- `__builtin_umul_overflow`, `__builtin_umull_overflow`, and
  `__builtin_umulll_overflow` cover the corresponding unsigned multiplication
  widths.
- `__builtin_rotateleft8` and `__builtin_rotateright8` rotate bits. The
  corresponding `16`, `32`, and `64` suffixes select wider types.
- `__builtin_stdc_rotate_left` and `__builtin_stdc_rotate_right` preserve the
  unsigned operand type, including supported bit-precise integer widths.
- `__builtin_clz` and `__builtin_ctz` count leading and trailing zero bits.
- `__builtin_clrsb` counts redundant sign bits.
- `__builtin_popcount` counts set bits.
- `__builtin_parity` returns the parity of the set bits.
- `__builtin_ffs` returns the one-based index of the lowest set bit, or zero.
  These count and scan families include the registered `l` and `ll` forms.
- `__builtin_clzg`, `__builtin_ctzg`, and `__builtin_popcountg` are type-generic
  bit counts.
- `__builtin_bswap16`, `__builtin_bswap32`, and `__builtin_bswap64` reverse
  bytes. GCC mode also supports `__builtin_bswap128`.
- `__builtin_bitreverse8`, `__builtin_bitreverse16`, `__builtin_bitreverse32`,
  and `__builtin_bitreverse64` reverse bits.

## Floating point

- `__builtin_isnan`, `__builtin_isinf`, `__builtin_isfinite`, and
  `__builtin_isnormal` classify values.
- `__builtin_issubnormal`, `__builtin_iszero`, and `__builtin_issignaling` test
  individual floating-point classes.
- `__builtin_signbit`, `__builtin_signbitf`, and `__builtin_signbitl` test the
  sign bit.
- `__builtin_isinf_sign` tests infinity and reports its sign.
- `__builtin_fpclassify` selects the supplied result for the value's class.
- `__builtin_isgreater`, `__builtin_isgreaterequal`, `__builtin_isless`,
  `__builtin_islessequal`, `__builtin_isunordered`, and
  `__builtin_islessgreater` implement quiet comparisons.
- `__builtin_complex` constructs a complex value from its components.
- `__builtin_inf` and `__builtin_huge_val` construct positive infinity in the
  registered binary floating-point widths.
- `__builtin_nan` and `__builtin_nans` construct quiet and signaling NaNs.
  Registered binary-width variants can fold a literal payload in a static
  initializer.
- `__builtin_flt_rounds` reads the current rounding mode through a C shim.
- `__builtin_fabs`, `__builtin_copysign`, `__builtin_fmax`, `__builtin_fmin`,
  `__builtin_fma`, and `__builtin_powi` have scalar lowering paths. The operand
  type and floating-point policy can select a C helper or library call.

## Control flow and hints

- `__builtin_unreachable`, `__builtin_assume`, and MSVC's `__assume` communicate
  reachability or assumptions.
- `__builtin_trap` and `__builtin_debugtrap` lower trap requests.
- `__builtin_expect`, `__builtin_expect_with_probability`, and
  `__builtin_unpredictable` accept branch prediction hints.
- `__builtin_prefetch` accepts a memory prefetch hint.
- `__builtin_assume_aligned` accepts a pointer alignment assumption.
- `__builtin_cpu_init` and `__builtin_cpu_supports` support CPU feature queries
  on supported targets and feature names.
- `__builtin_return_address`, `__builtin_frame_address`,
  `__builtin_extract_return_addr`, and `__builtin_frob_return_addr` support
  stack and return-address queries where the target provides a lowering.
- `__builtin_setjmp` and `__builtin_longjmp` have a translation path for
  supported jump patterns. See [setjmp and longjmp](../setjmp.md).
- `_exception_code`, `_exception_info`, and `_abnormal_termination` resolve
  Microsoft structured exception queries, including their double-underscore
  spellings.
- `__noop(...)` type-checks its arguments without evaluating them.

## Variadic arguments and storage

- `__builtin_va_list` selects the target's variadic list type.
- `__builtin_va_start`, `__builtin_va_arg`, `__builtin_va_copy`, and
  `__builtin_va_end` start, read, copy, and end variadic lists.
- `__builtin_c23_va_start` supports the C23 form of `va_start`.
- `__builtin_ms_va_list`, `__builtin_ms_va_start`, `__builtin_ms_va_copy`, and
  `__builtin_ms_va_end` handle Microsoft variadic lists on x86-64 and AArch64.
  `__va_start` is the Microsoft intrinsic used by UCRT headers. See
  [Variadic Calls and `va_arg`](../lowering/variadics.md) for ABI limits.
- `alloca`, `_alloca`, `__builtin_alloca`, and `__builtin_alloca_with_align`
  provide function-scoped dynamic storage. See
  [Dynamic Stack Allocation](../lowering/alloca.md).
- `__builtin___CFStringMakeConstantString` constructs a CoreFoundation constant
  string in Clang mode from a plain ASCII string literal, using the default
  runtime ABI.

## Atomics

- `__c11_atomic_*` covers initialization, loads, stores, exchanges,
  compare-exchange, fetch operations, and fences used by Clang's
  `<stdatomic.h>`.
- `__atomic_*` covers GNU loads, stores, exchanges, compare-exchange,
  read-modify-write operations, flag operations, and fences. This includes the
  generic pointer forms and typed `_n` forms where defined.
- `__scoped_atomic_*` carries an explicit synchronization scope. Supported
  operations and scopes depend on the target.
- `__sync_*` covers legacy fetch and update operations, compare-and-swap, locks,
  and synchronization. Recognized size-suffixed spellings take their width from
  the pointer operand.
- `__atomic_always_lock_free`, `__atomic_is_lock_free`, and
  `__c11_atomic_is_lock_free` query the target's lock-free widths. A runtime
  query can call `libatomic`.

For the resulting operations and ordering policies, see
[Atomics, Builtins, and Inline Assembly](../parsing/ir/special-operations.md).

## Library, memory, and string builtins

- `__builtin_memcpy`, `__builtin_memcpy_inline`, `__builtin_memmove`, and
  `__builtin_memset` copy, move, or fill memory.
- `__builtin_memcmp`, `__builtin_memchr`, `__builtin_mempcpy`, `__builtin_bcmp`,
  and `__builtin_bzero` cover additional memory operations.
- `__builtin_strlen`, `__builtin_strcmp`, `__builtin_strncmp`,
  `__builtin_strchr`, `__builtin_strrchr`, `__builtin_strcpy`,
  `__builtin_strncpy`, `__builtin_stpcpy`, `__builtin_strcat`,
  `__builtin_strcspn`, `__builtin_strspn`, `__builtin_strpbrk`, and
  `__builtin_strstr` cover standard string operations.
- `__builtin___memcpy_chk`, `__builtin___memmove_chk`, `__builtin___memset_chk`,
  `__builtin___stpcpy_chk`, `__builtin___strcat_chk`, and
  `__builtin___strcpy_chk` are checked forms used by fortified headers.
- `__builtin_malloc`, `__builtin_free`, `__builtin_printf`, `__builtin_sprintf`,
  and `__builtin_snprintf` resolve corresponding library operations.
- `__builtin___clear_cache` lowers through a target adapter where needed.

Library builtins can become an ordinary call to the corresponding C symbol. The
[builtin inventories](../tools/builtin-inventories.md) describe compiler and
language-mode coverage for additional library names.

## Vector and target intrinsics

- `__builtin_shufflevector` selects lanes. Rust lowering requires a supported
  vector shape and constant lane selection.
- `__builtin_convertvector` converts lanes to the destination element type.
- `__builtin_reduce_*` covers add, multiply, and, or, xor, and supported minimum
  and maximum reductions.
- `__builtin_elementwise_*` includes popcount, min, max, fused multiply-add, and
  square root.
- `__builtin_ia32_*` includes mapped x86 intrinsics and explicit expansions for
  selected shuffles, extracts, inserts, comparisons, blends, and masked loads
  and stores. Immediate operands must fold to constants.
- `__rdtsc` reads the x86 time-stamp counter.
- `__builtin_neon_*` includes Clang NEON lane operations, reductions, saturating
  arithmetic, loads and stores, rounding, conversions, and table lookups.
  Polymorphic forms depend on the constant NEON type code.
- GCC AArch64 `sv*` intrinsics from `arm_sve.h` use matching Rust
  `core::arch::aarch64` functions for mapped names and types. The
  [SVE mapping](https://github.com/Takashiidobe/slate/blob/main/crates/slate/src/frontend/lowerer/sve_table.rs)
  lists the individual entries.

For individual LLVM aliases and signature requirements, see the
[checked-in catalog](https://github.com/Takashiidobe/slate/blob/main/crates/slate/src/frontend/lowerer/intrinsics_table.rs)
and [Intrinsic Catalog](../tools/intrinsic-catalog.md).

## Microsoft intrinsics

Availability depends on the MSVC target and architecture. Clang mode with
Microsoft extensions also recognizes shared Microsoft intrinsics.

- `__cpuid` and `__cpuidex` query x86 CPU identification.
- `_BitScanForward` and `_BitScanReverse` locate a set bit and report whether
  the input contained one. The `64` forms take 64-bit operands.
- `_rotl`, `_rotr`, `_rotl64`, and `_rotr64` rotate integer bits.
- `__ll_lshift`, `__ll_rshift`, and `__ull_rshift` shift 64-bit values.
- `_mul128` and `_umul128` return the low half of a wide product and write its
  high half through a pointer.
- `__mulh` and `__umulh` return the high half of a wide product.
- `__shiftleft128` and `__shiftright128` shift across two 64-bit operands on
  x86-64.
- `__movsb`, `__movsw`, `__movsd`, and the corresponding `__stos*` names cover
  x86 copy and fill operations at the selected element width.
- `_bittest`, `_bittestandset`, `_bittestandreset`, and `_bittestandcomplement`
  test or modify a selected bit. The interlocked forms use atomic updates where
  supported.
- `_Interlocked*` covers exchange, compare-exchange, increment, decrement, fetch
  arithmetic, and bitwise updates. Recognized width and ordering suffixes depend
  on the operation and target.

The
[MSVC inventory](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/tools/msvc_intrinsics_alphabetical.jsonl)
lists individual names by architecture and records lowering probe results when
available.
