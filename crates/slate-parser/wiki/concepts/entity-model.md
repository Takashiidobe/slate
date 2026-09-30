# Declared-entity model

<!-- toc -->
- [Entity](#entity)
- [Merging](#merging)
- [Prototype scope](#prototype-scope)
- [Tags](#tags)
- [Audit](#audit)
- [Declaration sites](#declaration-sites)
<!-- /toc -->

One record per declared object, keyed by `BindingId`, merged across
redeclarations: `src/sema/entity.rs`, held as `TypeResolver.entities`.
The resolver, typer, and checker that fill it: [sema-passes](sema-passes.md).

## Entity

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

- `ty`: declared C type with qualifiers ([c-type-layer](c-type-layer.md)).
- `request`: what the declaration asked for (`aligned` / `_Alignas` /
  `__declspec(align)`, `common` / `nocommon`).
- Merging is monotonic: type via `CTypes::composite`, request via max.
  Declaration order never changes the result.

IR fields are projections of the entity:

| IR field | Source |
| --- | --- |
| `Variable.ty`, `Parameter.ty` | `layout(entity.ty)` |
| `is_const`, `restrict`, `access.volatile`, `access.atomic` | `quals(entity.ty)` / `access_of(entity.ty)` |
| `Variable.alignment` (`[align=N]`) | `effective_alignment(request.alignment, natural)` |
| `Global.common` | `request.common`, then `-fcommon` |
| register-address constraint | `is_register` |
| `__alignof__` of an object | `declared_alignment(request.alignment, natural)` |
| effects access map | `entities.types()`, once, after lowering |
| linkage, symbol attributes, storage duration, definition state | merged entity state |

The two alignment rules differ on purpose:
[object properties](ir/declarations.md#object-properties). Initializers and
bodies stay on IR declarations; they are payloads, not entity properties.

## Merging

- `TypeResolver::merge_redeclaration` is the only merge point, called only
  by the checker's `declare_object`: writes the composite back, or keeps
  the old type and returns a conflict (warned as `conflicting-types`).
- The checker records the merged type after each declaration in
  `declared_types` (by declarator or definition `NodeId`). Lowering's
  `redeclared` reads it into its entity table; it never merges. The type is
  point-in-time: `int a[]; ... int a[10];` keeps `int[]` for uses between
  the two.
- `names.rs::redeclares` treats `Object` and `Function` as one kind (a
  declarator only looks like a function when written with parameters, e.g.
  `extern __typeof(f) f`). Type agreement is `merge_redeclaration`'s job;
  name resolution runs before types exist.

## Prototype scope

- C11 6.2.1p4: parameter-list names end at the `)`. Only
  `names.rs::declarator` models scopes, around the `Declarator::Function`
  parameter loop. `TypeResolver` and the lowerer see only `BindingId`s, so
  resolving a prototype twice cannot leak names.
- Name resolution visits prototype parameter declarators, so `int a[n]`
  has references like any expression.
- Fixture: `sema/ir_prototype_scope_redeclaration.c`.

## Tags

- A tag's `BindingId` comes from name resolution; its type identity is a
  `TypeId`; properties live in `TypeResolver.definitions`.
- `tag_bindings: BindingId → TypeId`; `tag_ids: TagId → TypeId` memoizes
  bodies. `NameResolution.tags` exports definitions (`TagId → BindingId`),
  a reference per `TagSpecifier::Reference`, and a declaration per
  standalone `struct S;`.
- Scoping is `names.rs` only: two block-scope `struct Local` are two
  bindings and two `TypeId`s. `struct S;` binds a new tag hiding outer
  ones (6.7.2.3p8); `declare_forward_tag` gives it an incomplete `TypeId`
  (called by lowering and the checker).
- An unresolved tag reference declares an incomplete tag in the current
  scope (`typedef struct _IO_FILE FILE;`). `names.rs::define_tag` completes
  the innermost existing entry, so forward declaration and definition are
  one binding.
- Tag names are not keyed by kind; `same_tag_kind` rejects `struct S`
  after `union S`.
- `ms_asm_field` scans every tag binding before the `__asm` (watermark),
  newest first, not just visible ones: permissive.
- Fixtures: `sema/ir_tag_scopes.c`, `typedef_tag_binding_scopes.c`,
  `error/.../tag_kind_mismatch.c`.

## Audit

Re-run after touching sema; account for new hits.

```
grep -rn 'Variable {\|Parameter {\|Global {\|PlaceKind::CompoundLiteral {\|fields.push' src/sema
grep -rn 'requested_alignment\|\.request(\|object_alignment' src/sema
```

IR object construction sites:

| Site | Status |
| --- | --- |
| `module.rs::parameters` | from `TypeResolver::parameter_shape`, which also returns the adjusted `QualType` it declares |
| `module.rs::declaration` (`Variable`, two `Global` wrappers) | declared `QualType`; `[align=N]` and `common` from the request |
| `expression.rs::place` (`PlaceKind::CompoundLiteral`) | declared into the table; `access` from its `QualType` |
| `expression.rs` string-literal `Global` | synthetic `.strN` |
| `atomic.rs` trampoline `Parameter`s | synthetic |
| `module.rs` unnamed prototype `Parameter`s | function *type* prototypes have no parameter declarations |
| `types.rs::resolve_parameters` | `resolve_type_module` path: no entity table, counter-allocated ids; shares `parameter_shape` |
| `types.rs` record `Field`s | no `BindingId`; qualifiers and requests live in the record definition |
| `effects.rs` `PlaceKind::CompoundLiteral` | rebuilds an existing place |
| `module.rs`/`types.rs` `ArrayParameter` | grep false positive |

Alignment requests go through `entities.request` / `merge_request`, except
`field_request` (field-level) and `requested_alignment(self,
&tag.attributes)` (tag-level).

## Declaration sites

`sema/ir_entity_sites.c` dumps every property at every site. Empty cells:

- `_Alignas` on a parameter: constraint violation (C23 6.7.5p2), rejected.
- `const` on a record field: enforced and printed as a field type prefix
  with volatile and atomic access.
- Compound literals print as places, so no binding-level qualifier or
  alignment; qualifiers show in the address type and in `volatile`/`atomic`
  on reads.
