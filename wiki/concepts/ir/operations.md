# Operations, conversions, and floating semantics

<!-- toc -->

- [Operation policies](#operation-policies)
  - [Floating settings](#floating-settings)
  - [Floating pragmas](#floating-pragmas)
- [Conversions](#conversions)
  - [Who decides](#who-decides)
  - [Promotions stay visible](#promotions-stay-visible)
  - [`_Float16` excess precision](#_float16-excess-precision)
- [Operator rules checked in sema](#operator-rules-checked-in-sema)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Pointer operations are in
[places and pointers](places-pointers.md); per-family rules in
[type families](type-families.md). Printed forms are in the
[grammar](../ir-grammar.md#operations).

## Operation policies

Operations run on concrete widths and are never folded or reassociated.
Every one carries its UB and rounding contract explicitly. A policy states
what happens _if_ the case occurs, not that it will; `impossible` and range
proofs are analysis facts, never emitted by lowering.

| Operation                           | Policy                                                                                               |
| ----------------------------------- | ---------------------------------------------------------------------------------------------------- |
| signed `add`/`sub`/`mul`/`neg`      | `overflow=` from the context (`ub` by default; `-fwrapv`/`-ftrapv` change it)                        |
| unsigned arithmetic                 | `overflow=wrap`                                                                                      |
| signed `div`/`rem`                  | `by_zero=ub, min_by_neg_one=ub`, even under `-fwrapv`/`-ftrapv` (clang emits plain `sdiv`)           |
| unsigned `div`/`rem`                | `by_zero=ub`                                                                                         |
| `shl`/`shr`                         | `amount_out_of_range=ub`; signed `shl` also `overflow=ub` and `negative_left=ub` regardless of flags |
| `shr` fill                          | signed: `TargetInfo.signed_right_shift` (`sign_extend`); unsigned: `zero_extend`                     |
| float arithmetic                    | `rounding=`, `exceptions=`, `contract=` (see below)                                                  |
| `and`/`or`/`xor`/`not`, float `neg` | `ArithSema::Exact`: nothing printed                                                                  |
| float compare                       | only `exceptions=`: relational ops signal, `eq`/`ne` are quiet on NaN                                |

- `ValueKind::Arith` keyed by `ArithOp` (`add sub mul div rem and or xor shl
shr`); `ValueKind::Unary` (`neg`, `not`); `ValueKind::Compare` (`eq ne lt
le gt ge`), printed with the operand type and yielding `bool`.
- Shift operands promote independently; the node takes the left operand's
  type and the amount keeps its own.
- `%` and the bitwise ops reject floats.
- Floating extrema `minnum`/`maxnum` (NaN-ignoring, `-0`/`+0` unordered, C
  `fmin`), `minimum`/`maximum` (NaN-propagating, `-0 < +0`), and
  `minimum_num`/`maximum_num` (NaN-ignoring, `-0 < +0`) have no C operator;
  they exist for floating atomic fetches.
- `+x` keeps only the integer promotion. Usual arithmetic conversions
  compare C ranks, even at equal width.
- Truth: a numeric operand of `!`, `&&`, `||`, or a condition becomes
  `ne<T>(x, const<T>(0))` (pointers: `ne(p, null)`). `!` is `not<bool>`;
  `&&`/`||` are `logical_and`/`logical_or` (they short-circuit, unlike
  `and`/`or`). A `bool` used as an integer goes through `from_bool<int>`.
- Casts to `bool` use the truth comparison (fractional floats are true,
  both zeros false). `bool` → integer is 0/1; → float is `from_bool` then an
  exact `int_to_float`.

### Floating settings

- Default: `rounding=nearest_even`; `exceptions=ignore` for clang and MSVC,
  `observable` for gcc.
- `contract=` (FMA permission): `on` for clang (within an expression),
  `fast` for gcc `gnu*` (across statements), `off` for gcc ISO modes and
  MSVC (cl.exe 19.51 fuses `a * b + c` only under `/fp:contract` or
  `/fp:fast`). `-ffp-contract=off|on|fast` (clang also `fast-honor-pragmas`, same
  as `fast`) replaces the default. clang applies it in argv order with the
  fast-math umbrellas (`-ffast-math`/`-funsafe-math-optimizations` set
  `fast`; their `-fno-` restores the last `-ffp-contract=` or `on`); gcc's
  explicit flag wins regardless of order, else unsafe math gives `fast`.
  It and `range=` are recorded because a consumer can use them, not
  because ignoring them is wrong.
- `range=` (complex `mul`/`div`): `full` by default. `basic` and `improved`
  come from the complex-range flags ([compiler-flags](../compiler-flags.md)).
  Clang's `promoted` is expanded in sema: both operands convert to the next
  wider type (f16 to f32 without excess precision, f32 to f64, f64 to an
  f80/f128 long double), divide with `range=basic`, and convert back. Where
  there is no wider type (double on windows-msvc, long double) it falls back
  to `improved` and warns `higher-precision-for-complex-division`. On x86,
  `_Float16` already computes in float, so promoted is basic there.
- msvc `/fp:` (`MicrosoftFlags::floating_point`): `strict` gives
  `rounding=environment, exceptions=observable`, `except` gives
  `exceptions=observable`, `contract` gives `contract=on`, and `fast` gives
  `contract=fast` and clears the region's `precise`, so
  `float_control(except, on)` is rejected as cl.exe rejects it (C3199).
  `fast`'s other permissions are not represented
  ([compiler-flags](../compiler-flags.md)).
- Flags: `-f[no-]wrapv`, `-f[no-]trapv`, `-f[no-]strict-overflow`,
  `-f[no-]rounding-math`, `-f[no-]trapping-math`, and the long-double
  options. `pointer_wrap` (from strict-overflow) governs pointer offsets
  independently; `-fwrapv` alone doesn't change them. See
  [compiler-flags](../compiler-flags.md).
- Static and thread-storage initializers always use nearest-even with
  ignored exceptions (C F.8.5, translation-time evaluation), and `range=full`
  whatever the flags or pragma say, as clang and gcc fold them.
- Function-attribute overrides (`optimize`) are not wired in.

### Floating pragmas

`STDC FENV_ACCESS`/`FP_CONTRACT`/`CX_LIMITED_RANGE` and `float_control` are
region-scoped: sema carries a `FloatingRegion` (rounding, exceptions,
contraction, range, precise) that each compound statement saves and
restores. Clang and MSVC flavors, matching clang 22:

| Pragma                              | Effect                                                                        |
| ----------------------------------- | ----------------------------------------------------------------------------- |
| `FENV_ACCESS ON`                    | `rounding=environment, exceptions=observable`                                 |
| `FENV_ACCESS OFF`/`DEFAULT`         | nearest-even (even under `-frounding-math`), command-line exceptions          |
| `FP_CONTRACT ON\|OFF\|DEFAULT`      | `contract=on\|off\|<command line>`                                            |
| `CX_LIMITED_RANGE ON\|OFF\|DEFAULT` | `range=basic\|full\|<command line>`                                           |
| `float_control(except, on\|off)`    | exceptions observable / ignored                                               |
| `float_control(precise, on\|off)`   | `contract=on` / `contract=fast` (other fast-math permissions not represented) |

- Only uppercase values are recognized. A pragma must be at file scope or
  at the start of a compound statement; `push` forms and `pop` only at file
  scope. `FENV_ACCESS ON` and `except, on` are errors while precise is off,
  and `precise, off` while exceptions are observable.
- The parser hoists a pragma used as a statement body to just before the
  statement, so it's accepted where clang rejects it (permissive).
- gcc implements none of these (`-Wunknown-pragmas`): no effect under the
  gcc flavor.
- `STDC FLOAT_CONST_DECIMAL64 ON|OFF|DEFAULT` is gcc only (clang and msvc
  ignore it). Under `ON`, an unsuffixed decimal floating constant has type
  `_Decimal64` (`d64`). Hex constants, imaginary constants and the `d`
  suffix stay binary, and `DEFAULT` is `OFF`. Placement and scoping follow
  the rules above: a pragma after the first item of a compound statement is
  ignored, as gcc does with `-Wpragmas` (the warning is not emitted).
  `pragmas::collect` records the affected literals' `NodeId`s in one walk,
  and `TypeResolver::literal`/`literal_type` give those literals the
  `DecimalF64` suffix. The AST keeps the literal's own suffix.

## Conversions

Each conversion node does one thing; the reason is metadata.

| Node                                             | Meaning                                             | Metadata                                                               |
| ------------------------------------------------ | --------------------------------------------------- | ---------------------------------------------------------------------- |
| `widen<i32>(x)`                                  | sign/zero extend by source signedness               | `reason=promotion\|usual_arith\|assign\|arg\|vararg\|return\|explicit` |
| `truncate<i8>(x)`                                | keep low bits                                       | `fits=always\|unknown`                                                 |
| `reinterpret<u32>(x)`                            | same width, sign change                             | `fits=always\|unknown`                                                 |
| `from_bool<i32>(b)`                              | 0 or 1                                              |                                                                        |
| `bit_cast<T>(x)`                                 | `__builtin_bit_cast`; sizes equal, `T` not an array |                                                                        |
| `float_widen` / `float_narrow` / `float_convert` | exact / rounded / between binary and decimal        | rounding and exceptions on narrowing                                   |
| `int_to_float<f64>(x)`                           |                                                     | `exact=`, rounding, exceptions                                         |
| `float_to_int<i32>(x)`                           | truncates toward zero                               | `out_of_range=ub`, exceptions                                          |
| `enum_to_int` / `int_to_enum`                    | enum ↔ underlying integer                           |                                                                        |

- A change of width and signedness is two nodes: width first (in source
  signedness), then `reinterpret`. `(u32)(i8)x` →
  `reinterpret<u32>(widen<i32>(x))`.
- `fits` is filled only when trivially known (constants).
- Family conversions: [type families](type-families.md); pointer
  conversions: [places and pointers](places-pointers.md#pointer-conversions).

### Who decides

`CTypes::classify_conversion` (`src/sema/ctype/convert.rs`) decides legality
over C types and returns a `CastKind` plus an optional warning;
`Lowerer::emit_cast` only emits it. The context (`Assign`, `Arg`, `Return`,
`Cast`) separates 6.5.16.1 from 6.5.4: a cast is silent where assignment
warns. `CastKind::Identity` still emits an arithmetic conversion when the
layouts differ (a comparison typed `int` in C but `bool` in IR).

Always rejected: to or from `void`, to a function or array type, record ↔
unrelated type, pointer ↔ float.

### Promotions stay visible

```c
short inc(short s) { return s + 1; }
```

```text
return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(1)));
```

`s` really is stored as `i16`, so widen and truncate stay; the metadata lets
Slate choose `wrapping_add` or retype `s` once analysis proves every store
fits.

### `_Float16` excess precision

Clang evaluates `_Float16` (and `_Complex _Float16`) `+ - * /` in `float`
and rounds to half once, at the end of a chain of such operators, on every
target except aarch64, whose default CPUs have native half arithmetic
(`TargetInfo::float16_excess_precision`). Sema spells this out: each
operator widens its half operands with `reason=excess_precision`, computes
in `f32`/`complex<f32>`, and narrows the result with the same reason. An
operand that is already such a narrow is unwrapped, so `a * b + c` rounds
once:

```text
float_narrow<f16, reason=excess_precision>(add<f32>(mul<f32>(float_widen<f32, reason=excess_precision>(read<f16>(%1)), ...), ...))
```

Unary `-` joins a chain only when its operand is one. Comparisons, stores,
and explicit casts see the rounded half value: a same-type cast retags the
narrow as `reason=explicit`, which ends the chain like clang's
`(_Float16)(a * b)`.

## Operator rules checked in sema

- Modifiable lvalues (6.3.2.1p1): assignment, compound assignment, and
  `++`/`--` reject an array, a `const` lvalue, or a record with a
  (recursively) `const` member (`TypeResolver::require_modifiable_lvalue`,
  `ir_modifiable_lvalue.c`).
- Conditional operator (6.5.15p3-6, `ir_conditional_composite.c`):
  - two arithmetic operands: usual arithmetic conversions;
  - a null pointer constant takes the other operand's type, so
    `c ? (int *)0 : (void *)0` is `int *`;
  - two pointers: pointer to the composite type with the union of pointee
    qualifiers, `void *` winning over an object pointer. gcc-only
    exceptions (`conditional_pointers`): it drops `_Atomic`, and before C23
    a pointer to an array of `const` contributes no qualifier;
  - a `void` operand makes the result `void` (clang and gcc extension); the
    other arm lowers as `sequence(operand, void)`.
- GNU `a ?: b` evaluates `a` once: a `capture<%id>` around the
  `conditional`, both the test and the then-arm reading `%id` (clang's
  `BinaryConditionalOperator`, `ir_gnu_conditional.c`).
