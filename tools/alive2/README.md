# C-to-Rust translation validation

Compare Clang's LLVM IR with Slate-generated Rust's LLVM IR using Alive2. Run
from the workspace root:

```sh
python3 tools/alive2/install.py
cargo build --release -p slate-c2rust
python3 tools/alive2/validate.py crates/slate/tests/fixtures
```

The validator recursively attempts every `.c` file and discovers functions
automatically, including static helpers and `main`. It writes per-fixture
artifacts and an aggregate `report.json` under a new `target/alive2/run-*`
directory. Reports are updated after each fixture.

## Select fixtures

```sh
python3 tools/alive2/validate.py crates/slate/tests/fixtures \
  --glob=conditionals.c --glob=bitand.c
python3 tools/alive2/validate.py tools/alive2/fixtures/scalars.c -f add
python3 tools/alive2/smoke.py
```

Pass multiple files or directories. Repeat `--glob` to select relative paths,
`--function` to select functions, or `--c-arg=-fwrapv` to supply compiler flags.
Use `--limit` for a pilot batch and `--output` for a new artifact directory.
Unconditional `dg-options` and `dg-additional-options` are read from fixtures.
Target-conditional directives are reported as unsupported.

## Read results

| Status        | Meaning                                                                                                                  |
| ------------- | ------------------------------------------------------------------------------------------------------------------------ |
| `validated`   | Alive2 validated the selected function within its model and bounds.                                                      |
| `incorrect`   | Alive2 produced a counterexample; inspect the IR and assumptions.                                                        |
| `unsupported` | Missing function correspondence, unmatched runtime calls, unsupported instruction, or unsupported fixture configuration. |
| `timeout`     | A command or SMT query exceeded its limit.                                                                               |
| `unknown`     | Alive2 did not establish refinement.                                                                                     |
| `tool_error`  | Translation, compilation, IR parsing, or another tool failed.                                                            |

Exit codes are `0` only when every selected fixture is validated, `1` when a
counterexample is reported, and `2` for incomplete coverage or tool failures. A
run with no checked functions never succeeds.

Each fixture retains generated Cargo sources, raw and normalized `.ll` files,
symbol mappings, tool versions, command arguments, and stdout/stderr logs.
`alive-*.stdout` contains counterexamples. `pair-*` directories contain the
exact comparison inputs.

## Scope

- This checks Slate's existing translation and rewrite pipeline. It does not
  disable rewrites or modify generated Rust signatures.
- Normalization uses `always-inline,sroa,mem2reg`. Other functions remain
  available for separate validation.
- Comparison copies rename corresponding LLVM symbols, including the Rust
  implementation of C `main`. The generated startup wrapper is excluded.
- Remaining identical calls are treated according to Alive2's call model;
  different calls require relational models. For example, `printf` becoming Rust
  formatting currently reports unsupported.
- Loops use `--unroll` (default `4`); success is bounded translation validation,
  not an unbounded proof. `--smt-timeout` is milliseconds per query; `--timeout`
  is seconds per tool invocation.
- GCC/MSVC fixtures are recorded as unsupported: Clang's IR does not certify
  their compiler-specific semantics. Architecture directories select their Linux
  target; `--target` overrides it. Required Rust targets must be installed.
- Functions absent from emitted IR cannot be validated. C emission requests
  otherwise-unused declarations; missing Rust counterparts remain unsupported.
- Clang, rustc, and normalization are trusted. LLVM refinement does not prove
  Rust-specific validity obligations that rustc does not encode in LLVM.

## Tools

`install.py` installs `~/.local/bin/alive-tv`. It pins Alive2 revision
`01a5ec45c8152995755f7331827407a9de19f262` for LLVM 23 and adjusts CMake to link
the shared LLVM library. It requires Git, CMake, Ninja, a C++ compiler, lld,
LLVM development files, Z3 development files, and re2c. Source, build files, and
`installation.json` remain under `~/.local/share/alive2`.

Override executables with `--clang`, `--rustc`, `--opt`, `--cxxfilt`, `--cargo`,
`--slate`, and `--alive-tv`, or the corresponding `SLATE_*` environment
variables. All LLVM major versions must match. The default Slate binary is
`target/test-cache/release/slate`.

To check a deliberately wrong Rust implementation:

```sh
python3 tools/alive2/validate.py tools/alive2/fixtures/scalars.c -f add \
  --rust tools/alive2/fixtures/wrong.rs
```

[Alive2 documentation](https://github.com/AliveToolkit/alive2) describes its
supported semantics and limitations.
