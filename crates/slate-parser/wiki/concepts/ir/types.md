# IR types, enums, and record layout

<!-- toc -->
- [Shown type vs C type](#shown-type-vs-c-type)
- [Scalar formats](#scalar-formats)
  - [Integer literals](#integer-literals)
  - [`_BitInt`](#_bitint)
- [Tags](#tags)
- [Enums](#enums)
- [Records](#records)
  - [Alignment](#alignment)
  - [`#pragma pack` and `ms_struct`](#pragma-pack-and-ms_struct)
  - [Microsoft record layout](#microsoft-record-layout)
- [Deduced and derived declarator
  types](#deduced-and-derived-declarator-types)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Complex, imaginary, vector, and
fixed-point types are in [type families](type-families.md); pointers in
[places and pointers](places-pointers.md); `_Atomic` layout in
[atomics](atomics.md).

## Shown type vs C type

The shown type is concrete and target-resolved; the C type is metadata
([pipeline](pipeline.md#c-type-metadata)).

| C | Shown | Metadata |
| --- | --- | --- |
| `int` | `i32` | `c=int` |
| `long` (LP64 / LLP64) | `i64` / `i32` | `c=long` |
| `char` / `signed char` / `unsigned char` | `i8`/`u8` per target, `i8`, `u8` | `c=char` etc. |
| `_Bool` | `bool` | `c=_Bool` |
| `size_t` | `u64` | `c=size_t`, `c_canon=unsigned long` |
| `float` / `double` | `f32` / `f64` | |
| `long double` (x86) | `f80` | |
| `__float128` | `f128` | |
| `_Decimal32/64/128` | `d32` / `d64` / `d128` | |
| `_BitInt(128)` | `i128b` | `c=_BitInt(128)` |
| `const char *` | `ptr<const i8>` | `c=const char *` |
| `enum E` / `struct S` | `@typeN` | layout in the module |

- The whole typedef chain is kept (`uint32_t` → `__uint32_t` →
  `unsigned int`): it is the strongest idiomization signal.
- C-level distinctions (`char` vs `signed char`, `long` vs `long long` on
  LP64) live in sema operands and places and are erased only when emitting
  IR, so `_Generic` still sees them.
- Pointers, arrays, and function signatures print inline.
  `Type::Defined(TypeId)` references named or recursive definitions and
  keeps incomplete-type identity.

## Scalar formats

- `bool` is its own type, not a one-bit integer. Comparisons, `!`, `&&`,
  and `||` produce `bool` while their sema C type stays `int`.
- Nine float formats: binary `bf16`, `f16`, `f32`, `f64`, `f80`, `f128`,
  decimal `d32`, `d64`, `d128`. Value width is not storage size: `f80` is
  not a promise of a 10-byte object.
- `long double` resolves through `TargetInfo.long_double`
  (`-mlong-double-64/80/128`). `f64x` follows `target.float64x_format()`.
- `bf16` (`__bf16`) is a truncated f32 (8-bit exponent, 8-bit mantissa),
  incomparable with `f16`; `FloatType::widens_from` compares both widths, so
  a conversion either way is `float_narrow`. Rank is below `_Float16`
  (`__bf16 + _Float16` is `_Float16`), and neither takes default argument
  promotions. The `bf16`/`BF16` literal suffix is accepted in every flavor
  because gcc's predefines use it; in `FLOAT_SUFFIXES` it must precede
  `f16` (first `ends_with` match wins).
- Decimal floats: size and alignment 4/8/16 everywhere. Literals keep their
  digits (`const<d32>(1.5)`) and never fold. Mixing decimal and binary
  floats is rejected (C23 6.3.1.8); integers convert to the decimal format;
  casts between the families are `float_convert`.
- Float constants keep exact bits. f32/f64 print as round-trippable
  decimals (signed zero included); other formats print via `rustc_apfloat`;
  NaNs print as hex bits in every format.

### Integer literals

- Candidate order is standard- and target-dependent;
  `select_integer_candidate` is the single selection point, shared with
  validation and with the literal warnings in
  [diagnostic-severity](../diagnostic-severity.md).
- C89 (`long_long_type = Extension`) lets an unsuffixed or `l` decimal
  become `unsigned long` before the `long long` tail; C99 reaches
  `long long` first. They differ only on ILP32 and LLP64.
- A decimal that fits no signed type: clang and MSVC flavors take the
  unsigned form of the widest rank. The gcc flavor
  (`widest_integer_literal_fallback`) takes gcc's widest type, signed unless
  `u`-suffixed (`__int128` on 64-bit, else `long long`, so
  `18446744073709551615` is `long long` -1 on i686).
- A literal wider than `long long` is truncated first, as cpplib does, with
  `integer-literal-too-large`; clang rejects these, so every flavor takes
  gcc's value.

### `_BitInt`

- Distinct from the standard integer of the same width; prints with a `b`
  suffix. On x86-64 `__int128` is `i128` (16/16) and `_BitInt(128)` is
  `i128b` (16/8). arm32 rejects `__int128` but accepts `_BitInt(128)`.
  Layout: `ScalarLayouts::bit_precise`.
- Rules apply in every standard mode: clang and gcc accept `_BitInt` as an
  extension before C23 with identical results.
- `wb`/`uwb` literals take the narrowest fitting width (`1wb` is `i2b`,
  `0uwb` is `u1b`, `0xffwb` is `i9b`); radix never makes them unsigned.
  Above 65535 bits is an error.
- Exempt from integer promotions and default argument promotions:
  `_BitInt(8) + _BitInt(8)` stays `i8b`, and a variadic `_BitInt(8)` passes
  as `i8b`.
- At equal width the bit-precise type ranks below the standard one:
  `_BitInt(32) + int` is `int`; `_BitInt(40) + int` is `i40b`. The
  `i32b`↔`i32` change still emits an exact `reinterpret`.
- Fixtures: `ir_bitint_literals.c`, `ir_bitint_conversions.c` (clang 22 and
  gcc 16 agree; MSVC has no `_BitInt`).

## Tags

- Tag identity is per scope, not per name. A block-scope `struct Local`
  in two functions is two `TypeId`s. A bare `struct S;` declares a new
  incomplete tag in the current scope (C11 6.7.2.3p8), hiding an outer one.
  A forward declaration plus a definition in the same scope completes one
  tag (`ir_tag_scopes.c`). A block-scope `enum E : T;` also shadows.
- Redefinition in the same scope is an error, except C23 compatible
  redefinitions (same members), which reuse the first definition
  (`compatible_tag_redefinitions`).
- Block-scope typedefs follow the `TypeResolver` scope stack; aliases stay
  in the module's flat type table.

## Enums

- Enumerators enter scope as soon as their value is known, so a later one
  sees an earlier one with its type (`enum { A = 1, B = sizeof(A) }`).
- While the enum is open (C23 6.7.2.2, applied by gcc and clang in every
  mode): `int` when the value fits, else the promoted type of its
  expression; an implicit `previous + 1` keeps the previous type and widens
  within its signedness on overflow (`INT_MAX, B` makes `B` a `long`).
- Fixed underlying type: every constant has the enum type (the fixed type
  before C23), must fit, and the enum is complete before its first
  enumerator.
- Underlying type without a fixed one: from the value range; signed if any
  value is negative, first of `int`, `long`, `long long` that fits
  (`enum { A, B }` is `u32`, `{ A = -1, B = 1ll << 40 }` is `i64`).
- Enumerator type after the enum closes (clang): `int` if no fixed type and
  every value fits `int`; otherwise the underlying type before C23 and the
  enum type from C23 (`enumerators_have_enum_type`). Known gap: clang keeps
  a C23 enumerator at its own type when it equals the underlying type
  (`enum P { P0 = 0x80000000 }` stays `unsigned int`); we use the enum type.
- Enum values cross into arithmetic through `enum_to_int` and back into
  storage through `int_to_enum`, typedef chains included
  (`ir_enum_typing_c89.c`, `..._c23.c`).
- Values beyond `int`, a fixed type, and a trailing comma are only extension
  warnings before C23, so no mode rejects them.
- Enum storage keeps size and alignment separately, so alignment attributes
  don't change the underlying integer type.

## Records

- Fields keep source order and names; unnamed members and zero-width
  bit-fields stay in the list; anonymous members print `<anonymous>`.
- Layout records size, alignment, one byte offset per field, bit offsets for
  bit-fields, and byte extents of bit-field storage units.
  `tools/check_record_layout.py` diffs each target's layout fixture against
  clang's record-layout dump.
- ARM lets zero-width bit-fields raise record alignment, packed records
  included; x86 does not.
- A trailing flexible array member (or, GNU, an incomplete array anywhere
  in a union) has size 0 and the element's alignment. Its initialized extent
  lives on the initializer ([initialization](initialization.md#flexible-array-members)).

### Alignment

- `packed`, `aligned`, and `_Alignas` apply, including local field
  alignment.
- An aligned typedef stays structural (`typedef int A
  __attribute__((aligned(16)))` is still `i32`). Its alignment lives on the
  sema `Typedef` C type and reaches the IR only where alignment is already
  recorded: field placement, `align_of` constants, and `[align=N]` on
  globals and locals. Unlike a declaration-site request it replaces natural
  alignment and may lower it; the outermost aligned typedef in a chain wins,
  arrays inherit it from their element, `_Atomic` drops it, and an array
  whose element size isn't a multiple of it is rejected. Access alignment
  through pointers isn't modeled (loads and stores carry none).
- x86-64 psABI large arrays (Linux, Darwin, Windows): a declared array
  object of at least 16 bytes is 16-aligned (globals, `extern`s, static and
  automatic locals, inferred lengths). A declaration-site `aligned` or
  `_Alignas` suppresses it; a typedef's alignment does not. Not applied to
  records, incomplete arrays, VLAs, string literals, or compound literals.
  It is observable across units (clang emits `align 16` on
  `extern char x[32]`), so Rust must honor it. gcc's extra speed
  over-alignment is not modeled.

### `#pragma pack` and `ms_struct`

- `#pragma pack(N)` caps field alignment: `min(max(natural_or_packed,
  aligned_attr), N)`. It caps a field's `aligned`, not the record's, so
  `pack(1)` plus a record `aligned(16)` is a 16-aligned record of packed
  fields. A pack that isn't a small power of two is diagnosed and ignored
  (the previous value stays). While any pack is active, bit-fields stop
  padding to avoid straddling a unit, as `packed` does.
- `ms_struct` bit-field rules apply with `__attribute__((ms_struct))`, or
  under `#pragma ms_struct on` without `gcc_struct`:
  - each bit-field allocates a whole unit of its declared type, aligned to
    its size; later same-size bit-fields fill it until one doesn't fit;
  - a non-bit-field after bit-fields starts after the whole unit;
  - a zero-width bit-field is ignored after a non-bit-field, and after a
    bit-field it ends the unit and raises record alignment to its size;
  - `packed` doesn't reduce bit-field alignment, `#pragma pack` caps it;
  - in a union, bit-fields have alignment 1 and occupy their full size;
  - scalar fields (not enums, complex, `_BitInt`, `_Atomic`) align to their
    size (i686 `long long` becomes 8); a non-power-of-two scalar such as
    i686 `long double` is an error, as in clang.
  - Oracle: clang's ItaniumRecordLayoutBuilder `IsMsStruct` path.

### Microsoft record layout

Every `*-windows-msvc` triple uses clang's `MicrosoftRecordLayoutBuilder`,
under both clang and MSVC flavors, and `ms_struct`/`gcc_struct` make no
difference. Differences from Itanium and `ms_struct`:

- Bit-fields share a unit only when their declared types have the same size
  and the next fits (`struct { char a : 4; int b : 4; }` is 8 bytes). In a
  union they take their type's size but don't raise alignment
  (`union { int a : 3; }` is 4/1).
- A zero-width bit-field matters only right after a non-zero-width one:
  it rounds to its type's alignment and raises record alignment. Elsewhere
  it is ignored (`struct { char a; int : 0; char b; }` is 2 bytes).
- Record `packed` acts like `#pragma pack(1)`, bit-fields included. A
  `#pragma pack` wider than a pointer is ignored.
- Required alignment (field `aligned`/`__declspec(align)`/`_Alignas`, an
  aligned typedef, or a nested record's own) is not capped by pack. On a
  bit-field it raises that field's alignment. It is recorded as
  `required_align`, which the Win32 argument ABI reads.
- A record with no storage (empty, only zero-width bit-fields, only
  zero-length or flexible arrays) is 4 bytes, or its alignment if required
  alignment is at least 4.
- Oracle: `clang -Xclang -fdump-record-layouts` per triple, cross-checked
  with `tools/cl.exe` (`{clang,msvc}/windows/*/ms_record_layout.c`).

## Deduced and derived declarator types

- Declarator derivation visits prefix pointers and arrays before suffix
  function and array forms: `int *f(void)` is `fn() -> ptr<i32>`,
  `int *a[3]` is `array<ptr<i32>, 3>`, and grouped `int (*f)(int)` wraps
  the suffix type in the grouped core.
- `typeof`/`typeof_unqual` resolve in sema for types and expressions.
  Expression operands keep array and function types, emit nothing, and roll
  back temporary bindings and string globals, except that a variably
  modified operand with effects is evaluated (C23 6.7.3.6) as a synthetic
  temporary: `typeof(++i, (int (*)[i])a) q` increments `i` and gives
  `ptr<vla<i32, %i>>`. `typeof_unqual` drops outer qualifiers including
  `_Atomic`, keeping pointee ones (`ir_typeof.c`,
  `ir_typeof_qualifiers.c`).
- `__auto_type` and C23 `auto` resolve entirely in sema through the typeof
  path; the IR sees only the deduced type.
  - The deduced type is the initializer's lvalue-converted type (arrays and
    functions decay; `const`, `volatile`, `restrict` dropped) plus the
    specifier qualifiers. `_Atomic` is kept under clang, dropped under gcc.
  - Pattern declarators as in clang: `auto *p = cip` deduces `const int`;
    `auto (*fp)(int) = g` must match exactly; a top-level array is an error.
  - Several declarators must deduce the same type. gcc flavor: one plain
    identifier only.
  - Errors: no initializer, a braced one, `typedef`, `void`, and the name in
    its own initializer. A bit-field initializer is an error, except C23
    `auto` under gcc, which deduces an internal bit-field type slate can't
    represent (`Unimplemented`).
  - `auto n = nullptr` deduces `nullptr_t`; `auto *p = nullptr` is rejected
    as in clang (`c23_auto_inference.c`, `gcc_auto_inference.c`).
