# Extending Slate's C Testing

Slate should combine strict external test cases with generated differential
testing. For every runnable program, compile and run the original C, translate
it with Slate, compile and run the generated Rust, and compare stdout, stderr,
and termination exactly.

## Recommended Sources

1. [Csmith](https://github.com/csmith-project/csmith) — Add reproducible seeded generation, begin with a restricted profile matching Slate's supported subset, and run a broader non-gating profile to discover new gaps.
2. [c-testsuite](https://github.com/c-testsuite/c-testsuite) — Import its portable `single-exec` cases first, using their expected output and language/libc tags to build a strict supported-versus-unsupported corpus.
3. [GCC C torture tests](https://github.com/gcc-mirror/gcc/tree/master/gcc/testsuite/gcc.c-torture/execute) — Adapt the portable execution cases while initially excluding compile-only, diagnostic, target-specific, and special-driver tests.
4. [LLVM test-suite](https://github.com/llvm/llvm-test-suite) — Add `SingleSource` programs before `MultiSource` programs to expand from focused execution tests into realistic programs with reference outputs.
5. [libc-test](https://repo.or.cz/libc-test.git) — Use the functional and regression cases selectively to test preservation and idiomatization of libc behavior; translating musl itself is a later end-to-end milestone.

## Differential Runner

Each run should record the corpus revision or Csmith version, generator seed and
flags, target, compiler versions, source, generated Rust, compiler diagnostics,
and execution results. Apply the same arguments, stdin, environment, working
directory, resource limits, and timeout to both binaries.

Compare raw stdout bytes, raw stderr bytes, and the complete termination state:
a normal exit code, a terminating signal, or a timeout. For stronger reference
validation, require the C program to produce the same result under Clang at
`-O0` and `-O2`, and preferably GCC, before treating that result as the oracle.

Classify failures by stage:

- Reference C compilation or reference disagreement
- Slate translation failure
- Generated Rust compilation failure
- Runtime output or termination mismatch
- Timeout

Supported cases should gate CI. Unsupported cases should remain categorized and
executed so that an unexpected pass requires promotion instead of leaving a
stale skip. Reduce every generated failure to a small C fixture and retain its
seed and original artifacts for reproduction.
