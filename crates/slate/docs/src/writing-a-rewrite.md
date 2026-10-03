# Rewriting

`src/backend/engine` applies registered rules over the Rust AST. Whole-program
analyses live in `src/backend/interproc`; the backend receives target facts
from the parser module.

`translate` runs the full backend; project translation runs its control-flow
subset. There is no raw lowering mode, so the Slate nextest profile exercises
rewrites on every fixture.

See the [rewrite engine](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/rewrite-engine-v2.md)
and [pointer capability lattice](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/pointer-capability-lattice.md)
for the retained backend design.
