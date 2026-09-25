# Slate sysroots

This crate installs target sysroots for Slate. The library owns target selection,
installation, and path lookup; the binary is a small command-line facade.

Currently supported:

```sh
cargo run -- install x86_64-pc-windows-msvc
cargo run -- path x86_64-pc-windows-msvc
cargo run -- doctor x86_64-pc-windows-msvc
cargo run -- install aarch64-pc-windows-msvc
cargo run -- doctor aarch64-pc-windows-msvc
cargo run -- install x86_64-unknown-linux-gnu
cargo run -- install aarch64-unknown-linux-gnu
cargo run -- install x86_64-unknown-linux-musl
cargo run -- install aarch64-unknown-linux-musl
```

The Windows installer requires `xwin` on `PATH`, or set `XWIN` to its executable.
It uses `xwin` to acquire the Microsoft CRT and Windows SDK. `xwin` handles
license acceptance. The installed files are for local use and are not bundled
with Slate.

Installed sysroots live in Slate's platform-specific local data directory:

| Host | Root |
| --- | --- |
| Linux | `${XDG_DATA_HOME:-~/.local/share}/slate` |
| macOS | `~/Library/Application Support/Slate` |
| Windows | `%LOCALAPPDATA%\Slate\data` |

The installation is under `sysroots/<Rust target triple>/`. Downloaded `xwin`
packages go into the platform cache directory under `xwin/`. The public
`Paths::resolve` and `Paths::include_paths` methods supply paths for Slate's
translator. MSVC CRT headers stay in the sysroot alongside the Windows SDK
headers; the installer does not duplicate them under `compiler-headers`.

The supported Rust targets are `x86_64-pc-windows-msvc` and
`aarch64-pc-windows-msvc`, plus x86_64 and aarch64 Linux targets using glibc or
musl. Each target has its own sysroot and architecture specific libraries.

Linux installations use prebuilt sources. The glibc targets extract pinned
Debian cross packages and keep their `usr/<Debian triplet>` layout. The musl
targets extract the prebuilt sysroot from pinned musl-cross archives. Slate uses
`Paths::include_paths` to find the C headers in either layout, so no generated
aliases or linker-script edits are needed for header translation. These Linux
target installations do not install versioned Clang or GCC resource headers;
those remain separate compiler bundles.

`doctor` checks the expected header and library paths for each target. It
prints `✓` or `✗` for each group and exits unsuccessfully when any group is
missing.

The repository's shell scripts remain available while additional targets are
migrated.
