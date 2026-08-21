# Testing

Testing is done a few different ways:

## Our own fixtures

While implementing features, Slate grew regression tests to handle some
behavior.

## Chibicc's tests

[Chibicc](https://github.com/rui314/chibicc) has a set of tests that
compile in C11 mode that are used to test its original compiler. We also
run these tests, since they're a good suite for basic compliance. Some
are still failing, so that's also a WIP.

Tests are located at `tests/fixtures.chibicc`.

## Libc Test

The [libc test](https://wiki.musl-libc.org/libc-test) test suite is used
to test musl for regression tests in compliance. Slate passes all the
api tests, but fails some of the functional ones currently. This is a
WIP.

Tests are located at `tests/fixtures.libc-test`.

## GCC Torture Tests

Slate uses a comprehensive suite provided by gcc called the [torture
tests](https://gcc.gnu.org/onlinedocs/gccint/Torture-Tests.html). This
is a list of ~2000 c files that exercise obscure C compliance. We first
compared `gcc` vs `clang` here and found `clang` passes ~1500 of
them. Since we're using Clang IR as the basis for our lowering pass, we
only took the torture tests that clang could pass and used that as our
corpus. As of writing, there are 35 unsupported cases.

Some are ignored in the dir (`tests/fixtures.gcc-torture.ignored`).

There are three such cases now:

- `ieee__cdivchkd.c` (complex operations depend on which complex library
  you link to; libgcc and compiler-rt give different results for this
  test).
- `ieee__cdivchkf.c` (same reason as above).
- `strlen-5.c` (this tries to reach into another array, which is UB and
  will panic in rust, but works in C).

Supported tests are located at `tests/fixtures.gcc-torture.supported`
Unsupported tests are located at `tests/fixtures.gcc-torture.unsupported`

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
