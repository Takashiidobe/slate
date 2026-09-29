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
| `parse src.c --dump-ir-types --show-metadata` | aliases and function signatures, without lowering bodies |
| `parse src.c --dump-ir-expressions [--show-spans]` | expression roots only: no module header, declarations, return conversions, or control flow |

- `Module::display(false)` prints required semantics (target, layout,
  linkage, storage duration, operation contracts); `display(true)` adds
  metadata. Metadata is ordered per node and string values are escaped.
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
- `sizeof` of a VLA type is a runtime computation over captured extents,
  never a constant.
- `sema/fold.rs` goes past C's integer-constant-expression rule, as clang,
  gcc, and cl do: binary float arithmetic, conversions, comparisons, and
  conditionals fold (`enum { E = (int)(2.5 * 2) }`), each rounded
  nearest-even in its own format. Decimal floats never fold. An
  out-of-range float→int folds per flavor: clang saturates (NaN → 0), MSVC
  wraps below 2^64 and else gives 0, gcc does not fold
  (`ir_float_constant_folding.c`).
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

Consequence: unreachable declarations are never validated. A header error
surfaces only once something uses the declaration (e.g. glibc
`__attr_dealloc` under the gcc flavor); clang and gcc reject it on include.

Roots today:

- every declaration in the main file or a `-include` file;
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

Pragmas in headers are not roots and are pruned, so a header's
`#pragma pack` is lost (slate-parser-mvaj). Adding an AST variant means
extending the walk: [ast-enum-touchpoints](../ast-enum-touchpoints.md).
