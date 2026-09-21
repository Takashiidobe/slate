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

`TypeResolver::layout` returns `None` for `void` and `object_type` turns that
into an error, so any site that needs storage rejects void. `ir_type` does not:
the places where a void type is legal but has no storage call it directly —
`define_alias` (`typedef void f;`), the `extern` arm of a global declaration
(`extern void _text;`, a GNU idiom for a linker symbol) and the two `sizeof` /
`_Alignof` sites. `sizeof(void)` and `_Alignof(void)` are 1, and
`require_pointer_element` accepts `void` so `void *` arithmetic works; both are
the same GNU extension (`-Wgnu-pointer-arith` in clang, `-Wpointer-arith` in
gcc, pedantic-only in each), and slate-parser does not yet emit that warning.
Do not give `TargetInfo::storage_of(Type::Void)` a size instead: object,
field, parameter and `va_arg` paths all reach it and must keep failing
(slate-parser-dyd.6).

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

## Compatibility, composite types and conversions

`src/sema/ctype/compat.rs` transcribes 6.2.7: `compatible` (qualifiers must
match at each level), `compatible_unqualified`, and `composite`. Typedefs are
transparent because comparison is on canonical types; an enum is compatible
with its underlying integer type; arrays are compatible when their elements
are and their sizes are equal or either is unknown, which makes a VLA
compatible with any array of compatible element (6.7.6.2p6); functions compare
return types, and an unprototyped declaration is compatible with a
non-variadic prototype whose parameters are unchanged by the default argument
promotions. `merge_pointer` builds the conditional operator's result: the
composite of the two pointees carrying the union of both qualifier sets, with
`void` winning.

`src/sema/ctype/convert.rs` holds `classify_conversion`, which is the single
decision point for whether any conversion is legal. It takes the two C types,
a `ConversionContext` and whether the source is a null pointer constant, and
returns a `CastKind` plus an optional warning, or a `ResolveError`. Lowering
emits the kind and nothing else, so an unhandled case is now a rejection
rather than a silent `pointer_cast`. `ConversionContext` has no `Init` arm:
initialization reaches lowering as `ConversionReason::Assign` and the two
obey the same constraints, so a separate arm would be unreachable.

The layout-approximation helpers this replaces — `pointer_conversion_warning`,
`differ_only_in_sign`, `differ_only_in_nested_qualifiers` and
`compatible_ignoring_qualifiers` — are gone. Because signedness is now
compared on C types rather than IR widths, plain `char` is distinct from
`signed char` (slate-parser-4o9) and nested pointer levels are compared as
carefully as the first.

Taking the address of a `register` variable is rejected during expression
lowering, using the storage class recorded on its binding (slate-parser-zm8).

## Redeclaration merging

`TypeResolver::declared` holds the merged C type of each redeclared entity,
keyed by `BindingId`, alongside `bindings` (which holds the type currently in
scope). `merge_redeclaration` is called by both `declare_global` and
`declare_function` in `src/sema/module.rs`, *before* either looks for an
existing entry — the first declaration has to register its type, or the second
one looks like a first and the conflict goes unnoticed.

Compatible declarations merge into their `composite`, which is how
`int a[]; int a[5];` completes without a special case. Otherwise the conflict
table in [`ir-spec.md`](ir-spec.md) applies, and the only IR-level question
left is whether the two layouts coincide: `types::same_layout` answers it,
comparing lowered types while ignoring integer signedness. That is a genuine
layout question, not a type-identity one, which is why it survives the phase
that removed `types::compatible`, `same_layout_ignoring_sign` and
`function_redeclaration_conflict`.

`__builtin_types_compatible_p` calls `CTypes::compatible` directly, so it and
redeclaration merging cannot drift apart.

## The invariant, and where it is allowed to bend

The rule the layer exists to enforce: **no `ir::Type` equality or shape decides
C type identity**. Compatibility, conversions, redeclaration merging,
`_Generic` selection and `__builtin_types_compatible_p` all answer through
`CTypes`, and `ir::Type` is produced only by `layout()`.

The audit behind the table below is

```
grep -rn 'ty ==\|ty !=\|== Type::\|!= Type::' src/sema/
```

Every hit is listed here; re-run it after touching sema and account for any new
one. `ir::Type` is still compared in `src/sema`, and those comparisons are fine
because they ask a layout or representation question, not an identity one:

| Site | Question | Why it is allowed |
| --- | --- | --- |
| `numeric.rs`, `expression.rs::emit_cast` | `value.ty == to`, `ty == Type::Bool` | IR emission: has this value already got the representation we are about to build? `CastKind::Identity` relies on it, because a truth value has IR `Bool` and C type `int` |
| `fold.rs` | operand vs value type | constant folding over IR values |
| `effects.rs`, `atomic.rs` | `== Type::Void`, `== Type::Bool` | effect and atomic normalization over IR |
| `expression.rs` | `!= Type::VaList` | `VaList` is an IR marker type with no C-level counterpart |
| `types.rs::same_layout` | do two lowered types share a shape, ignoring integer signedness? | deliberately a layout question — it decides warning-vs-error for redeclarations, per MSVC's own C4142/C2371 rule |

One genuine exception remains. `same_tag_content` compares `ir::Type` field
types as a structural pre-filter for C23 compatible tag redefinitions. The
C-level question is answered by `same_field_types` through
`CTypes::compatible`, so the pre-filter only ever makes the check *stricter*,
and it changes the answer in exactly one measured case: a member of an
enumerated type against a member of that enum's underlying integer type, where
clang accepts and gcc rejects (`slate-parser-ntb`).

Three smaller residues worth knowing about:

- `type_of.rs::with_length` reads an inferred length back out of an
  `ir::Type::Array` to complete an incomplete C array extent. It flows layout
  into a C type, but decides no identity.
- `initializer.rs` decides brace elision — whether an expression initializes an
  aggregate member whole or is elided into its fields — by comparing unaliased
  `ir::Type`. C23 6.7.11 makes that a compatibility question. The two agree for
  every ordinary case, since `Defined(id)` is nominal; they diverge only for a
  compatible tag defined twice in one translation unit (`slate-parser-pd7`).
  The initializer walk is shaped over `Shape`/`ir::Type`, which is why the fix
  did not fit this phase.
- `convert.rs` still has functions named `differ_only_in_sign` and
  `compatible_ignoring_qualifiers`. These are *not* the deleted layout
  helpers of the same name — they take `QualType` and answer on C types. The
  `ir::Type` versions are gone.

## Where personality enters

`TypeResolver` carries a `CompilerFlavor`, set once in `with_tags` from
`unit.flavor`. Since all four resolvers (module lowering,
`resolve_type_module`, `resolve_module`, `assertion.rs`) are built through
that one constructor, they cannot disagree about personality — which matters
because `static_assert(sizeof(_Atomic struct { char a[3]; }) == 3)` has to
pass under `--flavor=gcc`, and the assertion checker builds its own resolver.

Two rules read it, both on `TypeResolver`:

- `atomic_layout`, consulted by `qualified_storage`: clang rounds an atomic
  object up to the next lock-free width, gcc does not promote aggregates at
  all, and MSVC gives anything that is not already a lock-free width a
  leading four-byte lock word. It dispatches on the flavor and delegates to
  `TargetInfo::atomic_storage` or `TargetInfo::msvc_atomic_storage`. See the
  `_Atomic` entry in [`ir-spec.md`](ir-spec.md) for the measured numbers.
- `effective_alignment`, consulted by `resolve_object_requests`: clang honors
  an alignment attribute below the type's natural alignment, gcc and MSVC
  raise it to the natural one.

Keeping both here is the point of the phase: `src/sema/module.rs` no longer
mentions `CompilerFlavor` at all. The flavor checks that remain in
`src/sema/validate.rs` are about character literals and specific diagnostics,
not layout, so they stay where they are.

`AbiClassifier` reads it too, since it holds a `&TypeResolver`. Argument
classification runs on `ir::Type`, which has lost `_Atomic` on aggregates, so
`AbiOperand` carries the qualifier alongside the type: clang makes an atomic
record argument MEMORY, gcc classifies it as the unqualified record, and the
Windows conventions reject it rather than guess.

## Adding a rule

Put it in `ctype/`, as a function over `QualType` that transcribes its
standard section, and call it from lowering. Do not reach for `ir::Type`: if
the rule needs a size or an alignment it wants `layout()`/`storage()`, and if
it needs to know what a type *is* it wants `CTypes`. If the answer differs
between compilers, it belongs next to `atomic_layout` and
`effective_alignment` on `TypeResolver`, decided by `CompilerFlavor` — and
per the project rule, reject only where clang, gcc and MSVC all reject;
otherwise accept with a named warning from `src/diagnostics.rs`.
