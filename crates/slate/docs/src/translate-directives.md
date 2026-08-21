# Translate Directives/Cross Compilation

## Why we can't trust clang-ast

Preprocessing a file picks one branch of every `#if` and throws the
rest away that's what preprocessing means. If Slate preprocessed on an
x86_64 Linux host and lowered the result, an `#ifdef __aarch64__` branch
would be deleted by the preprocessor.

To keep every target's code path, Slate has to run the
whole `C -> CIR -> parse -> lower` pipeline once per relevant preprocessor
configuration and then merge the results back into one file, using
`#[cfg(...)]`.

## Translating Directives (`src/frontend/directive_translate.rs`)

1. Scan the source for conditional chains without touching Clang at all
   (`preprocess::record`) every `#if`/`#ifdef`/`#elif`/`#else`/`#endif`
   region and its predicate expression.
2. Map each branch's predicate to a Rust `Cfg` (`pred_to_cfg`). Slate
   only accepts a fixed list of macros, like `__x86_64__`/`_M_X64`=
   `target_arch = "x86_64"`, `__linux__` = `target_os = "linux"`,
   `__APPLE__` = `target_vendor = "apple"`, `_WIN32` = the `windows` flag,
   `__LP64__`/`_ILP32` = `target_pointer_width`, `__ARMEB__`/`__AARCH64EB__`
   = a combined `target_arch` + `target_endian = "big"`, `NDEBUG` =
   `not(debug_assertions)`, and so on plus boolean combinations
   (`&&`/`||`/`!`) of those atoms. A branch whose predicate doesn't reduce to
   one of these is left unmapped.
3. Enumerate one clang invocation per branch (`plan_configs`), each
   pinning the cfgs that decide branching with `-D`/`-U` flags so Clang
   only ever sees one selected configuration at a time this is what keeps
   the CIR/AST/lowering pipeline unchanged; it never has to know about
   multi-config.
4. Translate each configuration independently (`translate_one`).
5. Merge variants: for each conditional region,
   every item (function, static, ...) produced by a branch's translation is
   wrapped in `#[cfg(<branch's Rust cfg>)]` and spliced back into the
   file. Code outside any conditional region is taken once from a baseline
   (unconfigured) translation.

Only whole top-level items can be merged this way a `#if` that opens or
closes partway through a function or struct body is rejected
(`ConditionalInBody`), since there's no Rust `#[cfg(...)]` that can gate part
of an item. There's also a cap (`MAX_CFG_VARIANTS`, currently 16) on how many
branch variants a file can expand to, since each one is a full clang
invocation and we don't want to run exponentially long.

## Example

Here's an example, `tests/fixtures.cfg/arch_targets.c`:

```c
#include <stdio.h>

#if defined(__x86_64__) || defined(_M_X64)
static int arch_code(void) { return 64; }
#elif defined(__i386__) || defined(_M_IX86)
static int arch_code(void) { return 86; }
#elif defined(__aarch64__) || defined(_M_ARM64)
static int arch_code(void) { return 128; }
#else
static int arch_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", arch_code());
  return 0;
}
```

`slate translate-directives arch_targets.c` produces one `arch_code` per
branch, each gated by the matching `target_arch`, with the final `#else`
becoming the negation of every other branch's condition (bodies abbreviated
below; the real output is baseline, unfixed-up lowering):

```rust
#[cfg(target_arch = "x86_64")]
fn arch_code() -> i32 {
    64
}

#[cfg(target_arch = "x86")]
fn arch_code() -> i32 {
    86
}

#[cfg(target_arch = "aarch64")]
fn arch_code() -> i32 {
    128
}


#[cfg(not(any(target_arch = "x86_64", target_arch = "x86", target_arch = "aarch64")))]
fn arch_code() -> i32 {
    0
}

fn main() {
   arch_code(); // returns 0, 64, 86, 128
}
```

The result is one crate where `cargo build --target aarch64-unknown-linux-gnu`
and `cargo build --target x86_64-pc-windows-msvc` each pick a different
`arch_code`, matching what the original C would have compiled to under a
cross toolchain for that target. Combined with `libc-shim`, you can
cross compile for any target that slate supports, since any external C
calls are also provided. This doesn't extend to non-libc code (Slate
cannot shim your custom code) but if that's also provided, then slate
can also handle that case too.
