# slate-intrinsic-gen

[slate-intrinsic-gen](https://github.com/Takashiidobe/slate-intrinsic-gen)
is a crate that generates `src/frontend/lowerer/intrinsics_table.rs`.

Given that there are thousands of intrinsics to generate, it's something
to automate. Shimming intrinsics doesn't work quite right, so we need to
map the C intrinsic to rust.

`slate-intrinsic-gen` builds a small C++ extractor
(`cpp/extract_intrinsics.cpp`) against a given LLVM build using
`llvm-config --cxxflags/--ldflags/--libs`. The extractor links against
`llvm/IR/Intrinsics.h` for a given prefix (`x86`, `aarch64`, `arm`,
`riscv`), walks every `Intrinsic::ID` with that prefix, decodes its IIT
type descriptors (`Intrinsic::IITDescriptor`), and prints one JSON object
per intrinsic: name, whether it's overloaded, return type, parameter
types, which parameters are immediate args, and which positions drive
overload resolution.

The Rust half shells out to that extractor per prefix, collects the JSON,
and renders it into a single generated Rust file:
`IntrinsicSignature`/`IntrinsicParam` structs plus one
`&[IntrinsicSignature]` static per prefix (e.g. `X86_INTRINSICS`,
`AARCH64_INTRINSICS`). It also adds the source LLVM commit as a header
comment when `--llvm-src` is given.

## stdarch overrides

x86/x86_64 intrinsics exposed by Rust's `core::arch` (`stdarch`) don't
always call the LLVM intrinsic under the exact name or signature you'd
expect from `Intrinsics.h`. Some are wrapped or renamed via
`#[link_name = "llvm.x86...."]`. When `--stdarch-src <core_arch/src dir>`
is passed, `slate-intrinsic-gen` also greps every `.rs` file under `x86/`
and `x86_64/` for `#[link_name = "llvm.x86...\"]` functions, extracting
the Rust-side parameter and return types.

These are emitted as a separate `X86_STDARCH_OVERRIDES` static, so the
lowerer can prefer stdarch's own signature over the raw LLVM one when one
exists.

## Usage

```bash
cd slate-intrinsic-gen
cargo r -- \
  --llvm-build ~/llvm-project/build-cir \
  --llvm-src ~/llvm-project \
  --stdarch-src rust/library/stdarch/crates/core_arch/src \
  --out slate/src/frontend/lowerer/intrinsics_table.rs
```

`--prefix` may be repeated to restrict which ISAs are regenerated. With no
`--prefix` given it generates all four (`x86`, `aarch64`, `arm`, `riscv`).
