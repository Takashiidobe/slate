# Slate overview

<!-- toc -->
- [Pipeline](#pipeline)
- [Use](#use)
- [Coverage and failures](#coverage-and-failures)
- [Further reading](#further-reading)
<!-- /toc -->

## Pipeline

Slate translates C to Rust through slate-parser's typed IR. C semantics belong
to the parser; Rust lowering and optional backend rewrites belong to Slate.

```text
C -> preprocess, parse, sema -> ir::Module -> Rust AST -> Rust source
```

## Use

Run from the workspace root; release binaries live in `target/test-cache/release`.

```bash
cargo build --release -p slate-c2rust
cargo run --release -p slate-c2rust -- sysroot install x86_64-unknown-linux-gnu
cargo run --release -p slate-c2rust -- translate -std=gnu17 input.c
cargo run --release -p slate-c2rust -- emit-slate-ir input.c
cargo run --release -p slate-c2rust -- lowering-barriers input.c
cargo run --release -p slate-c2rust -- translate-project \
  --compile-commands project/compile_commands.json project output-crate
```

- Translation uses slate-parser as a Rust library and target headers from slate-sysroots.
- A Rust nightly toolchain builds Slate and generated code. Differential tests also need `clang` on PATH as the C oracle.
- `translate-project` supports executable projects with one `main`, one configuration per unit, and unique module stems. It writes runtime bridges when needed.
- `SLATE_TARGET` sets the default target. Explicit compiler arguments take precedence; [target queries](slate-target-queries.md) documents configuration.
- `SLATE_JOBS` controls project worker parallelism; `SLATE_CARGO` selects the generated-code compiler and `SLATE_RUSTFMT` the fallback formatter.
- Published package: `slate-c2rust`; executable and library target: `slate`. [Release setup](../../RELEASING.md).
- Embedded support manifests use `Cargo.toml.template`; Cargo always excludes nested packages containing `Cargo.toml`. Generated projects receive ordinary manifests.

## Coverage and failures

| Evidence | Meaning |
| --- | --- |
| Supported fixture passes differential execution | C and generated Rust agree on stdout and exit status |
| Unsupported fixture fails | A tracked gap remains; triage identifies the first barrier |
| Parser emits IR | C parsing and semantic lowering succeeded; Rust translation is not yet established |
| Raw lowering profile passes | Baseline translation is covered; backend rewrites need their own exercising fixtures |

```bash
cargo nextest r --release --profile slate
bd ready
```

Typed errors distinguish parser/analysis failures, unsupported lowering and
invalid IR. CLI boundaries render diagnostics to text. FileCheck shape tests
are suspended; current coverage comes from the differential fixture suites.

## Further reading

- [Architecture](slate-architecture.md)
- [IR specification](ir-spec.md)
- [Differential fixtures](differential-fixtures.md)
- [Cross-target setup](cross-target-toolchains.md)
- [Historical index](../historical/index.md)
