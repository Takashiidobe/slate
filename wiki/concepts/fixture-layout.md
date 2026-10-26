# Fixture layout

<!-- toc -->
- [Path](#path)
- [Directives to write before
  generating](#directives-to-write-before-generating)
- [Ids in generated expectations](#ids-in-generated-expectations)
- [Matcher](#matcher)
<!-- /toc -->

## Path

A fixture's directory alone sets its compiler and target. `tests/filecheck.rs`
(`fixture_placement`) and `tools/update_filecheck.py` (`placement`) parse it
identically and fail on a path that doesn't fit.

```
tests/fixtures/[error/ | suites/<name>/]<flavor>/[<os>/[<arch> | <triple>]]/<fixture>.c
```

| Segment          | Values                                                                                                                                                                                                                                                                                                                                           |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `<flavor>`       | `gcc`, `clang`, `msvc`. Required.                                                                                                                                                                                                                                                                                                                |
| `<os>`           | `linux`, `windows`, `darwin`, `android`, `freebsd`. Default `windows` for msvc, else `linux`.                                                                                                                                                                                                                                                    |
| `<arch>`         | Mapped to the OS's usual triple by `CANONICAL_TRIPLES` (`windows/i686` = `i686-pc-windows-msvc`). Default `x86_64`.                                                                                                                                                                                                                              |
| `<triple>`       | Full triple when the table can't express it (`linux/armv7-unknown-linux-gnueabihf/`).                                                                                                                                                                                                                                                            |
| `error/`         | Every configuration errors. Mixed fixtures stay in the main tree.                                                                                                                                                                                                                                                                                |
| `suites/<name>/` | Imported suites (`gcc-dg`, `gcc-torture`, `clang-test`), written by sweep tools with `--migrate`. gcc-torture and gcc-dg run under both `gcc/` and `clang/` (gcc-dg under `clang/` only where clang accepts it, with the test's `-std`). A test in one flavor only is either rejected by the other oracle or has a gap under `slate-parser-cxg`. |
| `inputs/`        | Headers and sysroots reached via directives (`SLATE-FILECHECK-ISYSTEM`, `-I`, `--sysroot`, `-include`). Headers included relative to a fixture sit beside it, copied per directory.                                                                                                                                                              |

- `SLATE-FILECHECK-PREFIX-ARGS <prefix> --parse-only` skips semantic analysis
  for AST grammar coverage. It requires `SLATE-FILECHECK-AST`; IR modes reject
  `--parse-only`. Standard-dependent semantic constraints still belong to sema.
- `SLATE-FILECHECK-AST` selects AST output instead of the default IR
  output for parser-only fixtures.
- One file covers one compiler and target; multi-config fixtures are one
  file per directory (`target_registry.c`). File names don't repeat the
  directory.
- A valid `--target`/`--flavor` in `SLATE-FILECHECK-ARGS` or `PREFIX-ARGS`
  must match the path. Invalid values are allowed to test option errors
  (`error/clang/linux/x86_64/target_unknown.c`).

## Directives to write before generating

`update_filecheck.py` only writes the `BEGIN`/`END` blocks. Everything else is
a `// SLATE-FILECHECK-<NAME>` line you add first, because the generator and
`tests/filecheck.rs` read them to decide what to run and render. A fixture
without the marker it needs is left unchanged with no message, so a missing
directive looks like "nothing happened". Prefix names are free-form (`DEFAULT`, `WARN`, `IR`) and
become the `// <PREFIX>:` check lines.

| Directive                                   | Write it when                                                                          | Example                                      |
| ------------------------------------------- | -------------------------------------------------------------------------------------- | -------------------------------------------- |
| `DEFINES <prefix> [-DX ...]`                | a run is wanted under a named configuration (almost every IR fixture has `DEFAULT`)    | `// SLATE-FILECHECK-DEFINES DEFAULT`         |
| `ARGS <flags>`                              | the fixture needs compiler flags for all prefixes (`--dump-ir`, `--compact-ir`, `-std=`) | `// SLATE-FILECHECK-ARGS --dump-ir`          |
| `PREFIX-ARGS <prefix> <flags>`              | only one prefix needs the flags (`--parse-only`)                                       | `// SLATE-FILECHECK-PREFIX-ARGS AST --parse-only` |
| `STD <prefix> <std>`                        | the prefix pins a language standard                                                    | `// SLATE-FILECHECK-STD DEFAULT gnu23`       |
| `AST`                                       | the output is the AST instead of IR (bare line, no prefix)                             | `// SLATE-FILECHECK-AST`                     |
| `ERROR <prefix>`                            | the run must fail in preprocessing, parsing or sema; the block holds the diagnostic    | `// SLATE-FILECHECK-ERROR PARSE`             |
| `IR-ERROR <prefix>`                         | parsing succeeds and IR lowering must fail                                             | `// SLATE-FILECHECK-IR-ERROR DEFAULT`        |
| `WARNING <prefix>`                          | the run must succeed and the block holds warnings                                      | `// SLATE-FILECHECK-WARNING WARN`            |
| `PP <prefix>`                               | the block holds `slate-parser pp` output; bytes that are not UTF-8 print as `<0xE9>`   | `// SLATE-FILECHECK-PP DEFAULT`              |
| `ISYSTEM <dirs>`                            | headers come from `inputs/` or a sysroot                                               | `// SLATE-FILECHECK-ISYSTEM inputs/sysroot`  |
| `SHOW-IDS <prefix>`                         | the fixture checks raw ids, so id loosening must be off                                | `// SLATE-FILECHECK-SHOW-IDS DEFAULT`        |
| `EXAMPLE <name>`                            | the output comes from a `cargo run --example <name>` instead of the CLI                | `// SLATE-FILECHECK-EXAMPLE <name>`          |

Common starting points:

- Error fixture (`error/<flavor>/<os>/<arch>/`): the source, then a blank
  line and `// SLATE-FILECHECK-ERROR PARSE`. Nothing else
  (`error/clang/linux/x86_64/embed-missing-resource.c`).
- IR fixture: `DEFINES DEFAULT` and `ARGS --dump-ir`, plus `STD DEFAULT <std>`
  if the standard matters.
- AST fixture: `AST`, `DEFINES DEFAULT`, and `PREFIX-ARGS DEFAULT --parse-only`
  when sema is out of scope.
- Preprocessed-output fixture: `DEFINES <prefix>` and `PP <prefix>` per
  configuration. The scratch copy keeps the fixture's raw bytes, so a source
  byte that is not UTF-8 reaches the preprocessor unchanged
  (`msvc/windows/x86_64/pp_code_page.c`).

Then run `python3 crates/slate-parser/tools/update_filecheck.py --in-place <fixture>`.
If the file comes back unchanged, a directive is missing or misspelled.

## Ids in generated expectations

Translation-unit-wide ids become FileCheck variables so a new header
declaration doesn't restamp every fixture.

- AST: tag, file, node ids → numeric variables `FILE0`, `TAG0`, `NODE0` in
  first-use order (`loosen_ids`).
- IR: `@typeN` / `%N` → string variables named after the declaration
  (`TYPE__IO_FILE`, `VALUE_stderr`; `loosen_ir_ids`). Repeats count up
  (`VALUE_x_2`); unnamed get ordinals (`TYPE0`). `.strN` reuses its
  binding's variable. Quoted strings and asm `template:` lines are left
  alone (their `%N` are operand indices).
- Each IR type and binding has one definition line and blocks use `-NEXT`,
  so changed cross-references still fail; only renumbering is ignored.

## Matcher

`tests/filecheck/matcher.rs` checks output itself instead of LLVM FileCheck,
whose per-line regex compilation made large fixtures take seconds
(`cpp__embed-14.c`, `bitint-39.c`, `20001226-1.c`).

- Accepts only what the generator emits: `PREFIX:`, `PREFIX-NEXT:`;
  `[[V:[0-9]+]]`, `[[V]]`, `[[#V:]]`, `[[#V]]`; `{{...}}` bodies `[0-9]+`,
  `.*`, `\[[0-9, ]+\]` and the three escapes. Anything else is a "not
  supported" error; teach the matcher when the generator gains a construct.
- Stricter than FileCheck: a check matches a whole line after collapsing
  horizontal whitespace. A plain check matches the first such line after
  the previous match.
- On mismatch the harness also runs real FileCheck for its annotated dump.
