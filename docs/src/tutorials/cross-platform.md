# Translate zlib for two platforms

Continue from the [native zlib tutorial](zlib.md) on x86-64 GNU/Linux. Produce
one Rust crate for x86-64 and Arm64 GNU/Linux, with zlib's `FASTEST` macro
exposed as a Cargo feature.

## Install translation headers

```sh
slate sysroot install x86_64-unknown-linux-gnu
slate sysroot install aarch64-unknown-linux-gnu
slate sysroot install compiler-headers clang
```

Each target triple identifies the architecture, operating system, and ABI whose
headers Slate should read. The two target bundles supply the matching glibc
headers. The Clang resource headers are shared. Slate can use the native headers
for the x86-64 host. Installing both target bundles also lets you use installed
headers for both by setting `SLATE_SYSROOTS` to their parent directory. See
[Sysroots and Headers](../reference/sysroots.md).

## Describe the builds

Run these commands from the zlib checkout.

```sh
cd zlib
slate project target add slate.json linux-x86_64 build-clang/compile_commands.json \
  --triple x86_64-unknown-linux-gnu --build-target zlibstatic
slate project target add slate.json linux-aarch64 build-clang/compile_commands.json \
  --triple aarch64-unknown-linux-gnu --build-target zlibstatic
slate project feature add slate.json FASTEST
```

The two entries select the same source files and Linux configuration. Slate
applies each manifest triple when it reads the commands. Reuse a database only
when its flags, generated inputs, and source selection apply to both targets. If
your build probes produce different results on another platform, configure that
platform separately and register its own database. An explicit target in a
compile command must agree with its manifest entry.

Keep `FASTEST` out of the base compile commands so Slate can translate the
library with the macro both enabled and disabled. The manifest records `fastest`
as the Cargo feature name. See
[Project Manifest](../reference/project-manifest.md).

## Merge and build

```sh
slate translate-project --manifest slate.json --merge \
  --crate-type staticlib . ../zlib-merged
cargo +nightly build --manifest-path ../zlib-merged/Cargo.toml \
  --target x86_64-unknown-linux-gnu
```

`--merge` puts the translated variants in one crate. Rust `#[cfg]` conditions
select the code for the requested target and enabled features. Read
`../zlib-merged/merge-report.txt` to see the configurations and gated items. See
[Merge](../project/merge.md) for projection checks.

To build for Arm64 from x86-64, install the Arm64 Rust target and make an Arm64
Linux C toolchain available on `PATH`. The example assumes its compiler is named
`aarch64-linux-gnu-gcc`.

```sh
rustup target add --toolchain nightly aarch64-unknown-linux-gnu
CC_aarch64_unknown_linux_gnu=aarch64-linux-gnu-gcc \
CARGO_TARGET_AARCH64_UNKNOWN_LINUX_GNU_LINKER=aarch64-linux-gnu-gcc \
cargo +nightly build --manifest-path ../zlib-merged/Cargo.toml \
  --target aarch64-unknown-linux-gnu --features fastest
```

The Rust target supplies the target standard library. Cargo uses the linker
setting for target links, and the `CC_` setting selects the C compiler if the
generated crate builds any C support. `--features fastest` selects the variant
translated with `FASTEST`. Build each target both with and without the feature
to check all four combinations. To repeat the C example on Arm64, compile it
with the Arm64 toolchain and run it on Arm64 or under an emulator.
