# IR Spec

_created 2026-09-13 — living design doc, decisions marked **Decided** / **Open**_

Pipeline: C → AST (target-independent) → **IR (targeted)** → Rust → rewritten
Rust (Slate). This page covers the IR only. Epic: `slate-parser-lh7`.

For proposed node fields and information ownership, see [IR Shape](ir-shape.md).

The IR is not Clang IR and does not aim for CIR compatibility. It exists for
translation to Rust, not optimization. Slate's current CIR consumer will be
refactored on top of this; there is no backwards-compatibility constraint.

Guiding rules:

1. **Shown types and ops are concrete and Rust-shaped.** `int` is shown as
   `i32`, `long double` as `f80`. A conversion is `widen<i32>(x)`, not
   "integer promotion". The Rust lowering never needs C's conversion rules.
2. **C-specific information is metadata.** Original C type, typedef names,
   why a conversion happened, macro origin, array decay, header provenance —
   all kept, all optional to read. Useful context for later analyses and
   idiomization.
3. **No temporaries or storage slots beyond what the source has.** Locals are
   the source's locals; reads are implicit by position.
4. **Prune what isn't useful.** Pure C bookkeeping with no translation signal
   is dropped once resolved.

## Pipeline placement

**Decided:**

```
AST ──sema/lowering──▶ IR ──analysis pass(es)──▶ IR + facts ──▶ Rust
```

- Sema lowers the AST directly to the IR: name resolution, typing,
  conversion insertion, constant folding. No intermediate typed AST.
- Derived facts (mutability, address-taken, ranges, aliasing) are a
  **separate pass after lowering**. Lowering does not compute them. Slate's
  current analysis on lowered Rust may move here if the IR proves easier to
  analyze.

### Code layout

- `src/sema/` — semantic analysis and direct AST → typed IR lowering.
  Resolves types and operation contracts while constructing IR nodes;
  there is no intermediate semantic AST or second tree-copying pass.
- `src/sema/validate.rs` — existing early validation, still exposed through
  `TranslationUnit::analyze`. Passing it does not establish full semantic
  validity.
- `src/ir/` — typed node definitions, required semantic properties, source
  spans, and text printing. Does not interpret AST nodes or compiler flags.

### Implemented module lowering

`slate-parser ir <source.c>` (also `parse <source.c> --dump-ir`) invokes
`sema::resolve_module`. The module retains the effective target and spanned
function declarations, definitions, globals, and statements. Direct calls,
local declarations, assignment, compound assignment, increments, member and
index access, conditional and comma expressions, and scalar casts are typed
while constructing IR. Return, assignment, argument, and variadic conversions
carry their reasons. Function parameter arrays and functions adjust to pointers.
Spans retain the original AST node identity and provenance.

The module node model includes named aliases, records with field layouts,
enums with typed enumerators, globals, and function prototypes or definitions.
Pointers, arrays, and function-pointer signatures are structural types printed
inline at their uses. A direct function's `fn` declaration carries its complete
signature; it has no duplicate type-table entry. `Type::Defined(TypeId)`
references named or recursive definitions, preserving incomplete type identity.
Variables and parameters have binding IDs; root places use those IDs and concrete types.
Statements support declarations and writes as well as the numeric seed.
Pointer nulls, address-of values, byte-array constants, array decay, and
direct calls through function binding IDs are represented explicitly.
Each call value carries the signature sema resolved at its own call site,
printed as `signature=fn(...) -> T`, preserving the prototype, variadic,
and unprototyped distinction even when a later redeclaration of the same
function changes it
(`tests/fixtures/sema/ir_call_signatures.c`); `--compact-ir` hides it.

`Module::display(false)` prints required semantics, including the available
target properties, record layout, linkage, storage duration, and operation
contracts. `display(true)` additionally prints source context from the
`NodeId`-keyed metadata table, including metadata on nested values.
The CLI enables this with `--show-metadata`; spans continue to belong to
individual nodes rather than being duplicated into the table. Metadata
entries are ordered within each node and string values are escaped.

The canonical semantic module dump is `slate-parser ir source.c` or
`slate-parser parse source.c --dump-ir`. Add `--show-metadata` to either
spelling for source annotations. These commands share the same lowering and
printer, and lowering must succeed for the whole module before output is
printed. The expression and name dump modes are separate diagnostic views.
`slate-parser parse source.c --dump-ir-types --show-metadata` resolves and
prints type aliases and function signatures without lowering function bodies.
This declaration view works for `tests/fixtures/add.c`. Derived pointer, array, and function
types use `TypeId` definitions. Each resolved declaration carries `c`,
`c_canon` when different, and `typedef_chain` metadata; qualifiers are kept
as `c_const`, `c_volatile`, `c_restrict`, and `c_atomic` metadata. The shown
type is target concrete, so `size_t` and `unsigned long` both display as
`u64` on x86_64 SysV while their C metadata remains distinct.
Add `--compact-ir` to either module command for a typed view that hides
conversion reasons and operation policies such as overflow, rounding,
exceptions, and shift fill. This affects only printing; the default dump
retains those facts for FileCheck and semantic inspection. Compact mode also
propagates through dereference, field, and index places to nested values.

FileCheck fixtures select the module dump with `SLATE-FILECHECK-ARGS --dump-ir`;
add `--show-metadata` there for the annotated view. The fixture runner and
expectation generator pass dedicated defines, include paths, flavor, standard,
and show-ID options first, followed by `SLATE-FILECHECK-ARGS` in source order.
Thus the latter wins when a CLI option accepts repeated values. Extra arguments
are split on whitespace; shell quoting is not interpreted.
`tests/fixtures/sema/ir_module_promotions.c` exercises real C arithmetic,
explicit narrowing, integer promotion, and return widening.

The module dump also handles `tests/fixtures/add.c`: its stdio declaration,
parameters, local variable, direct calls, string literal, and fallthrough metadata.
Control flow retains `if`, `while`, `do/while`, `for`, `switch`, case/range/default
labels, named labels, goto, computed goto, and blocks rather than canonicalizing
loops or switches. Controlled bodies retain explicit `Block` nodes; `Null` is
separate from an empty block. A body is a list to accommodate declarations with
multiple declarators without inventing a C scope. For clauses retain their
initializer list, optional condition, and optional increment separately.

Loops and switches have unique IDs. Break targets the nearest loop or switch;
continue targets the nearest loop (including through a switch). Case labels
retain their nearest switch ID even when nested inside loops (Duff's device),
and remain nested statements rather than flattened arms. Statement order and
absence of a break preserve fallthrough. `[[fallthrough]];` is a null statement
with source metadata. Case endpoints are folded to the promoted switch type;
ordinary conditions and arithmetic are not folded. Conditions explicitly become
boolean values. Missing for conditions remain omitted, meaning unconditional.
Enum switch operands use an explicit `enum_to_int` conversion to the resolved
underlying integer type before promotion. Enumerator references in case
expressions resolve by binding and AST identity to typed constants.

Named labels, gotos, and `label_addr<ptr<void>>` use resolved label binding IDs,
including distinct GNU local labels. Name resolution records each label
definition's AST identity separately from its declaration binding identity.
Computed goto retains its pointer-valued operand. Short-circuit and conditional
expressions remain expression nodes; side-effect hoisting is separate work in
`slate-parser-lh7.2.5`. The `ir_control_*.c` FileCheck fixtures cover these forms,
nesting, compact output, and invalid control contexts.

Other unsupported statements and attributes are diagnosed instead of omitted.
Aggregate initialization and general attributed-statement lowering remain
separate work.

Target selection separates CPU family (`TargetFamily`), OS (`TargetOs`), and
ABI environment (`TargetEnvironment`) from compiler flavor. Existing Linux
profiles use GNU, GNU EABI, or GNU EABI hard-float environments.
`x86_64-pc-windows-msvc` and `aarch64-pc-windows-msvc` are experimental Windows
MSVC-environment profiles, with LLP64 widths, binary64 long double, and unsigned
16-bit wchar_t. Their target triples do not implicitly select compiler flavor:
`--flavor=msvc` loads the checked-in MSVC 19.51.36256 snapshots; the default
Clang flavor loads the Clang 22.1.8 Windows snapshots. Neither loads Linux/glibc
shim defaults. GCC on these Windows profiles is rejected rather than falling
back to Linux macros. Existing Linux flavor behavior is unchanged.

This is selection scaffolding, not Windows compatibility: Microsoft record
layout, calling conventions, extended-type availability, compiler-option
validation, standard-mode macro adjustments for native MSVC, and SDK/header
integration remain incomplete. In particular, the current aggregate algorithm
must not be treated as a Microsoft ABI oracle. Only 64-bit Windows targets are
selected for now; the checked-in 32-bit snapshots remain unwired.

The type view resolves struct, union, and enum tag definitions. On the
supported Linux x86_64, x86, AArch64, and ARM32 targets, record layout records byte size, aggregate alignment,
one byte offset per field, bit offsets for bit-fields, and byte extents for
contiguous bit-field storage units. It applies packed,
aligned, and `_Alignas` requests, including local field alignment. Unnamed
members and zero-width bit-fields remain in the field list. Enum values are
evaluated in declaration order, including references to prior enumerators;
the underlying integer type is selected from the represented range unless
the source fixes it. Enum storage retains size and alignment separately so
alignment attributes need not change the underlying integer type.
`tools/check_record_layout.py` compares each target directory's generated
layout fixture with clang's target-specific record layout dump. The fixture
runner and expectation generator derive the target triple from that directory.
ARM targets let zero-width bit-fields raise record alignment, including in
packed records; x86 targets do not.

The bit-field units describe occupied bytes; access types and bit slices for
Rust emission remain future work, as do flexible-array semantics. Full
qualifiers, callable types/ABI contracts,
projected places, and structured aggregate initializers remain in their
respective lowering tasks. The name-resolution dump remains a separate
diagnostic view, not the module declaration representation.

### Required constant evaluation, not optimization

Preserve ordinary source expressions for Rust translation. `1 + 2` remains
an addition, even when both operands are constants. Do not fold ordinary
arithmetic, casts, comparisons, or conditional expressions as an optimization.
For `sizeof(int) * 8`, fold only the `sizeof` leaf and retain the multiply.

Layout queries fold using target layout and retain `size_of`, `align_of`, or
`offset_of` metadata on their resulting constants. Evaluate expressions when
a concrete constant is required to resolve a type or layout, such as fixed
array bounds, `_BitInt` widths, and constant `offsetof` array indices. This
required evaluation must not rewrite the surrounding source expression tree.

### Implemented numeric seed

`sema::numeric::Context::resolve` lowers integer and binary floating-point
literals, parentheses, same-concrete-type `+`, `-`, `*`, `/`, `%`, `&`, `|`,
`^`, integer `<<`/`>>`, unary `-`, integer `~`, unary `+`, same-type
comparisons, `!`, `&&`, `||`, and `true`/`false` directly to
`ir::Value`. Integer literal selection uses the existing C candidate order
and target integer widths. Floating constants retain their exact value bits.
The dump prints f32/f64 numerically using round-trippable decimal formatting
(including signed zero); NaNs retain hexadecimal bits to preserve payloads.
The f16, f80, and f128 printer uses `rustc_apfloat` directly, without an f64
conversion. NaNs retain hexadecimal bits in every format.
These lower to one `ValueKind::Arith` node keyed by `ArithOp`
(`add`/`sub`/`mul`/`div`/`rem`/`and`/`or`/`xor`/`shl`/`shr`), all carrying `ArithSema` metadata. They are
not folded or reassociated. Signed overflow defaults to
`ub`, unsigned overflow to `wraps`; floating arithmetic defaults to
nearest-even rounding with ignored exceptions for the default Clang flavor
(observable exceptions for GCC). Translation-unit operation options
initialize the context's independent integer and floating semantic settings.
Overflow metadata describes the operation's behavior if overflow occurs,
not a prediction that these operands overflow. Signed add/sub/mul use the
context's signed-overflow policy; unsigned operations wrap. Signed `div`/`rem`
carry `by_zero=ub, min_by_neg_one=ub`: Clang 22 emits plain `sdiv`/`srem`
under both `-fwrapv` and `-ftrapv`, and GCC documents those flags only for
add/sub/mul. Unsigned division/remainder carry only `by_zero=ub`; they have
no overflow case. These are explicit `ArithSema::Division` policies, not
operand-range predictions. `%` on floating operands is rejected as invalid operands.
`and`/`or`/`xor` carry no overflow metadata and reject floating operands.
Shift operands are not converted to a common type: the node takes the left
operand's type and the amount keeps its own. Clang 22 emits plain
`shl`/`ashr`/`lshr` under `-fwrapv` and `-ftrapv`, so signed `shl` is always
`overflow=ub` (unsigned `overflow=wraps`). Both shifts explicitly carry
`amount_out_of_range=ub` (negative amounts or amounts at least the promoted
left width). Signed `shl` also carries `negative_left=ub`, independently of
overflow flags. `shr` resolves signed fill through `TargetInfo.signed_right_shift`
(`sign_extend` on the supported x86_64 target); unsigned fill is `zero_extend`.
Both shift operands independently undergo
integer promotion. Unary `-` and `~` lower
to `ValueKind::Unary` keyed by `UnaryArithOp` (`neg`/`not`). Signed `neg`
uses the context's signed-overflow policy (Clang 22: `sub nsw 0, x`, plain
`sub` under `-fwrapv`, `ssub.with.overflow` under `-ftrapv`); unsigned wraps.
Floating `neg` is exact and quiet (`fneg`, unaffected by `-frounding-math`),
so it prints no metadata: `neg<f64>(..)`. `not` prints no metadata and rejects
floating operands. `ArithSema::Exact` marks operations with no overflow,
rounding, or exception behavior (`and`/`or`/`xor`/`not`, floating `neg`).
Unary `+` retains only integer promotion; bool promotes to target `int`,
and integer types narrower than `int` widen before arithmetic.
Comparisons, `!`, `&&`, and `||` produce `ir::Type::Bool`, a type separate
from `NumericType`. Comparisons lower to `ValueKind::Compare` keyed by
`CompareOp` (`eq`/`ne`/`lt`/`le`/`gt`/`ge`); the printed type is the operand
type (`lt<i32>(..)`), the result is always `bool`. Mixed-type comparisons
use the usual arithmetic conversions. Floating comparisons print only
`exceptions=`: relational ops are signaling and `eq`/`ne` quiet on NaN
(Clang 22 `constrained.fcmps`/`fcmp` under `-ftrapping-math`), so the op
implies which, and rounding never applies. A numeric operand of `!`, `&&`,
or `||` lowers to `ne<T>(x, const<T>(0))`; the synthesized zero reuses the
operand's span. `!` is `not<bool>(..)`; `&&`/`||` are
`logical_and<bool>`/`logical_or<bool>`, distinct from bitwise `and`/`or`
because they short-circuit. `true`/`false` are `const<bool>(..)`. A bool
used as an integer operand (`(1 < 2) + 1`, `-true`, `(a < b) == (c < d)`)
promotes through `from_bool<int>` before the usual arithmetic conversions.
Explicit casts to bool use the same truth comparison, preserving fractional
nonzero floats and treating both signed zeros as false. Casts from bool to
integers produce 0 or 1 directly; casts to floats use `from_bool<int>` then
`int_to_float`, marked exact. The module path also inserts return and assignment conversions, and
compound assignments and increments share the same operator lowering as
ordinary binary expressions. The expression-only diagnostic view has no
places and therefore still rejects updates.

The initial numeric type stores integer width/signedness or one of five
floating formats: f16, f32, f64, f80, f128. These denote value formats, not
storage sizes. Sema resolves `long double` through `TargetInfo.long_double`:
the current x86-64 Linux baseline uses f80; `-mlong-double-64/80/128`
selects the corresponding format. Literal digits are parsed directly into
that format, never rounded through an intermediate f80 or f64 value.
It does not yet implement the full type/storage metadata proposed below.
Builtin scalar casts and mixed-type arithmetic insert conversion nodes with
`reason=promotion|usual_arith|explicit`. Integer width changes precede sign
changes. Float narrowing and integer-to-float conversions carry rounding
and exception settings; float-to-int truncates toward zero and records
`out_of_range=ub` plus exception settings. Widening floats is exact.
Typedef and non-scalar casts, `_BitInt`, decimal/imaginary
literals, target-dependent `f64x` suffixes, and other expressions return
explicit unsupported errors.
Supported flags are `-f[no-]wrapv`, `-f[no-]trapv`,
`-f[no-]strict-overflow`, `-f[no-]rounding-math`, and
`-f[no-]trapping-math`, plus the long-double options above. Scoped pragma
semantics and function attribute overrides are not yet wired into this path.
The `pointer_wrap` setting implied by strict-overflow options controls pointer
offset overflow, independently of integer overflow. `-fwrapv` alone does not
change pointer contracts.

`CompilerOptions` groups operation and layout settings and preserves ordered
compiler arguments on the translation unit. Argument provenance supplies no
missing operation semantics. The module dump retains the effective target;
the expression-only diagnostic mode does not print a module header.
Other flag families remain unsupported.

Each `Value` owns `Span<ValueKind>`, retaining node identity, spelling and
expansion locations, header provenance, and macro origin from its AST node.
Parentheses are transparent; the surviving inner operation retains its own
span. The printer always shows required operation semantics. Optional
`--show-spans` prints spelling/expansion file IDs and byte ranges on every
node; the accompanying `Files` from parsing resolves those IDs.

```sh
cargo run -- parse source.c --dump-ir-expressions --show-spans
```

This diagnostic mode prints expression roots from function expression and
return statements, not functions or an executable module. It does not
resolve return conversions, declarations, or control flow. Fixtures live in
`tests/fixtures/sema/`; `SLATE-FILECHECK-ARGS` supplies extra renderer
arguments to both the test harness and expectation generator.

```text
add<i32, overflow=ub>(const<i32>(1), const<i32>(2))
add<u32, overflow=wraps>(const<u32>(1), const<u32>(2))
add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), const<f64>(2.0))
sub<u32, overflow=wraps>(const<u32>(1), const<u32>(2))
```

### Validation and declaration pruning

**Decided:** early validation reports structural errors and may remove
structurally invalid items with diagnostics. Checks requiring scopes,
resolved types, conversions, or layout belong to `src/sema/`. Failures
there produce diagnostics; lowering must not assume that surviving early
validation proves a node valid or silently discard failed operations.

Name resolution reads `TranslationUnit.standard`. C89/GNU89 control statements
and unbraced bodies introduce no implicit scope; explicit AST `Block` nodes do.
C99+ selection/iteration statements and their bodies each have nested scopes,
with an explicit `Block` supplying the braced body's scope. In these modes,
keep `for` clause bindings outside the body scope and retire them after the loop. Resolve enum
and tag definitions inside expressions at their lexical declaration points.
The AST preserves each body as one statement; name resolution must not flatten
away compound scopes or infer scopes solely from braces in C99+.

For the IR pipeline, resolve declarations before pruning them. Reachability
uses resolved symbol dependencies and explicit roots: requested translation
entries, exported symbols, and declarations retained for linkage or
attributes, including constructors and `used` declarations. Preserve the
types and declarations required by those roots. Rust emission must not need
to repeat name lookup to discover dependencies.

The current parser calls `filter_translation_unit` before resolution, and
`src/reachability.rs` indexes declarations by string names. Moving that
filter after resolution is required for this design.

### Remaining prerequisites

- **Scopes and general typing.** The numeric seed does not resolve names,
  declarations, or assignment/argument/return conversions.
- **Target data layout.** Supported Linux x86_64, x86, AArch64, and ARM32
  profiles expose endian, pointer, scalar storage, long-double, stack alignment,
  and record layout policy. Calling ABI details remain future work.
- **Full provenance model.** Existing spans and macro origins survive the
  numeric lowering. The expansion records proposed below remain future work.

## Module shape

```
Module
  target:    triple + endian + scalar/pointer storage + stack ABI policy
  types:     records (with computed layout), enums
  globals:   name, type, linkage, initializer, metadata
  functions: name, linkage, parameters, return type, variadic, body, metadata
  metadata:  side table NodeId → Metadata
```

**Open:** "type parameters" on functions — needs a concrete C use case
(`_Generic`, `<tgmath.h>`, type-generic macros) before it gets a slot.

### Function bodies

**Decided:** a body is a list of statements containing typed expression
trees.

- Statements: `let`, assignment/`write`, expression statement, `return`,
  `if`, `loop`/`while`, `switch`, `break`, `continue`, `goto`, label, block.
- Expressions are side-effect-free except calls. Every node has a `NodeId`
  and concrete type; metadata is looked up by `NodeId`.
- Local and global references use stable binding IDs; field references use
  field IDs. Names such as `c` and `c#1` are printer spellings, not identity.
  Locals are bound by `let`; a local name in value position prints a read
  of its place (see [Places and values](#places-and-values)).

### Resolved control flow

**Decided:** keep structured loops and switches, with resolved destinations.
Loops, switches, and labels have stable IDs. `break` identifies its loop or
switch; `continue` identifies its loop and continuation point; direct
`goto` and label-address values identify labels rather than source names.
Computed goto retains its evaluated destination value.

- A `for` continuation evaluates the increment, then the condition.
- A `while` continuation evaluates the condition; a `do`/`while`
  continuation reaches the trailing condition.
- Switch dispatch maps resolved case values/ranges and default to label
  IDs. Fallthrough destinations are explicit, including cases nested
  inside other statements; do not assume every switch is a list of
  independent arms.

Rust lowering consumes these destinations directly. It does not rediscover
targets by walking AST parents or looking up label names. The source loop
form remains available for producing readable Rust.

### Side effects are hoisted into statements

**Decided:** assignments, `++`/`--`, and compound assignments nested in
expressions are pulled out into their own statements, since Rust has no
compact form for them.

**Decided:** control flow keeps its source form. `while`, `do`/`while`,
`for`, `switch` and `goto` lower as themselves, not as `loop` + `break`.
The goal is semantics first, then syntax, not optimization; the original
shape tells the Rust rewriter what the "ideal" form was.

Side effects in a loop's condition or `for` increment are hoisted into a
statement block evaluated in that position, with the condition as its
trailing expression (valid Rust: `while { ch = getc(f); ch != EOF } {}`):

```c
a[i++] = x;
while ((ch = getc(f)) != EOF) { ... }
```

```
write(index(a, i), x);
i = add(i, 1i32) [overflow=ub, from=post_inc];

while { ch = getc(f); ne(ch, -1i32 [macro=EOF]) } {
    ...
}
```

Hard cases hoisting must respect (evaluation order and sequencing):

- `f(i++)`: C increments before the call executes. `f(i); i = i + 1;` is only
  valid if `f` cannot observe `i` (a local that is not address-taken, but
  that fact isn't computed yet during lowering). Otherwise a synthetic
  temp is needed: `let t#0 = i [synthetic]; i = add(i, 1); f(t#0);`.
  **Open:** always emit the temp during lowering and let the analysis pass
  remove it, or special-case never-escaping locals up front.
- `&&`, `||`, `?:`, comma with side effects in a later operand: hoisting
  must turn into `if`, not unconditional statements.
  `if (p && p->n++)` → `if p != null { let t = read(field(deref(p), n)); write … ; if t != 0 { … } }`.
- Loop conditions and `for` increments with side effects stay attached to
  their `while`/`for` as a statement block with a trailing expression, so
  they re-run per iteration without changing the loop's form.
- Assignment used as a value (`x = y = 0`) → sequential statements, with the
  value read back from the assigned target.

## Types

**Decided:** the shown type is the concrete, target-resolved type. The
original C type is metadata.

| C                                        | Shown                            | Metadata                              |
| ---------------------------------------- | -------------------------------- | ------------------------------------- |
| `int`                                    | `i32`                            | `c=int`                               |
| `long` (LP64 / LLP64)                    | `i64` / `i32`                    | `c=long`                              |
| `char` / `signed char` / `unsigned char` | `i8`/`u8` per target, `i8`, `u8` | `c=char` etc.                         |
| `_Bool`                                  | `bool`                           | `c=_Bool`                             |
| `size_t`                                 | `u64`                            | `c=size_t`, `c_canon=unsigned long`   |
| `float` / `double`                       | `f32` / `f64`                    | `c=float` / `c=double`                |
| `long double` (x86)                      | `f80`                            | `c=long double`                       |
| `__float128`                             | `f128`                           |                                       |
| `const char *`                           | `*const i8`                      | `c=const char *`                      |
| `enum E`                                 | `enum E` (underlying `u32`)      | underlying type computed per compiler |
| `struct S`                               | `struct S`                       | layout in module                      |

The whole typedef chain is kept in metadata (`uint32_t` → `__uint32_t` →
`unsigned int`) since it's the strongest idiomization signal
(`size_t` → `usize`, `c_int` in FFI signatures).

### Records

Layout is computed during lowering: field offsets, padding, alignment,
bit-field storage units. Source field order and names kept; anonymous
members get a synthesized name plus `anonymous` metadata.

## Objects, lifetime, and initialization

**Decided:** object identity, storage duration, and initialization are
semantic information available to Rust lowering, not optional source-form
metadata. Object declarations identify their concrete type, automatic,
static, or thread storage duration, and lifetime scope. Static locals have
stable global object identity even though their names have block scope.
Compound literals retain their own object identity and resolved lifetime;
they are not merely interchangeable aggregate values.

- Distinguish uninitialized storage from initialized values. Track explicit
  initialization and semantic zero initialization, including omitted
  aggregate subobjects. Do not replace semantic zero initialization with
  an assumed all-zero byte pattern or synthesize zero for an uninitialized
  automatic object.
- Resolve initializer designators to field IDs and element indices/ranges.
  Keep aggregate initialization structured, with omitted-element defaults
  and evaluation behavior explicit, rather than expanding every element
  into a store. Braces, trailing commas, and designator spelling remain
  source context.
- Union initialization identifies the selected member and its value.
  Bit-field access retains the field's width, signedness, and storage-unit
  layout so reads and writes preserve the required behavior.
- Variable-length arrays retain runtime extents, their evaluation points,
  and their object lifetime. Subsequent size computations and indexing use
  the captured extents; do not re-evaluate the original bound expression.
  Runtime `sizeof` is represented as a computation rather than folded.

These facts support Rust storage and initialization choices; ownership,
escape, and definite-initialization analysis can derive additional facts
in later passes. Related implementation work: `lh7.2.8` and `lh7.2.9`.

## Provenance

**Decided:** every value is both concrete and optionally traceable.

```
Origin {
  loc:       spelling + expansion Loc
  system_header: Provenance (first system header entered from user code)
  expansion: Option<ExpansionId>
  reason:    Option<Reason>   // promotion, implicit main return, ...
}

Expansion {
  macro:      "INT_MAX"
  kind:       ObjectLike | FunctionLike { args }
  defined_at: Loc + Provenance
  parent:     Option<ExpansionId>   // INT_MAX → __INT_MAX__
}
```

`2147483647i32 [macro=INT_MAX]` lets Rust lowering emit `c_int::MAX` for
`<limits.h>` names or a `const` for user macros; a consumer that ignores
metadata just sees the number. A function-like expansion covers every node it
produced, so a run can be turned back into a `fn`.

Requires: pp records an `ExpansionId` per expanded token with its chain;
the parser propagates it into every expression node, including `ConstExpr`.

## Conversions

Each conversion node does exactly one thing; the reason is metadata.

| Node                                           | Meaning                                                  | Metadata                                                       |
| ---------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------------- |
| `widen<i32>(x)`                                | value-preserving sign/zero extend (by source signedness) | `reason=promotion\|usual_arith\|assign\|arg\|vararg\|explicit` |
| `truncate<i8>(x)`                              | keep low bits                                            | `fits=always\|unknown`                                         |
| `reinterpret<u32>(x)`                          | same width, sign change                                  | `fits=always\|unknown`                                         |
| `from_bool<i32>(b)`                            | 0-1 (truth conversion is `ne(x, 0)`, not a node)         |                                                                |
| `float_widen<f64>(x)` / `float_narrow<f32>(x)` |                                                          |                                                                |
| `int_to_float<f64>(x)`                         |                                                          | `exact=true\|false`                                            |
| `float_to_int<i32>(x)`                         |                                                          | `out_of_range=ub`                                              |

A C conversion changing width and signedness is two nodes in fixed order:
width first (in source signedness), then reinterpret.
`(u32)(i8)x` → `reinterpret<u32>(widen<i32>(x))`.

`fits` is only filled in when trivially known during lowering (constants);
range-based facts belong to the analysis pass.

### Promotions

```c
short inc(short s) { return s + 1; }
```

```
fn inc(s: i16 [c=short]) -> i16 [c=short] {
    return truncate<i16>(
        add(widen<i32>(s) [reason=promotion], 1i32) [overflow=ub]
    ) [reason=return];
}
```

The widen and truncate stay in the IR because `s`'s storage really is `i16`
(struct layout, stores truncate). The metadata is what lets Slate decide to
emit `s.wrapping_add(1)` or retype `s` as `i32` when analysis proves every
store fits.

## Arithmetic

Ops run on concrete widths: `add(a, b)`, `sub`, `mul`, `div`, `rem`, `shl`,
`shr`, `and`, `or`, `xor`, `neg`, `not`, `eq`/`ne`/`lt`/…

- `overflow=wrap|ub` (unsigned / signed). `impossible` is an analysis fact,
  not emitted by lowering.
- `div`/`rem`: `by_zero=ub`; signed also `min_by_neg_one=ub`.
- `shl`/`shr`: amount type kept separately; `amount_out_of_range=ub`; `shl` of
  a negative signed value is `ub`. Right shift of negative signed values is
  resolved by target (`shr<i32, fill=sign_extend>`).
- Comparisons, `!`, `&&`, `||` produce `bool`; `from_bool<i32>` is inserted
  only where the result is used as an integer.
- Scalars in boolean context lower to `ne(x, 0)` / `is_non_null(p)`.
- Short-circuit and `?:` with side-effect-free operands stay as expression
  nodes (`logical_and`, `logical_or`, `select`); with side effects they become `if`
  statements (see hoisting).

## Pointers

A C pointer can be a borrow, a nullable borrow, a slice cursor, an owning
handle, an out-parameter, a C string, an opaque handle or a function pointer.
**Lowering does not decide which.** It emits uniform pointer operations and
keeps every local fact as metadata for the analysis pass and Slate.

### Implemented pointer arithmetic

`PointerOffset` records the pointer, promoted integer amount, element type,
add/subtract direction, and pointer-overflow policy. Offsets are in element
units, not bytes. Keeping subtraction as a direction avoids negating an
unsigned amount (which would wrap before applying the offset). Pointer
`+`/`-`, `+=`/`-=`, pre/post increment/decrement, and indexing share this path;
indexing currently lowers to `deref(ptr_offset(...))`. An update keeps a single
place and an `OldValue` computation, so side-effecting indices are not duplicated.

`PointerDifference` records both pointers and their compatible element type;
its signed result is pointer-width on the supported x86_64 target (`ptrdiff_t`
is i64). Its operation contract requires pointers into the same array (or its
one-past position), and a difference representable in the result type. Neither
condition is claimed proven. Pointee const differences are allowed. Complete
record and fixed-array elements retain their structural types; incomplete,
void, function, incompatible, and noninteger-offset cases are rejected rather
than guessed. GNU void/function-pointer arithmetic is not implemented.

`ir_operator_semantics.c` and its wrap/trap/compact variants cover numeric
contracts through module lowering. `ir_pointer_arithmetic.c` and its wrap
variant cover offsets, differences, updates, and element types. Error fixtures
cover unsupported pointer cases. No range-based facts are inferred.

### Places and values

**Decided:** a typed place describes a storage location; a value is the
result of computation. Places retain object and projection structure:

```
Place = Local(LocalId) | Global(GlobalId) | Deref(Value)
      | Field(Place, FieldId) | Index(Place, Value)
```

`read(place)`, `write(place, value)`, and `addr_of(place)` consume places.
`Index` projects into an array place. Pointer indexing uses
`Deref(ptr_offset(pointer, index))`, with offsets in element units.
Each place has a resolved type; field IDs refer to the record layout.
Bit-fields are readable/writable projections but are not addressable.

For `p->a[i]`, where `a` is an array member, the place is
`index(field(deref(p), a), i)`. Reading or writing it accesses that element,
without reading the whole record or flattening away the array member.
Selecting a field from an aggregate value is a value projection and does
not imply that the value has addressable storage.

Local reads and assignments may print as ordinary names and `x = value`;
this does not require allocating a storage slot for every local. Places
fit the existing statement and typed-expression-tree representation.
Accesses retain the applicable alignment and volatile/atomic behavior;
forming a place alone does not read its stored value. Lowering must preserve
single evaluation of side-effecting bases and indices when reusing a place.

This resolves the member-access choice for `lh7.2.6` and `lh7.2.7` in favor
of projections on places.

| C                        | IR                                          | Metadata                           |
| ------------------------ | ------------------------------------------- | ---------------------------------- |
| `a[i]` (array read)      | `read(index(a, i))`                         | `form=index`                       |
| `a[i] = v` (array)       | `write(index(a, i), v)`                     | `form=index`                       |
| `*p`                     | `read(deref(p))`                            | `form=deref`                       |
| `*p = v`                 | `write(deref(p), v)`                        | `form=deref`                       |
| `*(p + i)` / `p[i]`      | `read(deref(ptr_offset(p, i)))`             | `form=deref_offset` / `form=index` |
| `p + i`, `p++`           | `ptr_offset(p, i)` / `p = ptr_offset(p, 1)` | elem type                          |
| `p - q`                  | `ptr_diff(p, q)` → `i64`                    | elem type, `c=ptrdiff_t`           |
| `p < q`                  | `ptr_lt(p, q)`                              |                                    |
| `&x`                     | `addr_of(x)`                                |                                    |
| `arr` in pointer context | address of its first element, typed `*T`    | `decay[len=N]`                     |
| `f` as value             | `f` typed `*fn(..)`                         | `decay=function`                   |
| `0`, `NULL`, `(void*)0`  | `null<*T>`                                  | `macro=NULL` if applicable         |
| `if (p)`, `!p`           | `is_non_null(p)` / `is_null(p)`             |                                    |
| `char* → const char*`    | (no node)                                   | `add_const`                        |
| `void* ↔ T*`             | `ptr_cast<*T>(p)`                           | `implicit`                         |
| `(uintptr_t)p` / `(T*)n` | `ptr_to_int<u64>(p)` / `int_to_ptr<*T>(n)`  |                                    |

Original pointer qualifiers are retained as metadata; volatile/atomic
access behavior is also resolved on the actual accesses. Pointee `const`
is shown in the type (`*const T`) since Rust distinguishes it.

## Things C leaves implicit that the IR materializes

- C99 and later `main` falling off the end → `fallthrough=ret_zero` on its definition.
- A void function falling off the end → `fallthrough=ret_void`; a non-void function reached at the end has `fallthrough=ub_if_used`.
- Constant `sizeof`/`_Alignof`/`offsetof` → folded value with `size_of=T`
  metadata; runtime array sizes use captured extents.
- String literals: `c"..."` typed `*const i8`, metadata `c=char[N]`, `decay[len=N]`.
- Tentative definitions and `extern` declarations merge into one global with linkage; an incomplete tentative array completes to one element.
- Pre-C23 `f()` is unprototyped; C23 `f()` and `f(void)` are zero-parameter prototypes.
- Default argument promotions for variadic calls → `widen`/`float_widen`
  with `reason=vararg`.
- Struct and union copy on initialize/assign/pass/return → `copy<T, reason=...>` around the source value.

## Worked example

```c
#include <stdio.h>
int add(int a, int b) { int c = a + b; return c; }
int main(void) { printf("%d\n", add(2, 3)); }
```

The executable version is `cargo run --example ir_module`; its generated
FileCheck fixture is `tests/fixtures/ir/module.c`. It includes additional
declarations to exercise the module tables. These excerpts omit those
tables and the target header for readability. IDs identify declarations;
source names remain available beside them.

Source metadata shown (excerpt):

```text
fn %10 @add(%0 a: i32 [c="int"], %1 b: i32) -> i32 [linkage=external] [source="add.c"] {
    let %2 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%0) [c="a"], read<i32>(%1)) [source="a + b"] [c="int c"];
    return read<i32>(%2);
}
```

Source metadata hidden (required semantics remain visible):

```text
fn %10 @add(%0 a: i32, %1 b: i32) -> i32 [linkage=external] {
    let %2 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%0), read<i32>(%1));
    return read<i32>(%2);
}

fn %17 @main() -> i32 [linkage=external] {
    call<i32>(%15, array_decay<@type11>(%14), call<i32>(%10, const<i32>(2), const<i32>(3)));
    return const<i32>(0);
}
```

## Open questions

1. Hoisting `f(i++)`: always emit synthetic temps and let analysis remove
   them, or special-case during lowering.
2. What function "type parameters" represent in C.
3. Metadata printer syntax (`[k=v]` trailing per node is the working form).

## Explicit calling conventions at the AST boundary

The AST carries typed calling-convention requests on declaration specifiers
and nested/trailing declarator attributes, including unevaluated `regparm`
expressions. Sema must combine these with the target and compiler options
when resolving function and function-pointer ABIs. This parsing support
does not yet implement ABI resolution or change the current IR schema.
