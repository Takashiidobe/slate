# Publishing

Run all Cargo commands from the workspace root with Rust nightly.

| Publishing order | Package          | Executable     |
| ---------------- | ---------------- | -------------- |
| 1                | `slate-parser`   | `slate-parser` |
| 2                | `slate-sysroots` |                |
| 3                | `slate-c2rust`   | `slate`        |

`slate-intrinsic-gen` is development tooling and is not published.

## First release

Verify all three packaged crates together, including unpublished dependencies:

```sh
cargo package -p slate-parser -p slate-sysroots -p slate-c2rust --all-features --locked
```

Publish the two dependencies before the translator:

```sh
cargo login
cargo publish -p slate-parser --all-features --locked
cargo publish -p slate-sysroots --all-features --locked
cargo publish -p slate-c2rust --all-features --locked
```

Use `--dry-run` to check a package before publishing it. Checking the translator
requires its dependencies to be available on crates.io.

## Tag releases

Trusted publishing uses temporary tokens; no repository API token secret is
needed. It must be configured after each crate's first manual publication.
See [crates.io trusted publishing](https://crates.io/docs/trusted-publishing).

Set all three package versions to the release version, update the workspace
dependency requirements in `Cargo.toml`, and regenerate `Cargo.lock` with
`cargo metadata --no-deps --format-version 1`. Commit the changes, then create
and push one tag for the whole release:

```sh
git tag v0.1.1
git push origin v0.1.1
```

The workflow validates all three package versions against the tag before
publishing anything. It publishes `slate-parser`, then `slate-sysroots`, then
`slate-c2rust`, using trusted publishing for all three. Concurrent release runs
are serialized. Existing versions are skipped, so rerunning a partially
completed release publishes only the remaining crates.

## Prebuilt binaries

After crates.io publication, the same workflow builds `slate` and `slate-parser`
for these native targets:

| Runner              | Target                     |
| ------------------- | -------------------------- |
| Ubuntu 22.04        | `x86_64-unknown-linux-gnu` |
| macOS Intel         | `x86_64-apple-darwin`      |
| macOS Apple Silicon | `aarch64-apple-darwin`     |
| Windows 2022        | `x86_64-pc-windows-msvc`   |

Install the `slate` executable with `cargo binstall slate-c2rust` and the
parser with `cargo binstall slate-parser`.
