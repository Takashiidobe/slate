# Cross-target toolchains

Slate's cross-target differential tests compile the C oracle and generated Rust
for the same target, then run both programs under a user-mode emulator. A
target setup therefore needs all of the following:

- a Clang/CIR target triple and a target libc sysroot;
- a target linker and startup libraries for the Rust binary;
- an installed Rust target;
- a QEMU user-mode runner for executing the target binary; and
- libc-shim declarations and layouts matching that target's C ABI.

The sysroot is part of the ABI contract. Do not use host headers or libraries
as a substitute for the target sysroot: a successful link against the wrong
libc can still produce a binary with incorrect layouts or symbol behavior.

## Musl oracle sysroots

The libc ABI probes use a real target musl installation rather than the musl
source tree or the host libc. Build the four supported musl targets with:

```bash
SLATE_MUSL_SYSROOT_ROOT="$HOME/toolchains/slate-musl" \
SLATE_MUSL_BUILD_ROOT="$HOME/toolchains/slate-musl/build" \
./tools/bootstrap-musl-sysroots.sh
```

The script builds x86-64, i686, ARM32 hard-float, and AArch64 sysroots under
`$SLATE_MUSL_SYSROOT_ROOT/{x86_64,i386,arm,aarch64}`. It records the musl
revision and compiler in `.slate-musl-target`, and refuses to reuse a build
directory after its source or compiler changes. Override `MUSL_SOURCE`,
`SLATE_MUSL_ARM_CC`, or `SLATE_MUSL_AARCH64_CC` when the local toolchain is in
a nonstandard location.

These are the musl oracle inputs for libc-shim probes. Glibc probes must use
the corresponding real glibc target sysroots; neither libc is a substitute for
the other. Compile-only probes require the headers and libraries, while runtime
differential tests additionally require the target loader and QEMU.

The shared ABI probe is run twice for one target and compared as normalized JSON:

```bash
python3 tools/libc-abi-probe.py run \
  --compiler "$HOME/toolchains/slate-musl/x86_64/bin/musl-clang" \
  --target x86_64-linux-musl \
  --sysroot "$HOME/toolchains/slate-musl/x86_64" \
  --extra-arg=-static \
  --output /tmp/musl-oracle.json

python3 tools/libc-abi-probe.py compare /tmp/musl-oracle.json /tmp/musl-shim.json
```

The shim invocation adds `--shim libc-shim/include` and the exact Slate target
defines. Records are keyed by kind and name, so a size and alignment for the
same type cannot overwrite one another. The comparison rejects different probe
sources before comparing records.

## Target matrix

| Target | Clang triple | Rust target | Linker | QEMU | Slate status |
| --- | --- | --- | --- | --- | --- |
| ARM32 GNU hard-float | `armv7-linux-gnueabihf` | `armv7-unknown-linux-gnueabihf` | `arm-none-linux-gnueabihf-gcc` or distro equivalent | `qemu-arm-static` | implemented |
| AArch64 GNU | `aarch64-linux-gnu` | `aarch64-unknown-linux-gnu` | `aarch64-linux-gnu-gcc` | `qemu-aarch64-static` | implemented |
| i686 GNU | `i686-linux-gnu` | `i686-unknown-linux-gnu` | Clang with `-m32` | `qemu-i386-static` or native execution | implemented |

The i686 target is 32-bit x86 (`target_arch = "x86"`), not x86-64's x32
ABI. The Rust target and the Clang target must both select the ordinary i386
32-bit GNU ABI.

## ARM32

Install the Rust target and an ARM GNU toolchain with its sysroot. The
toolchain used by this workspace has the `arm-none-linux-gnueabihf` prefix;
distribution packages may use `arm-linux-gnueabihf` instead.

```bash
rustup target add armv7-unknown-linux-gnueabihf
export SLATE_ARM_SYSROOT="$HOME/toolchains/<arm-toolchain>/arm-none-linux-gnueabihf/libc"
export SLATE_ARM_LINKER="$HOME/toolchains/<arm-toolchain>/bin/arm-none-linux-gnueabihf-gcc"
export SLATE_ARM_QEMU=qemu-arm-static
```

Run the two ARM profiles:

```bash
cargo nextest r --release --profile arm-lowering
cargo nextest r --release --profile arm-rewrites
```

`arm-lowering` enables raw lowering through `NEXTEST_PROFILE`; the rewrites
profile exercises the normal backend fixups as well. Select one fixture while
developing with `SLATE_DIFF_FIXTURE=<name>`.

The runner passes `--target=armv7-linux-gnueabihf` and the sysroot to Clang,
uses the configured linker for Cargo, and invokes `qemu-arm-static -L
<sysroot>` for execution.

## AArch64

Install the Rust target and an AArch64 GNU toolchain:

```bash
rustup target add aarch64-unknown-linux-gnu
export SLATE_AARCH64_SYSROOT=/usr/aarch64-linux-gnu
export SLATE_AARCH64_LINKER=aarch64-linux-gnu-gcc
export SLATE_AARCH64_QEMU=qemu-aarch64-static
```

Run the profiles:

```bash
cargo nextest r --release --profile aarch64-lowering
cargo nextest r --release --profile aarch64-rewrites
```

The runner passes `--target=aarch64-linux-gnu` and the sysroot to Clang,
configures Cargo with the AArch64 linker, and invokes `qemu-aarch64-static -L
<sysroot>` for execution. `SLATE_AARCH64_CC` can override the C compiler.

## i686

i686 uses the same runner structure as ARM32 and AArch64, with Arch's native
multilib layout:

```bash
rustup target add i686-unknown-linux-gnu
export SLATE_I686_SYSROOT=/usr/i686-linux-gnu
export SLATE_I686_CC=clang
export SLATE_I686_LINKER=clang
export SLATE_I686_QEMU=qemu-i386-static
```

The runner uses:

```text
SLATE_TARGET=i686-unknown-linux-gnu
Clang: --target=i686-linux-gnu -m32
Cargo: CARGO_TARGET_I686_UNKNOWN_LINUX_GNU_LINKER=clang
Cargo linker flags: --target=i686-linux-gnu -m32
QEMU: qemu-i386-static -L $SLATE_I686_SYSROOT
```

The `i686-lowering` and `i686-rewrites` nextest profiles accept
`SLATE_I686_CC`, `SLATE_I686_LINKER`, `SLATE_I686_SYSROOT`, and
`SLATE_I686_QEMU`, matching the ARM and AArch64 naming scheme. The linker is
Clang itself; target-specific Cargo Rust flags pass `--target=i686-linux-gnu`
and `-m32` to the linker driver, so no wrapper script is needed.

The host may be x86-64, but every object and library in the link must be
32-bit. `clang -m32` is useful for local probes when multilib is installed.
On Arch's native multilib layout, `/usr` is not a relocatable cross sysroot:
passing `--sysroot=/usr` would make Clang search for `/usr/usr/lib32`.
Therefore the runner should select i686 with `--target` and `-m32`, while
letting the Arch compiler driver find `/usr/lib32`; `/usr` is used only as
QEMU's `-L` runtime root.

## libc-shim work

The existing `Architecture::X86` path already covers the basic 32-bit
architecture selection. Adding an i686 differential target still requires
checking its ABI against the real sysroot. Prioritize:

- pointer, `long`, `size_t`, and alignment widths;
- glibc `struct stat`, directory, signal, pthread, and time layouts;
- `jmp_buf`, `ucontext`, and other opaque storage sizes;
- i386 `long double` and floating-point environment types;
- variadic argument representation;
- feature-test macros, symbol aliases, and 32-bit syscall constants.

Keep glibc and musl branches separate. In particular, do not assume that an
ARM32 or RISC-V32 layout is valid for i386 merely because all three targets
are ILP32. Validate each layout with `sizeof` and `offsetof` probes compiled
against the real sysroot and the shim.

## Inline assembly

i686 reuses the x86 assembler dialect, but it is not just x86-64 with smaller
register names. Target-specific fixtures should cover:

- `eax` through `edi`, `esp`, and `ebp` register names;
- 32-bit addressing and the absence of RIP-relative addressing;
- GCC `q`/`Q` byte-register constraints;
- high-byte registers `ah`, `bh`, `ch`, and `dh`;
- the `A` constraint's `edx:eax` pair semantics in 32-bit mode;
- x87 and MMX constraints;
- 32-bit PIC sequences involving `ebx`.

The Rust backend uses `target_arch = "x86"` and x86 register classes for
i686. Do not reuse x86-64 assumptions about available registers, byte
registers, operand-size suffixes, or pointer-sized operands.

## Validation checklist

For a new target, validate in this order:

1. Compile a `sizeof`/`offsetof` probe with the real target compiler and the
   shim.
2. Translate and run one small fixture against the C oracle under QEMU.
3. Add target-specific fixture checks and regenerate FileCheck output.
4. Run the target's lowering profile, then its rewrites profile.
5. Run the libc profile for header and ABI changes.
6. Run `cargo fmt` and `cargo clippy` before closing the target work.

The differential-fixture marker and regeneration workflow is documented in
[differential fixtures](differential-fixtures.md).
