# Builtins

<!-- toc -->

- [Compile-time queries and types](#compile-time-queries-and-types)
- [Source location and function
  names](#source-location-and-function-names)
- [Integer arithmetic and bits](#integer-arithmetic-and-bits)
- [Floating point](#floating-point)
- [Control flow and hints](#control-flow-and-hints)
- [CoreFoundation constant strings](#corefoundation-constant-strings)
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

`__builtin_classify_type` also folds inside constant expressions (for
`__builtin_choose_expr` conditions).

`__builtin_align_up`, `__builtin_align_down` and `__builtin_is_aligned` (clang)
take an integer or pointer and an integer alignment. They expand to wrapping
arithmetic on a captured `mask = alignment - 1` in the value's type (pointers
use `size_t` via `ptr_to_int`): up is `(x + mask) & ~mask`, down is `x & ~mask`,
and `is_aligned` is `(x & mask) == 0` typed `_Bool`. Pointers are captured once
and moved with a byte `ptr_offset` (forward by `aligned - x` for up, back by
`x & mask` for down), so the result keeps the pointer's provenance and type.

`__builtin_clear_padding(p)` becomes `intrinsic<void, slate.clear_padding>(p)`.
Slate derives from the pointee layout which bytes and bit-field bits are data
(named bit-fields and scalars; union members all count) and zeroes the rest
(`lowerer/padding.rs`, little-endian only).

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
- `__builtin_stdc_rotate_left`, `__builtin_stdc_rotate_right`: retained calls
  preserve both operand types, return the first unsigned integer type, and
  evaluate each argument once. Standard, extended, and bit-precise widths
  are supported without integer promotions. Required constant evaluation
  rotates modulo the value width; negative counts normalize under clang
  and are undefined under gcc ([GCC](https://gcc.gnu.org/onlinedocs/gcc/Bit-Operation-Builtins.html),
  [Clang](https://clang.llvm.org/docs/LanguageExtensions.html)).
- `__builtin_clz`, `__builtin_ctz`, `__builtin_popcount`, `__builtin_parity`,
  `__builtin_ffs`, `__builtin_clrsb`, with their registered width suffixes
- `__builtin_clzg`, `__builtin_ctzg`, `__builtin_popcountg`
- `__builtin_bswap{16,32,64}`, `__builtin_bitreverse{8,16,32,64}`

Parser: `sema/expression.rs` and `sema/vector_builtins.rs` emit overflow IR;
`sema/fold.rs` folds constant bit queries and type-generic rotations.
Retained polymorphic builtin declarations are keyed by name and C signature. Slate: `lowerer/arithmetic.rs` handles
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
- `__builtin_inf`, `__builtin_huge_val`, and their `f`, `l`, `f16`, `f128` forms: fold to
  the positive-infinity `Number::FloatBits` constant of the return type, so they are valid
  in static initializers. Decimal forms stay calls.
- `__builtin_nan`, `__builtin_nans` and their `f`, `l`, `f16`, `f128` forms: a plain
  string literal argument (decimal, `0x` or leading-`0` octal payload, or empty) folds to a
  `Number::FloatBits` constant with that payload; `nans` leaves the quiet bit clear. Any other
  argument stays a call to the library function.
- `__builtin_flt_rounds`
- `__builtin_{inf,nan,nans,fabs}d{32,64,128,64x}` (gcc only): decimal float values
  that clang lacks, so they come from the generated gcc table
  (see [library calls](#library-and-target-intrinsic-calls)). gcc's `float.h`
  builds `DEC_INFINITY`, `DEC_NAN` and `DEC*_SNAN` from them. They stay
  retained calls; `__has_builtin` reports them for gcc only.

Classification, quiet comparisons, and complex construction expand in
`sema/expression.rs::custom_builtin`. Slate handles scalar classification and
infinities in `lowerer/values.rs`. `flt_rounds` calls the C helper in
`frontend/shims/fenv.c`, mapping `fegetround()` to `FLT_ROUNDS` values.

## Control flow and hints

- `__builtin_unreachable`, `__builtin_assume`, `__assume`
- `__builtin_expect`, `__builtin_expect_with_probability`, `__builtin_unpredictable`
- `__builtin_prefetch`, `__builtin_assume_aligned`
- `__builtin_cpu_init`, `__builtin_cpu_supports`
- MS SEH: `_exception_code`, `_exception_info`, `_abnormal_termination`
  (and `__` forms) lower to `seh_*` values; see
  [control-flow](control-flow.md#structured-exception-handling).
- MS `__noop(...)`: `const<i32>(0)`; arguments are type-checked (`void` is
  rejected) and never evaluated.

Parser: `sema/expression.rs::function_like_builtin`. Slate: `lowerer/values.rs`
for hints and `lowerer/target_features.rs` for CPU feature queries.

## CoreFoundation constant strings

The clang flavor predeclares `struct __NSConstantString_tag { const int *isa;
int flags; const char *str; long length; }` and `typedef ... __NSConstantString`
from `<built-in>` (`Preprocessor::predeclared_typedefs`, all targets).
`__builtin___CFStringMakeConstantString("lit")` (`CustomBuiltin::CfString`,
`Lowerer::cf_string`) needs a plain ASCII string literal and lowers to
`addr_of` of a fresh internal global `{ __CFConstantStringClassReference, 1992,
"lit", length }`; the extern `int[]` class reference is created on first use.
The registry's `constant_CFString` resolves to `__NSConstantString`. Only the
default (objc) runtime ABI is modelled, not `-fcf-runtime-abi=swift`.

## Variadic arguments

- `__builtin_va_list`
- `__builtin_va_arg`, `__builtin_va_start`, `__builtin_va_end`, `__builtin_va_copy`

- `__builtin_ms_va_list`, `__builtin_ms_va_start`, `__builtin_ms_va_end`,
  `__builtin_ms_va_copy` (x86-64 and AArch64 only)
- `__va_start(&ap, ...)`, the Microsoft intrinsic behind ucrt's
  `__crt_va_start` on x64 and ARM64: `va_start` on `*(&ap)`, other
  arguments evaluated for typing only. ucrt's `va_arg` and `va_end` are
  plain `char *` arithmetic, so they need no IR operation. A redeclaration
  with external linkage (ucrt's `void __va_start(va_list *, ...)`) keeps any
  non-library builtin without a checked signature, as in clang.

`slate-parser/src/target_info.rs` selects the target's `va_list` layout;
`sema/expression.rs` emits the variadic IR operations. Slate lowers them in
`lowerer/values.rs` and `lowerer/statements.rs`.

`__builtin_ms_va_list` is `char *`. Each va operation carries a
`VaConvention`: `win64` (printed `va_arg<T, win64>`, `va_start<win64>`)
when the list is a Microsoft one on a target whose native `va_list` is not
`char *`, `Target` otherwise (so Windows and Darwin lists stay native).
As in clang's `BuildVAArgExpr`, `va_arg` on any modifiable `char *` lvalue
is `win64` on such targets. The clang flavor also applies clang's
`va_start` checks: `__builtin_ms_va_start` only in Win64 functions (any
function on Windows except `sysv_abi`), the native `va_start` not in a
Win64 function off Windows nor a `sysv_abi` function on Windows, and
neither in a function with fixed args. gcc accepts all three.
`va_arg` of a struct or union on the SysV x86-64 convention carries a
`VaPassing`: `regs=[...]` lists one class per eightbyte (`int`, `sse`, or
`sse_wide` for a 16-byte float) when the whole value fits in registers, and
`memory` when it is over 16 bytes or classified memory. Scalars print nothing.
Slate lowers it to `__slate_sysv_va::arg`, which takes the pieces from the
register save area while enough GP and SSE registers remain and from the
overflow area otherwise. On i386 cdecl every record is `memory`: the
`char *` list advances in 4-byte slots, and slate lowers every `va_arg` on
that list, scalar or record, through `__slate_i386_va::arg`. A 16-aligned
type (`__float128`) first rounds the cursor up to 16, as gcc's caller and
`va_arg` do; clang's i386 `va_arg` reads it at 4-byte alignment and
disagrees with its own caller, so `gcc/i686/float128_va_arg.c` uses gcc as
its oracle. On AAPCS64 a scalar binary128 (`long double`)
takes one vector register slot, lowered as a one-member
`__slate_aapcs_va::hfa`. On AAPCS64 a
homogeneous float aggregate is `hfa=<count>x<bytes>` (one 16-byte vector
register per member), another record over 16 bytes is `indirect` (a
pointer in one GP slot), and the rest are `regs=[int, ...]`; slate lowers
them to `__slate_aapcs_va::{hfa, indirect, gp}`, following clang's
`EmitAAPCSVAArg` (offset bumped before the stack fallback, 16-byte
alignment for over-aligned types). Records containing vectors stay a
barrier on i386 and AAPCS64, as do other conventions.
Slate lowers `win64` `va_arg` on x86-64 to `__slate_win64_va::arg`
(8-byte slots; larger or non-power-of-two types by reference), `va_copy`
to a pointer copy, and `va_end` to nothing. `va_start<win64>` only
appears in variadic `ms_abi` definitions, which slate rejects.

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

- MS `_Interlocked*`: `And`, `Or`, `Xor`, `ExchangeAdd`, `ExchangeSub`,
  `Increment`, `Decrement`, `Exchange[Pointer]`,
  `CompareExchange[Pointer]`, `CompareExchange128`, and on aarch64 `Add`,
  with their 8/16/64 widths and `_acq`, `_rel`, `_nf` orderings
- MS `_bittest`, `_bittestandset`, `_bittestandreset`,
  `_bittestandcomplement` (and `64`), `_interlockedbittestandset` and
  `_interlockedbittestandreset` (and `64`, `_acq`, `_rel`, `_nf`)

Lowered by `slate-parser/src/sema/atomic.rs` into reads, writes, updates,
compare-exchanges, and fences; Rust lowering is in `slate/src/frontend/lowerer/atomics.rs`.
Supported operations and ordering rules: [atomic builtins](atomics.md#atomic-builtins).

The MS intrinsics are `Expansion::Ms` (`sema/builtins.rs`): their arguments
convert through the registry prototype (`msint32_t` is `long` where long is
32 bits, else `int`), then `Lowerer::ms_intrinsic` builds the IR. Plain
forms are `seq_cst`, `_acq`/`_rel`/`_nf` are acquire/release/relaxed, and a
release compare-exchange fails relaxed. `Increment`/`Decrement`/`Add` return the
new value, the rest the old one. `_InterlockedCompareExchange(dest, exchange,
comparand)` is a `form=old` compare-exchange expecting `comparand`. A bit test
addresses the byte at `base + (offset >> 3)` (arithmetic shift) and bit
`offset & 7`, as clang does; the plain forms are nonatomic `read`/`update`,
the interlocked ones an atomic `update` of that byte.
`_InterlockedCompareExchange128(dest, high, low, comparand_result)` is a
`form=write_back` compare-exchange on `unsigned __int128` (dest and
comparand cast from `long long *`, desired `(high << 64) | low` with both
halves zero-extended), its bool converted to `unsigned char`. aarch64 builtins
come from `clang_aarch64_builtins.rs`, selected in `target_builtin`.

## Library and target intrinsic calls

- `__rdtsc`
- `__builtin_memcpy`, `__builtin_memmove`, `__builtin_memset`, `__builtin_memcmp`
- `__builtin_strlen`, `__builtin_strcmp`, `__builtin_strncmp`, `__builtin_strchr`
- Registered allocation, math, and process-control library builtins
- Target builtin aliases in the generated intrinsic catalog, including
  supported `__builtin_ia32_*` calls

`sema/builtins.rs` resolves signatures from `sema/clang_builtins.rs` and
`sema/gcc_builtins.rs` (generated from gcc's `builtins.def`), and
`sema/expression.rs` emits calls with `c_builtin` metadata. `clang_builtin`
looks a name up in this order:

- clang and MSVC flavors: the clang table, then (clang only) the target table.
- gcc flavor: the clang entry, unless its prototype names a header type
  (`FILE *`, `jmp_buf`) that gcc spells as a plain pointer; then the gcc entry,
  then for `__builtin_X` the library entry `X` (clang's, or gcc's when clang's
  names a header type). A gcc library entry is gated by its `builtins.def`
  standard (`DEF_C99_BUILTIN` and so on) unless the mode is a GNU one. gcc
  records are the `BUILT_IN_*` enum names, so custom semantics keyed on clang
  records do not apply to gcc-only builtins such as `__builtin_add_overflow_p`. A `__builtin_X`
  declaration whose `X` is a registered library builtin also carries
  `c_builtin_library="X"`, the symbol clang calls when it does not open-code the
  builtin; Slate declares the extern with that `link_name`. Slate maps library
  symbols through `lowerer/names.rs` and intrinsic aliases through
  `lowerer/intrinsics.rs` plus `lowerer/intrinsics_table.rs`.
  `__rdtsc` expands in `sema/vector_builtins.rs`. See
  [intrinsic lowering](../intrinsic-lowering.md) for catalog and signature limits.
