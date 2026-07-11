# Preprocessor cfg fixtures

These fixtures are for the future multi-configuration frontend. They are not
part of `tests/fixtures/` because the current differential harness translates a
single preprocessed target view.

The intended pipeline is:

1. inspect preprocessing predicates and conditional source ranges;
2. run Clang/CIR for a bounded configuration matrix;
3. lower each observed variant through the existing CIR path;
4. merge whole items behind Rust `#[cfg(...)]` attributes.

Expected predicate mappings live in `expected_cfgs.json`.

`NDEBUG` is mapped to `not(debug_assertions)` here as the initial convention.
Project build metadata may need to override that later because C build systems
can define `NDEBUG` independently of compiler optimization level.
