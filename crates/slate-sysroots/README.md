# Slate sysroots

This crate installs target sysroots for Slate. The library provides target
selection, installation, and path lookup. Manage sysroots through Slate's
`sysroot` command. Run the following commands from the workspace root:

Currently supported:

```sh
cargo run -p slate -- sysroot install x86_64-pc-windows-msvc
cargo run -p slate -- sysroot remove x86_64-pc-windows-msvc
cargo run -p slate -- sysroot install i686-pc-windows-msvc
cargo run -p slate -- sysroot path x86_64-pc-windows-msvc
cargo run -p slate -- sysroot doctor x86_64-pc-windows-msvc
cargo run -p slate -- sysroot install aarch64-pc-windows-msvc
cargo run -p slate -- sysroot doctor aarch64-pc-windows-msvc
cargo run -p slate -- sysroot install thumbv7a-pc-windows-msvc
cargo run -p slate -- sysroot install x86_64-unknown-linux-gnu
cargo run -p slate -- sysroot install aarch64-unknown-linux-gnu
cargo run -p slate -- sysroot install i686-unknown-linux-gnu
cargo run -p slate -- sysroot install armv7-unknown-linux-gnueabi
cargo run -p slate -- sysroot install armv7-unknown-linux-gnueabihf
cargo run -p slate -- sysroot install x86_64-unknown-linux-musl
cargo run -p slate -- sysroot install aarch64-unknown-linux-musl
cargo run -p slate -- sysroot install x86_64-unknown-freebsd
cargo run -p slate -- sysroot install aarch64-unknown-freebsd
cargo run -p slate -- sysroot install x86_64-linux-android
cargo run -p slate -- sysroot install aarch64-linux-android
cargo run -p slate -- sysroot install x86_64-apple-darwin --sdk /path/to/MacOSX.sdk
cargo run -p slate -- sysroot install aarch64-apple-darwin --sdk /path/to/MacOSX.sdk
cargo run -p slate -- sysroot install compiler-headers clang
cargo run -p slate -- sysroot install compiler-headers apple-clang
cargo run -p slate -- sysroot install compiler-headers gcc
cargo run -p slate -- sysroot install compiler-headers msvc
cargo run -p slate -- sysroot doctor compiler-headers clang
cargo run -p slate -- sysroot path compiler-headers gcc
cargo run -p slate -- sysroot path compiler-headers msvc x86_64-pc-windows-msvc
```

The Windows installer uses the `xwin` library to acquire the Microsoft CRT and
Windows SDK from the Visual Studio 2026 manifest. It asks you to accept the
Microsoft license on stdin; set `XWIN_ACCEPT_LICENSE` to accept it
non-interactively. Versions are pinned to match the `cl.exe` oracle: MSVC CRT
14.51 with Windows SDK 10.0.26100. `thumbv7a-pc-windows-msvc` is the exception:
MSVC 14.44 and SDK 10.0.22621 are the last releases with 32-bit ARM support.
The Universal CRT always comes from the pinned SDK. xwin on its own would pick
a standalone UCRT package that is older than the CRT, which breaks
`threads.h`. `doctor` checks for this mismatch, and `SYSROOT-MANIFEST.txt`
records the versions that were installed. The installed files are for local
use and are not bundled with Slate.

Installed sysroots live in Slate's platform-specific local data directory:

| Host    | Root                                     |
| ------- | ---------------------------------------- |
| Linux   | `${XDG_DATA_HOME:-~/.local/share}/slate` |
| macOS   | `~/Library/Application Support/Slate`    |
| Windows | `%LOCALAPPDATA%\Slate\data`              |

The installation is under `sysroots/<Rust target triple>/`. Downloaded `xwin`
packages go into the platform cache directory under `xwin/`. The public
`Paths::resolve` and `Paths::include_paths` methods supply sysroot paths for
Slate's translator. `Paths::include_paths_with_compiler` adds the selected
compiler headers before the target headers. MSVC CRT headers stay in the
sysroot alongside the Windows SDK headers; the installer does not duplicate
them under `compiler-headers`.

Compiler headers are installed under `compiler-headers/clang-22.1.8/include`
and `compiler-headers/gcc-16.2.0/<family>/include` in the same data directory.
The Clang installer requires `git` and copies the upstream release's resource
headers. GCC's headers differ by target, so the GCC installer writes one
include directory per header family: `x86` (i686 and x86_64), `aarch64` and
`arm`. It downloads the release archive into Slate's cache, checks its
SHA-256, and assembles what GCC's `stmp-int-hdrs` step would install for a
Linux target of each family: the `ginclude` headers, `limits.h` as
`limitx.h` + `glimits.h` + `limity.h`, `syslimits.h` from `gsyslimits.h`,
`stdint.h` from `stdint-wrap.h`, the family's `extra_headers` from
`gcc/config.gcc` (x86 also gets `mm_malloc.h` from `pmm_malloc.h`), and
`unwind.h` from libgcc's `unwind-generic.h`, or `config/arm/unwind-arm.h`
plus `unwind-arm-common.h` for arm. The x86 and arm families are
byte-identical to installed GCC 16.2 compilers for those architectures, minus
the runtime-library headers (`omp.h`, `gcov.h`, `sanitizer/`, ...). `install compiler-headers gcc`
installs every family; add `x86`, `aarch64` or `arm` to print or inspect one.
`install compiler-headers msvc` installs all supported Windows
MSVC sysroots and returns their existing CRT header paths. Add a Windows MSVC
target triple to install or inspect one architecture only.

On macOS, `install compiler-headers apple-clang` copies resource headers from
the active Xcode or Command Line Tools compiler into a versioned
`compiler-headers/apple-clang-<version>/include` bundle. The stable
`apple-clang-current/include` path points to that bundle. Apple Clang headers
are acquired locally for the user and are not distributed with Slate.

The supported Rust targets are `i686-pc-windows-msvc`,
`x86_64-pc-windows-msvc`, `aarch64-pc-windows-msvc`, and
`thumbv7a-pc-windows-msvc`, plus x86_64 and
aarch64 targets for Linux (glibc and musl), FreeBSD, Android, and macOS, and
`i686-unknown-linux-gnu`, `armv7-unknown-linux-gnueabi`, and
`armv7-unknown-linux-gnueabihf`. Each target has its own sysroot.

Linux installations use prebuilt sources. The glibc targets extract pinned
Debian cross packages and keep their `usr/<Debian triplet>` layout: i686 uses
Debian's i386 packages (`usr/i686-linux-gnu`), and the two armv7 targets use
armel (`usr/arm-linux-gnueabi`) and armhf (`usr/arm-linux-gnueabihf`). Debian
no longer builds armel cross packages, so it stays on Bookworm's glibc 2.36
like aarch64; x86_64, i686 and armhf use Sid's glibc 2.43. The musl
targets extract the prebuilt sysroot from pinned musl-cross archives. Slate uses
`Paths::include_paths` to find the C headers in either layout, so no generated
aliases or linker-script edits are needed for header translation. These Linux
target installations do not install versioned Clang or GCC resource headers;
those remain separate compiler bundles.

FreeBSD targets extract only `usr/include` and `COPYRIGHT` from the pinned
FreeBSD 15.1 base archives. Android targets download Google's NDK r27d Linux
ZIP to Slate's cache, verify its SHA-256, and extract only Bionic headers and
the NDK notice for the selected architecture. The same upstream ZIP supplies
headers on Linux, macOS, and Windows. FreeBSD and Android installs are header
only and do not provide link libraries or startup files.

macOS targets refer to a local SDK with a symlink at `sysroots/<target>/SDK`.
On macOS, the installer finds the active SDK through `xcrun`; on any host you
can set `OSX_CROSS_SDK` or pass `--sdk <path>`. The SDK must stay at that path
for `path` and `doctor` to succeed. The SDK is not copied or downloaded.
If macOS has no SDK, install Apple's Command Line Tools with
`xcode-select --install` and retry. On other hosts, `install` reports that
automatic SDK discovery is unavailable and points to the explicit SDK path.
Clang's framework search path is `SDK/System/Library/Frameworks` and must be
passed with `-F` when translating framework includes.

Slate and this crate now share a Cargo workspace. Automatic handoff of the
active macOS SDK to Slate's translator and linker remains future integration
work; Mac users currently provide the SDK with `OSX_CROSS_SDK` or `--sdk`.

`doctor` checks the expected header and library paths for each target. It
prints `✓` or `✗` for each group and exits unsuccessfully when any group is
missing.

The target and compiler-header acquisition flows now live in the Rust crate;
the shell scripts and their repository-local test fixtures have been removed.
Clang and GCC use pinned versions, and MSVC CRT headers are reused from the
sysroot instead of copied into a second bundle. New installs use Slate's
platform data directory.
