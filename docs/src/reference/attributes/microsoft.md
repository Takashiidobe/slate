# Microsoft spellings

Use `--flavor=msvc` for the Microsoft examples. Clang also recognizes a fixed
set of `__declspec` names with Microsoft extensions enabled. See the
[compiler registry](registry.md) for recognition rules.

- `__declspec(align(N))` requests alignment like GNU `aligned(N)`.
- `__declspec(allocate("NAME"))` assigns a section like GNU `section`.
- `__declspec(deprecated)` accepts deprecation and an optional message.
- `__declspec(dllexport)` marks a symbol for export.
- `__declspec(dllimport)` marks an imported symbol.
- `__declspec(naked)` marks a function as naked.
- `__declspec(noinline)` records a never-inline preference.
- `__declspec(noreturn)` records that a function never returns.
- `__declspec(selectany)` marks an eligible external definition as discardable.
- `__declspec(thread)` gives an object thread-local storage.
- `[[msvc::noinline]]` records a never-inline preference.
- `[[msvc::forceinline]]` records an always-inline preference.
- `__declspec(noalias)` and `__declspec(restrict)` accept memory and return
  aliasing annotations. Slate does not emit Rust aliasing guarantees for them.
- `__declspec(nothrow)` accepts the no-unwind annotation.
- `__declspec(cpu_dispatch(...))` and `__declspec(cpu_specific(...))` accept
  declarations without emitting runtime dispatch.
- `__declspec(code_seg("NAME"))` parses the requested code section. Slate
  reports it as unimplemented.

Slate registers these additional `__declspec` names in Clang mode for
compatibility. They have no translation effect.

- `allocator` identifies an allocation routine for tooling.
- `no_init_all` opts out of automatic initialization requested by the compiler.
- `property` names accessor methods for a Microsoft property.
- `safebuffers` opts out of buffer security checks.
- `uuid` associates a GUID with a type.
- `empty_bases` requests more compact C++ empty-base layout on Windows.
- `guard` controls Windows control flow guard behavior.
- `layout_version` selects a Windows layout compatibility version.
- `novtable` suppresses vtable initialization for applicable C++ classes on
  Windows.

## `__declspec(noinline)`

Translate this example with `--flavor=msvc --target=x86_64-pc-windows-msvc`.

```c
__declspec(noinline) int inc(int x) { return x + 1; }
```

```rust
#[inline(never)]
fn inc(x: i32) -> i32 {
    x + 1
}
```

## `__declspec(thread)`

```c
__declspec(thread) int attempts = 0;
int bump(void) { return ++attempts; }
```

The generated static carries `#[thread_local]`. The Rust excerpt shows the
storage declaration.

```rust
#[thread_local]
static mut attempts: i32 = 0;
```

## `__declspec(code_seg)`

```c
__declspec(code_seg(".text$custom")) int inc(int x) { return x + 1; }
```

Slate stops before producing Rust and reports
`not implemented: code segment attribute`.
