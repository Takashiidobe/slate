# Slate sysroots

This crate installs target sysroots for Slate. The library owns target selection,
installation, and path lookup; the binary is a small command-line facade.

Currently supported:

```sh
cargo run -- install x86_64-pc-windows-msvc
cargo run -- remove x86_64-pc-windows-msvc
cargo run -- install i686-pc-windows-msvc
cargo run -- path x86_64-pc-windows-msvc
cargo run -- doctor x86_64-pc-windows-msvc
cargo run -- install aarch64-pc-windows-msvc
cargo run -- doctor aarch64-pc-windows-msvc
cargo run -- install thumbv7a-pc-windows-msvc
cargo run -- install x86_64-unknown-linux-gnu
cargo run -- install aarch64-unknown-linux-gnu
cargo run -- install x86_64-unknown-linux-musl
cargo run -- install aarch64-unknown-linux-musl
cargo run -- install x86_64-unknown-freebsd
cargo run -- install aarch64-unknown-freebsd
cargo run -- install x86_64-linux-android
cargo run -- install aarch64-linux-android
cargo run -- install x86_64-apple-darwin --sdk /path/to/MacOSX.sdk
cargo run -- install aarch64-apple-darwin --sdk /path/to/MacOSX.sdk
cargo run -- install compiler-headers clang
cargo run -- install compiler-headers apple-clang
cargo run -- install compiler-headers gcc
cargo run -- install compiler-headers msvc
cargo run -- doctor compiler-headers clang
cargo run -- path compiler-headers gcc
cargo run -- path compiler-headers msvc x86_64-pc-windows-msvc
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
and `compiler-headers/gcc-16.1.0/include` in the same data directory. The
Clang installer requires `git` and copies the upstream release's resource
headers. The GCC installer downloads the release archive into Slate's cache,
checks its SHA-256, and extracts GCC's generic `ginclude` headers. GCC's
generated and target-specific compiler headers are not part of that source
directory. `install compiler-headers msvc` installs all supported Windows
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
aarch64 targets for Linux (glibc and musl), FreeBSD, Android, and macOS. Each
target has its own sysroot.

Linux installations use prebuilt sources. The glibc targets extract pinned
Debian cross packages and keep their `usr/<Debian triplet>` layout. The musl
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

When this crate is integrated into the Slate binary, Slate should resolve the
active macOS SDK automatically on macOS and pass that path to its translator
and linker. Mac users should not need to export `SDKROOT` before running Slate.

`doctor` checks the expected header and library paths for each target. It
prints `✓` or `✗` for each group and exits unsuccessfully when any group is
missing.

The target and compiler-header acquisition flows now live in the Rust crate;
the shell scripts and their repository-local test fixtures have been removed.
Clang and GCC use pinned versions, and MSVC CRT headers are reused from the
sysroot instead of copied into a second bundle. New installs use Slate's
platform data directory.
