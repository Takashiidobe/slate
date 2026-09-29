# Declared-entity model

<!-- toc -->
- [Entity](#entity)
- [Merging](#merging)
- [Expression typer](#expression-typer)
- [Shared resolver](#shared-resolver)
- [Checker-owned rules](#checker-owned-rules)
- [Prototype scope](#prototype-scope)
- [Tags](#tags)
- [Audit](#audit)
- [Declaration sites](#declaration-sites)
<!-- /toc -->

One record per declared object, keyed by `BindingId`, merged across
redeclarations: `src/sema/entity.rs`, held as `TypeResolver.entities`.

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

- `TypeResolver::merge_redeclaration` is the only merge point: writes the
  composite back, or keeps the old type and returns a conflict.
- It must run above the "already declared?" early return in
  `declare_global` / `declare_function`, or `int x; long x;` is accepted.
- `names.rs::redeclares` treats `Object` and `Function` as one kind (a
  declarator only looks like a function when written with parameters, e.g.
  `extern __typeof(f) f`). Type agreement is `merge_redeclaration`'s job;
  name resolution runs before types exist.

## Expression typer

`TypeResolver::typed` (`sema/typer.rs`) types an expression without
lowering it.

- Used for unevaluated operands (`sizeof`, `_Generic` control, `typeof`,
  `__auto_type` initializers, classify and derived-signature builtins), the
  static-assertion checker, and `typeof` resolution
  (`TypeResolver::expression_type`). Nothing is lowered speculatively.
- Memoizes `Typed { c, lvalue, bits }` per `NodeId` in `expression_types`.
  Only successes are memoized (an enum body is typed before its
  enumerators exist). Missing rules return `Unimplemented`, reported as an
  error by unevaluated callers.
- `Lowerer::expr` / `Lowerer::place` compare their type with the typer's
  and return `Internal` on mismatch. Shared rules are factored, not copied:
  `binary_types`, `arithmetic_type`, `literal_type`, `derived_signature`,
  `builtin_callee`, `chosen_expr`, `real_floating_component`,
  `statement_expression_parts`, `AtomicBuiltin::result`, `swizzle`,
  `shuffle`, `predefined_name`.
- A typed `sizeof` operand is not lowered unless it is a VLA.
- Null pointer constants: `TypeResolver::integer_constant_zero`, an ICE by
  6.6p6 operand rules that folds to zero (`(void *)(1 - 1)` and
  `(void *)(unsigned long)0` yes; `(void *)(0, 0)`, `(int)(0.0 + 0.0)`,
  `(size_t)(void *)0` no). Used by `is_null_pointer_constant` and pointer
  `?:`; a cast of a zero ICE lowers to `null<ptr<T>>`.
- `__func__` reads `TypeResolver::function_names`. Locals declared inside a
  statement expression are typed into `TypeResolver::locals`
  (`declarator_type` + `completed_array`), consulted after `entities`.
- Variably modified operands: the typer sets `provisional_extents`, so
  unbound sizes become `vla<T, *>`; types with unbound extents
  (`CTypes::has_unbound_extent`) are never memoized, so lowering's
  cross-check gets the bound `vla<T, %id>`.
  - `sizeof` lowers a VLA operand and keeps it if it has effects
    (capturing an extent counts).
  - `typeof` of a VM expression is evaluated where its extents are captured
    (`Lowerer::typeof_evaluations`, from `capture_extents` /
    `type_name_extents`), only if it has effects.
  - `__auto_type` reserves `BindingId`s for non-constant sizes first
    (`reserve_extents`); lowering's `extents` binds into them.

## Shared resolver

- `Sema` builds one `TypeResolver` (`with_names`, from the one
  `NameResolution`). The checker walks the unit over it in `analyze`;
  `lower` takes it over and resets only `entities`. Type and expression
  dumps build their own.
- `references` (identifier id → `BindingId`), `declarations` (declaring
  node → `BindingId`). Identifier type: `entities.ty(binding)`; constant
  value (enumerator, folded `constexpr`): `constants[binding]`. Typedef
  names and tag references carry `Span<String>` whose `NodeId` is the key:
  `aliases[binding]`, `tag_bindings[binding]`.
- Definitions are memoized by AST node: `define_tag` by `TagId` (failures
  too, in `tag_failures`), `define_alias` by declarator `NodeId`. A
  definition's span is its `owner` (declarator, parameter, or function
  being resolved); ownerless tags use the body span. The module prints each
  definition's final state.
- Checker resolver warnings are stored per item (`item_diagnostics`) and
  replayed when lowering reaches it; order is unchanged.
- `TypeId`s are numbered in source order (the checker resolves type names
  inside expressions too). VLA typedefs are numbered when lowering reaches
  them: the checker only gives them a provisional alias
  (`declare_provisional_alias`).
- The checker declares everything lowering would before typing its uses:
  objects, parameters, and casts under `provisional_extents`; parameter
  array qualifiers (`int a[restrict 4]`); linkage for functions and
  file-scope/`extern` objects; `extern void` objects; implicit function
  declarations (`NameResolution.implicit_functions`,
  `CTypes::implicit_function`). It walks every expression lowering
  evaluates (`case` labels, asm operands, array sizes, `typeof` operands).
- Anything scanning all tags must bound itself by position: `__asm` member
  lookup only sees tag bindings below the names.rs watermark
  (`NameResolution.ms_asm_members`).

## Checker-owned rules

The checker rejects; lowering's copies are `Internal`. Shared rule
functions are called from both, with lowering wrapping them in
`ResolveError::checked` (`Rejected` → `Internal`).

- **Statements** (`StatementContext`, reset per function): `break` /
  `continue` / `case` / `default` / `[[fallthrough]]` outside their
  construct; non-integer switch; unfoldable case label; non-pointer
  computed goto; non-scalar condition; value `return` in a void function
  (a void operand is allowed and lowered as a statement then `return`);
  valueless `return` per flavor; fallthrough on a non-empty statement
  (non-gcc); nested function definitions (non-gcc). `constant_value` folds
  `?:` without requiring the unselected arm to be constant (`case (1 ? 1 :
  i)`).
- **Declarations**: shared `module::linkage`, `symbol_attributes`,
  `function_symbol`, `deduced_initializer` / `inferred_base` /
  `check_inferred` (`auto`, `__auto_type`), `attribute_error` (warning-free
  half of `check_attributes`). Restated in `Checker::object_rules`: void
  object; function initializer / thread-local / block-scope `static`;
  thread-local automatic; block-scope `extern` initializer; constexpr
  without initializer; VLA initializer; static VLA; weakref / selectany
  linkage; multiple initializers (`Checker::initialized`); conflicting
  `always_inline`/`noinline` (`Checker::inlining`). Arrays of `void` never
  resolve, so `validate.rs` checks them syntactically. Conversions to or
  from an incomplete enum are rejected by `classify_conversion`.
- **Initializers**: `check_initializer` / `check_braced`
  (`initializer.rs`) mirror lowering's `braced` / `fill` / `init_into` /
  `item_entry` / `designate` / `resume` step for step (brace elision
  ignores an already-claimed designator; a union is full once any member is
  written). Records each element's conversion and target in
  `element_targets`; `convert_element` fails `Internal` if its target is
  not compatible (compatibility, because of provisional extents).
  `inferred_array_length` runs the same walk unchecked.
- **Expressions**: the checker calls `typed` on every expression, children
  first, and reports each rejection (`ResolveError::is_rejection`) once at
  the innermost expression (`rejected_at`). The typer owns lvalue / const
  targets, `&` of bit-field / register / vector element / rvalue, members,
  `->`, callees and argument counts, subscripts and pointer arithmetic
  (`pointer_offset`, `require_pointer_element`), conditions, `?:` operands,
  `sizeof`/`_Alignof` of incomplete types and bit-fields, bit casts,
  `__builtin_convertvector`, `va_arg` and `va_*`, custom and atomic builtin
  operands. Shared with emitters: `numeric::binary_rule` / `unary_rule`
  (called by `emit_binary` / `emit_unary_arith`), `atomic::fetch_rule`,
  `AtomicBuiltin::operands`, `asm_operand_rule`, `typeof_expression`.
  `sizeof` of a record with a failed definition (`failed_definition`) is
  left to the definition's own error.
- **Type resolution**: `Checker::resolution` reports the first rejection
  per declaration. `declare_object` merges through `merge_redeclaration`
  after `inherit_convention` (and `apply_convention` for definitions), so
  conflicting types and conventions are reported at the declarator.
- **FP pragmas**: the checker runs its own `FloatingPragmas` /
  `FloatingRegion` state, restored at each compound; a pragma right after
  `({ ... })` is rejected, as clang does.
- Lowering's item and `finish_module` errors go through
  `ResolveError::checked`, so lowering returns no `Rejected`.

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
