# triplers

[![Crates.io](https://img.shields.io/crates/v/triplers.svg)](https://crates.io/crates/triplers)
[![Documentation](https://docs.rs/triplers/badge.svg)](https://docs.rs/triplers)
[![License](https://img.shields.io/crates/l/triplers.svg)](LICENSE)
[![Rust](https://img.shields.io/badge/rust-1.70%2B-blue.svg)](https://www.rust-lang.org)

> **Faster, safer, and more compact target triple parser than `target-lexicon` — supporting _every_ LLVM target, including the exotic ones.**

`triplers` is a Rust library for parsing, manipulating, and serializing target triples (e.g., `x86_64-unknown-linux-gnu-elf`). It aims to be a drop-in replacement for [`target-lexicon`](https://crates.io/crates/target-lexicon) with **broader coverage**, **better performance**, and **`no_std` support by design**.

## ✨ Features

- ✅ **Complete LLVM coverage** – supports all architectures, vendors, kernels, environments, and object formats known to LLVM (including `dxil`, `spirv`, `kalimba`, `ve`, `nvptx`).
- ⚡ **Fast & lightweight** – zero-allocation parsing, minimal code size, and optimized prefix matching.
- 🔒 **Safe Rust** – no `unsafe`, no dependencies except `strum` (with optional `serde`).
- 📦 **`no_std` ready** – works on embedded systems and kernels (disable default features).
- 🎯 **Round-trip canonicalization** – parse any triple and regenerate the canonical string.
- 🧩 **Modular design** – each triple component (`Arch`, `Vendor`, `Kernel`, `Env`, `Obj`) is a separate enum.
- 🧪 **Extensively tested** – over 200 test cases, including all LLVM triples.

## 📦 Installation

```bash
cargo add triplers
```

Enable `serde` support:

```bash
cargo add triplers --features serde
```

## 🚀 Quick Start

```rust
use triplers::Triple;
use std::str::FromStr;

fn main() {
    let triple = Triple::from_str("aarch64-unknown-linux-gnu-elf").unwrap();

    println!("Architecture: {}", triple.arch.canonicalize());  // "aarch64"
    println!("Vendor: {:?}", triple.vendor);                   // Some(Unknown)
    println!("Kernel: {}", triple.kernel.canonicalize());      // "linux"
    println!("Environment: {:?}", triple.env);                 // Some(GNU)
    println!("Object format: {:?}", triple.obj);               // Some(ELF)
    println!("Bitness: {} bits", triple.bitness());            // 64
}
```

## 🔍 Parsing & Display

```rust
use triplers::Triple;

// Parse a full triple with object format
let t = Triple::parse("wasm32-unknown-wasi-wasm").unwrap();
assert_eq!(t.to_string(), "wasm32-unknown-wasi-wasm");

// Missing vendor is allowed
let t = Triple::parse("armv7-linux-gnueabihf").unwrap();
assert_eq!(t.vendor, None);
assert_eq!(t.to_string(), "armv7-linux-gnueabihf");

// Freestanding (bare-metal) triples
let t = Triple::parse("arm-none-eabi").unwrap();
assert!(t.is_freestanding());

// Exotic LLVM targets
let t = Triple::parse("spirv1.6-unknown-unknown").unwrap();
assert_eq!(t.arch.canonicalize(), "spirv1.6");
```

## 🌍 Supported Target Components

### Architectures (partial list)

`x86`, `x86_64`, `arm` (v4t … v9.7a), `aarch64` (arm64, arm64e, arm64ec), `mips` (32/64, r6, el), `powerpc` (32/64, spe, el), `riscv32`, `riscv64`, `wasm32`, `wasm64`, `spirv` (1.0–1.6), `dxil` (1.0–1.9), `kalimba` (3/4/5), `ve`, `nvptx`, and more.

### Vendors

`unknown`, `apple`, `pc`, `microsoft`, `nvidia`, `intel`, `ibm`, `mesa`, `suse`, `scei`, `meta`, `kvin`, `freescale`, `imagtech`, `mipstech`, `openemb`, `csr`, `nodellvm`.

### Kernels / OSes

`linux`, `windows` (canonicalized to `win32`), `freebsd`, `openbsd`, `netbsd`, `dragonfly`, `darwin`, `macosx`, `ios`, `tvos`, `watchos`, `visionos`, `bridgeos`, `fuchsia`, `solaris`, `illumos`, `hermitcore`, `emscripten`, `wasi`, `wasip1`/`p2`/`p3`, `cuda`, `nvcl`, `amdhsa`, `vulkan`, `opencl`, `uefi`, `redox`, `haiku`, `rtems`, `none`, and many more.

### Environments / ABIs

`gnu`, `gnueabi`, `gnueabihf`, `musl`, `musleabi`, `musleabihf`, `android`, `msvc`, `llvm`, `itanium`, `coreclr`, `eabi`, `eabihf`, plus DXIL shader stages (`raygeneration`, `closesthit`, `pixel`, etc.).

### Object Formats

`elf`, `macho`, `coff`, `xcoff`, `goff`, `wasm`, `spirv`, `dxcontainer`.

_(See the [docs](https://docs.rs/triplers) for the complete list.)_

## 🛠️ Cargo Features

- **`std`** (enabled by default) – disables `#![no_std]` crate-level attribute.
- **`serde`** – derives `Serialize`/`Deserialize` for all triple components.

Disable default features for `no_std` environments:

```toml
[dependencies.triplers]
default-features = false
```

## 💡 Why another target triple library?

`target-lexicon` is great, but it doesn't cover the full LLVM spectrum. When working with exotic architectures like Kalimba, VE, or SPIR-V, you'd hit `UnknownVariant` errors. `triplers` fills that gap by:

- Including **every** target component defined in LLVM's `Triple.h`.
- Providing **canonical** string representations that round-trip exactly.
- Being **faster** because of careful parser design and minimal dependencies.

## 📄 License

Licensed under MIT license ([LICENSE])(LICENSE)

## 🤝 Contributing

Contributions are welcome! Please open an issue or a pull request. For LLVM updates, feel free to add new target components.

---

**Made with ❤️ for the Rust and LLVM ecosystems.**
