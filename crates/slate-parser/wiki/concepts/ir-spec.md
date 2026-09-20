# IR Spec

_created 2026-09-13 — living design doc, decisions marked **Decided** / **Open**_

Pipeline: C → AST (target-independent) → **IR (targeted)** → Rust → rewritten
Rust (Slate). This page covers the IR only. Epic: `slate-parser-lh7`.

The syntax of the printed IR (every node, its fields, and their choices) is
specified in [IR Grammar](ir-grammar.md). This page explains the design
behind it and records what is implemented. For proposed node fields and
information ownership, see [IR Shape](ir-shape.md).

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
Spans retain the source location and provenance of the AST node they lower.
Every IR node has its own `NodeId`: a node synthesized from another node's
span (conversion wrappers, hoisted temporaries, reads of temporaries,
initializer values anchored at a declarator, type definitions created by a
declarator) takes a fresh id via `Span::derive`. A rewrite of the same node
(the effects pass) keeps its id with `Span::with_value`. Nothing looks up an
IR node by an AST id: enumerator references resolve through
`TypeResolver.enumerators`, keyed by the AST enumerator's id.
`Module.metadata` is keyed by IR node id and written with `Module::annotate`
on the node that owns the fact, so each key prints on exactly one node. A
declarator's C type annotates the declared entity (let, global, function,
parameter, typedef alias), not type definitions it happens to create.
`tests/fixtures/sema/ir_metadata_single_owner.c` covers this.

The module node model includes named aliases, records with field layouts,
enums with typed enumerators, globals, and function prototypes or definitions.
Pointers, arrays, and function-pointer signatures are structural types printed
inline at their uses. A direct function's `fn` declaration carries its complete
signature; it has no duplicate type-table entry. `Type::Defined(TypeId)`
references named or recursive definitions, preserving incomplete type identity.

Tag identity is per scope, not per name (`slate-parser-8lv`). A block-scope
`struct Local { ... }` in two different functions gets two `TypeId`s with their
own layouts, and a bare `struct S;` declares an *incomplete* tag in the current
scope per C11 6.7.2.3p8, hiding any outer tag of that name rather than
referring to it. So after `struct Outer { int x; };` at file scope, a function
body containing `struct Outer;` sees a distinct incomplete `Outer`, and
`sizeof` of it is rejected as clang and gcc reject it. A forward declaration
followed by a definition in the *same* scope completes the one tag.
Fixture: `sema/ir_tag_scopes.c`.
Variables and parameters have binding IDs; root places use those IDs and concrete types.
Statements support declarations, writes, and numeric operations.
Pointer nulls, address-of values, byte-array constants, array decay,
function decay, and calls are represented explicitly. A call names its callee
as a binding ID (`call<T>(%3, ...)`) when the designator resolves to a known
function, as a builtin spelling (`call<T>(__builtin_memcpy, ...)`) for a
compiler-provided function-like builtin, or as a pointer value
(`call<T>(read<ptr<fn(..)>>(%7), ...)`) for an indirect call. Builtin callees
carry `c_builtin` metadata and have no fabricated source declaration or binding.
Each call value carries the signature sema resolved at its own call site,
printed as `signature=fn(...) -> T`, preserving the prototype, variadic,
and unprototyped distinction even when a later redeclaration of the same
function changes it
(`tests/fixtures/sema/ir_call_signatures.c`); `--compact-ir` hides it.

Clang's target-independent `Builtins.td` is expanded through `clang-tblgen`
into the checked-in Rust registry in `src/sema/clang_builtins.rs`. The generator
parses each `Prototype` spelling into a typed `BuiltinPrototype` and emits
`BuiltinAttribute`/`BuiltinLanguage` enum values, so the registry carries types
rather than strings and an unparseable prototype fails generation. Sema derives
a builtin's call signature from that model
(`TypeResolver::builtin_signature`), which is what lets a recognized builtin be
called without a source declaration. A builtin gets no derived signature when it
is marked `CustomTypeChecking`, when its prototype is variadic with no named
parameters, or when the prototype names something with no C type in our model
(`FILE`, `jmp_buf`, ObjC `id`, HLSL resources, C++ references, ext-vectors);
those still report an unsupported-builtin diagnostic. Named types resolve to the
target's canonical types, not to any typedef the translation unit declares.
Ordinary declarations take precedence over implicit builtin recognition.

Builtins whose result cannot come from a prototype are dispatched by their
tblgen record through `builtins::custom_builtin`, which returns a typed
`CustomBuiltin` rather than matching spellings at the lowering site. The
floating classification family lowers to
`float_class<bool, test=..>(x)`, a non-trapping IEEE class test that the
`int`-returning builtins wrap in `from_bool<int>`: `__builtin_isnan`,
`isinf`, `isfinite`, `isnormal`, `issubnormal`, `iszero`, `issignaling` and
`signbit`. `__builtin_isinf_sign` composes two of them into nested
`conditional<int>` to produce -1/0/1. The quiet relational builtins
(`isgreater`, `isgreaterequal`, `isless`, `islessequal`) lower to the ordinary
comparison with `exceptions=ignore`. `__builtin_isunordered` and
`__builtin_islessgreater` combine two such tests with a bitwise `or<int>`
rather than `logical_or`, because `logical_or` short-circuits and both
operands of these builtins are always evaluated; `islessgreater` is
`lt | gt`, not `ne`, since it is false for NaN. `__builtin_complex(re, im)`
lowers to the same `aggregate<complex<T>>` a complex value gets anywhere
else. The generic checked arithmetic builtins lower to
`overflow_add/sub/mul<bool>(left, right, place)`,
which computes in the mathematical domain, stores the converted result through
the destination place, and returns whether conversion overflowed. Fixture:
`tests/fixtures/sema/ir_implicit_builtins.c`.

`__builtin_va_list` lowers to the opaque `Type::VaList` (printed `va_list`),
with per-target storage: 24/8 on x86_64 Linux, 32/8 on aarch64 Linux, pointer
sized elsewhere. It does not model the array-to-pointer decay the x86_64 and
aarch64 ABIs give a `va_list` parameter; it is passed as one scalar handle.
`__builtin_va_arg(ap, T)` lowers to `va_arg<T>(place)`: a type-directed read
that advances the list, so it counts as a side effect for hoisting like a call.
`T` may be a record (`tests/fixtures/sema/ir_va_arg.c`). `__builtin_va_start`,
`__builtin_va_end` and `__builtin_va_copy` lower to void `va_start(place)`,
`va_end(place)` and `va_copy(dest, src)` values, also effects
(`tests/fixtures/sema/ir_va_start_end_copy.c`). They are recognized by callee
name in sema, not declared; va_start's last-named-parameter argument is
resolved but dropped, as the IR does not need it.

The source-location builtins `__builtin_FILE`, `__builtin_FILE_NAME`,
`__builtin_FUNCTION`, `__builtin_LINE` and `__builtin_COLUMN` are Clang
keywords rather than `Builtins.td` records, so they are recognized by callee
name too. `LINE` and `COLUMN` fold to `int` constants and the rest to an
internal `.strN` global that decays like any string literal. Their position
comes from the callee token's own `Loc` resolved through the `Files` source
map, not from `Provenance`: the preprocessor stamps one `Provenance` on every
token of a logical line (`expand_line` covers the line with its first token's
location), so `Provenance.line` is exact for any token on that line but a
column taken from it would always be the line's first token. `Files` carries
the line table the preprocessor recorded, and `Files::position` turns a
`(FileId, offset)` into a zero-based line and column.

`__func__`, `__FUNCTION__` and `__PRETTY_FUNCTION__` lower to the same kind of
internal global, as an lvalue of type `char[N]`, so `sizeof`, indexing and
decay all behave like a string literal. `__func__` and `__FUNCTION__` are the
bare function name in every personality. `__PRETTY_FUNCTION__` differs by
personality and is one of the places a flavor changes meaning: GCC spells it
as the bare name in C, while Clang spells the whole declaration
(`unsigned long n(int)`), which sema renders with
`CTypes::declaration_spelling`.

`__builtin_choose_expr(cond, a, b)` also resolves during lowering and never
reaches the IR. It is a Clang keyword rather than a `Builtins.td` record, so
the generated registry does not contain it; sema recognizes it by callee name
like `__builtin_constant_p`. The condition must be an integer constant
expression; the selected operand is lowered in place of the call and supplies
the result type, and the unselected operand is not evaluated, so its side
effects are discarded. Both operands are still name-resolved and type-checked,
matching Clang and GCC. In a constant expression the call folds to the
selected operand's constant value.

`_Generic` resolves during lowering rather than reaching the IR: sema types the
controlling operand, matches it against the association types, and lowers only
the selected expression, so the selected branch can be constant or runtime and
the others produce no IR at all (`tests/fixtures/sema/ir_generic_selection.c`).
The controlling operand is unevaluated, so typing it rolls back any binding IDs
or string-literal globals its lowering would have created. A selection is also a
place when the selected expression is one, which is what makes `_Generic(...) = v`
and `&_Generic(...)` lower.

Selection uses C types after lvalue conversion, preserving distinctions such
as `char` versus `signed char`, `long` versus `long long` on LP64, and
`int` versus `long` on LLP64. These distinctions are carried by sema operands
and places and erased only when emitting IR. See [C type layer](c-type-layer.md).

`typeof` and `typeof_unqual` resolve in sema for both type names and expression
operands. Expression operands retain array and function types without decay;
typing them emits no runtime effects and rolls back temporary binding IDs and
string globals. `typeof_unqual` removes outer qualifiers, including atomic
qualification, while preserving pointee qualifiers. Resolved declarations keep
their `typeof` spelling, canonical C type, and applicable typedef chain metadata.
Function types can declare functions without a new parameter list, and existing
variable array types retain their captured extents. See
`tests/fixtures/sema/ir_typeof.c` and `ir_typeof_qualifiers.c`.

Declarator type derivation visits prefix pointers and arrays before wrapping
suffix function and array forms, so `int *f(void)` is `fn() -> ptr<i32>` and
`int *a[3]` is `array<ptr<i32>, 3>`; grouped declarators such as
`int (*f)(int)` build the suffix type on the base and let the grouped core
wrap it, matching C's precedence. Pointer↔integer casts lower as explicit
`ptr_to_int`/`int_to_ptr` conversions, and pointer relational comparisons
reuse `CompareOp::{lt, le, gt, ge}` on pointer operands
(`tests/fixtures/sema/ir_pointers.c`: buffer fill cursor, string walk,
out-parameter write, pointer round-trip). A function designator used as a
value decays to `function_decay<ptr<fn(..)>>(place)`, and since C makes `*f`
on a function designator that same designator, `(*fp)(x)` lowers identically
to `fp(x)` and `(*f)(x)` stays a direct call
(`tests/fixtures/sema/ir_indirect_calls.c`: parameter, local, struct field,
pointer table, conditional callee, variadic, higher-order argument, and a
callee whose subexpression has side effects).

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
Each resolved declaration carries `c`,
`c_canon` when different, and `typedef_chain` metadata; qualifiers are kept
as `c_const`, `c_volatile`, `c_restrict`, and `c_atomic` metadata. All of these
are rendered from the interned C type (`src/sema/ctype/`), never assembled
from strings: `c` is the written spelling, `c_canon` desugars typedefs and
`typeof` (keeping `_Atomic(T)`) and prints function types with adjusted
parameters (`int(int, const volatile int *, int (*)(int))`), and a C23 `()`
prints as `(void)`. Function types carry no qualifier metadata. See
[c-type-layer](c-type-layer.md). The shown
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
Enum operands use an explicit `enum_to_int` conversion to the resolved
underlying integer type before promotion, in switch discriminants and equally in
arithmetic, conditions, and comparisons; storing an integer into enum-typed
storage is an explicit `int_to_enum`. Enumerator types follow clang: with no
fixed type and every value in `int`, enumerators are `int`; otherwise they are
the underlying integer before C23 and the enum type itself from C23
(`StandardFeatures::enumerators_have_enum_type`), so a C23 enumerator reference
crosses through `enum_to_int` like any other enum value
(`tests/fixtures/sema/ir_enum_typing_c89.c`, `..._c23.c`). Known gap: clang
keeps a C23 enumerator at its own type when that equals the underlying type
(`enum P { P0 = 0x80000000 }` stays `unsigned int`); we use the enum type.
Both conversions follow typedef chains, so an alias to an enum behaves the same.
Enumerator references in case expressions resolve by binding and AST identity to
typed constants.

A block-scoped declaration may reference a file-scope `struct`, `union`, or
`enum` tag. Defining a tag inside a block is still rejected, because the
lowering type table keys tags by name without block scoping and two blocks
defining the same tag name would collapse onto one `TypeId` (slate-parser-rsm).

Named labels, gotos, and `label_addr<ptr<void>>` use resolved label binding IDs,
including distinct GNU local labels. Name resolution records each label
definition's AST identity separately from its declaration binding identity.
Computed goto retains its pointer-valued operand. The `ir_control_*.c` FileCheck
fixtures cover these forms, nesting, compact output, and invalid control
contexts.

Side-effect hoisting runs as a final sema pass over the resolved module rather
than during expression lowering. Assignment (`a = b`), compound assignment, and
`++`/`--` become `Statement::Write` on a place; the written value is the
lowered store result, so `x = y = 0` chains through a read of the first store
result without re-reading y. `update<i...>` expression nodes are gone: the old
value is snapshotted into a `[synthetic]` temporary and postfix forms read that
snapshot, while the write targets the one-evaluation stable place. Calls stay
expression values; call arguments evaluate left to right, so later arguments
read earlier state and no cross-argument freezing is needed. Comma sequences
lower left-to-right with the last operand as the value. Short-circuit `&&`/`||`
and `?:` with effectful operands become `Statement::If` writing a result
temporary per branch (void operands discard instead). Loop conditions and for
increments carry an `Evaluation` block of leading statements plus the condition
or increment value; the increment block discards its value (`yield void`).
Do-while condition evaluations run inside the loop after the body. Synthetic
temporaries print as `let %id: ty [synthetic] = ...` and reuse the `Evaluation`
statement list rather than entering name resolution. `ir_side_effects_hoisted.c`
and `ir_effects_sequencing.c` cover the acceptance cases (a[i++], chained
assignment, getc loop, f(i++), short-circuit member increment) plus ternary and
comma sequencing.

Other unsupported statements and attributes are diagnosed instead of omitted.
General attributed-statement lowering remains separate work; aggregate
initialization is described under "Objects, lifetime, and initialization".

Target selection separates CPU family (`TargetFamily`), OS (`TargetOs`), and
ABI environment (`TargetEnvironment`) from compiler flavor. Existing Linux
profiles use GNU, GNU EABI, or GNU EABI hard-float environments.
`x86_64-pc-windows-msvc` and `aarch64-pc-windows-msvc` are experimental Windows
MSVC-environment profiles, with LLP64 widths, binary64 long double, and unsigned
16-bit wchar_t. Their target triples do not implicitly select compiler flavor:
`--flavor=msvc` loads the checked-in MSVC 19.51.36256 snapshots; the default
Clang flavor loads the Clang 22.1.8 Windows snapshots. Neither loads Linux/glibc
shim defaults. GCC on these Windows profiles is rejected rather than falling
back to Linux macros.

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
the source fixes it: with a negative value `int`, else `long`; with none,
`unsigned int` (so `enum { A, B }` is `u32`), else `unsigned long`. Values
outside `int`, a fixed underlying type and a trailing comma are only
extension warnings in clang before C23, so no mode rejects them. Enum storage retains size and alignment separately so
alignment attributes need not change the underlying integer type.
`tools/check_record_layout.py` compares each target directory's generated
layout fixture with clang's target-specific record layout dump. The fixture
runner and expectation generator derive the target triple from that directory.
ARM targets let zero-width bit-fields raise record alignment, including in
packed records; x86 targets do not.

**Decided:** member access is a projection path, not a `field_ptr` value.
`PlaceKind::Field { base, index, bits }` names the field by its declaration
index in the record's field list, so `o->inner.x` is
`field0(field0(deref(..)))` and a union member is the same projection onto an
overlapping `Type::Defined` — a union has no tag and no active-member node.
Anonymous struct and union members keep their own field index, and a promoted
member name expands during lowering into the chain through them, so nothing
downstream repeats anonymous member lookup.

A bit-field's place carries the resolved slice from the record layout:
`bitfield1<unit=0, bytes=0..2, bits=3..8>(..)` is field 1, held in storage
unit 0 (record bytes 0 through 2), occupying bits 3 through 8 counted from
the least significant bit of that decoded unit. The place type stays the
declared field type, which is what gives the read its signedness; the width
is what the emitter masks and extends to. A bit-field rvalue promotes by its
declared width rather than its storage type, so `unsigned low : 3` reads as
`reinterpret<i32, reason=promotion>(read<u32>(bitfield0(..)))` and the
compound-assignment old value promotes the same way before the operator runs.
Zero-width bit-fields have no storage and are not addressable members. A
bit-field is not an object with its own address or layout, so `&f->low`,
`sizeof(f->low)`, `_Alignof(f->low)`, and `offsetof` on one are rejected as
`ResolveError::Invalid` — a distinct variant from `Unsupported`, because these
are invalid C rather than unimplemented lowering. Named zero-width bit-fields
are already rejected at layout.

The width-based promotion is an rvalue rule, so unevaluated contexts do not
see it: `_Generic`'s controlling operand undergoes lvalue conversion but not
integer promotion, and therefore selects on the declared bit-field type
(`_Generic(f->low, unsigned: .., int: ..)` picks `unsigned`).

Flexible-array semantics remain future work (layout and initializers exist; the extent lives on the initializer value),
and callable types/ABI contracts, in their respective lowering tasks. `tests/fixtures/sema/ir_records.c` covers nested
records, unions, anonymous members, and bit-field reads, writes, compound
assignment, and increment. The name-resolution dump remains a separate
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
The one exception is `sizeof` of a variable-length array type: it is a
runtime computation over the captured extents, never a folded constant.

### Numeric operations

`sema::numeric::Context::resolve` lowers integer and binary floating-point
literals, parentheses, same-concrete-type `+`, `-`, `*`, `/`, `%`, `&`, `|`,
`^`, integer `<<`/`>>`, unary `-`, integer `~`, unary `+`, same-type
comparisons, `!`, `&&`, `||`, and `true`/`false` directly to
`ir::Value`. Integer literal selection uses the standard-dependent C
candidate order and target integer widths: `StandardFeatures::long_long_type`
is `Standard` from C99 on and `Extension` in C89, so a C89 unsuffixed or
`l`-suffixed decimal literal may become `unsigned long` (C89 6.1.3.2) before
falling back to the `long long` extension tail, while C99 (6.4.4.1) reaches
`long long` first. In every mode a decimal literal that fits no signed type
falls back to the unsigned form of the widest rank, matching clang's
`-Wimplicitly-unsigned-literal`. `select_integer_candidate` is the single
selection point, shared with validation, and the candidate it picks is what
[`diagnostic-severity.md`](diagnostic-severity.md) derives the literal
warnings from. C89 and C99 only diverge where `long` is
narrower than `long long`, i.e. ILP32 and LLP64 targets, not LP64. Floating constants retain their exact value bits.
The dump prints f32/f64 numerically using round-trippable decimal formatting
(including signed zero); NaNs retain hexadecimal bits to preserve payloads.
The f16, f80, and f128 printer uses `rustc_apfloat` directly, without an f64
conversion. NaNs retain hexadecimal bits in every format.
These lower to one `ValueKind::Arith` node keyed by `ArithOp`
(`add`/`sub`/`mul`/`div`/`rem`/`and`/`or`/`xor`/`shl`/`shr`), all carrying `ArithSema` metadata. They are
not folded or reassociated. Signed overflow defaults to
`ub`, unsigned overflow to `wrap`; floating arithmetic defaults to
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
`overflow=ub` (unsigned `overflow=wrap`). Both shifts explicitly carry
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
and standard integers of rank at most `int` promote to `int` when representable,
otherwise `unsigned int`. Usual arithmetic conversions compare C integer ranks,
including when two ranks have the same width.
Comparisons, `!`, `&&`, and `||` produce `ir::Type::Bool`, a type separate
from `NumericType`, while their sema C type remains `int` for `_Generic`,
`typeof`, and `sizeof`. Comparisons lower to `ValueKind::Compare` keyed by
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

The initial numeric type stores integer width/signedness/bit-precision or
one of eight floating formats: binary f16, f32, f64, f80, f128 and decimal
d32, d64, d128 (`_Decimal32/64/128`, size and alignment 4/8/16 on every
target; availability is not target-gated). Decimal literals (`1.5DF`) keep
their digit spelling as `const<d32>(1.5)` and are never converted to binary.
Decimal arithmetic is never folded. Mixing a decimal and a binary floating
operand in arithmetic or comparison is rejected (C23 6.3.1.8); integers
convert to the decimal operand's format. Explicit casts and assignments between
the families use `float_convert`, since the value is neither widened nor
narrowed. Bit-precise
integers (`_BitInt(N)`) are a distinct type from the standard integer of
the same width and print with a `b` suffix (`i128b`, `u32b`): on x86-64
`__int128` is `i128` with size 16 align 16 while `_BitInt(128)` is `i128b`
with size 16 align 8, and arm32 rejects `__int128` while still accepting
`_BitInt(128)`. Bit-precise layouts come from `ScalarLayouts::bit_precise`,
which rounds the width up to the widest standard integer's alignment. These denote value formats, not
storage sizes.

Bit-precise integers follow their own conversion rules rather than the
standard-integer ones. These rules are **not** gated on the standard mode.
`_BitInt` is a C23 feature, but clang and gcc both accept it as an extension
in every earlier mode (diagnosed only under `-pedantic` /
`-Wbit-int-extension`) and apply identical conversion rules there; result
types were checked to be byte-identical across c89/c99/c11/c17/c23 and their
gnu variants in both compilers. Lowering therefore applies them
unconditionally, and `TranslationUnit.standard` does not reach this path.

They are exempt from the integer promotions (C23 6.3.1.1), so `_BitInt(8) + _BitInt(8)` stays `i8b`, and `~a`, `-a`, `+a`,
and a shift's left operand keep their declared width instead of widening to
`i32`. The exemption covers the default argument promotions too: a
`_BitInt(8)` passed to a variadic function is passed as `i8b`, matching
clang's `i8 signext`.

Rank at equal width puts the bit-precise type _below_ the standard one, so
the usual arithmetic conversions resolve `_BitInt(32) + int` to `int` and
`unsigned _BitInt(32) + int` to `unsigned int`, while a wider bit-precise
type still wins (`_BitInt(40) + int` is `i40b`). Because `i32b` and `i32`
share a value representation, the conversion between them changes only the
type; `convert` emits an exact `reinterpret` for it rather than nothing, so
the operand type cannot silently disagree with the operation's type.
`tests/fixtures/sema/ir_bitint_conversions.c` pins every case; its result
types were verified against clang 22.1.8 and gcc 16.2.1, which agree on all
of them. MSVC 19.51 has no `_BitInt` at all, so there is no third oracle.
slate-parser accepts `_BitInt` in every mode without an extension warning;
matching clang's `-Wbit-int-extension` is separate work (slate-parser-jgf). Sema resolves `long double` through `TargetInfo.long_double`:
the current x86-64 Linux baseline uses f80; `-mlong-double-64/80/128`
selects the corresponding format. Literal digits are parsed directly into
that format, never rounded through an intermediate f80 or f64 value.
Builtin scalar casts and mixed-type arithmetic insert conversion nodes with
`reason=promotion|usual_arith|explicit`. Integer width changes precede sign
changes. Float narrowing and integer-to-float conversions carry rounding
and exception settings; float-to-int truncates toward zero and records
`out_of_range=ub` plus exception settings. Widening floats is exact.
Target-dependent `f64x` suffixes return explicit unsupported errors.
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
add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), const<f64>(2.0))
sub<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
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

## Module shape

A module is its target, type definitions, globals and functions, plus a
`NodeId`-keyed metadata side table ([grammar](ir-grammar.md#module)).

**Open:** "type parameters" on functions — needs a concrete C use case
(`_Generic`, `<tgmath.h>`, type-generic macros) before it gets a slot.

### Function bodies

**Decided:** a body is a list of statements containing typed expression
trees.

- Statements are those in [the grammar](ir-grammar.md#statements).
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

### Inline asm

**Decided:** a GNU `asm` statement lowers to `Statement::Asm`, which keeps
everything the parser resolved rather than a token dump: the raw template,
the `volatile`/`inline`/`goto` qualifiers, the resolved template pieces,
output operands as places, input operands as values, clobbers, and goto
labels as label `BindingId`s. Instructions inside the template are still not
parsed — that is deliberately the assembler's job, and Slate treats the
template as opaque text with holes.

- Operands are numbered outputs first, then inputs, the same numbering the
  template's `%N` uses. A resolved `Label(n)` piece indexes the statement's
  label list, not the operand number the source wrote, so `%l1` and
  `%l[done]` both print as `%l0` when `done` is the first label.
- An output is a `Place` because an asm output must be an lvalue; sema
  rejects non-lvalue outputs before lowering. A `+` (read-write) output
  stays a single output carrying the `ReadWrite` modifier; it is not split
  into a tied output/input pair.
- Constraints keep the parsed alternative list (modifiers, hard register,
  matching operand number, or letters). The printer reconstructs the GNU
  spelling from it, so the printed text round-trips the parse rather than
  echoing the source.
- Registers carry the source spelling and, when the target register table
  recognized them, the canonical name. Width is dropped: a clobber clobbers
  the whole register, and an operand's width is its IR type.
- Side effects in operand expressions hoist ahead of the statement like any
  other operand, so the asm itself never contains an embedded effect.
  Fixture: `sema/ir_asm.c`.

File-scope `asm` and declarator asm labels are separate work
(`slate-parser-dyd.3`, `slate-parser-dyd.4`); this covers statement asm only.

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
trailing expression, the `evaluation` form in the grammar (valid Rust:
`while { ch = getc(f); ch != EOF } {}`).

A GNU statement expression `({ stmts; e; })` lowers to
`ValueKind::StatementExpression(Evaluation)`: the leading statements plus
the value of a trailing expression statement (`void` when the block does not
end in one). It always counts as effectful, and hoisting always removes it,
so it never reaches printed IR. When the value is used, it becomes a
synthetic `let %t: T;` followed by a `{ ... }` block that ends by writing
the value into `%t`, and the expression reads `%t`. When the value is
discarded or `void`, the block alone remains. The block keeps the
statement expression's scope, and `return`/`break`/`goto` inside it act on
the enclosing function and loops as in GCC. Under `&&`/`||`/`?:` it goes
through the same `if` lowering as any other effect, so it stays
conditional. Fixture: `sema/ir_statement_expressions.c`.

Hard cases hoisting must respect (evaluation order and sequencing):

- `f(i++)`: C increments before the call executes. Lowering always
  snapshots the old value in a synthetic temporary, writes `i`, and passes
  the temporary; removing temporaries that turn out unnecessary is left to
  the analysis pass.
- `&&`, `||`, `?:`, comma with side effects in a later operand: hoisting
  must turn into `if`, not unconditional statements.
- Loop conditions and `for` increments with side effects stay attached to
  their `while`/`for` as a statement block with a trailing expression, so
  they re-run per iteration without changing the loop's form.
- Assignment used as a value (`x = y = 0`) → sequential statements, with the
  value read back from the assigned target.

## Types

**Decided:** the shown type is the concrete, target-resolved type. The
original C type is metadata.

| C                                           | Shown                            | Metadata                              |
| ------------------------------------------- | -------------------------------- | ------------------------------------- |
| `int`                                       | `i32`                            | `c=int`                               |
| `long` (LP64 / LLP64)                       | `i64` / `i32`                    | `c=long`                              |
| `char` / `signed char` / `unsigned char`    | `i8`/`u8` per target, `i8`, `u8` | `c=char` etc.                         |
| `_Bool`                                     | `bool`                           | `c=_Bool`                             |
| `size_t`                                    | `u64`                            | `c=size_t`, `c_canon=unsigned long`   |
| `float` / `double`                          | `f32` / `f64`                    | `c=float` / `c=double`                |
| `long double` (x86)                         | `f80`                            | `c=long double`                       |
| `__float128`                                | `f128`                           |                                       |
| `_Decimal32` / `_Decimal64` / `_Decimal128` | `d32` / `d64` / `d128`           |                                       |
| `_BitInt(128)`                              | `i128b`                          | `c=_BitInt(128)`                      |
| `const char *`                              | `ptr<const i8>`                  | `c=const char *`                      |
| `enum E`                                    | `@typeN` (underlying `u32`)      | underlying type computed per compiler |
| `struct S`                                  | `@typeN`                         | layout in module                      |

The whole typedef chain is kept in metadata (`uint32_t` → `__uint32_t` →
`unsigned int`) since it's the strongest idiomization signal
(`size_t` → `usize`, `c_int` in FFI signatures).

### Complex, imaginary, vector, and fixed-point types

**Decided (shape only):** these are four distinct value-type families, not
`NumericType` variants. A complex type contains a resolved real component
type; an imaginary type contains a resolved component type but is not a
complex value with a known-zero real part. A vector contains a resolved scalar
element type and a lane count; source byte sizing is converted to lanes after
the element's target storage size is known. A fixed-point type contains its
kind (`_Fract` or `_Accum`), rank, signedness, saturation mode, and resolved
scale/storage information. Source spelling and typedefs stay in type metadata.
These are intended as structural `Type` variants, like pointer and array,
because arithmetic and representation must remain visible without consulting
source metadata. This is the type-shape decision of `slate-parser-lh7.2.17`,
not an implemented IR API.

**Implemented for complex:** `Type::Complex(NumericType)` prints as
`complex<f64>` and retains a concrete component type. Target storage is two
adjacent components with the component's alignment. The module lowerer handles
floating and GNU integer-complex declarations, scalar/complex and
complex/complex conversions, `+ - * /`, `== !=`, truth tests, unary negation,
and `__real__`/`__imag__` reads and writes. Mixed scalar/complex arithmetic
retains the scalar operand, which matters for floating multiplication and
division. `ArithSema::ComplexFloating` carries rounding and exception policy;
`ComplexInteger` carries overflow and division-by-zero policy. The component
of a complex conversion is converted on each side, with the conversion
contract recorded on the operation. `tests/fixtures/sema/ir_complex.c` pins
these forms, the common complex sizes, and GNU integer-complex spelling.
Function declarations and calls now carry an `AbiSignature` resolved from the
target's ISA and ABI environment. It records the calling convention and the
passing shape of each argument and the result without replacing source-level
types with hidden pointers or machine registers. Scalar passes remain implicit
in the default dump; nontrivial signatures print `abi=...`. `coerce<...>`
records direct value pieces, `direct` a value passed in registers as its own
type, `byval` a copied memory argument, `byref` an indirect argument, and
`sret` an indirect result. `native_c` leaves an
unclassified record to the target's ordinary C ABI for `repr(C)` emission;
it is explicit so Rust lowering does not mistake a guessed coercion for a
verified one. Call nodes keep their own ABI signature because an indirect
callee or a variadic call can differ from the enclosing function.

The initial matrix covers SysV x86-64, Windows x86-64 MSVC, i386 cdecl,
AArch64 Linux and Windows, and ARM32 soft/hard-float for complex values,
128-bit integers where Clang supports them, and flat records. For example,
`complex<f64>` is two direct floating pieces on SysV x86-64, a copied memory
argument plus indirect result on i386, and a reference argument plus indirect
result on Windows x86-64. ARM32 picks hard-float from the resolved float ABI
(`TargetIsa::Arm`, default from the gnueabihf/gnueabi triple, overridden by
`-mfloat-abi`), not from the triple environment directly. ARM hard-float
variadic signatures use base AAPCS;
Windows AArch64 variadic aggregate arguments use integer pieces. The
`ir_call_abi.c` and target-specific `abi_target.c` FileCheck fixtures pin
these cases against Clang IR signatures. More elaborate records use
`native_c` until their ABI coercion is modeled explicitly.

**Implemented for imaginary (C23 Annex G):** `Type::Imaginary(FloatType)`
prints as `imaginary<f64>`. Only binary real floating components are
accepted; the parser already rejects `int _Imaginary` and decimal
components. Storage, alignment, and ABI passing are those of the component
(G.2), so signatures pass it as `scalar`. Neither clang nor gcc accepts
`_Imaginary`, so no oracle exists for these contracts; they follow the
standard text.

Conversions (G.4) are explicit kinds: `real_to_imaginary` and
`imaginary_to_real` produce positive zero and discard the operand's value;
`imaginary_to_complex` sets a positive-zero real part; `complex_to_imaginary`
keeps the imaginary part with the same component; `imaginary_convert`
changes the component with floating narrowing rules. Truth tests compare
against `const<imaginary<fN>>(0.0)`.

Binary operators (G.5) apply only the common real type: each operand keeps
its own domain (real, imaginary, complex), so the operand types plus the
result type select the Annex G formula. Result families:

| Operator   | real/imaginary | imaginary/imaginary | any complex operand |
| ---------- | -------------- | ------------------- | ------------------- |
| `*` `/`    | imaginary      | real                | complex             |
| `+` `-`    | complex        | imaginary           | complex             |
| `==` `!=`  | bool           | bool                | bool                |

Operations with a complex result carry `complex=true`. Relational
operators, `%`, bitwise operators, shifts, and `~` reject imaginary
operands; unary `-` is exact negation. The conditional operator and other
usual-arithmetic sites use imaginary for two imaginary operands and complex
for mixed domains.

Imaginary literals (`2.0i`, `3.0fj`, `3i`, `3ui`) are a GNU extension that
the standard does not define; GNU types them as complex, so they lower to
`aggregate<complex<T>>(index0 = 0, index1 = value)`, the same shape as a
braced complex initializer. An integer imaginary literal's component is the
type the literal would have without `i`/`j` (`5000000000i` is
`complex<i64>`), matching clang. Imaginary literals are not integer
constant expressions, so `#if 3i` and array bounds reject them.
`tests/fixtures/sema/ir_imaginary.c` and `ir_imaginary_invalid.c` pin these
forms.

**Implemented for vectors:** `Type::Vector { element, lanes }` prints as
`vector<i32, 4>`. Both source forms collapse into it: `vector_size(N)`
divides `N` by the element's target storage size, rejecting a size that is
not a positive multiple of it, and `ext_vector_type(N)` is the lane count
directly. Only integer (including `_BitInt`) and binary floating elements are
accepted; `_Bool` and decimal elements are rejected, matching clang.

Target storage is `next_power_of_two(lanes * element_size)` for both size and
alignment, so a 3-lane `int` vector is 16 bytes aligned to 16, and a 3-lane
`short` vector is 8 aligned to 8. That rounding is clang's, and it is why
lanes cannot be recovered from `sizeof` alone.

Vector arguments and results are classified per convention. `AbiPass::Direct`
is a vector passed in registers as its own type, distinct from `scalar` and
from `native_c`. The classification follows the effective x86 ISA
(`TargetInfo.x86_isa`), the same value that generates the ISA feature
predefines, so the preprocessor and the ABI always agree on what the target can
do. On `sysv64` the "> 16 bytes" column below is really "wider than the widest
vector register": 16 bytes at baseline, 32 with AVX (`-mavx`,
`-march=x86-64-v3`), 64 with AVX-512F. A vector no wider than that is `direct`;
wider ones are `byval` (clang's `-Wpsabi` case). The other conventions do not
depend on the ISA.

| Convention           | < 8 bytes     | 8 bytes                      | 16 bytes | > 16 bytes                      |
| -------------------- | ------------- | ---------------------------- | -------- | ------------------------------- |
| `sysv64`             | `coerce<iN>`  | `coerce<f64>`                | `direct` | arg `byval`, result `direct`    |
| `win64`              | `direct`      | `direct`                     | `direct` | `direct`                        |
| `x86_cdecl`          | `direct`      | arg `coerce<i64>` if MMX     | `direct` | `direct`                        |
| `aapcs64`/`win_arm64`| arg `coerce<i32>` | `direct`                 | `direct` | arg `byref`, result `sret` (align 16) |
| `aapcs32`(`_hard_float`) | arg `coerce<i32>` | `direct`             | `direct` | arg `direct`, result `sret` (align 8) |

Results take the `direct` form wherever the table names a coercion only for
arguments. Two corners are inherited from gcc and pinned by fixtures: a
one-lane `double` vector is passed in memory on `sysv64` (`vector<f64, 1>` is
`byval<align=8>`, though its result is `direct`), and an eight-byte vector of
integer lanes narrower than 64 bits is an MMX type that `x86_cdecl` passes as
`i64`, while `vector<i64, 1>` and float-lane vectors of that size pass
directly. `tests/fixtures/sema/ir_vector_abi.c` pins baseline `sysv64`
(`ir_vector_abi_avx.c` and `ir_vector_abi_avx512f.c` pin the wider ISAs) including the
indirect and variadic call sites, and the `abi_target.c` fixture in each
target directory pins the rest; all of them were diffed against clang's IR
signatures for that target.

Arithmetic is per-lane. An operation whose result type is a vector prints
`elementwise=true` and otherwise carries the element's scalar contract, with
one difference: integer lanes wrap on overflow regardless of signedness,
because clang emits no `nsw` for vector arithmetic. `%` and the bitwise and
shift operators require integer elements. Comparisons yield a lane mask, not
a `bool`: a signed integer vector whose element has the same storage size as
the operand element (`vector<f64, 2>` compares to `vector<i64, 2>`, `f80`
lanes to `i128`), printed as `result=` on the comparison. A vector is not a
scalar condition, so `if (v)`, `!v`, `&&`, and `||` are rejected.

Mixed operands follow clang's lax vector conversions: the **left** operand
fixes the result type, a vector operand of the same total size is
reinterpreted with `vector_bit_cast`, and a scalar operand is converted to
the element type and then `vector_splat`. The same lax rule applies to
assignment, argument passing, return, and casts; a conversion between vectors
of different sizes is an error. Clang additionally rejects a splat whose
scalar type would truncate (`v4si + double`); we accept it and record the
element conversion in the IR instead, since sema is a debugging aid for input
that already compiles. Braced initializers fill lanes like array elements,
with omitted lanes zero-filled. `v[i]` on a vector lvalue is the place
`lane(place, index)`, so a write or compound assignment stays a write to that
lane; on a vector rvalue, where there is no storage, it is the value
`lane<T>(vector, index)`. A lane is not addressable, and the index is an
ordinary runtime value, not a constant.

`ext_vector` component syntax selects lanes by name: `x/y/z/w`, `r/g/b/a`,
`sN` in hex, and `lo`/`hi`/`even`/`odd`, where the half is taken over the lane
count rounded up to a power of two, so a 3-lane `.hi` names one lane that does
not exist. One component is a `lane(..)` place, several distinct components in
range are a `swizzle<lanes=[..]>(..)` place, and a repeated or out-of-range
selection is not assignable and becomes a one-operand `shuffle<..>` value.
Clang accepts these only on `ext_vector_type`; `Type::Vector` does not record
which spelling declared it, so we accept them on `vector_size` vectors too,
consistent with treating sema as a debugging aid for input that already
compiles.

`__builtin_shufflevector` is the same `shuffle<..>` value with two operands,
its mask indexing their concatenation, and `-1` printed as an `undef` lane;
its two-argument form takes a runtime integer mask vector and prints
`mask=dynamic(..)`. `__builtin_convertvector` converts per lane and prints as
the ordinary scalar conversion of the element types over vector operand and
result types (`int_to_float<vector<f32, 4>>(..)`), so it carries the same
exactness and exception policy as the scalar conversion it mirrors.
`tests/fixtures/sema/ir_vector.c` and `ir_vector_invalid.c` pin these forms.

**Out of scope for current IR lowering:** fixed-point types remain
unrepresented, tracked by `slate-parser-lh7.2.17.4`:

| Family      | Reason lowering remains out of scope                                                                                                                                              |
| ----------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fixed-point | The AST currently loses signedness, and target-specific widths, scale, overflow, saturation, and rounding are not modeled.                                                        |

`ArithSema` needs a distinct saturating fixed-point case rather than treating
saturation as integer overflow. No numeric operation should silently accept a
fixed-point type until its contract and conversions are pinned by FileCheck
fixtures. Declaration-only support also requires target storage and ABI
rules, so adding a bare `Type` variant alone would not make headers lower.

### Records

Layout is computed during lowering: field offsets, padding, alignment,
bit-field storage units. Source field order and names kept; anonymous
members print as `<anonymous>`.

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

**Implemented for static locals (`e0s`):** a block-scope `static` object is
lowered to a module `Global` with `storage=static`, `linkage=internal` and its
own `BindingId`; no statement is emitted at the declaration, and uses read the
global's binding. Its initializer is lowered once like any global
initializer, and an absent one means zero initialization. Two locals (or a
local and a file-scope object) sharing a source name stay distinct globals.

**Implemented for globals and linkage (`lh7.2.9`):** name resolution gives
every declaration node its binding (`NameResolution.declarations`), and every
declaration with linkage (file-scope objects and functions, block-scope
`extern` objects, block-scope function prototypes) shares one binding per
name, even when the block-scope declaration comes first or the name is
shadowed by a local. Lowering merges redeclarations by `BindingId`, not by
name. That gives one `Global` per object: array types complete, the
initializer comes from whichever declaration has one, and `definition` is set
by any non-`extern` declaration, initializer, or `alias`. It also gives one
`Function` per function, taking the body and parameters from the definition,
or else the first prototype. Linkage stays internal once any declaration is
`static`.

Conflicting redeclarations follow one rule, checked against clang 22, gcc, and
MSVC (slate-parser-wxs.3). Where all three reject, lowering rejects. Where one
only warns (always MSVC), lowering accepts with a `-Wconflicting-types`
warning:

| Conflict                                                            | Result  |
| ------------------------------------------------------------------- | ------- |
| return/object type of another kind, size, or pointer depth          | error   |
| return/object type: incompatible C types of identical layout, also through pointers | warning (MSVC C4142) |
| prototyped parameter lists differing in types, count, or `...`     | warning (MSVC C4028/C4030/C4031/C4052) |
| unprototyped vs prototyped, or only top-level parameter qualifiers  | accepted silently |
| a struct/union/enum redefined in the same scope                     | error   |
| the same, in C23, when fields (name, type, constness, access, bit width) or enumerators (name, value) match | accepted, reuses the first definition |

Merging is decided on C types, in `TypeResolver::merge_redeclaration`
(`src/sema/types.rs`). Compatible declarations merge into their composite
type, which is what completes `int a[]; int a[5];`. Otherwise the layouts
decide: `types::same_layout` compares the lowered types ignoring integer
signedness, and an incompatible pair that still shares a layout is the
`conflicting-types` warning rather than an error. This is MSVC's own rule —
C4142 "benign redefinition" fires exactly when the layouts coincide and C2371
"different basic types" when they do not — so on Windows (LLP64) `int x;
long x;` and `int f(int); long f(int);` warn, while on LP64 they are errors
and `int x; long long x;` is an error everywhere (slate-parser-y47). Parameter
lists are the exception: they may always differ, because MSVC only warns.
Fixture: `sema/x86_64-pc-windows-msvc/ir_redeclaration_layout.c`. Tag
redefinitions are caught in `TypeResolver::define_tag`: a second
definition of a tag that is already complete in the same scope; C23 mode is
`StandardFeatures::compatible_tag_redefinitions`. Fixtures:
`sema/ir_redeclaration_conflicts.c`, `sema/ir_redeclaration_compatible.c`. `_Thread_local`, `__thread`, and `__declspec(thread)` give
`storage=thread` at file scope and on block-scope `static`/`extern`; on an
automatic local they are invalid. Symbol attributes are a semantic
`SymbolAttributes` on both `Global` and `Function`, printed after the linkage
and merged across redeclarations (first value wins, flags OR): `asm_name`
(from `asm("sym")` labels), `visibility`, `weak`, `alias`, `section`, `used`,
`retain`, `tls_model`, `dllimport`/`dllexport`, `weakref`, `selectany`.
Attributes on typedefs or parameters remain unsupported; on automatic locals
only alignment attributes are accepted. Fixture: `sema/ir_globals_linkage.c`.

**Object attributes (`lh7.2.22`):** `weakref("t")` is not `weak` plus
`alias`: `alias` defines a symbol, while a weakref emits none and its uses
resolve to an `extern_weak` reference to `t`. So a weakref global prints as
`extern` with `[weakref="t"]`, and must have internal linkage; it also applies
to functions. `selectany` (clang: `weak_odr` plus a COMDAT) requires external
linkage. Alignment and common-ness are object properties on `Global`, not
symbol attributes, resolved after all redeclarations merge:

- `[align=N]` comes from `aligned`, `_Alignas`, and `__declspec(align)`, taking
  the largest request across declarations. It is a property of the *variable*,
  so it prints on a `let` as well as a `global`, and it prints only when it
  differs from the type's natural alignment. Clang honors the request as
  written, even below natural (`aligned(1)` on an `int` gives `align=1`); gcc
  and MSVC take the max with natural. Clang's LLVM IR is the oracle here: it
  carries `align 1` for both a global and an `alloca`, while the emitted
  assembly's `.p2align 2` is a later section-level decision.
- `[common]` marks an external tentative definition: a definition with no
  initializer on any declaration, not thread-local, and not `alias`, `section`,
  `weak`, or `selectany`. `common` on any declaration wins over `nocommon`
  (clang); otherwise `nocommon` opts out; otherwise `-fcommon`/`-fno-common`
  decides (default off, rejected under MSVC). On an MSVC target, an alignment
  request also opts out.

**Alignment of a declared object (`lh7.2.31`, `lh7.2.32`):** `__alignof__` of
an object reports the object's alignment, not its type's, and that is a
*different* rule from the one above. clang and gcc both report the declared
request even when it is below natural; MSVC raises it to natural. gcc therefore
lays a `aligned(1) int` out at 4 but reports 1, which is why
`TypeResolver::declared_alignment` exists alongside `effective_alignment`
rather than reusing it. Both rules read the same request, held once per
`BindingId` on the sema entity — see the
[declared-entity model](entity-model.md).

Fixtures: `sema/ir_object_attributes.c`, `sema/ir_object_attributes_fcommon.c`,
`sema/ir_object_alignment_gcc.c`, `sema/ir_object_alignment_sites.c`,
`sema/ir_entity_sites.c`, `sema/x86_64-pc-windows-msvc/ir_selectany.c`.

**Function specifiers (`lh7.2.14`):** `FunctionSemantics` separates inlining
preference (`hint`, `always`, `never`), definition emission, and `noreturn`.
`InlineSemantics::SupressDef` makes an external `extern inline` body inline-only;
`ProvideDef` makes it supply an external definition. The standard selects the
default; `-fgnu89-inline` selects `SupressDef`, and `-fno-gnu89-inline` selects
`ProvideDef`, with the last flag winning. The preprocessor's inline-mode macros
follow the same setting. A function's `gnu_inline` attribute selects `SupressDef`.
File-scope redeclarations before or after a definition participate in resolving
`ProvideDef`: any declaration without `inline`, or with `extern`, requires an
external definition. Static inline bodies retain internal linkage and a definition.
These rules follow [GCC's inline documentation](https://gcc.gnu.org/onlinedocs/gcc/Inline.html).

`always_inline` and `noinline` resolve into the preference enum independently of
emission; contradictory preferences are diagnosed. `_Noreturn`, `[[noreturn]]`,
and the GNU attribute merge across declarations into the same flag. Reaching the
end of such a body is `fallthrough=ub`. Function attributes also survive as
`c_attributes` metadata, including attributes not otherwise interpreted by sema;
retention does not implement attribute-specific ABI or optimization behavior.

`Variable.constexpr` distinguishes C23 constexpr objects from ordinary const
objects. Their types are implicitly const-qualified, file-scope objects have
internal linkage, and an initializer is required. The initializer remains
structured IR. Fixtures: `sema/ir_inline*.c` and `sema/ir_function_specifiers.c`.

**Implemented for variable-length arrays (`er8`):** a block-scope declarator
whose array bound is not a constant emits a synthetic size_t
`Statement::Temporary` capturing the bound at the declaration, and the
declared type is `Type::VariableArray { element, extent }` (printed
`vla<T, %extent>`) referencing that binding. Each non-constant dimension gets
its own extent, evaluated left to right; `int (*p)[m]` is
`ptr<vla<i32, %m>>`. The object decays like an array
(`array_decay<..., length=None>`). `sizeof` of a VLA-typed operand is a
runtime `mul` of the captured extent and the element size, never a
re-evaluation of the bound.

**Implemented for VLA parameters and `[*]` (`lh7.2.11`):** a function
definition evaluates each non-constant parameter bound once at entry, as a
synthetic temporary at the head of the body, in parameter order (C11
6.9.1p10). The outermost bound is captured too, because it is evaluated even
though the parameter adjusts it away (`int a[n]` becomes `ptr<i32>`), while
`int a[n][m]` is `ptr<vla<i32, %m>>`. Prototype scope has no captured
extents, so a `[*]` bound, or a non-constant bound in a function type or
non-defining prototype, is `vla<T, *>`: `int f(int n, int a[*][n]);` gives
`ptr<vla<i32, *>>`, and so does a definition's function type. Its parameter
bindings still carry the captured extents. Pointer arithmetic and
differences accept a VLA element, with the stride being its runtime size
(`element=vla<i32, %m>`). VLA types are compatible with each other and with
fixed arrays whose elements are compatible (C11 6.7.6.2p6), whatever their
extents, so assigning between them is a `pointer_cast`. Fixture:
`sema/variable_length_array_parameters.c`.

**Implemented for VLA type names in expressions (`lh7.2.24`):** a type name
inside a function body (`sizeof(int[n])`, `(int (*)[n])p`) captures its
non-constant bounds where the expression is evaluated, not at a declaration.
Lowering wraps the expression in `ValueKind::Capture { id, extent, value }`
(printed `capture<%id>(extent, value)`), one per bound, outermost first; the
side-effect hoisting pass turns each into a synthetic `Statement::Temporary`
at that evaluation point, so a capture under `?:` or `&&` runs only in its
arm and never survives into printed function bodies. `sizeof` captures only
when the named type is itself a VLA (C11 6.5.3.4p2): `sizeof(int (*)[n])` is
a folded constant and does not evaluate `n`. `_Alignof` of a VLA type or
object is its innermost fixed element's alignment and evaluates nothing.
Casts capture every bound in the type, before the operand, as clang does.
Fixture: `sema/variable_length_array_type_names.c`.

**Implemented for block-scope typedefs and VLA `{}` (`lh7.2.25`):** block-scope
`typedef` is accepted (block-scope tag definitions are still unsupported).
Lowering pushes a `TypeResolver` scope for each function body, compound
statement, selection/iteration statement and its substatements (C11 6.8.4p3,
6.8.5p5), and statement expression, so an inner typedef shadows and then
releases an outer one. `typedef int T[n];` captures `n` once at the typedef,
like an object declarator, and every later `T` (objects, `sizeof(T)`) shares
that extent even if `n` changes. Aliases stay in the module's flat type table
(`type @typeN T = vla<i32, %e>;`), so a block-scope alias there can name a
function-local extent binding. The only valid VLA initializer, C23 `{}`,
lowers to `aggregate<vla<T, %e>, zero_fill=true>()`; any other VLA initializer
is `Invalid`. Fixture: `sema/variable_length_array_typedefs.c`.

**Implemented (`lh7.2.8`):** braced and string initializers lower to
`ValueKind::Aggregate { members, zero_fill }` (`sema/initializer.rs`),
printed `aggregate<T, zero_fill=..>(field0 = v, index2 = v, index3..=5 = v)`.

- `members` are sorted by target, and each `AggregateTarget` is a resolved
  `Field(index)`, `Index(i)`, or `Range { start, end }` (inclusive, GNU
  `[a ... b]`). Designators, brace elision, anonymous-member paths, and
  `.a.x = 1, .a.y = 2` merging are all resolved away; nested subobjects are
  nested `Aggregate` values.
- `zero_fill` is true when any initializable member (named fields and
  anonymous records; unnamed bit-fields are skipped) or array element was
  omitted. Unions carry only the selected member and never `zero_fill`.
- A later initializer for the same target replaces the earlier one. A
  partially overlapping range is `Unsupported`.
- `T a[] = ...` completes the array length from the last initialized
  element. `char`-like arrays from string literals (also `{"..."}`) stay
  `CodeUnits` on the declared array type, zero-padded or truncated to length.
- Compound literals lower to `PlaceKind::CompoundLiteral { object, storage,
initializer }`, printed `compound_literal %id [storage=..] = <initializer>`.
  Each literal gets a fresh `BindingId` (its own object identity) and its type
  is the initializer value's type, so `(int[]){1,2}` is `array<i32, 2>`.
  Storage is `Static` outside a function body and `Automatic` inside; values
  read or decay from the place like any other object.
- `sizeof` of a brace-inferred array (or an unsized compound literal) in
  `static_assert` uses `TypeResolver::inferred_array_length`, which counts
  elements with the same designator and brace-elision rules.
- A designator whose target is an aggregate and whose value is a bare
  expression continues brace elision with the following items
  (`.a = 1, 2, 3` fills `a[0..3]`).
- A two-element braced complex initializer (`_Complex float c = {re, im}`)
  lowers to `aggregate<complex<T>>(index0 = re, index1 = im)`, each converted
  to the component type. `{x}` and a bare scalar stay `real_to_complex`.
  Brace elision into a complex component pair is not modeled.
- A trailing flexible array member has size 0 and the element's alignment in
  the record layout. Omitted, it is skipped by `zero_fill` and absent from the
  members. Initialized (`{1, {2, 3}}`, elided `{1, 2, 3}`, or `.d = {..}`),
  it appears as a normal `Field(last)` member whose value has a sized
  `array<T, N>` type. The variable and aggregate keep the declared record
  type; the object's extent is the record size plus that member's size, so
  consumers read it from the initializer rather than the type.

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

| Node                                                                     | Meaning                                                  | Metadata                                                       |
| ------------------------------------------------------------------------ | -------------------------------------------------------- | -------------------------------------------------------------- |
| `widen<i32>(x)`                                                          | value-preserving sign/zero extend (by source signedness) | `reason=promotion\|usual_arith\|assign\|arg\|vararg\|explicit` |
| `truncate<i8>(x)`                                                        | keep low bits                                            | `fits=always\|unknown`                                         |
| `reinterpret<u32>(x)`                                                    | same width, sign change                                  | `fits=always\|unknown`                                         |
| `from_bool<i32>(b)`                                                      | 0-1 (truth conversion is `ne(x, 0)`, not a node)         |                                                                |
| `bit_cast<T>(x)`                                                         | `__builtin_bit_cast`: same object representation         | `reason=explicit`; sizes checked equal, `T` not an array       |
| `float_widen<f64>(x)` / `float_narrow<f32>(x)` / `float_convert<d64>(x)` |                                                          |                                                                |
| `int_to_float<f64>(x)`                                                   |                                                          | `exact=true\|false`                                            |
| `float_to_int<i32>(x)`                                                   |                                                          | `out_of_range=ub`                                              |

A C conversion changing width and signedness is two nodes in fixed order:
width first (in source signedness), then reinterpret.
`(u32)(i8)x` → `reinterpret<u32>(widen<i32>(x))`.

`fits` is only filled in when trivially known during lowering (constants);
range-based facts belong to the analysis pass.

Which conversions are legal is decided entirely by
`CTypes::classify_conversion` (`src/sema/ctype/convert.rs`) over C types, not
over IR types. It answers with a `CastKind` plus an optional warning, and
`Lowerer::emit_cast` (`src/sema/expression.rs`) only emits the kind it is
given, so no conversion is accepted merely because nothing rejected it. The
context — `Assign`, `Arg`, `Return` or `Cast` — is what separates the
assignment constraints of 6.5.16.1 from the cast constraints of 6.5.4: a cast
is silent where an assignment warns. Because classification is on C types
while emission is on layout, `CastKind::Identity` still emits an arithmetic
conversion when the two layouts differ (a `_Bool`-valued comparison whose C
type is `int`, for example).

Rejected outright: conversion to or from `void`, to a function or array type,
between a struct/union and an unrelated type, and between a pointer and a
floating type.

### Modifiable lvalues

6.3.2.1p1 is enforced on simple assignment, compound assignment and `++`/`--`
by `TypeResolver::require_modifiable_lvalue` (`src/sema/types.rs`), which runs
on the C type of the place. An array, a `const`-qualified lvalue, or a
struct/union with a (recursively) `const`-qualified member is
`ResolveError::Invalid`. All three compilers reject these, so they are errors
rather than warnings. Fixture:
`tests/fixtures/sema/ir_modifiable_lvalue.c`.

### Pointer comparisons

`==`/`!=` and the relational operators accept a pointer against a null pointer
constant, and a `void *` against any object pointer, silently. Comparing
pointers whose pointees have no composite type warns
`compare-distinct-pointer-types`; comparing a pointer against an integer that
is not a null pointer constant warns `pointer-integer-compare`. Both are
warnings in clang, gcc and MSVC alike. Fixture:
`tests/fixtures/sema/ir_pointer_comparison.c`.

### Conditional operator

6.5.15p3-6, in order: two arithmetic operands go through the usual arithmetic
conversions; if one operand is a null pointer constant the result is the other
operand's type; otherwise two pointers merge into a pointer to the composite
type carrying the union of both pointee qualifier sets, with `void *` winning
over an object pointer. `(void *)0` is a null pointer constant, so
`c ? (int *)0 : (void *)0` is `int *`, not `void *` — verified against clang
22 and gcc 16. Fixture: `tests/fixtures/sema/ir_conditional_composite.c`.

### Promotions

```c
short inc(short s) { return s + 1; }
```

```
fn %2 @inc(%3 s: i16 [c="short"]) -> i16 [linkage=external] [fallthrough=ub_if_used] [c_return="short"] {
    return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(1)));
}
```

The widen and truncate stay in the IR because `s`'s storage really is `i16`
(struct layout, stores truncate). The metadata is what lets Slate decide to
emit `s.wrapping_add(1)` or retype `s` as `i32` when analysis proves every
store fits.

## Arithmetic

Ops run on concrete widths: `add(a, b)`, `sub`, `mul`, `div`, `rem`, `shl`,
`shr`, `and`, `or`, `xor`, `minnum`, `maxnum`, `minimum`, `maximum`,
`minimum_num`, `maximum_num`, `neg`, `not`, `eq`/`ne`/`lt`/…

- `overflow=wrap|ub` (unsigned / signed). `impossible` is an analysis fact,
  not emitted by lowering.
- `div`/`rem`: `by_zero=ub`; signed also `min_by_neg_one=ub`.
- `shl`/`shr`: amount type kept separately; `amount_out_of_range=ub`; `shl` of
  a negative signed value is `ub`. Right shift of negative signed values is
  resolved by target (`shr<i32, amount_out_of_range=ub, fill=sign_extend>`).
- The six extrema are floating only and have no C operator spelling; they
  exist for the floating atomic fetches, and are named after the C builtin
  spellings, which are also LLVM's intrinsic names. `minnum`/`maxnum` ignore
  NaN and leave `-0` versus `+0` unordered (LLVM `minnum`, C `fmin`);
  `minimum`/`maximum` propagate NaN and order `-0` below `+0`;
  `minimum_num`/`maximum_num` ignore NaN like `minnum` but order `-0` below
  `+0` (IEEE 754-2019 `minimumNumber`).
- Comparisons, `!`, `&&`, `||` produce `bool`; `from_bool<i32>` is inserted
  only where the result is used as an integer.
- Scalars in boolean context lower to `ne<T>(x, const<T>(0))`; pointers to
  `ne<ptr<T>>(p, null<ptr<T>>)`.
- Short-circuit and `?:` with side-effect-free operands stay as expression
  nodes (`logical_and`, `logical_or`, `conditional`); with side effects they
  become `if` statements (see hoisting).

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
result of computation. Places retain object and projection structure
([grammar](ir-grammar.md#places)).

`read(place)`, `write(place, value)`, and `addr_of(place)` consume places.
`Index` projects into an array place. Pointer indexing uses
`Deref(ptr_offset(pointer, index))`, with offsets in element units.
`Lane` and `Swizzle` project into a vector place.
Each place has a resolved type; field IDs refer to the record layout.
Bit-fields and vector lanes are readable/writable projections but are not
addressable.

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

| C   | IR  | Metadata |
| --- | --- | -------- |

Types and policies are elided below; the grammar has the full forms.

| C                        | IR                                               |
| ------------------------ | ------------------------------------------------ |
| `a[i]` (array read)      | `read(deref(ptr_offset(array_decay(a), i)))`     |
| `a[i] = v` (array)       | `write(deref(ptr_offset(array_decay(a), i)), v)` |
| `*p`                     | `read(deref(p))`                                 |
| `*p = v`                 | `write(deref(p), v)`                             |
| `*(p + i)` / `p[i]`      | `read(deref(ptr_offset(p, i)))`                  |
| `p + i`                  | `ptr_offset(p, i)`                               |
| `p - q`                  | `ptr_diff<i64, element=T, ...>(p, q)`            |
| `p < q`                  | `lt<ptr<T>>(p, q)`                               |
| `&x`                     | `addr_of(x)`                                     |
| `arr` in pointer context | `array_decay<ptr<T>, length=Some(N)>(arr)`       |
| `f` as value             | `function_decay<ptr<fn(..)>>(f)`                 |
| `0`, `NULL`, `(void*)0`  | `null<ptr<T>>`                                   |
| `if (p)`, `!p`           | `ne(p, null)` / `not<bool>(ne(p, null))`         |
| `char* → const char*`    | `pointer_cast<ptr<const i8>>(p)`                 |
| `void* ↔ T*`             | `pointer_cast<ptr<T>>(p)`                        |
| `unsigned* → int*`       | `pointer_cast<ptr<i32>>(p)`                      |
| `(uintptr_t)p` / `(T*)n` | `ptr_to_int<u64>(p)` / `int_to_ptr<ptr<T>>(n)`   |

Every implicit conversion between object pointers is accepted as a
`pointer_cast` and reported as a warning; none is an error. This follows the
consensus rule: MSVC only warns (C4047/C4057/C4133/C5292) on every case clang
22 and gcc 16 reject, so `unsigned ** → int **`, `unsigned * → long *` and
`struct A * → struct B *` are warnings rather than the errors earlier versions
of this document specified. `classify_conversion` (`src/sema/ctype/convert.rs`)
picks the warning, in this order:

| pointees                                    | warning                                     |
| ------------------------------------------- | ------------------------------------------- |
| compatible, `to` drops no qualifier          | none                                        |
| either is `void`                             | none                                        |
| compatible, `to` drops `_Atomic`             | `incompatible-pointer-types`                |
| compatible, `to` drops `const`/`volatile`    | `incompatible-pointer-types-discards-qualifiers` |
| integers differing only in signedness        | `pointer-sign`                              |
| compatible apart from nested qualifiers      | `incompatible-pointer-types-discards-qualifiers` |
| anything else                                | `incompatible-pointer-types`                |

Plain `char` is a distinct type from `signed char` and `unsigned char`, so
`char * → signed char *` warns `pointer-sign` in either direction. Unlike the
old layout-based check, signedness is compared at every pointer level, so
`unsigned ** → int **` warns `pointer-sign` too.

Integer/pointer conversions across an assignment, argument, return or
initializer warn `int-conversion` and still emit `int_to_ptr`/`ptr_to_int`; an
explicit cast is silent. A null pointer constant (an integer constant
expression 0, or such an expression cast to `void *`) becomes `null<ptr<T>>`
rather than a converted integer, so it never warns.

Fixtures: `tests/fixtures/sema/ir_pointer_sign.c`,
`tests/fixtures/sema/ir_pointer_conversion_warnings.c`,
`tests/fixtures/sema/ir_conversion_rules.c`,
`tests/fixtures/sema/ir_atomic_pointee.c`.

Original pointer qualifiers are retained as metadata; volatile/atomic
access behavior is also resolved on the actual accesses. Pointee `const`
is shown in the type (`ptr<const T>`) since Rust distinguishes it.

### Qualified access

Each qualifier lives where its meaning does, following CIR's
`cir.load volatile` / `atomic(seq_cst)` flags on the memory op rather than
on the type:

- `volatile` is a property of the place. Every place carries an `access`
  (volatile, plus whether the object is `_Atomic`), and volatile prints on
  the node that touches memory: `read<i32, volatile>(%g)`. Forming a place
  (`addr_of`, `array_decay`) prints nothing because nothing is accessed.
- Atomicity is a property of the operation, because GCC `__atomic_*`
  builtins work on plain objects. `read`, `write`, `store` and `update`
  carry an optional memory ordering (`atomic=acquire`); none means a
  non-atomic access. A plain access to an `_Atomic` object gets
  `atomic=seq_cst`, derived from the place's `_Atomic` qualifier, since C
  gives it no other ordering.
- A place's access comes from the binding's own qualifiers, from the
  pointee qualifiers of the pointer it dereferences, and for fields from
  the base's access plus the field's own (`field0 status: volatile i32` in
  the record). Array element access is the array object's access, and
  decay and `&` carry it back into the pointer type.
- Pointer types keep pointee `volatile`/`_Atomic` next to `const`
  (`ptr<volatile i32>`). The access through `*p` cannot be recovered any
  other way.
- A declaration also carries its own access, printed as a prefix on its
  type exactly as a record field's is (`global %1 counter: atomic i32`,
  `let %9 flag: volatile i32`, `%13 a: atomic i32` on a parameter). This is
  a property of the *object*, not of one access, so it appears even when
  the translation unit never reads or writes it (`slate-parser-lh7.2.28`);
  Slate needs it to choose the Rust type. Variable, parameter and global
  all derive it from the declared C type, and a redeclaration re-derives it
  from the merged composite.
- An atomic update (compound assignment or `++`/`--` on an `_Atomic`
  object, or a fetch/exchange builtin) is one read-modify-write, so
  side-effect hoisting keeps `update<T, result=..., atomic=...>(place,
  f(old))` whole in a synthetic temporary instead of splitting it into
  read, compute, write. Volatile non-atomic updates are split, and both the
  read and the write stay volatile.
- `restrict` is an aliasing promise about a pointer binding, so it is a
  `[restrict]` flag on the parameter or variable. For an array parameter,
  the qualifiers inside its first brackets (`int a[restrict 4]`) are the
  adjusted pointer's own; `const` there is `[const]` on the parameter and
  `volatile`/`_Atomic` there are its access prefix, so
  `int a[restrict volatile 4]` is `volatile ptr<i32> [restrict]`, while a
  `const` on the element type stays in the pointee (`const int a[3]` is
  `ptr<const i32>`).
- A top-level `const` on an object is `[const]` on its `let` or `global`
  (`int *const q` is `ptr<i32> [const]`), which is what distinguishes an
  immutable binding: `Type::Pointer::is_const` is the pointee's. It follows
  the declarator's outermost qualifiers, so `const int a[2]` (element const,
  and therefore an unmodifiable object) and C23 `constexpr` objects (which
  are const) also carry it; `const int *p` does not.
- Qualifiers inherited through a typedef, and the `_Atomic(T)` specifier,
  count the same as written qualifiers.
- An array parameter adjusts to a pointer, but the brackets it was written
  with survive as `[array=...]` on the parameter: `[array=3]` for a fixed
  length, `[array=%n]` for a captured VLA extent, `[array=*]` for `[*]`,
  and a leading `static` (`[array=static 3]`) for the C99 6.7.6.3p7
  guarantee that the argument gives access to at least that many elements,
  which also implies it is non-null. Plain `int a[]` adds nothing, since it
  is exactly the adjusted pointer.

- `_Atomic` changes layout as well as access. An `_Atomic`-qualified value
  type is padded up to a power-of-two size and aligned to that size, so an
  atomic access to it can be lock-free: `_Atomic struct { char a[3]; }` is
  4 bytes aligned to 4. The promotion applies only up to the target's widest
  promotable width (16 bytes on x86-64 and AArch64, 8 on i386 and ARM32);
  wider objects keep their natural layout, and a zero-sized one still gets
  one byte. The qualifier is never on an array type, so an array of atomic
  elements promotes each element, not the whole array. Sizes and alignments
  reaching the IR are already promoted; nothing in the dump re-derives them.

  That is clang's rule, and it is now selected by the compiler personality
  rather than applied unconditionally. Under `--flavor=gcc` there is no
  promotion at all: gcc keeps the unqualified layout (3/1 where clang says
  4/4, 5/1 where clang says 8/8) and routes the resulting non-lock-free
  accesses through libatomic. The divergence is confined to aggregates —
  scalars agree, because their natural alignment already satisfies the
  promotion — but it reaches member offsets too: `struct { char head;
  _Atomic struct { char a[3]; } value; char tail; }` is 12 bytes with the
  value at offset 4 under clang, and 5 bytes at offset 1 under gcc. Measured
  against clang 22.1.8 and gcc 16.2.1; fixtures
  `tests/fixtures/sema/ir_atomic_layout.c` (clang) and
  `ir_atomic_layout_gcc.c` (gcc).

  `--flavor=msvc` stays on clang's rule for now, and is known wrong: MSVC
  prepends a lock word to a non-lock-free atomic, giving
  `4 + round_up(sizeof(T), 4)` at alignment 4, which changes the object
  representation rather than only its size (`slate-parser-rol`). The two
  personalities also disagree on how an atomic aggregate *argument* is passed
  (`slate-parser-lh7.2.29`), which is still open: the flavor reaches
  `AbiClassifier`, but the C type does not, because it classifies `ir::Type`
  and `_Atomic struct S` lowers to the same `Type::Defined` as `struct S`.

`tests/fixtures/sema/ir_qualified_access.c`,
`tests/fixtures/sema/ir_array_parameter.c` and
`tests/fixtures/sema/ir_atomic_layout.c` cover these.

### Explicit atomic operations

The `__c11_atomic_*` builtins (what clang's `<stdatomic.h>` expands to) and
the GCC `__atomic_*` builtins resolve in sema without declarations. They
are type-generic and resolve from the pointer argument's pointee. They
lower onto the existing access nodes; only compare-exchange and fences get
their own:

| Source | IR |
|---|---|
| `load(p, o)` | `read<T, atomic=o>(deref(p))` |
| `store(p, v, o)` | `write<T, atomic=o>(deref(p), v)` |
| `fetch_OP(p, v, o)` | `update<T, result=old, atomic=o>(deref(p), OP(old, v))` |
| `__atomic_OP_fetch(p, v, o)` | `update<T, result=new, atomic=o>(deref(p), OP(old, v))` |
| `exchange(p, v, o)` | `update<T, result=old, atomic=o>(deref(p), v)` |
| nand / min / max | `not(and(old, v))` / `conditional(lt\|gt(old, v), old, v)` |
| `__atomic_test_and_set` / `__atomic_clear` | `update` to 1 / `write` of 0 (a `void *` pointee is `u8`) |
| generic `__atomic_load/store/exchange` | the same nodes, reading/writing through the pointer arguments |
| `__c11_atomic_init(p, v)` | non-atomic `write` |
| compare-exchange | `compare_exchange<T, weak=, success=, failure=>(place, expected, desired)` |
| `*_thread_fence` / `*_signal_fence` | `fence<scope=thread\|signal, order=o>` statement |
| `__sync_fetch_and_OP` / `__sync_OP_and_fetch` | `update` with `result=old` / `result=new`, `atomic=seq_cst` |
| `__sync_bool_compare_and_swap` / `__sync_val_compare_and_swap` | `compare_exchange<T, form=success\|old, weak=false, success=seq_cst, failure=seq_cst>` with a value `expected` |
| `__sync_lock_test_and_set` / `__sync_swap` | `update<T, result=old>` of the value, `atomic=acquire` / `seq_cst` |
| `__sync_lock_release` | `write<T, atomic=release>` of 0 |
| `__sync_synchronize` | `fence<scope=thread, order=seq_cst>` |
| lock-free queries | `const<bool>` when decidable, else a call to libatomic's `__atomic_is_lock_free` |

- Orderings are recorded exactly as written, including ones that are UB
  for the operation (an acquire store, a failure ordering stronger than
  success). `consume` stays distinct from `acquire`.
- A constant ordering folds to its name (a required fold). A non-constant
  one prints as `atomic=dynamic(v)`.
- Fetch arithmetic is in `T` itself with `overflow=wrap`; C11 7.17.7.5
  gives atomic fetch operations no undefined results. On a pointer, C11
  `__c11_atomic_fetch_add` offsets in elements (`ptr_offset<..., element=T>`)
  and GCC `__atomic_fetch_add` in bytes (`element=u8`).
- The builtin's name is kept as `c_builtin` metadata on the call's node,
  since an exchange or fetch no longer names itself once it is an `update`.
  When the builtin's identifier came from a macro, `c_macro` names the
  innermost one, which for `<stdatomic.h>` is the standard name
  (`atomic_fetch_add_explicit`) even through a user macro wrapping it.
- A constant GCC compare-exchange `weak` argument folds to `weak=true|false`;
  a non-constant one prints as `weak=dynamic(v)`, since clang then picks
  strong or weak at run time.
- Clang accepts floating `add`/`sub` fetches (`atomicrmw fadd`); they lower
  to floating `add`/`sub` with the ambient floating semantics. Floating
  `min`/`max` (`atomicrmw fmin`/`fmax`) lower to `minnum`/`maxnum` with the
  same semantics: those are NaN-ignoring (LLVM `minnum`, C `fmin`), which the
  integer compare-and-select form cannot express, since `lt(old, v) ? old : v`
  answers NaN when `v` is NaN. `__atomic_fetch_fminimum`/`fmaximum` and their
  `_num` variants lower the same way to `minimum`/`maximum` and
  `minimum_num`/`maximum_num`; they reject a non-floating object.
  `__sync_fetch_and_min/max` stay integer-only, as clang requires there.
- Not every family spells every operation. `min`/`max` have no `__sync_`
  spelling beyond the explicit `fetch_and_[u]min/max` names, and the floating
  extrema exist only as `__atomic_fetch_<op>` — there is no
  `__atomic_fminimum_fetch` or `__c11_atomic_fetch_fminimum`. Names outside a
  family's set are not atomic builtins at all and fall through to ordinary
  name resolution, matching clang's "call to undeclared function".
- `__sync_*` builtins are all `seq_cst` except `lock_test_and_set`
  (acquire, as gcc documents it; clang strengthens it to `seq_cst`) and
  `lock_release` (release). Trailing "protected variable" arguments are
  ignored. `__sync_fetch_and_min/max` compare signed and `umin/umax`
  unsigned, whatever the object's signedness, through `reinterpret`
  conversions of both operands. On a pointer, an integer operand offsets in
  bytes, as with `__atomic_*` (gcc; clang requires a pointer operand).
- `__c11_atomic_is_lock_free(n)`, `__atomic_is_lock_free(n, p)` and
  `__atomic_always_lock_free(n, p)` follow clang's constant evaluator: true
  when `n` is a power of two no wider than the target's max inline atomic
  width (`TargetInfo::max_atomic_inline_bytes`: 16 on AArch64 and on x86-64
  with `cx16`, else 8), and `n == 1`, `p` is null, or `p`'s pointee is
  aligned to at least `n`. Otherwise `always_lock_free` is false and the
  other two call libatomic's `bool __atomic_is_lock_free(size_t, const
  volatile void *)`, declared on first use.

`tests/fixtures/sema/ir_atomic_builtins.c`, `ir_atomic_stdatomic.c`,
`ir_atomic_sync.c`, `ir_atomic_sync_gcc.c`, `ir_atomic_extensions.c` and
`ir_atomic_lock_free.c` (plus the AArch64 and `-mcx16` variants) cover these.

## Things C leaves implicit that the IR materializes

- C99 and later `main` falling off the end → `fallthrough=ret_zero` on its definition.
- A void function falling off the end → `fallthrough=ret_void`; a non-void function reached at the end has `fallthrough=ub_if_used`.
- Constant `sizeof`/`_Alignof`/`offsetof` → folded value with `size_of=T`
  metadata; runtime array sizes use captured extents.
- `__builtin_types_compatible_p(A, B)` → `const<i32>(0|1)` with
  `types_compatible="A, B"` metadata naming the two unqualified canonical C
  types. It answers with `CTypes::compatible` (6.2.7), the same predicate
  redeclaration merging uses. Top-level qualifiers (and array element
  qualifiers) are ignored; distinct C integer types that share an IR type
  (`long`/`long long` on LLP64, `char`/`signed char`) are incompatible;
  an enum is compatible with its underlying integer type but not with another
  enum; `int[]` and `int[5]` are compatible; an unprototyped function is
  compatible with a prototype whose parameters survive the default argument
  promotions, so `int(*)()` and `int(*)(int)` are compatible before C23 and
  incompatible from C23 on, where `()` is itself a prototype. Verified against
  clang 22 and gcc 16 on both standards. It also folds in `static_assert`
  conditions.
- `__builtin_constant_p(x)` → `const<i32>(0|1)` with
  `c_builtin="__builtin_constant_p"` metadata; the operand is not evaluated.
  It is 1 when the lowered operand folds (enumerators, literals, `sizeof`,
  arithmetic on those) and 0 otherwise, which is clang's `-O0` answer: a
  GCC or optimized build may say 1 for an object whose value inlining makes
  known. It is an integer constant expression, so it folds in
  `static_assert` conditions and array bounds too.
- String literals: an internal `.strN` global holding `code_units<array<i8, N>>(..)`,
  used through `array_decay<ptr<i8>, length=Some(N)>`.
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

`tests/fixtures/sema/ir_add.c` is the executable version; it includes
`tests/fixtures/add.c`. The excerpts omit the target header and the string
literal global.

Source metadata shown (`--show-metadata`):

```text
fn %1 @add(%2 a: i32 [c="int"], %3 b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
    let %4 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3)) [c="int"];
    return read<i32>(%4);
}
```

Source metadata hidden (required semantics remain visible):

```text
fn %1 @add(%2 a: i32, %3 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    let %4 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3));
    return read<i32>(%4);
}
fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
    call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(2), const<i32>(3)));
}
```

## Open questions

1. What function "type parameters" represent in C.

## Explicit calling conventions at the AST boundary

The AST carries typed calling-convention requests on declaration specifiers
and nested/trailing declarator attributes, including unevaluated `regparm`
expressions. Sema must combine these with the target and compiler options
when resolving function and function-pointer ABIs. This parsing support
does not yet implement ABI resolution or change the current IR schema.
