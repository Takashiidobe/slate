# Merge

`slate translate-project --merge` writes one Cargo crate for multiple target
triples or C preprocessor feature combinations. The generated crate uses Rust
`#[cfg]` to select the code for the target and Cargo features you build.

From the C project directory, register each target's compilation database. This
example assumes the project's C sources use `FAST` as an optional macro and that
you have configured separate builds for x86-64 Linux and AArch64 Linux.

```sh
slate project target add targets.json linux-x86_64 build-x86_64/compile_commands.json \
  --triple x86_64-unknown-linux-gnu
slate project target add targets.json linux-aarch64 build-aarch64/compile_commands.json \
  --triple aarch64-unknown-linux-gnu
slate project feature add targets.json FAST
slate translate-project --merge --manifest targets.json . ../project-rs
cargo +nightly build --manifest-path ../project-rs/Cargo.toml \
  --target x86_64-unknown-linux-gnu --features fast
```

## Define the configurations

A manifest target names a triple and a compilation database. It can also specify
the compiler flavor, sysroot, or C build target. Use separate databases when
platforms build different source files or generate different headers. The same
database can serve multiple platforms when its commands and inputs apply to all
of them. Commands with an explicit target must agree with the manifest.

Slate derives the Cargo feature name from the macro, so `FAST` becomes `fast`.
Keep feature macros out of the base compile commands so the manifest can toggle
them. Use `slate project feature conflict` to record combinations the C project
disallows. To merge only some targets, pass their comma-separated names to
`--manifest-target`.

## How Slate combines the code

For each target and source file, Slate checks the relevant feature assignments
during preprocessing and reuses a translation when the preprocessed input is
identical. A file compiled on only some platforms gets a target-gated Rust
module.

Slate shares items that translate identically. When only part of a function
changes, it can keep a shared body and gate the differing parts. It keeps
separate gated definitions when the variants cannot be combined. This lets Cargo
choose the code at compile time, including differences in C types, record
fields, and function bodies.

## Check a merge

Read `merge-report.txt` in the generated crate for the configurations Slate
compiled, gated items, and any projection check results. Add
`--check-projection all` to the merge command to compare the Rust selected by
each valid target and feature combination with a direct translation of that
configuration. To inspect one configuration directly, run `translate-project`
with `--manifest`, one `--manifest-target`, and any enabled `--features`. Build
and test the generated crate on the platforms you intend to ship.
