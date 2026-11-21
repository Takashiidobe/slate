# Analyses

Backend analyses operate on Slate's Rust AST. Interprocedural pointer, length
and C-string facts live in `crates/slate-analysis` on the IR; local worklist
rules maintain the facts needed for their edits.

C semantic facts belong in slate-parser. Avoid reconstructing C conversions or
layout from generated Rust. See [architecture](architecture.md) for ownership
and [rewriting](writing-a-rewrite.md) for the backend workflow.
