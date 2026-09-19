# C type layer

Sema reasons over C types; `ir::Type` is only produced by erasing one through
`layout`. The layer lives in `src/sema/ctype/` (epic slate-parser-9ve).

## Representation

- `CTypes` interns `CTypeKind` into `CTypeId`s. A `QualType` is an id plus
  `Qualifiers` (const, volatile, restrict, `_Atomic`).
- Sugar kinds: `Typedef{name, underlying}`, `TypeOf{spelling, underlying}`,
  `AtomicSpecifier(inner)`. They exist for spelling and `typedef_chain` only.
- Every entry stores its canonical `QualType` at intern time, so
  `canonical(q) = entry.canonical ∪ q.quals` never mutates the interner.
  - `_Atomic(T)` canonicalizes to `T` with the atomic qualifier.
  - Array qualifiers are pulled up (C23 6.7.3p10): a canonical array's quals
    mean its element's quals and the canonical element is unqualified.
  - A canonical function type has adjusted (array/function → pointer),
    top-level-unqualified parameters; the `Function` kind keeps parameters
    as written for spelling.
- `unqualified` keeps sugar when no hidden qualifiers exist, otherwise
  desugars (rebuilding arrays with an unqualified element). This is what
  `typeof_unqual` and lvalue conversion use.
- Integer kinds carry a rank (`Short`..`Int128`), not a width; `char`,
  `signed char`, `unsigned char`, `long` and `long long` stay distinct even
  when their widths coincide. `__fp16` is distinct from `_Float16`.

## Rendering

`render.rs` is a clang TypePrinter-style declarator printer with a spelling
mode and a canonical mode. Conventions: `int *`, `char *const`,
`int (*)(int)`, `int[3]`, `int(int)`, `(void)`/`()`/`...`, qualifier order
`const volatile restrict _Atomic`. Tags print by their definition name.

## Layout

`layout.rs` erases a C type to `ir::Type`. Pointer `is_const`/`access` come
from the pointee's canonical qualifiers; parameters use the adjusted pointer;
enums and records map to `Defined(id)`; `long double` follows the target.

## Typed lowering

`sema::operand::Operand` pairs an IR value with its `QualType`; `Lvalue`
pairs an IR place with its declared `QualType`. Bit-field storage and width
remain in the place's `BitFieldAccess`. Binding and enumerator tables carry
C types directly. The access map needed by effects normalization is derived
from binding qualifiers once, after lowering.

An operand's IR type is the layout of its C type, except that comparisons,
`!`, `&&`, and `||` have C type `int` and use IR `bool`. Integer uses emit
`from_bool`; conditions keep the boolean representation. Consequently
`_Generic(a < b, int: 1, _Bool: 0)` selects `int`, and `sizeof(a < b)`
uses the size of `int`.

Runtime expressions and required constant expressions share typed arithmetic
in `operand.rs`. `ctype/arith.rs` chooses C result types before `numeric.rs`
emits operations and conversions. There is no reverse mapping from layouts
to C types, auxiliary binding-type map, or separate scalar cast resolver.
`typeof` reads the lvalue or operand type without reconstructing it from the AST.

| Rule | Implementation | C23 section |
| --- | --- | --- |
| Lvalue conversion and array/function decay | `CTypes::lvalue_conversion` | 6.3.2.1p2–4 |
| Address of a qualified object | `Lowerer::expr`, pointer to `Lvalue.c` | 6.5.3.2 |
| Member qualification | `Lowerer::field_place`, field qualifiers union base qualifiers | 6.5.2.3p3–4 |
| Integer and bit-field promotions | `CTypes::integer_promotion` | 6.3.1.1p2 |
| Default argument promotions | `CTypes::default_promotion` | 6.5.2.2p6 |
| Usual arithmetic conversions | `CTypes::usual_real_type` | 6.3.1.8 |
| Pointer difference | `Lowerer::binary`, unqualified compatible pointees and `ptrdiff_t` result | 6.5.6 |

Standard integer ranks remain distinct at equal widths; a standard integer
outranks a bit-precise integer of the same width. Enums promote through their
compatible integer type. Floating components retain their C kind even when
their layouts coincide. Complex and imaginary result domains are chosen
separately from component rank.

`size_t` and `ptrdiff_t` use the supported targets' predefined integer ranks:
`long` on LP64, `long long` on LLP64, and `int` on ILP32, with the appropriate
signedness. Character and string literal types retain plain `char`, signed
character kinds, and encoding-specific integer types.

Clang 22.1.8's emitted LLVM IR confirms that `_Float16` stays `half` in a
variadic call; `float` and `__fp16` promote to `double`. The earlier 9ve.2
design note claiming `_Float16` promotes to `double` was incorrect.

## Remaining phases

Conversion legality and redeclaration merging still contain layout-based
decisions. The typed conversion entry points deliberately retain those
decisions for 9ve.3 and 9ve.4; this phase establishes the C-type data flow
they need without broadening the set of rejected programs.
