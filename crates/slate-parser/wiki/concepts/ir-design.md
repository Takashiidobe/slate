# IR Design (draft)

_created 2026-09-13 — living design doc, decisions marked **Decided** / **Open**_

Pipeline: C → AST (target-independent) → **IR (targeted)** → Rust → rewritten
Rust (Slate). This page covers the IR only. Epic: `slate-parser-lh7`.

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

- `src/ir/sema/` — type resolution, target width resolution, and metadata
  capture over AST nodes. Produces no IR nodes. Epic `slate-parser-lh7.1`.
- `src/ir/` — AST → IR lowering, IR node types, and the text printer.
  Consumes `src/ir/sema`. Epic `slate-parser-lh7.2` (blocked by lh7.1).
- `src/sema.rs` — existing AST validation; unchanged.

### Prerequisites (not present today)

- **Scopes and typing in sema.** `sema.rs` currently only validates
  declarations.
- **Target data layout.** `src/target/` has no sizes, alignments, char
  signedness, `long` width, enum underlying types or bit-field layout.
- **Macro expansion records.** `Span<T>` has `spelling`/`expansion` `Loc`s but
  no macro identity, and `ConstExpr` nodes carry no spans, so a folded
  `INT_MAX` is indistinguishable from `2147483647`. See [Provenance](#provenance).

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

- Statements: `let`, assignment/`ptr_write`, expression statement, `return`,
  `if`, `loop`/`while`, `switch`, `break`, `continue`, `goto`, label, block.
- Expressions are side-effect-free except calls. Every node has a `NodeId`
  and concrete type; metadata is looked up by `NodeId`.
- A name in value position is a read. Locals are bound by `let`; shadowed
  names are disambiguated (`c`, `c#1`).

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
ptr_write(a, i, x) [decay[len=4]];
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
  `if (p && p->n++)` → `if p != null { let t = ptr_read(p, 0).n; ptr_write … ; if t != 0 { … } }`.
- Loop conditions and `for` increments with side effects stay attached to
  their `while`/`for` as a statement block with a trailing expression, so
  they re-run per iteration without changing the loop's form.
- Assignment used as a value (`x = y = 0`) → sequential statements, with the
  value read back from the assigned target.

## Types

**Decided:** the shown type is the concrete, target-resolved type. The
original C type is metadata.

| C | Shown | Metadata |
|---|---|---|
| `int` | `i32` | `c=int` |
| `long` (LP64 / LLP64) | `i64` / `i32` | `c=long` |
| `char` / `signed char` / `unsigned char` | `i8`/`u8` per target, `i8`, `u8` | `c=char` etc. |
| `_Bool` | `bool` | `c=_Bool` |
| `size_t` | `u64` | `c=size_t`, `c_canon=unsigned long` |
| `float` / `double` | `f32` / `f64` | `c=float` / `c=double` |
| `long double` (x86) | `f80` | `c=long double` |
| `__float128` | `f128` | |
| `const char *` | `*const i8` | `c=const char *` |
| `enum E` | `enum E` (underlying `u32`) | underlying type computed per compiler |
| `struct S` | `struct S` | layout in module |

The whole typedef chain is kept in metadata (`uint32_t` → `__uint32_t` →
`unsigned int`) since it's the strongest idiomization signal
(`size_t` → `usize`, `c_int` in FFI signatures).

### Records

Layout is computed during lowering: field offsets, padding, alignment,
bit-field storage units. Source field order and names kept; anonymous
members get a synthesized name plus `anonymous` metadata.

## Provenance

**Decided:** every value is both concrete and optionally traceable.

```
Origin {
  loc:       spelling + expansion Loc
  header:    Provenance (outermost header, System/User)
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

| Node | Meaning | Metadata |
|---|---|---|
| `widen<i32>(x)` | value-preserving sign/zero extend (by source signedness) | `reason=promotion\|usual_arith\|assign\|arg\|vararg\|explicit` |
| `truncate<i8>(x)` | keep low bits | `fits=always\|unknown` |
| `reinterpret<u32>(x)` | same width, sign change | `fits=always\|unknown` |
| `to_bool(x)` / `from_bool<i32>(b)` | `!= 0` / 0-1 | |
| `float_widen<f64>(x)` / `float_narrow<f32>(x)` | | |
| `int_to_float<f64>(x)` | | `exact=true\|false` |
| `float_to_int<i32>(x)` | | `out_of_range=ub` |

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

**Decided:** memory access through a pointer or array is an explicit
read/write at an offset, in element units.

| C | IR | Metadata |
|---|---|---|
| `a[i]` (read) | `ptr_read(a, i)` | `form=index`, `decay[len=N]` if `a` is an array |
| `a[i] = v` | `ptr_write(a, i, v)` | same |
| `*p` | `ptr_read(p, 0)` | `form=deref` |
| `*p = v` | `ptr_write(p, 0, v)` | `form=deref` |
| `*(p + i)` | `ptr_read(p, i)` | `form=deref_offset` |
| `p + i`, `p++` | `ptr_offset(p, i)` / `p = ptr_offset(p, 1)` | elem type |
| `p - q` | `ptr_diff(p, q)` → `i64` | elem type, `c=ptrdiff_t` |
| `p < q` | `ptr_lt(p, q)` | |
| `&x` | `addr_of(x)` | |
| `arr` as value | `arr` typed `*T` | `decay[len=N]` |
| `f` as value | `f` typed `*fn(..)` | `decay=function` |
| `0`, `NULL`, `(void*)0` | `null<*T>` | `macro=NULL` if applicable |
| `if (p)`, `!p` | `is_non_null(p)` / `is_null(p)` | |
| `char* → const char*` | (no node) | `add_const` |
| `void* ↔ T*` | `ptr_cast<*T>(p)` | `implicit` |
| `(uintptr_t)p` / `(T*)n` | `ptr_to_int<u64>(p)` / `int_to_ptr<*T>(n)` | |

Pointer qualifiers are metadata: `restrict`, `volatile`, pointee `const` is
shown in the type (`*const T`) since Rust distinguishes it.

**Open:** member access through pointers. Candidates:

- Projection path on read/write: `ptr_read(p, 0).f`, `ptr_write(p, 0, .f[i], v)`
  — one node per access, MIR-like.
- Separate `field_ptr(p, f)` producing `*T` then `ptr_read(…, 0)` — uniform
  but verbose for `p->a.b[i]`.

Leaning toward projection paths, since `p->x = 5` should be one write.

## Things C leaves implicit that the IR materializes

- `main` falling off the end → `return 0i32 [implicit=main_return]`.
- Non-void function falling off the end → `unreachable [ub]`.
- `sizeof`/`_Alignof`/`offsetof` → folded value with `size_of=T` metadata.
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

1. Member access through pointers: projection paths vs `field_ptr`.
2. Hoisting `f(i++)`: always emit synthetic temps and let analysis remove
   them, or special-case during lowering.
3. What function "type parameters" represent in C.
4. Metadata printer syntax (`[k=v]` trailing per node is the working form).
