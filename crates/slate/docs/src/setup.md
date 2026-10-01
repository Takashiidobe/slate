# Setup

Run every command from the workspace root.

## Build

Slate requires a Rust nightly toolchain. It links slate-parser and
slate-sysroots as workspace libraries.

```bash
cargo build --release -p slate
cargo run --release -p slate -- sysroot install x86_64-unknown-linux-gnu
```

Release binaries live under `target/test-cache/release/` because
`.cargo/config.toml` selects that target directory.

## Configuration

| Variable | Purpose |
| --- | --- |
| `SLATE_SYSROOTS` | Header installation root; defaults to `~/.local/share/slate/sysroots` |
| `SLATE_TARGET` | Default target; otherwise the Cargo build target |
| `SLATE_CLANG_ARGS` | Inherited compiler arguments consumed by slate-parser; explicit arguments follow them |
| `SLATE_JOBS` | Project translation workers |
| `SLATE_CARGO` | Cargo used to compile generated code |
| `SLATE_RUSTFMT` | Fallback formatter when prettyplease cannot parse output |

## Tests

Differential tests require `clang` on PATH to compile the C oracle. Generated
crates containing runtime bridges also use a C compiler through the `cc` crate.

```bash
cargo nextest r --release --profile slate
cargo nextest r --release --profile parser
```

Run the profile for each crate changed. See [testing](testing.md) for fixture
selection and triage.
