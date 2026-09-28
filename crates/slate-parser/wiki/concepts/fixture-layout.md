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
  instead, like `linux/armv7-unknown-linux-gnueabihf/` or
  `linux/i386-unknown-linux-gnu/`.
- `error/` holds fixtures where every configuration errors. A fixture with
  both passing and error configurations stays in the main tree.
- `suites/<name>/` holds imported test suites (`gcc-dg`, `gcc-torture`,
  `clang-test`), kept apart because their sweep tools write there with
  `--migrate`. gcc-torture runs under both `clang/` and `gcc/`, since the
  IR differs between the flavors for about a quarter of it.
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
