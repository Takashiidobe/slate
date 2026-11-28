# Compile-time queries and types

- `__builtin_constant_p` tests whether an expression can be folded. At optimized
  levels, some scalar queries can resolve after inlining.
- `__builtin_types_compatible_p` compares two C types.
- `__builtin_choose_expr` selects an expression at compile time.
- `__builtin_classify_type` folds to the compiler's type category.
- `__builtin_offsetof` computes a member's byte offset from the target layout.
- `__builtin_bit_cast` reinterprets the bits of a supported value as another
  type of the same size.
- `__builtin_addressof` takes an object's address.
- `__builtin_counted_by_ref` retrieves a pointer to a field's associated count.
  See
  [Allocation, bounds, and cleanup](../attributes/allocation-bounds-cleanup.md).
- `__builtin_object_size` computes remaining bytes in an object or subobject.
  The mode controls which size to report and the fallback for an unknown size.
- `__builtin_dynamic_object_size` also allows a size computed at runtime.
- `__builtin_align_up` rounds an integer or pointer up to an alignment in Clang
  mode.
- `__builtin_align_down` rounds an integer or pointer down in Clang mode.
- `__builtin_is_aligned` tests alignment in Clang mode.
- `__builtin_clear_padding` zeros object padding using the resolved layout.
  Lowering covers little-endian targets.

## `__builtin_types_compatible_p`

```c
int same_type(void) { return __builtin_types_compatible_p(unsigned, unsigned int); }
```

Slate compares the C types during translation and emits a constant.

```rust,ignore
fn same_type() -> i32 {
    1
}
```

## `__builtin_offsetof`

```c
struct Header { char tag; int length; };
unsigned long length_offset(void) { return __builtin_offsetof(struct Header, length); }
```

Slate uses the record layout for the selected target.

```rust,ignore
fn length_offset() -> u64 {
    core::mem::offset_of!(Header, length) as u64
}
```

## `__builtin_object_size`

```c
unsigned long remaining(void) {
    return __builtin_object_size("hello" + 2, 0);
}
```

Mode zero measures the remaining bytes in the whole object. Four bytes remain,
including the terminator.

```rust,ignore
fn remaining() -> u64 {
    4
}
```

## `__builtin_bit_cast`

```c
unsigned bits(float x) { return __builtin_bit_cast(unsigned, x); }
```

This preserves the representation bits of the float.

```rust,ignore
fn bits(x: f32) -> u32 {
    unsafe { core::mem::transmute::<f32, u32>(x) }
}
```

## `__builtin_constant_p`

```c
int example(void) {
    return __builtin_constant_p(42);
}
```

The argument is a literal, so the query folds to `1`.

```rust,ignore
fn example() -> i32 {
    1
}
```

## `__builtin_choose_expr`

```c
int example(int a, int b) {
    return __builtin_choose_expr(1, a, b);
}
```

The constant condition selects `a`. Slate emits only the selected value.

```rust,ignore
fn example(a: i32, b: i32) -> i32 {
    a
}
```

## `__builtin_dynamic_object_size`

```c
unsigned long example(void) {
    return __builtin_dynamic_object_size("hello", 0);
}
```

This call allows a runtime size. Here the input is a string literal, so Slate can still fold the query.

```rust,ignore
fn example() -> u64 {
    6
}
```

## `__builtin_align_up`

```c
unsigned example(unsigned x) {
    return __builtin_align_up(x, 16);
}
```

Slate rounds the value up to a multiple of 16 using wrapping arithmetic and a mask.

```rust,ignore
fn example(x: u32) -> u32 {
    let __v0: u32 = 16u32.wrapping_sub(1u32);
    x.wrapping_add(__v0) & !__v0
}
```
