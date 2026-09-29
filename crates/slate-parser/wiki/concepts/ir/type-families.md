# Complex, imaginary, vector, and fixed-point

<!-- toc -->
- [Complex](#complex)
- [Imaginary (C23 Annex G)](#imaginary-c23-annex-g)
- [Vector](#vector)
- [Fixed-point (N1169)](#fixed-point-n1169)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Four value-type families, each a
structural `ir::Type` variant (like pointer and array) rather than a
`NumericType`, so representation and arithmetic are visible without reading
metadata. Source spelling and typedefs stay in C type metadata. To add a
family, follow [type-family-touchpoints](../type-family-touchpoints.md).
Their ABI passing is in [calls and ABI](calls-abi.md).

## Complex

- `Type::Complex(NumericType)` prints `complex<f64>`. Storage is two
  adjacent components with the component's alignment. GNU integer complex
  is supported.
- Lowered: declarations, scalar↔complex and complex↔complex conversions
  (the component converts, with its contract on the operation), `+ - * /`,
  `== !=`, truth tests, `-`, and `__real__`/`__imag__` reads and writes.
- Mixed scalar/complex arithmetic keeps the scalar operand (it matters for
  `*` and `/`).
- `ArithSema::ComplexFloating` carries rounding, exceptions, and `range=`
  (`full` unless `CX_LIMITED_RANGE` gives `basic`); `ComplexInteger`
  carries overflow and division-by-zero policy.
- GNU `~` on complex is conjugation for float and integer components (gcc
  and clang agree), printed `not<complex<..>>`
  (`ir_complex_conjugate.c`).
- `__real__`/`__imag__` accept a real operand, as gcc does: `__imag__ x` is
  zero of the promoted type.
- `{re, im}` initializes as `aggregate<complex<T>>(index0 = re, index1 =
  im)`; `{x}` and a bare scalar stay `real_to_complex`. Brace elision into
  the pair is not modeled. `__builtin_complex(re, im)` gives the same
  aggregate.
- Fixture: `ir_complex.c`.

## Imaginary (C23 Annex G)

Neither clang nor gcc accepts `_Imaginary`, so these follow the standard
text with no oracle.

- `Type::Imaginary(FloatType)` prints `imaginary<f64>`; binary real float
  components only. Storage, alignment, and ABI are the component's.
- Conversions (G.4): `real_to_imaginary` and `imaginary_to_real` give +0
  and discard the value; `imaginary_to_complex` sets a +0 real part;
  `complex_to_imaginary` keeps the imaginary part; `imaginary_convert`
  changes the component with narrowing rules. Truth tests compare against
  `const<imaginary<fN>>(0.0)`.
- Binary operators apply only the common real type; each operand keeps its
  domain. Results with a complex result carry `complex=true`:

  | Operator | real/imag | imag/imag | any complex |
  | --- | --- | --- | --- |
  | `*` `/` | imaginary | real | complex |
  | `+` `-` | complex | imaginary | complex |
  | `==` `!=` | bool | bool | bool |

- Relational operators, `%`, bitwise, shifts, and `~` reject imaginary;
  unary `-` is exact. `?:` gives imaginary for two imaginary operands and
  complex for mixed domains.
- GNU imaginary literals (`2.0i`, `3i`) are typed complex, as GNU does:
  `aggregate<complex<T>>(index0 = 0, index1 = value)`, the component being
  the literal's type without `i` (`5000000000i` is `complex<i64>`). They are
  not integer constant expressions.
- Fixtures: `ir_imaginary.c`, `ir_imaginary_invalid.c`.

## Vector

- `Type::Vector { element, lanes }` prints `vector<i32, 4>`.
  `vector_size(N)` divides by the element size (must be a positive
  multiple); `ext_vector_type(N)` is the lane count. Elements are integers
  (`_BitInt` included) or binary floats; `_Bool` and decimal are rejected.
- Storage is `next_power_of_two(lanes * element_size)` for both size and
  alignment (clang's rounding), so lanes can't be recovered from `sizeof`.
- Arithmetic is per lane and prints `elementwise=true` with the element's
  scalar contract, except integer lanes always wrap (clang emits no `nsw`).
  `%`, bitwise, and shifts need integer elements.
- Comparisons yield a lane mask: a signed integer vector with the element's
  storage size (`vector<f64, 2>` → `vector<i64, 2>`, `f80` → `i128`),
  printed `result=`. A vector is not a condition: `if (v)`, `!v`, `&&`,
  `||` are rejected.
- Lax conversions as in clang: the left operand fixes the result type; a
  same-size vector is `vector_bit_cast`; a scalar converts to the element
  and is `vector_splat`. Same for assignment, arguments, return, and casts;
  different total sizes are an error. Clang rejects a truncating splat
  (`v4si + double`); we accept it and record the conversion (permissive).
- Braced initializers fill lanes like array elements, omitted lanes zeroed.
- `v[i]` on an lvalue is the place `lane(place, i)`; on an rvalue it is the
  value `lane<T>(v, i)`. Lanes are not addressable; the index is a runtime
  value.
- `ext_vector` components (`x/y/z/w`, `r/g/b/a`, hex `sN`,
  `lo`/`hi`/`even`/`odd` over the lane count rounded up to a power of two):
  one component is a `lane` place, several distinct in-range ones a
  `swizzle<lanes=[..]>` place, repeated or out-of-range ones a non-assignable
  one-operand `shuffle<..>` value. Accepted on `vector_size` vectors too,
  since `Type::Vector` doesn't record the spelling (permissive).
- `__builtin_shufflevector` is a two-operand `shuffle<..>` (`-1` prints
  `undef`); the two-argument runtime-mask form prints `mask=dynamic(..)`.
  `__builtin_convertvector` prints the scalar conversion over vector types
  (`int_to_float<vector<f32, 4>>(..)`) with the same policies.
- Fixtures: `ir_vector.c`, `ir_vector_invalid.c`.

## Fixed-point (N1169)

- `Type::FixedPoint` prints `fixed<i32, 15>` (`sat_fixed<..>` for `_Sat`):
  a `width`-bit two's-complement word with `scale` fractional bits. Widths
  are not target-varying: `_Fract` is 8 bits at `short` rank and doubles per
  rank, `_Accum` is twice its rank's `_Fract`, and unsigned types spend the
  sign bit on one more fractional bit (no padding bit, as clang).
  `short _Fract` = `fixed<i8, 7>`, `_Accum` = `fixed<i32, 15>`,
  `unsigned _Accum` = `fixed<u32, 16>`, `long long _Accum` =
  `fixed<i128, 63>`. Storage and ABI are the storage integer's.
- `ArithSema::FixedPoint`: `overflow=saturate|ub`, `rounding=toward_zero`
  for discarded bits, plus `by_zero`/`amount_out_of_range` where they apply.
  Used by `+ - * /`, shifts, and unary `-`. `%`, bitwise, and `~` are
  rejected. Conditions compare against a zero of the same type.
- Conversions: `int_to_fixed`, `fixed_to_int`, `float_to_fixed`,
  `fixed_to_float`, `fixed_convert`, each with the destination's saturation
  and rounding. A `fixed_convert` that keeps every value is `Exact`;
  `fixed_to_float` uses the float policy.
- Common type: the wider kind (`_Accum` over `_Fract`), the greater rank,
  signed if either is, saturating if either is; it absorbs integers and
  yields to real floats. Clang computes it from max integral and fractional
  bits instead, which can be wider than either operand; we follow N1169's
  C-type rule. Mixing with complex or imaginary is rejected.
- Literals (`r`/`k` with `u` and `h`/`l`/`ll`) convert decimal digits
  directly to the storage integer, truncating toward zero. Hex fixed-point
  literals and overflow diagnostics are not implemented.
- Fixtures: `ir_fixed_point.c`, `ir_fixed_point_invalid.c`.
