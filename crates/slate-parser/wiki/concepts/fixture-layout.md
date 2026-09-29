# Fixture layout

A fixture's directory is the only thing that sets its compiler and target.
Both `tests/filecheck.rs` (`fixture_placement`) and
`tools/update_filecheck.py` (`placement`) read it the same way, and each
fails on a path that doesn't fit:

```
tests/fixtures/[error/ | suites/<name>/]<flavor>/[<os>/[<arch> | <triple>]]/<fixture>.c
```

- `<flavor>` is `gcc`, `clang` or `msvc`, and it is required. No fixture
  lives at the root, so nothing runs with an implicit compiler or target.
- `<os>` is `linux`, `windows`, `darwin`, `android` or `freebsd`. It
  defaults to `windows` for msvc and `linux` otherwise.
- `<arch>` maps to that OS's usual triple through `CANONICAL_TRIPLES`
  (`windows/i686` is `i686-pc-windows-msvc`), and defaults to `x86_64`.
  Anything the table can't express uses the full triple as the directory
  instead, like `linux/armv7-unknown-linux-gnueabihf/`.
- `error/` holds fixtures where every configuration errors. A fixture with
  both passing and error configurations stays in the main tree.
- `suites/<name>/` holds imported test suites (`gcc-dg`, `gcc-torture`,
  `clang-test`), kept apart because their sweep tools write there with
  `--migrate`. gcc-torture runs under both `clang/` and `gcc/`, since the
  IR differs between the flavors for about a quarter of it. gcc-dg does
  too: `gcc/` holds the language-feature tests gcc accepts, and `clang/`
  holds the same tests wherever clang also accepts them, each with the
  test's own `-std`. A test that only one flavor has is either rejected by
  that compiler's oracle or has a slate gap filed under `slate-parser-cxg`.
- `inputs/` holds headers and sysroots that fixtures reach through a
  directive path (`SLATE-FILECHECK-ISYSTEM`, `-I`, `--sysroot`,
  `-include`). A header reached relative to the fixture (`#include "x.h"`)
  sits beside it instead, and is copied into each directory that uses it.

Every existing fixture sits at a full `<flavor>/<os>/<arch>` leaf. The
shorter forms are there for new fixtures. One file covers exactly one
compiler and target: a fixture that needs several is one file per
directory, as with `target_registry.c`.

`SLATE-FILECHECK-FLAVOR` no longer exists. A valid `--target` or `--flavor`
in `SLATE-FILECHECK-ARGS` or `PREFIX-ARGS` must match the path, or the
fixture fails. Invalid values stay allowed so that option-parsing errors
remain testable (`error/clang/linux/x86_64/target_unknown.c`).

File names don't repeat the directory: `ms-winnt-keywords.c` exists under
both `clang/windows/x86_64/` and `msvc/windows/x86_64/`.

## Ids in generated expectations

Ids numbered over the whole translation unit, system headers included, are
matched by FileCheck variables instead of literal numbers, so one more header
declaration doesn't restamp every fixture that includes the header.

- AST dumps: tag, file and node ids become numeric variables (`FILE0`,
  `TAG0`, `NODE0`) in first-use order (`loosen_ids`).
- IR dumps: `@typeN` and `%N` become string variables named after the declared
  name: `TYPE__IO_FILE`, `VALUE_stderr` (`loosen_ir_ids`). `_` is the only
  separator FileCheck allows, so the kind prefix runs into a leading `_`.
  Repeated names count up (`VALUE_x_2`), and unnamed entities get ordinals
  (`TYPE0`, `VALUE0`). A `.strN` global name reuses its binding's variable.
  Quoted strings and asm `template:` lines are left alone, because their
  `%N` are operand indices.

Every IR type and binding has exactly one definition line, and blocks are
checked with `-NEXT`, so a use still has to name the same entity as before.
Only the absolute number stops mattering: a changed cross-reference fails,
and renumbering alone doesn't. String variables, unlike numeric ones, can be
reused on the line that defines them.

### Checking: the harness matcher, not FileCheck

`tests/filecheck/matcher.rs` checks the output itself. FileCheck turns any
line with a `[[...]]` or `{{...}}` into a regex that LLVM compiles per line with
a slow regex engine. The time grows with line length times the text searched,
and even faster with the number of captured variables. A 256-parameter
signature (`cpp__embed-14.c`) took 5.5s to check, and 20KB constants with one
variable each (`bitint-39.c`) took 3.7s. Rust's `regex` fixed those two, but it
was no faster on `20001226-1.c`'s 16k ordinary lines, because building a new
regex for every line is the cost. Splitting long lines with `-SAME` was tried
and dropped: it churned 228 fixtures and left the rest of the suite as slow.

The matcher accepts exactly what the generator emits:
- **Directives:** plain `PREFIX:` and `PREFIX-NEXT:`.
- **Variables:** `[[V:[0-9]+]]` and `[[V]]` string variables, and `[[#V:]]` and
  `[[#V]]` numeric ones.
- **`{{...}}` bodies:** `[0-9]+`, `.*`, `\[[0-9, ]+\]`, and the three escapes.

Anything else is an explicit "not supported" error, so if the generator gains a
construct, the matcher needs to learn it too. Each piece is a literal, a digit
run or a variable, so no regex is involved. It is stricter than FileCheck: a
check must match its whole output line once horizontal whitespace is collapsed.
A plain check matches the first such line after the previous match. On a
mismatch the harness also runs real FileCheck on the same input for its
annotated dump.
