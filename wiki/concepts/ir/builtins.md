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
- [alloca](#alloca)
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

`__builtin_object_size(p, type)` and `__builtin_dynamic_object_size` target
clang `-O2`, not `-O0` (`sema/object_size.rs`). The front end folds what
clang's `tryEvaluateBuiltinObjectSize` folds: a pointer built from a declared
object, string or compound literal (whole object), or from a member of any
base including `p->m` (the closest sub-object, bit 0 of `type`), moved by
constant offsets. It folds to `size_t` remaining bytes, 0 past either end.
Arithmetic leaving the sub-object, or stepping a non-array sub-object by a
different element size, drops the sub-object. Type 1 on a trailing array of an
unknown base (flexible under `-fstrict-flex-arrays=N`) is not folded. An
argument with side effects is not evaluated and gives the unknown value
(`(size_t)-1` for types 0/1, 0 for 2/3); an unfolded type 3 is 0 (gcc flavor
asks LLVM for the minimum instead). Everything else becomes
`intrinsic<size_t, llvm.objectsize>(p, min = type & 2, false, dynamic)`, so
rustc's optimizer resolves `p = local` and `malloc(n)` as clang `-O2` does.
Optimizer-dependent fixtures live in `crates/slate/tests/fixtures.release/`,
whose C oracle builds at `-O2`.

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
- `__builtin_c23_va_start` (C23 `va_start(ap, ...)`): `va_start` on its
  first argument. In the gcc flavor the parser drops every token from the
  first top-level comma to the closing parenthesis before parsing, since
  gcc never evaluates them and they may be unbalanced brackets or braces
  (`va_start (ap, ]]]{{{)`); clang parses them as expressions.

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
its oracle. On AAPCS32 (armv7, soft or hard float) every record is also
`memory`: the `void *` list holds 4-byte slots, rounded up to 8 for an
8-aligned type, and slate lowers records and `_Float16` (a `half` in its
own slot) through `__slate_arm_va::arg`. On AAPCS64 a scalar binary128 (`long double`)
takes one vector register slot, lowered as a one-member
`__slate_aapcs_va::hfa`. On AAPCS64 a
homogeneous float aggregate is `hfa=<count>x<bytes>` (one 16-byte vector
register per member), another record over 16 bytes is `indirect` (a
pointer in one GP slot), and the rest are `regs=[int, ...]`; slate lowers
them to `__slate_aapcs_va::{hfa, indirect, gp}`, following clang's
`EmitAAPCSVAArg` (offset bumped before the stack fallback, 16-byte
alignment for over-aligned types). Records containing vectors stay a
barrier on i386, AAPCS32 and AAPCS64, as do other conventions.
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
- `__builtin_ia32_pmul{u,}dq{128,256,512}`
- `__builtin_ia32_pshufd{,256,512}`
- `__builtin_ia32_select{b,w,d,q,ps,pd}_{128,256,512}`

`sema/expression.rs` handles shuffle and conversion;
`sema/vector_builtins.rs` expands reductions and target operations into vector
IR or LLVM intrinsics. Masked extract and ternary-logic forms require all-ones
masks. Slate consumes these in `lowerer/vectors.rs` and `lowerer/intrinsics.rs`.

`pmuludq`/`pmuldq` bit-cast both operands to 64-bit lanes (unsigned or signed),
shift each lane left then right by 32 (zero or sign fill) and multiply with
wrapping. `pshufd` with a constant immediate is a lane shuffle that repeats the
immediate per 128-bit group; a non-constant immediate stays a call. The
`select*` builtins (runtime-mask lane select, unlike the constant-mask forms
above) become `intrinsic<T, slate.vector_select>(mask, a, b)`: lane `i` is `a`
when bit `i` of the integer mask is set, else `b`. Slate lowers it to
`std::simd::Select::select(Mask::<iN, L>::from_bitmask(mask), a, b)`; slate
vectors (`__SlateVector`) are not supported.
See [vector types](type-families.md#vector) for supported shapes.

`sema/x86_vector_builtins.rs` expands the x86 builtins clang's CGBuiltin
rewrites by hand. Immediates must fold to constants (otherwise the call stays):

| Builtins | Expansion |
| --- | --- |
| `__builtin_elementwise_sqrt` | `intrinsic<T, llvm.sqrt>` (scalars too) |
| `undef{128,256,512}`, `__builtin_nondeterministic_value` | zero of the result type (an argument with effects is sequenced first) |
| `vec_set_*` / `vec_ext_*` | two-operand shuffle with a splat / `lane(v, imm & (lanes-1))` |
| `pblend{w,d}*`, `blendp{s,d}*` | shuffle, lane `i` from `b` when imm bit `i % 8` is set |
| `permd{i,f}{256,512}` | shuffle `i/4*4 + (imm >> 2*(i%4) & 3)` |
| `ps{r,l}ldqi{128,256,512}_byteshift` | bit cast to bytes, per-16-byte shuffle against zero (imm > 15 is zero), bit cast back |
| `cmp{eq,lt,le,unord,neq,nlt,nle,ord}{ps,pd,ss,sd}`, `cmpp{s,d}[256]`, `cmps{s,d}` | predicate (`imm & 15`, signalling bit ignored) built from `eq/lt/le/gt/ge/ne` compares, `ord = (a==a)&(b==b)`, `unord = (a!=a)\|(b!=b)` and `not`; mask bit cast to the float type; scalar forms shuffle lane 0 into `a` |
| `cmpp{s,d}{128,256,512}_mask` | the same predicate, `slate.vector_sign_bitmask`, `and` with the mask unless all-ones |
| `cvtmask2{b,w,d,q}*` / `cvt{b,w,d,q}2mask*` | `slate.vector_select(k, splat(-1), 0)` / `slate.vector_sign_bitmask(v)` |
| `loaddqu{qi,hi,si,di}*_mask`, `loadup{s,d}*_mask` | `slate.masked_load(ptr, passthrough, k)` |
| `insert128i256`, `vinsertf128_*`, `insert{f,i}{32x4,64x4,32x8,64x2}*` | widen the part by shuffle, then a two-operand shuffle at `imm % parts` |
| `[u]cmp{b,w,d,q}{128,256,512}_mask` | lane compare (`imm & 7`: eq, lt, le, false, ne, ge, gt, true) on signed or unsigned lanes, `slate.vector_sign_bitmask`, `and` with the mask |
| `shufp{s,d}[256,512]` | clang's per-128-bit-lane selector: `imm * 0x01010101`, low half of each lane from `a`, high half from `b` |
| `palignr{128,256,512}` | per-16-byte shuffle of `(b, a)`; shift > 16 shifts `a` against zero, shift >= 32 is zero |
| `permti256`, `vperm2f128_*` | each 128-bit half from `a`/`b` low/high (`imm >> 4*half`), zero when bit 3 is set |
| `vfmaddsubp{s,d}512_mask{,z,3}` | `llvm.x86.avx512.vfmaddsub.*.512`, then `slate.vector_select` with `a`, zero or `c` |
| `storedqu{qi,hi,si,di}*_mask`, `storeup{s,d}*_mask` | `slate.masked_store(ptr, v, k)` |
| `pshuf{l,h}w[256,512]` | per-8-word shuffle of the low / high four words by 2-bit selectors; the other half unchanged |
| `vpermilps[256,512]` / `vpermilpd[256,512]` | same lane formula as `pshufd` / lane `i/2*2 + (imm >> i%8 & 1)` |
| `k{and,andn,or,xor,xnor,not,add,mov}{qi,hi,si,di}`, `kunpck{hi,si,di}`, `kshift{l,r}i*` | integer ops on the mask type (`add` wraps, unpack is `a << w/2 \| b & low`, constant shifts of `>= w` give zero) |
| `round{ps,pd,ss,sd}[256]`, `vcvtph2ps512_mask`, `reduce_fm{ax,in}_p{s,d}512`, 512-bit gathers and scatters, AVX-512VL `gather3{si,di}v*` and `scatter{si,di}v*` | the matching LLVM intrinsic, arguments unchanged (VL forms use the `i8`-mask `llvm.x86.avx512.gather3siv8.si` family) |
| `fpclassp{s,d}{128,256,512}_mask` | Intel's CheckFPClass on the lanes bit cast to unsigned: QNaN `abs >= inf\|quiet`, ±0 and ±Inf by equality, denormal `0 < abs < min_normal`, negative finite `sign && 0 < abs < inf`, SNaN `inf < abs < inf\|quiet`; selected classes OR'd, `slate.vector_sign_bitmask`, `and` with the mask unless all-ones |

Operands used more than once are bound once (`bind_once`), so their side
effects run exactly once.

clang NEON builtins (`__builtin_neon_*`, `Expansion::Neon`, `sema/neon_builtins.rs`)
lower to the LLVM intrinsic clang's CGBuiltin picks, so lanes, NaNs and
saturation match the hardware instruction. Polymorphic `_v` forms take
`int8x16_t`/`int8x8_t` operands and a constant NEON type code as the last
argument (`& 0xf` element: 0-3 int8..int64, 4-6 poly8/16/64, 9 f32, 10 f64;
`0x10` unsigned; `0x20` quad): operands are bit cast to the decoded vector,
the result back to the prototype's vector. Codes for f16, bf16 and poly128 fall
back to the extern call.

| Builtins | Expansion |
| --- | --- |
| `vget[q]_lane_*` | `lane` read, index modulo lanes, converted to the return type |
| `vaddv[q]_*` | f32/f64 `llvm.aarch64.neon.faddv` (pairwise), integers `llvm.vector.reduce.add` |
| `vaddlv[q]_*` | lanes widened to twice the width, then `llvm.vector.reduce.add` |
| `vmaxv`/`vminv[q]_*`, `vmaxnmv`/`vminnmv[q]_*` | floats `llvm.aarch64.neon.f{max,min}[nm]v`, integers `llvm.vector.reduce.{s,u}{max,min}` |
| `vbsl[q]_v` | `b ^ ((a ^ b) & mask)` on the bytes (falls back if an operand has effects) |
| `vabs[q]_v` | `llvm.aarch64.neon.abs` (integers, wraps at MIN), `llvm.fabs` |
| `vmax`/`vmin[q]_v`, `vmaxnm`/`vminnm[q]_v` | `llvm.aarch64.neon.{s,u,f}{max,min}`, `llvm.aarch64.neon.f{max,min}nm` |
| `vqadd`/`vqsub[q]_v` | `llvm.aarch64.neon.{s,u}q{add,sub}` |
| `vfma[q]_v(a, b, c)` | `llvm.fma(b, c, a)` |
| `vsqrt[q]_v`, `vcagt[q]_v` | `llvm.sqrt`; `llvm.aarch64.neon.facgt` (the code gives the integer result, operands are floats of that width) |
| `vld1[q]_v` / `vst1[q]_v` | `read` / `store` of the decoded vector through the pointer cast to it, `align=<element size>` |
| `vld1[q]_lane_v` / `vst1[q]_lane_v` | element `read` spliced in by a two-operand shuffle with its splat / `store` of `lane(v, n % lanes)` |
| `vld2[q]_v(&ret, p)` / `vst2[q]_v(p, a, b)` | two under-aligned vector reads of `p`, `p + 1` and even/odd shuffles into `ret[0]`, `ret[1]` / interleaving shuffles of `a`, `b` stored to `p`, `p + 1`; pointers and vectors bound once |
| `vshl[q]_n_v` / `vshr[q]_n_v` | `shl` / `shr` by a splat constant; `vshr` by the lane width is `shr` by width - 1 when signed and zero when unsigned |
| `vshl[q]_v` | `llvm.aarch64.neon.{s,u}shl` (signed per-lane count, negative shifts right) |
| `vqtbl{1,2,3,4}[q]_v` | `llvm.aarch64.neon.tbl{1,2,3,4}`: tables stay `int8x16_t`, index and result take the decoded type |
| `vrnd`, `vrndm`, `vrndn`, `vrndp`, `vrnda`, `vrndx`, `vrndi` `[q]_v` | `llvm.trunc`, `floor`, `roundeven`, `ceil`, `round`, `rint`, `nearbyint` |
| `vcvt{n,m,p,a}[q]_{s,u}{32,64}_v` | `llvm.aarch64.neon.fcvt{n,m,p,a}{s,u}` (saturating, NaN is 0); the code gives the integer result, operands are floats of that width |
| `vmovl_v` / `vmovn_v` | lane-wise extend from half width / truncate from double width (the code gives the result) |
| `vmull_v` | both operands extended to the result width, then a wrapping `mul` (what LLVM selects as `smull`/`umull`); poly codes (`vmull_p8`) fall back |

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

- clang flavor: the clang table, then the target table.
- MSVC flavor: the clang table, then the target table's `ALL_MS_LANGUAGES`
  entries only (`__cpuid`, `_umul128`, `_BitScanForward`, ...).
  - `__builtin_ia32_*` stays unknown.
  - cl.exe accepts these intrinsics with no declaration and no C4013 warning.
    So an undeclared call gets the builtin's prototype, not an implicit
    `int()`.
  - Slate binds such declarations through the libc catalog (`slate_support`)
    even though they have no system-header provenance.
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

## alloca

`alloca`, `_alloca`, `__builtin_alloca` and the `_with_align` variants are
retained calls. `Module::alloca` (`ir::AllocaLayout`, from
`AllocaLayout::for_dialect`) records how the flavor's compiler spaces them,
measured against gcc 15/16, clang 23 and cl.exe:

| Flavor and target            | `padding` | `alignment`                          |
| ---------------------------- | --------- | ------------------------------------ |
| gcc x86_64                   | 8         | `__BIGGEST_ALIGNMENT__` (16/32/64)   |
| gcc i686                     | 12        | `__BIGGEST_ALIGNMENT__`              |
| any flavor, armv7            | 0         | 8                                    |
| everything else              | 0         | 16 (clang ignores `-mavx` here)      |

Each call takes `size + padding` bytes from the top of a downward-growing
stack and rounds the new top down to `alignment`, so consecutive allocas are
the same distance apart as in the C build (`alloca(16)` twice: 16 under
clang and cl.exe, 32 under gcc x86_64). gcc with `-mavx` realigns the
stack dynamically and its spacing varies with the incoming stack address;
only the alignment matches there. `_with_align` uses
`max(bits / 8, alignment)` and no padding.

Slate does not put the memory on the real stack. The runtime
(`crates/slate/src/frontend/support/alloca.rs`) is a thread-local `Vec` of
heap blocks (1 MB, or larger for a bigger request) with a bump pointer that
moves down. Each function that calls alloca holds a `Mark` that restores the
pointer when the function returns, which gives alloca's function-scoped
lifetime. Blocks stay allocated for the thread's lifetime. Code that compares
alloca memory with real stack addresses, or relies on stack overflow to
bound it, sees the difference; this is the accepted emulation.
