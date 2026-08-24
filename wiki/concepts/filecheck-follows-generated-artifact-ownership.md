# FileCheck follows generated artifact ownership

_created 2026-08-24_

FileCheck consumes text; the producing harness decides which artifact a
fixture owns.

Single-TU directives check the generated Rust file. In multi-TU and library
projects, `foo.c` checks `src/foo.rs`, so assertions stay beside the source
facts that caused them without concatenating unrelated modules.

Generated project scaffolding has no one-to-one C owner. Checks for `lib.rs`,
`types.rs`, Cargo manifests, smoke tests, C shims, diagnostics, and linked
binary properties remain in the Rust harness.

`translate-directives` checks its stdout with an explicit `DIRECTIVES` prefix.
It does not reuse `LOWERING` because conditional-compilation reconstruction is
a separate producer.

Rewrite directives remain stored in fixtures while passes are disabled. The
matcher skips their execution but runtime differential comparison continues.
