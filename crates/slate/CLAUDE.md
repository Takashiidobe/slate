# Slate

Workspace rules (beads, testing from the root, session completion, commits)
live in [the root AGENTS.md](../../AGENTS.md). Run every command below from
the workspace root.

Slate translates C to Rust. Correctness is the only bar, and it is checked by
**differential testing**: compile and run both the C and the generated Rust,
then require identical stdout and exit code.

## Architecture

```
C -> slate-parser (preprocess, parse, sema) -> ir::Module
  -> src/frontend/lowerer.rs -> rust_ast::Program -> Rust source
```

- `crates/slate-parser` produces a typed `ir::Module` (places, bindings,
  explicit conversions, hoisted side effects). Slate consumes it directly as a
  library; never parse its printed IR.
- `src/frontend.rs` drives slate-parser;
  `src/frontend/lowerer.rs` lowers IR to `rust_ast`.
  `lower()` collects a `Barrier` (an unsupported `Construct` plus the IR
  node's `Site`) per function or global it cannot lower yet. A broken IR
  invariant aborts lowering with `InvalidIr`; it is never a barrier, and any
  test run that hits it panics.
- IR semantics are specified in [ir-spec](../../wiki/concepts/ir-spec.md) and
  its `wiki/concepts/ir/` subpages. Read the relevant subpage before lowering
  a new `ValueKind`, `Statement`, `PlaceKind`, or `Type`.
- [Slate architecture](../../wiki/concepts/slate-architecture.md) defines the
  single frontend and backend boundary. [Historical records](../../wiki/historical/index.md)
  preserve prior designs.

The frontend consumes slate-parser IR exclusively. `src/backend/` holds
rewrites and code generation. Rewrites are always on: `translate` and
`translate-project` both run the full backend. A project runs the
interprocedural rewrites once over all units, then the per-function rules per
unit. Executable projects (one unit defines `main`) may also lift exported
functions to pointer-ABI-identical types (`&T`, `&mut T`, `Box<T>`) and retype
other units' extern decls; address-taken, `main`, libc-named, and
specially-linked symbols keep their C signature. They also move structurally
identical types into `src/__slate_types.rs`, imported by each unit, and call an
export by Rust path instead of an extern decl when every decl matches its
definition (`src/backend/link/`); an address-taken export keeps its C ABI and
symbol. Library projects only change `static`
functions, so exported signatures never change.
`tests/fixtures.release/` fixtures go through `translate-project` and a
`cargo build --release` (`release_build_differential`), for behavior that only
shows up after LLVM optimization or across translation units: a `.c` file is a
one-unit project, a directory of `.c` files is a multi-unit project. A unit's
`.c` file may carry a `SLATE-FILECHECK` block, checked against that unit's
generated file; `update_filecheck.py` accepts these paths too.
`tests/fixtures.library/<name>/src/` projects have no `main`: `library_differential`
translates them as a `staticlib`, links each `tests/*.c` driver against the
archive, and compares it with an all-native build.
`tests/fixtures.dylink/<name>/` fixtures (`dylink`) split a prebuilt dependency
(`lib/*.c`, always built natively as a `.so` or `.dll`) from the host
(`host/*.c`) that loads it with `dlopen` or `LoadLibraryA` through
`tests/fixtures.dylink/dylink.h`. Each host is built natively as the oracle and
translated with `translate-project`; both run in the directory holding the
dependency and must match. `dylink_linux` always runs. `dylink_windows` builds
with `cl.exe` under wine and the translated host for `x86_64-pc-windows-msvc`
(linked by `lld-link` against msvc-wine's libs), and skips with a message when
`wine`, `lld-link`, msvc-wine, or the `x86_64-pc-windows-msvc` rustup target
(from `rust-toolchain.toml`) is missing; `SLATE_DYLINK_REQUIRE_WINDOWS=1` turns the skip into a failure.

## Toolchain

| Var              | Default                         | Role                                                                                                                                      |
| ---------------- | ------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| `SLATE_SYSROOTS` | `~/.local/share/slate/sysroots` | slate-parser reads target headers from `<dir>/<triple>`; when set, host targets skip native `clang` headers. Test harnesses always set it |
| `SLATE_CARGO`    | `cargo`                         | compiles the generated Rust                                                                                                               |
| `SLATE_RUSTFMT`  | `rustfmt`                       | fallback formatter when syn rejects generated Rust                                                                                        |

The Rust toolchain and test targets are pinned in the root
`rust-toolchain.toml` ([Toolchain](../../AGENTS.md#toolchain)).

Differential tests use `clang` on PATH as the C oracle. Slate frontend translation does not
invoke Clang.

Install a sysroot with `cargo run -p slate-c2rust -- sysroot install <triple>`.
Then run `python3 tools/clangd_flags.py` to write the gitignored
`compile_flags.txt` files, which point clangd at the same x86_64 linux
compiler headers and sysroot slate-parser uses.

## Debugging a fixture

```bash
cargo run --release -p slate-c2rust -- emit-slate-ir <file.c>                         # the IR slate receives
cargo run --release -p slate-c2rust -- translate <file.c>                            # Rust output
cargo run --release -p slate-c2rust -- lowering-barriers <file.c>                    # first barrier per function
```

## Testing

```bash
cargo nextest r --release --profile slate
```

The `slate` profile runs `differential` (`tests/fixtures`) plus the
c-testsuite, gcc-torture, gcc-dg, and chibicc corpus suites, all through the
slate frontend. `chibicc_suite` translates each two-TU fixture with
`translate-project`; external definitions become
`#[unsafe(no_mangle)]` items and the linker resolves symbols across modules.
Additional suites are tracked for restoration under `slate-p58o.6`; their
fixture directories remain.

Each suite is a ratchet: `tests/fixtures/` must pass, and
`tests/fixtures.unsupported/` must still fail. When an unsupported fixture
starts passing, `fixtures_unsupported_tests_still_fail` fails and prints the
`git mv` (from the workspace root) that promotes it. Corpus suites work the same way with their own
`*.unsupported` directories.

`generated_differential` runs once per target in `SLATE_TEST_TARGETS`
(default `x86_64-unknown-linux-gnu`). For each target it runs the
target-independent `tests/fixtures/*.c` plus `tests/fixtures/<arch>/`, where
`<arch>` is the triple's first component (`x86_64`, `i686`, `aarch64`), and
`tests/fixtures/gcc/` plus `tests/fixtures/gcc/<arch>/`, which translate with
`--flavor=gcc` and use real gcc as the oracle, and `tests/fixtures/msvc/` plus
`tests/fixtures/msvc/<arch>/`, which translate with `--flavor=msvc` and use
`crates/slate-parser/tools/cl.exe` under wine as the oracle (`MSVC_ARCH=x64`
for x86_64, `x86` for i686, `arm64` for aarch64). msvc fixtures translate for
`<arch>-pc-windows-msvc` and their Rust is built for that target (lld-link,
clang-cl and msvc-wine's libs, `CrossTarget::windows_msvc`) and run under
wine. wine cannot run ARM64 binaries on an x86_64 host, so aarch64 is
build-only (`CrossTarget::build_only`): cl.exe compiles the C, the Rust is
built and FileChecked, and neither runs. `tests/fixtures/msvc/aarch64/`
fixtures therefore check at compile time (`#error`, `_Static_assert` with
`/std:c11`), not with `abort()`. When that toolchain is missing, they
translate for the linux triple instead (the rustup targets come from
`rust-toolchain.toml`). Every msvc fixture passes `/Zc:preprocessor` in its
`dg-options`, since cl.exe defaults to the traditional preprocessor. Each
one checks its own results and calls `abort()` on a mismatch, so without
msvc-wine the Rust must just exit 0 with empty stdout. There are no
`.expected` files.
Profiles in `.config/nextest.toml` set the targets with setup scripts:

| Profile         | Targets                                                      | Runs                                                                                |
| --------------- | ------------------------------------------------------------ | ----------------------------------------------------------------------------------- |
| `slate`         | x86_64                                                       | every suite                                                                         |
| `slate-i686`    | i686, natively with `-m32`                                   | `generated_differential`, unsupported ratchet                                       |
| `slate-aarch64` | aarch64, under `qemu-aarch64-static`                         | `generated_differential`, unsupported ratchet                                       |
| `slate-targets` | all three                                                    | `generated_differential`, unsupported ratchet                                       |
| `clang`, `gcc`  | all three, that flavor's fixtures only (`SLATE_DIFF_FLAVOR`) | `generated_differential`, unsupported ratchet, slate-parser fixtures of that flavor |
| `msvc`          | x86_64 and i686, plus aarch64 build-only; msvc fixtures only | same, against cl.exe                                                                |

`CrossTarget::for_triple` in `tests/support/mod.rs` holds each target's C
oracle flags, Rust linker and emulator.
`tests/fixtures.unsupported/<triple>.txt` lists the target-independent
fixtures that still fail on that target. The run fails when a listed
fixture passes (remove it from the list) or an unlisted one fails.
`tests/fixtures.unsupported/` also runs per target (root plus `<arch>/`); a
fixture that passes prints the `git mv`, the `update_filecheck.py` command,
and the per-target list entries it needs. `tests/fixtures/arm/` is not collected yet (slate-5vj0.8).

Isolate one fixture while developing:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --profile slate --test differential -E 'test(generated_differential)' --nocapture
```

Find the first barrier for every unsupported fixture in every suite:

```bash
cargo nextest r --release --profile slate --ignore-default-filter -E 'package(slate-c2rust) & test(/_unsupported_triage_report$/)' --run-ignored ignored-only --no-capture
```

Corpus suites use their own selectors: `SLATE_GCC_TORTURE_FIXTURE`,
`SLATE_GCC_DG_FIXTURE`, `SLATE_C_TESTSUITE_FIXTURE`. Beads that promote
unsupported fixtures carry the `coverage` label.

Batch suites write translated Rust under `crates/slate/target/*-suite/` and
build it under `crates/slate/target/test-cache/`. A `could not parse/generate
dep info` error usually means one cache subdirectory is stale; remove only
that `target-*` directory and rerun.

## Workflow for a lowering change

1. Put the fixture in `tests/fixtures.unsupported/` (or find the existing one)
   and confirm it fails with the expected barrier: run the triage report with
   `SLATE_DIFF_FIXTURE=<name>`.
2. Implement the lowering until the triage report prints `PASS <name>`.
3. `git mv` it into `tests/fixtures/`.
4. Run the full `slate` profile. It must be green; promote any other fixtures
   the change makes pass.

## Workflow for a rewrite change

1. Add or extend a `tests/fixtures/` fixture and confirm the current output
   lacks the rewrite. Every fixture needs a `SLATE-FILECHECK` block; by
   default it checks the whole generated file.
2. Implement the rewrite, then run
   `crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/<name>.c`
   to regenerate its `SLATE-FILECHECK` block. Read the block: it should show
   the rewrite firing.
3. Run the full `slate` profile. FileCheck runs after runtime parity passes.
   For each `FileCheck failed` fixture, regenerate it with the tool and review
   the diff.

[FileCheck](../../wiki/concepts/differential-fixtures.md#filecheck) documents
the block format and directives. Never hand-edit a generated block.

## Conventions

- **Never comment.**
- **Every feature starts with a C fixture**, and it must fail before you
  write the fix.
- **Feature testing is e2e differential fixtures, never unit tests.**
- **Transliterate first, idiomatize later.** Baseline Rust may be ugly:
  `#[repr(C)]`, raw pointers, explicit temps, `libc`, and `unsafe` are all
  acceptable.
