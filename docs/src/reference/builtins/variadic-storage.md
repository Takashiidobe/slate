# Variadic arguments and storage

- `__builtin_va_list` selects the target's variadic list type.
- `__builtin_va_start`, `__builtin_va_arg`, `__builtin_va_copy`, and
  `__builtin_va_end` start, read, copy, and end variadic lists.
- `__builtin_c23_va_start` supports the C23 form of `va_start`.
- `__builtin_ms_va_list`, `__builtin_ms_va_start`, `__builtin_ms_va_copy`, and
  `__builtin_ms_va_end` handle Microsoft variadic lists on x86-64 and AArch64.
  `__va_start` is the Microsoft intrinsic used by UCRT headers. See
  [Variadic Calls and `va_arg`](../../lowering/variadics.md) for ABI limits.
- `alloca`, `_alloca`, `__builtin_alloca`, and `__builtin_alloca_with_align`
  provide function-scoped dynamic storage. See
  [Dynamic Stack Allocation](../../lowering/alloca.md).
- `__builtin___CFStringMakeConstantString` constructs a CoreFoundation constant
  string in Clang mode from a plain ASCII string literal, using the default
  runtime ABI.

## `__builtin_va_start`, `__builtin_va_arg`, and `__builtin_va_end`

```c
int first(int count, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, count);
    int value = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    return value;
}
```

Slate reads the first variadic argument using the Rust variadic list.

```rust,ignore
unsafe extern "C-unwind" fn first(count: i32, mut __va_args: ...) -> i32 {
    let mut ap: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
    ap = unsafe {
        core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(
            __va_args.clone(),
        )
    };
    let value: i32 = unsafe { ap.next_arg::<i32>() };
    value
}
```

## `__builtin_alloca`

```c
int first_byte(unsigned long size) {
    if (size == 0) return 0;
    unsigned char *p = __builtin_alloca(size);
    p[0] = 7;
    return p[0];
}
```

Slate uses a runtime allocation and a scope guard to give the storage a function
lifetime.

```rust,ignore
fn first_byte(size: u64) -> i32 {
    let __slate_alloca_mark = __slate_alloca::Mark::new();
    if size == 0 {
        return 0;
    }
    let p: *mut u8 = (__slate_alloca::alloca(size as usize, 16usize) as *mut ())
        as *mut u8;
    unsafe {
        *p.add(0) = 7;
    }
    unsafe { *p.add(0) as i32 }
}
```

## `__builtin_va_copy`

```c
int example(int count, ...) {
    __builtin_va_list args, copy;
    __builtin_va_start(args, count);
    __builtin_va_copy(copy, args);
    int value = __builtin_va_arg(copy, int);
    __builtin_va_end(copy);
    __builtin_va_end(args);
    return value;
}
```

Slate clones the argument list and reads the next argument from the copy.

```rust,ignore
unsafe extern "C-unwind" fn example(count: i32, mut __va_args: ...) -> i32 {
    let mut args: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
    let mut copy: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
    args = unsafe {
        core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(
            __va_args.clone(),
        )
    };
    copy = args.clone();
    let value: i32 = unsafe { copy.next_arg::<i32>() };
    value
}
```

## `__builtin_c23_va_start`

```c
int example(...) {
    __builtin_va_list args;
    __builtin_c23_va_start(args);
    int value = __builtin_va_arg(args, int);
    __builtin_va_end(args);
    return value;
}
```

In C23, the argument list can start without naming the last fixed parameter. Translate with `-std=c23`.

```rust,ignore
unsafe extern "C-unwind" fn example(mut __va_args: ...) -> i32 {
    let mut args: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
    args = unsafe {
        core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(
            __va_args.clone(),
        )
    };
    let value: i32 = unsafe { args.next_arg::<i32>() };
    value
}
```

## `__builtin_alloca_with_align`

```c
int example(unsigned long n) {
    if (!n) return 0;
    unsigned char *p = __builtin_alloca_with_align(n, 128);
    p[0] = 7;
    return p[0];
}
```

The second argument specifies alignment in bits. Slate converts it to bytes and applies the target's minimum alignment.

```rust,ignore
fn example(n: u64) -> i32 {
    let __slate_alloca_mark = __slate_alloca::Mark::new();
    if n == 0 {
        return 0;
    }
    let p: *mut u8 = (__slate_alloca::alloca(
        n as usize,
        ((128 as usize) / 8usize).max(16usize),
    ) as *mut ()) as *mut u8;
    unsafe {
        *p.add(0) = 7;
    }
    unsafe { *p.add(0) as i32 }
}
```
