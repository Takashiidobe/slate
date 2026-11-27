# Operating Systems

Pass the target triple to `slate translate --target` and use headers for that
target. The triples below are available through `slate sysroot install`. To
compile and link the generated Rust, use a toolchain for the same target and any
libraries the original program links against.

## Linux

- **glibc** — `x86_64-unknown-linux-gnu`, `i686-unknown-linux-gnu`,
  `aarch64-unknown-linux-gnu`, `armv7-unknown-linux-gnueabi`, and
  `armv7-unknown-linux-gnueabihf`.
- **musl** — `x86_64-unknown-linux-musl` and `aarch64-unknown-linux-musl`.

The Linux runtime differential tests cover glibc on x86-64, i686, AArch64, and
hard-float Armv7. The test harness can run the Arm binaries under QEMU. See
[Setup](../setup.md#choose-a-test-profile) for the profiles and tools.

## macOS

Use `x86_64-apple-darwin` for Intel or `aarch64-apple-darwin` for Apple silicon.
Slate uses a local macOS SDK from Xcode or the Command Line Tools. When
installing a sysroot on another host, pass `--sdk /path/to/MacOSX.sdk` to
register the SDK. See [Setup](../setup.md) for the install commands.

## Windows

The MSVC ABI targets are `x86_64-pc-windows-msvc`, `i686-pc-windows-msvc`,
`aarch64-pc-windows-msvc`, and `thumbv7a-pc-windows-msvc`. The sysroots include
the Microsoft CRT and Windows SDK headers. On these targets, C `long` is 32
bits, including on 64-bit Windows, and `long double` uses binary64.

The MSVC differential suite runs x86-64 and i686 programs and build-checks
AArch64. The parser fixtures also cover 32-bit Arm Windows.

## Android and FreeBSD (tier 2)

Slate has parser fixtures for these targets. Routine end-to-end runtime tests
are unavailable. The installed sysroots contain headers only. Supply the
target's own libraries and startup files when linking.

- **Android** uses the NDK's Bionic headers for `x86_64-linux-android` and
  `aarch64-linux-android`.
- **FreeBSD** uses the base system headers for `x86_64-unknown-freebsd` and
  `aarch64-unknown-freebsd`.
