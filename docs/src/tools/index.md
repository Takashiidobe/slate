# Tools

Run the commands in this chapter from the Slate workspace root. See
[Setup](../setup.md) for release binaries, compiler headers, and target
sysroots.

- [Regenerate FileCheck Expectations](filecheck.md) for parser and translation
  fixtures.
- [Generate Builtin Inventories](builtin-inventories.md) from Clang, GCC, and
  MSVC inputs.
- [Generate the Intrinsic Catalog](intrinsic-catalog.md) from LLVM and Rust
  stdarch sources.
- [Generate System Bindings](system-bindings.md) for target-specific
  `slate_support` mappings.
- [Sweep and Test the C Corpus](corpus.md) from compilation databases through
  project translation, building, and runtime comparison.
- [Compare Against a Compiler](compiler-comparisons.md) to investigate a
  preprocessor, layout, or flag discrepancy, or run bounded Alive2 validation.
- [Measure Performance](benchmarks.md) for generated SQLite code and
  interprocedural translation cost.
