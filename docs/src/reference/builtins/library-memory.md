# Library, memory, and string builtins

- `__builtin_memcpy`, `__builtin_memcpy_inline`, `__builtin_memmove`, and
  `__builtin_memset` copy, move, or fill memory.
- `__builtin_memcmp`, `__builtin_memchr`, `__builtin_mempcpy`, `__builtin_bcmp`,
  and `__builtin_bzero` cover additional memory operations.
- `__builtin_strlen`, `__builtin_strcmp`, `__builtin_strncmp`,
  `__builtin_strchr`, `__builtin_strrchr`, `__builtin_strcpy`,
  `__builtin_strncpy`, `__builtin_stpcpy`, `__builtin_strcat`,
  `__builtin_strcspn`, `__builtin_strspn`, `__builtin_strpbrk`, and
  `__builtin_strstr` cover standard string operations.
- `__builtin___memcpy_chk`, `__builtin___memmove_chk`, `__builtin___memset_chk`,
  `__builtin___stpcpy_chk`, `__builtin___strcat_chk`, and
  `__builtin___strcpy_chk` are checked forms used by fortified headers.
- `__builtin_malloc`, `__builtin_free`, `__builtin_printf`, `__builtin_sprintf`,
  and `__builtin_snprintf` resolve corresponding library operations.
- `__builtin___clear_cache` lowers through a target adapter where needed.

Library builtins can become an ordinary call to the corresponding C symbol. The
[Builtin Inventories](inventories.md) describe compiler and language-mode
coverage for additional library names.

## `__builtin_memcpy`

```c
void copy_bytes(void *dst, const void *src, unsigned long n) {
    __builtin_memcpy(dst, src, n);
}
```

Slate lowers the builtin to the corresponding memory operation.

```rust,ignore
fn copy_bytes(dst: *mut (), src: *const (), n: u64) {
    unsafe {
        core::ptr::copy_nonoverlapping(src as *const u8, dst as *mut u8, n as usize)
    };
}
```

## `__builtin_strlen`

```c
unsigned long length(const char *text) { return __builtin_strlen(text); }
```

Slate emits a C ABI declaration that links to `strlen`.

```rust,ignore
unsafe extern "C" {
    #[link_name = "strlen"]
    fn __builtin_strlen(arg0: *const i8) -> u64;
}

fn length(text: *const i8) -> u64 {
    unsafe { __builtin_strlen(text) }
}
```

## `__builtin_memmove`

```c
void *example(void *dst, const void *src, unsigned long n) {
    return __builtin_memmove(dst, src, n);
}
```

The source and destination may overlap. Slate emits the Rust pointer copy operation that allows overlap.

```rust,ignore
fn example(dst: *mut (), src: *const (), n: u64) -> *mut () {
    {
        unsafe { core::ptr::copy(src as *const u8, dst as *mut u8, n as usize) };
        dst as *mut ()
    }
}
```

## `__builtin___memcpy_chk`

```c
void *example(void *dst, const void *src, unsigned long n, unsigned long capacity) {
    return __builtin___memcpy_chk(dst, src, n, capacity);
}
```

The fourth argument supplies the destination capacity to the fortified C library call.

```rust,ignore
fn example(dst: *mut (), src: *const (), n: u64, capacity: u64) -> *mut () {
    unsafe { __builtin___memcpy_chk(dst, src, n, capacity) }
}
```

## `__builtin_printf`

```c
int example(int x) {
    return __builtin_printf("%d", x);
}
```

Slate keeps the C `printf` call and emits flushes around it to coordinate Rust and C output.

```rust,ignore
fn example(x: i32) -> i32 {
    let _ = std::io::Write::flush(&mut std::io::stdout());
    let __slate_stdout_writer_tail = unsafe { __builtin_printf(c"%d".as_ptr(), x) };
    unsafe { fflush(core::ptr::null_mut()) };
    __slate_stdout_writer_tail
}
```
