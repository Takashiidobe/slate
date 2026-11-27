# Setup

Run builds and tests from the workspace root. Rustup selects the pinned nightly,
components, and Rust targets from `rust-toolchain.toml`.

## Build Slate

Install a native C toolchain and its development libraries. On Linux, have `gcc`
and the system linker on `PATH`. Install Cargo nextest to run the tests.

```sh
cargo install cargo-nextest --locked
cargo build --release -p slate-c2rust
```

The release binary is `target/test-cache/release/slate`.

## Set up test tools on Linux

Install the tools for the profiles you run and make sure their executables are
on `PATH`. If you install a tool outside the system directories, add its
containing directory to `PATH` in your shell's startup configuration.

- `clang` compiles C fixtures for comparison with the generated Rust.
- `gcc` compiles GCC-specific fixtures for comparison.
- `FileCheck` checks parser and translator output. Some distributions put it in
  a separate LLVM tools directory.
- GCC multilib support and 32-bit C development libraries let Clang and GCC link
  i686 binaries with `-m32`.
- `aarch64-linux-gnu-gcc` links AArch64 Linux binaries. Install its target C
  libraries at `/usr/aarch64-linux-gnu`.
- `qemu-aarch64-static` runs AArch64 Linux binaries.
- `arm-linux-gnueabihf-gcc` links ARMv7 Linux binaries. Install a toolchain with
  a sysroot that `arm-linux-gnueabihf-gcc -print-sysroot` can locate.
- `qemu-arm-static` runs ARMv7 Linux binaries.
- `wine` runs MSVC and the Windows test executables. Enable support for both x86
  and x86-64 executables.
- `clang-cl` compiles C runtime code for generated Windows crates.
- `lld-link` links the generated Windows binaries.
- `llvm-lib` creates Windows library archives.

### Install MSVC and the Windows SDK

Use
[msvc-wine](https://github.com/mstorsjo/msvc-wine#build-instructions-for-local-installation)
to install MSVC and the Windows SDK in `~/.local/share/msvc-wine/opt/msvc`. The
harness uses that location to find `cl.exe` for x64, x86, and ARM64 and selects
the compiler for each test.

### Install test headers

After building Slate, install the headers for the compiler flavors and targets
you test.

#### Clang and GCC compiler headers

```sh
target/test-cache/release/slate sysroot install compiler-headers clang
target/test-cache/release/slate sysroot install compiler-headers gcc
```

#### Linux

```sh
target/test-cache/release/slate sysroot install x86_64-unknown-linux-gnu
target/test-cache/release/slate sysroot install i686-unknown-linux-gnu
target/test-cache/release/slate sysroot install aarch64-unknown-linux-gnu
target/test-cache/release/slate sysroot install armv7-unknown-linux-gnueabihf
target/test-cache/release/slate sysroot install armv7-unknown-linux-gnueabi
target/test-cache/release/slate sysroot install x86_64-unknown-linux-musl
target/test-cache/release/slate sysroot install aarch64-unknown-linux-musl
```

#### Windows

These commands install the MSVC and Windows SDK headers that Slate reads. Use
the MSVC and SDK installation above to compile and link the test programs.

```sh
target/test-cache/release/slate sysroot install compiler-headers msvc x86_64-pc-windows-msvc
target/test-cache/release/slate sysroot install compiler-headers msvc i686-pc-windows-msvc
target/test-cache/release/slate sysroot install compiler-headers msvc aarch64-pc-windows-msvc
target/test-cache/release/slate sysroot install compiler-headers msvc thumbv7a-pc-windows-msvc
```

#### FreeBSD and Android

```sh
target/test-cache/release/slate sysroot install x86_64-unknown-freebsd
target/test-cache/release/slate sysroot install aarch64-unknown-freebsd
target/test-cache/release/slate sysroot install x86_64-linux-android
target/test-cache/release/slate sysroot install aarch64-linux-android
```

#### macOS

On macOS, Slate locates the SDK from Xcode or the Command Line Tools. On Linux,
append `--sdk /path/to/MacOSX.sdk` to each command to use a local SDK.

```sh
target/test-cache/release/slate sysroot install x86_64-apple-darwin
target/test-cache/release/slate sysroot install aarch64-apple-darwin
```

### Install Rust sources

Install `rust-src` to run the freestanding tests in the full `slate` profile.

```sh
rustup component add rust-src
```

## Choose a test profile

```sh
cargo nextest run --release --profile slate
```

Replace `slate` with the profile you need. Run `slate` and `parser` before
committing work that affects both crates. Add `merge` for changes to `--merge`
or its fixtures.

| Profile    | What it runs                                                                                               | What to install                                                                                                                                                                        |
| ---------- | ---------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `slate`    | Translator differential, corpus, and project tests, including cross-target tests                           | `clang`, `gcc`, `FileCheck`, `rust-src`, i686 multilib, `aarch64-linux-gnu-gcc`, `qemu-aarch64-static`, `wine`, MSVC, Windows SDK, `clang-cl`, `lld-link`, `llvm-lib`, fixture headers |
| `parser`   | Parser and preprocessor fixtures across compiler flavors                                                   | `FileCheck`, fixture headers                                                                                                                                                           |
| `merge`    | Multi-target project merge fixtures and host comparisons                                                   | `clang`, `FileCheck`, fixture headers                                                                                                                                                  |
| `clang`    | Both crates' Clang fixtures on x86-64, i686, and AArch64 Linux                                             | `clang`, `FileCheck`, i686 multilib, `aarch64-linux-gnu-gcc`, `qemu-aarch64-static`, fixture headers                                                                                   |
| `gcc`      | Both crates' GCC fixtures on x86-64, i686, and AArch64 Linux                                               | `gcc`, `clang`, `FileCheck`, i686 multilib, `aarch64-linux-gnu-gcc`, `qemu-aarch64-static`, fixture headers                                                                            |
| `msvc`     | Both crates' MSVC fixtures. Runs x86-64 and i686 Windows programs and build-checks ARM64 Windows programs. | `FileCheck`, `wine`, MSVC, Windows SDK, `clang-cl`, `lld-link`, `llvm-lib`, fixture headers                                                                                            |
| `i686`     | Translator differential fixtures on i686 Linux                                                             | `clang`, `FileCheck`, i686 multilib, fixture headers                                                                                                                                   |
| `aarch64`  | Translator differential fixtures on AArch64 Linux                                                          | `clang`, `FileCheck`, `aarch64-linux-gnu-gcc`, `qemu-aarch64-static`, fixture headers                                                                                                  |
| `arm`      | Translator differential fixtures on ARMv7 Linux                                                            | `clang`, `FileCheck`, `arm-linux-gnueabihf-gcc`, `qemu-arm-static`, fixture headers                                                                                                    |
| `targets`  | Translator differential fixtures on x86-64, i686, AArch64, and ARMv7 Linux                                 | `clang`, `FileCheck`, i686 multilib, `aarch64-linux-gnu-gcc`, `qemu-aarch64-static`, `arm-linux-gnueabihf-gcc`, `qemu-arm-static`, fixture headers                                     |
| `mutation` | Host differential tests on mutated IR                                                                      | `clang`, host C toolchain, fixture headers                                                                                                                                             |
| `default`  | Workspace tests outside the dedicated merge and mutation tests                                             | `slate` profile dependencies, `parser` profile dependencies                                                                                                                            |

See [Testing](testing.md) for fixture selection and triage.
