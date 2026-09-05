These are runnable `gcc.dg` tests that pass the Clang compile-and-run gate but
are not yet passing Slate differential testing.

The full-corpus admission filter is `tools/filter-gcc-dg.sh`. It keeps
`dg-do run` cases whose Clang binary compiles and exits zero, excludes any
`dg-options` or `dg-additional-options` optimization flag, and skips cases
that declare additional source files. Local quoted helper includes are
inlined into staged fixtures before Slate translation. The 2026-09-05 batch
admitted 1,167 cases and found 214 genuine Slate failures; 209 new failures
are now tracked here.

The suite reports an unexpected pass so cases can be promoted to
`fixtures.gcc-dg` when their lowering and generated Rust are correct.
