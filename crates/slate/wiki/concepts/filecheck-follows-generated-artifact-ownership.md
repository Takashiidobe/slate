# FileCheck follows generated artifact ownership

_created 2026-08-24_

FileCheck consumes text; the producing harness decides which artifact a
fixture owns.

Single-TU directives check the generated Rust file. In multi-TU and library
projects, `foo.c` checks `src/foo.rs`, so assertions stay beside the source
facts that caused them without concatenating unrelated modules.

Generated project scaffolding has no one-to-one C owner. Checks for `lib.rs`,
`types.rs`, Cargo manifests, smoke tests, C shims, diagnostics, and linked
binary properties remain in the Rust harness when their text or binary artifact
is the observable. If an owned generated module references a synthesized type,
shim, or export, checking that reference and compiling the complete generated
crate already verifies the project-level property without a duplicate harness
assertion.

`translate-directives` checks its stdout with an explicit `DIRECTIVES` prefix.
It does not reuse `LOWERING` because conditional-compilation reconstruction is
a separate producer.

Rewrite directives are checked against the new worklist engine's generated
artifacts. The baseline contains only assertions satisfied by that engine.
