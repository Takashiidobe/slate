# Cross-target toolchains

<!-- toc -->
- [Translation](#translation)
- [Building generated Rust](#building-generated-rust)
- [Coverage](#coverage)
<!-- /toc -->

## Translation

- slate-parser models target facts from its registry and reads target headers from slate-sysroots.
- [Target configuration](slate-target-queries.md) defines defaults and compiler argument precedence.
- Install headers for each target before translating:

```bash
cargo run --release -p slate-c2rust -- sysroot install aarch64-unknown-linux-gnu
cargo run --release -p slate-c2rust -- translate \
  --target=aarch64-unknown-linux-gnu input.c
```

- `translate --targets=<triple>,<triple> input.c` can merge supported whole-item configuration variants into Rust cfg items.
- `translate-project` consumes compile commands. When a file has commands with different arguments, it translates one build target, named by the directory of each entry's object file (`zlibstatic.dir` is `zlibstatic`, Meson's `libz.so.p` is `libz.so`). It prefers targets with no `main`, then the most files, then no `-fPIC`, then the first name, and prints the choice; `--build-target <name>` overrides this and also applies without a conflict. It does not generate a merged multi-target project.
- `translate-project --matrix <file> --matrix-target <name> [--features a,b] <project> <crate>` translates one target and feature set from a hand-written matrix instead of a compile database, so host configure results cannot leak into other targets. `tools/corpus/matrices/zlib.json` is the zlib example.
- `translate-project --plan --matrix <file> [--matrix-target <name>] <project>` prints, per target and translation unit, the feature configurations to compile, using only the preprocessor plus a parse and sema check of the chosen ones. It exits nonzero if a unit cannot be planned. `--plan-exhaustive` also tries every feature subset (at most 16 features) and reports what the plan missed.
- `translate-project --merge --matrix <file> [--matrix-target a,b] [--check-projection all|<count>] <project> <crate>` writes one crate covering the selected targets and features. Each unit is compiled alone, with `Exports::Fixed`, under every assignment of the features its preprocessor trace says it depends on. Each item's `#[cfg]` is minimized from that truth table, and assignments that fail to translate (`#error`, sema) are don't-cares. There is no `__slate_types` module, because `Exports::Fixed` never hoists types. `--check-projection` translates whole-project configurations directly and requires the items the merged crate resolves to for that configuration; `all` covers every valid feature subset. A distinct `link_section` depends on other units' address-taken symbols, so the merge protects any symbol taken in any compile of the target; the check allows this extra protection and rejects lost protection. The merge also fails if a symbol shared between units has incompatible signatures in any valid feature assignment of a target. It compares by-value layouts and pointee names, and accepts what C allows across units: an unprototyped declaration, an unsized `extern` array, a pointer to an opaque record, and the alignment wrapper on a definition. A third unit's `#error` can make the reported assignment impossible, so a report can be a false positive, but no valid assignment goes unchecked. Two pointers to records of the same name match even if the record differs between units, which has no effect on the call.

  The file holds `sources` and `args`, relative to `<project>`; `targets` (`name`, `triple`, `flavor`, optional `sysroot` and `args`, with generated headers given as `-I` in `args`); `features` (`name`, used as the Cargo feature name, and `defines`); and `invalid_features`, groups that cannot be selected together. Arguments apply in the order target, project, features. Leave out macros that are not real variants: zlib's `Z_PREFIX` is a rename, `MAKECRCH` adds a table-generator `main`, and `Z_SOLO` and `NO_STRERROR` do not compile under clang either.
- A function whose variants differ in one value (a literal, or a primitive type at any number of positions) keeps one shared body that names one `#[cfg]`-gated module-level `const __SLATE_HOIST_<fn>` or `type __SlateHoist_<fn>` (`src/project_hoist.rs`). A literal's const needs a type that every use agrees on, from its suffix or its context (return, typed `let`, direct cast, arithmetic). Anything else, including a difference in a node the walker does not enter, stays whole-item duplication. `--check-projection` expands the bindings before comparing.

## Building generated Rust

- Install the matching Rust target, linker and target C libraries.
- Generated crates with C runtime bridges also need a C cross compiler configured for the `cc` build dependency.
- Run cross-built binaries on the target or with a suitable user-mode emulator.

## Coverage

- The `slate` nextest profile executes x86_64 Linux differential suites.
- `slate-i686`, `slate-aarch64` and `slate-targets` run `tests/fixtures` for other Linux targets, under `qemu-aarch64-static` for aarch64 ([targets](differential-fixtures.md#targets)). aarch64 needs `aarch64-linux-gnu-gcc` and its sysroot in `/usr/aarch64-linux-gnu`.
- Other environments and OSes are tracked in `slate-5vj0.8`; corpus suites remain host-only.
- Successful translation alone is not cross-target runtime validation.
