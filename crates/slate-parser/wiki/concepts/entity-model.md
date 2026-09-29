# Declared-entity model

<!-- toc -->
- [What an entity holds](#what-an-entity-holds)
- [Merging](#merging)
- [Prototype scope](#prototype-scope)
- [Identifier lookup](#identifier-lookup)
- [Tags](#tags)
- [Audit](#audit)
- [Declaration sites](#declaration-sites)
<!-- /toc -->

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
[object properties](ir/declarations.md#object-properties).

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
`_Generic`), so `TypeId`s are numbered in source order; a VLA typedef is
numbered when lowering reaches it, because the checker gives it only a
provisional alias (`declare_provisional_alias`: `aliases[binding]` over the
`vla<T, *>` type, no `TypeId`, no memo).

The checker declares what lowering would before typing uses of it, since its
answers are memoized for lowering: objects, parameters and casts resolve with
`provisional_extents` so VLA locals and parameters are declared; parameters
keep their array declarator's qualifiers (`int a[restrict 4]`); functions and
file-scope or `extern` objects get their linkage (so `builtin_callee` sees a
redeclared library builtin the same way); `extern void` objects are declared;
and each item's implicit function declarations are declared from
`NameResolution.implicit_functions` (`CTypes::implicit_function`). It walks
every expression lowering evaluates: `case` labels, asm operands, declarator
array sizes and `typeof` operands included.

The checker also owns statement-context rules (slate-parser-cc94.5.2): its
`StatementContext` counts enclosing loops, breakables and switches and knows
the function's `Returns` (unknown, void, value), reset per function so a GNU
nested function starts fresh. It rejects `break`/`continue`/`case`/`default`/
`[[fallthrough]]` outside their construct, a non-integer switch discriminant,
a case label `constant_integer` cannot fold, a non-pointer computed goto, a
non-scalar `if`/`while`/`do`/`for` condition, a non-void `return` in a void
function (a void operand is allowed, as clang and gcc do; lowering emits it
as a statement then `return`), a valueless `return` per flavor, a fallthrough
attribute on a non-empty statement (non-gcc) and a nested function definition
(non-gcc). Lowering's copies of these are `Internal`. Case labels fold like
gcc and clang: `constant_value` evaluates a conditional whose condition folds
without requiring the unselected arm to be constant, so `case (1 ? 1 : i)`
and `int [0 ? f() : 1]` are constant.

Declaration rules are the checker's too (slate-parser-cc94.5.3). Rules with a
shared implementation are called from both walks: `module::linkage`,
`symbol_attributes`, `function_symbol`, `TypeResolver::deduced_initializer`
/ `inferred_base` / `check_inferred` (the `auto` / `__auto_type` rules), and
attribute placement (`TypeResolver::attribute_error` is the warning-free half
of `check_attributes`). The checker reports their `Rejected`; lowering maps
the same call through `ResolveError::checked`, which turns `Rejected` into
`Internal`. One-line rules are restated in `Checker::object_rules` with
lowering's copy `Internal`: void object, function initializer / thread-local
/ block-scope `static`, thread-local automatic, block-scope `extern`
initializer, constexpr without initializer, VLA initializer and static VLA,
weakref/selectany linkage, multiple initializers of one linked object
(`Checker::initialized`) and conflicting `always_inline`/`noinline`
(`Checker::inlining`). An array of `void` never resolves, so `validate.rs`
keeps a syntactic check for it. A conversion to or from an incomplete enum is
rejected by `classify_conversion`.

Initializers are checked by one current-object walk over types
(slate-parser-cc94.5.4): `TypeResolver::check_initializer` / `check_braced`
in `initializer.rs` mirror lowering's `braced` / `fill` / `init_into` /
`item_entry` / `designate` / `resume` step for step, including the item a
designator already claimed (brace elision ignores its designator) and a union
being full once any member is written. The walk reports designator, shape,
incomplete-record and string-element errors, and records each element's
conversion together with its target (`TypeResolver::element_targets`).
Lowering's `convert_element` consumes the recorded conversion and fails
`Internal` if its own target is not compatible with the checker's, so a
divergence between the two walks is loud rather than a silently wrong cast.
Types are compared by compatibility because the checker resolves variably
modified types with provisional extents. `inferred_array_length` runs the same
walk without checking, so an array's inferred length and lowering's always
agree.

Expression operand rules belong to the typer (slate-parser-cc94.5.5). The
checker calls `TypeResolver::typed` on every expression it walks, children
first, and reports a rejection (`ResolveError::is_rejection`: `Rejected`,
`InvalidOperands`, `InvalidOperand`) once, at the innermost expression that
raised it (`TypeResolver::rejected_at`, set by `typed`). The typer rejects
what lowering used to: non-lvalue or const assignment and increment targets,
`&` of a bit-field / register variable / vector element / rvalue, unknown and
non-record members, `->` on a non-pointer, non-function callees and
prototyped argument counts, subscripts and pointer arithmetic
(`pointer_offset`, `require_pointer_element`), non-scalar conditions,
conditional operand mismatches, `sizeof`/`_Alignof` of incomplete types and
bit-fields, bit casts, `__builtin_convertvector`, `va_arg` and the `va_*`
builtins, and the custom and atomic builtins' arity and operand types.
Binary and unary arithmetic share one rule with the emitters:
`numeric::binary_rule` / `unary_rule` check the converted IR operand types
and `Context::emit_binary` / `emit_unary_arith` call them through
`ResolveError::checked` first, so their own error arms are `Internal`. Atomic
fetches share `atomic::fetch_rule` the same way, and
`AtomicBuiltin::operands` names which arguments are objects, pointers, values
(whose `Arg` conversions the checker records) and fetch operands. Asm operand
lvalue, bit-field and register rules are `TypeResolver::asm_operand_rule`;
`typeof` of a bit-field is `typeof_expression`. A `sizeof` of a record whose
definition failed (`failed_definition`) is left unreported, since
lowering diagnoses the definition itself. Lowering's copies in
`expression.rs`, `atomic.rs`, `asm.rs` and `numeric.rs` are `Internal`.

Type resolution errors are the checker's too (slate-parser-cc94.5.6). It
used to resolve every declarator and discard the `Rejected`, leaving tag
redefinitions and kind mismatches, vector and `__ptr32` attribute errors,
`ms_struct` layout, unsupported `_FloatN` and the like for lowering to report.
`Checker::resolution` now reports the first rejection per declaration (at the
declaration, or the function or parameter), and `declare_object` merges a
redeclaration through the shared `merge_redeclaration`, after
`inherit_convention` (and, for a definition, `apply_convention` of its
attributes) exactly as lowering does, so conflicting function types and
calling conventions are reported at the declarator. The floating-point pragma
state machine runs in the checker too: its own `FloatingPragmas` and
`FloatingRegion`, file-scope and statement placement, the region restored at
each compound. Lowering leaked `compound_start` out of a statement
expression's value, so a pragma right after `({ ... })` was accepted; the
checker rejects it, as clang does. `typeof(type-name)` operands are walked
like any type name, so an assignment in a VLA size inside one has its
conversion recorded. Lowering's item and `finish_module` errors pass through
`ResolveError::checked`, so lowering returns no `Rejected`; a hook that logged
every rejection reaching lowering over the whole gate found none after this.

Anything that scans all tags must bound itself by position:
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
