# Fixture layout

<!-- toc -->
- [Path](#path)
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

| Segment | Values |
| --- | --- |
| `<flavor>` | `gcc`, `clang`, `msvc`. Required. |
| `<os>` | `linux`, `windows`, `darwin`, `android`, `freebsd`. Default `windows` for msvc, else `linux`. |
| `<arch>` | Mapped to the OS's usual triple by `CANONICAL_TRIPLES` (`windows/i686` = `i686-pc-windows-msvc`). Default `x86_64`. |
| `<triple>` | Full triple when the table can't express it (`linux/armv7-unknown-linux-gnueabihf/`). |
| `error/` | Every configuration errors. Mixed fixtures stay in the main tree. |
| `suites/<name>/` | Imported suites (`gcc-dg`, `gcc-torture`, `clang-test`), written by sweep tools with `--migrate`. gcc-torture and gcc-dg run under both `gcc/` and `clang/` (gcc-dg under `clang/` only where clang accepts it, with the test's `-std`). A test in one flavor only is either rejected by the other oracle or has a gap under `slate-parser-cxg`. |
| `inputs/` | Headers and sysroots reached via directives (`SLATE-FILECHECK-ISYSTEM`, `-I`, `--sysroot`, `-include`). Headers included relative to a fixture sit beside it, copied per directory. |

- One file covers one compiler and target; multi-config fixtures are one
  file per directory (`target_registry.c`). File names don't repeat the
  directory.
- A valid `--target`/`--flavor` in `SLATE-FILECHECK-ARGS` or `PREFIX-ARGS`
  must match the path. Invalid values are allowed to test option errors
  (`error/clang/linux/x86_64/target_unknown.c`).

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
