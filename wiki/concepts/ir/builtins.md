# Builtins

<!-- toc -->
- [Registry](#registry)
- [Builtins as functions](#builtins-as-functions)
  - [Redeclared builtins (clang
    flavor)](#redeclared-builtins-clang-flavor)
  - [MSVC implicit declarations](#msvc-implicit-declarations)
- [Custom lowering](#custom-lowering)
- [Recognized by callee name](#recognized-by-callee-name)
  - [`va_list`](#va_list)
  - [Source location and function
    names](#source-location-and-function-names)
  - [Constant queries](#constant-queries)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Atomic builtins are in
[atomics](atomics.md); how builtin calls print is in
[calls and ABI](calls-abi.md#calls).

## Registry

- Clang's `Builtins.td`, `BuiltinsX86.td`, and `BuiltinsX86_64.td` are
  expanded through `clang-tblgen` into one table each in
  `src/sema/clang_builtins.rs` (`CLANG_BUILTINS`, `CLANG_X86_BUILTINS`,
  `CLANG_X86_64_BUILTINS`), sharing one prototype pool. The generator parses
  each prototype into a typed `BuiltinPrototype` and emits
  `BuiltinAttribute`/`BuiltinLanguage` enums; an unparseable prototype fails
  generation.
- Target tables apply only under the clang flavor: x86 gets the x86 table,
  x86-64 both. `__builtin_ia32_*` and `__rdtsc` are unresolved on other
  targets and under gcc and MSVC (`clang/linux/x86_64/ir_x86_target_builtins.c`,
  `error/clang/linux/aarch64/x86_target_builtin.c`). `Features` is recorded
  but not checked.
- `_Vector<N, T>` prototypes are GNU `vector_size` vectors (`vector<T, N>`).
- `TypeResolver::builtin_signature` derives the call signature. None is
  derived (and the call reports an unsupported builtin) for
  `CustomTypeChecking`, variadic prototypes with no named parameters, and
  prototypes naming types outside the model (`FILE`, `jmp_buf`, ObjC `id`,
  HLSL resources, C++ references, ext-vectors).
- Named types resolve to the target's canonical types, not to typedefs the
  unit declares.

## Builtins as functions

Builtins look like functions, as clang's lazily created `FunctionDecl`s do.

- A builtin used without a declaration gets one implicit `fn` per spelling
  with unnamed parameters and the registry prototype
  (`fn %9 @__builtin_abort() -> void [linkage=external] [noreturn]`).
  Every builtin function carries `c_builtin` metadata. A builtin used only
  inside unevaluated `sizeof`/`_Generic` leaves no declaration.
- That implicit declaration redeclares any function of the same name with
  linkage: it binds to an earlier block-scope `extern` or a later one at any
  scope, and the unit gets one `fn`. Calls before the later declaration keep
  the builtin's signature (`gcc/.../ir_implicit_builtin_redeclared.c`).
- Attributes: `NoReturn` → `[noreturn]`; `Const`/`Pure` →
  `[memory=none]`/`[memory=read]`, like GNU `const`/`pure` (`const` wins).
  `ConstIgnoringErrno` doesn't count; `NoThrow` is dropped (C without
  `-fexceptions` never unwinds) (`ir_builtin_noreturn.c`).

### Redeclared builtins (clang flavor)

- An ordinary declaration keeps builtin status when, as in clang, it has
  external linkage and a type compatible with the builtin (an unprototyped
  `int abs();`, a `const` parameter, or a missing `noreturn` still match).
  Calls use the builtin's signature, and the declaration gains `noreturn`
  and `c_builtin`.
- An incompatible or `static` declaration shadows the builtin. With
  external linkage it still inherits `noreturn` (clang merges it into the
  function type: `int exit(long);` is `[noreturn]`), but not `Const`/`Pure`
  (`ir_redeclared_builtins.c`).
- gcc (`builtin_prefixed_library.c`) and MSVC keep compiling after such a
  call.
- MSVC flavor: library builtins (`ClangBuiltinKind::Library`: `exit`,
  `abort`, `toupper`, `cbrt`) get neither `noreturn` nor `Const`/`Pure`,
  declared or implicit; cl.exe learns noreturn only from
  `__declspec(noreturn)`. They keep `c_builtin`, which names the libc
  entity rather than claiming semantics (`msvc/linux/x86_64/ir_library_builtins.c`).
- Header provenance plays no part: it decides libc identity for Rust, not
  builtin semantics.

### MSVC implicit declarations

Under the MSVC flavor, calling an undeclared non-builtin identifier declares
`extern int name()` at file scope, as cl.exe does (C4013, level 3, so
`-Wimplicit-function-declaration` is off by default). The unit gets one
unprototyped `fn` with `c_implicit` metadata before the first calling item;
later calls reuse it, a compatible later declaration merges, and an
incompatible one is the C2371 conflicting-types error. Only call targets are
declared this way; clang and gcc keep rejecting
(`msvc/windows/i686/implicit_function_declaration.c` and its error
variants).

## Custom lowering

Builtins whose result can't come from a prototype dispatch on their tblgen
record through `builtins::custom_builtin`, which returns a typed
`CustomBuiltin` rather than matching spellings at the call site.

| Builtin | IR |
| --- | --- |
| `isnan`, `isinf`, `isfinite`, `isnormal`, `issubnormal`, `iszero`, `issignaling`, `signbit` | `from_bool<int>(float_class<bool, test=..>(x))`, a non-trapping class test |
| `isinf_sign` | nested `conditional<int>` over two class tests (-1/0/1) |
| `isgreater`, `isgreaterequal`, `isless`, `islessequal` | the ordinary comparison with `exceptions=ignore` |
| `isunordered`, `islessgreater` | two quiet tests joined by bitwise `or<int>` (both operands always evaluate; `islessgreater` is `lt \| gt`, false for NaN) |
| `__builtin_complex(re, im)` | `aggregate<complex<T>>` |
| `__builtin_add/sub/mul_overflow` | `overflow_add/sub/mul<bool>(l, r, place)`: math-domain result stored through `place`, returns whether it overflowed |
| `__builtin_bit_cast` | `bit_cast<T>` |
| `__builtin_shufflevector`, `__builtin_convertvector` | see [vectors](type-families.md#vector) |

Fixture: `ir_implicit_builtins.c`.

## Recognized by callee name

These are clang keywords rather than `Builtins.td` records, so sema matches
the callee name.

### `va_list`

- `__builtin_va_list` follows clang's per-target `BuiltinVaListKind`
  (`TargetInfo::va_list_kind`). Where clang makes it `char *` (Windows,
  i686, aarch64 Darwin) it is exactly `char *` and the va builtins take a
  `ptr<i8>` place. Elsewhere it is the opaque `va_list` type: 24/8 on
  x86-64 SysV, 32/8 on AAPCS64, pointer-sized on 32-bit ARM. The ABI's
  array-to-pointer decay of a `va_list` parameter is not modeled; it passes
  as one handle.
- `__builtin_va_arg(ap, T)` is `va_arg<T>(place)`, a reading effect that
  advances the list; `T` may be a record (`ir_va_arg.c`).
- `va_start(place)`, `va_end(place)`, `va_copy(dest, src)` are void effect
  values; `va_start`'s last-parameter argument is dropped
  (`ir_va_start_end_copy.c`).

### Source location and function names

- `__builtin_LINE`/`COLUMN` fold to `int` constants; `__builtin_FILE`,
  `FILE_NAME`, `FUNCTION` become an internal `.strN` global. The position
  comes from the callee token's `Loc` via `Files::position`, not from
  `Provenance`: the preprocessor stamps one `Provenance` per logical line,
  so its column is always the line's first token.
- `__func__`, `__FUNCTION__`, `__PRETTY_FUNCTION__` are internal `char[N]`
  lvalues (so `sizeof`, indexing, decay work), bound in `Lowerer::place`
  ahead of name resolution, one `.strN` per occurrence. gcc spells
  `__PRETTY_FUNCTION__` as the bare name; clang spells the declaration
  (`unsigned long n(int)`, via `CTypes::declaration_spelling`). Outside a
  function: `""`, and `"top level"` for the pretty form
  (`ir_function_name_builtins.c`, `ir_function_name_builtins_gcc.c`).

### Constant queries

- `__builtin_constant_p(x)` → `const<i32>(0|1)` with `c_builtin` metadata;
  the operand is not evaluated. 1 when the lowered operand folds, else 0
  (clang `-O0`'s answer). It is an integer constant expression.
- `__builtin_types_compatible_p(A, B)` → `const<i32>(0|1)` with
  `types_compatible="A, B"`, answered by `CTypes::compatible` (6.2.7), the
  predicate redeclaration merging uses. Top-level and element qualifiers
  are ignored; distinct C integer types sharing an IR type are
  incompatible; an enum matches its underlying type but no other enum;
  `int[]` matches `int[5]`; `int(*)()` matches `int(*)(int)` before C23 but
  not from C23 (verified with clang 22 and gcc 16).
- `__builtin_choose_expr`: see [control flow](control-flow.md#resolved-away-before-the-ir).
