# Slate sysroots

Manage the target C headers and compiler headers used by Slate. This crate
provides the library; the command is `slate sysroot`.

## Synopsis

```text
slate sysroot <install|remove|path|doctor> <target>
slate sysroot install <Darwin target> [--sdk <path>]
slate sysroot <install|path|doctor> compiler-headers <clang|apple-clang>
slate sysroot <install|path|doctor> compiler-headers gcc [x86|aarch64|arm]
slate sysroot <install|path|doctor> compiler-headers msvc [MSVC target]
```

From a source checkout, run commands from the workspace root using
`cargo run -p slate-c2rust -- sysroot …` in place of `slate sysroot …`.

## Description

A **target sysroot** supplies the operating system and C library headers for
one Rust target triple, such as glibc headers for `x86_64-unknown-linux-gnu`
or Windows SDK headers for `x86_64-pc-windows-msvc`.

**Compiler headers** supply compiler-specific definitions such as `stdarg.h`,
`stddef.h`, and intrinsic headers. Choose these to match the compiler used
for the C input. Clang, Apple Clang, and GCC bundles are installed separately
from target sysroots. MSVC headers already live in the Windows sysroot.

Install the target sysroot and the appropriate compiler headers, then run
`doctor` for each. These commands acquire headers; they do not install a
compiler executable. FreeBSD and Android sysroots contain headers only and
cannot supply link libraries or startup files.

## Commands

| Command | Effect |
| --- | --- |
| `install <target>` | Acquire the target sysroot and print its installed path. For macOS, register a local SDK. |
| `install compiler-headers <compiler> [selector]` | Acquire compiler headers and print their include paths. MSVC installs the selected Windows sysroots and reuses their CRT headers. |
| `doctor <target>` | Check the target's expected header and library paths, where applicable. Windows checks also detect CRT/UCRT version mismatches. |
| `doctor compiler-headers <compiler> [selector]` | Check the selected compiler bundle's required headers and accompanying license or manifest files. |
| `path <target>` | Validate the target installation and print its sysroot path. |
| `path compiler-headers <compiler> [selector]` | Validate the selected compiler bundle and print its include path. |
| `remove <target>` | Remove the target sysroot installation. Compiler bundles have no `remove` command. |

`doctor` prints `✓` or `✗` for each check and exits unsuccessfully if any
check fails. It checks the named target or compiler bundle; it does not
install missing files or check the other bundles. A successful check confirms
that the expected files exist, rather than compiling or linking a test program.

## Compiler headers

All four compiler names support `install`, `path`, and `doctor`.

| Compiler | Header source | Target use | Optional selector |
| --- | --- | --- | --- |
| `clang` | Upstream Clang 22.1.8 resource headers; requires `git` to install | Shared bundle for all supported targets | None |
| `apple-clang` | Resource headers copied from the active Xcode or Command Line Tools compiler; installation requires macOS | The two macOS targets | None |
| `gcc` | GCC 16.2.0 release archive, verified by SHA-256; assembled as Linux compiler headers | Linux targets with the matching architecture family | `x86`, `aarch64`, or `arm`; omitted means all families |
| `msvc` | CRT headers from the pinned Microsoft packages | The four Windows MSVC targets; headers must match the exact target | A Windows MSVC target triple; omitted means all four targets |

GCC's `x86` family covers i686 and x86_64; `aarch64` covers aarch64;
`arm` covers both armv7 Linux targets. Installing GCC headers always installs
all three families, even when a selector is supplied. The selector limits
which paths are printed or checked. These bundles omit runtime-library
headers such as `omp.h`, `gcov.h`, and `sanitizer/`.

Clang can use the Windows SDK and CRT together with its own resource headers.
Use `apple-clang` for input built with Apple's compiler and `msvc` for input
built with MSVC. Compiler-header availability alone does not establish
compiler compatibility with an operating system or ABI.

## Supported targets

Each target has its own sysroot. Every target below supports `install`,
`path`, `doctor`, and `remove`.

### Linux (glibc)

- `x86_64-unknown-linux-gnu`
- `aarch64-unknown-linux-gnu`
- `i686-unknown-linux-gnu`
- `armv7-unknown-linux-gnueabi`
- `armv7-unknown-linux-gnueabihf`

Installs pinned Debian cross packages. aarch64 and armel use Bookworm's
glibc 2.36; x86_64, i686, and armhf use Sid's glibc 2.43. Headers retain the
`usr/<Debian triplet>/include` layout. i686 uses i386 packages, and the armv7
targets use armel and armhf respectively.

### Linux (musl)

- `x86_64-unknown-linux-musl`
- `aarch64-unknown-linux-musl`

Installs prebuilt sysroots from pinned musl-cross archives.

### FreeBSD

- `x86_64-unknown-freebsd`
- `aarch64-unknown-freebsd`

Extracts only `usr/include` and `COPYRIGHT` from pinned FreeBSD 15.1 base
archives.

### Android

- `x86_64-linux-android`
- `aarch64-linux-android`

Downloads Google's NDK r27d Linux ZIP, verifies its SHA-256, and extracts only
Bionic headers and the NDK notice for the selected architecture. The same
archive supplies headers on Linux, macOS, and Windows hosts.

### macOS

- `x86_64-apple-darwin`
- `aarch64-apple-darwin`

Registers a local macOS SDK. See [macOS SDK and licensing](#macos-sdk-and-licensing).

### Windows (MSVC)

- `i686-pc-windows-msvc`
- `x86_64-pc-windows-msvc`
- `aarch64-pc-windows-msvc`
- `thumbv7a-pc-windows-msvc`

Downloads the Microsoft CRT and Windows SDK through `xwin`.
See [Windows packages and licensing](#windows-packages-and-licensing).

## macOS SDK and licensing

The SDK must come from a local Apple installation; Slate does not download,
copy, or redistribute it. On macOS, `install` discovers the active SDK through
`xcrun`. If no SDK is available, install Apple's Command Line Tools with
`xcode-select --install` and retry.

On any host, supply an SDK path with `--sdk <path>` or `OSX_CROSS_SDK`.
The installer creates `sysroots/<target>/SDK` as a symlink, so the SDK must
remain at that location for `path` and `doctor` to succeed. Automatic discovery
is available only on macOS.

Apple Clang headers are copied locally for the user into a versioned bundle
and are not distributed with Slate. Use of the SDK and Apple toolchain remains
subject to Apple's license terms.

For translation and linking, currently supply the SDK through `OSX_CROSS_SDK`
or `--sdk`; automatic handoff of the active SDK remains future integration
work. Framework includes also require
`-F <SDK>/System/Library/Frameworks`.

## Windows packages and licensing

The installer uses `xwin` to acquire packages from the Visual Studio 2026
manifest. It prompts for acceptance of Microsoft's license on stdin. Set
`XWIN_ACCEPT_LICENSE` to accept non-interactively. Installed files are for
local use and are not bundled with Slate.

| Targets | MSVC CRT | Windows SDK |
| --- | --- | --- |
| i686, x86_64, aarch64 | 14.51 | 10.0.26100 |
| thumbv7a | 14.44 | 10.0.22621 |

The thumbv7a versions are the last releases with 32-bit ARM support. The
Universal CRT comes from the pinned SDK to avoid an older standalone UCRT
package that breaks `threads.h`. `SYSROOT-MANIFEST.txt` records installed
versions, and `doctor <target>` checks for this mismatch.

`install compiler-headers msvc` installs all four Windows sysroots. Supply a
target triple to install just one. It returns the existing `crt/include`
paths rather than creating duplicate compiler-header bundles.

## Files

Installations use Slate's platform-specific local data directory:

| Host | Data directory |
| --- | --- |
| Linux | `${XDG_DATA_HOME:-~/.local/share}/slate` |
| macOS | `~/Library/Application Support/Slate` |
| Windows | `%LOCALAPPDATA%\Slate\data` |

Paths relative to that directory:

| Path | Contents |
| --- | --- |
| `sysroots/<target>/` | Target sysroot |
| `compiler-headers/clang-22.1.8/include/` | Clang resource headers |
| `compiler-headers/gcc-16.2.0/<family>/include/` | GCC headers for one architecture family |
| `compiler-headers/apple-clang-<version>/include/` | Locally copied Apple Clang resource headers |
| `compiler-headers/apple-clang-current/include/` | Stable path to the active Apple Clang bundle |
| `sysroots/<MSVC target>/crt/include/` | MSVC compiler headers |

Downloads use Slate's platform cache directory; `xwin` packages are stored
under `xwin/` there.

## Examples

Install and check a Linux target with Clang headers:

```sh
slate sysroot install x86_64-unknown-linux-gnu
slate sysroot install compiler-headers clang
slate sysroot doctor x86_64-unknown-linux-gnu
slate sysroot doctor compiler-headers clang
```

Inspect GCC headers for an ARM target:

```sh
slate sysroot path compiler-headers gcc arm
```

Register an explicit macOS SDK:

```sh
slate sysroot install aarch64-apple-darwin --sdk /path/to/MacOSX.sdk
```

Install and check one Windows architecture, including its CRT headers:

```sh
slate sysroot install compiler-headers msvc x86_64-pc-windows-msvc
slate sysroot doctor x86_64-pc-windows-msvc
slate sysroot doctor compiler-headers msvc x86_64-pc-windows-msvc
```

## Library interface

`Paths::resolve` returns a validated sysroot path. `Paths::include_paths`
returns its C header directories, accounting for each target's layout.
`Paths::include_paths_with_compiler` places the selected compiler headers
before target headers, avoiding duplicate paths for MSVC. It requires GCC's
architecture family to match the target, MSVC's target to match exactly,
and Apple Clang to use a macOS target.
