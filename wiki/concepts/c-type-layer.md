# C type layer

<!-- toc -->
- [Representation](#representation)
- [Rendering](#rendering)
- [Layout](#layout)
- [Typed lowering](#typed-lowering)
- [Compatibility and composite
  types](#compatibility-and-composite-types)
- [Conversions](#conversions)
- [Redeclaration merging](#redeclaration-merging)
- [Invariant: no IR type decides C
  identity](#invariant-no-ir-type-decides-c-identity)
- [Flavor-dependent rules](#flavor-dependent-rules)
- [Adding a rule](#adding-a-rule)
<!-- /toc -->

Sema reasons over C types in `src/sema/ctype/`; `ir::Type` is produced only
by erasing one through `layout`.

## Representation

- `CTypes` interns `CTypeKind` into `CTypeId`s. `QualType` = id +
  `Qualifiers` (const, volatile, restrict, `_Atomic`).
- Sugar: `Typedef{name, underlying}`, `TypeOf{spelling, underlying}`,
  `AtomicSpecifier(inner)`; only for spelling and `typedef_chain`.
- Each entry stores its canonical `QualType` at intern time;
  `canonical(q) = entry.canonical ∪ q.quals`.
  - `_Atomic(T)` canonicalizes to `T` + atomic qualifier.
  - Array qualifiers move to the element (C23 6.7.3p10); the canonical
    element is unqualified and the array carries its quals.
  - Canonical function types have adjusted, top-level-unqualified
    parameters; `Function` keeps the written ones for spelling.
- `unqualified` keeps sugar unless hidden qualifiers exist, else desugars
  (arrays rebuilt with unqualified elements). Used by `typeof_unqual` and
  lvalue conversion.
- Integer kinds carry a rank (`Short`..`Int128`), not a width; `char`,
  `signed char`, `unsigned char`, `long`, `long long` stay distinct at
  equal widths. `__fp16` ≠ `_Float16`.

## Rendering

`render.rs` is a clang TypePrinter-style printer with spelling and
canonical modes: `int *`, `char *const`, `int (*)(int)`, `int[3]`,
`int(int)`, `(void)`/`()`/`...`, qualifier order `const volatile restrict
_Atomic`. Tags print by definition name.

## Layout

- `layout.rs` erases a C type to `ir::Type`. Pointer `is_const`/`access`
  come from the pointee's canonical qualifiers; parameters use the adjusted
  pointer; enums and records map to `Defined(id)`; `long double` follows
  the target.
- `TypeResolver::layout` returns `None` for `void`; `object_type` makes that
  an error, so storage sites reject void. Sites where void is legal call
  `ir_type` directly: `define_alias` (`typedef void f;`), the `extern` arm
  of a global (`extern void _text;`), and the `sizeof` / `_Alignof` sites.
- `sizeof(void)` = `_Alignof(void)` = 1 and `require_pointer_element`
  accepts `void` (GNU extension; the `-Wpointer-arith` warning is not
  emitted).
- Never give `TargetInfo::storage_of(Type::Void)` a size: object, field,
  parameter, and `va_arg` paths must keep failing.

## Typed lowering

- `sema::operand::Operand` = IR value + `QualType`; `Lvalue` = IR place +
  declared `QualType`. Bit-field storage/width stay in `BitFieldAccess`.
  Binding and enumerator tables hold C types. The effects access map is
  derived from binding qualifiers after lowering.
- An operand's IR type is its C type's layout, except comparisons, `!`,
  `&&`, `||`: C type `int`, IR `bool`. Integer uses emit `from_bool`.
  `_Generic(a < b, int: ...)` selects `int`; `sizeof(a < b)` is
  `sizeof(int)`.
- Runtime and required-constant expressions share typed arithmetic in
  `operand.rs`; `ctype/arith.rs` picks C result types, `numeric.rs` emits.
  No layout→C-type reverse mapping. `typeof` reads the operand's C type.

| Rule | Implementation | C23 |
| --- | --- | --- |
| Lvalue conversion, array/function decay | `CTypes::lvalue_conversion` | 6.3.2.1p2–4 |
| Address of a qualified object | `Lowerer::expr`, pointer to `Lvalue.c` | 6.5.3.2 |
| Member qualification | `Lowerer::field_place`, field ∪ base qualifiers | 6.5.2.3p3–4 |
| Integer and bit-field promotions | `CTypes::integer_promotion` | 6.3.1.1p2 |
| Default argument promotions | `CTypes::default_promotion` | 6.5.2.2p6 |
| Usual arithmetic conversions | `CTypes::usual_real_type` | 6.3.1.8 |
| Pointer difference | `Lowerer::binary`, unqualified compatible pointees, `ptrdiff_t` | 6.5.6 |

- A standard integer outranks a bit-precise one of the same width. Enums
  promote through their compatible integer type. Floating components keep
  their C kind. Complex/imaginary domains are chosen separately from
  component rank.
- `size_t` / `ptrdiff_t`: `long` on LP64, `long long` on LLP64, `int` on
  ILP32. Character and string literals keep plain `char`, signed character
  kinds, and encoding-specific types.
- Variadic calls: `float` and `__fp16` promote to `double`; `_Float16`
  stays `half` (clang 22.1.8 IR).

## Compatibility and composite types

`ctype/compat.rs` transcribes 6.2.7: `compatible` (qualifiers match at each
level), `compatible_unqualified`, `composite`.

- Typedefs are transparent (canonical comparison). Enums are compatible
  with their underlying integer.
- Arrays: compatible elements and equal-or-unknown sizes; a VLA is
  compatible with any array of compatible element (6.7.6.2p6).
- Functions compare returns; unprototyped matches a non-variadic prototype
  whose parameters are unchanged by default promotions. Different calling
  conventions (x86-32 `stdcall`/`fastcall`/`vectorcall`/`thiscall`, x86-64
  `vectorcall`) are never compatible; `CTypes::with_convention` sets the
  convention on the first reachable function type.
- A prototyped parameter of `transparent_union` type is compatible with
  any type compatible with one of its members, as in clang's
  `mergeTransparentUnionType` and gcc (`CTypes::set_transparent_members`,
  filled when sema marks the union). The composite keeps the left
  (earlier) parameter, not clang's merged member, so a redeclared
  function's call signature stays equal to its definition's and
  function-pointer conversions between the two spellings stay explicit
  `pointer_cast`s (`transparent_union_function_compat.c`).
- Tags are nominal (one `TypeId` per definition) except C23 N3037: complete
  same-tag types with matching content are compatible within a TU.
  `TypeResolver::join_compatible_tag` runs on each named tag completion and
  puts matching earlier definitions in one class (`CTypes::tag_classes`;
  `same_tag_shape` for names, access, widths, enumerators;
  `same_field_types` via `compatible`). The new id joins before the member
  check, so self-referential members compare coinductively. Anonymous tags
  never join. Attributes are ignored (gcc; clang rejects an `aligned`
  difference).
- `classify_conversion` treats compatible distinct records as `RecordCopy`;
  IR `copy<T>` may read a layout-identical record of another type.
- `merge_pointer` (for `?:`): composite pointee, union of qualifiers, `void`
  wins.
- `__builtin_types_compatible_p` calls `CTypes::compatible` directly.

## Conversions

- `ctype/convert.rs::classify_conversion` is the only legality decision:
  (from, to, `ConversionContext`, is-null-pointer-constant) → `CastKind` +
  optional warning, or `ResolveError`. Lowering emits the kind; an unhandled
  case is a rejection. There is no `Init` context; initialization is
  `ConversionReason::Assign`.
- The static-assertion checker classifies conversions at simple assignment,
  call arguments (prototyped and variadic), `return`, explicit casts, and
  top-level non-array initializers. It records a `Conversion` per operand
  `NodeId` in `TypeResolver::conversions` (`record_conversion`); ill-formed
  ones are `SemaError`s from `analyze`. Lowering's `convert_recorded` emits
  the recorded kind and warning; a missing record is `Internal`.
- Also recorded: braced-initializer elements (initializer walk) and atomic
  value operands (`AtomicBuiltin::operands`). Lowering has no unrecorded
  `convert`; nothing classifies a conversion there.
- `conversions` is `HashMap<NodeId, Conversion>` with `Conversion { kind,
  warning }`: one per operand node, no target type. `convert_recorded`'s
  caller supplies the target from a fact: the callee signature's
  parameter, the cast's type name, the assigned place, the declared return
  type, the element target. Keeping the target out of the record keeps
  VM targets bound to lowering's extents.
- `CastKind::EnumToInt(EnumTail)` carries the second hop (underlying
  integer to target: identity, arithmetic, vector, int-to-enum or
  int-to-pointer), so `emit_cast` never classifies again.
- Operand conversions are recorded per edge in `operand_conversions`:
  `(owner NodeId, Slot) → Vec<Step { kind, to, reason }>`. The owner is the
  operator (or the `switch` / case label statement); a slot names the
  operand (`Left`, `Right`, `Operand`, `Then`, `Else`, `Result`,
  `Discriminant`, `CaseStart`, `CaseEnd`, `Argument(i)`, `Extent`,
  `Parameter`), so GNU `x ?: y` and compound assignment never collide. `StepKind::Arithmetic` emits through
  `arithmetic_conversion` (it wraps enums itself), `StepKind::Cast` through
  `emit_cast`. An empty list means no conversion; a missing key is
  `Internal`. `computation_types` holds the type an arithmetic operation
  (or compound assignment) is performed at. The typer records them in
  `binary_type`, `record_update`, `record_pointer_comparison`, the `?:`,
  unary and `Index` arms; the checker records `switch` / `case`
  (`record_switch`, `record_case`). Lowering applies them with `converted`
  / `converted_at`.
- Call arguments beyond the parameters (variadic and unprototyped) record
  their default promotion as `Argument(i)` on the call (`call_type`), as
  do the float builtins, `__builtin_fpclassify` arms, the atomic fetch
  operand and `__atomic_is_lock_free`'s size and pointer. VLA sizes record
  `Extent` on the size expression (`record_extent`); K&R parameters whose
  promoted type differs record `Parameter` on the parameter and the
  promoted type in `promoted_parameters`. `record_casts` records a chain
  of casts, classifying each hop from the previous target.
- Atomic lowering's own values (a `_Bool` object handled as `unsigned
  char`, pointer bitwise ops through `size_t`, min/max signedness) are
  emitter choices: it names the `CastKind` directly.
- Recorded targets may hold unbound extents (`vla<T, *>`); `operand_steps`
  and `computation` then drop the owner's memo and type it again under
  lowering's bound extents.
- Pointer comparisons convert both operands to one operand's type (the
  non-null pointer side), not clang's composite type; null pointer
  constants use the typer's rule, so `(const void *)0` is a pointer cast,
  not `null`, as in clang.
- `record_conversion` rejects a `Vector` cast between different storage
  sizes.
- The checker sees VM types with unbound extents (`vla<T, *>`), so a
  pointer-to-VM conversion between `same` types classifies as `Pointer`;
  `emit_cast` drops a `Pointer` cast whose IR types are equal.
- `differ_only_in_sign` and `compatible_ignoring_qualifiers` in `convert.rs`
  take `QualType`; signedness and nested qualifiers are compared on C types
  (plain `char` ≠ `signed char`).
- Address of a `register` variable is rejected by the typer from the
  binding's storage class.
- Type resolution rejections (`TypeResolver::resolve`, `declarator_type`,
  tag definitions) are reported by the checker; any reaching lowering is
  `Internal`.

## Redeclaration merging

- `TypeResolver::declared` holds each entity's merged C type by
  `BindingId`; `bindings` holds the type in scope.
- `merge_redeclaration` runs in the checker's `declare_object`, which
  records the result per declaration in `declared_types`; lowering's
  `declare_global` / `declare_function` only read it (`redeclared`).
- Compatible declarations merge into their `composite` (`int a[]; int
  a[5];`). Otherwise the
  [conflict table](ir/declarations.md#redeclaration-conflicts) applies;
  `types::same_layout` (lowered types, signedness ignored) decides
  warning vs error.

## Invariant: no IR type decides C identity

Compatibility, conversions, redeclaration merging, `_Generic`, and
`__builtin_types_compatible_p` answer through `CTypes`. Audit:

```
grep -rn 'ty ==\|ty !=\|== Type::\|!= Type::' src/sema/
```

Every hit must be in this table; account for new ones.

| Site | Question | Why allowed |
| --- | --- | --- |
| `numeric.rs`, `expression.rs::emit_cast` | `value.ty == to`, `ty == Type::Bool` | Representation check during emission; `CastKind::Identity` needs it (truth values are IR `Bool`, C `int`) |
| `fold.rs` | operand vs value type | Folding over IR values |
| `effects.rs`, `atomic.rs` | `== Type::Void`, `== Type::Bool` | IR normalization |
| `expression.rs` via `types.rs::is_va_list` | lowered type == target `va_list` | `VaList` is IR-only; `char *` targets use `ptr<i8>` |
| `types.rs::same_layout` | same shape ignoring signedness | Warning-vs-error for redeclarations (MSVC C4142/C2371) |

Tag redefinitions:

- Same-scope C23 redefinitions compare members on C types, stricter than
  compatibility (both compilers reject `int (*)[]` vs `int (*)[3]`). gcc
  and msvc flavors: `CTypes::same`. clang flavor:
  `CTypes::same_or_enum_underlying` (enum = underlying integer at any depth,
  qualifiers must match).
- Cross-scope (`join_compatible_tag`) uses `compatible`, like gcc; clang is
  stricter, so the clang flavor is permissive.
- Known permissive gap: `same_tag_shape`'s enum arm compares underlying
  types as `ir::Type`, so `enum E : long` vs `: long long` is accepted on
  LP64.

Other residues:

- `type_of.rs::with_length` reads an inferred length from
  `ir::Type::Array` to complete a C array extent (no identity decision).

## Flavor-dependent rules

`TypeResolver` holds the `Dialect`, cloned once in `with_names`, the only
constructor for all four resolvers (module lowering, `resolve_type_module`,
`Sema::lower`, `assertion.rs`), so they agree
([configuration-threading](configuration-threading.md)).

- `atomic_layout` (via `qualified_storage`): clang rounds up to the next
  lock-free width, gcc does not promote aggregates, MSVC adds a four-byte
  lock word to non-lock-free sizes. Delegates to
  `TargetInfo::atomic_storage` / `msvc_atomic_storage`. Numbers:
  [`_Atomic` layout](ir/atomics.md#_atomic-layout-per-flavor).
- `effective_alignment` (via `resolve_object_requests`): clang honors
  under-alignment attributes; gcc and MSVC raise to natural alignment.
- `AbiClassifier` holds `&TypeResolver`. `ir::Type` drops `_Atomic` on
  aggregates, so `AbiOperand` carries the qualifier: clang passes atomic
  records in MEMORY, gcc as the unqualified record, MSVC by `win64` size
  rules on the lock-word layout. `win_arm64` rejects it.
- Layout does not branch on flavor in `sema/module.rs`.

## Adding a rule

- Write it in `ctype/` as a function over `QualType` transcribing its
  standard section; call it from the checker/lowering.
- Sizes and alignments come from `layout()`/`storage()`; type identity from
  `CTypes`. Never `ir::Type`.
- Compiler-dependent rules go on `TypeResolver` next to `atomic_layout`,
  keyed by `CompilerFlavor`. Reject only where clang, gcc, and MSVC all
  reject; otherwise accept with a named warning (`src/diagnostics.rs`).
