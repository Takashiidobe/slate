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

The shared ABI probe source lives at `tests/fixtures.abi-probe/linux-libc-abi.c`
and is compiled twice per target -- once against the real target libc headers
(the oracle) and once against `libc-shim/include` (the candidate) -- then the
two runs' `kind\tname\tvalue` record streams are diffed in memory. This whole
matrix is a single native Rust nextest test, `tests/libc_abi_matrix_suite.rs`
(backed by `tests/support/libc_probe.rs`), not an external script:

```bash
cargo nextest r --release --profile libc -E 'test(libc_abi_matrix)'
```

By default it runs the full 4-arch x {musl, glibc} matrix, since that
completes in about a second. On this development host, `tests/support/libc_probe.rs`
has defaults for the installed glibc sysroots and linkers; environment
variables override those defaults:

```bash
export SLATE_GLIBC_SYSROOT_X86_64=/path/to/x86_64/sysroot
export SLATE_GLIBC_SYSROOT_X86=/path/to/i686/sysroot
export SLATE_GLIBC_SYSROOT_ARM=/path/to/arm/sysroot
export SLATE_GLIBC_SYSROOT_AARCH64=/path/to/aarch64/sysroot
```

Narrow to one target/libc/family for focused checks during local iteration
with `SLATE_LIBC_ABI_LIBC`, `SLATE_LIBC_ABI_ARCH`, and `SLATE_LIBC_ABI_FAMILY`
(comma-separated for multiple arches/families):

```bash
SLATE_LIBC_ABI_LIBC=musl SLATE_LIBC_ABI_ARCH=aarch64 SLATE_LIBC_ABI_FAMILY=pthread \
  cargo nextest r --release --profile libc -E 'test(libc_abi_matrix)'
```

Available ABI families are `pthread`, `setjmp-ucontext`, `socket-epoll`,
`sched`, and `stat-time`.

A failure panics with every per-record `FAIL name: oracle=... candidate=...`
line, so the first ABI/declaration mismatch is visible directly in the test
output, not just a compilation failure. A target whose toolchain prerequisites
are missing (no sysroot, no linker, no QEMU) fails with a clear "is missing"
message rather than a compile/link error.

The local defaults are `/` for x86-64 and i386, the checked-in ARM GNU
toolchain's libc directory for ARM32, and `/usr/aarch64-linux-gnu` for
AArch64. For non-native glibc targets, set the matching linker using either
the `SLATE_GLIBC_LINKER_*` name or the existing target linker variable such as
`SLATE_ARM_LINKER` or `SLATE_AARCH64_LINKER` when using a different machine.

The x86-64 glibc probe uses dynamic linking when the sysroot does not provide
static `libc.a`; static linking is not required for an ABI oracle. i686 probes
can execute natively on an x86-64 Linux kernel, so the matrix does not route
them through QEMU. ARM32 and AArch64 still require QEMU for execution.

AArch64 musl links with a fully manual `-nostdlib -nostartfiles -nodefaultlibs`
sequence (explicit `crt1.o`/`crti.o`/`crtn.o` plus a `--start-group -lc -lgcc
-lgcc_eh --end-group`) instead of the `musl-gcc` wrapper's own specs file,
because the host's `aarch64-linux-gnu-gcc` defaults pull in a nonexistent
`-latomic_asneeded`. Override that linker with `SLATE_MUSL_AARCH64_LINKER`
(default `aarch64-linux-gnu-gcc`). x86-64 and i386 musl link through the
sysroot's own `bin/musl-clang` wrapper instead; that wrapper only preserves a
caller's `-l*`/`-L*` flags when the caller supplies at least one `-l` flag
itself (it wraps the *whole* argument list in `-l-user-start`/`-l-user-end`
markers the first time it sees `-l*` anywhere), so callers must pass an
explicit `-Wl,--start-group -lc -lgcc -Wl,--end-group` rather than relying on
Clang's own default library injection, or the link silently drops `libc`.
`tests/support/libc_probe.rs::resolve` already encodes all of this per arch;
reuse it rather than re-deriving the link line.

QEMU selection can be overridden per architecture regardless of libc with
`SLATE_LIBC_QEMU_<ARCH>` (`X86_64`, `X86`, `ARM`, `AARCH64`), which must point
at an existing executable when set.

## Runtime differential matrix

The ABI matrix above only proves layout and declaration parity; it does not
compile or run an arbitrary C program end to end. `tests/libc_runtime_matrix_suite.rs`
(`libc_runtime_matrix` test) closes that gap: for every (arch, libc) pair it
translates a handful of small, architecture-portable fixtures, builds both the
C oracle (via the same `libc_probe::resolve` toolchain used by the ABI matrix)
and the generated Rust (via a matching Cargo cross-target), runs both under
the same QEMU runner (or natively), and asserts stdout and exit status match.
It is deliberately light -- three fixtures, not the full `tests/fixtures/`
corpus -- so it stays fast enough to run every time the `libc` profile does.

```bash
cargo nextest r --release --profile libc -E 'test(libc_runtime_matrix)'
```

Like the ABI matrix, it runs the full 4-arch x {musl, glibc} matrix by default
and skips (not fails) any target whose sysroot, cross linker, or installed
Rust target is missing. Narrow it the same way:

```bash
SLATE_LIBC_RUNTIME_LIBC=musl SLATE_LIBC_RUNTIME_ARCH=aarch64 SLATE_LIBC_RUNTIME_FIXTURE=alias_global \
  cargo nextest r --release --profile libc -E 'test(libc_runtime_matrix)'
```

The Rust side needs the matching `rustup target add` for each musl/glibc
triple (`x86_64-unknown-linux-musl`, `i686-unknown-linux-musl`,
`armv7-unknown-linux-musleabihf`, `aarch64-unknown-linux-musl`, and the
`*-gnu` glibc equivalents already covered by the target matrix below). musl
Rust targets need no custom sysroot -- rustup's musl `rust-std` component is
self-contained -- just a linker driver (`SLATE_MUSL_RUST_CC`, default
`clang`) and, for ARM32/AArch64, `-fuse-ld=lld` since the host's default
linker does not understand those `ld` emulation modes. The glibc Rust
cross-targets reuse the same `config.linker`/`config.linker_args` that
`libc_probe::resolve` already validated for the C oracle, so no separate
glibc-specific Rust env vars are needed.

## Standards baseline vs. glibc oracle vs. musl oracle

Slate's own C headers (`libc-shim/include`) are the *standards baseline*: a
standards-compliant (mostly musl-shaped, POSIX-first) set of declarations that
Slate compiles user C against with `-nostdlib`, and that the generated Rust's
`libc` crate bindings must match at the ABI level. See
[libc](../../docs/src/libc.md) for why this baseline exists and why glibc and
musl can disagree on a function's exact signature (`strerror_r` is the classic
example).

The *glibc oracle* and *musl oracle* are the real system headers/libraries for
each libc, used only to check the standards baseline for correctness -- never
as a substitute for each other, and never as a substitute for the shim in
translated output. The ABI matrix compiles the same probe source against an
oracle and against the shim (`-D__SLATE_LIBC_SHIM`) and diffs the two record
streams; the runtime matrix compiles the *fixture under test* against an
oracle only (the shim is exercised indirectly, through the `libc` crate, on
the Rust side). A target passing the ABI matrix does not by itself prove the
runtime matrix passes, and vice versa: the ABI matrix catches layout/signature
drift, the runtime matrix catches behavioral drift (wrong lowering, wrong
codegen) that layout-correct headers can still hide.

## Cache locations

Both libc test suites cache their build artifacts under `target/` and reuse
them across runs (nextest does not clean between runs):

- `target/libc-abi-probe/<libc>-<arch>-{oracle,shim}/` -- ABI matrix probe
  objects, executables, and JSON records, one directory per (libc, arch,
  oracle-or-shim) combination.
- `target/libc-runtime-matrix/<libc>-<arch>/<fixture>/` -- runtime matrix
  translated Rust, the C oracle object/executable, the Cargo batch crate, and
  run output, one directory per (libc, arch, fixture) combination.
- `target/test-cache/<mangled-project-path>/` -- the Cargo `--target-dir` used
  by every generated-crate build across the test suites, including the
  runtime matrix's per-fixture Cargo projects (`test_target_dir_for_project`
  in `tests/support/mod.rs`). This is where `slate`'s own release binary ends
  up during testing; `./target/release/slate` built by a plain `cargo build`
  is a different, stale binary.
- `$SLATE_MUSL_SYSROOT_ROOT` (default `$HOME/toolchains/slate-musl`) -- the
  bootstrapped musl sysroots themselves, outside `target/` since they are a
  toolchain, not a build artifact.

Deleting a specific stale subdirectory (e.g. after a toolchain upgrade) is
safe; deleting all of `target/` forces every suite to rebuild everything.

## Target matrix

| Target | Clang triple | Rust target | Linker | QEMU | Slate status |
| --- | --- | --- | --- | --- | --- |
| ARM32 GNU hard-float | `armv7-linux-gnueabihf` | `armv7-unknown-linux-gnueabihf` | `arm-none-linux-gnueabihf-gcc` or distro equivalent | `qemu-arm-static` | implemented |
| AArch64 GNU | `aarch64-linux-gnu` | `aarch64-unknown-linux-gnu` | `aarch64-linux-gnu-gcc` | `qemu-aarch64-static` | implemented |
| i686 GNU | `i686-linux-gnu` | `i686-unknown-linux-gnu` | Clang with `-m32` | native execution | implemented |

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
