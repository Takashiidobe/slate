# Remove obsolete snapshot test infrastructure

> Historical record. See the [archive index](index.md) for scope and source revision.

_created 2026-08-24_

The repository no longer keeps full generated-Rust snapshots. Generated-source
shape assertions live as profile-scoped FileCheck directives beside their C
fixtures, while differential execution remains the semantic correctness check.

The old `tests/snapshot.rs` harness, `tests/snapshots/` corpus, `insta`
dependency, and lowering-profile selector were removed together. Inspect raw
translation output directly when authoring a new FileCheck expectation.
