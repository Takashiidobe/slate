# IR pipeline, dumps, and pruning

<!-- toc -->
- [Placement](#placement)
- [Failure kinds](#failure-kinds)
- [Node identity and metadata](#node-identity-and-metadata)
  - [C type metadata](#c-type-metadata)
  - [Macro invocations](#macro-invocations)
- [Walking the IR](#walking-the-ir)
- [Dumps and CLI](#dumps-and-cli)
- [Constant evaluation is required, not an
  optimization](#constant-evaluation-is-required-not-an-optimization)
- [Name-resolution scopes](#name-resolution-scopes)
- [Reachability pruning](#reachability-pruning)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). How sema produces the IR, how to print it,
what lowering may fold, and which declarations survive.

## Placement

```text
AST ──sema (check + lower)──▶ IR ──analysis pass(es)──▶ IR + facts ──▶ Rust
```

- Sema lowers the AST straight to typed IR: names, types, conversions, and
  required constants are resolved while IR nodes are built. There is no
  intermediate typed AST and no second tree-copying pass.
- Derived facts (mutability, address-taken, ranges, aliasing) belong to a
  separate pass after lowering. Lowering never computes them.
- Code layout:
  - `src/sema/`: semantic analysis and AST → IR lowering.
  - `src/sema/validate.rs`: early structural validation, exposed through
    `Sema::analyze`. Passing it does not prove a node valid.
  - `src/ir/`: node definitions, spans, and printers. It never interprets AST
    nodes or compiler flags.

## Failure kinds

`tools/corpus_sweep.py` triages failures by `ResolveError` variant: `Rejected`
prints the bare message, `Unimplemented` prints `not implemented: …`, `Internal`
prints `internal error: …`. Full policy:
[sema-passes](../sema-passes.md#resolveerror-policy).

## Node identity and metadata

- Every IR node has its own `NodeId` and a `Span` carrying spelling and
  expansion locations, header provenance, and macro origin from the AST node it
  lowers.
- A node synthesized from another node's span (conversion wrappers, hoisted
  temporaries and their reads, declarator-anchored initializers, type
  definitions created by a declarator) takes a fresh id via `Span::derive`. A
  rewrite of the same node (the effects pass) keeps its id with
  `Span::with_value`.
- Nothing looks an IR node up by AST id. Enumerator references resolve through
  `TypeResolver.constants`, keyed by `BindingId`.
- `Module.metadata` is keyed by IR node id and written with `Module::annotate`
  on the node that owns the fact, so each key prints on exactly one node. A
  declarator's C type annotates the declared entity, not type definitions it
  happens to create (`ir_metadata_single_owner.c`).
- Parentheses are transparent; the inner operation keeps its own span.
- Source comments are non-executing metadata held in `Module.comments`
  (`ir::Comments`); `Statement::Comment` keeps tagged sibling comments inside
  bodies. Every comment is an `ir::Comment` (raw text, `attach`, `doc`; see
  [comment-placement](../comment-placement.md)) with source spans.
  - `owned` maps the `NodeId` of a function, global, type definition, field,
    enumerator, or parameter to its `leading`, `trailing`, and `inner` comments.
    Sema resolves ownership from the AST siblings, so consumers never see the
    AST.
  - `detached` lists file prologue and section comments, each with a `position`
    into `order`.
  - `order` lists the top-level IR nodes in source order with their file, so a
    detached comment anchors by node id: the next emitted node of the same file,
    else the previous one.
- Effects normalization keeps comments in order. Default IR dumps hide them;
  `--show-comments` prints `comment <attach> [doc] "raw text"` with source
  locations.
- Rust lowering moves owned comments onto the Rust node (`FnDef`, `FnParam`,
  `Item::Static`, `RecordDef`, `RecordField`) and anchors detached comments, and
  owned comments whose node is not emitted, to the next emitted item by node id.
  Comments in unreachable dispatcher blocks are retained at the end of the
  generated function.
- Codegen formats its own output: `backend::codegen` drives the Oppen printer
  vendored from prettyplease (`backend/printer/`), with comments as ordinary
  tokens. No rustfmt or syn round trip; run rustfmt on the output for its
  style.

### C type metadata

Each resolved declaration carries `c`, `c_canon` when different, and
`typedef_chain`. Qualifiers are `c_const`, `c_volatile`, `c_restrict`,
`c_atomic`, and `c_unaligned`. All are rendered from the interned C type
(`src/sema/ctype/`, see [c-type-layer](../c-type-layer.md)), never assembled
from strings:

- `c` is the written spelling.
- `c_canon` desugars typedefs and `typeof`, keeps `_Atomic(T)`, prints function
  types with adjusted parameters, and prints a C23 `()` as `(void)`.
- Function types carry no qualifier metadata.

`Module::c_types` (`CTypeTable`, `src/ir/c_types.rs`) holds the same C types in
structured form, next to the resolved `ir::Type`, which stays the only type that
decides semantics:

- `types` and `definitions` are the sema interner and tag definitions, so a
  consumer can walk a typedef chain or desugar without parsing strings.
- `nodes` maps a `NodeId` to its C type: values, record fields, parameters,
  locals, globals, functions, and alias definitions. `bindings` maps a
  `BindingId` to its declared type, including hoisted temporaries. `place()`
  derives a place's type from these, since places have no node id; a swizzle has
  none.
- An entry always erases to its node's `ir::Type`, so comparisons (C `int`, IR
  `bool`) have none. Nodes that don't correspond to a C expression, such as the
  inner `widen` of `reinterpret(widen(x))`, have none either; use the width.
- Typedefs survive arithmetic: the usual arithmetic conversions return the
  operand with the higher rank, sugar included, so `uLong + int` is `uLong`. A
  no-op conversion replaces the entry with the target type.
- `--dump-ir-types` builds its module without lowering, so its table is empty.

Dumps print an entry as `[c="..."]` only when the written spelling differs from
the canonical one, because `const<u64>(1) [c="unsigned long"]` says nothing the
reader needs next to `u64`. `unsigned long` is still in the table. A `write`
prints its place's type after the statement. This needs no flag, and
`--show-metadata` adds the rest of the metadata above.

### Macro invocations

`Module.macros` (`MacroInvocations`) lists the macro expansions in user code,
including those that expand to nothing, with their definitions and argument
ranges. `node.macro_nesting` says which expansions produced a node and whether
it is body or argument code; see
[macro invocations](../preprocessor.md#macro-invocations). Lowering must not
depend on it for semantics: it is source context for recovering macros in the
output. `--show-macros` prints it.

`Sema::with_macro_structure(true)` also puts expansions in the function bodies,
which `--show-macros` turns on. `sema/macro_structure.rs` decides from the AST
and the [cover table](../preprocessor.md#macro-invocations) before lowering:

- `Statement::MacroExpansion { expansion, body }` wraps the statements an
  expansion covers. Nested expansions nest. It is not a scope: a `let` in `body`
  stays visible to later statements.
- Holes and value expansions are not nodes. `Module::macro_values` maps a
  value's or place's `NodeId` (`Place::new` gives each place its own) to its
  `MacroWrap`s, innermost first:
  `MacroWrap::Argument { expansion, argument }` marks one argument occurrence
  inside its open expansion, and `MacroWrap::Value(expansion)` marks an
  expansion that covers one expression below statement level, read as a value
  in function code (`int k = M(i);`). A value expansion is not used for an
  expression the program writes to or takes the address of, a constant context
  (`case`, array size, `sizeof`, `static` initializer) or asm operands. On one
  node, the argument wraps sit outside the value wraps. `--show-macros` prints
  them as `[macro_value="#E"]` and `[macro_arg="#E.k"]` metadata. A walk sees
  them only through `ir::walk_body_with_macros`, which calls
  `Visitor::macro_value` and opens the expansion or hole around the value.
  An lvalue occurrence is an argument wrap on the place's id, printed on the
  place (`write<i32>(%i [macro_arg="#E.0"], ...)`); the walker calls
  `enter_place_hole`/`leave_place_hole` around it.
  `Module::macro_provenance(id)` is the one query for a node's wraps, written
  cast, expansions with their definitions, and hidden caller-local bindings.
- A conversion the argument
  needs is applied by the parent after the hole closes, so `ADD_ONE(c)` on a
  `u8` has its `widen` outside. An explicit integer cast whose operand is a
  hole lowers as it would without macros (`widen`, `truncate`, `reinterpret`,
  or nothing for an identity cast). `Module::macro_written_casts` records it,
  keyed by the cast's result node and naming the hole node, so that a template
  writes one `$x as T` for every site (`--show-macros` prints
  `[macro_cast="T"]`). An identity cast names its own node, and the cast sits
  outside that node's wraps. When an argument is passed down to a nested macro
  the inner hole wraps the outer one. Slate's `--merge` also wraps caller
  locals a kept body uses, as argument wraps inserted innermost on the binding
  place's id: an `argument` past the definition's parameters indexes
  `Module::macro_hidden_arguments[expansion]`.
- A callee argument gets a hole unless it names a function: that call stays
  `Callee::Direct`, with the name in the body. Each argument of a `f args` list
  gets its own hole. A type or name argument gets no hole.
  `CTypeTable.macro_type_arguments` holds a type argument's resolved type, read
  from the `TranslationUnit.macro_type_arguments` the parser recorded. It skips
  spellings with `{` or `(`.
- An expansion gets none of these when an argument is not one expression, type,
  name, callee or call argument list (`TIMES2(i + j)`, a declaration or
  statements), when an argument reads a binding the body declares
  (`TEMP(temp + a)`), when the statements hold asm, labels, switch labels, or a
  `static` or `extern` declaration, or when a value expansion's only tokens are
  its own argument (`#define ID(x) x`).

Hoisting ignores macro structure: the value tree is the one built without it,
so effects hoist exactly as without structure. An expansion whose value hoisted
statements, or whose root value was replaced (`r += NEXT()` with `*cursor++`,
`GT_OFF(x)` with a call under `&&`), is split: `effects_statements::normalize`
records it in `Module::macro_rejections` as `hoist_split_region`, and
`Module::macro_hoisted_runs` maps the root (or hole) node id to a
`HoistedRun { statements, value }`: the ids of the statements hoisting emitted
for it, in order, and the id of the value left in their place. Slate's
`frontend::macro_fold` folds a run back into a `StatementExpression` at that
value when the run is contiguous, its declarations are used only there, the
value is reached unconditionally, and nothing evaluated between them conflicts
with the run's effects; the wraps move to the new node and the rejection is
dropped. A folded expansion that macro_keep still rejects is unfolded before the
strip, so inline output is unchanged. An unfolded split stays inline. Analyses match on value shape directly; one that must not reach into
macro code (`inline_temporaries`, the pointer analysis's pinned calls) reads
`Module::macro_values`.

IR transforms stop at expansion and hole boundaries; see [macro provenance in
IR transforms](../macro-provenance-transforms.md).

Slate's frontend (`crates/slate/src/frontend.rs`, `lower_parsed`) builds with
structure on in every mode; `slate-parser ir` builds it only with
`--show-macros`.

Invariant: sema makes no value decision because a node is a hole or an
expansion. Stripping every expansion must give the IR built without structure,
node metadata included. `Module::strip_macro_structure` moves a removed
wrapper's C spelling onto the node that replaces it.
`SLATE_CHECK_MACRO_STRUCTURE=1` checks this in `lower_parsed` and fails the
translation on the first differing line. `project_merge` sets it.
Sema always returns the full structure. Slate's `lower_parsed` then calls
`crates/slate/src/frontend/macro_keep.rs` `kept_expansions`, which picks the
expansions that are safe to lift from the normalized module (split expansions
included in its rejections), and `Module::strip_macro_structure`
(`ir/macro_strip.rs`, an `ir::Visitor` whose `keep` drops expansions) removes
the rest. Normalization does not depend on structure, so nothing is normalized
twice. A stripped expansion stays inline in the
[generated Rust](../slate-architecture.md#macro-templates).

Per-value expansion chains are not in the IR yet (only atomic builtins record
`c_macro`); see [open design](open-design.md#macro-provenance).

## Walking the IR

`ir::walk_body` and `ir::walk_value` take a mutable `ir::Visitor`.
`ir::walk_module_ref` takes a `ReadVisitor` over shared IR.
Both walkers use the same exhaustive matches over statements, places, and
values in `ir/visit.rs`. Mutable observe hooks (`statement`,
`value_kind`, `place_kind`, `binding`) run before the children; rewrite hooks
(`rewrite_place`, `rewrite_value`, `rewrite_statement`) run after them, bottom
up. The macro hooks (`keep`, `enter_expansion`, value and place holes) default
to keeping and ignoring macro structure.

Use `for_each_function_ref` for read-only scope discovery. It visits top-level
functions, GNU nested functions, and block bodies, including blocks in global
initializers.

## Dumps and CLI

| Command                                            | Output                                                                                     |
| -------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| `slate-parser ir src.c` / `parse src.c --dump-ir`  | the module; lowering must succeed for the whole module first                               |
| `+ --show-metadata`                                | adds the `NodeId`-keyed metadata, nested values included                                   |
| `+ --show-macros`                                  | adds `[macro=...]` to statements and values, and the invocation table at the module's end  |
| `+ --compact-ir`                                   | hides conversion reasons, operation policies, and call signatures (print only)             |
| `+ --show-spans`                                   | adds `[spelling=file:offset+len, expansion=file:offset+len]` to each global and function   |
| `parse src.c --dump-ir-types --show-metadata`      | aliases and function signatures, without lowering bodies                                   |
| `parse src.c --dump-ir-expressions [--show-spans]` | expression roots only: no module header, declarations, return conversions, or control flow |

- `Module::display(false)` prints required semantics (target, layout, linkage,
  storage duration, operation contracts); `display(true)` adds metadata.
  Metadata is ordered per node and string values are escaped.
- `Span<Statement>::display()` prints one statement (and its nested body)
  exactly as the module printer does, without metadata, for library consumers
  such as Slate's lowering barriers. `Value::display` and `Place`'s `Display`
  cover values and places.
- The module header's `storage` lines cover every directly nameable scalar
  format (`bool`, every standard integer width, all nine float formats). A line
  is omitted only when the target lacks the type.
- Fixtures pick a renderer with `SLATE-FILECHECK-ARGS`. The harness passes
  defines, include paths, flavor, standard, and show-id options first and then
  these arguments in order, so the latter win for repeatable options. They are
  split on whitespace, with no shell quoting.

## Constant evaluation is required, not an optimization

- Keep source expressions for Rust: `1 + 2` stays an `add`. Never fold ordinary
  arithmetic, casts, comparisons, or conditionals.
- Fold only where a concrete constant is required: array bounds, `_BitInt`
  widths, case labels, bit-field widths, alignment requests, atomic orderings.
- `sizeof` keeps its operand type as a `size_of<size_t, T>(N)` leaf
  (`ValueKind::LayoutQuery`), which every constant evaluator reads as `N`, so
  `sizeof(int) * 8` stays a `mul` over the leaf and a case label folds to a
  `const`. Rust lowering prints `size_of::<T>()` where the Rust layout of `T`
  matches and `N` otherwise (void, long double, vectors, complex, `_BitInt`,
  flexible arrays, and any record holding one of these, an `_Atomic` field, or a
  bit-field unit that Rust aligns below its size). An `_Atomic` operand, a
  declared alignment, and a function operand have no usable `T` and stay a
  `const` with `size_of` metadata.
- `_Alignof` and `__alignof__` keep the same shape, `align_of<size_t, T>(N)`,
  and lower to `align_of::<T>()` under the same layout gate. They stay a `const`
  with `align_of` metadata for an `_Atomic` operand, a declared alignment on the
  type, object or member, a function operand, and a GNU preferred alignment
  above the ABI alignment.
- `offsetof` keeps its member path as `offset_of<size_t, T, a.b>(N)` and lowers
  to `offset_of!(T, a.b)` under the same gate, through unions too. Rust lowering
  prints `N` for a member reached through an anonymous record or an erased
  field. An `_Atomic` type or an array index in the path has no usable form and
  stays a `const` with `offset_of` metadata.
- An `offsetof` member name resolves through anonymous struct and union members
  at any depth, as member access does, and adds each level's offset
  (`clang/linux/x86_64/offsetof_anonymous_members.c`). A bit-field found that
  way is still `Rejected` (`error/.../ir_layout_constants.c` `ANON_BITFIELD`).
- An `offsetof` array index that is not an integer constant (the kernel's
  `container_of(p, struct task_struct, pid_links[type])`) lowers as clang emits
  it: the constant part folds into the `offset_of` leaf, then each runtime index
  is explicitly converted to `size_t` and added as
  `add(offset, mul(index, element_size))`, all wrapping
  (`clang/linux/x86_64/offsetof_runtime_index.c`). Such an `offsetof` is not an
  integer constant expression; a non-integer index is `Rejected`
  (`FLOAT_INDEX`).
- `sizeof` of a VLA type is a runtime computation over captured extents, never a
  constant.
- `sema/fold.rs` goes past C's integer-constant-expression rule, as clang, gcc,
  and cl do: binary float arithmetic, conversions, comparisons, and conditionals
  fold (`enum { E = (int)(2.5 * 2) }`), each rounded nearest-even in its own
  format. Decimal floats never fold. An out-of-range float→int folds per flavor:
  clang saturates (NaN → 0), MSVC wraps below 2^64 and else gives 0, gcc does
  not fold (`ir_float_constant_folding.c`).
- Casts through a pointer fold too, as all three oracles do: `null`,
  `int_to_ptr` of a constant (wrapped to the target's pointer width) and
  `pointer_cast` give an address, which `ptr_to_int`, a pointer `eq`/`ne` and a
  cast to `_Bool` read. The kernel's `case (unsigned long) SEND_SIG_NOINFO:`
  needs it (`ir_pointer_cast_constants.c` on x86_64 and i686). Pointer
  arithmetic, pointer difference, comparisons, `!p`, `&&`/`||` on pointers and
  `&((T *)0)->m` (members, indices, `*p`) fold in the checker
  (`TypeResolver::constant_pointer_binary`, `constant_place`), which has the
  layout `sema/fold.rs` lacks. They leave an `int_to_ptr` of a constant or a
  plain integer constant in the IR (`ir_pointer_arithmetic_constants.c`).
- clang flavor only: a bound that reads a `const` integer object with a constant
  initializer folds when the array has static storage or is `extern`
  (`const int n = 3; int a[n];`, `static int b[n];`), as clang does under
  `-Wgnu-folding-constant`. gcc and cl reject both. The checker records such
  objects (`TypeResolver::declare_folded_object`, value wrapped to the object's
  type) and `constant` reads them only while `fold_object_bounds` is set, which
  the declaration paths set for static and file-scope declarators; an automatic
  `int d[n]` stays a VLA (`ir_const_object_array_bounds.c`).
- Layout constants fold through `TypeResolver::constant_integer`, which is
  target-aware and sees predefines, enumerators, and `constexpr` objects. The
  token-level folder in `const_expr.rs` is for the preprocessor only. Attribute
  operands are name-resolved (`aligned(sizeof(x))` finds `x`).

## Name-resolution scopes

- C89/GNU89: control statements and unbraced bodies introduce no scope; only
  explicit `Block`s do. An `enum { T = 1 }` in an `if` condition stays visible
  afterwards.
- C99+: each selection/iteration statement and its substatements get nested
  scopes (C11 6.8.4p3, 6.8.5p5); `for` clause bindings live outside the body and
  retire after the loop. Gate: `control_statement_scopes`
  (`control_body_scopes.c`).
- Enum and tag definitions inside expressions resolve at their lexical
  declaration point. Name resolution never flattens compound scopes.

## Reachability pruning

`filter_translation_unit` (`src/reachability.rs`) runs in the parser, before
sema, and drops every file-scope declaration and tag not reachable from a root,
so unused header contents are never resolved or emitted. The target design
resolves first and prunes on resolved dependencies, so Rust emission never
repeats name lookup.

A reachable tag name also keeps the declaration that first names it at file
scope, including a mention inside a member declaration
(`struct g { struct e *p; };`) or a nested definition. Dropping it would leave a
later prototype's `struct e` prototype-scoped and so a distinct type.

Consequence: unreachable declarations are never validated. A header error
surfaces only once something uses the declaration (e.g. glibc `__attr_dealloc`
under the gcc flavor); clang and gcc reject it on include.

Roots today:

- every declaration in the main file or a `-include` file;
- every file-scope pragma;
- declarations with a retention attribute: `used`, `retain`, `constructor`,
  `destructor`, `alias`, `weakref`, `ifunc`;
- from any file, every definition clang would emit: non-`static` function
  definitions, file-scope object definitions without `extern` (tentative ones
  included, function-typedef declarations excluded), and `extern` objects with
  an initializer. This keeps `#include "other.c"` tests from lowering to an
  empty module (`reachable_from_include.c`).
- an `inline` definition only when clang gives it a non-discardable linkage,
  judged over every file-scope declaration of the name:
  - `gnu_inline` or gnu89 keeps plain `inline` and drops `extern inline`;
  - C99 keeps `extern inline`, or `inline` with any non-`inline` redeclaration;
  - windows-msvc keeps only `dllexport` or an `extern` redeclaration;
  - everything else is `linkonce_odr` in clang and kept only if used
    (`reachable_inline_from_include*.c`).

Edges are by name, not scope. Each file-scope declaration is indexed under its
declared names, the names of tags it defines or first references, and its
enumerators. Marking a declaration walks its types, declarators, initializers,
bodies, attributes, and asm operands; every identifier, typedef name, or tag
name found marks every declaration indexed under it. A local that shadows a
global keeps the global: over-approximation is harmless. Attribute edges:
`alias`, `weakref`, `ifunc`, `cleanup` targets, and expression operands
(`aligned`, `vector_size`, `alloc_size`, ...).

Keeping a declaration keeps every declaration of its name, since redeclarations
change emission and attributes.

Every file-scope pragma is a root, since pack, `ms_struct`, visibility, and FP
pragmas in a header affect later declarations in any file
(`pruning-header-pragmas.c`). Adding an AST variant means extending the walk:
[ast-enum-touchpoints](../ast-enum-touchpoints.md).
