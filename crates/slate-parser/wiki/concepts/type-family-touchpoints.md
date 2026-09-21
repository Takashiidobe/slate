# Type Family Touchpoints

_created 2026-09-20_

A **type family** is a kind of value type with its own representation and its
own arithmetic rules: complex, imaginary, vector, and fixed-point are the four
that exist. Adding one is not adding a `TypeSpecifier` variant — it is a walk
down five layers, most of which the compiler will not remind you about.

[[ast-enum-touchpoints]] covers the AST half and stops at `TypeSpecifier`
(parsing, validation, reachability, rendering). This page is the sema/IR half:
everything between "the parser produces the specifier" and "the IR prints and
the ABI passes it". See [[architecture_single_configuration]] for the pipeline
these layers sit in.

Update this page whenever a match site over `ir::Type` or `CTypeKind` is added
or removed, or a new type family appears.

## The layer chain

A family has to exist at every one of these, in this order:

| Layer         | Type                              | Home                                  |
| ------------- | --------------------------------- | ------------------------------------- |
| Source        | `TypeSpecifier`                   | `src/ast.rs`, `src/parser/declarator.rs` |
| Semantic      | `CTypeKind`                       | `src/sema/ctype/mod.rs`               |
| Lowered       | `ir::Type`                        | `src/ir/numeric.rs`                   |
| Target        | `StorageLayout`                   | `src/target_info.rs`                  |
| Printed       | `Display for Type`, `CTypes::name` | `src/ir/numeric.rs`, `src/sema/ctype/render.rs` |

`CTypeKind` keeps the source shape (kind, rank, signedness, sugar) so the C
spelling can be reconstructed for metadata. `ir::Type` keeps only what the
target resolved (width, scale, lane count, component). Both are needed: the
first answers "what did the header say", the second answers "what does this
compile to".

## What the compiler catches

Verified by adding a dummy variant to each enum and reading the `E0004` list.

A new `ir::Type` variant fails the build at four sites:

- `src/ir/numeric.rs` — `Display for Type`, the printed IR spelling.
- `src/target_info.rs` — `storage_of`, size and alignment for the target.
- `src/sema/abi.rs` — `abi_pass`, register vs. memory classification.
- `src/sema/numeric.rs` — the `numeric()` helper, which decides what counts as
  a plain numeric operand and rejects everything else.

A new `CTypeKind` variant fails at three:

- `src/sema/ctype/layout.rs` — `ir_type`, the `CTypeKind` to `ir::Type` map.
- `src/sema/ctype/layout.rs` — `numeric()`, the component type used for
  complex components and vector elements.
- `src/sema/ctype/render.rs` — `name`, the C spelling used in metadata.

A new `TypeSpecifier` variant fails at `src/sema/types.rs` `base()`, the
`TypeSpecifier` to `CTypeKind` map, on top of the AST-side sites in
[[ast-enum-touchpoints]].

A new `ir::FloatType` variant — a float *format*, not a family — fails at only
one site, `src/sema/fold.rs` `float_to_integer`. Everything else it needs is a
non-exhaustive list you have to find: `Display`, `exact_integer_bits` and
`exponent_bits` in `src/ir/numeric.rs`, the storage table in
`src/target_info.rs`, the constant printer's `format_apfloat` dispatch in
`src/ir/mod.rs`, `float_rank` in `src/sema/ctype/arith.rs`, and the hardcoded
`storage` header list in `src/ir/module_print.rs` (which omits the decimal
formats and bf16). `FloatType` derives `Ord`, but `widens_from` deliberately
does **not** use it: it compares mantissa and exponent width, because bf16 and
f16 are incomparable and declaration order would call one a widening of the
other.

Handling those makes a declaration lower. It does **not** make any operation on
the type correct.

## What the compiler does not catch

Every site below has a `_ =>` arm that will accept a new family and do
something wrong with it, silently. This is the list that is expensive to
rediscover.

**Classification predicates, which gate conversions**

- `src/sema/ctype/convert.rs` — `is_integer`, `is_floating`, `is_fixed_point`,
  `is_vector`, `is_complex_domain`, and the `is_arithmetic` / `is_scalar`
  built on them. A family missing from `is_arithmetic` cannot be converted to
  or from anything: `classify_conversion` falls through to "incompatible or
  unsupported conversion".
- `src/sema/ctype/convert.rs` — `classify_conversion` itself is ordered, not
  exhaustive. Guards run before the generic arithmetic case, so a rejection
  (for example fixed-point mixed with complex) has to be inserted at the right
  point in that sequence.

**Promotion and common type**

- `src/sema/ctype/arith.rs` — `integer()` returns `None` for a non-integer,
  which is what makes `integer_promotion` leave the type alone. Usually
  correct by default; check it rather than assume it.
- `src/sema/ctype/arith.rs` — `usual_real_type` and `arithmetic_component`
  implement the usual arithmetic conversions for integers and floats. A family
  with its own common-type rule needs its own function (`usual_fixed_type` is
  the model) and a branch that reaches it.

**Operand conversion, where the branch order matters**

- `src/sema/operand.rs` — `binary_operand` picks the result type through an
  ordered chain: vector, then shift, then family-specific, then the general
  real/complex domain logic. A new family needs its branch placed where its
  operands cannot be captured by an earlier one.
- `src/sema/operand.rs` — `arithmetic_operands` (used where both operands are
  converted to a common type) and `arithmetic_domain` need the same treatment.

**The arithmetic contract**

- `src/sema/numeric.rs` — `emit_binary` dispatches per family before falling
  through to scalar arithmetic; `emit_unary_arith` does the same for `-` and
  `~`; `condition()` builds the compare-against-zero that makes a value a
  truth value.
- `src/sema/numeric.rs` — `emit_arithmetic_conversion` is the big `(from, to)`
  match that emits every `ConversionKind`. Exhaustiveness is not available
  over pairs, so it ends in a postcondition instead: if the match did not
  produce a value of the requested type, it returns
  `Unsupported("arithmetic conversion")` rather than handing back the operand
  at its source type. A missing pair is a clean error, not a miscompile — but
  it is still a missing pair, and only you can write the arm.
- `src/sema/expression.rs` — `condition()` **again**. This is a second,
  separate truth-test path (the one `if`, `?:`, `!`, `&&` and `||` reach) and
  it rejects anything it does not recognise as a scalar condition. Missing it
  is the single easiest mistake here.
- `src/sema/expression.rs` — `type_class`, the `__builtin_classify_type` code.
  Returning `None` is a legitimate answer for a family gcc has no code for.

**Printing the new policies**

- `src/ir/mod.rs` — `format_semantics` prints `ArithSema`, and the
  `ConversionSema` match inside the `ValueKind::Convert` arm prints conversion
  policies. Both are exhaustive over their own enums, so adding a variant to
  `ArithSema` or `ConversionSema` *is* caught; adding a family that reuses an
  existing variant is not.

## Order of work

Each step builds and is worth verifying before the next:

1. **AST and parser.** Add the specifier fields, then regenerate the parse
   fixture: `python3 tools/update_filecheck.py --in-place <fixture>`.
2. **`ir::Type` and target storage.** Now a declaration lowers. Check with
   `slate-parser ir <file> --dump-ir` on bare globals before writing any
   expression.
3. **`CTypeKind`, resolution, rendering.** Check the C spelling with
   `--dump-ir --show-metadata`.
4. **Conversions and arithmetic.** Predicates, common type, `ArithSema`,
   `ConversionKind`. Probe each operator by hand — including the ones that
   must be *rejected* — before generating a fixture.
5. **Fixtures and docs.** A `tests/fixtures/sema/ir_<family>.c` and an
   `ir_<family>_invalid.c`, both generated, never hand-written. Then
   `ir-spec.md`, `ir-grammar.md`, and the AST pages in the same commit.

When sema knowingly departs from clang, say so in `ir-spec.md` in the same
change, with the reason. Input is assumed to already compile, so a deliberate
simplification is fine; an unrecorded one reads as a bug later.

## Precedent

Four families exist, all following this walk: complex and imaginary
(`Type::Complex`, `Type::Imaginary`), vector (`Type::Vector`), and fixed-point
(`Type::FixedPoint`). `ir-spec.md` has a section per family covering the shape
decision and what is implemented.

Fixed-point is the most recent and the most complete worked example, including
a new `ArithSema` variant, five new `ConversionKind`s, and its own common-type
rule:

```
git log --grep "lower fixed-point types" -p
```

Refer to commits by subject, not by hash. The beads post-commit hook in
`.beads/hooks` re-exports `issues.jsonl` and amends, so every hash shifts once
after it is created and a hash written into the commit that mentions it is
always stale.

## Process note

This map was reconstructed while adding fixed-point lowering
(slate-parser-lh7.2.17.4). The "compiler catches" list is empirical, not
remembered: it came from adding a throwaway variant to `ir::Type`, `CTypeKind`
and `TypeSpecifier` and collecting the `E0004` errors. Repeat that probe if you
suspect this page has drifted — it takes a minute and is more trustworthy than
grep.

Three sites moved from the silent list to the caught list afterwards, once the
map made clear what they cost: `base()` and `numeric()` became exhaustive, and
`emit_arithmetic_conversion` gained its postcondition. The predicates were left
as `matches!` deliberately — a new family answering "no" to "is this an
integer?" is correct, and ~170 arms of mechanical `=> false` would train the
next reader to add theirs without thinking. Exhaustiveness is worth it where
the answer for a new variant needs thought, not everywhere it is possible.
