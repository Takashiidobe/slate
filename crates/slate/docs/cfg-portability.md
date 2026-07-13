# Single-config vs. portable multi-config translation

A C file is only fully defined *after* preprocessing, and preprocessing depends
on the target and build macros in effect. `#if`/`#ifdef` gates mean a single
Clang invocation sees exactly one branch of each conditional — the untaken
branches never reach the AST or CIR. So Slate has two translation modes with
different portability guarantees.

## Single-config (`translate`)

`slate translate file.c` lowers the one preprocessed view Clang produces for the
current target and flags (`SLATE_TARGET`, `SLATE_CLANG_ARGS`, `-D`/`-U`). The
generated Rust is faithful to *that* configuration only. If the source gates
code behind `#if defined(_WIN32)` and you translate on Linux, the Windows branch
is simply absent from the output — not wrong, just not present. This is the
right mode when you have one target in mind.

## Multi-config (`translate-cfg`)

`slate translate-cfg file.c` recovers portability. It:

1. records the preprocessor conditional regions of the source (the oracle in
   `src/preprocess.rs`; see [passes.md](passes.md));
2. maps each branch predicate to a Rust `cfg`;
3. re-runs Clang/CIR once per branch — each with the branch's macros pinned
   (`-D` its atoms, `-U` the rest) so exactly that branch is active — and lowers
   each observed variant through the normal CIR path;
4. merges the results: items that are byte-for-byte identical across variants are
   emitted once; items that differ are emitted once per variant behind the
   branch's `#[cfg(...)]` attribute.

Recovering the untaken branches genuinely requires the extra Clang/CIR
invocations — there is no way to see them from a single preprocessed view.

A source with no conditional regions passes straight through to single-config
lowering, so `translate-cfg` is a safe superset of `translate` for such files.

## Supported predicate → `cfg` mappings

Only these `defined(MACRO)` predicates (and boolean combinations of them with
`!`, `&&`, `||`) map to a Rust `cfg`. The table is the source of truth in
`known_cfg` (`src/preprocess.rs`).

| C predicate                | Rust `cfg`                  |
| -------------------------- | --------------------------- |
| `defined(_WIN64)`          | `all(windows, target_pointer_width = "64")` |
| `defined(_WIN32)`          | `windows`                   |
| `defined(__linux__)` / `defined(__linux)` / `defined(linux)` | `target_os = "linux"` |
| `defined(__ANDROID__)`     | `target_os = "android"`     |
| `defined(__FreeBSD__)`     | `target_os = "freebsd"`     |
| `defined(__unix__)` / `defined(__unix)` | `unix`         |
| `defined(__APPLE__)`       | `target_vendor = "apple"`   |
| `defined(__x86_64__)` / `defined(_M_X64)`   | `target_arch = "x86_64"`  |
| `defined(__i386__)` / `defined(_M_IX86)`    | `target_arch = "x86"`     |
| `defined(__aarch64__)` / `defined(_M_ARM64)`| `target_arch = "aarch64"` |
| `defined(__arm__)` / `defined(_M_ARM)`      | `target_arch = "arm"`     |
| `defined(__powerpc64__)` / `defined(__PPC64__)` | `target_arch = "powerpc64"` |
| `defined(__powerpc__)` / `defined(__POWERPC__)` / `defined(_M_PPC)` | `target_arch = "powerpc"` |
| `defined(__wasm64__)`      | `target_arch = "wasm64"`    |
| `defined(__wasm32__)`      | `target_arch = "wasm32"`    |
| `defined(_M_RISCV64)`      | `target_arch = "riscv64"`   |
| `defined(_M_RISCV32)`      | `target_arch = "riscv32"`   |
| `defined(__LP64__)` / `defined(_LP64)`      | `target_pointer_width = "64"` |
| `defined(__ILP32__)` / `defined(_ILP32)`    | `target_pointer_width = "32"` |
| `defined(__ARMEB__)`       | `all(target_arch = "arm", target_endian = "big")` |
| `defined(__ARMEL__)`       | `all(target_arch = "arm", target_endian = "little")` |
| `defined(__AARCH64EB__)`   | `all(target_arch = "aarch64", target_endian = "big")` |
| `defined(__AARCH64EL__)`   | `all(target_arch = "aarch64", target_endian = "little")` |
| `defined(NDEBUG)`          | `not(debug_assertions)`     |

Combinations compose as expected: `||` becomes `any(...)`, `&&` becomes
`all(...)`, `!` becomes `not(...)`, and `#else` becomes the negation of the
`any(...)` of its chain's prior predicates (so the `#else` of a lone
`#if defined(NDEBUG)` collapses to `debug_assertions`).

### `NDEBUG` caveat

`NDEBUG` is mapped to `not(debug_assertions)` as the initial convention. This is
not exact: C build systems can define `NDEBUG` independently of optimization
level, whereas Rust's `debug_assertions` tracks the profile. A project that
decouples the two will need to override this mapping.

## Providing project-specific configurations

Slate deliberately does **not** guess arbitrary macro combinations. It only
merges branches whose predicates are in the table above. To translate a variant
selected by a project-specific macro, pin it yourself and use single-config
mode:

```sh
SLATE_CLANG_ARGS="-DMY_FEATURE=1" slate translate file.c
```

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

- **`unmapped-macro`** — a clean `defined(...)` predicate that names a macro with
  no entry in the table above (e.g. a project-specific `defined(MY_FEATURE)`).
  The predicate is *recorded and understood* — it just needs a user-supplied
  mapping. The diagnostic names the offending macro(s). A branch that is also
  inactive in the queried config is flagged **uncovered**: it would vanish from
  the output entirely.
- **`opaque-predicate`** — a predicate whose *shape* is outside the
  boolean-over-`defined()` subset (arithmetic, comparisons, bare macros, e.g.
  `#if VERSION > 3`), which cannot be normalized to a `cfg` at all.

Structural problems (`stray-directive`, `unterminated-if`) are reported the same
way.

## What is refused

`translate-cfg` emits a diagnostic and refuses (rather than guessing) when a
conditional region cannot be stitched as whole Rust items. These split into
*unsupported-but-recorded predicates* and *code shapes that cannot be merged
cleanly*:

- **Unmapped predicates** — a predicate with no entry in the table above
  (the `unmapped-macro`/`opaque-predicate` classes above).
- **Fragment cuts** — a directive inside a function or record body, where the cut
  does not fall on an item boundary.
- **Multiple or nested chains** — currently only a single top-level
  `#if`/`#endif` chain is merged.
- **Predicates that cannot be isolated** — e.g. a negated or `#ifndef` opening
  branch that the pinning heuristic cannot make uniquely active.

Out of scope for now: expression-fragment macro surgery and arbitrary
combinatorial macro exploration.
