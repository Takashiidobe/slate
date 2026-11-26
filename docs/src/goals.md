# Goals

## Full C23 support

Slate aims to translate valid C23 and supports extensions from Clang, GCC, and
MSVC. See [feature coverage](features.md) for tested cases and remaining gaps.

## Cross-platform by default

Translated Rust should preserve behavior on each supported target. See
[cross compilation](compilation.md) to generate one output for multiple targets.

## Idiomatic Rust

Slate uses deterministic analysis and rewrite passes to produce readable Rust
while preserving the C program's behavior.
