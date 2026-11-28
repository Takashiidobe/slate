# Source locations and names

- `__builtin_LINE` and `__builtin_COLUMN` return the source line and column.
- `__builtin_FILE` and `__builtin_FILE_NAME` return the source path or basename.
- `__builtin_FUNCTION` returns the enclosing function name.
- `__func__`, `__FUNCTION__`, and `__PRETTY_FUNCTION__` are predefined function
  names.

## `__builtin_LINE`

```c
int source_line(void) {
    return __builtin_LINE();
}
```

The call is on line two. Slate emits that source location as a constant.

```rust,ignore
fn source_line() -> i32 {
    2
}
```

## `__builtin_FUNCTION`

```c
const char *current_function(void) { return __builtin_FUNCTION(); }
```

Slate emits a C string containing the enclosing function name.

```rust,ignore
fn current_function() -> *const i8 {
    c"current_function".as_ptr()
}
```

## `__builtin_COLUMN`

```c
int example(void) {
    return __builtin_COLUMN();
}
```

The builtin call begins in column 12.

```rust,ignore
fn example() -> i32 {
    12
}
```

## `__builtin_FILE_NAME`

```c
const char *example(void) {
    return __builtin_FILE_NAME();
}
```

For an input named `example.c`, Slate emits its basename as a C string.

```rust,ignore
fn example() -> *const i8 {
    c"example.c".as_ptr()
}
```

## `__PRETTY_FUNCTION__`

```c
const char *example(void) { return __PRETTY_FUNCTION__; }
```

In Clang mode, the string includes the C return type and parameter list.

```rust,ignore
fn example() -> *const i8 {
    c"const char *example(void)".as_ptr()
}
```
