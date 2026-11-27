# Generate System Bindings

`slate-libc-gen` finds C system declarations that can use a known
`slate_support` path in generated Rust. It probes each target's headers against
the vendored Rust bindings. A target with a mismatched declaration keeps its
local declaration.

## Probe one target

Install the target sysroot and compiler headers first. Build the parser, then
write a single-target run to alternate files so you can inspect the result.

```sh
cargo build --release -p slate-parser
cargo run --release -p slate-libc-gen -- \
  --target x86_64-unknown-linux-gnu \
  --out target/libc-records-linux.rs \
  --symbols-out target/libc-symbols-linux.rs \
  --report target/libc-gen-linux.md
```

The record catalog records where target layout agrees. The symbol catalog
records function and global names, scalar typedefs, and integer constants with
their target eligibility and canonical paths. Use `--work <directory>` to keep
the probe crates and objects.

## Read an exclusion

- A record must match size, alignment, and public field offsets. A header macro
  that uses a reserved field can also make the record ineligible.
- A function or global must match the C type and refer to the same link symbol.
  Common exclusions include an ABI attribute or an unverified type.
- A typedef must match width, alignment, and signedness. An integer constant
  must match by name and value.

The report names accepted mappings and exclusions. If a generated file keeps a
local `extern` despite a matching report entry, check the declaration seen by
the translated unit. Feature-test macros can change a signature, and Slate
checks the unit's signature against the catalog before binding it.

Apple targets also probe public vendored `mach2` bindings. On Windows the
catalog covers the C runtime headers, while Win32 declarations remain local.
After changing the vendored bindings or sysroots, regenerate both checked-in
catalogs using the default full target matrix.
