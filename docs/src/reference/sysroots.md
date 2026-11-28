# Sysroots and Headers

Slate reads two sets of C headers. A target sysroot provides operating system
and C library headers for one target triple. Compiler headers provide resource
headers such as `stddef.h` and intrinsic declarations. Select a sysroot by
target and a compiler bundle by the flavor of the original C build.

## Command syntax

```text
slate sysroot <install|path|doctor|remove> TRIPLE
slate sysroot install DARWIN_TRIPLE [--sdk PATH]
slate sysroot <install|path|doctor> compiler-headers <clang|apple-clang>
slate sysroot <install|path|doctor> compiler-headers gcc [x86|aarch64|arm]
slate sysroot <install|path|doctor> compiler-headers msvc [MSVC_TRIPLE]
slate sysroot <path|doctor> native [clang|gcc|msvc]
```

A triple selects a target sysroot, such as `aarch64-unknown-linux-gnu`. See
[Target Support](../targets/index.md) for the supported targets.

| Flag           | Applies to                           | Effect                                                              |
| -------------- | ------------------------------------ | ------------------------------------------------------------------- |
| `--sdk PATH`   | `install` with a macOS target triple | Register a local macOS SDK. Supply the path as a separate argument. |
| `-h`, `--help` | `slate sysroot` and its subcommands  | Print command help.                                                 |

For macOS SDK installation, `--sdk PATH` takes precedence over `OSX_CROSS_SDK`.
If neither is set, Slate can discover the active SDK on macOS through `xcrun`.
On other hosts, supply a path. Slate registers a symlink, so keep the SDK at
that path. Supplying a different path replaces the registration.

## Compiler and native selectors

| Compiler      | Selector                   | Default and behavior                                                                                                                     |
| ------------- | -------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| `clang`       | None                       | One upstream Clang resource bundle shared across targets.                                                                                |
| `apple-clang` | None                       | Copies resource headers from the active Apple toolchain when installed on macOS.                                                         |
| `gcc`         | `x86`, `aarch64`, or `arm` | By default, print or check all Linux families. A selector limits the paths printed or checked. Installation acquires all three families. |
| `msvc`        | Windows MSVC target triple | By default, operate on all supported Windows targets. Installation with a triple selects one Windows sysroot and uses its CRT headers.   |

`native` queries an installed host toolchain. It defaults to `clang`. Use
`native gcc` or `native msvc` to query the matching compiler.

## Choose which headers to use

For the host target, Slate queries the native compiler for its header search
paths when available. For project translation, the compiler named in the
compilation database supplies those paths when it can answer the query. Slate
can fall back to installed headers when native discovery fails.

For a target other than the host, install the target sysroot and the compiler
bundle. Slate also uses installed headers when `SLATE_SYSROOTS` points to a
directory containing the target sysroots. For example:

```sh
slate sysroot install aarch64-unknown-linux-gnu
slate sysroot install compiler-headers clang
```

Explicit compiler options such as `--sysroot` and `-isystem` also affect header
lookup. See [Headers and Include Search](../parsing/headers.md). Keep project
headers and generated configuration headers in the original build's include
paths.

## Command behavior

| Command   | Behavior                                                                                                                                                 |
| --------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `install` | Install or refresh a target sysroot or compiler bundle and print its path.                                                                               |
| `path`    | Validate an installed sysroot or bundle and print its path. With `native`, print the discovered include directories.                                     |
| `doctor`  | Check expected files for the selection and exit unsuccessfully if a check fails. With `native`, check the discovered directories and look for `stdio.h`. |
| `remove`  | Remove the installed sysroot for one target triple.                                                                                                      |

A successful `doctor` checks the selected installation. To build generated Rust,
also install the Rust target and any required linker, libraries, or C compiler.
See [Target Support](../targets/index.md).

## Environment variables

| Variable              | Effect                                                                                         |
| --------------------- | ---------------------------------------------------------------------------------------------- |
| `OSX_CROSS_SDK`       | Supplies the local macOS SDK path when `install` has no `--sdk` flag.                          |
| `XWIN_ACCEPT_LICENSE` | Records acceptance of Microsoft's license for noninteractive Windows SDK and CRT installation. |
| `SLATE_SYSROOTS`      | Points the translator to the directory containing the per-target sysroots.                     |
