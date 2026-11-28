# Standard C attributes

- `[[deprecated]]` records a deprecation marker and an optional message. GCC and
  Clang also accept `deprecated` in GNU syntax.
- `[[fallthrough]]` marks an intentional fallthrough within a switch. Slate
  checks its placement. GCC and Clang also accept GNU `fallthrough`.
- `[[maybe_unused]]` records that a declaration can go unused. GNU `unused` is
  the GCC and Clang spelling.
- `[[nodiscard]]` records that a result should be used and accepts an optional
  message. GNU `warn_unused_result` is covered under
  [function contracts](contracts-hints.md).
- `[[noreturn]]` and `[[_Noreturn]]` mark a function that never returns. Slate
  also accepts the `_Noreturn` function specifier and the GNU `noreturn`
  attribute.
- `[[reproducible]]` is recognized by GCC-mode feature queries. It carries no
  function semantics in Slate.
- `[[unsequenced]]` is recognized by GCC-mode feature queries. It carries no
  function semantics in Slate.

## `[[deprecated]]` and `[[nodiscard]]`

Use `-std=c23` for the standard attribute syntax.

```c
[[deprecated("use fresh")]] int old(void) { return 1; }
[[nodiscard]] int fresh(void) { return 2; }
```

Slate records both markers. The Rust functions have neither `#[deprecated]` nor
`#[must_use]`.

```rust
fn old() -> i32 {
    1
}

fn fresh() -> i32 {
    2
}
```

## `[[noreturn]]`

```c
[[noreturn]] void stop(void);
void run(void) { stop(); }
```

Slate marks the path past the call as unreachable.

```rust
unsafe extern "C" {
    fn stop();
}

fn run() {
    unsafe { stop() };
    unsafe { core::hint::unreachable_unchecked() };
}
```

## `[[fallthrough]]`

```c
int choose(int x) {
    switch (x) {
    case 0: ++x; [[fallthrough]];
    case 1: return x;
    default: return 0;
    }
}
```

Slate checks the placement of `[[fallthrough]]`. The generated `match` preserves
the switch behavior through its arm bodies. The arm for zero is shown here.

```rust,ignore
0 => {
    x += 1;
    return x;
}
```
