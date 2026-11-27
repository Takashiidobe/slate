# Expressions and Idioms

An expression rewrite works with the Rust type and context chosen by lowering.
It can remove syntax that is redundant in that context.

## Remove a literal cast

**Before**

```rust
let n: i32 = 7i32 as i32;
```

**After**

```rust
let n: i32 = 7;
```

The initializer context supplies the type. A cast that changes the value or
supplies a type needed for inference still does work. For example, a narrowing
integer cast can change the bits and a floating conversion can change rounding.
See the
[literal cast fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/literal_cast_fold.c).

## Use a pointer method for a null comparison

The `null_compare` rule can express an ordinary raw pointer null comparison with
`is_null()`. A target comparison that must use `slate_support::opaque_is_null`
already has a different operation in the tree. That helper keeps the check
observable under the C null pointer flag. See
[Translation Unit Configuration](../parsing/configuration.md#how-a-flag-changes-rust-output).

Some idioms come directly from IR. For example, lowering emits `+=` when a
write adds to a read of the same place and the access has the appropriate
semantics.
