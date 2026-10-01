# Analyses

Backend analyses operate on Slate's Rust AST. Interprocedural pointer, length
and string facts live in `src/backend/interproc`; local worklist rules maintain
the facts needed for their edits.

C semantic facts belong in slate-parser. Avoid reconstructing C conversions or
layout from generated Rust. See [architecture](architecture.md) for ownership
and [rewriting](writing-a-rewrite.md) for the backend workflow.
