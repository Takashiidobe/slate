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

Translated Rust should preserve behavior on each supported target. Target facts
come from slate-parser and target headers from slate-sysroots. [Cross
compilation](compilation.md) documents current translation and runtime limits;
a merged multi-target project remains a goal.

## Idiomatic output, without an LLM

Slate has a set of analysis passes and rewriting passes that turns
unidiomatic Rust code into better Rust code without compromising safety.
