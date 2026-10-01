# Slate target queries

<!-- toc -->
- [Ownership](#ownership)
- [Configuration](#configuration)
- [Preprocessing](#preprocessing)
- [Oracle](#oracle)
<!-- /toc -->

## Ownership

| Facts | Source |
| --- | --- |
| Scalar widths, char signedness, long double, endianness | `slate-parser::target_info::TargetInfo` |
| Supported triples and predefine snapshots | `slate-parser::target_registry` |
| Active target selection | `slate::target` |
| Rust target cfg fields | `slate::frontend::{preprocess,directive_translate}` |
| Target facts used by rewrites | IR module's `target`, passed to `backend::apply_with_target` |

## Configuration

- Default triple: `SLATE_TARGET`, then `SLATE_BUILD_TARGET`.
- `target::parse_args`: default triple, inherited `SLATE_CLANG_ARGS`, explicit arguments.
- Explicit `--target` wins over the default; `Dialect` applies layout overrides.
- No Clang or `getconf` calls to query target facts.
- Unsupported triples use slate-parser's target diagnostic.

## Preprocessing

- `preprocess::record_file`: slate-parser predefines and ordered `-D`/`-U` inputs.
- `preprocess::record_translation_unit`: slate-parser preprocessing, including forced files and headers.
- Active `#error` / `#warning` records come from slate-parser diagnostics and source spans.
- `record-cfg` uses translation-unit recording so quoted includes resolve beside the input.

## Oracle

- Differential tests and corpus filters use `clang` on PATH.
- `SLATE_CLANG` is removed; no compiler override in translation or test support.
- CIR ingestion and its fixed compiler path are removed.
- GCC corpus admission excludes `dg-shouldfail` cases; sanitizer UB tests cannot
  serve as uninstrumented differential references.
