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
`tests/fixtures/clang/linux/x86_64/ir_metadata_single_owner.c` covers this.

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
Variables with GNU named-register declarations retain the register spelling on
their binding so later lowering can preserve the fixed-register constraint.
Statements support declarations, writes, and numeric operations.
Pointer nulls, address-of values, byte-array constants, array decay,
function decay, and calls are represented explicitly. A call names its callee
as a binding ID (`call<T>(%3, ...)`) when the designator resolves to a known
function or a function-like builtin, or as a pointer value
(`call<T>(read<ptr<fn(..)>>(%7), ...)`) for an indirect call. Builtins look
like functions, as clang's lazily created builtin `FunctionDecl`s do: a builtin
used without a declaration gets one implicit `fn` per spelling
(`fn %9 @__builtin_abort() -> void [linkage=external] [noreturn]`) with unnamed
parameters and the registry prototype, and every function that is a builtin
carries `c_builtin` metadata. As in clang, that implicit declaration is a
redeclaration of any function of the same name with linkage, so a builtin
called without a visible declaration binds to an earlier block-scope `extern`
declaration or to a later one at any scope (`__builtin_exit` included), and
the unit gets one `fn` for it
(`tests/fixtures/gcc/linux/x86_64/ir_implicit_builtin_redeclared.c`); calls lowered before
the later declaration still use the builtin's signature. The builtin's `NoReturn` attribute becomes the
function's `[noreturn]` (`tests/fixtures/clang/linux/x86_64/ir_builtin_noreturn.c`), and
`Const`/`Pure` become `[memory=none]`/`[memory=read]`, exactly as GNU
`__attribute__((const))`/`((pure))` on a declaration do (`const` wins when both
are given). `ConstIgnoringErrno` and similar variants do not count, and
`NoThrow` is dropped since C without `-fexceptions` never unwinds. A builtin used only inside an
unevaluated `sizeof`/`_Generic` operand leaves no declaration.
Each call value carries the signature sema resolved at its own call site,
printed as `signature=fn(...) -> T`, preserving the prototype, variadic,
and unprototyped distinction even when a later redeclaration of the same
function changes it
(`tests/fixtures/clang/linux/x86_64/ir_call_signatures.c`); `--compact-ir` hides it.

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
An ordinary declaration of a builtin's name keeps builtin status when, as in
clang, it is a function with external linkage whose type is compatible with
the builtin's signature (an unprototyped `int abs();`, a `const` parameter, or
a missing `noreturn` still match); calls then go to that declaration with the
builtin's signature, and the declaration picks up the builtin's `noreturn` and
`c_builtin`. An incompatible or `static` declaration shadows the
builtin and its calls go to the declared function
(`tests/fixtures/clang/linux/x86_64/ir_redeclared_builtins.c`). An incompatible one with
external linkage still inherits a builtin's `noreturn`, because clang keeps
noreturn in the function type it merges with the implicit builtin
declaration (`int exit(long);` is `[noreturn]`); `Const`/`Pure` are
attributes tied to builtin status and are not inherited. This is the clang
flavor only: GCC (`void _Exit(long);` in
`tests/fixtures/gcc/linux/x86_64/builtin_prefixed_library.c`) and MSVC keep compiling
code after such a call. Under the MSVC flavor a library builtin
(`ClangBuiltinKind::Library`: `exit`, `abort`, `toupper`, `cbrt`) gets
neither `noreturn` nor `Const`/`Pure` from the registry, declared or
implicit: cl.exe calls `toupper(x) + toupper(x)` twice and keeps code after
`exit`, and learns noreturn only from `__declspec(noreturn)`, which like the
GNU attribute applies to every declaration and call of the function
(`tests/fixtures/msvc/linux/x86_64/ir_library_builtins.c`). The declaration keeps
`c_builtin`, which names the libc entity rather than claiming semantics.
Header provenance plays no
part: it decides libc identity for the Rust handoff, not builtin semantics.

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
`tests/fixtures/clang/linux/x86_64/ir_implicit_builtins.c`.

`__builtin_va_list` follows clang's per-target `BuiltinVaListKind`
(`TargetInfo::va_list_kind`). Where clang makes it `char *` (Windows, i386,
aarch64 Darwin) it is exactly `char *`, and the va builtins take a `ptr<i8>`
place: the MSVC CRT declares `typedef char* va_list` and clang's `vadefs.h`
feeds it to `__builtin_va_start`. Elsewhere it lowers to the opaque
`Type::VaList` (printed `va_list`), with storage 24/8 for the x86_64 SysV ABI
(Linux, Darwin, Android, FreeBSD), 32/8 for AAPCS64 (non-Darwin aarch64) and
pointer sized for 32-bit ARM. It does not model the array-to-pointer decay the x86_64 and
aarch64 ABIs give a `va_list` parameter; it is passed as one scalar handle.
`__builtin_va_arg(ap, T)` lowers to `va_arg<T>(place)`: a type-directed read
that advances the list, so it counts as a side effect for hoisting like a call.
`T` may be a record (`tests/fixtures/clang/linux/x86_64/ir_va_arg.c`). `__builtin_va_start`,
`__builtin_va_end` and `__builtin_va_copy` lower to void `va_start(place)`,
`va_end(place)` and `va_copy(dest, src)` values, also effects
(`tests/fixtures/clang/linux/x86_64/ir_va_start_end_copy.c`). They are recognized by callee
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
`CTypes::declaration_spelling`. None of the three is a declared entity, so
they are bound in `Lowerer::place` ahead of name resolution and each
occurrence emits its own `.strN`; a use outside a function is rejected, where
Clang warns and recovers. `tests/fixtures/clang/linux/x86_64/ir_function_name_builtins.c`
pins the Clang spelling together with `sizeof`, indexing and a static
initializer, and `ir_function_name_builtins_gcc.c` pins the GCC one.

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
the others produce no IR at all (`tests/fixtures/clang/linux/x86_64/ir_generic_selection.c`).
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
`tests/fixtures/clang/linux/x86_64/ir_typeof.c` and `ir_typeof_qualifiers.c`.

**Decided:** `__auto_type` and C23 `auto` (`TypeSpecifier::Inferred`) are
resolved entirely in sema, through the typeof path, so the IR only ever sees the
deduced concrete type and never a mark that it was inferred.
`Lowerer::declaration` types the initializer speculatively (`speculative_type`)
and hands the deduced base to `TypeResolver` for that one declarator. Everything
after type resolution runs unchanged. The deduced type is the
initializer's lvalue-converted type: arrays and functions decay; `const`,
`volatile` and `restrict` are dropped; qualifiers in the specifiers are added
back. Clang keeps `_Atomic` and gcc drops it, so that is decided per flavor.

The declarator may be a pattern, as in clang's C++-style deduction:
`auto *p = cip` deduces `const int`; `const auto *p = ip` adds `const`;
`auto (*fp)(int) = g` and `auto (*pa)[3] = &arr` must match the initializer
exactly; a top-level array is an error.

Several declarators must deduce the same type after the specifier qualifiers
are removed. Under `--flavor=gcc`, only one declarator is allowed, and it must be
a plain identifier.

Always errors: no initializer; a braced initializer; `typedef`; `void`; and the
declared name appearing in its own initializer (checked against the name
bindings before typing). A bit-field initializer is an error, except C23
`auto` under gcc: gcc deduces its internal bit-field type (`unsigned char:3`),
which slate cannot represent, so that case is `Unsupported`. `static auto` and
other storage classes combine with the C23 inference `auto`.

Known gap: slate types `nullptr` as `void *` rather than `nullptr_t`, so
`auto n = nullptr` deduces `void *`, and `auto *p = nullptr` is accepted where
clang rejects it. See `tests/fixtures/clang/linux/x86_64/c23_auto_inference.c` and
`gcc_auto_inference.c`.

Declarator type derivation visits prefix pointers and arrays before wrapping
suffix function and array forms, so `int *f(void)` is `fn() -> ptr<i32>` and
`int *a[3]` is `array<ptr<i32>, 3>`; grouped declarators such as
`int (*f)(int)` build the suffix type on the base and let the grouped core
wrap it, matching C's precedence. Pointer↔integer casts lower as explicit
`ptr_to_int`/`int_to_ptr` conversions, and pointer relational comparisons
reuse `CompareOp::{lt, le, gt, ge}` on pointer operands
(`tests/fixtures/clang/linux/x86_64/ir_pointers.c`: buffer fill cursor, string walk,
out-parameter write, pointer round-trip). A function designator used as a
value decays to `function_decay<ptr<fn(..)>>(place)`, and since C makes `*f`
on a function designator that same designator, `(*fp)(x)` lowers identically
to `fp(x)` and `(*f)(x)` stays a direct call
(`tests/fixtures/clang/linux/x86_64/ir_indirect_calls.c`: parameter, local, struct field,
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
as `c_const`, `c_volatile`, `c_restrict`, `c_atomic` and `c_unaligned` metadata. All of these
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
`tests/fixtures/clang/linux/x86_64/ir_module_promotions.c` exercises real C arithmetic,
explicit narrowing, integer promotion, and return widening.

The module dump also handles `tests/fixtures/clang/linux/x86_64/add.c`: its stdio declaration,
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
with source metadata. An attribute statement without `fallthrough` becomes a
null statement. An attributed statement (`[[x]] stmt`) lowers to its statement,
and its attributes are dropped, since clang and gcc only warn about them;
`fallthrough` on a non-empty statement is an error except under the gcc flavor,
which ignores it. A nested function definition is an error under clang and
MSVC. Under gcc it is still unsupported (`slate-parser-dyd.56`). Case endpoints are folded to the promoted switch type;
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
(`tests/fixtures/clang/linux/x86_64/ir_enum_typing_c89.c`, `..._c23.c`). Known gap: clang
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
`x86_64-pc-windows-msvc`, `aarch64-pc-windows-msvc`, `i686-pc-windows-msvc`
and `thumbv7a-pc-windows-msvc` are experimental Windows MSVC-environment
profiles, with 32-bit `long`, binary64 long double, and unsigned 16-bit
wchar_t. On i686 `long long` and `double` are 8-aligned (4 on i386 Linux),
pointers are 4 bytes, the stack alignment is 4, and the convention is
`x86_win32`. thumbv7a has signed `char` (unsigned on Arm32 Linux), 4-byte
pointers, stack alignment 8, a `char *` va_list, and the same
`aapcs32_hard_float` convention as armv7 gnueabihf, whose clang signatures it
matches. Their target triples do not implicitly select compiler flavor:
`--flavor=msvc` loads the checked-in MSVC 19.51.36257 snapshots (19.44.35228
for thumbv7a, the last toolset with Arm32); the default
Clang flavor loads the Clang 22.1.8 Windows snapshots. Neither loads Linux/glibc
shim defaults. GCC on these Windows profiles is rejected rather than falling
back to Linux macros.

This is selection scaffolding, not full Windows compatibility: calling
conventions, extended-type availability, compiler-option validation,
standard-mode macro adjustments for native MSVC, and SDK/header integration
remain incomplete. Record layout on these triples is Microsoft's (see
"Microsoft record layout" below). The checked-in 32-bit ARM MSVC snapshot
remains unwired.

The type view resolves struct, union, and enum tag definitions. On the
supported Linux x86_64, x86, AArch64, and ARM32 targets, record layout records byte size, aggregate alignment,
one byte offset per field, bit offsets for bit-fields, and byte extents for
contiguous bit-field storage units. It applies packed,
aligned, and `_Alignas` requests, including local field alignment. An
`aligned` typedef stays structural in the IR type (`typedef int A
__attribute__((aligned(16)))` is still `i32`); its alignment lives on the
sema-side `Typedef` C type and reaches the IR only where alignment is already
recorded: record field placement, `align_of` constants, and `[align=N]` on
globals and locals. Unlike a declaration-site `aligned`, it replaces the
natural alignment and may lower it (`aligned(1)` gives 1 under clang and
gcc); the outermost aligned typedef in a chain wins, arrays inherit it from
their element, `_Atomic` drops it, and an array whose element size is not a
multiple of it is rejected. Access alignment through a pointer to such a type
is not modeled, since IR loads and stores carry no alignment.

On every x86-64 target (Linux, Darwin, Windows) the psABI large-array rule
raises a declared array object of at least 16 bytes to 16-byte alignment:
globals, `extern` declarations, static and automatic locals, arrays of
records, arrays of under-aligned typedef elements, and arrays whose length
comes from the initializer. clang and gcc agree on it. An `aligned` or
`_Alignas` on the declaration suppresses it (`long long a[2]
__attribute__((aligned(4)))` stays 4); a typedef's own alignment does not.
It does not apply to records, incomplete arrays, VLAs, string literals, or
compound literals. It is observable because other translation units may
assume it (clang emits `align 16` on an `extern char x[32]`), so the Rust side
must give such arrays 16-byte alignment. gcc additionally over-aligns data for
speed, even at -O0 (a 20-byte record to 16, `char[100]` to 32, `char[15]` to 8);
that is a speed choice no other unit can rely on and is not modeled.

Unnamed members and zero-width bit-fields remain in the field list. Enum
values are evaluated in declaration order, each enumerator entering the ordinary scope
as an `int` constant the moment its value is known, so a later enumerator
sees it with its type (`enum { A = 1, B = sizeof(A) }`) rather than as a
substituted literal;
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
and callable types/ABI contracts, in their respective lowering tasks. `tests/fixtures/clang/linux/x86_64/ir_records.c` covers nested
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
The bf16, f16, f80, and f128 printer uses `rustc_apfloat` directly, without an
f64 conversion. NaNs retain hexadecimal bits in every format.
These lower to one `ValueKind::Arith` node keyed by `ArithOp`
(`add`/`sub`/`mul`/`div`/`rem`/`and`/`or`/`xor`/`shl`/`shr`), all carrying `ArithSema` metadata. They are
not folded or reassociated. Signed overflow defaults to
`ub`, unsigned overflow to `wrap`; floating arithmetic defaults to
nearest-even rounding with ignored exceptions for the default Clang flavor
(observable exceptions for GCC). Floating arithmetic also carries
`contract=`, the fused multiply-add permission: `on` for Clang and MSVC
(within one expression), `fast` for GCC in `gnu*` modes (across statements),
`off` for GCC in ISO modes. Complex floating arithmetic carries `range=`,
`full` unless `CX_LIMITED_RANGE` permits `basic`. Both are recorded because
they are useful to a consumer, not because ignoring them is wrong.
Translation-unit operation options
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
one of nine floating formats: binary bf16, f16, f32, f64, f80, f128 and decimal
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

`__bf16` lowers to `bf16`, its own format rather than an alias of `f16`: it is
a truncated f32, so it has 8 mantissa bits and f32's 8 exponent bits, while
f16 has 11 mantissa bits and 5 exponent bits. Neither format can hold every
value of the other, so `FloatType::widens_from` compares mantissa and exponent
width instead of the enum's declaration order — bf16 and f16 are incomparable,
and a conversion either way prints as `float_narrow`. `bf16` widens only to
f32 and above. Its rank in the usual arithmetic conversions sits below f16
(`__bf16 + _Float16` is `_Float16`, matching clang's `FloatingRank`), and like
`_Float16` it is not subject to the default argument promotions. `_Complex
__bf16` is represented as `complex<bf16>`. There is no `bf16` literal suffix, so a bf16
value is written `1.5bf16` / `1.5BF16`, which gcc accepts and clang does not.
The suffix is accepted in every flavor, like the gcc-only `df`/`dd`/`dl` and
`q` suffixes already were: input is assumed to already compile. It matters
because gcc spells its own limit macros with it (`__BFLT16_MAX__` is
`3.389…e+38BF16`), so a gcc predefine set is unusable without it. In
`FLOAT_SUFFIXES` the entry must precede `f16`, since the suffix is found by the
first `ends_with` match over an ordered list.

Bit-precise integers follow their own conversion rules rather than the
standard-integer ones. These rules are **not** gated on the standard mode.
`_BitInt` is a C23 feature, but clang and gcc both accept it as an extension
in every earlier mode (diagnosed only under `-pedantic` /
`-Wbit-int-extension`) and apply identical conversion rules there; result
types were checked to be byte-identical across c89/c99/c11/c17/c23 and their
gnu variants in both compilers. Lowering therefore applies them
unconditionally, and `TranslationUnit.standard` does not reach this path.

A `wb`/`uwb` literal takes the narrowest bit-precise type that holds its
value (C23 6.4.4.1p6): `N` is the value's bit count for `uwb`, one more than
that for `wb`, floored at 1 and 2 respectively, so `1wb` is `i2b`, `0uwb` is
`u1b`, and `0xffwb` is `i9b`. The radix never makes an unsuffixed literal
unsigned the way it does for standard integers — only `u` does. A value
needing more than `BIT_INT_MAX_WIDTH` (65535) bits is diagnosed as too large
for any `_BitInt` type rather than falling back to a standard integer.
Fixture: `tests/fixtures/clang/linux/x86_64/ir_bitint_literals.c`; widths verified against
clang 22.1.8.

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
`tests/fixtures/clang/linux/x86_64/ir_bitint_conversions.c` pins every case; its result
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
`-f[no-]trapping-math`, plus the long-double options above. Region-scoped
floating pragmas override them (see the pragma section); function attribute
overrides are not yet wired into this path. Initializers of static and
thread storage duration objects always use nearest-even rounding and
ignored exceptions, whatever the region: C F.8.5 evaluates them at
translation time, and clang and gcc both fold them that way.
The `pointer_wrap` setting implied by strict-overflow options controls pointer
offset overflow, independently of integer overflow. `-fwrapv` alone does not
change pointer contracts.

`CompilerOptions` groups operation and layout settings and preserves ordered
compiler arguments on the translation unit. Argument provenance supplies no
missing operation semantics. The module dump retains the effective target;
the expression-only diagnostic mode does not print a module header. The
header's `storage` lines are exhaustive over the directly nameable scalar
formats -- `bool`, every standard integer width, and all nine floating
formats including `bf16` and the decimals -- with a line omitted only when the
target does not support that type. It is not a summary of common types, so a
module can never name a scalar whose size and alignment the header leaves
unstated.
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
resolve return conversions, declarations, or control flow. Fixtures opt in
with `--dump-ir-expressions` in `SLATE-FILECHECK-ARGS`, which supplies extra
renderer arguments to both the test harness and expectation generator.

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
filter after resolution is required for this design. Its roots today are
every declaration in the main file or a `-include` file, declarations with
a retention attribute, and — from any file — every definition clang would
emit: non-`static` function definitions, file-scope object definitions
without `extern` (tentative ones included, declarations of a function
typedef excluded), and `extern` objects with an initializer. That last group
is what keeps a gcc-style `#include "other.c"` test from lowering to an
empty module (`tests/fixtures/clang/linux/x86_64/reachable_from_include.c`). An `inline`
definition counts only when clang gives it a non-discardable linkage,
judged over every file-scope declaration of the name: `gnu_inline` or
gnu89 keeps plain `inline` and drops `extern inline`, C99 keeps
`extern inline` or an `inline` with any non-`inline` redeclaration, and a
windows-msvc target keeps only `dllexport` or an `extern` redeclaration.
Everything else there is `linkonce_odr` in clang, emitted only if used,
even though clang sometimes emits an unused one depending on declaration
order (`reachable_inline_from_include*.c` under `tests/fixtures/clang/linux/x86_64/`
and `tests/fixtures/clang/windows/x86_64/`). Keeping a
declaration also keeps every other declaration of its name, since
redeclarations change emission and attributes.

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

- Operands are one list, `InlineAsm::operands`, in GCC order (outputs
  then inputs), so an `AsmPiece::Operand { index }` is a direct subscript.
  A resolved `Label(n)` piece indexes the statement's label list, not the
  operand number the source wrote, so `%l1` and `%l[done]` both print as
  `%l0` when `done` is the first label.
- `AsmOperandKind` is `In(Value)`, `Out { place }`, or
  `InOut { place, input }`; an output is a `Place` because an asm output
  must be an lvalue. `AsmOperand::direction()` maps to Rust's operand
  forms, and the mapping is inverted from the naive reading: GCC's plain
  `=` lets the allocator reuse an input's register, which is Rust's late
  form. `"=r"` is `lateout`, `"=&r"` is `out`, `"+r"` is `inlateout`,
  `"+&r"` is `inout`. The `=`/`+` comes from the first alternative (as
  clang reads it); `&` in any alternative makes the operand early-clobber,
  which is always safe to over-claim.
- An input whose chosen alternative matches output `n` (`"0"`, `"0,m"`
  when alternative 0 is chosen, `"[out]"`) is folded into that output as
  `InOut { input: Some(tied) }` and removed, and template pieces are
  renumbered. With no alternative chosen, only a tie in every alternative
  folds. A `+` output is `InOut { input: None }`: the place itself is
  read.
- Rust takes one class per operand, so lowering picks one alternative for
  the whole asm (GCC indexes alternatives across all operands): the first
  where every operand has a usable class. Within an alternative a
  constant input prefers the immediate (both compilers print `$5` for
  `"ri"(5)`), then a register class the width fits, then an explicit
  register, then memory. gcc prefers the register for `rm` and clang the
  memory; either is correct, and the register is what Rust expresses
  without a pointer. A class is unusable when it is unresolved, x87/MMX/AMX
  (clobber-only in Rust, including `t`/`u`/`{st}`), a register the width
  doesn't fit (a 24-byte struct under `r`, i64 on i386), an immediate
  whose value isn't an integer constant, a symbol that isn't a link-time
  address, or a match on an output. The chosen alternative is
  `InlineAsm::alternative`, each operand's class is
  `AsmOperand::selected`, and the ruled-out alternatives are kept as
  `AsmRejection`s. When nothing fits, `alternative` is `None` and the
  operands lower as if unselected. Fixture: `sema/ir_asm_alternatives.c`.
- `i` (and so `g`) offers `Symbol` beside `Immediate`; `n` and the
  target immediate letters offer only `Immediate`, since both compilers
  reject a symbol there. An input selects `Symbol` when its value folds to
  a static-storage object or function plus a constant byte offset, through
  address-of, array/function decay, field and constant index projections,
  constant pointer arithmetic, and casts that keep pointer width
  (`(long)&g`, not `(int)(long)&g` on x86-64). It becomes
  `AsmOperandKind::Symbol(AsmSymbol { binding, offset })`, ranked with an
  immediate, so `"g"(&g)` prints `$g` as both compilers do. String literals
  qualify (their synthetic global), automatic objects and non-constant
  indices do not, and a thread-local does only under gcc (clang rejects
  it). Functions are accepted although both compilers reject them under
  PIE, which slate does not model. Fixtures: `sema/ir_asm_symbols*.c`.
- An input that selects `Immediate` is `In(const<ty>(n))`: its value is
  replaced by the folded integer, since Rust's `const` operand needs a
  constant expression and cannot read a C object. The fold follows clang's
  evaluator: besides integer constant expressions it reads const,
  non-volatile objects of any storage duration through their initializers
  (`static const int k = 2;` gives `$2`, as do a const local, `table[1]`,
  `*(table + 2)`, `s.field`, a string element, a zero-filled element, a
  bit-field wrapped to its width, `*pointer` to a const, and `(int)2.5`),
  so `"g"(k)` also picks the immediate. gcc folds only direct reads at
  `-O0` and the rest from `-O1`, and additionally reads a const tentative
  definition as zero, which slate models for the gcc flavor only. The same
  fold scales symbol indices (`&arr[k]`). Volatile, non-const and extern
  objects, and initializers that are not constant, stay unfolded. Fixtures:
  `sema/ir_asm_const_objects*.c`.
- The parser rejects what both compilers reject and the fold relies on: an
  output without `=`/`+`, `=`/`+`/`&` on an input, a match past the outputs
  or to a `+` output, and two inputs tied to one output. `-` is rejected
  inside a function; gcc accepts it only in file-scope asm, which clang
  has no operand form for, so `Pic` survives only on `Module::asm`. Under clang an
  input also may not match two different outputs, and any-alternative
  matches count as ties; gcc accepts both, so only full ties count there.
- Effects hoist output places first, in operand order, then input values
  in source order, as clang and gcc evaluate them. A tied input sits in
  its output's slot, so `InOut { input }` is an `AsmTiedInput` that keeps
  the source operand number the fold would otherwise lose, and
  `InlineAsm::inputs_in_source_order` rebuilds the source order from it.
  Fixture: `tied_order` in `sema/ir_asm.c`.
- Constraints keep the parsed alternative list (modifiers, hard register,
  matching operand number, or letters). The printer reconstructs the GNU
  spelling from it, so the printed text round-trips the parse rather than
  echoing the source.
- Letters resolve per target to `AsmOperandClass`es (register class,
  explicit register, memory, immediate), keeping the raw letters beside
  them. `g` is reg, mem and imm. Per-target facts checked against the
  compilers: `q` is `reg_abcd` on i386 but any register on x86-64, and `R`
  is `reg` on i386 but the legacy eight on x86-64 (`reg_legacy`).
  x86 `Y`/`W`/`j`/`B`, AArch64 `U` (three chars) and Arm `U` are
  multi-letter constraints. `?`, `!`, `*`, `^` and `$` are preference
  hints and are skipped, and `#` ends the alternative. Anything else,
  including `l`, `X`, `s` and `p`, is `Unresolved` with its letters rather
  than a guess.
- `reg_legacy` (x86-64 `R`) and `vreg_low8` (AArch64 `y`, v0-v7) are
  register subsets with no Rust class. None of their usable members is
  reserved, so emission pins the operand to a free explicit register from
  the set (`in("v7")`, `in("rsi")`), avoiding the asm's other explicit
  operands and clobbers. No save/restore is needed; that trick is only for
  reserved registers such as `rbx`.
- An input whose chosen class is memory (`m`, `o`, Arm `Q`, or `rm`
  whose register won't fit) lowers to `AsmOperandKind::InPlace(place)`
  when the operand is an addressable lvalue, so the object is named rather
  than loaded before the asm. Emission turns it into
  `in(reg) &raw const x` and wraps the reference site. An input that
  chose a register reads the place instead. With no alternative chosen,
  any memory-capable constraint gives `InPlace`. Outputs already carry
  places.
- An unaddressable memory-capable operand (bit-field, vector lane,
  register variable, temporary) falls back to a value: compilers spill it
  to a temporary, which an input cannot tell from a copy. Errors are only
  where compilers agree, for memory-only constraints: a non-lvalue and a
  bit-field (both), and a register variable under gcc (clang spills it).
  clang also rejects bit-fields and lanes under any memory-capable
  constraint and gcc takes the register there; we follow gcc
  (permissive). Fixtures: `sema/ir_asm.c` `memory()`,
  `sema/ir_asm_memory_gcc.c`, `error/asm-memory-*.c`.
- `x` is `xmm_reg`, widened to `ymm_reg` or `zmm_reg` by a 256- or
  512-bit operand (gcc and clang print `%ymm`/`%zmm` there). `v` is
  `zmm_reg` because only that Rust class reaches xmm16-31.
  Fixtures: `sema/**/ir_asm_classes.c`.
- Each operand carries `width`, its type's storage size in bits (so
  `long double` is 128 on x86-64, not 80). A width modifier resolves to an
  `AsmRegisterView` on the piece: x86 `b` 8, `w` 16, `k` 32, `q` 64, `h`
  high byte, `x`/`t`/`g` 128/256/512; AArch64 `b`/`h`/`s`/`d`/`q`
  8-128 and `w`/`x` 32/64. Other modifiers (`c`, `n`, `P`, ...) keep only
  the raw letter.
- An unmodified x86 reference prints at the operand's width (`%dil` for a
  `char`), but Rust's `{0}` prints the full register (`rdi`), so emission
  must add a Rust modifier. An unmodified AArch64 reference prints the full
  `x`/`v` register in both compilers, like Rust. The Rust modifier depends
  on the chosen class (`AsmOperand::selected`), from the view
  or else the width: x86 GPR classes 8 `l`, 16 `x`, 32 `e`, 64 `r`, high
  byte `h`; x86 vector classes up to 128 `x`, 256 `y`, 512 `z`; AArch64
  `reg` 32 `w`, 64 `x`, and `vreg` 8/16/32/64/128 `b`/`h`/`s`/`d`/`q`.
  An explicit register can't appear in a Rust template, so emission names
  it at that width instead (`eax`). No view maps to nothing on Arm.
- A tied input folded into its output keeps its own width at its
  reference sites on x86: gcc prints `"=r"(char) : "0"(int)` as `%dil`
  and `%edi`, so `%1` becomes `%0(32)`. clang rejects size-mismatched
  ties. Fixtures: `sema/**/ir_asm_widths.c`.
- Registers carry the source spelling and, when the target register table
  recognized them, the canonical name. Width is dropped: a clobber clobbers
  the whole register, and an operand carries its own `width`.
- Side effects in operand expressions hoist ahead of the statement like any
  other operand, so the asm itself never contains an embedded effect.
  Fixture: `sema/ir_asm.c`.
- Each asm carries its dialect (`AsmDialect::{Att, Intel}`) on x86 and
  `None` elsewhere. It is per statement, not per module, because MSVC
  `__asm` (Intel) and GNU asm (AT&T) can share a translation unit. The
  target arch is not copied onto the node; it is `Module::target.family`.
  GNU asm is always AT&T until `-masm` is supported.
- A `{att|intel}` alternation is resolved during lowering to the dialect's
  side (first for AT&T, second for Intel, empty when missing), so the
  backend never sees one; the raw template string keeps the source.
  `%{`/`%|`/`%}` are literal braces and bars, not alternation markers.
- A statement asm carries `InlineAsm::options: Some(AsmOptions)`, Rust's
  `asm!` options derived once during lowering; file-scope asm and asm in a
  naked function have `None` (`global_asm!` and `naked_asm!` take none).
  Rules, verified against clang `-emit-llvm` and gcc `-O2` / `-fdump-rtl-expand`:
  - `memory` (`AsmMemory::{None, ReadOnly, Any}` = `nomem` / `readonly` /
    neither) comes from the operands: a memory input (`InPlace`, or an `In`
    whose selected class is `mem`) reads, a memory output writes. A
    `"memory"` clobber or basic asm forces `Any` (gcc gives basic asm a
    memory clobber). It depends on the flavor. Clang also forces `Any` for
    any side-effecting asm (volatile, `goto`, or no outputs), because it
    attaches `memory(none)`/`memory(read)` only to non-side-effecting asm.
    Gcc keeps `nomem` for volatile extended asm: it removes a dead store
    across `asm volatile("nop" :: "r"(x))`.
  - `pure` = not volatile, not `goto`, has an output, and `memory` is not
    `Any` (Rust requires `nomem` or `readonly` with `pure`).
  - `nostack` is always set for GNU asm: neither compiler guarantees stack
    alignment or the red zone to it (clang never emits `alignstack`).
    MSVC `__asm` leaves it off.
  - `preserves_flags` is never set on x86 (both compilers clobber
    flags/dirflag/fpsr implicitly). Elsewhere it is set unless `"cc"` is
    written (gcc aarch64 adds `(clobber (reg:CC cc))` only for `"cc"`).
  - `may_unwind` = an `"unwind"` clobber.
  Fixtures: `sema/ir_asm_options.c`, `sema/ir_asm_options_gcc.c`,
  `sema/aarch64-unknown-linux-gnu/ir_asm_options.c`,
  `sema/armv7-unknown-linux-gnueabihf/ir_asm_options.c`. On 32-bit ARM,
  gcc (`arm-none-eabi-gcc` 16.2, `-fdump-rtl-expand`) adds a CC clobber
  only for `"cc"` in ARM, Thumb-2 and Thumb-1 (armv6-m) alike.
- MSVC `__asm` lowers to the same `Statement::Asm` (`src/sema/ms_asm.rs`,
  design in `wiki/concepts/msvc-asm.md`): volatile, `dialect = Intel`, and
  options with `memory = Any` and nothing else set (no `nostack`, since the
  asm may `push`/`pop`), or `None` in a naked function. `template` is the
  statement rebuilt from the AST. Source operands have no constraint:
  - each C object is one `AsmOperandKind::Memory { place, access }`, however
    often the asm names it; the asm receives its address, and `access`
    (`AsmAccess::{Read, Write, ReadWrite}`) says what the asm may do there,
    joined over every reference (`src/sema/ms_asm_effects.rs`);
  - a function is a `Symbol` operand, and `offset x` an `In` of
    `AddressOf(x)`.
  Each reference to an object is an `AsmPiece::Address { operand,
  displacement, base, index, size }`: `arr[4]`, `s.f` and `x + 4` are byte
  displacements, and registers stay by spelling. `size` is the explicit
  `PTR`, or, where MASM infers it from the C type (no register operand, or
  `movzx`/`movsx`, shifts and rotates, `shld`/`shrd`), the size of the
  innermost element type. That matches the `dword ptr`/`qword ptr` clang
  inserts. Clang-flavor asm labels are `AsmPiece::LocalLabel(name)`, lowercased.
  In the MSVC flavor, an asm label definition is
  `AsmPiece::EntryLabel { name, binding }`, so a C `goto` can target its
  exact position in the block. A jump from asm to a C label is an asm-goto
  `%lN` piece with that C label binding in `labels`.
  `clobbers` are `AsmClobber::Register`s from that effects table:
  written registers widened to their 32-bit name (`al` gives `eax`),
  implicit defs, `st`..`st(7)` for any x87 instruction, and `eax`/`ecx`/
  `edx` for `call`. `esp` is never listed (`nostack` is off) and neither are
  flags (`preserves_flags` is off).
  On 32-bit x86, integer and pointer results of at most 8 bytes add explicit
  EAX (and EDX for results larger than 4 bytes) `Out` operands to every MS
  asm. These write shared synthetic `u32` locals. A written return register
  is an early output and is removed from the clobber list; an unwritten one
  is a late output. `Fallthrough::Return(Value)` reads the captures only at
  the function end, combining the halves as `u64(eax) | (u64(edx) << 32)`
  before conversion to the return type. It prints as `fallthrough=ret(value)`.
  Narrow results truncate; `_Bool` uses the low bit, matching clang's return
  slot load. Explicit returns bypass this value. C99 `main` initializes its
  capture to zero; other captures are uninitialized until an asm writes them.
  Naked functions have no captures, and noreturn functions retain `ub`
  fallthrough. Float, aggregate, wider integer, and x86_64 implicit returns
  remain outside this lowering. Fixtures: `sema/i686-pc-windows-msvc/ms_asm_*.c`.

A naked function (`__attribute__((naked))` on any of its declarations) sets
`FunctionSemantics::naked`, so the backend picks `naked_asm!` from the function
rather than sniffing its body. It is keyed on the attribute alone, not on the
body shape: clang accepts any number of asm statements, basic or extended with
operands that reference no parameter (`"i"(42)`), plus null statements, and
all of them fit one `naked_asm!` (templates joined, `const`/`sym` operands).
Keying on "a single basic asm" would turn every other naked function into an
ordinary one with a prologue. Clang rejects other statements and parameter
references; gcc accepts anything. We accept anything too (permissive), so a
backend must still check the body is asm-only. Fallthrough is `ub` (clang ends
the body in `unreachable`), and asm statements inside carry
`options: None`, since `naked_asm!` takes no options. Fixture:
`sema/ir_naked.c`.

File-scope `asm` lowers to `Module::asm`, a source-ordered list of the same
`InlineAsm`, printed before the type definitions. It is a separate list rather
than a pseudo-declaration because it declares no entity and has no place in the
symbol table; keeping it out of `globals`/`functions` means nothing downstream
has to filter it out. It is almost always basic asm, but the GNU personality
accepts operands at file scope, so it reuses `InlineAsm` and the statement
printer rather than carrying only a template string. Name resolution binds
those operand expressions the same way it binds a statement asm's; only labels
are impossible, since there is no function to hold them.

Pragmas are never IR nodes. They are positional -- each applies to whatever
follows it -- so `sema::pragmas` resolves them in one ordered walk over the
declarations (descending into function bodies, where `StmtKind::Pragma` changes
the same state) and hands the result to the places that already own the thing
the pragma modifies: `pack` and `ms_struct` to record layout, `weak`,
`visibility` and `redefine_extname` to `SymbolAttributes`. Nothing downstream
has to know a pragma was involved.

The walk resolves them to a lookup rather than a running state because sema does
not visit declarations in source order: tag bodies are laid out on demand. Packs
key on `TagId`, symbol pragmas on the symbol name. An explicit attribute always
wins over a pragma, which is why `apply` only fills fields the declaration left
open.

`#pragma pack(N)` is a cap on field alignment, not a request to pack to one
byte: a field's alignment is `min(max(natural_or_packed, aligned_attr), N)`. It
caps an explicit `aligned` on a *field*, but not one on the record, which is why
`#pragma pack(1)` with `__attribute__((aligned(16)))` still gives a 16-aligned
record of packed fields. A pack that is not a small power of two is diagnosed
and ignored, leaving the previous value in effect rather than resetting it, and
while any pack is in effect bit-fields stop padding to avoid straddling a
storage unit -- the same rule `__attribute__((packed))` triggers.

A record is laid out with MS bit-field rules when it carries
`__attribute__((ms_struct))`, or was defined under `#pragma ms_struct on`
without `__attribute__((gcc_struct))`. Each bit-field allocates a whole unit
of its declared type, aligned to the unit's size. Later bit-fields whose types
have the same size fill that unit until one no longer fits, and then a new unit
starts. A non-bit-field following bit-fields begins after the whole unit.

A zero-width bit-field is ignored after a non-bit-field. After a bit-field, it
ends the unit and raises the record's alignment to its type's size.

`packed` does not reduce a bit-field's alignment, but `#pragma pack` caps it.
In a union, bit-fields have alignment 1 and occupy their full type size.

Scalar fields (not enums, `_Complex`, `_BitInt` or `_Atomic`) are aligned to
their size, which matters on i686, where `long long` becomes 8-aligned. A
scalar whose size is not a power of two, such as i686 `long double`, is an
error, as it is in clang. Storage units are the declared-type units, matching
clang's discrete bit-field codegen. Oracle: clang's ItaniumRecordLayoutBuilder
`IsMsStruct` path. Microsoft *target* layout (`*-windows-msvc`) is a separate
algorithm, below.

### Microsoft record layout

Every `*-windows-msvc` triple (x86_64, i686, aarch64, thumbv7a) lays out every
record with clang's `MicrosoftRecordLayoutBuilder`. That holds under both the
Clang and MSVC flavors, and `ms_struct`/`gcc_struct` make no difference; clang
chooses the builder from the target alone. Where it differs from Itanium and
from `ms_struct`:

- Bit-fields share a unit only when their declared types have the same size
  and the next one fits (`struct { char a : 4; int b : 4; }` is 8 bytes).
  In a union, bit-fields take their type's size but do not raise alignment
  (`union { int a : 3; }` is size 4, align 1).
- A zero-width bit-field matters only directly after a non-zero-width
  bit-field. There it rounds the offset up to its type's alignment and raises
  record alignment. Anywhere else it is ignored (`struct { char a; int : 0;
  char b; }` is 2 bytes).
- `packed` on the record acts like `#pragma pack(1)`, bit-fields included.
  A `#pragma pack` wider than a pointer is ignored.
- Required alignment comes from `aligned`/`__declspec(align)`/`_Alignas` on a
  field, from an aligned typedef, or from a nested record's own required
  alignment. Pack does not cap it (a pack(1) record containing an
  `aligned(16)` struct still puts it at 16). On a bit-field, the requested
  alignment raises that field's alignment instead. It is recorded as
  `required_align`, which the Win32 argument ABI reads.
- A record with no storage (empty, only zero-width bit-fields, only
  zero-length or flexible arrays) is 4 bytes, or its alignment when required
  alignment is at least 4.
- Bit units are the discrete declared-type units, as for `ms_struct`.

Oracle: `clang -Xclang -fdump-record-layouts` on each triple, cross-checked
with `tools/cl.exe` (x64, x86, arm64). Fixtures:
`{clang,msvc}/windows/*/ms_record_layout.c`.

`STDC FENV_ACCESS`/`FP_CONTRACT`/`CX_LIMITED_RANGE` and `float_control` are
region-scoped rather than declaration-scoped: sema carries a `FloatingRegion`
(rounding, exceptions, contraction, complex range, precise) that each
compound statement saves on entry and restores on exit, and a pragma changes
it from its position on. Under the Clang and MSVC flavors, matching clang 22:

- `FENV_ACCESS ON` gives `rounding=environment, exceptions=observable`.
  `OFF` and `DEFAULT` give nearest-even rounding (even under
  `-frounding-math`) and the command-line exception behavior.
- `FP_CONTRACT ON|OFF|DEFAULT` gives `contract=on|off|<command line>`;
  `CX_LIMITED_RANGE ON|OFF|DEFAULT` gives `range=basic|full|<command line>`.
  Only the uppercase values are recognized; clang ignores anything else.
- `float_control(except, on|off)` sets exceptions observable or ignored;
  `float_control(precise, on)` gives `contract=on`, `(precise, off)`
  `contract=fast`. The other fast-math permissions `precise, off` grants
  (reassociation, no-NaNs, ...) are not yet represented.
- A pragma must be at file scope or at the start of a compound statement,
  after nothing but other pragmas. The `push` forms and `float_control(pop)`
  must be at file scope, where they save and restore the whole region.
  `FENV_ACCESS ON` and `float_control(except, on)` are errors while precise
  is off, and `float_control(precise, off)` is an error while exceptions are
  observable. A malformed `float_control` is an error.
- The parser hoists a pragma that sits inside a statement to just before
  that statement, so a pragma used as an `if` or loop body is accepted as
  if it began the enclosing compound statement, where clang rejects it.

GCC implements none of these pragmas and ignores them with
`-Wunknown-pragmas`, so under the GCC flavor they have no effect and are
never errors: regions keep the command-line semantics.

Declarator asm labels are separate work (`slate-parser-dyd.4`).

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
  must turn into `if`, not unconditional statements. A direct call to a
  `[memory=none]` or `[memory=read]` function is not a side effect, only its
  arguments can be, so `a && square(b)` stays `logical_and` instead of
  spilling into a synthetic and an `if`
  (`tests/fixtures/clang/linux/x86_64/ir_const_pure_calls.c`).
- Loop conditions and `for` increments with side effects stay attached to
  their `while`/`for` as a statement block with a trailing expression, so
  they re-run per iteration without changing the loop's form.
- Assignment used as a value (`x = y = 0`) → sequential statements, with the
  value read back from the assigned target.

### Unsequenced expressions follow Clang

The cases above are sequenced, so C fixes the order and lowering reproduces it.
Where C leaves two side effects on one object *unsequenced* (C11 6.5p2) the
behaviour is undefined, there is no order to reproduce, and the three oracles
disagree. Same source, `i` starting at 1:

| | result | arguments | final `i` |
| --- | --- | --- | --- |
| Clang 22 `-O0`/`-O2` | 12 | `f(1, 2)` | 3 |
| GCC 16 `-O0`/`-O2` | 21 | `f(2, 1)` | 3 |
| MSVC 19.51 `/Od`,`/O2` | 11 | `f(1, 1)` | 3 |
| slate | 12 | `f(1, 2)` | 3 |

```c
int f(int a, int b) { return a * 10 + b; }
int i = 1; f(i++, i++)
```

All three apply both increments and differ in what reaches the arguments: Clang
interleaves read and write per operand left to right, GCC does the same right to
left, MSVC takes both reads before either write.

**Decided:** slate hoists left to right, which follows Clang. Emulating a flavor
here would pin three compiler *versions'* codegen accidents rather than
documented semantics — GCC's order falls out of its argument-pushing convention
and is not guaranteed between releases. The flavor mechanism is for cases where
the compilers give the same C different *defined* meanings; this is the absence
of a meaning. Left to right is also Rust's own argument order, so it is the
choice that translates without contortion.

Because the order is arbitrary rather than required, the statements that commit
to it are marked, the way every other UB the backend must know about is an
attribute rather than a diagnostic (`overflow=ub`, `by_zero=ub`,
`fallthrough=ub_if_used`):

```
let %18: i32 [synthetic, unsequenced] = read<i32>(%3);
write<i32, unsequenced>(%3, read<i32>(%19));
```

`sema/sequencing.rs` decides it, comparing what each operand of an unsequenced
group reads and writes. A location is a binding — or the object one pointer
binding points at — plus the path of members and *constant* subscripts reached
inside it, so `x.a++` and `x.b++` do not conflict even sharing a storage unit,
`b[0]` and `b[1]` are distinct, and a computed subscript compares equal only to
another computed one. Two locations conflict when one is a prefix of the other
and at least one access is a write. The groups are a call's callee and
arguments, both operands of an arithmetic or comparison operator, and an
assignment's target against its value — the last restricted to a *second side
effect* on the target, since C11 6.5.16p3 sequences the update after both
operands' value computations and so leaves `i = i + 1` well defined.

Checked against `gcc -Wsequence-point` over the corpus: 1963 configurations both
accept, with no case marked here and silent there or the reverse. Clang's
`-Wunsequenced` is narrower — it tracks plain scalars only and reports neither
`g.a++` nor `p->a++` — so GCC is the oracle for detection even though Clang is
the oracle for the order. Fixture: `sema/ir_unsequenced.c`.

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
resolved width, scale, signedness, and saturation mode; the source kind
(`_Fract` or `_Accum`) and rank survive only in the C type metadata. Source spelling and typedefs stay in type metadata.
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
division. `ArithSema::ComplexFloating` carries rounding, exception and range policy;
`ComplexInteger` carries overflow and division-by-zero policy. GNU `~` on a
complex operand is conjugation, not a bitwise complement, for both floating
and integer components (gcc and clang both implement it that way); it prints
as `not<complex<...>>` and is the only unary operator besides `neg` a complex
operand accepts. `_Imaginary` still rejects `~`, matching clang, which has no
imaginary support at all. Fixture:
`tests/fixtures/clang/linux/x86_64/ir_complex_conjugate.c`. The component
of a complex conversion is converted on each side, with the conversion
contract recorded on the operation. `tests/fixtures/clang/linux/x86_64/ir_complex.c` pins
these forms, the common complex sizes, and GNU integer-complex spelling.
Function declarations and calls now carry an `AbiSignature` resolved from the
target's ISA and ABI environment. It records the calling convention and the
passing shape of each argument and the result without replacing source-level
types with hidden pointers or machine registers. Scalar passes remain implicit
in the default dump; nontrivial signatures print `abi=...`. `coerce<...>`
records direct value pieces, `direct` a value passed in registers as its own
type, `byval` a copied memory argument, `byref` an indirect argument, and
`sret` an indirect result. Call nodes keep their own ABI signature because an
indirect callee or a variadic call can differ from the enclosing function.

**`native_c` means trust rustc.** The signature is a label for Slate, not a
lowering: rustc's `extern "C"` (and `"stdcall"` etc.) computes coerce, sret and
byval from a `repr(C)` type's layout and field kinds the same way clang does.
A record or complex value is `native_c` whenever the obvious `repr(C)` stand-in
(same size and alignment, stable Rust field types, `[T; 0]` for a zero-length or
flexible array, a same-size integer for a float format stable Rust lacks)
already makes rustc pass it like the C compiler for the flavor. An explicit
shape means rustc would diverge, so Slate needs a boundary conversion (or a C
shim, when the shape names a type stable Rust cannot express such as `f16`
or `f80`). Scalars, vectors and `void` keep their own forms; `__int128` on
`win64` is plain `scalar` because rustc's `i128` matches clang there.

Every divergent case was found by comparing `rustc --emit=llvm-ir` (with
`#![no_core]`, so it runs for every target without std) against
`clang -emit-llvm` and gcc's assembly. They are exactly these; everything else
(flat, nested, packed, over-aligned, bit-field, union and array records, i686
record arguments, `x86_win32` over-aligned arguments, `win64` records) was
verified to agree:

| Case | Conventions | Explicit shape |
| ---- | ----------- | -------------- |
| `_Atomic` record/complex, clang and msvc flavors | `sysv64`, `x86_cdecl`, `x86_win32`, `aapcs64`, `win_arm64`, `aapcs32_hard_float` | memory, integer, or non-HFA form of the qualified layout |
| flexible array member, clang and msvc flavors (gcc agrees with rustc) | `sysv64` (≤ 16 bytes), `win64` (1/2/4/8 bytes) | `byval`/`byref` + `sret` |
| zero-length or flexible array in an otherwise homogeneous float record: C disqualifies it, rustc ignores the zero-sized field | `aapcs64`, `win_arm64`, `aapcs32_hard_float` | integer pieces |
| `_Float16`, `__bf16`, `long double` (f80/f128), `__float128` members where they set the register class | `sysv64`, `aapcs64`, `win_arm64` | SysV eightbyte classes or HFA |
| complex with such a component | `sysv64`, `aapcs64`, `win_arm64` | same |
| GNU empty record result | `x86_cdecl` | `sret` |
| empty record result (no fields but zero-length arrays, unnamed bit-fields and empty records; 4+ bytes under MS layout), clang flavor only (cl returns it in EAX like rustc) | `x86_win32` | `void` |
| register-sized record result clang still returns in memory (a `char[3]`, `_BitInt` or flexible array member) | `x86_win32` | `sret` |
| complex result of ≤ 8 bytes (gcc agrees: `_Complex char` in AX, `int`/`float` in EDX:EAX) | `x86_cdecl` | `coerce<i16/i32/i64>` |
| homogeneous float record, `_Complex float`/`double` (rustc only uses VFP for `hf` triples) | `aapcs32_hard_float` on Windows | `coerce<fN...>` |
| homogeneous float record or complex float passed variadically | `win_arm64` | integer pieces, `byref` above 16 bytes |

The divergence check runs the same classifier twice: once over the C types and
once over the Rust stand-ins, and it prints `native_c` when the results agree.
On `sysv64` that classifier is a full eightbyte classifier over flattened
scalars (INTEGER/SSE/SSEUP/X87/X87UP/MEMORY, including unaligned members going
to memory and X87 arguments going to memory). An SSE eightbyte of 16-bit floats
prints as clang's vector types: `pair<f16>` is `<2 x half>`, `quad<f16>` is
`<4 x half>`. A divergent record containing a vector member is not classified
and stays `native_c`. On the HFA conventions the C view admits every binary
float format on AArch64 and only `f32`/`f64` on ARM32 (clang does not use
`_Float16` as an HFA base there), and it skips GNU empty records of size zero.
`tests/fixtures/*/*/*/abi_rust_divergence.c` pins every row per target.

`x86_win32` is i686-pc-windows-msvc's cdecl (clang's `X86_32ABIInfo` with
`IsWin32StructABI`). A fixed (non-variadic) non-record argument whose
`required_align` is above 4 is `byref` (records get the same from rustc, so
they are `native_c`). The first three vector arguments are `direct` (XMM
registers); later ones, and any wider than 64 bytes, are `byref`. Results:
clang returns a record, union or complex of 1, 2, 4 or 8 bytes in registers
only when its non-empty fields are all themselves register-sized (zero-length
arrays are skipped; a flexible array member, a field like `char[3]`, or a
`_BitInt` field disqualify it). rustc returns every record of those sizes in
registers, so the disqualified ones are the explicit `sret` cases. clang
ignores an empty record result before that rule, so it is `void`.
`tests/fixtures/clang/windows/i686/abi_target.c` pins these against clang.

**x86-32 calling conventions.** `__stdcall`, `__fastcall`, `__vectorcall`
and `__thiscall` (keywords or GNU attributes) are part of the function type
on 32-bit x86 targets only: `CTypeKind::Function` and `ir::Type::Function`
carry a `CallConv`, printed `fn stdcall(i32) -> i32`, and the function's and
each call's `AbiSignature.calling` repeats it (`x86_win32 stdcall(...)`), so
a call keeps its convention even when compact printing drops the signature.
`__cdecl` is the default `C`. Placement follows clang: a declaration-specifier
or trailing attribute goes to the innermost function declarator (or to a
function typedef named by the specifiers), and one on a pointer or grouped
declarator to the function type beneath it. A convention on a variadic
function is dropped, as clang does (`-Wignored-attributes`). On x86-64 and
other targets the attributes are ignored, as clang does for all of them
except `vectorcall`, which clang honours on x86-64 too but slate does not yet
model. Types differing only in convention are incompatible (pointer
conversions, `_Generic`, `__builtin_types_compatible_p`); a redeclaration
without a convention inherits the earlier one, and one that adds or changes
it is rejected, as clang, gcc and MSVC all do.

The convention is recorded, not modelled. What it implies -- callee stack
cleanup for stdcall, ECX/EDX (`inreg`) assignment for fastcall and
vectorcall, XMM assignment of vectors and HVAs for vectorcall, and the
Windows symbol decoration (`_f@8`, `@f@8`, `f@@8`) -- is left to the Rust
side, where `extern "stdcall"` etc. lower to the same LLVM conventions clang
uses. The argument `abi_pass` shapes are the platform's cdecl ones.
`clang/{windows,linux}/i686/calling_conventions.c`,
`clang/linux/x86_64/calling_conventions_ignored.c` and the two
`error/clang/windows/i686/calling_convention_*.c` fixtures pin this.

ARM32 picks hard-float from the resolved float ABI (`TargetIsa::Arm`, default
from the gnueabihf/gnueabi triple, overridden by `-mfloat-abi`), not from the
triple environment directly. ARM hard-float variadic signatures use base
AAPCS, which rustc also does. The `ir_call_abi.c` and target-specific
`abi_target.c` FileCheck fixtures pin the per-target signatures.

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
`tests/fixtures/clang/linux/x86_64/ir_imaginary.c` and `ir_imaginary_invalid.c` pin these
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
| `x86_win32`          | `direct`      | `direct`                     | `direct` | arg `byref` past 64 bytes or 3 vectors |
| `aapcs64`/`win_arm64`| arg `coerce<i32>` | `direct`                 | `direct` | arg `byref`, result `sret` (align 16) |
| `aapcs32`(`_hard_float`) | arg `coerce<i32>` | `direct`             | `direct` | arg `direct`, result `sret` (align 8) |

Results take the `direct` form wherever the table names a coercion only for
arguments. Two corners are inherited from gcc and pinned by fixtures: a
one-lane `double` vector is passed in memory on `sysv64` (`vector<f64, 1>` is
`byval<align=8>`, though its result is `direct`), and an eight-byte vector of
integer lanes narrower than 64 bits is an MMX type that `x86_cdecl` passes as
`i64`, while `vector<i64, 1>` and float-lane vectors of that size pass
directly. `tests/fixtures/clang/linux/x86_64/ir_vector_abi.c` pins baseline `sysv64`
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
`tests/fixtures/clang/linux/x86_64/ir_vector.c` and `ir_vector_invalid.c` pin these forms.

**Implemented for fixed-point:** `Type::FixedPoint` prints as `fixed<i32, 15>`
(`sat_fixed<..>` for `_Sat`): a two's-complement storage word of `width` bits
holding `scale` fractional bits, so the integral bits are
`width - scale - signed`. Widths follow N1169 as clang and gcc resolve them
and are not target-varying: a `_Fract` is 8 bits at rank `short` and doubles
per rank, an `_Accum` is twice as wide as the `_Fract` of its rank, and an
unsigned type spends the sign bit on one more fractional bit (there is no
unsigned padding bit, matching clang's default). `short _Fract` is
`fixed<i8, 7>`, `_Accum` is `fixed<i32, 15>`, `unsigned _Accum` is
`fixed<u32, 16>`, `long long _Accum` is `fixed<i128, 63>`. Storage and
alignment are the storage integer's, so a fixed-point value passes in the ABI
as a scalar.

`ArithSema::FixedPoint` carries `overflow` (`saturate` for a `_Sat` type,
`ub` otherwise -- saturation is not integer overflow), `rounding`, which is
always `toward_zero` for the fractional bits an operation discards, and the
`by_zero` and `amount_out_of_range` policies where they apply. It is used by
`+ - * /`, `<< >>`, and unary `-`. `%`, the bitwise operators, and `~` are
rejected; a fixed-point value is a scalar condition, so `if`, `!`, `&&` and
`||` compare it against a zero of its own type.

Conversions are `int_to_fixed`, `fixed_to_int`, `float_to_fixed`,
`fixed_to_float`, and `fixed_convert`, each carrying the destination's
saturation and rounding, except that a `fixed_convert` keeping every
representable value of the source (no fewer fractional bits, no fewer
integral bits, and not signed to unsigned) is `Exact`. `fixed_to_float`
carries the ordinary floating rounding and exception policy instead.

The common type of two fixed-point operands takes the wider kind (`_Accum`
over `_Fract`) and the greater rank, is signed if either is, and saturating
if either is; a fixed-point operand absorbs an integer operand and yields to
a real floating one. Clang instead computes the common type from the maximum
integral and fractional bit counts, which can name a type that is wider than
either operand; we model the result at the C type level, which is what N1169
describes and is enough for translation. A fixed-point operand mixed with a
complex or imaginary operand is rejected. `tests/fixtures/clang/linux/x86_64/ir_fixed_point.c`
and `ir_fixed_point_invalid.c` pin these forms.

N1169 fixed-point literal suffixes (`r`/`k`, with optional `u` and
`h`/`l`/`ll`) are parsed into the same fixed-point type metadata. Decimal
digits are converted directly to the IR storage integer at the destination
scale, truncating discarded fractional bits toward zero. The literal spelling
remains in the AST for diagnostics. Hexadecimal fixed-point literals and
overflow diagnostics remain unsupported.

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
Fixture: `sema/ir_globals_linkage.c`.

**Declaration attribute classification (`dyd.4`):** every attribute written on
an object or typedef declaration is classified once, by the exhaustive
`sema::attributes::declaration_use`, into `Symbol` (folded into
`SymbolAttributes`), `Layout` (consumed by type resolution or the object
request), `Ignored` (no meaning the IR needs to carry), or `Unsupported` with a
reason string. Before this there were three blanket refusals -- "typedef
attributes or asm label", "declaration attribute", "automatic variable
attributes or asm label" -- which rejected a module over `[[deprecated]]` or
`may_alias` just as readily as over something that changes layout. The
classification is what decides; `Ignored` covers diagnostic-only attributes
(`deprecated`, `nodiscard`, `maybe_unused`, `warn_unused_result`, unknown and
vendor attributes), aliasing hints the IR does not model (`may_alias`), and
function attributes written where they have no object meaning. `Unsupported`
names what is missing instead: `machine mode attribute`,
`address space attribute`, `transparent union attribute`, `scalar storage order
attribute`, `record layout attribute`, `ifunc attribute`, `code segment
attribute`. Functions are not classified this way -- `function_symbol` keeps its
own filter and `record_function` retains the rest as `c_attributes` metadata.

An `asm("sym")` label is ignored on a typedef and on an automatic local, which
is what clang does; a register label (`register int x asm("eax")`) still gives
`register asm label`, because the register binding is real and dropping it
would be silently wrong.

Alignment written on a *typedef* is refused with `typedef alignment attribute`
rather than dropped. It belongs to the aliased type -- clang propagates it to
every object, field and `_Alignof` of that alias, and `aligned(1)` on a typedef
*lowers* alignment where the same attribute on a declaration cannot -- but
`ir::Type` is structural and desugars typedefs away, so there is nowhere to put
it without a type-model change (`slate-parser-dyd.30`). Fixture:
`sema/ir_declaration_attributes.c`.

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
On a windows-msvc target (the Microsoft ABI, under any flavor) neither mode
applies unless the function is `gnu_inline`: clang gives every C inline
definition a comdat of its own — `weak_odr` when `dllexport` or any
redeclaration is `extern`, otherwise `linkonce_odr`, emitted on first use —
so none is inline-only (`sema/x86_64-pc-windows-msvc/ir_inline.c`).

`always_inline` and `noinline` resolve into the preference enum independently of
emission; contradictory preferences are diagnosed. `_Noreturn`, `[[noreturn]]`,
and the GNU attribute merge across declarations into the same flag. Reaching the
end of such a body is `fallthrough=ub`. Function, parameter, and object
declaration attributes also survive as `c_attributes` metadata, including
attributes not otherwise interpreted by sema; retention does not implement
attribute-specific ABI or optimization behavior.
Integer constant expressions in expression-valued attributes are folded before
they are retained, so metadata carries their resolved integer values.

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
releases an outer one. The selection/iteration statement's own scope (around
its controlling expression) is gated by `control_statement_scopes`: under
C89/GNU89 an `enum { T = 1 }` in an `if`/`while`/`switch` condition stays
visible after the statement and hides a file-scope `typedef ... T`; under C99+
it ends with the statement. Fixture: `sema/control_body_scopes.c`. `typedef int T[n];` captures `n` once at the typedef,
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
  partially overlapping range is split so the members stay disjoint: the parts
  outside the new designation keep their old value, the parts inside are
  reinitialized. Only the scalars an initializer actually reaches are
  overwritten, so `{[2 ... 4]... = 1, [2] = 2}` leaves the rest of `[2]` alone
  while `{... , [2] = {2}}` (braced) replaces it whole.
- A designation moves the current object to the subobject it names, so
  following items without designators continue *inside* it and then walk back
  out one level at a time: `{[1][0] = 1, 2, 3}` on `int[3][2]` fills
  `[1][0], [1][1], [2][0]`. A range designation replays the whole remaining
  path over every part of the range — each element merging with what it
  already holds — but the continuation lands only in the range's last element.
- `T a[] = ...` completes the array length from the last initialized
  element. `char`-like arrays from string literals (also `{"..."}`) stay
  `CodeUnits` on the declared array type, zero-padded or truncated to length.
- Compound literals lower to `PlaceKind::CompoundLiteral { object, storage,
initializer }`, printed `compound_literal %id [storage=..] = <initializer>`.
  Each literal gets a fresh `BindingId` (its own object identity) and its type
  is the initializer value's type, so `(int[]){1,2}` is `array<i32, 2>`.
  Storage is `Static` outside a function body and `Automatic` inside; values
  read or decay from the place like any other object.
- Layout constants — bit-field widths, array bounds, array designators and
  `aligned`/`_Alignas` — all fold through `TypeResolver::constant_integer`,
  which is target-aware and sees predefined macros, prior enumerators and
  `constexpr` objects. The token-level folder in `const_expr.rs` is for the
  preprocessor and knows none of those; it is not used for layout.
  Attribute operands are name-resolved like any other expression, so
  `__attribute__((aligned(sizeof(x))))` finds `x`.
- The folder (`sema/fold.rs`) goes past C's integer-constant-expression
  rule, which admits a floating constant only as a cast's immediate operand,
  and folds binary floating arithmetic, conversions, comparisons and
  conditionals like clang, gcc and cl do (`enum { E = (int)(2.5 * 2) }`). Each
  operation rounds to nearest-even in its own format, so `0.1f + 0.2f == 0.3f`
  is 1 and `0.1 + 0.2 == 0.3` is 0. Decimal floating values never fold. A
  float-to-integer conversion that is out of range (undefined) folds per
  flavor: clang saturates and maps NaN to 0, msvc wraps an integer part below
  2^64 and folds anything else to 0, gcc does not fold it
  (`tests/fixtures/clang/linux/x86_64/ir_float_constant_folding.c`).
- A `constexpr` object whose initializer folds is declared as an ordinary
  constant, so it is usable as an integer constant expression
  (`constexpr int w = 7; int a[w];`).
- `__real__`/`__imag__` accept a real operand, as GCC does: `__real__ x` is
  the promoted operand and `__imag__ x` is zero of that type.
- A member access on a record rvalue (`f().x`, `(s, t).x`) materializes the
  base into `PlaceKind::Temporary { object, initializer }`, printed
  `temporary %id = <value>`, and projects the field from it. The temporary
  gets a fresh `BindingId` and the value's type. It is not an object: a place
  rooted in a temporary (through fields, complex parts, lanes or swizzles, but
  not through a `deref` of a pointer read out of it) rejects assignment with
  `expression is not assignable` and `&` with `address of a temporary`.
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
- A GNU cast to a union type (`(union U)x`, clang's `ToUnion`) lowers to the
  rvalue `aggregate<@U>(fieldN = x)`, with no conversion node. `N` is the
  first named member whose unqualified type is exactly `x`'s after lvalue,
  array and function conversion. No other conversion applies, so a `long` or
  an `enum` operand does not match an `int` member and the cast is rejected.
  Casting from the same or a compatible union stays an ordinary copy. Like
  clang, a bit-field member can match; gcc rejects that.
- A trailing flexible array member, or (GNU) an incomplete array member at any
  position in a union, has size 0 and the element's alignment in
  the record layout. Omitted, it is skipped by `zero_fill` and absent from the
  members. Initialized (`{1, {2, 3}}`, elided `{1, 2, 3}`, or `.d = {..}`),
  it appears as a normal `Field` member whose value has a sized
  `array<T, N>` type. The variable and aggregate keep the declared record
  type; the object's extent is the record size plus that member's size for a
  struct, or the larger of the two for a union, so consumers read it from the
  initializer rather than the type.

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
`tests/fixtures/error/clang/linux/x86_64/ir_modifiable_lvalue.c`.

### Pointer comparisons

`==`/`!=` and the relational operators accept a pointer against a null pointer
constant, and a `void *` against any object pointer, silently. Comparing
pointers whose pointees have no composite type warns
`compare-distinct-pointer-types`; comparing a pointer against an integer that
is not a null pointer constant warns `pointer-integer-compare`. Both are
warnings in clang, gcc and MSVC alike. Fixture:
`tests/fixtures/clang/linux/x86_64/ir_pointer_comparison.c`.

### Conditional operator

6.5.15p3-6, in order: two arithmetic operands go through the usual arithmetic
conversions; if one operand is a null pointer constant the result is the other
operand's type; otherwise two pointers merge into a pointer to the composite
type carrying the union of both pointee qualifier sets, with `void *` winning
over an object pointer. `(void *)0` is a null pointer constant, so
`c ? (int *)0 : (void *)0` is `int *`, not `void *` — verified against clang
22 and gcc 16. Fixture: `tests/fixtures/clang/linux/x86_64/ir_conditional_composite.c`.

The GNU omitted-middle form `a ?: b` evaluates `a` once and uses it as both
the truth test and the then-operand. It lowers to a `capture<%id>` around the
`conditional`, the same let-binding node VLA extents use, with the test and
the then-operand both reading `%id`; everything after that is the ordinary
conditional walk above, so the result type is still the usual-arithmetic or
composite-pointer type of `a` and `b`. This is the IR's answer to clang's
`BinaryConditionalOperator`/`OpaqueValueExpr` pair. Fixture:
`tests/fixtures/clang/linux/x86_64/ir_gnu_conditional.c`.

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
record and fixed-array elements retain their structural types. Incomplete and
void elements, incompatible pointers, and noninteger offsets are rejected.
GNU function-pointer arithmetic is accepted as an extension; the function type
is retained as the element type, and function addresses use one-byte offset
units. GNU void-pointer arithmetic remains unsupported.

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
| `_Bool b = p`, `(_Bool)p` | `ne(p, null)`                                   |
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
explicit cast is silent. A pointer converted to `_Bool`, implicitly or by cast,
is the comparison against null C 6.3.1.2 specifies: `ne(p, null)`, never a
warning (`tests/fixtures/clang/linux/x86_64/ir_pointer_to_bool.c`). A null pointer constant (an integer constant
expression 0, or such an expression cast to `void *`) becomes `null<ptr<T>>`
rather than a converted integer, so it never warns.

Fixtures: `tests/fixtures/clang/linux/x86_64/ir_pointer_sign.c`,
`tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c`,
`tests/fixtures/clang/linux/x86_64/ir_conversion_rules.c`.

Original pointer qualifiers are retained as metadata; volatile/atomic
access behavior is also resolved on the actual accesses. Pointee `const`
is shown in the type (`ptr<const T>`) since Rust distinguishes it.

### MS mixed-size pointers

Under MS extensions, `__ptr32`, `__ptr64`, `__sptr` and `__uptr` give a pointer
a representation other than the target's own. `Type::Pointer` carries it as
`space: PointerSpace`, printed as a trailing `ptr<T, space>` argument and
named after clang's address spaces:

| space        | clang addrspace | width | widened to 64 bits |
| ------------ | --------------- | ----- | ------------------ |
| `ptr32_sptr` | 270             | 32    | sign-extended      |
| `ptr32_uptr` | 271             | 32    | zero-extended      |
| `ptr64`      | 272             | 64    | n/a                |

Which space a declaration gets follows clang's rule on the target pointer
width. On 64-bit targets, `__ptr32` gives `ptr32_uptr` with `__uptr` and
`ptr32_sptr` otherwise, and `__ptr64` is the plain pointer. On 32-bit targets,
`__ptr64` gives `ptr64`, `__uptr` gives `ptr32_uptr` even without `__ptr32`,
and `__ptr32 [__sptr]` is the plain pointer. Anything else is the default
space. A modifier on a non-pointer, `__ptr32` with `__ptr64`, and `__sptr`
with `__uptr` are errors. Through a typedef, a modifier applies to the
typedef's pointer (`P __ptr32`).

A non-default space is part of the C type, so it matters for `_Generic`,
redeclarations and composite types. Converting between spaces is implicit,
without a diagnostic, and lowers to `address_space_cast`: it truncates when
narrowing and extends when widening, zero-extending from `ptr32_uptr` and
sign-extending otherwise. Comparisons convert the right operand to the left
operand's pointer type, and `?:` produces the default space, both like
clang. `ptr_to_int` from a narrow pointer zero-extends, like LLVM
`ptrtoint`. Storage and ABI chunks use the space's width
(`TargetInfo::pointer_storage`).

In the msvc flavor, `__sptr`/`__uptr` is not part of type identity: cl.exe
selects `int *__ptr32` for an `int *__ptr32 __uptr` in `_Generic` and accepts
redeclaring one as the other. `CTypes::ptr32_extension_is_qualifier` makes the
two ptr32 spaces compatible there. The conversion between them still lowers
to `address_space_cast`.

Fixtures: `ms_mixed_pointers.c` under `tests/fixtures/clang/windows/x86_64/`,
`tests/fixtures/msvc/windows/x86_64/` and `tests/fixtures/msvc/windows/i686/`,
and `ms-*-conflict.c` and `ms-pointer-modifier-non-pointer.c` under
`tests/fixtures/error/clang/windows/x86_64/`.

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
  `tests/fixtures/clang/linux/x86_64/ir_atomic_layout.c` (clang) and
  `tests/fixtures/gcc/linux/x86_64/ir_atomic_layout.c` (gcc).

  `--flavor=msvc` has a third rule, from MSVC's non-standard
  `/experimental:c11atomics` opt-in. There is no power-of-two promotion: a
  value whose size is already a lock-free width (1, 2, 4 or 8) keeps that
  size and is aligned to it, and anything else gets a leading four-byte lock
  word, so the object lays out exactly like `struct { int lock; T value; }` —
  size `round_up(max(4, alignof(T)) + sizeof(T), max(4, alignof(T)))` at
  alignment `max(4, alignof(T))`. So `char a[3]` is 8/4 where clang says 4/4
  and gcc 3/1, `char a[16]` is 20/4 where clang says 16/16, and
  `struct { double d; int i; }` (16/8) is 24/8. Because the lock leads the
  object, `struct { char head; _Atomic struct { char a[3]; } value; char
  tail; }` is 16 bytes with the value at offset 4 and the tail at 12.
  Measured against cl.exe 19.51 `/std:c17 /experimental:c11atomics` on
  x86_64-pc-windows-msvc; fixture
  `tests/fixtures/msvc/windows/x86_64/ir_atomic_layout.c`.

  We model only the size and alignment, not a hidden lock field: C forbids
  reaching a member of an atomic aggregate, so the interior offsets are not
  observable, and everything that is — `sizeof`, `_Alignof`, and the offsets
  of enclosing members — comes out of the layout alone.

  One measured MSVC inconsistency is deliberately not reproduced. The
  over-alignment in the lock-free branch depends on how `_Atomic` is spelled:
  `_Atomic struct S` (size 4, natural alignment 1) is 4/4, but `_Atomic T`
  and `_Atomic(T)` through a typedef of the same struct are 4/1. The lock
  branch agrees across all three spellings. Layout cannot see the spelling,
  and an 8-byte lock-free atomic at alignment 1 cannot actually be accessed
  atomically, so we take the elaborated-specifier answer for every spelling.

  The personalities also disagree on how an atomic aggregate *argument* is
  passed, and that is settled separately (`slate-parser-lh7.2.29`, closed):
  `AbiOperand` carries an `atomic` flag taken from the C qualifiers, because
  the lowered `ir::Type` has lost them — `_Atomic struct S` is the same
  `Type::Defined` as `struct S`. Under clang an atomic record or complex
  argument is MEMORY on SysV64 and x86 cdecl regardless of size; under gcc it
  classifies as the unqualified record. On `win64` the lock-prefixed
  layout is simply what gets classified, so an atomic aggregate can land in a
  register where the plain record goes indirect: `struct { char a, b, c; }` is
  3 bytes and indirect, `_Atomic` of it is 8 and passes in one register.
  Returns agree. Measured against cl.exe 19.51 `/experimental:c11atomics`,
  including the sizes copied for the indirect cases (12 for a 5-byte record,
  32 for a 24-byte one), which is what pins the parameter object as the
  lock-prefixed one. On AArch64, clang classifies atomic records and complex
  values as integer values or indirect objects using their qualified layout;
  MSVC does this for atomic records. GCC uses the ordinary record or complex
  ABI, including HFA passing.

`tests/fixtures/clang/linux/x86_64/ir_qualified_access.c`,
`tests/fixtures/clang/linux/x86_64/ir_array_parameter.c` and
`tests/fixtures/clang/linux/x86_64/ir_atomic_layout.c` cover these.

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
- `__atomic_fetch_uinc`/`udec` (`atomicrmw uinc_wrap`/`udec_wrap`, and note
  the builtin names carry no `_wrap`) are integer-only counters that saturate
  against the operand rather than wrapping at the type's width. They need no
  arithmetic op of their own, because the compare-and-select form states them
  exactly: `uinc` is `ge(old, v) ? 0 : add(old, 1)` and `udec` is
  `logical_or(eq(old, 0), gt(old, v)) ? v : sub(old, 1)`, with `overflow=wrap`
  on the step, which the guard already makes unreachable. Both compares are
  unsigned whatever the object's signedness, so a signed object reinterprets
  both operands, as `__sync_fetch_and_umin/umax` does.
- Not every family spells every operation. `min`/`max` have no `__sync_`
  spelling beyond the explicit `fetch_and_[u]min/max` names, and the floating
  extrema and `uinc`/`udec` exist only as `__atomic_fetch_<op>` — there is no
  `__atomic_uinc_fetch` or `__c11_atomic_fetch_fminimum`. Names outside a
  family's set are not atomic builtins at all and fall through to ordinary
  name resolution, matching clang's "call to undeclared function".
- A `_Bool` object's fetch arithmetic runs in its storage byte, as `u8`:
  clang emits `atomicrmw add ptr, i8`, which is the only way a `_Bool` can
  come to hold a byte outside `{0, 1}`. The place keeps its `ptr<bool>`
  spelling while the operation is `update<u8, ...>`, the same reinterpretation
  `__atomic_test_and_set` already does for a `void *` flag. The value
  parameter still has the object's declared type, so the operand converts to
  `bool` first and then widens (`from_bool<u8>(ne<i32>(v, 0))`, clang's
  `icmp ne` plus `zext`), and the result converts back to `bool`. Reading an
  object that already holds a byte above 1 is the one place this differs from
  clang, which truncates to the low bit where the conversion back asks `!= 0`;
  such an object is a trap representation and its use is undefined. Only the
  arithmetic fetches need this view — `load`, `store`, `exchange`,
  `test_and_set` and `clear` cannot put a `_Bool` out of range, so they stay
  `bool` operations.
- The `__scoped_atomic_*` family is the `__atomic_*` one with a trailing
  synchronization-scope argument, and lowers to the same nodes with
  `sync_scope=` on them: `system` (never printed, since it is the default and
  what every unscoped builtin means), `device`, `workgroup`, `wavefront`,
  `single`, `cluster`, or `dynamic(v)` for a non-constant scope, which clang
  turns into a run-time switch exactly as it does for a non-constant ordering.
  The scope is recorded as written, not as the target would lower it: on a CPU
  target clang maps every scope to the system scope and emits no `syncscope`,
  while on `amdgcn` the same source gives `syncscope("agent")`,
  `syncscope("workgroup")` and so on. Keeping the C scope leaves that mapping
  to whoever consumes the IR. The family covers `load`/`load_n`,
  `store`/`store_n`, `exchange`/`exchange_n`, `compare_exchange`/`_n`, the
  fetch operations and `thread_fence`; it has no `signal_fence`,
  `test_and_set`, `clear`, `init` or lock-free query, so those spellings are
  not builtins at all.
- `__sync_*` builtins are all `seq_cst` except `lock_test_and_set`
  (acquire, as gcc documents it; clang strengthens it to `seq_cst`) and
  `lock_release` (release). Trailing "protected variable" arguments are
  ignored. `__sync_fetch_and_min/max` compare signed and `umin/umax`
  unsigned, whatever the object's signedness, through `reinterpret`
  conversions of both operands. On a pointer, an integer operand offsets in
  bytes, as with `__atomic_*` (gcc; clang requires a pointer operand). gcc's
  legacy size-suffixed spellings (`__sync_fetch_and_add_1` through `_16`, and
  the same for `sub`/`and`/`or`/`xor`/`nand`, both fetch forms,
  `val`/`bool_compare_and_swap`, `lock_test_and_set`, `lock_release` and
  `swap`) lower identically to the unsuffixed ones: clang ignores the suffix
  and takes the width from the pointee, so `__sync_fetch_and_add_8` on a
  `char` object is an `i8` update. `min`/`max`/`umin`/`umax` and
  `synchronize` have no sized spellings.
- `__c11_atomic_is_lock_free(n)`, `__atomic_is_lock_free(n, p)` and
  `__atomic_always_lock_free(n, p)` follow clang's constant evaluator: true
  when `n` is a power of two no wider than the target's max inline atomic
  width (`TargetInfo::max_atomic_inline_bytes`: 16 on AArch64 and on x86-64
  with `cx16`, else 8), and `n == 1`, `p` is null, or `p`'s pointee is
  aligned to at least `n`. Otherwise `always_lock_free` is false and the
  other two call libatomic's `bool __atomic_is_lock_free(size_t, const
  volatile void *)`, declared on first use.

`tests/fixtures/clang/linux/x86_64/ir_atomic_builtins.c`, `ir_atomic_stdatomic.c`,
`ir_atomic_sync.c`, `ir_atomic_sync_gcc.c`, `ir_atomic_extensions.c`,
`ir_atomic_scoped.c` and `ir_atomic_lock_free.c` (plus the AArch64 and
`-mcx16` variants) cover these.

## Things C leaves implicit that the IR materializes

- C99 and later `main` falling off the end → `fallthrough=ret_zero` on its definition.
- A void function falling off the end → `fallthrough=ret_void`; a non-void function reached at the end has `fallthrough=ub_if_used`. A valueless `return` in a non-void function
  prints as `return` with the same meaning. It is accepted only where the
  compiler accepts it: under gcc in C89/gnu89, and under MSVC, which gives
  only warning C4033 (per Microsoft's docs; not measured). Clang rejects it in
  every mode.
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
- Struct and union copy on initialize/assign/pass/return → `copy<T, reason=...>` around the source value. Under C23 the source can be a different record type that is compatible with `T` (same tag and members, defined in another scope), so the operand's type is not always `T`.

## Worked example

```c
#include <stdio.h>
int add(int a, int b) { int c = a + b; return c; }
int main(void) { printf("%d\n", add(2, 3)); }
```

`tests/fixtures/clang/linux/x86_64/ir_add.c` is the executable version; it includes
`tests/fixtures/clang/linux/x86_64/add.c`. The excerpts omit the target header and the string
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
