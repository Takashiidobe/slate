# Places, pointers, and qualified access

<!-- toc -->
- [Places and values](#places-and-values)
  - [Members and bit-fields](#members-and-bit-fields)
- [Pointer arithmetic](#pointer-arithmetic)
- [Pointer conversions](#pointer-conversions)
- [`nullptr_t`](#nullptr_t)
- [MS mixed-size pointers](#ms-mixed-size-pointers)
- [Qualified access](#qualified-access)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Printed forms are in the
[grammar](../ir-grammar.md#places).

## Places and values

A place is a typed storage location; a value is a computed result.
`read(place)`, `write(place, value)`, and `addr_of(place)` consume places.
Forming a place never reads it.

- Locals are bound by `let`; a name in value position prints as a read of
  its place. No storage slot is invented per local.
- Projections: `field`, `bitfield`, `index` (arrays), `lane` and `swizzle`
  (vectors), `deref`. `p->a[i]` with an array member is
  `index(field(deref(p), a), i)`, which touches only that element.
- Bit-fields and vector lanes are readable and writable but not
  addressable.
- Reusing a place evaluates a side-effecting base or index once.

| C | IR (types and policies elided) |
| --- | --- |
| `a[i]` on an array | `read(deref(ptr_offset(array_decay(a), i)))` |
| `*p = v` | `write(deref(p), v)` |
| `p[i]`, `*(p + i)` | `read(deref(ptr_offset(p, i)))` |
| `p - q` | `ptr_diff<i64, element=T, ...>(p, q)` |
| `p < q` | `lt<ptr<T>>(p, q)` |
| `&x` | `addr_of(x)` |
| `arr` as pointer | `array_decay<ptr<T>, length=Some(N)>(arr)` |
| `f` as value | `function_decay<ptr<fn(..)>>(f)` |
| `0`, `NULL`, `(void *)0` | `null<ptr<T>>` |
| `if (p)`, `(_Bool)p` | `ne(p, null)` |
| `(uintptr_t)p` / `(T *)n` | `ptr_to_int<u64>(p)` / `int_to_ptr<ptr<T>>(n)` |
| `T *` → `U *` | `pointer_cast<ptr<U>>(p)` |

### Members and bit-fields

- `PlaceKind::Field { base, index, bits }` names a field by declaration
  index: `o->inner.x` is `field0(field0(deref(..)))`. A union member is the
  same projection onto an overlapping type; unions have no tag or
  active-member node.
- Anonymous members keep their own field index; a promoted member name
  expands during lowering into the chain through them.
- A bit-field place carries its slice: `bitfield1<unit=0, bytes=0..2,
  bits=3..8>(..)` is field 1 in storage unit 0 (record bytes 0-2), bits 3-8
  from the unit's least significant bit. The place type is the declared
  field type (it gives the read its signedness).
- A bit-field rvalue promotes by declared width, not storage type:
  `unsigned low : 3` reads as `reinterpret<i32, reason=promotion>(..)`;
  compound assignment promotes the old value the same way. As in clang's
  `getSourceBitField`, the result of `=`, compound assignment, and prefix
  `++`/`--` on a bit-field promotes the same way too (`Typed::source_bits`),
  so `(s.bf = x) > -1` compares as `i32`; postfix forms do not. `_Generic` sees
  the declared type, since it applies lvalue conversion but not promotion.
- `&`, `sizeof`, `_Alignof`, and `offsetof` on a bit-field are `Rejected`;
  zero-width bit-fields have no storage and are not members.
- A member access on a record rvalue (`f().x`) materializes
  `temporary %id = <value>` and projects from it. A place rooted in a
  temporary (not through a `deref`) is not assignable and has no address.

## Pointer arithmetic

- `PointerOffset` records the pointer, promoted amount, element type,
  add/subtract direction, and pointer-overflow policy. Offsets are in
  elements. Subtraction is a direction, so an unsigned amount is never
  negated. `+ - += -= ++ --` and indexing share it; an update keeps one
  place and an old-value computation.
- `PointerDifference` records both pointers and the element type; its
  result is pointer-width signed. Its contract requires the same array (or
  one past) and a representable result; neither is claimed proven.
- Pointee `const` differences are allowed; incomplete elements,
  incompatible pointers, and non-integer offsets are rejected.
- GNU `void *` and function-pointer arithmetic use one-byte units
  (`element=void`, or the function type).
- VLA elements stride by their runtime size (`element=vla<i32, %m>`).
- Fixtures: `ir_pointer_arithmetic.c` and its wrap variant; numeric
  contracts in `ir_operator_semantics.c`.

## Pointer conversions

Every implicit conversion between object pointers is a `pointer_cast` plus
at most a warning, never an error: MSVC only warns where clang and gcc
reject. `classify_conversion` picks the first matching row:

| Pointees | Warning |
| --- | --- |
| compatible, target drops no qualifier | none |
| either is `void` | none |
| compatible, target drops `_Atomic` | `incompatible-pointer-types` |
| compatible, target drops `const`/`volatile` | `incompatible-pointer-types-discards-qualifiers` |
| integers differing only in signedness | `pointer-sign` |
| compatible apart from nested qualifiers | `incompatible-pointer-types-discards-qualifiers` |
| anything else | `incompatible-pointer-types` |

- Plain `char` is distinct from `signed char` and `unsigned char`;
  signedness is compared at every level (`unsigned **` → `int **` warns
  `pointer-sign`).
- Integer ↔ pointer across assignment, argument, return, or initializer
  warns `int-conversion`; a cast is silent. A null pointer constant becomes
  `null<ptr<T>>` and never warns.
- Comparisons: pointer vs null constant and `void *` vs object pointer are
  silent; no composite type warns `compare-distinct-pointer-types`; pointer
  vs non-null integer warns `pointer-integer-compare`.
- Fixtures: `ir_pointer_sign.c`, `ir_pointer_conversion_warnings.c`,
  `ir_conversion_rules.c`, `ir_pointer_comparison.c`,
  `ir_pointer_to_bool.c`.

## `nullptr_t`

- A distinct sema scalar (`CTypeKind::NullPtr`), so `_Generic` can select
  it, but it lowers to `ptr<void>`; the spelling survives as
  `c_canon="nullptr_t"`. `nullptr` is `null<ptr<void>>`.
- C23 6.3.2.4: converts implicitly to any pointer and to `bool`; a null
  pointer constant converts to it; every other conversion is `Rejected`,
  cast or not. Equality with a pointer converts to the pointer type;
  relational comparison and arithmetic are rejected. As a variadic argument
  it passes as `ptr<void>`; `__builtin_classify_type` gives -1.
- Fixtures: `{clang,gcc}/linux/x86_64/ir_nullptr.c`,
  `error/.../ir_nullptr_invalid.c`.

## MS mixed-size pointers

`__ptr32`, `__ptr64`, `__sptr`, `__uptr` give a pointer a non-default
representation: `Type::Pointer.space`, printed `ptr<T, space>`.

| Space | clang addrspace | Width | Widened to 64 bits |
| --- | --- | --- | --- |
| `ptr32_sptr` | 270 | 32 | sign-extended |
| `ptr32_uptr` | 271 | 32 | zero-extended |
| `ptr64` | 272 | 64 | n/a |

- 64-bit targets: `__ptr32` is `ptr32_uptr` with `__uptr`, else
  `ptr32_sptr`; `__ptr64` is the plain pointer. 32-bit targets: `__ptr64`
  is `ptr64`, `__uptr` alone is `ptr32_uptr`, `__ptr32 [__sptr]` is plain.
- Errors: a modifier on a non-pointer, `__ptr32` with `__ptr64`, `__sptr`
  with `__uptr`. Through a typedef the modifier applies to its pointer.
- The space is part of the C type (`_Generic`, redeclarations, composites).
  Conversion between spaces is implicit and silent, lowering to
  `address_space_cast`. Comparisons convert the right operand to the left's
  type; `?:` gives the default space. `ptr_to_int` from a narrow pointer
  zero-extends. Storage and ABI use the space's width
  (`TargetInfo::pointer_storage`).
- MSVC flavor: `__sptr`/`__uptr` are not part of type identity
  (`ptr32_extension_is_qualifier`).
- Fixtures: `ms_mixed_pointers.c` under `clang/windows/x86_64`,
  `msvc/windows/{x86_64,i686}`; `error/clang/windows/x86_64/ms-*.c`.

## Qualified access

Each qualifier lives where its meaning does, on the place or memory operation
rather than the value type:

- `volatile` belongs to the place. Every place carries an `access`
  (volatile, `_Atomic` object), printed on the node that touches memory:
  `read<i32, volatile>(%g)`. Forming a place prints nothing.
- Atomicity belongs to the operation, since gcc `__atomic_*` work on plain
  objects: `read`/`write`/`store`/`update` take an optional ordering. A
  plain access to an `_Atomic` object is `atomic=seq_cst`. See
  [atomics](atomics.md).
- A place's access comes from the binding's qualifiers, the dereferenced
  pointer's pointee qualifiers, and for fields the base's access plus the
  field's own. Array elements take the array object's access; decay and `&`
  carry it back into the pointer type.
- Pointer types keep pointee `volatile`/`_Atomic` next to `const`
  (`ptr<volatile i32>`); the access through `*p` can't be recovered any
  other way.
- Declarations carry their own access as a type prefix
  (`global %1 counter: atomic i32`, `let %9 flag: volatile i32`), even if
  never accessed; Slate needs it to pick the Rust type. Redeclarations
  re-derive it from the merged type.
- Volatile non-atomic updates split into read, compute, write, both
  volatile.
- `restrict` is a `[restrict]` flag on the parameter or variable.
- A top-level `const` object is `[const]` on its `let`/`global`
  (`int *const q` is `ptr<i32> [const]`); `Type::Pointer::is_const` is the
  pointee's. `const int a[2]` and `constexpr` objects carry it too;
  `const int *p` does not.
- Typedef-inherited qualifiers and `_Atomic(T)` count as written ones.
- Array parameters: qualifiers inside the first brackets belong to the
  adjusted pointer (`int a[restrict volatile 4]` is
  `volatile ptr<i32> [restrict]`). The brackets survive as `[array=3]`,
  `[array=%n]`, `[array=*]`, or `[array=static 3]` (C99 6.7.6.3p7: at least
  that many elements, so non-null). Plain `int a[]` adds nothing.
- Fixtures: `ir_qualified_access.c`, `ir_array_parameter.c`.
