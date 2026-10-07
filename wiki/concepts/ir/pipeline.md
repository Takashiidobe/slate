# IR pipeline, dumps, and pruning

<!-- toc -->
- [Placement](#placement)
- [Failure kinds](#failure-kinds)
- [Node identity and metadata](#node-identity-and-metadata)
  - [C type metadata](#c-type-metadata)
- [Dumps and CLI](#dumps-and-cli)
- [Constant evaluation is required, not an
  optimization](#constant-evaluation-is-required-not-an-optimization)
- [Name-resolution scopes](#name-resolution-scopes)
- [Reachability pruning](#reachability-pruning)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). How sema produces the IR, how to print
it, what lowering may fold, and which declarations survive.

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

`tools/corpus_sweep.py` triages failures by `ResolveError` variant:
`Rejected` prints the bare message, `Unimplemented` prints
`not implemented: …`, `Internal` prints `internal error: …`. Full policy:
[sema-passes](../sema-passes.md#resolveerror-policy).

## Node identity and metadata

- Every IR node has its own `NodeId` and a `Span` carrying spelling and
  expansion locations, header provenance, and macro origin from the AST
  node it lowers.
- A node synthesized from another node's span (conversion wrappers, hoisted
  temporaries and their reads, declarator-anchored initializers, type
  definitions created by a declarator) takes a fresh id via `Span::derive`.
  A rewrite of the same node (the effects pass) keeps its id with
  `Span::with_value`.
- Nothing looks an IR node up by AST id. Enumerator references resolve
  through `TypeResolver.constants`, keyed by `BindingId`.
- `Module.metadata` is keyed by IR node id and written with
  `Module::annotate` on the node that owns the fact, so each key prints on
  exactly one node. A declarator's C type annotates the declared entity, not
  type definitions it happens to create (`ir_metadata_single_owner.c`).
- Parentheses are transparent; the inner operation keeps its own span.
- Source comments are non-executing metadata held in `Module.comments`
  (`ir::Comments`); `Statement::Comment` keeps tagged sibling comments inside
  bodies. Every comment is an `ir::Comment` (raw text, `attach`, `doc`; see
  [comment-placement](../comment-placement.md)) with source spans.
  - `owned` maps the `NodeId` of a function, global, type definition, field,
    enumerator, or parameter to its `leading`, `trailing`, and `inner`
    comments. Sema resolves ownership from the AST siblings, so consumers never
    see the AST.
  - `detached` lists file prologue and section comments, each with a
    `position` into `order`.
  - `order` lists the top-level IR nodes in source order with their file, so a
    detached comment anchors by node id: the next emitted node of the same
    file, else the previous one.
- Effects normalization keeps comments in order. Default IR dumps hide them;
  `--show-comments` prints `comment <attach> [doc] "raw text"` with source
  locations.
- Rust lowering moves owned comments onto the Rust node (`FnDef`, `FnParam`,
  `Item::Static`, `RecordDef`, `RecordField`) and anchors detached comments, and
  owned comments whose node is not emitted, to the next emitted item by node id.
  Comments in unreachable dispatcher blocks are retained at the end of the
  generated function.
- `translate-project` always formats with rustfmt. Single-file output uses
  rustfmt when it contains comments, since syn/prettyplease discard them.

### C type metadata

Each resolved declaration carries `c`, `c_canon` when different, and
`typedef_chain`. Qualifiers are `c_const`, `c_volatile`, `c_restrict`,
`c_atomic`, and `c_unaligned`. All are rendered from the interned C type
(`src/sema/ctype/`, see [c-type-layer](../c-type-layer.md)), never assembled
from strings:

- `c` is the written spelling.
- `c_canon` desugars typedefs and `typeof`, keeps `_Atomic(T)`, prints
  function types with adjusted parameters, and prints a C23 `()` as `(void)`.
- Function types carry no qualifier metadata.

Macro expansion chains are not in the IR yet (only atomic builtins record
`c_macro`); see [open design](open-design.md#macro-provenance).

## Dumps and CLI

| Command | Output |
| --- | --- |
| `slate-parser ir src.c` / `parse src.c --dump-ir` | the module; lowering must succeed for the whole module first |
| `+ --show-metadata` | adds the `NodeId`-keyed metadata, nested values included |
| `+ --compact-ir` | hides conversion reasons, operation policies, and call signatures (print only) |
| `+ --show-spans` | adds `[spelling=file:offset+len, expansion=file:offset+len]` to each global and function |
| `parse src.c --dump-ir-types --show-metadata` | aliases and function signatures, without lowering bodies |
| `parse src.c --dump-ir-expressions [--show-spans]` | expression roots only: no module header, declarations, return conversions, or control flow |

- `Module::display(false)` prints required semantics (target, layout,
  linkage, storage duration, operation contracts); `display(true)` adds
  metadata. Metadata is ordered per node and string values are escaped.
- `Span<Statement>::display()` prints one statement (and its nested body)
  exactly as the module printer does, without metadata, for library
  consumers such as Slate's lowering barriers. `Value::display` and
  `Place`'s `Display` cover values and places.
- The module header's `storage` lines cover every directly nameable scalar
  format (`bool`, every standard integer width, all nine float formats). A
  line is omitted only when the target lacks the type.
- Fixtures pick a renderer with `SLATE-FILECHECK-ARGS`. The harness passes
  defines, include paths, flavor, standard, and show-id options first and
  then these arguments in order, so the latter win for repeatable options.
  They are split on whitespace, with no shell quoting.

## Constant evaluation is required, not an optimization

- Keep source expressions for Rust: `1 + 2` stays an `add`. Never fold
  ordinary arithmetic, casts, comparisons, or conditionals.
- Fold only where a concrete constant is required: `sizeof`/`_Alignof`/
  `offsetof` leaves (with `size_of`/`align_of`/`offset_of` metadata), array
  bounds, `_BitInt` widths, case labels, bit-field widths, alignment
  requests, atomic orderings. `sizeof(int) * 8` folds the leaf and keeps the
  `mul`.
- An `offsetof` member name resolves through anonymous struct and union
  members at any depth, as member access does, and adds each level's offset
  (`clang/linux/x86_64/offsetof_anonymous_members.c`). A bit-field found that
  way is still `Rejected` (`error/.../ir_layout_constants.c` `ANON_BITFIELD`).
- An `offsetof` array index that is not an integer constant (the kernel's
  `container_of(p, struct task_struct, pid_links[type])`) lowers as clang
  emits it: the constant part folds into the `offset_of` leaf, then each
  runtime index is explicitly converted to `size_t` and added as
  `add(offset, mul(index, element_size))`, all wrapping
  (`clang/linux/x86_64/offsetof_runtime_index.c`). Such an `offsetof` is not
  an integer constant expression; a non-integer index is `Rejected`
  (`FLOAT_INDEX`).
- `sizeof` of a VLA type is a runtime computation over captured extents,
  never a constant.
- `sema/fold.rs` goes past C's integer-constant-expression rule, as clang,
  gcc, and cl do: binary float arithmetic, conversions, comparisons, and
  conditionals fold (`enum { E = (int)(2.5 * 2) }`), each rounded
  nearest-even in its own format. Decimal floats never fold. An
  out-of-range float→int folds per flavor: clang saturates (NaN → 0), MSVC
  wraps below 2^64 and else gives 0, gcc does not fold
  (`ir_float_constant_folding.c`).
- Casts through a pointer fold too, as all three oracles do: `null`,
  `int_to_ptr` of a constant (wrapped to the target's pointer width) and
  `pointer_cast` give an address, which `ptr_to_int`, a pointer `eq`/`ne`
  and a cast to `_Bool` read. The kernel's
  `case (unsigned long) SEND_SIG_NOINFO:` needs it
  (`ir_pointer_cast_constants.c` on x86_64 and i686). Pointer arithmetic,
  pointer comparisons and `!p` written in the expression are `Unimplemented`
  (slate-parser-6x05.43).
- Layout constants fold through `TypeResolver::constant_integer`, which is
  target-aware and sees predefines, enumerators, and `constexpr` objects.
  The token-level folder in `const_expr.rs` is for the preprocessor only.
  Attribute operands are name-resolved (`aligned(sizeof(x))` finds `x`).

## Name-resolution scopes

- C89/GNU89: control statements and unbraced bodies introduce no scope; only
  explicit `Block`s do. An `enum { T = 1 }` in an `if` condition stays
  visible afterwards.
- C99+: each selection/iteration statement and its substatements get nested
  scopes (C11 6.8.4p3, 6.8.5p5); `for` clause bindings live outside the body
  and retire after the loop. Gate: `control_statement_scopes`
  (`control_body_scopes.c`).
- Enum and tag definitions inside expressions resolve at their lexical
  declaration point. Name resolution never flattens compound scopes.

## Reachability pruning

`filter_translation_unit` (`src/reachability.rs`) runs in the parser, before
sema, and drops every file-scope declaration and tag not reachable from a
root, so unused header contents are never resolved or emitted. The target
design resolves first and prunes on resolved dependencies, so Rust emission
never repeats name lookup.

A reachable tag name also keeps the declaration that first names it at file
scope, including a mention inside a member declaration (`struct g { struct
e *p; };`) or a nested definition. Dropping it would leave a later
prototype's `struct e` prototype-scoped and so a distinct type.

Consequence: unreachable declarations are never validated. A header error
surfaces only once something uses the declaration (e.g. glibc
`__attr_dealloc` under the gcc flavor); clang and gcc reject it on include.

Roots today:

- every declaration in the main file or a `-include` file;
- every file-scope pragma;
- declarations with a retention attribute: `used`, `retain`,
  `constructor`, `destructor`, `alias`, `weakref`, `ifunc`;
- from any file, every definition clang would emit: non-`static` function
  definitions, file-scope object definitions without `extern` (tentative
  ones included, function-typedef declarations excluded), and `extern`
  objects with an initializer. This keeps `#include "other.c"` tests from
  lowering to an empty module (`reachable_from_include.c`).
- an `inline` definition only when clang gives it a non-discardable linkage,
  judged over every file-scope declaration of the name:
  - `gnu_inline` or gnu89 keeps plain `inline` and drops `extern inline`;
  - C99 keeps `extern inline`, or `inline` with any non-`inline`
    redeclaration;
  - windows-msvc keeps only `dllexport` or an `extern` redeclaration;
  - everything else is `linkonce_odr` in clang and kept only if used
    (`reachable_inline_from_include*.c`).

Edges are by name, not scope. Each file-scope declaration is indexed under
its declared names, the names of tags it defines or first references, and
its enumerators. Marking a declaration walks its types, declarators,
initializers, bodies, attributes, and asm operands; every identifier, typedef
name, or tag name found marks every declaration indexed under it. A local
that shadows a global keeps the global: over-approximation is harmless.
Attribute edges: `alias`, `weakref`, `ifunc`, `cleanup` targets, and
expression operands (`aligned`, `vector_size`, `alloc_size`, ...).

Keeping a declaration keeps every declaration of its name, since
redeclarations change emission and attributes.

Every file-scope pragma is a root, since pack, `ms_struct`, visibility,
and FP pragmas in a header affect later declarations in any file
(`pruning-header-pragmas.c`). Adding an AST variant means
extending the walk: [ast-enum-touchpoints](../ast-enum-touchpoints.md).
