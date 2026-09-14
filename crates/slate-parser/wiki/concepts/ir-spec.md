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

### Implemented numeric seed

`sema::numeric::Context::resolve` lowers integer and binary floating-point
literals, parentheses, and same-concrete-type addition directly to
`ir::Value`. Integer literal selection uses the existing C candidate order
and target integer widths. Floating constants retain their exact value bits.
Addition is not folded or reassociated. Signed overflow defaults to
`undefined`, unsigned overflow to `wrap`; floating addition defaults to
nearest-even rounding with ignored exceptions. The context exposes separate
integer and floating semantic settings for future option resolution.

The initial numeric type stores integer width/signedness or floating width.
It does not yet implement the full type/storage metadata proposed below.
Mixed-type additions requiring conversions, `_BitInt`, decimal/imaginary
and target-dependent long-double literals, and other expressions return
explicit unsupported errors. Flags and scoped pragma semantics are not yet
wired into this path.

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
add<i32, overflow=undefined>(const<i32>(1), const<i32>(2))
add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(bits=0x3ff0000000000000), const<f64>(bits=0x4000000000000000))
```

### Validation and declaration pruning

**Decided:** early validation reports structural errors and may remove
structurally invalid items with diagnostics. Checks requiring scopes,
resolved types, conversions, or layout belong to `src/sema/`. Failures
there produce diagnostics; lowering must not assume that surviving early
validation proves a node valid or silently discard failed operations.

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
  declarations, promotions, or conversions.
- **Target data layout.** `TargetInfo` has integer/pointer widths and
  character signedness, but no complete object layout or calling ABI.
- **Full provenance model.** Existing spans and macro origins survive the
  numeric lowering. The expansion records proposed below remain future work.

## Module shape

```
Module
  target:    triple + data layout
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
| `to_bool(x)` / `from_bool<i32>(b)`             | `!= 0` / 0-1                                             |                                                                |
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

- `overflow=wraps|ub` (unsigned / signed). `impossible` is an analysis fact,
  not emitted by lowering.
- `div`/`rem`: `by_zero=ub`; signed also `min_by_neg_one=ub`.
- `shl`/`shr`: amount type kept separately; `amount_out_of_range=ub`; `shl` of
  a negative signed value is `ub`. Right shift of negative signed values is
  resolved by target (`shr [arithmetic]`).
- Comparisons, `!`, `&&`, `||` produce `bool`; `from_bool<i32>` is inserted
  only where the result is used as an integer.
- Scalars in boolean context lower to `ne(x, 0)` / `is_non_null(p)`.
- Short-circuit and `?:` with side-effect-free operands stay as expression
  nodes (`and`, `or`, `select`); with side effects they become `if`
  statements (see hoisting).

## Pointers

A C pointer can be a borrow, a nullable borrow, a slice cursor, an owning
handle, an out-parameter, a C string, an opaque handle or a function pointer.
**Lowering does not decide which.** It emits uniform pointer operations and
keeps every local fact as metadata for the analysis pass and Slate.

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

- `main` falling off the end → `return 0i32 [implicit=main_return]`.
- Non-void function falling off the end → `unreachable [ub]`.
- Constant `sizeof`/`_Alignof`/`offsetof` → folded value with `size_of=T`
  metadata; runtime array sizes use captured extents.
- String literals: `c"..."` typed `*const i8`, metadata `c=char[N]`, `decay[len=N]`.
- Tentative definitions and `extern` merging → one global with linkage.
- Unprototyped `f()` (pre-C23) vs `f(void)`.
- Default argument promotions for variadic calls → `widen`/`float_widen`
  with `reason=vararg`.
- Struct copy on assign/pass/return → `copy` metadata.

## Worked example

```c
#include <stdio.h>
int add(int a, int b) { int c = a + b; return c; }
int main(void) { printf("%d\n", add(2, 3)); }
```

x86_64-linux, metadata shown:

```
module target=x86_64-unknown-linux-gnu

extern fn printf(format: *const i8 [c=const char *, restrict], ...) -> i32 [c=int]
    [origin=system:<stdio.h>:170]

fn add(a: i32 [c=int], b: i32 [c=int]) -> i32 [c=int]
    [origin=user:add.c:2]
{
    let c: i32 [c=int] = add(a, b) [overflow=ub];
    return c;
}

fn main() -> i32 [c=int, entry]
    [origin=user:add.c:7]
{
    printf(c"%d\n" [c=char[4], decay[len=4], add_const],
           add(2i32, 3i32) [reason=vararg, promote=none]);
    return 0i32 [implicit=main_return];
}
```

Metadata hidden (printer flag):

```
fn add(a: i32, b: i32) -> i32 {
    let c: i32 = add(a, b);
    return c;
}

fn main() -> i32 {
    printf(c"%d\n", add(2i32, 3i32));
    return 0i32;
}
```

## Open questions

1. Hoisting `f(i++)`: always emit synthetic temps and let analysis remove
   them, or special-case during lowering.
2. What function "type parameters" represent in C.
3. Metadata printer syntax (`[k=v]` trailing per node is the working form).
