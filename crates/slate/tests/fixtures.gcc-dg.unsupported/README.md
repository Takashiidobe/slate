These are runnable `gcc.dg` tests that pass the Clang compile-and-run gate but
are not yet passing Slate differential testing.

The suite reports an unexpected pass so cases can be promoted to
`fixtures.gcc-dg` when their lowering and generated Rust are correct.
