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

| Runner | Target |
| ------ | ------ |
| Ubuntu 22.04 | `x86_64-unknown-linux-gnu` |
| macOS Intel | `x86_64-apple-darwin` |
| macOS Apple Silicon | `aarch64-apple-darwin` |
| Windows 2022 | `x86_64-pc-windows-msvc` |

Each package gets an archive named
`<package>-<target>-v<version>.tar.gz`, containing its executable and licenses
at the archive root. Windows executables retain the `.exe` suffix. Builds use
`target/test-cache/<target>/release/` from the workspace Cargo configuration.
The workflow creates a GitHub Release for the existing tag and uploads all
archives; reruns replace the matching assets.

Both binary crates declare `[package.metadata.binstall]` matching these paths.
Users install the `slate` executable with `cargo binstall slate-c2rust` and the
parser with `cargo binstall slate-parser`.

Existing crates.io versions are immutable, but binstall can discover binaries
without explicit metadata through its
[default archive conventions](https://github.com/cargo-bins/cargo-binstall/blob/main/SUPPORT.md#defaults).
To backfill an already published version, build its original tag and attach
archives to the GitHub Release for that tag. For example:

```text
slate-c2rust-x86_64-unknown-linux-gnu-v0.1.0.tgz
  slate-c2rust-x86_64-unknown-linux-gnu-v0.1.0/slate
slate-parser-x86_64-unknown-linux-gnu-v0.1.0.tgz
  slate-parser-x86_64-unknown-linux-gnu-v0.1.0/slate-parser
```

Use the same layout for other targets, with `.exe` for Windows binaries. This
requires no tag movement or crates.io republication. The workflow only runs on
new tag pushes; backfilling an existing tag requires a manual build and upload.
Future versions carry the explicit metadata and use the archive-root layout
described above. No separate publishing token is needed for automated binaries:
the release job uses `GITHUB_TOKEN` with `contents: write`.
