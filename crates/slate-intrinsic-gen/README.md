# slate-intrinsic-gen

Workspace tooling for the checked-in
[`intrinsics_table.rs`](../slate/src/frontend/lowerer/intrinsics_table.rs).
Normal Slate builds use that catalog without LLVM or C++ build dependencies.

Run from the workspace root:

```bash
cargo run -p slate-intrinsic-gen -- \
  --llvm-build /path/to/llvm-project/build \
  --llvm-src /path/to/llvm-project \
  --stdarch-src /path/to/rust/library/stdarch/crates/core_arch/src \
  --out crates/slate/src/frontend/lowerer/intrinsics_table.rs
cargo fmt
```

- Requires `bin/llvm-config`, `bin/llvm-tblgen`, LLVM libraries and headers,
  and a C++ compiler (`CXX`, default `c++`).
- `--llvm-src` records the LLVM commit. Without it, the source directory is
  obtained from `llvm-config --src-root`.
- Always includes general LLVM intrinsics; defaults to x86, AArch64, ARM,
  and RISC-V. Repeat `--prefix` to generate a restricted catalog for inspection;
  use all default prefixes for the frontend's checked-in catalog.
- The extractor reads LLVM signatures, immediate parameters, and overload
  positions. TableGen JSON supplies C builtin aliases; names are never guessed
  from C spellings.
- Optional `--stdarch-src` retains x86/x86_64 LLVM link-name signature overrides.
  Those describe internal stdarch declarations, not public `core::arch` APIs.
- The frontend currently consumes non-overloaded scalar signatures without
  immediate parameters. Other mapped calls produce lowering barriers.
