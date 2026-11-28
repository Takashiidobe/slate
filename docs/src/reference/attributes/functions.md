# Function behavior and targeting

- `always_inline` records an always-inline preference.
- `noinline` records a never-inline preference. When both preferences occur on a
  function, Slate keeps the first one.
- `gnu_inline` uses GNU inline definition emission rules.
- `const` records that a function does not access memory.
- `pure` records that a function can read memory without writing it.
- `constructor` registers a startup function. An optional integer sets its
  priority.
- `destructor` registers a shutdown function. An optional integer sets its
  priority.
- `naked` requests a function without a compiler-generated prologue or epilogue.
  Slate lowers supported assembly bodies. GCC mode registers it on x86 and
  Arm32.
- `interrupt` marks a handler. Slate emits the Rust x86 interrupt ABI for
  supported x86 handlers. Parser recognition also covers some non-x86 targets.
- `overloadable` enables Clang-style overload resolution. Slate resolves calls
  to supported overloads. Externally linked overload sets can require name
  mangling that Slate does not implement.
- `target("FEATURES")` selects per-function target features. Slate records them
  in IR and maps supported features to Rust.
- `target_clones(...)` accepts a function multiversioning request. Slate does
  not generate the clones or runtime dispatch.
- `cpu_dispatch(...)` accepts Clang's CPU dispatch declaration. Slate does not
  emit CPU dispatch.
- `cpu_specific(...)` accepts Clang's CPU-specific variant declaration. Slate
  does not emit CPU-specific multiversioning.

## `noinline`, `always_inline`, and `target`

```c
__attribute__((noinline)) int inc(int x) { return x + 1; }
__attribute__((always_inline)) int twice(int x) { return x * 2; }

__attribute__((target("avx2")))
int avx_inc(int x) { return x + 1; }
```

On x86-64 Linux, the inlining hints become `#[inline(never)]` and
`#[inline(always)]`. The AVX2 request becomes a Rust target feature on an unsafe
function.

```rust
#[inline(never)]
fn inc(x: i32) -> i32 {
    x + 1
}

#[inline(always)]
fn twice(x: i32) -> i32 {
    x * 2
}

#[target_feature(enable = "avx2")]
unsafe fn avx_inc(x: i32) -> i32 {
    x + 1
}
```

## `constructor`

```c
int ready;
__attribute__((constructor)) static void boot(void) { ready = 1; }
```

On ELF, Slate emits a wrapper for `boot` and places a pointer to it in the
startup section. This excerpt shows the registration.

```rust,ignore
#[allow(non_upper_case_globals)]
#[used(compiler)]
#[unsafe(link_section = ".init_array")]
static __SLATE_INIT_boot: Option<unsafe extern "C-unwind" fn()> = Some(
    __slate_init_boot,
);
```
