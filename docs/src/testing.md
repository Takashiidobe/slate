# Testing

Testing is organized around release `cargo nextest` profiles. Use `lowering`
for CIR/frontend changes, `rewrites` for backend fixups, and `libc` for
libc-shim or libc-test changes. Cross-target profiles are available for ARM32
and AArch64; the setup and environment requirements are documented in
[Setup](setup.md).

```bash
cargo nextest r --release --profile lowering
cargo nextest r --release --profile rewrites
cargo nextest r --release --profile libc
cargo nextest r --release --profile arm-lowering
cargo nextest r --release --profile arm-rewrites
cargo nextest r --release --profile aarch64-lowering
cargo nextest r --release --profile aarch64-rewrites
```

## Our own fixtures

The primary regression tests are C fixtures under `tests/fixtures/`. Each
fixture is compiled and run as C and generated Rust, with stdout and exit code
compared. Embedded FileCheck directives additionally assert generated-Rust
shape. See `wiki/concepts/differential-fixtures.md` for marker syntax and
regeneration commands.

## Chibicc's tests

[Chibicc](https://github.com/rui314/chibicc) has a set of tests that
compile in C11 mode that are used to test its original compiler. We also
run these tests, since they're a good suite for basic compliance. Some
are still failing, so that's also a WIP.

Tests are located at `tests/fixtures.chibicc`.

## Libc Test

The [libc test](https://wiki.musl-libc.org/libc-test) suite provides API
declaration checks and functional runtime checks. The API and functional
sub-suites have separate supported/unsupported buckets; use the commands in
`tests/fixtures.libc-test/README.md` when triaging one case.

Tests are located at `tests/fixtures.libc-test`.

## GCC Torture Tests

Slate uses GCC's [torture tests](https://gcc.gnu.org/onlinedocs/gccint/Torture-Tests.html)
after a Clang/CIR admission filter. Supported cases are in
`tests/fixtures.gcc-torture/`; tracked gaps are in
`tests/fixtures.gcc-torture.unsupported/`; deliberately untracked cases are in
`tests/fixtures.gcc-torture.ignored/`. Current counts and triage commands are
maintained in each corpus README and `wiki/concepts/gcc-torture-triage.md`.

To inspect one unsupported case:

```bash
SLATE_GCC_TORTURE_FIXTURE=<name> cargo nextest r --release --test gcc_torture_suite \
  -E 'test(gcc_torture_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

The same pattern applies to `SLATE_GCC_DG_FIXTURE` and
`SLATE_LIBC_TEST_FIXTURE` with their respective suite test names.

## Fuzzing

Fuzzing is done by creating C programs with
[yarpgen](https://github.com/intel/yarpgen), which are then translated
to rust and then compared for exit code and stdout:

We keep the seeds we've run for posterity.

Slate hasn't run into any errors in at least 1000 cases so that's good
but there's still a lot more seeds to run through:

```sh
./tools/yarpgen-fuzz.sh --continuous -n 4
```

## Suggestions for Improvements

Test count: Realistically it would be nice to remove redundant tests.
One way to do that is to use mutation testing or find some way to figure
out that tests are testing roughly the same thing.

Effects testing: making sure that C and the translated rust have
equivalent effects by having an interpreter that can read C and Rust and
make sure both sides agree on having "equivalent" effects, not just exit
codes and stdout/stderr. Slate used to have a version of this to test
that rewrites were valid, but this didn't catch many bugs and was
cumbersome to extend as new features were added, so this was removed. A
V2 that does better would be nice to have since it extends testing.

Implementing better branch testing: Slate has pretty decent line
coverage (through `cargo llvm-cov --branch nextest`). I'd like support
for `mc/dc` coverage, but rust doesn't have a way to do this yet.
There's a tracking issue here:
[Issue](https://github.com/rust-lang/rust/issues/124144)
