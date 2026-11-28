# Objects and symbols

- `alias("SYMBOL")` aliases a function or object to an assembler symbol.
- `common` requests common linkage for an eligible tentative definition.
- `nocommon` overrides common symbol emission for an object.
- `dllimport` marks an imported Windows symbol.
- `dllexport` marks a Windows symbol for export.
- `ifunc("RESOLVER")` names the resolver for an indirect function. Slate lowers
  this on ELF targets.
- `retain` asks the linker to keep a symbol.
- `section("NAME")` assigns a function or object to a named section.
- `selectany` marks an external definition as discardable when another
  definition exists. GCC's GNU spelling is Windows-only.
- `tls_model("MODEL")` records the addressing model for a thread-local object.
- `used` keeps an otherwise unreferenced symbol in the generated output.
- `visibility("KIND")` records symbol visibility. Clang mode warns and ignores
  unknown kinds. GCC mode rejects them.
- `weak` marks a symbol as weak. Backend support depends on the target and
  whether the item is a definition or an external reference.
- `weak_import` accepts the Clang weak-import annotation. Slate does not lower
  it as a weak linker reference.
- `weakref("SYMBOL")` redirects an internal declaration's uses to a weak
  reference. Bare `weakref` can take its target from `alias("SYMBOL")`.

## `section`

```c
__attribute__((section(".text.compute"), used))
int compute(int x) { return x + 1; }
```

`section` becomes a Rust link section. `used` keeps the function in the
translation.

```rust
#[unsafe(link_section = ".text.compute")]
fn compute(x: i32) -> i32 {
    x + 1
}
```

## `used`

```c
static int value __attribute__((used, section(".data.stats"))) = 1;
int read_value(void) { return value; }
```

For a static object, Slate can emit `#[used(compiler)]` alongside the section.

```rust
#[used(compiler)]
#[unsafe(link_section = ".data.stats")]
static mut value: i32 = 1;

fn read_value() -> i32 {
    unsafe { value }
}
```

## `alias`

```c
int real(int x) { return x + 1; }
extern int alias_name(int) __attribute__((alias("real")));
int call_alias(int x) { return alias_name(x); }
```

When the definition is available in the same translation unit, this call
resolves directly to the aliased function.

```rust,ignore
fn call_alias(x: i32) -> i32 {
    real(x)
}
```
