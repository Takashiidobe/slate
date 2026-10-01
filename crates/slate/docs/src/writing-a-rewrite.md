# Rewriting

`src/backend/engine` applies registered rules over the Rust AST. Whole-program
analyses live in `src/backend/interproc`; the backend receives target facts
from the parser module.

`translate` invokes the backend; `translate-lowered` and project translation
currently emit raw lowered programs. The Slate nextest profile exercises raw
lowering, so rewrite work needs fixtures that also execute backend translation.

See the [rewrite engine](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/rewrite-engine-v2.md)
and [pointer capability lattice](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/pointer-capability-lattice.md)
for the retained backend design.
