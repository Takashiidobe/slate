# Differential fixture directives

Runnable fixtures in `tests/fixtures/` remain semantic differential tests:
Slate compiles and runs the C source and generated Rust, then compares stdout
and exit status. FileCheck directives add generated-Rust shape assertions; they
do not replace runtime comparison.

`NEXTEST_PROFILE=lowering` activates `COMMON` and `LOWERING` directives. The
`rewrites` profile activates `COMMON` and `REWRITES` directives.

While rewrite passes are disabled, the FileCheck handler recognizes but does
not execute `REWRITES` directives. Runtime differential comparison remains
active, so this does not hide C-to-Rust execution failures.

Cross-target fixtures combine the profile and target in one prefix, such as
`REWRITES-MACOS`, `REWRITES-MSVC`, or `REWRITES-BIONIC-X86_64`. This keeps a
target assertion from running under the other profile or ABI.

Platform/libc fixture directories use the normal cross-target collector.
Special target cases outside those directories, such as a multi-file ABI
fixture, use a separate target-check entry instead of inventing a fixture
flavor.

Global checks use normal FileCheck syntax. Each global assertion runs as an
independent FileCheck invocation, so generated Rust item and import ordering is
not significant:

```c
// COMMON-DAG: unsafe extern "C"
// LOWERING-DAG: let _v{{[0-9]+}}: i32
// REWRITES-NOT: todo!()
```

Function checks start with `LABEL` and end at the function's column-zero
closing brace:

```c
// REWRITES-LABEL: {{^}}fn alias_impl(
// REWRITES-DAG: real_impl(_0)
// REWRITES-NOT: todo!()
// REWRITES: {{^}}}
```

The runner invokes each label block independently, so fixture check order does
not constrain generated function order. Each `DAG` or `NOT` assertion is also
checked independently within the labeled function. This prevents a match from
leaking into a later function while preserving position-independent checks.

Use plain profile checks, optionally with `NEXT`, `SAME`, or `EMPTY`, when
relative order is itself the behavior. An ordered sequence inside a function
block is checked together and remains bounded by that function.

`SLATE_FILECHECK` overrides the FileCheck executable. Otherwise the runner uses
the `FileCheck` next to `SLATE_CLANG` when that variable is set, then falls back
to `FileCheck` from `PATH`.

For a multi-translation-unit project, directives in `foo.c` check the generated
`src/foo.rs`. Library fixtures use the same mapping for C files under `src/`.
Assertions about synthesized `lib.rs`, `types.rs`, manifests, C shims, smoke
tests, diagnostics, or binary symbols stay in the Rust project harness.

`translate-directives` fixtures use `DIRECTIVES` prefixes against that
command's stdout. Those checks are separate from lowering and rewrite profile
selection because conditional-compilation reconstruction is its own producer.
