# Control flow and hints

- `__builtin_unreachable`, `__builtin_assume`, and MSVC's `__assume` communicate
  reachability or assumptions.
- `__builtin_trap` and `__builtin_debugtrap` lower trap requests.
- `__builtin_expect`, `__builtin_expect_with_probability`, and
  `__builtin_unpredictable` accept branch prediction hints.
- `__builtin_prefetch` accepts a memory prefetch hint.
- `__builtin_assume_aligned` accepts a pointer alignment assumption.
- `__builtin_cpu_init` and `__builtin_cpu_supports` support CPU feature queries
  on supported targets and feature names.
- `__builtin_return_address`, `__builtin_frame_address`,
  `__builtin_extract_return_addr`, and `__builtin_frob_return_addr` support
  stack and return-address queries where the target provides a lowering.
- `__builtin_setjmp` and `__builtin_longjmp` have a translation path for
  supported jump patterns. See [setjmp and longjmp](../../setjmp.md).
- `_exception_code`, `_exception_info`, and `_abnormal_termination` resolve
  Microsoft structured exception queries, including their double-underscore
  spellings.
- `__noop(...)` type-checks its arguments without evaluating them.

## `__builtin_expect`

```c
long likely(long value) { return __builtin_expect(value, 1); }
```

The first argument is the value of the expression. In this return expression,
the translated Rust simply returns that value.

```rust,ignore
fn likely(value: i64) -> i64 {
    value
}
```

## `__builtin_unreachable`

```c
int one_case(int x) {
    if (x == 0) return 1;
    __builtin_unreachable();
}
```

Reaching the C builtin is undefined behavior. The generated Rust preserves that
contract.

```rust,ignore
fn one_case(x: i32) -> i32 {
    if x == 0 {
        return 1;
    }
    unsafe { core::hint::unreachable_unchecked() };
    unsafe { core::mem::zeroed() }
}
```

## `__builtin_assume`

```c
void example(int x) {
    __builtin_assume(x > 0);
}
```

Slate passes the assumption to Rust. The caller must ensure `x > 0` whenever the function runs.

```rust,ignore
fn example(x: i32) {
    unsafe { core::hint::assert_unchecked(x > 0) };
}
```

## `__builtin_trap`

```c
void example(void) {
    __builtin_trap();
}
```

Slate emits a call to `llvm.trap`.

```rust,ignore
unsafe extern "llvm-intrinsic" {
    #[link_name = "llvm.trap"]
    fn __slate_intrinsic_46d8fd79c9d1f86c();
}

fn example() {
    unsafe { __slate_intrinsic_46d8fd79c9d1f86c() };
}
```

## `__builtin_prefetch`

```c
void example(const void *p) {
    __builtin_prefetch(p, 0, 3);
}
```

Slate accepts the read prefetch hint. The emitted function performs no prefetch.

```rust,ignore
fn example(p: *const ()) {}
```

## `__builtin_cpu_supports`

```c
int example(void) {
    return __builtin_cpu_supports("avx2");
}
```

Slate uses Rust's runtime feature detection for AVX2 on x86-64.

```rust,ignore
fn example() -> i32 {
    std::arch::is_x86_feature_detected!("avx2") as i32
}
```
