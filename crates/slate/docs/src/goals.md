# Goals

There are two existing approaches to translating C to Rust, and Slate
exists because neither is enough on its own.

**C2Rust** is the mature solution. It supports C99, but that misses all
of modern C. As well, there is refactoring for the non-supported set,
but this leans on LLMs.

**TRACTOR** (DARPA's "Translating All C TO Rust") forgoes that part and
just uses an LLM for translation.

Slate's goal is to close that gap without an LLM anywhere in the pipeline:
full C support, not just C99, and idiomatic output good enough to pass for
hand-translated Rust, produced by a deterministic, testable pipeline instead
of a model.

## Full C23 support

If a construct is valid C23, slate aims to support it. Any valid C23
that Slate can't translate is a bug.

## Cross-platform by default

Translated Rust code should keep working everywhere the original C project
did, not just on the machine that ran the translator. [Cross Compilation](./compilation.md)
covers how `--target` produces one crate that cross-compiles for every
requested triple, with `#ifdef`-gated C translated once per target and
merged behind matching `#[cfg(...)]` attributes. That only works because
translation never depends on the host's system headers. See [Libc](./libc.md)
for the details.

## Idiomatic output, without an LLM

Slate has a set of analysis passes and rewriting passes that turns
unidiomatic Rust code into better Rust code without compromising safety.
