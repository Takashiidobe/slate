# Calls, ABI signatures, and calling conventions

<!-- toc -->
- [Calls](#calls)
- [ABI signatures](#abi-signatures)
  - [`native_c` means trust rustc](#native_c-means-trust-rustc)
  - [`x86_win32`](#x86_win32)
  - [Vectors](#vectors)
  - [Transparent unions](#transparent-unions)
- [Calling conventions](#calling-conventions)
- [Targets](#targets)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Printed forms are in the
[grammar](../ir-grammar.md#abi-signatures); builtin calls in
[builtins](builtins.md).

## Calls

- A call names its callee by binding (`call<T>(%3, ...)`) when it resolves
  to a function or function-like builtin, or by pointer value
  (`call<T>(read<ptr<fn(..)>>(%7), ...)`). `*f` on a designator is the
  designator, so `(*fp)(x)` lowers like `fp(x)` and `(*f)(x)` stays direct
  (`ir_indirect_calls.c`).
- Each call carries the signature resolved at its own site,
  `signature=fn(...) -> T`, keeping prototype, variadic, and unprototyped
  distinctions even if a later redeclaration changes them
  (`ir_call_signatures.c`). `--compact-ir` hides it.
- Arguments carry explicit conversions (`reason=arg`); variadic arguments
  take the default promotions (`reason=vararg`). Parameter arrays and
  functions adjust to pointers.

## ABI signatures

Function declarations and calls carry an `AbiSignature` resolved from the
target ISA and ABI environment: the calling convention and a pass shape per
argument and result. Source types are never replaced with hidden pointers or
registers. Calls keep their own because an indirect or variadic call can
differ from the callee.

| Pass | Meaning |
| --- | --- |
| `scalar` | implicit in the default dump |
| `native_c` | rustc's `extern "C"` already passes the obvious `repr(C)` stand-in like the C compiler |
| `direct` | a vector passed in registers as its own type |
| `coerce<...>` | direct value pieces |
| `byval` / `byref` | copied memory argument / indirect argument |
| `sret` | indirect result |

- A declaration may name a record incomplete at that point; its ABI is
  computed at the end of the unit. A record never completed prints
  `[abi=incomplete]` (`Function::abi` is `None`). Only a definition or call
  with an incomplete type is ill-formed.
- ARM32 hard-float comes from the resolved float ABI (triple default or
  `-mfloat-abi`); hard-float variadic signatures use base AAPCS, as rustc
  does. Fixtures: `ir_call_abi.c` and per-target `abi_target.c`.

### `native_c` means trust rustc

The signature is a label for Slate, not a lowering. rustc computes coerce,
sret, and byval from a `repr(C)` layout the way clang does, so a record or
complex is `native_c` whenever the obvious stand-in (same size and
alignment, stable Rust field types, `[T; 0]` for zero-length or flexible
arrays, a same-size integer for float formats stable Rust lacks) passes like
the C compiler. An explicit shape means rustc diverges and Slate needs a
boundary conversion, or a C shim when the shape names `f16`/`f80`. Scalars,
vectors, and `void` keep their own forms; `__int128` on `win64` is `scalar`.

Divergences found by diffing `rustc --emit=llvm-ir` (`#![no_core]`) against
`clang -emit-llvm` and gcc asm. Everything else (flat, nested, packed,
over-aligned, bit-field, union, and array records; i686 records;
`x86_win32` over-aligned; `win64` records) agrees:

| Case | Conventions | Explicit shape |
| --- | --- | --- |
| `_Atomic` record/complex, clang and msvc flavors | `sysv64`, `x86_cdecl`, `x86_win32`, `aapcs64`, `win_arm64`, `aapcs32_hard_float` | memory, integer, or non-HFA form of the qualified layout |
| flexible array member, clang and msvc (gcc agrees with rustc) | `sysv64` (≤ 16 bytes), `win64` (1/2/4/8 bytes) | `byval`/`byref` + `sret` |
| zero-length or flexible array in an otherwise homogeneous float record | `aapcs64`, `win_arm64`, `aapcs32_hard_float` | integer pieces |
| `_Float16`, `__bf16`, `long double`, `__float128` members setting the register class | `sysv64`, `aapcs64`, `win_arm64` | SysV eightbyte classes or HFA |
| complex with such a component | `sysv64`, `aapcs64`, `win_arm64` | same |
| GNU empty record result | `x86_cdecl` | `sret` |
| empty record result, clang flavor only | `x86_win32` | `void` |
| register-sized record result clang returns in memory (`char[3]`, `_BitInt`, flexible array) | `x86_win32` | `sret` |
| complex result ≤ 8 bytes (gcc agrees) | `x86_cdecl` | `coerce<i16/i32/i64>` |
| homogeneous float record, `_Complex float`/`double` | `aapcs32_hard_float` on Windows | `coerce<fN...>` |
| homogeneous float record or complex float, variadic | `win_arm64` | integer pieces, `byref` above 16 bytes |

- The check classifies twice, over the C types and over the Rust stand-ins,
  and prints `native_c` when they agree. On `sysv64` that is a full
  eightbyte classifier (INTEGER/SSE/SSEUP/X87/X87UP/MEMORY, unaligned
  members and X87 arguments to memory). An SSE eightbyte of 16-bit floats
  prints as clang's vectors (`pair<f16>` = `<2 x half>`). A divergent
  record with a vector member stays `native_c`. HFA bases: every binary
  float on AArch64, only `f32`/`f64` on ARM32; GNU zero-size empty records
  are skipped.
- Fixtures: `tests/fixtures/*/*/*/abi_rust_divergence.c`.

### `x86_win32`

i686-pc-windows-msvc cdecl (clang's `X86_32ABIInfo` with
`IsWin32StructABI`):

- A fixed non-record argument with `required_align` above 4 is `byref`
  (records get the same from rustc).
- The first three vector arguments are `direct`; later ones and any wider
  than 64 bytes are `byref`.
- A record, union, or complex result of 1, 2, 4, or 8 bytes returns in
  registers only when every non-empty field is register-sized; rustc
  returns all of those in registers, so the rest are explicit `sret`. An
  empty record result is `void`.
- Fixture: `clang/windows/i686/abi_target.c`.

### Vectors

Classification follows the effective x86 ISA (`TargetInfo.x86_isa`), the
same value that drives ISA predefines. On `sysv64` "> 16 bytes" means wider
than the widest vector register (16 baseline, 32 with AVX, 64 with
AVX-512F): no wider is `direct`, wider is `byval` (clang's `-Wpsabi`).

| Convention | < 8 bytes | 8 bytes | 16 bytes | > 16 bytes |
| --- | --- | --- | --- | --- |
| `sysv64` | `coerce<iN>` | `coerce<f64>` | `direct` | arg `byval`, result `direct` |
| `win64` | `direct` | `direct` | `direct` | `direct` |
| `x86_cdecl` | `direct` | arg `coerce<i64>` if MMX | `direct` | `direct` |
| `x86_win32` | `direct` | `direct` | `direct` | arg `byref` past 64 bytes or 3 vectors |
| `aapcs64`/`win_arm64` | arg `coerce<i32>` | `direct` | `direct` | arg `byref`, result `sret` (align 16) |
| `aapcs32`(`_hard_float`) | arg `coerce<i32>` | `direct` | `direct` | arg `direct`, result `sret` (align 8) |

- Results are `direct` where the table coerces only arguments.
- gcc corners: `vector<f64, 1>` is `byval<align=8>` on `sysv64` (result
  `direct`); an 8-byte vector of lanes under 64 bits is MMX and passes as
  `i64` on `x86_cdecl`, while `vector<i64, 1>` and float lanes pass
  directly.
- Fixtures: `ir_vector_abi.c`, `ir_vector_abi_avx.c`,
  `ir_vector_abi_avx512f.c`, and each target's `abi_target.c`.

### Transparent unions

A fixed `transparent_union` parameter keeps the union type but is classified
from its first member (`AbiClassifier::transparent_first_member`), as clang
and gcc do. A call argument that isn't the union converts silently to the
first member it matches (`TypeResolver::transparent_member`) and lowers as
`aggregate<U, zero_fill=false>(fieldN = value)`. The attribute is dropped
when the first member is float or vector, or a member differs in size or
needs more alignment. Rust's `repr(C)` union doesn't pass like its first
member, so Slate must pass that member's type at the boundary
(`transparent_union_call.c`).

## Calling conventions

- `__stdcall`, `__fastcall`, `__vectorcall`, `__thiscall` (keywords or GNU
  attributes) are part of the function type on 32-bit x86, and
  `__vectorcall` on x86-64 too. `CTypeKind::Function` and
  `ir::Type::Function` carry a `CallConv` (`fn stdcall(i32) -> i32`), and
  `AbiSignature.calling` repeats it, so a call keeps it in compact output.
  `__cdecl` is the default.
- Placement follows clang: a specifier or trailing attribute goes to the
  innermost function declarator (or a function typedef in the specifiers);
  one on a pointer or grouped declarator goes to the function type beneath.
- Dropped as clang does: on variadic functions, the other three on x86-64,
  all four on other targets.
- Types differing only in convention are incompatible. A redeclaration
  without one inherits it; adding or changing one is rejected (all three
  compilers).
- The convention is recorded, not modeled: stack cleanup, `inreg`
  registers, vectorcall XMM/HVA assignment, and decoration (`_f@8`,
  `@f@8`, `f@@8`) are left to rustc's `extern "stdcall"` etc. Pass shapes
  are the platform's cdecl ones. Known gap: x86-64 `vectorcall`, where
  rustc matches neither clang shape (`slate-parser-x74n`).
- Fixtures: `clang/{windows,linux}/i686/calling_conventions.c`,
  `clang/linux/x86_64/calling_conventions_ignored.c`,
  `clang/{windows,linux}/x86_64/calling_convention_vectorcall.c`,
  `error/clang/.../calling_convention_*.c`.

## Targets

Target selection separates CPU family (`TargetFamily`), OS (`TargetOs`), and
ABI environment (`TargetEnvironment`) from compiler flavor; the triple never
selects the flavor. How to add one: [adding-a-target](../adding-a-target.md).

- Linux profiles use GNU, GNU EABI, or GNU EABI hard-float environments.
- `{x86_64,aarch64,i686,thumbv7a}-pc-windows-msvc`: 32-bit `long`, binary64
  `long double`, unsigned 16-bit `wchar_t`, Microsoft record layout.
  - i686: `long long`/`double` 8-aligned (4 on i686 Linux), 4-byte pointers,
    stack alignment 4, `x86_win32`.
  - thumbv7a: signed `char`, 4-byte pointers, stack alignment 8, `char *`
    va_list, `aapcs32_hard_float`.
  - `--flavor=msvc` loads the MSVC 19.51 snapshots (19.44 for thumbv7a);
    clang loads clang 22 Windows snapshots; gcc is rejected.
- Still incomplete on Windows: compiler-option validation, native-MSVC
  standard-mode macros, and SDK/header integration.
