# Single-config vs. portable multi-config translation

A C file is only fully defined _after_ preprocessing, and preprocessing depends
on the target and build macros in effect. `#if`/`#ifdef` gates mean a single
Clang invocation sees exactly one branch of each conditional — the untaken
branches never reach the AST or CIR. So Slate has two translation modes with
different portability guarantees.

## Single-config (`translate`)

`slate translate file.c` lowers the one preprocessed view Clang produces for the
current target and flags (`SLATE_TARGET`, `SLATE_CLANG_ARGS`, `-D`/`-U`). The
generated Rust is faithful to _that_ configuration only. If the source gates
code behind `#if defined(_WIN32)` and you translate on Linux, the Windows branch
is simply absent from the output — not wrong, just not present. This is the
right mode when you have one target in mind. Conditional scanning recognizes
C23 `#elifdef` and `#elifndef` continuations, including active and inactive
branches after literal `#if 0` and `#if 1` gates.

## Multi-config (`translate-directives`)

`slate translate-directives file.c` recovers portability. It:

1. records the preprocessor conditional regions of the source (the oracle in
   `src/preprocess.rs`; see [passes.md](passes.md));
2. maps each branch predicate to a Rust `cfg`;
3. re-runs Clang/CIR once per branch of each conditional region — each with
   that region's macros pinned (`-D` its atoms, `-U` the rest) so exactly that
   branch is active while independent regions stay on their default branch — and
   lowers each observed variant through the normal CIR path;
4. merges the results: items outside conditional regions are emitted once; items
   inside a recovered region are emitted once per branch behind that branch's
   `#[cfg(...)]` attribute.

Recovering the untaken branches genuinely requires the extra Clang/CIR
invocations — there is no way to see them from a single preprocessed view.

A source with no conditional regions passes straight through to single-config
lowering, so `translate-directives` is a safe superset of `translate` for such files.

## Non-conditional directive dispositions

Slate records non-conditional directives even when they do not become Rust
syntax. Their disposition determines whether Clang supplies the semantics,
whether no output is required, or whether translation must stop.

| Directive | Disposition | Behavior |
| --- | --- | --- |
| `#define`, `#undef` | consumed by Clang | The selected macro environment is reflected in the Clang AST and CIR. Macro invocation recovery remains on the macro-dump plugin path; Slate does not create Rust externs for macros. |
| `#include`, `#include_next`, `#import` | consumed by Clang | Header declarations, types, constants, and expansions reach lowering through Clang. The directive itself is not reproduced in Rust. |
| `#line` | consumed by Clang | Presumed filenames and line values affect Clang constants such as `__LINE__`. Physical source lines and offsets remain the keys for Slate's AST/CIR/directive joins. |
| `#embed` | consumed by Clang | Forms accepted by the configured Clang, including standard `limit`, `prefix`, `suffix`, and `if_empty` parameters, lower from CIR. Missing files and unsupported parameters are Clang translation errors. |
| `#pragma once` | no output | Include-once behavior is consumed while Clang preprocesses headers. No Rust syntax is required. |
| `#ident`, `#sccs` | no output | Compiler/object metadata has no generated Rust source form and does not affect program execution. |
| null directive (`#`) | no output | The directive has no effect. |

Unknown directives and directives classified as unsupported semantic operations
do not disappear silently. An active instance stops single-config translation.
Under `translate-directives`, a conditional instance becomes a cfg-gated
`compile_error!` when its effective predicate maps to Rust; otherwise
translation stops.

### Pragma inventory

Compilers may ignore pragmas they do not recognize, but Slate cannot infer that
a pragma was unknown to the compiler and build configuration for which the C
source was written. A recognized pragma can affect ABI or evaluation even when
the configured Clang ignores the same spelling. Slate therefore uses an
allowlist: exact `#pragma once` is no-output, diagnostic controls retain the
diagnostic-only disposition, and every other pragma remains unsupported until
its effect is recovered.

| Family | Examples | Current disposition | Follow-up |
| --- | --- | --- | --- |
| Include guard | `once` | no output | supported |
| Diagnostic controls | `GCC diagnostic`, `clang diagnostic`, MSVC `warning(...)` | diagnostic only | warning handling is covered by `slate-9msj.7` |
| Standard floating point | `STDC FENV_ACCESS`, `STDC FP_CONTRACT`, `STDC CX_LIMITED_RANGE` | unsupported semantic | `slate-9msj.10` |
| Record packing | `pack(push, n)`, `pack(pop)`, `pack(n)` | unsupported semantic | `slate-9msj.11` |
| Vendor symbol and code generation controls | GCC/Clang visibility, section, optimize, and target pragmas; MSVC segment and optimization controls | unsupported semantic | `slate-9msj.12` |
| Unknown vendor pragmas | any other pragma outside the allowlist | unsupported semantic | add a focused ticket before extending the allowlist |

The repository fixture inventory currently contains diagnostic pragmas and
`#pragma pack`; no supported fixture relies on another semantic pragma. The
inventory fixture also locks in explicit rejection for the three standard
floating-point controls and representative GCC and unknown-vendor forms.

## Supported predicate → `cfg` mappings

These `defined(MACRO)` predicates (and boolean combinations of them with `!`,
`&&`, `||`) map to built-in Rust `cfg`s. Project-style macros also map to Cargo
features as described below. The table is the source of truth in `known_cfg`
(`src/preprocess.rs`).

| C predicate                                                         | Rust `cfg`                                               |
| ------------------------------------------------------------------- | -------------------------------------------------------- |
| `defined(_WIN64)`                                                   | `all(windows, target_pointer_width = "64")`              |
| `defined(_WIN32)`                                                   | `windows`                                                |
| `defined(__linux__)` / `defined(__linux)` / `defined(linux)`        | `target_os = "linux"`                                    |
| `defined(__ANDROID__)`                                              | `target_os = "android"`                                  |
| `defined(__FreeBSD__)`                                              | `target_os = "freebsd"`                                  |
| `defined(__unix__)` / `defined(__unix)`                             | `unix`                                                   |
| `defined(__APPLE__)`                                                | `target_vendor = "apple"`                                |
| `defined(__x86_64__)` / `defined(_M_X64)`                           | `target_arch = "x86_64"`                                 |
| `defined(__i386__)` / `defined(_M_IX86)`                            | `target_arch = "x86"`                                    |
| `defined(__aarch64__)` / `defined(_M_ARM64)`                        | `target_arch = "aarch64"`                                |
| `defined(__arm__)` / `defined(_M_ARM)`                              | `target_arch = "arm"`                                    |
| `defined(__powerpc64__)` / `defined(__PPC64__)`                     | `target_arch = "powerpc64"`                              |
| `defined(__powerpc__)` / `defined(__POWERPC__)` / `defined(_M_PPC)` | `target_arch = "powerpc"`                                |
| `defined(__wasm64__)`                                               | `target_arch = "wasm64"`                                 |
| `defined(__wasm32__)`                                               | `target_arch = "wasm32"`                                 |
| `defined(_M_RISCV64)`                                               | `target_arch = "riscv64"`                                |
| `defined(_M_RISCV32)`                                               | `target_arch = "riscv32"`                                |
| `defined(__LP64__)` / `defined(_LP64)`                              | `target_pointer_width = "64"`                            |
| `defined(__ILP32__)` / `defined(_ILP32)`                            | `target_pointer_width = "32"`                            |
| `defined(__ARMEB__)`                                                | `all(target_arch = "arm", target_endian = "big")`        |
| `defined(__ARMEL__)`                                                | `all(target_arch = "arm", target_endian = "little")`     |
| `defined(__AARCH64EB__)`                                            | `all(target_arch = "aarch64", target_endian = "big")`    |
| `defined(__AARCH64EL__)`                                            | `all(target_arch = "aarch64", target_endian = "little")` |
| `defined(NDEBUG)`                                                   | `not(debug_assertions)`                                  |

Project feature macros are identifiers that do not start with `_` and are not
one of the built-in mappings above. They map mechanically to Cargo features:
`defined(MY_FEATURE)` becomes `feature = "my_feature"`, and
`#ifndef MY_FEATURE` becomes `not(feature = "my_feature")`. Non-alphanumeric
characters collapse to `_`, and the feature name is lowercased. A C build that
previously selected a branch with `-DMY_FEATURE` should select the translated
Rust branch with `--features my_feature`.

Combinations compose as expected: `||` becomes `any(...)`, `&&` becomes
`all(...)`, `!` becomes `not(...)`, and `#else` becomes the negation of the
`any(...)` of its chain's prior predicates (so the `#else` of a lone
`#if defined(NDEBUG)` collapses to `debug_assertions`).

### `NDEBUG` caveat

`NDEBUG` is mapped to `not(debug_assertions)` as the initial convention. This is
not exact: C build systems can define `NDEBUG` independently of optimization
level, whereas Rust's `debug_assertions` tracks the profile. A project that
decouples the two will need to override this mapping.

## Project feature configurations

Slate deliberately keeps project macro handling boolean. `translate-directives` can
recover whole-item branches gated by `#ifdef MY_FEATURE`,
`#if defined(MY_FEATURE)`, and boolean combinations of such predicates. Multiple
independent top-level regions are handled per region rather than as a cross
product, and nested regions are supported while the generated branch count stays
within the built-in cap of 16 variants. If a source exceeds that cap,
`translate-directives` refuses with a diagnostic pointing at the region instead of
exploding into an unbounded matrix.

Run `slate record-cfg file.c [clang args...]` to see the recorded conditional
regions, their normalized predicates, mapped `cfg`s, and which branch is active
for a given set of args — useful for understanding why a region was or was not
merged.

## Diagnostics

Slate never guesses a `cfg` for a predicate it does not understand, so it reports
the branches it cannot recover instead of silently dropping them. `record-cfg`
emits these as structured `diagnostics` entries (`kind`, `line`, `message`),
distinguishing two predicate classes so you know whether to add a config-matrix
entry or rewrite the source:

- **`unmapped-macro`** — a clean `defined(...)` predicate that names a reserved
  or system-style macro with no Rust cfg mapping, such as `_FILE_OFFSET_BITS` or
  an unsupported compiler identity macro. The predicate is _recorded and
  understood_, but Slate refuses to pretend it is a Cargo feature because those
  macros often reshape ABI, libc, or compiler behavior. The diagnostic names the
  offending macro(s). A branch that is also inactive in the queried config is
  flagged **uncovered**: it would vanish from the output entirely.
- **`opaque-predicate`** — a predicate whose _shape_ is outside the
  boolean-over-`defined()` subset (arithmetic, comparisons, bare macros, e.g.
  `#if VERSION > 3`), which cannot be normalized to a `cfg` at all.

Structural problems (`stray-directive`, `unterminated-if`) are reported the same
way.

## What is refused

`translate-directives` emits a diagnostic and refuses (rather than guessing) when a
conditional region cannot be stitched as whole Rust items. These split into
_unsupported-but-recorded predicates_ and _code shapes that cannot be merged
cleanly_:

- **Unmapped predicates** — a predicate with no built-in cfg or project-feature
  mapping (the `unmapped-macro`/`opaque-predicate` classes above).
- **Fragment cuts** — a directive inside a function or record body, where the cut
  does not fall on an item boundary.
- **Too many region variants** — nested and independent chains are bounded by
  the 16-variant cap.
- **Predicates that cannot be isolated** — e.g. a negated or `#ifndef` opening
  branch that the pinning heuristic cannot make uniquely active.

Out of scope for now: expression-fragment macro surgery, value-carrying
predicates such as `#if FOO == 2`, and arbitrary combinatorial macro
exploration.
