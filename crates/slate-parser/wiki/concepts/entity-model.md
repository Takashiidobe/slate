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

`Entities::discard_after(next_id)` drops entities created while lowering a
speculatively evaluated expression (`sizeof`, `_Generic`, an unevaluated
`typeof`) whose bindings are rolled back.

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
block scope ending at the `)`. Three passes push that scope independently, and
all three had leaked it:

- `names.rs::declarator` around the `Declarator::Function` parameter loop.
- `types.rs::resolve` likewise, through `TypeResolver::with_scope`.
- `module.rs`, which resolves parameters a *second* time to build `Parameters`:
  inside `scoped` for a prototype, and for a definition inside one scope that
  also covers the body, since a definition's parameter scope extends into it.

That second resolution is why `types.rs::define_tag` re-declares its tag name
and enumerators on the memoized path. A tag body is resolved once, keyed by
AST `TagId`, but its *names* belong to whichever scope the specifier is written
in, and for a definition that is the function scope rather than the discarded
prototype scope the first resolution ran in.

Without this a file-scope `typedef double T` was destroyed by an unrelated
`int f(enum { T = 2 } v);` later in the file. Fixture:
`sema/ir_prototype_scope_redeclaration.c`, where `int a[sizeof(T)]` in the same
prototype is `array=4` (the enumerator) while `T after_prototype` is still
`f64` (the typedef).

## Tags

Tags are not in this table. A tag has no `BindingId`; its identity is a
`TypeId` and its properties (kind, name, fields, completeness, layout) live in
`TypeResolver.definitions`. `TypeResolver` is the tag entity table:

- `tag_ids: HashMap<TagId, TypeId>` maps an AST tag occurrence to its type.
- `tag_names: Vec<HashMap<(TagKind, String), TypeId>>` is a scope stack, pushed
  and popped with the lowerer's scopes, so two block-scoped `struct Local`
  definitions get distinct `TypeId`s (slate-parser-rsm).
- `declare_incomplete_tag` makes a standalone `struct S;` declare an incomplete
  tag *in the current scope*, hiding any outer one, per C11 6.7.2.3p8
  (slate-parser-9wx). A declarator-less declaration declares the tag even when
  it carries a fixed underlying type (`enum E : unsigned char;`).
- A tag reference that resolves to nothing is not an error: per C11 6.7.2.3p8 it
  *declares* an incomplete tag in the current scope, so `typedef struct _IO_FILE
  FILE;` and `struct Holder { struct Member *m; };` work with no prior
  declaration (slate-parser-dyd.5).
- Because of that, both `names.rs::define_tag` and `types.rs::define_tag`
  complete the entry already present in the *innermost* scope instead of minting
  a fresh one, so a forward or implicit declaration and its later definition are
  one binding and one `TypeId`.
- `TypeSpecifier::Tag(Reference)` in `types.rs` never falls back to searching
  `unit.tags` by name. That fallback bound a block-scope `struct T *p;` to an
  unrelated file-scope `struct T` defined later; a miss must push a fresh
  incomplete tag into the current scope and let `define_tag` complete it.

Fixture: `sema/ir_tag_scopes.c`.

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
