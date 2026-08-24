# Differential fixture directives

Runnable fixtures in `tests/fixtures/` remain semantic differential tests:
Slate compiles and runs the C source and generated Rust, then compares stdout
and exit status. FileCheck directives add generated-Rust shape assertions; they
do not replace runtime comparison.

`NEXTEST_PROFILE=lowering` activates `COMMON` and `LOWERING` directives. The
`rewrites` profile activates `COMMON` and `REWRITES` directives.

Global checks use normal FileCheck syntax. Prefer `DAG` because generated Rust
item and import ordering is not significant:

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

Use ordered `NEXT` or `SAME` checks only when statement order is the behavior
under test. The first harness slice supports unordered `DAG` and `NOT` inside
function blocks; ordered function sequences are tracked under `slate-n6s7`.

`SLATE_FILECHECK` overrides the FileCheck executable. Otherwise the runner uses
the `FileCheck` next to `SLATE_CLANG` when that variable is set, then falls back
to `FileCheck` from `PATH`.
