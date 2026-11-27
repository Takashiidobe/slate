# Generate the Intrinsic Catalog

`slate-intrinsic-gen` generates the checked-in catalog that maps supported C
builtin names to LLVM intrinsics. Normal Slate builds use the generated file.
Regeneration needs an LLVM build with `llvm-config`, `llvm-tblgen`, libraries,
and headers, plus a C++ compiler.

```sh
cargo run -p slate-intrinsic-gen -- \
  --llvm-build /path/to/llvm-project/build \
  --llvm-src /path/to/llvm-project \
  --stdarch-src /path/to/rust/library/stdarch/crates/core_arch/src \
  --out crates/slate/src/frontend/lowerer/intrinsics_table.rs
cargo fmt
```

Use matching LLVM sources and build artifacts. The generator records the LLVM
source revision. `--stdarch-src` supplies optional x86 and x86-64 signature
overrides from stdarch's internal LLVM declarations.

## Follow a catalog entry

1. LLVM's intrinsic API supplies the intrinsic name, signature, immediate
   parameters, and overload positions.
2. TableGen supplies any C builtin aliases. This connects a builtin spelling to
   an intrinsic entry without a naming heuristic.
3. Slate looks up a recognized builtin using its target and the declaration's
   builtin identity. It uses the operand and result types to resolve an
   overloaded signature. An immediate parameter still requires a constant
   argument in the generated Rust.

For example, the `intrinsic_immediate_args.c` fixture calls
`__builtin_ia32_xabort(3 + 4)`. The generated Rust uses the catalog's
`llvm.x86.xabort` link name and emits the argument as a `const` expression
converted to `i8`. The builtin spelling, LLVM name, and immediate operand
requirement all matter when reviewing this mapping.

The default catalog includes general, x86, AArch64, Arm, and RISC-V intrinsics.
Use an alternate `--out` and repeat `--prefix` for a narrower inspection run.
Use the default prefix set when updating the checked-in file. Review changed
aliases, signatures, overload positions, and immediate arguments, then run the
affected Slate differential fixtures.
