# Function contracts and compiler hints

Slate accepts these attributes as annotations. They do not by themselves
generate Rust diagnostics, checks, or optimization directives.

- `annotate("TEXT")` records a Clang annotation string.
- `availability(...)` accepts a Clang platform availability annotation.
- `format(...)` records the format family, format argument, and first argument
  to check. Slate checks that the attribute applies to the type.
- `format_arg(...)` records an argument that supplies a format string.
- `lifetimebound` accepts a Clang lifetime annotation.
- `nonnull` or `nonnull(N, ...)` records non-null expectations. Slate checks
  whether the attribute applies to the declaration's type.
- `returns_nonnull` records a non-null return expectation.
- `sentinel` or `sentinel(N)` records a variadic sentinel expectation.
- `warn_unused_result` accepts the GNU unused-result annotation.
- `nothrow` records the no-unwind annotation.
- `returns_twice` accepts the returns-twice annotation.
- `cold` accepts a cold-function hint.
- `hot` accepts a hot-function hint.
- `leaf` accepts a leaf-function hint.
- `flatten` accepts the request to inline calls within a function.
- `no_split_stack` accepts a request to disable split stacks for a function.
- `noipa` accepts GCC's request to disable interprocedural optimization.
- `noclone` accepts GCC's request to prevent function cloning.
- `optimize(...)` accepts GCC's per-function optimization request.
- `optnone` accepts Clang's request to disable optimization.

## `warn_unused_result` and `cold`

```c
__attribute__((warn_unused_result, cold))
int status(void) { return 1; }
```

The generated function has neither `#[must_use]` nor `#[cold]`.

```rust
fn status() -> i32 {
    1
}
```
