# Type Family Touchpoints

<!-- toc -->
- [Layers](#layers)
- [Caught by the compiler](#caught-by-the-compiler)
- [Not caught by the compiler](#not-caught-by-the-compiler)
- [Order of work](#order-of-work)
- [Precedent](#precedent)
<!-- /toc -->

A type family is a value type with its own representation and arithmetic:
complex, imaginary, vector, fixed-point. This page is the sema/IR half;
the AST half is [ast-enum-touchpoints](ast-enum-touchpoints.md). Update it
when a match site over `ir::Type` or `CTypeKind` is added or removed.

## Layers

| Layer | Type | Home |
| --- | --- | --- |
| Source | `TypeSpecifier` | `src/ast.rs`, `src/parser/declarator.rs` |
| Semantic | `CTypeKind` | `src/sema/ctype/mod.rs` |
| Lowered | `ir::Type` | `src/ir/numeric.rs` |
| Target | `StorageLayout` | `src/target_info.rs` |
| Printed | `Display for Type`, `CTypes::name` | `src/ir/numeric.rs`, `src/sema/ctype/render.rs` |

`CTypeKind` keeps the source shape (kind, rank, signedness, sugar) for
metadata; `ir::Type` keeps only the resolved shape (width, scale, lanes,
component).

## Caught by the compiler

Found by adding a dummy variant and collecting `E0004`; repeat the probe if
this list looks stale. Handling these makes a declaration lower, not any
operation correct.

- `ir::Type`: `Display for Type` (`ir/numeric.rs`), `storage_of`
  (`target_info.rs`), `abi_pass` (`sema/abi.rs`), `numeric()`
  (`sema/numeric.rs`).
- `CTypeKind`: `ir_type` and `numeric()` (`sema/ctype/layout.rs`), `name`
  (`sema/ctype/render.rs`).
- `TypeSpecifier`: `base()` (`sema/types.rs`), plus the AST sites.
- `ir::FloatType` (a format): `float_to_integer` (`sema/fold.rs`),
  `float_rank` (`sema/ctype/arith.rs`). Not caught: `Display`,
  `exact_integer_bits`, `exponent_bits` (`ir/numeric.rs`), the storage
  table (`target_info.rs`), `format_apfloat` dispatch (`ir/mod.rs`), the
  `storage` header list in `ir/module_print.rs` (omits decimal formats and
  bf16). `widens_from` compares mantissa and exponent widths, not the
  derived `Ord`, because bf16 and f16 are incomparable.
- `FloatKind` (a C type over a format, e.g. `_Float32`): `float_type`,
  `float_name` (`sema/ctype`), `float_rank`'s tie class (C23: `_FloatN` >
  standard > `_FloatNx`). `default_promotion` and `promotes_to_itself`
  name only `float`; new kinds are not promoted. Target-dependent formats
  (`_Float64x`, `_Float128`) are gated by `TargetInfo::float64x_format` /
  `has_float128` in `base()`.

## Not caught by the compiler

These have `_ =>` arms that silently accept a new family.

Classification (`sema/ctype/convert.rs`):
- `is_integer`, `is_floating`, `is_fixed_point`, `is_vector`,
  `is_complex_domain`, and `is_arithmetic` / `is_scalar`. Missing from
  `is_arithmetic` means no conversions at all.
- `classify_conversion` is ordered; insert family guards (e.g. fixed-point
  with complex is rejected) before the generic arithmetic case.
- Kept as `matches!` on purpose: "no" is the right default answer.

Promotion and common type (`sema/ctype/arith.rs`):
- `integer()` returns `None` for non-integers, so `integer_promotion`
  leaves them alone.
- `usual_real_type`, `arithmetic_component`: a family with its own
  common-type rule needs its own function (model: `usual_fixed_type`).

Operand conversion (`sema/operand.rs`):
- `binary_types` is ordered: vector, shift, family-specific, then
  real/complex. Place the new branch before anything that would capture
  its operands.
- `arithmetic_type`, `arithmetic_domain`: same. The typer records the
  resulting steps; lowering and the constant folder (`folded_operand`) only
  apply them.

Arithmetic (`sema/numeric.rs`, `sema/expression.rs`):
- `emit_binary`, `emit_unary_arith` (`-`, `~`), `condition()` in
  `numeric.rs`.
- `emit_arithmetic_conversion`: `(from, to)` match with a postcondition;
  a missing pair returns `Unsupported("arithmetic conversion")`.
- `condition()` in `expression.rs`: the separate truth test for `if`,
  `?:`, `!`, `&&`, `||`. Easiest to miss.
- `type_class` (`__builtin_classify_type`): `None` is fine if gcc has no
  code.

Pointers:
- `Type::Pointer` carries a `PointerSpace` (`__ptr32`/`__ptr64`). Take
  storage from `TargetInfo::pointer_storage(space)` or `storage_of` and ABI
  widths from `space.width(..)`, never `target.pointer` /
  `target.pointer_width`.

Printing (`ir/mod.rs`):
- `format_semantics` (`ArithSema`) and the `ConversionSema` match in the
  `ValueKind::Convert` arm are exhaustive over their enums; a family that
  reuses existing variants is not caught.

## Order of work

1. AST and parser; regenerate the parse fixture.
2. `ir::Type` and storage; check bare globals with
   `slate-parser ir <file> --dump-ir`.
3. `CTypeKind`, resolution, rendering; check spelling with
   `--dump-ir --show-metadata`.
4. Conversions and arithmetic (predicates, common type, `ArithSema`,
   `ConversionKind`); probe each operator by hand, including rejections.
5. Generated fixtures `ir_<family>.c` and `ir_<family>_invalid.c`; update
   [ir/type-families](ir/type-families.md), [ir-grammar](ir-grammar.md),
   and the AST pages in the same commit. Record deliberate departures from
   clang, with the reason, in the matching `ir/` subpage.

## Precedent

- `ir/type-families.md` has one section per existing family.
- `CTypeKind::NullPtr` (`nullptr_t`) is the smallest sema-only type: it
  reuses `ir::Type::Pointer`. Silent sites it needed: `classify_conversion`
  (guard before vectors), `is_scalar`, equality lowering in
  `expression.rs` (picks the comparison type by IR pointer-ness),
  `Checker::comparison_warning`, `type_class`.
- Fixed-point is the fullest example (new `ArithSema`, five
  `ConversionKind`s, own common type): `git log --grep "lower fixed-point
  types" -p`. Cite commits by subject; the beads post-commit hook amends
  every commit, so hashes shift.
