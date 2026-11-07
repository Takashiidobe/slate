# Slate target queries

<!-- toc -->
- [Ownership](#ownership)
- [Configuration](#configuration)
- [Preprocessing](#preprocessing)
- [Oracle](#oracle)
<!-- /toc -->

## Ownership

| Facts                                                   | Source                                                       |
| ------------------------------------------------------- | ------------------------------------------------------------ |
| Scalar widths, char signedness, long double, endianness | `slate-parser::target_info::TargetInfo`                      |
| Supported triples and predefine snapshots               | `slate-parser::target_registry`                              |
| Active target selection                                 | `slate::target`                                              |
| Rust target cfg fields                                  | `slate::project::cfg`                                        |
| Target facts used by rewrites                           | IR module's `target`, passed to `backend::apply_with_target` |

## Configuration

- Default triple: `SLATE_TARGET`, then `SLATE_BUILD_TARGET`. The `slate-parser` binary defaults to its own build target when the registry supports it, otherwise `x86_64-unknown-linux-gnu`.
- System headers for the host target come from the native compiler (`slate_parser::native_headers`) before installed sysroots, unless `SLATE_SYSROOTS` is set; [slate-sysroots](../../crates/slate-sysroots/README.md#native-headers) lists the order. Test harnesses set `SLATE_SYSROOTS` so fixtures never see host headers.
- `target::parse_args`: default triple, inherited `SLATE_CLANG_ARGS`, explicit arguments.
- Explicit `--target` wins over the default; `Dialect` applies layout overrides.
- No Clang or `getconf` calls to query target facts.
- Unsupported triples use slate-parser's target diagnostic.

## Preprocessing

- Active `#error` / `#warning` records come from slate-parser diagnostics and source spans.

## Oracle

- Differential tests and corpus filters use `clang` on PATH.
- GCC corpus admission excludes `dg-shouldfail` cases; sanitizer UB tests cannot
  serve as uninstrumented differential references.
