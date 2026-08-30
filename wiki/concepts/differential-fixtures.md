# Differential fixture directives

Runnable fixtures in `tests/fixtures/` remain semantic differential tests:
Slate compiles and runs the C source and generated Rust, then compares stdout
and exit status. FileCheck directives add generated-Rust shape assertions; they
do not replace runtime comparison.

`NEXTEST_PROFILE=lowering` activates `COMMON` and `LOWERING` directives. The
`rewrites` profile activates `COMMON` and `REWRITES` directives.

`REWRITES` directives are enforced against the new worklist engine's output.
Only assertions satisfied by the current engine belong in the baseline; add
new assertions alongside each newly ported rewrite.

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

## Region-scoped generation with `@begin`/`@end` directives

Prefer scaffolding the FileCheck blocks with `tools/update_filecheck.py` over
hand-writing them. Wrap the C statements whose generated Rust you want to assert
in comment directives, then let the tool emit the `SLATE-FILECHECK-BEGIN/END`
blocks. See `tests/fixtures/global_bool.c` and the `buffer_const_bound*`
fixtures for the pattern:

```c
// @lowering-begin
// @rewrite-begin
int r = sum_fixed(arr);
// @rewrite-end
// @lowering-end
```

`@lowering-begin`/`@lowering-end` scope the `LOWERING` block, `@rewrite-begin`/
`@rewrite-end` the `REWRITES` block; the `-not` variants (`@rewrite-not-begin`,
etc.) assert the region's pre-rewrite text is _absent_ after fixups. Regions
must nest, not cross. Generate or refresh in place with:

```bash
python3 tools/update_filecheck.py --in-place tests/fixtures/<name>.c
```

Markers only work **inside a function body**. The tool injects `__asm__` sentinels
around each region, and an asm statement outside a function fails to compile
(`meaningless 'volatile' on asm outside function`), so you cannot wrap a function
signature or a top-level definition. To assert something about a signature, wrap a
statement in the body that reflects it instead: e.g. to prove a pointer param
became `&mut T`, wrap a field store — the generated store reads `(*arg0).f = x`
when the param is `&mut T` versus `(*(arg0 as *const T as *mut T)).f = x` when it
is wrongly `&T`, so the store text alone captures the difference.

Multiple, disjoint regions in one fixture are fine — each `@lowering`/`@rewrite`
pair contributes to the single `SLATE-FILECHECK-BEGIN/END` block the tool emits,
so wrap every statement you care about.

Scope the region to the smallest observable of the change — for a
signature/call-site lift, wrapping the call statement is enough, since the
bridged call only appears once the lift fires. Re-run the tool whenever the
generated output legitimately changes so the checked-in block stays in sync.

## Per-fixture clang flags

Fixtures emit CIR at `-O0` by default. Some bugs only exist in optimized CIR
(e.g. a `const` pointer mem2reg promotes to an SSA temp reused across flattened
goto/dispatch blocks — `slate-a28e.3`). Give one fixture different flags via
`fixture_clang_arg_overrides(name) -> Vec<String>` in `tests/differential.rs`,
e.g. `"my_fixture" => vec!["-O2".into()]`. Prefer a small harness hook like this
over a one-off manual repro when a corpus bug won't reproduce at `-O0`.
