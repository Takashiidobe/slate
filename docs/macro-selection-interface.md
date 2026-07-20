# Macro selection interface (value-carrying predicates)

Status: **design** (slate-mt9.1). Not yet implemented. This describes the
planned TOML interface and the pipeline changes needed to recover
value-carrying preprocessor predicates as portable Rust.

## Why this exists

`translate-cfg` recovers `#if`/`#ifdef` regions by mapping their predicates to
Rust `#[cfg(...)]` (see [cfg-portability.md](cfg-portability.md)). That works
because the predicate reduces to a _boolean presence_ check — a macro is defined
or it is not. But Cargo features and Rust `cfg` carry **no value**, so a
predicate that compares a macro to a number —

```c
#if LOG >= 3
```

— has no direct `cfg` form and is refused today with the `opaque-predicate`
diagnostic. There is no `cfg(feature("log") >= 3)`.

The only faithful, non-guessing way to recover these is for the user to declare
the values a macro actually takes across the builds they care about. Slate then
turns the value axis into a small set of **mutually-exclusive Cargo features**
and lowers each comparison to a boolean `cfg` over them. The user declares
_values_, never features — Slate derives the feature set itself.

Scope note: this covers **integer-valued object-like macros** compared against
constants (character constants like `'A'` count as integers, so they are in
scope). Boolean `#ifdef FOO` flags use the feature mapping in slate-rs5, not
this. Each declared macro must also declare a **type**, which bounds the values
it may take — see [Types and validation](#types-and-validation).

## The TOML interface

A single `slate-macros.toml` declares, per macro, the **build value set** — the
concrete values you compile with:

```toml
# slate-macros.toml

# LOG is compared against constants in the source (`#if LOG >= 3`).
# `type` bounds the legal values; `values` is the set you actually ship.
[macros.LOG]
type = "int"
values = [1, 1000000]
default = 1                      # value backing the crate's default feature

# A range is shorthand for an inclusive integer set. A wide range is safe:
# Slate reduces it to the distinct classes the *code* discriminates, so a
# 0..=1_000_000 range does not create a million features.
[macros.VERBOSITY]
type = "unsigned char"
range = { min = 0, max = 5 }

# Character constants are integers; a `char` macro is declared like any other.
[macros.MODE]
type = "char"
values = [65, 66]                # 'A', 'B'

# Multiple macros are each declared independently — one set per macro.
[macros.FEATURE_LEVEL]
type = "int"
values = [0, 2, 4]
```

Rules:

- `type` is required per macro and must be a C integer type; it bounds the legal
  values — see [Types and validation](#types-and-validation).
- `values` (or `range`) is required per macro; `default` is optional and falls
  back to the first `values` entry / `range.min`.
- Values are integer constants (the preprocessor evaluates integer arithmetic
  only). Any integer within the declared type's range is legal.
- **One set per macro. No cross-macro declarations.** See
  [Out of scope](#out-of-scope).

### Types and validation

Every value a macro is declared with must fit its `type`. Slate rejects an
out-of-range value up front rather than pinning a `-D` Clang would truncate or
reinterpret — declaring `type = "char"` and listing `256` is denied, since a
`char` cannot hold it.

| `type`                     | range (target-dependent widths shown for LP64) |
| -------------------------- | ---------------------------------------------- |
| `signed char`              | −128 … 127                                     |
| `unsigned char`            | 0 … 255                                        |
| `char`                     | signedness follows the target's convention     |
| `short` / `unsigned short` | −32768 … 32767 / 0 … 65535                     |
| `int` / `unsigned int`     | −2³¹ … 2³¹−1 / 0 … 2³²−1                       |
| `long` / `unsigned long`   | −2⁶³ … 2⁶³−1 / 0 … 2⁶⁴−1                       |
| `long long` / `unsigned …` | −2⁶³ … 2⁶³−1 / 0 … 2⁶⁴−1                       |

Widths and `char` signedness resolve against the active target (`SLATE_TARGET`),
so validation matches what Clang would actually see. A value outside the range,
or a non-integer literal, is a hard error naming the macro, its type, and the
offending value.

## The reduction

Enumerating declared values 1:1 into features would be wasteful — declaring
`LOG` over `0..=1_000_000` should not emit a million features when the code only
ever tests `LOG >= 3`. Two composing reductions bound the feature count by the
**code**, not the declared domain:

### 1. Predicate partition

Collect every distinct constant any comparison uses on the macro. Sorted, these
breakpoints cut the value line into **equivalence classes** — each exact
constant, plus the open intervals between consecutive constants (clamped to the
declared set). Within a class _every_ predicate has a constant truth value, so
one feature per class suffices.

Worked example — the source uses `LOG == 1`, `LOG >= 3`, `LOG < 5`:

| value | (`==1`, `>=3`, `<5`) | class |
| ----- | -------------------- | ----- |
| 1     | T, F, T              | A     |
| 2     | F, F, T              | B     |
| 3, 4  | F, T, T              | C     |
| 5 …   | F, T, F              | D     |

Four classes, regardless of how wide the domain is.

**Soundness rule:** derive breakpoints from _every_ constant, including `==` and
`!=` targets. Then two values collapse into one class only when no predicate can
tell them apart, and merging them is provably output-identical.

### 2. Build-set intersection

Keep only the classes that contain a declared build value. If the user declares
`LOG ∈ {1, 1000000}`, value `1 → A` and `1000000 → D`, so only **two** classes
survive — the `B` and `C` code is never built and is not emitted. Final feature
cardinality ≤ number of declared values.

## Generated features and lowering

For each surviving class Slate emits one Cargo feature, named by the class's
smallest declared value (deterministic, readable):

- `log_1`, `log_1000000`

Each comparison lowers to a `cfg` over the class features whose class satisfies
it (evaluated at any representative of the class, since truth is constant there):

```
#if LOG >= 3    →   #[cfg(feature = "log_1000000")]
#if LOG == 1    →   #[cfg(feature = "log_1")]
```

Boolean operators compose as usual — `&&` → `all`, `||` → `any`, `!` → `not` —
and this composes across macros as long as each atom is still a single
macro-vs-constant test:

```
#if LOG == 1 && VERBOSITY >= 2   →   #[cfg(all(feature = "log_1", feature = "verbosity_2"))]
```

### Mutual exclusion

Cargo features are additive and not mutually exclusive, so the generated crate
guards the invariant at compile time:

```rust
#[cfg(all(feature = "log_1", feature = "log_1000000"))]
compile_error!("features `log_1` and `log_1000000` are mutually exclusive");

#[cfg(not(any(feature = "log_1", feature = "log_1000000")))]
compile_error!("select exactly one LOG feature: `log_1` or `log_1000000`");
```

The generated `Cargo.toml` sets a default so `cargo build` matches one real
configuration out of the box:

```toml
[features]
default = ["log_1"]
log_1 = []
log_1000000 = []
```

## CLI: generating a crate

`translate-cfg` gains a `--macros` flag and an `--out` directory:

```sh
# emit merged Rust to stdout, using the declared value classes
slate translate-cfg file.c --macros slate-macros.toml

# write a buildable crate (Cargo.toml with generated [features] + src/)
slate translate-cfg file.c --macros slate-macros.toml --out out/mycrate
```

Selecting a configuration is then ordinary Cargo:

```sh
cd out/mycrate
cargo build                                             # default feature (LOG=1)
cargo build --no-default-features --features log_1000000
```

The macro classes join the existing target/debug `cfg` matrix from
[cfg-portability.md](cfg-portability.md): each declared macro contributes one
pinned `-D<NAME>=<value>` per class to the Clang/CIR invocation, exactly as
`pin_args` already pins `defined()` atoms. Independent regions do not multiply;
only interacting/nested predicates would, and those follow the bound in
slate-rs5.

## Implementation notes

Where each piece lands in the current pipeline:

- **Predicate grammar** (`src/preprocess.rs`): today `LOG >= 3` tokenizes to a
  non-`defined` shape and becomes `PredExpr::Opaque`. Extend the parser to also
  recognize `<macro> <cmp> <int-const>` (and the reverse) as a new
  `PredExpr::Compare { macro, op, value }` **only when the macro is declared in
  the TOML**; otherwise keep it opaque so undeclared macros still refuse.
- **Type validation**: on load, resolve each declared `type` to a `[min, max]`
  against the active target and reject any declared value (or `range` bound)
  outside it, before any Clang invocation. Pure and unit-testable.
- **Class computation**: a new module derives the breakpoints per declared
  macro, builds the class partition, intersects with the declared build set, and
  assigns feature names. Pure and unit-testable, no Clang needed.
- **Config planning** (`src/cfg_translate.rs`): extend `plan_configs` /
  `pin_args` to add `-D<NAME>=<repr-value>` for each surviving class of each
  declared macro. Reuse `merge_variants` unchanged — items identical across
  classes collapse; differing items gate behind the class feature.
- **`cfg` synthesis**: `pred_to_cfg` gains a `Compare` arm that resolves to the
  `any(...)` of the class features satisfying it. Requires the class table as
  context (unlike the current context-free mapping).
- **Crate emission** (`--out`): render `Cargo.toml` (`[features]` + `default`),
  the merged `src/`, and the `compile_error!` mutual-exclusion guard.
- **Diagnostics**: a comparison on a macro _not_ declared in the TOML stays an
  `opaque-predicate` diagnostic that names the macro, so the user knows which
  macro to add to the file.

## Out of scope

- **Cross-macro operands** — a single comparison mixing two macros
  (`#if LOG > METRIC`, `#if LOG + METRIC > 5`). Truth depends on the _pair_, so
  it needs the cartesian product of both class sets, re-expanding the
  cardinality the reduction removes. Refused via `opaque-predicate`. There are
  no cross-macro declarations; one set per macro only.
- **Affine arithmetic on a single macro** (`#if LOG * 2 > 10`, which folds to
  `LOG > 5`) — a possible later fold, not part of this.
- **Non-integer macros** and boolean `#ifdef` flags — the latter is slate-rs5.
- **Guessing value domains** — without a TOML entry, value predicates refuse.
