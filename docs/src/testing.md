# Testing

See [Tools](tools/index.md) for regenerating checks, sweeping projects, and
comparing Slate's results with a compiler.

Tests belong beside the layer whose behavior they check:

| Concern | Test location | What to assert |
| --- | --- | --- |
| Preprocessing, syntax, diagnostics, C types, and parser IR | `crates/slate-parser/tests/fixtures/` | Parser output or diagnostics with FileCheck. |
| IR analysis and transforms | `crates/slate/tests/fixtures.transforms/<passes>/` | Transformed IR shape. |
| C-to-Rust lowering and runtime behavior | `crates/slate/tests/fixtures/` | Native C and translated Rust have the same output and exit status; FileCheck pins generated Rust when needed. |
| Project generation, merging, or library behavior | Matching `crates/slate/tests/fixtures.*` suite | Generated files, build behavior, or runtime behavior for that suite. |

If a parser change affects the IR contract, add parser fixtures first, then
cover the consumer behavior in Slate. Parser fixtures establish what sema
produces; Slate fixtures establish how analysis or lowering uses it.

## Slate fixture directories

The suite name identifies the test harness. Supported and unsupported buckets
form a ratchet: supported cases must pass, and unsupported cases must continue
to fail until they are promoted.

| Directory | Purpose |
| --- | --- |
| `fixtures/` | Supported end-to-end C-to-Rust differential fixtures. Includes architecture and flavor subdirectories. |
| `fixtures.unsupported/` | Reproducers for constructs Slate does not yet translate. The test requires each case to fail; when it passes, move it to `fixtures/`. |
| `fixtures.ignored/` and suite-specific `*.ignored` | Cases Slate will never support. These are not run; each needs a `SLATE:` comment explaining why. |
| `fixtures.transforms/<passes>/` | IR snapshots after selected `slate-analysis` transforms. |
| `fixtures.merge/` | Multi-target `--merge` plans, generated files, and builds. |
| `fixtures.project/`, `fixtures.plan/` | Project translation behavior and project-plan output. |
| `fixtures.library/`, `fixtures.release/`, `fixtures.dylink/` | Generated library crates, release-mode behavior, and dynamic-linking behavior. Related `.unsupported` directories hold known failures. |
| `fixtures.host-include/` | Header lookup and host-include behavior. |
| `fixtures.arch-mode/` | Architecture-mode options and target selection. |
| `fixtures.gcc-torture/`, `fixtures.gcc-dg/`, `fixtures.c-testsuite/`, `fixtures.chibicc/` | Imported compiler test suites, split into supported and unsupported buckets where applicable. |

`.unsupported` means “not supported yet, and still expected to fail.”
`.ignored` means “intentionally excluded permanently.” Do not put a temporary
failure in `.ignored`; use the suite's unsupported bucket so the ratchet keeps
it visible.

## Parser fixtures

Parser tests live in `crates/slate-parser/tests/fixtures/`. Directory placement
selects compiler flavor and target; `error/` contains expected diagnostics,
`inputs/` holds shared input files, and `suites/` groups larger parser test
suites. A fixture declares what to print with `SLATE-FILECHECK-*` directives;
generated `CHECK` lines must not be edited by hand.

```bash
python3 crates/slate-parser/tools/update_filecheck.py --in-place <fixture>
cargo nextest r --release --profile parser
```

Use parser fixtures for preprocessing output, AST shape, semantic diagnostics,
and IR. Do not add unit tests for parser paths.

## Analysis fixtures

Analysis transforms run in Slate's frontend, so transform fixtures live in
`crates/slate/tests/fixtures.transforms/<passes>/`. The directory name selects
the passes. These fixtures check IR before and after analysis without involving
Rust code generation or a runtime comparison.

Use this suite when the expected behavior is a transform or analysis fact. If
the change also affects emitted Rust or program behavior, add an end-to-end
fixture under `crates/slate/tests/fixtures/` too.

## Lowering fixtures

`crates/slate/tests/fixtures/` tests supported C-to-Rust translation. The
harness compiles the native C and translated Rust, compares output and exit
status, and checks generated Rust where a fixture has FileCheck directives.
Put a new unsupported lowering case in `fixtures.unsupported/`, confirm the
expected barrier, then promote it when translation works.

Generate FileCheck blocks with:

```bash
python3 crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/<fixture>
```

## Running tests

Run release nextest profiles from the workspace root:

```bash
cargo nextest r --release --profile slate
cargo nextest r --release --profile parser
cargo nextest r --release --profile merge
```

Run the profile for each crate changed. Run `merge` when changing `--merge` or
`fixtures.merge/`. The Slate profile also runs c-testsuite, GCC torture, GCC
dg, and chibicc suites.

For a fixture that crosses the unsupported ratchet, use `SLATE_DIFF_FIXTURE` to
isolate it. Corpus suites use `SLATE_GCC_TORTURE_FIXTURE`,
`SLATE_GCC_DG_FIXTURE`, and `SLATE_C_TESTSUITE_FIXTURE`.
