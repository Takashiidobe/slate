# Declarations, linkage, and attributes

<!-- toc -->
- [Globals, statics, and linkage](#globals-statics-and-linkage)
- [Redeclaration conflicts](#redeclaration-conflicts)
- [Symbol attributes](#symbol-attributes)
- [Object properties](#object-properties)
- [Declaration attribute
  classification](#declaration-attribute-classification)
- [Function semantics](#function-semantics)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Merged per-entity state is described
in the [declared-entity model](../entity-model.md); `SymbolAttributes`
printing in the [grammar](../ir-grammar.md#globals).

## Globals, statics, and linkage

- Name resolution gives every declaration its binding. Every declaration
  with linkage (file-scope objects and functions, block-scope `extern`
  objects and prototypes) shares one binding per name, even when the
  block-scope one comes first or is shadowed.
- Lowering merges redeclarations by `BindingId`: one `Global` per object
  (arrays complete, initializer from whichever has one, `definition` set by
  any non-`extern` declaration, initializer, or `alias`) and one `Function`
  per function (body and parameters from the definition, else the first
  prototype). Linkage stays internal once any declaration is `static`.
- The merged node's span (spelling, expansion, provenance) is its
  definition's: the function body, else the global's initializer, else its
  first non-`extern` declaration, else the first declaration. Its `NodeId`,
  and so its metadata, stays the first declaration's. Slate's directive
  translation uses the span to place each item in its `#if` branch.
  Fixture: `definition_spans.c`.
- Tentative definitions and `extern`s merge into one global; an incomplete
  tentative array completes to one element.
- A block-scope `static` is a module `Global` with `storage=static`,
  `linkage=internal`, and its own binding; no statement at the declaration.
  Same-named statics stay distinct.
- `_Thread_local`, `__thread`, `__declspec(thread)` give `storage=thread`
  at file scope and on block-scope `static`/`extern`; invalid on an
  automatic local.
- `constexpr` (`Variable.constexpr`): implicitly const, internal linkage at
  file scope, initializer required and kept as structured IR. One that
  folds is usable in integer constant expressions (`constexpr int w = 7;
  int a[w];`).
- Pre-C23 `f()` is unprototyped; C23 `f()` and `f(void)` are zero-parameter
  prototypes.
- Fixtures: `ir_globals_linkage.c`, `static_locals.c`.

## Redeclaration conflicts

One rule, checked against clang 22, gcc, and MSVC: where all three reject,
reject; where one only warns (always MSVC), accept with
`-Wconflicting-types`.

| Conflict | Result |
| --- | --- |
| return/object type of another kind, size, or pointer depth | error |
| incompatible C types of identical layout, also through pointers | warning (MSVC C4142) |
| prototyped parameter lists differing in types, count, or `...` | warning (MSVC C4028/C4030/C4031/C4052) |
| unprototyped vs prototyped, or only top-level parameter qualifiers | accepted |
| struct/union/enum redefined in the same scope | error |
| same, C23, with matching members or enumerators | accepted, reuses the first |

- `TypeResolver::merge_redeclaration` (`src/sema/types.rs`) decides on C
  types. Compatible declarations merge into the composite (completing
  `int a[]; int a[5];`). Otherwise `types::same_layout` (lowered types,
  ignoring signedness) separates the warning from the error, which is MSVC's
  own C4142/C2371 split: on LLP64 `int x; long x;` warns, on LP64 it errors.
- Tag redefinitions: `TypeResolver::define_tag`.
- Fixtures: `ir_redeclaration_conflicts.c`, `ir_redeclaration_compatible.c`,
  `x86_64-pc-windows-msvc/ir_redeclaration_layout.c`.

## Symbol attributes

`SymbolAttributes` sits on both `Global` and `Function`, printed after the
linkage and merged across redeclarations (first value wins, flags OR):
`asm_name` (from `asm("sym")`), `visibility`, `weak`, `alias`, `section`,
`used`, `retain`, `tls_model`, `dllimport`/`dllexport`, `weakref`,
`ifunc`, `selectany`.

- `weakref("t")` is not `weak` + `alias`: it defines no symbol, and its
  uses resolve to an `extern_weak` reference to `t`. It prints as `extern`
  with `[weakref="t"]`, needs internal linkage, and applies to functions
  and objects.
- A bare `weakref` takes its target from `alias("t")` (which then defines
  nothing). clang: the alias must be in the same declaration, and a
  definition is rejected. gcc: from any redeclaration (`bare_weakrefs`,
  resolved in `resolve_object_requests`); without a target, or on a
  function body or initialized object, it is ignored (with a target on an
  initialized object, rejected). With both `weakref("t")` and `alias("s")`
  the IR uses `t` (clang); gcc uses `s`.
- `ifunc("r")` names the resolver's assembler symbol; the declaration is
  printed as a bodiless `fn` with `[ifunc="r"]`. Resolver semantics are
  left to Slate. Not checked (both oracles reject): an undefined resolver,
  a body on the same function, `weak` (gcc).
- `selectany` (clang: `weak_odr` + COMDAT) needs external linkage.

## Object properties

Resolved after all redeclarations merge, on `Global` (and `let` for
alignment):

- `[align=N]` from `aligned`, `_Alignas`, `__declspec(align)`, taking the
  largest request, printed only when it differs from natural. clang honors
  it below natural (`aligned(1)` on `int` gives `align=1`, as its LLVM IR
  shows); gcc and MSVC take the max with natural.
- `__alignof__` of an object reports the declared request even below
  natural under clang and gcc (gcc lays out `aligned(1) int` at 4 but
  reports 1); MSVC raises it to natural. Hence
  `TypeResolver::declared_alignment` beside `effective_alignment`, both
  reading one request per `BindingId`. `_Static_assert` sees the same merged
  entities. `__alignof__` of a member is its laid-out alignment (`packed` 1,
  `packed, aligned(2)` 2). A typedef redeclared in one scope keeps its
  largest `aligned`.
- `[common]` marks an external tentative definition: no initializer
  anywhere, not thread-local, not `alias`/`section`/`weak`/`selectany`.
  `common` on any declaration beats `nocommon`, which beats
  `-f[no-]common` (default off, rejected under MSVC). On MSVC targets an
  alignment request opts out.
- Fixtures: `ir_object_attributes.c`, `ir_object_attributes_fcommon.c`,
  `ir_object_alignment_gcc.c`, `ir_object_alignment_sites.c`,
  `ir_entity_sites.c`, `x86_64-pc-windows-msvc/ir_selectany.c`.

## Declaration attribute classification

- Classification (`declaration_use`) and consumers: [attribute pipeline](../attributes.md).
- Function attributes are kept as `c_attributes` metadata (integer
  constant arguments folded). Keeping an attribute is not implementing it.
- `asm("sym")` is ignored on a typedef and an automatic local (as clang); a
  register label (`register int x asm("eax")`) keeps the register spelling
  on the binding.
- Fixture: `ir_declaration_attributes.c`.

## Function semantics

`FunctionSemantics` separates inlining preference, definition emission, and
control-flow facts:

- Preference: `hint`, `always` (`always_inline`), `never` (`noinline`);
  contradictions are diagnosed.
- `InlineSemantics::SupressDef` makes an external `extern inline` body
  inline-only; `ProvideDef` makes it supply the definition. The standard
  picks the default; `-f[no-]gnu89-inline` (last wins) and `gnu_inline`
  override, and the preprocessor's inline macros follow. Any file-scope
  declaration without `inline`, or with `extern`, requires an external
  definition. `static inline` keeps internal linkage and a definition.
  Rules follow [GCC's inline docs](https://gcc.gnu.org/onlinedocs/gcc/Inline.html).
- windows-msvc (any flavor): neither mode applies unless `gnu_inline`;
  clang gives every inline definition its own comdat (`weak_odr` with
  `dllexport` or an `extern` redeclaration, else `linkonce_odr`), so none
  is inline-only (`x86_64-pc-windows-msvc/ir_inline.c`).
- `noreturn` merges `_Noreturn`, `[[noreturn]]`, and the GNU attribute
  across declarations. GNU `const`/`pure` become `[memory=none]`/
  `[memory=read]` (`const` wins).
- `malloc(f[, n])` on a function returning a pointer becomes
  `[deallocator=%f, argument=n-1]`, one per distinct pair across
  declarations. `f` resolves like a callee (an undeclared builtin such as
  `__builtin_free` gets its builtin declaration). A non-function `f` is
  rejected; a bad `n` (out of range, non-pointer parameter) is rejected
  under clang and dropped under gcc. clang ignores the attribute but
  validates it; it is kept for Slate either way.
- `naked` is in [asm](asm.md#naked-functions).
- `target("...")` becomes `[target=...]` (grammar in
  [ir-grammar](../ir-grammar.md#functions)); `target_clones`, `cpu_dispatch`,
  and `cpu_specific` multiversioning are not lowered.
- Fixtures: `ir_inline*.c`, `ir_function_specifiers.c`, `ir_target_attribute.c`.
