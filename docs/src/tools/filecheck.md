# Regenerate FileCheck Expectations

Slate has two `update_filecheck.py` scripts. Choose the one that owns the
fixture. Review the generated diff after changing a parser contract, an IR
transform, or the shape of emitted Rust.

## Slate parser fixtures

Preview the changes, then pass `--in-place` to update the fixture. The preview
prints a unified diff and exits with status 1 when the checks would change.

```sh
python3 crates/slate-parser/tools/update_filecheck.py \
  crates/slate-parser/tests/fixtures/clang/linux/x86_64/add.c
python3 crates/slate-parser/tools/update_filecheck.py --in-place \
  crates/slate-parser/tests/fixtures/clang/linux/x86_64/add.c
```

The script uses `target/test-cache/release/slate-parser`. The fixture's location
selects its compiler flavor and target. `SLATE-FILECHECK-DEFINES` names a run
and its defines, `SLATE-FILECHECK-ARGS` selects what to print or adds compiler
arguments, and `SLATE-FILECHECK-ERROR` selects an expected error. The `add.c`
fixture's `DEFAULT` block checks the IR. A file without a recognized fixture
directive can be skipped.

## Slate translation fixtures

```sh
python3 crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/add.c
```

The Slate updater runs the owning harness and writes the check block into the
fixture. A standard differential fixture checks generated Rust. Paired
`@check-fn-begin` and `@check-fn-end` markers restrict the check to the selected
functions. A fixture in `fixtures.transforms/<passes>/` checks transformed IR,
and project fixtures can check plans, reports, or generated files.

- Pass a fixture path or the fixture ID printed by a failing test. With no
  paths, the tool updates all its supported fixtures.
- Repeat `--target <triple>` to choose the block's targets. By default it
  retains the targets on the existing block or uses the fixture's default.
  Multi-target blocks use common `CHECK` lines and target-specific prefixes.
- Use `--no-run` to reuse artifacts from a previous harness run. Use the normal
  run after a change that affects the generated artifact.

## Review the diff

Check that the updated lines pin the intended behavior. Generated ID captures
allow incidental renumbering. An unexpected changed type, operation, or
diagnostic is a reason to revisit the fixture or implementation. Then run the
owning test profile as described in [Testing](../testing.md).
