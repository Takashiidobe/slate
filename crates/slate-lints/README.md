# slate-lints

Project-specific lints, run with [dylint](https://github.com/trailofbits/dylint).
The crate is outside the Cargo workspace and builds against rustc internals.

| Lint | Fires on |
| --- | --- |
| `slate::sensitive_wildcard` | A `_`, binding, or `matches!` catch-all over a `#[slate::sensitive]` enum when the arms name two or more variants. Single-variant projections and arms that return `Err(..)` or diverge are exempt. |
| `slate::sensitive_field_rest` | A `{ .. }` or `( .. )` pattern that skips a `#[slate::sensitive]` field. |

Policy (when to mark, how to resolve a hit, when an `#[expect]` is acceptable):
[wiki/concepts/linting.md](../../wiki/concepts/linting.md).

Both lints report on the match arm or pattern, so you can suppress one place
with `#[expect(slate::sensitive_wildcard, reason = "...")]`. The `slate` tool
namespace is registered in `slate-parser` and `slate`; ordinary builds ignore the
attributes.

## Setup (once per machine)

```bash
cargo install cargo-dylint dylint-link
rustup component add rustc-dev llvm-tools-preview --toolchain nightly-2026-10-25
rustup toolchain link slate-nightly-x86_64-unknown-linux-gnu \
  ~/.rustup/toolchains/nightly-2026-10-25-x86_64-unknown-linux-gnu
```

The `slate-nightly` alias is the same compiler as `rust-toolchain.toml`. dylint
reads the date in a toolchain name and adds clippy's extra symbols for that
date, and on `nightly-2026-10-25` that list duplicates symbols rustc already
interns, so the driver panics. These lints don't use `clippy_utils`, and an
undated name skips the extra symbols. When the workspace nightly changes,
relink the alias to the new toolchain directory.

## Run

```bash
cargo dylint --all -p slate-parser -p slate-c2rust   # from the workspace root
cd crates/slate-lints && cargo test                  # UI tests: ui/main.rs vs ui/main.stderr
```
