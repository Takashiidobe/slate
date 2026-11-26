# Cross compilation

## Translate for another target

Install the target's C headers and pass its triple before the input file.

```sh
slate sysroot install aarch64-unknown-linux-gnu
slate translate --target=aarch64-unknown-linux-gnu input.c > output.rs
```

For host translation, `slate sysroot doctor native` reports the native compiler
and system headers Slate found. See the
[sysroot guide](https://github.com/takashiidobe/slate/blob/main/crates/slate-sysroots/README.md)
for platform-specific setup, including macOS SDKs and Windows headers.

To build the generated Rust, you also need the matching Rust target, a linker,
and the target's C libraries. Generated C runtime code needs a C compiler that
can build for the target.

## Combine targets in one output

For a single file, give Slate a comma-separated list of triples.

```sh
slate translate \
  --targets=x86_64-unknown-linux-gnu,aarch64-unknown-linux-gnu \
  input.c > output.rs
```

For a project, start with a compilation database for each target. From the C
project directory, add each database to a Slate manifest and translate the
selected targets with `--merge`.

```sh
slate project target add slate.json linux-x86_64 build-x86_64/compile_commands.json \
  --triple=x86_64-unknown-linux-gnu
slate project target add slate.json linux-aarch64 build-aarch64/compile_commands.json \
  --triple=aarch64-unknown-linux-gnu
slate translate-project --manifest slate.json --merge \
  --manifest-target linux-x86_64,linux-aarch64 --crate-type staticlib . ../project-rs
```

This example generates a static library. For an executable, omit
`--crate-type staticlib` and use databases that include the executable's `main`.
Slate uses Rust `#[cfg]` attributes where generated code differs by target. Use
`slate project feature add --help` to see how to map C defines to Cargo features
in the same manifest.
