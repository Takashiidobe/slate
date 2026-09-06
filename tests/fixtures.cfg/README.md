# Preprocessor cfg fixtures

`slate translate` auto-detects whole-item `#if`/`#ifdef` regions gated
solely by target/arch/os macros (`__x86_64__`, `__linux__`, `NDEBUG`, ...)
and expands them into a multi-config build: one clang invocation per branch,
merged back behind Rust `#[cfg(...)]` attributes
(`src/frontend/directive_translate.rs`). Project-defined feature macros are
explicitly out of scope for this expansion and always fall back to plain
single-config translation of whichever branch the host's default macro
state selects. These fixtures are not part of `tests/fixtures/` because the
differential harness there translates a single preprocessed target view.

Expected predicate mappings live in `expected_cfgs.json`, checked against
`preprocess::pred_to_cfg` by `tests/preprocess.rs`.

`NDEBUG` is mapped to `not(debug_assertions)` here as the initial convention.
Project build metadata may need to override that later because C build systems
can define `NDEBUG` independently of compiler optimization level.

The `reject/` sources exercise directives that stay unsupported regardless of
configuration (unconditional pack/pragma/embed/poison failures); they are not
about the multi-config expansion itself.
