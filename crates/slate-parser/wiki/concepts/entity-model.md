# Declared-entity model

One record per declared object, keyed by `BindingId`, merged across
redeclarations. The table lives in `src/sema/entity.rs` and hangs off
`TypeResolver` as `types.entities` (epic slate-parser-8lv).

## What an entity holds

```rust
struct Entity {
    ty: QualType,
    request: ObjectRequest,
    is_register: bool,
    linkage: Option<Linkage>,
    storage: Option<StorageDuration>,
    definition: bool,
    symbol: SymbolAttributes,
}
struct ObjectRequest { alignment: Option<u64>, common: Option<bool> }
```

`ty` is the declared C type, qualifiers included, from the
[C type layer](c-type-layer.md). `request` is what the declaration *asked
for* as opposed to what the type gives: an `aligned`/`_Alignas`/
`__declspec(align)` request and `common`/`nocommon`. Both merge monotonically
across redeclarations — the type through `CTypes::composite`, the request by
taking the maximum — so declaration order never changes the answer.

Everything an IR declaration says about an object is a projection of this
record:

| IR field | Derived from |
| --- | --- |
| `Variable.ty`, `Parameter.ty` | `layout(entity.ty)` |
| `is_const`, `restrict`, `access.volatile`, `access.atomic` | `quals(entity.ty)` / `access_of(entity.ty)` |
| `Variable.alignment` (`[align=N]`) | `effective_alignment(request.alignment, natural)` |
| `Global.common` | `request.common`, then `-fcommon` |
| register-address constraint | `is_register` |
| `__alignof__` of an object | `declared_alignment(request.alignment, natural)` |
| the access map handed to effects normalization | `entities.types()`, once, after lowering |
| `Global` / `Function` linkage and symbol attributes | merged declaration state on the entity |
| `Global` storage duration and definition state | merged declaration state on the entity |

Two alignment rules read the one request, because they genuinely differ: see
the `lh7.2.31` paragraph in [`ir-spec.md`](ir-spec.md).

## Merging

`TypeResolver::merge_redeclaration` is the single merge point. It composes the
previous and newly declared types and writes the composite back, or keeps the
previous type and returns a conflict message. The ordering invariant found the
hard way in slate-parser-9ve.4 still holds: registration must happen *above*
the "already declared?" early return in `declare_global`/`declare_function`, or
the first declaration never registers and `int x; long x;` is silently
accepted.

Unevaluated operands (`sizeof`, `_Generic`'s controlling expression,
`typeof`, `__auto_type` initializers, classify and derived-signature builtin
arguments) are never lowered to learn their type; nothing rolls back
`next_id` or entities. `TypeResolver::typed` (`sema/typer.rs`) types an
expression without lowering it and memoizes `Typed { c, lvalue, bits }` per
`NodeId` in `expression_types`. It answers when it has a rule for the
expression and every subexpression it needs;
anything else is `Unimplemented`, which the unevaluated callers report as a
real error. Only answers are memoized: the checker types an enum
body before its enumerators are declared, and a remembered failure would hide
the later answer. `Lowerer::expr` and `Lowerer::place` compare every answer
with the type they lowered and return `Internal` on disagreement, so the rules
cannot drift: typing rules shared with lowering (`binary_types`,
`arithmetic_type`, `literal_type`, `derived_signature`, `builtin_callee`,
`chosen_expr`, `real_floating_component`, `statement_expression_parts`,
`AtomicBuiltin::result`, `swizzle`, `shuffle`, `predefined_name`) are factored
out of the IR-building code rather than copied. A `sizeof` operand the typer answers is not lowered at all unless its
type is a VLA, which C evaluates (see the variably modified paragraph below).

The static-assertion checker and `typeof` resolution type expressions through
the same typer (`TypeResolver::expression_type`), so a constant expression
like `sizeof(f())` is accepted wherever lowering would type `f()`.

Whether an integer operand is a null pointer constant is one AST predicate,
`TypeResolver::integer_constant_zero`: an integer constant expression by the
C 6.6p6 operand rules (so `(void *)(1 - 1)` and `(void *)(unsigned long)0` are
null, `(void *)(0, 0)`, `(int)(0.0 + 0.0)` and `(size_t)(void *)0` are not)
whose value folds to zero. Lowering's `is_null_pointer_constant` and the
typer's pointer `?:` both ask it, and an explicit cast passes its operand
expression, so a cast of a zero ICE lowers to `null<ptr<T>>`.

`__func__` and friends need the enclosing function's names, which live on
`TypeResolver::function_names` (set by lowering and by the checker per
function body). A statement expression's result can name a local declared in
its own body, which lowering has not declared yet when `typeof`/`sizeof` ask;
the typer types the body's top-level declarations itself
(`declarator_type` + `completed_array`, shared with the checker) into
`TypeResolver::locals`, which `object()` consults after `entities`.

A variably modified operand (a cast to `int (*)[n]`, a VLA local in a
statement expression) needs an extent binding that only lowering creates when
it evaluates the size. While resolving its own type names and statement
locals the typer sets `TypeResolver::provisional_extents`, so `derive` gives
such a size `Extent::Variable(None)` (`vla<T, *>`) instead of failing, and
`typed` never memoizes a type with an unbound extent
(`CTypes::has_unbound_extent`): lowering's cross-check asks again after the
size is bound and gets the exact `vla<T, %id>`. The callers then evaluate the
operand for real: `sizeof` lowers a VLA-typed operand
and keeps it when it has effects (a `Capture` of a new extent counts); `typeof`
of a variably modified expression is evaluated where its extents would be
captured (`Lowerer::typeof_evaluations`, from `capture_extents` and
`type_name_extents`), again only when it has effects, so `typeof(g)` of a VM
pointer reads nothing; `__auto_type` reserves `BindingId`s for its
initializer's non-constant sizes first (`reserve_extents`), so the typer
answers the exact type and lowering's `extents` binds into the reserved ids.

`names.rs::redeclares` decides whether a second declaration shares the first
one's entity, and it treats `Object` and `Function` as one kind. A declarator
only *looks* like a function when it is written with parameters, so
`extern __typeof(f) f` and a typedef'd function type arrive as objects; whether
the two types actually agree is `merge_redeclaration`'s question, and
`int x; void x(void);` still gets a conflict there. Name resolution runs before
any type is resolved, so it cannot answer it and must not pre-empt it
(slate-parser-dyd.12).

## Prototype scope

C11 6.2.1p4: names declared in a function declarator's parameter list have
block scope ending at the `)`. Only `names.rs::declarator` models scopes: it
pushes one around the `Declarator::Function` parameter loop. `TypeResolver`
and the lowerer have no scopes of their own; every typedef, tag and ordinary
name reaches them already bound to a `BindingId`, so resolving a prototype's
parameters twice (once for the type, once in `module.rs` to build
`Parameters`) cannot leak or lose a name.

Before that, a file-scope `typedef double T` was destroyed by an unrelated
`int f(enum { T = 2 } v);` later in the file. Fixture:
`sema/ir_prototype_scope_redeclaration.c`, where `int a[sizeof(T)]` in the same
prototype is `array=4` (the enumerator) while `T after_prototype` is still
`f64` (the typedef).

## Identifier lookup

`Sema` builds one `TypeResolver` (`TypeResolver::with_names`, from the one
`NameResolution`); the static-assertion checker walks the unit over it in
`analyze`, and `lower` takes it over, so lowering starts from the checker's
tags, aliases, constants and memoized expression types (slate-parser-cc94.6).
Lowering resets only `entities`, which it redeclares in order. The type and
expression dumps still build their own. `with_names` gives a resolver `references` (identifier expression
id -> `BindingId`) and `declarations` (declaring node id -> `BindingId`). An
identifier's type is `entities.ty(binding)`; its constant value, for an
enumerator or a folded `constexpr` object, is `constants[binding]`. The
assertion checker fills both before lowering exists, so a `_Static_assert`
sees function definitions and names that shadow an enumerator exactly as
lowering does (slate-parser-evdx). A typedef name (`TypeSpecifier::Named`)
and a tag reference (`TagSpecifier::Reference`) carry their name as a
`Span<String>`, whose `NodeId` is the reference key, so typedef aliases are
`aliases[binding]` and tags `tag_bindings[binding]` (slate-parser-cc94.3).

Sharing works because every definition is memoized by its AST node, so the
second walk finds rather than recreates it: `define_tag` by `TagId` (a failure
too, in `tag_failures`, since a failed body leaves a half-built `TypeId`),
`define_alias` by declarator `NodeId` (returning the alias `TypeId`, which
lowering annotates). A definition's span is its `owner`, the declarator,
parameter or function definition being resolved when `push` created it,
whichever walk that was; the checker sets the same owners as lowering, and a
tag with no owner falls back to its body's span. The module prints each
definition's final state, so a tag completed after its first use prints
complete. The checker's resolver warnings are kept per item
(`item_diagnostics`) and replayed when lowering reaches that item, so the
warning order is unchanged. The checker walks in source order and resolves
the type names inside expressions (casts, `sizeof`, compound literals,
`_Generic`), so `TypeId`s are numbered in source order; a VLA typedef, which
the checker cannot resolve without extent bindings, is numbered when lowering
reaches it. Anything that scans all tags must bound itself by position:
`__asm` member lookup only sees tag bindings below the watermark names.rs
records for it (`NameResolution.ms_asm_members`), because the checker has
already defined later tags.

Name resolution binds prototype parameters in their prototype scope and
visits their declarators, so `int a[COUNT]` and `int a[n]` in a prototype
have references like any other expression.

## Tags

A tag's `BindingId` comes from name resolution like any other name; its
identity as a type is a `TypeId`, and its properties (kind, name, fields,
completeness, layout) live in `TypeResolver.definitions`:

- `tag_bindings: HashMap<BindingId, TypeId>` maps a tag binding to its type,
  and `tag_ids: HashMap<TagId, TypeId>` memoizes an AST tag body. Name
  resolution exports `NameResolution.tags` (`TagId` -> `BindingId`) for
  definitions, a reference for each `TagSpecifier::Reference`, and a
  declaration for a standalone `struct S;` that opens a new tag.
- Scoping is entirely `names.rs`: two block-scoped `struct Local` definitions
  are two bindings, so two `TypeId`s (slate-parser-rsm). A standalone
  `struct S;` binds a new tag in the current scope, hiding any outer one, per
  C11 6.7.2.3p8 (slate-parser-9wx); `declare_forward_tag` only gives that
  binding an incomplete `TypeId`. Both the lowerer and the static-assertion
  checker call it (slate-parser-rdj).
- A tag reference that resolves to nothing is not an error: per C11 6.7.2.3p8 it
  *declares* an incomplete tag in the current scope, so `typedef struct _IO_FILE
  FILE;` and `struct Holder { struct Member *m; };` work with no prior
  declaration (slate-parser-dyd.5). `names.rs::define_tag` completes the entry
  already present in the innermost scope, so a forward or implicit declaration
  and its later definition are one binding and one `TypeId`.
- Names are not keyed by kind, so `struct S` after `union S` in the same scope
  is the same binding; `same_tag_kind` rejects it, as clang and gcc do.
- The MS inline-asm member lookup (`ms_asm_field`) scans every tag binding
  declared before the `__asm` (by the names.rs watermark), newest first,
  rather than only the visible ones: permissive.

Fixtures: `sema/ir_tag_scopes.c`, `typedef_tag_binding_scopes.c`,
`error/.../tag_kind_mismatch.c`.

## Audit

Re-run these after touching sema and account for any new hit.

```
grep -rn 'Variable {\|Parameter {\|Global {\|PlaceKind::CompoundLiteral {\|fields.push' src/sema
grep -rn 'requested_alignment\|\.request(\|object_alignment' src/sema
```

The first finds every place sema builds an IR object declaration. Every hit is
listed here; each one either derives its properties from an entity or is a
synthetic object with no C declaration behind it:

| Site | Status |
| --- | --- |
| `module.rs::parameters` | parameter; type, qualifiers and array shape from `TypeResolver::parameter_shape`, which also returns the adjusted `QualType` it declares into the table |
| `module.rs::declaration` (`Variable`, and the two `Global` wrappers) | file scope, block scope and static local; type and qualifiers from the declared `QualType`, `[align=N]` and `common` from the request |
| `expression.rs::place` (`PlaceKind::CompoundLiteral`) | compound literal; the object is declared into the table and its `access` comes from the same `QualType` |
| `expression.rs` string-literal backing `Global` | synthetic: a `.strN` object no C declaration names, all properties constant by construction |
| `atomic.rs` builtin trampoline `Parameter`s | synthetic, same reason |
| `module.rs` unnamed prototype `Parameter`s | a prototype written as a function *type* has no parameter declarations, and canonicalization has already stripped top-level parameter qualifiers |
| `types.rs::resolve_parameters` | the standalone `resolve_type_module` path, which has no `Lowerer` and so no entity table, and hands out `BindingId`s from a counter. It shares the `QualType` -> `ir::Parameter` projection with `module.rs::parameters` via `parameter_shape`; what stays split is id allocation and the entity declaration |
| `types.rs` record `Field`s (`fields.push`) | fields have no `BindingId`; their qualifiers and alignment requests live in the record definition, next to the offsets they determine |
| `effects.rs` `PlaceKind::CompoundLiteral` | not a declaration site: effect normalization rebuilds an existing place |
| `module.rs`/`types.rs` `ArrayParameter` | not a declaration site: the grep's `Parameter {` matches the array-extent shape too |

The second grep finds every reader and writer of an alignment request. All of
them go through `entities.request`/`merge_request`, except `field_request` (a
field's own request, held in the record definition) and the
`requested_alignment(self, &tag.attributes)` calls, which are a *tag's*
alignment, not an object's.

Linkage, storage duration, definition state and symbol attributes merge into
the entity alongside its type and object request. `ir::Global` and
`ir::Function` retain those values as the lowered module representation, but
are projections of the entity; redeclaration merging does not use them as a
second source of semantic state. Initializers and function bodies remain on
their IR declarations because they are emitted payloads, not entity
properties.

## Declaration sites

`sema/ir_entity_sites.c` is the acceptance matrix: every property at every
site, in one dump. Three cells are empty and cannot be filled:

- `_Alignas` on a parameter is a constraint violation (C23 6.7.5p2); sema
  rejects it as "parameter attributes".
- `const` on a record field is enforced (assigning to a struct with a
  `const`-qualified member is rejected, as clang does) and is printed as a
  field type prefix alongside volatile and atomic access (slate-parser-eiq).
- A compound literal prints no binding-level qualifier or alignment
  annotation, because it prints as a place rather than a declaration. Its
  qualifiers are still observable, in the type of its address and in the
  `volatile`/`atomic` flag on reads through it.
