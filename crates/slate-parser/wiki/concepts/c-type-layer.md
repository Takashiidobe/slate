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

## Transitional pieces

`TypeResolver::reverse_layout` maps an `ir::Type` back to a C type for
expression `typeof` fallbacks that are not yet typed. It is lossy (width →
rank) and goes away with typed lowering (slate-parser-9ve.2).
