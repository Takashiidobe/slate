# Objects, initialization, and VLAs

<!-- toc -->
- [Principles](#principles)
- [Aggregate initializers](#aggregate-initializers)
- [String literals](#string-literals)
- [Compound literals and
  temporaries](#compound-literals-and-temporaries)
- [Record copies](#record-copies)
- [Flexible array members](#flexible-array-members)
- [Variable-length arrays](#variable-length-arrays)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Object identity, storage duration, and
initialization are required semantics, not optional metadata.

## Principles

- Uninitialized storage differs from zero initialization. Semantic zero
  initialization (omitted subobjects, static storage) is explicit; it is
  never an assumed all-zero byte pattern, and an uninitialized automatic
  object never gets a synthesized zero.
- Aggregate initialization stays structured, with designators resolved to
  field indices and element indices or ranges, not expanded into stores.
  Brace and designator spelling is source context.
- `int b = a` initializes from `a`'s current value; later writes are
  `write` statements, not changes to the initializer.
- Compound literals and string literals have their own object identity.

## Aggregate initializers

Braced and string initializers lower to `ValueKind::Aggregate { members,
zero_fill }` (`sema/initializer.rs`), printed
`aggregate<T, zero_fill=..>(field0 = v, index2 = v, index3..=5 = v)`.

- Members are sorted by target: `Field(index)`, `Index(i)`, or
  `Range { start, end }` (inclusive, GNU `[a ... b]`). Designators, brace
  elision, anonymous-member paths, and `.a.x = 1, .a.y = 2` merging are
  resolved; nested subobjects are nested aggregates.
- `zero_fill` is true when any initializable member (named fields and
  anonymous records; unnamed bit-fields skipped) or element was omitted.
  Unions carry only the selected member and never `zero_fill`.
- Current object, as clang: a designation moves into the named subobject;
  following undesignated items continue inside it, then walk back out one
  level at a time (`{[1][0] = 1, 2, 3}` on `int[3][2]` fills `[1][0]`,
  `[1][1]`, `[2][0]`). A designator whose value is a bare expression
  continues brace elision (`.a = 1, 2, 3` fills `a[0..3]`).
- A range replays the remaining designator path over every part of the
  range, each merging with what it holds; the continuation lands only in
  the range's last element.
- A later initializer for the same target replaces the earlier one; a
  partially overlapping range is split so members stay disjoint. Only the
  scalars an initializer reaches are overwritten: `{[2 ... 4] = .., [2] =
  2}` keeps the rest of `[2]`, while a braced `[2] = {2}` replaces it whole.
- The checker runs the same walk over types
  (`TypeResolver::check_initializer`); lowering's `convert_element` fails
  `Internal` if its target disagrees, so change both together.
- Excess items are dropped with a warning, as gcc and clang do. An unbraced
  item aimed at a zero-length aggregate is consumed as its whole initializer
  (`int a[][0] = {1, 2}` is `array<array<i32, 0>, 2>`). gcc's quirks for
  zero-size element types (`int[][0][2]`) are not reproduced.
- A braced scalar uses its first item; `{}` is `aggregate<T,
  zero_fill=true>()` on the scalar.
- `T a[] = ...` takes its length from the last initialized element;
  `T a[] = {}` is GNU `array<T, 0>`. `sizeof` in `static_assert` uses
  `TypeResolver::inferred_array_length`, the same walk without checking.
- `char`-like arrays from a string (or `{"..."}`) stay `code_units` on the
  declared array type, zero-padded or truncated.
- GNU cast to union `(union U)x` is `aggregate<@U>(fieldN = x)` with no
  conversion node; `N` is the first named member whose unqualified type is
  exactly `x`'s after lvalue conversion (so `long` doesn't match `int`). A
  bit-field member can match, as in clang (gcc rejects).

## String literals

A string literal is an internal `.strN` global holding
`code_units<array<i8, N>>(..)`, used through
`array_decay<ptr<i8>, length=Some(N)>`. `char s[] = "abc"` is a separate
writable array initialized from the units, not a pointer to the literal.
The richer three-view design is in [open design](open-design.md#string-literal-views).

## Compound literals and temporaries

- `PlaceKind::CompoundLiteral { object, storage, initializer }`, printed
  `compound_literal %id [storage=..] = <init>`. Each has a fresh
  `BindingId`; its type is the initializer's (`(int[]){1,2}` is
  `array<i32, 2>`). Storage is static outside a function, automatic inside.
- `PlaceKind::Temporary` materializes a record rvalue for member access
  ([places](places-pointers.md#members-and-bit-fields)).

## Record copies

Struct and union copies on initialize, assign, pass, and return are
`copy<T, reason=...>` around the source. Under C23 the source may be a
different but compatible record type (same tag and members, another scope).

## Flexible array members

Omitted, a flexible array member is skipped by `zero_fill` and absent from
the members. Initialized (`{1, {2, 3}}`, elided `{1, 2, 3}`, or `.d = {..}`),
it is an ordinary `Field` member whose value is a sized `array<T, N>`. The
variable keeps the declared record type; the object's extent is the record
size plus that member (struct) or the larger of the two (union), so read it
from the initializer, not the type.

## Variable-length arrays

- Declaration: each non-constant bound is captured left to right as a
  synthetic `size_t` `Statement::Temporary`, and the type is
  `vla<T, %extent>` (`int (*p)[m]` is `ptr<vla<i32, %m>>`). It decays like
  an array with `length=None`. `sizeof` is a runtime `mul` of the captured
  extents, never a re-evaluation of the bound.
- Parameters: a definition captures each non-constant bound once at entry,
  in parameter order (C11 6.9.1p10), including the outermost one that the
  pointer adjustment drops (`int a[n][m]` is `ptr<vla<i32, %m>>`).
  Prototype scope has no extents, so `[*]` and bounds in non-defining
  prototypes or function types are `vla<T, *>`
  (`variable_length_array_parameters.c`).
- Type names in expressions (`sizeof(int[n])`, `(int (*)[n])p`) wrap the
  expression in `capture<%id>(extent, value)`, one per bound, outermost
  first; hoisting turns each into a temporary at that point, so a capture
  under `?:` runs only in its arm. `sizeof` captures only when the named
  type is itself a VLA (`sizeof(int (*)[n])` is constant). Casts capture
  every bound before the operand. `_Alignof` evaluates nothing
  (`variable_length_array_type_names.c`).
- `typedef int T[n];` captures `n` once; every later `T` shares the extent
  even if `n` changes. The alias sits in the flat type table and may name
  a function-local extent (`variable_length_array_typedefs.c`).
- The only valid VLA initializer is C23 `{}`, lowering to
  `aggregate<vla<T, %e>, zero_fill=true>()`. Static VLAs are invalid.
- VLA types are compatible with each other and with fixed arrays of
  compatible elements (C11 6.7.6.2p6), so assignment between them is a
  `pointer_cast`.
