# Differential fixtures

<!-- toc -->
- [Suites](#suites)
  - [Targets](#targets)
- [Supported/unsupported ratchet](#supportedunsupported-ratchet)
- [Triage](#triage)
- [Workflow](#workflow)
- [Per-fixture compiler flags](#per-fixture-compiler-flags)
- [FileCheck](#filecheck)
<!-- /toc -->

Fixtures are C programs. Slate compiles and runs the C source with
`clang` on PATH (the oracle) and the generated Rust, then requires identical
stdout and exit status. Runtime parity is the only correctness gate.

## Suites

`generated_differential`, `gcc_torture_suite`, `gcc_dg_suite`, and
`c_testsuite_suite` translate every fixture through the slate-parser frontend
(`support::translate_slate`, i.e. `slate translate`).
That runs the backend rewrite pipeline. Headers come from slate-sysroots.
Corpus suites run for the host target only. Together with `chibicc_suite`,
these suites make up the `slate` nextest profile:

```bash
cargo nextest r --release --profile slate
```

Run it from the workspace root. `chibicc_suite` uses `translate-project` for
two-TU fixtures. Additional suite coverage is tracked for restoration in
`slate-p58o.6.7` through `slate-p58o.6.17`; the fixture directories remain.

### Targets

`generated_differential` runs once per target in `SLATE_TEST_TARGETS`
(comma-separated, default `x86_64-unknown-linux-gnu`). Each target runs the
target-independent `tests/fixtures/*.c` plus `tests/fixtures/<arch>/`, where
`<arch>` is the triple's first component (`arm` for `armv7`). Put a fixture in an arch directory
only when it cannot be target-independent (inline asm, ISA intrinsics).
`tests/fixtures/gcc/` and `tests/fixtures/gcc/<arch>/` hold gcc-flavor
fixtures: they translate with `--flavor=gcc` and the oracle is real gcc
(`gcc -m32` for i686, `aarch64-linux-gnu-gcc` for aarch64,
`arm-linux-gnueabihf-gcc` for armv7). Leave
`--flavor=gcc` out of their `dg-options`. `tests/fixtures/msvc/` and
`tests/fixtures/msvc/<arch>/` translate with `--flavor=msvc`, and the oracle
is real `cl.exe` under wine ([MSVC oracle](msvc-oracle.md), `MSVC_ARCH=x64`
for x86_64, `x86` for i686 and `arm64` for aarch64). They translate for
`<arch>-pc-windows-msvc`, so Windows predefines and LLP64 layout match the
oracle, and the Rust is built for that target with lld-link, clang-cl and
msvc-wine's `LIB`/`INCLUDE` and run under wine, as a separate batch. wine
on an x86_64 host cannot run ARM64 PE binaries, so aarch64 is build-only:
cl.exe compiles the C, the Rust builds and is FileChecked, and nothing runs.
`msvc/aarch64/` fixtures check at compile time instead (`#error`, and
`_Static_assert` with `/std:c11`). Without that toolchain or rustup
target they translate for the linux triple. Each passes `/Zc:preprocessor`
in `dg-options`, because cl.exe's default is the traditional preprocessor,
which slate never emulates.
They also check themselves, calling `abort()` on a mismatch: when wine or
msvc-wine is missing, the Rust must exit 0 with empty stdout.
A Windows FileCheck block lists `<arch>-pc-windows-msvc`;
`update_filecheck.py` runs the matching linux target to produce it. The `.unsupported` tree has the same `gcc/` and `msvc/`
directories. Nextest
profiles pick targets through setup scripts in `.config/nextest.toml`:

```bash
cargo nextest r --release --profile slate-i686      # natively, with -m32
cargo nextest r --release --profile slate-aarch64   # under qemu-aarch64-static
cargo nextest r --release --profile slate-arm       # armv7-unknown-linux-gnueabihf under qemu-arm-static
cargo nextest r --release --profile slate-targets   # x86_64, i686, aarch64 and armv7
cargo nextest r --release --profile clang           # clang fixtures of both crates
cargo nextest r --release --profile gcc             # gcc fixtures of both crates
cargo nextest r --release --profile msvc            # msvc fixtures of both crates
```

`SLATE_DIFF_FLAVOR=<clang|gcc|msvc>` limits `generated_differential` and
the unsupported ratchet to that flavor's fixtures: the root and arch
directories for `clang`, `gcc/` or `msvc/` otherwise. `gcc` and `msvc`
also skip the check that every `<triple>.txt` entry ran, since those lists
name clang fixtures. Each flavor profile sets it, adds slate-parser's
fixtures of that flavor, and runs x86_64, i686 and aarch64 (`msvc`: aarch64
build-only).

`tests/fixtures.unsupported/<triple>.txt` lists target-independent fixtures
that still fail on that target. A listed fixture that passes, or an unlisted
one that fails, fails the run. `fixtures_unsupported_tests_still_fail` and
`fixtures_unsupported_triage_report` also run once per target, over
`tests/fixtures.unsupported/*.c` plus `tests/fixtures.unsupported/<arch>/`, and
the target profiles include the ratchet. A fixture that passes on any target
prints its `git mv`, the `update_filecheck.py` command for the targets it
passed on, and a `<triple>.txt` entry for each target it still fails on. Other environments and OSes (musl, FreeBSD,
Android, macOS, Windows) are tracked in `slate-5vj0.8`.

## Supported/unsupported ratchet

Each suite has a supported and an unsupported bucket. Supported fixtures must
pass; unsupported fixtures must still fail. When an unsupported fixture starts
passing, `*_unsupported_tests_still_fail` fails and prints the `git mv` that
promotes it.

| Suite       | Supported                    | Unsupported                              | Selector                    |
| ----------- | ---------------------------- | ---------------------------------------- | --------------------------- |
| fixtures    | `tests/fixtures`             | `tests/fixtures.unsupported`             | `SLATE_DIFF_FIXTURE`        |
| gcc-torture | `tests/fixtures.gcc-torture` | `tests/fixtures.gcc-torture.unsupported` | `SLATE_GCC_TORTURE_FIXTURE` |
| gcc-dg      | `tests/fixtures.gcc-dg`      | `tests/fixtures.gcc-dg.unsupported`      | `SLATE_GCC_DG_FIXTURE`      |
| c-testsuite | `tests/fixtures.c-testsuite` | `tests/fixtures.c-testsuite.unsupported` | `SLATE_C_TESTSUITE_FIXTURE` |

Paths are relative to `crates/slate`. `tests/fixtures/<arch>/` and
`tests/fixtures.unsupported/<arch>/` run only for that architecture's targets.
`*.ignored` buckets (`tests/fixtures.ignored`, `tests/fixtures.gcc-dg.ignored`,
...) hold only features Slate will never support, each with a `SLATE:`
comment saying why; no test reads them. Selectors take a fixture stem.

## Triage

Every suite has an ignored `*_unsupported_triage_report` test. Each prints
one `PASS`/`FAIL` line per unsupported fixture, with the first barrier
classified as `parse/sema`, `unsupported lowering` (with the construct, for
example `function main: unprototyped definition`), `rustc`, or
`runtime mismatch`. Run all of them at once, or one fixture with a selector:

```bash
cargo nextest r --release --profile slate --ignore-default-filter \
  -E 'package(slate-c2rust) & test(/_unsupported_triage_report$/)' --run-ignored ignored-only --no-capture
SLATE_DIFF_FIXTURE=<stem> cargo nextest r --release -p slate-c2rust --test differential \
  -E 'test(fixtures_unsupported_triage_report)' --run-ignored ignored-only --no-capture
```

With `--profile slate-targets`, the `tests/fixtures.unsupported` report
covers every target. Coverage beads carry the `coverage` label.

`slate lowering-barriers [compiler args] <file.c>` lists every function
defined in one translation unit with `ok` or its first lowering barrier, plus
`<module>` lines for top-level barriers (unsupported globals, top-level asm).
It never emits Rust and exits non-zero when any barrier exists. Strict
translation (`translate`) still fails on the first
barrier. Record and enum definitions only block the functions that use them;
sema already resolves typedefs, so the module's type list is not a gate.

## Workflow

1. Put the fixture in the unsupported bucket and confirm the triage report
   shows the expected barrier.
2. Implement the lowering until the report prints `PASS <stem>`.
3. `git mv` the fixture into the supported bucket.
4. Run the full `slate` profile and promote anything else the change fixed.

## Per-fixture compiler flags

`dg-options "..."` comments in a fixture add compiler flags to both
slate-parser and the C oracle; `dg-additional-options "..."` adds them to
slate-parser only. A `{ target ... }` clause that does not match the host
triple drops the directive.

- Own fixtures (`tests/fixtures*`) keep every flag, so a fixture can test
  a flag slate does not accept yet (`fixtures.unsupported/flag_*.c`).
- Imported gcc-dg and gcc-torture tests keep only semantic flags (`-std=`,
  `-O0`..`-O3`, and `is_semantic_dg_flag` in `tests/support/mod.rs`),
  because their options are mostly warnings and codegen.
- A flag the clang oracle rejects goes in the `gcc/` or `msvc/` directory
  of the compiler that accepts it (`gcc/flag_unsigned_bitfields.c`,
  `msvc/x86_64/flag_msvc_unsigned_char.c`).

## FileCheck

Every `tests/fixtures/` fixture (including the arch directories) pins the
Rust that the rewrite pipeline emits in a trailing
`// SLATE-FILECHECK-BEGIN <triple>...` ... `// SLATE-FILECHECK-END` block.
The header lists the targets the block covers. Runtime parity is still the
correctness gate. A check runs only after the fixture's runtime comparison
passes. A mismatch, or a fixture with no block, fails
`generated_differential`. A run for a target the header does not list checks
runtime parity only.

`tests/fixtures.release/` blocks are optional. A unit's `.c` file that has
one is checked by `release_build_differential` against that unit's generated
file (`src/main.rs` for the unit that defines `main` and for single-file
projects, otherwise `src/<stem>.rs`). The shared `src/__slate_types.rs` of an
executable project has no block; the release build checks it.

`tests/fixtures.plan/` fixtures pin `translate-project --plan` output instead
of Rust. `plan_filecheck` (`tests/project_plan.rs`) treats each `.c` file as a
one-unit project, using `<stem>.json` as its matrix when present and
`matrix.json` otherwise, and runs it once per matrix target. The checked text
is stdout, stderr and the exit status, then an `exhaustive:` line comparing the
plan with every feature subset. A `// plan-args:` line replaces the arguments,
which is how usage errors are pinned. The test also requires the same output at
`SLATE_JOBS=1`. Nothing is compiled, so these fixtures have no runtime gate.

Generate the block from the workspace root:

```bash
crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/<name>.c
```

The tool runs the harness, then rewrites the block. By default the block
checks the whole generated file: a `CHECK` on its first line, `CHECK-NEXT`
lines for the rest, and a final `// CHECK-NOT: {{.}}` so appended output fails
too. Generated names (`__v12`, `__slate_slot_3`) become FileCheck variables,
so renumbering does not break checks. With no arguments the tool regenerates
every fixture. `--no-run` reuses the Rust from the last test run. Fixture
names, as a failing run prints them (`<triple>/<name>`), work as arguments too.

`--target <triple>` (repeatable) sets the targets a block covers. Without it
the tool keeps the targets the header lists, or uses
`x86_64-unknown-linux-gnu` (an arch directory's own target for arch
fixtures). With several targets, lines every target emits identically get
`CHECK`; lines that differ get that target's prefix, for example:

```c
// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu aarch64-unknown-linux-gnu
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn is_negative(c: i8) -> i32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn is_negative(c: u8) -> i32 {
// CHECK-NEXT: }
```

Each run reads only `CHECK` and its own target's lines.

When a whole-file block is a problem for a fixture, narrow it to specific C
functions:

```c
// @check-fn-begin
static int count(int n) { ... }
// @check-fn-end
```

Each marked function then gets a `CHECK-LABEL` on its Rust signature,
`CHECK-NEXT` lines for its body, and a closing `// CHECK: {{^}}}`.

Review the regenerated block: it is the claim that a rewrite fired. When a
change alters output, the `slate` profile reports `FileCheck failed` for each
affected fixture. Regenerate exactly those and read the diff.

Prefixes are `CHECK` and the upper-cased target triple. Supported directives
are `CHECK`, `-LABEL`, `-NEXT`, `-SAME`, `-EMPTY`, `-DAG` and `-NOT`. A label
block must end with `// CHECK: {{^}}}`, and generated Rust must never contain
a directive. Prefer the tool over hand-written checks. The old
`@lowering`, `@rewrite` and `@slate-lowerer` markers are gone.
