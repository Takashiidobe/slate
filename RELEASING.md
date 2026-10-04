# Publishing

Run all Cargo commands from the workspace root with Rust nightly.

| Package | Executable | Release tag |
| --- | --- | --- |
| `slate-parser` | `slate-parser` | `0.1.0-slate-parser` |
| `slate-sysroots` | | `0.1.0-slate-sysroots` |
| `slate-c2rust` | `slate` | `0.1.0-slate-c2rust` |

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

Create the GitHub Actions environment `release`. Configure a trusted publisher
on crates.io for each of the three packages:

| Setting | Value |
| --- | --- |
| Repository owner | `Takashiidobe` |
| Repository | `slate` |
| Workflow filename | `release.yml` |
| Environment | `release` |

Trusted publishing uses temporary tokens; no repository API token secret is
needed. It must be configured after each crate's first manual publication.
See [crates.io trusted publishing](https://crates.io/docs/trusted-publishing).

Commit the version changes, then create and push the corresponding tag:

```sh
git tag 0.1.0-slate-c2rust
git push origin 0.1.0-slate-c2rust
```

The workflow checks that the tag prefix matches the package version and skips
versions already published. Tags have no `v` prefix, matching `../clang-ir`.
Release changed dependencies first and wait for their workflows to succeed
before pushing the translator tag. Unchanged dependencies need no new release.
