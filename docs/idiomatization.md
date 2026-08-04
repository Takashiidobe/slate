# Idiomatization

V0 produces faithful but ugly Rust: `unsafe`, `libc` calls, raw pointers,
mechanical temps. Idiomatic safe Rust is recovered afterward by a set of
fixups. A fixup is different from a lowering feature:

- a **feature** expands which C programs baseline Slate can translate;
- a **fixup** rewrites already-correct generated Rust into better Rust.

Each fixup:

- is an independent pass, verified by the differential harness (semantics must
  not change);
- is optional — you can skip any fixup and still have valid, correct Rust;
- may consult the **Clang AST / source oracle** when CIR alone lacks the needed
  information (this is what justifies the three-source design — see
  [architecture.md](architecture.md)).

Cleanup passes live under `src/fixups/` and are wired through a fixed post-lower
entry point. Baseline lowering owns correctness; fixups own readability. See
[passes.md](passes.md) for the current pass catalog and pipeline order.

Standard-library semantics require a `Known` function identity. Slate derives
that identity from the Clang declaration reached by the call, its canonical
function type, and plugin evidence that the declaration came through the
corresponding trusted system header. A matching spelling alone is never enough.
Project declarations, shadowing definitions, indirect calls, and declarations
from similarly named project headers remain ordinary calls throughout lowering,
fixups, and effects interpretation.

Clang's reserved `__builtin_*` memory operations are also `Known`: their
semantics come from the compiler rather than a C header. CIR operations created
directly for language intrinsics, such as `__builtin_bit_cast`, carry the same
compiler-defined identity without pretending to be libc declarations.

Runtime symbol interposition such as `LD_PRELOAD` or link-time replacement of a
proven standard-library symbol is outside Slate's supported semantics. Those
mechanisms intentionally change the program after translation and cannot be
reconciled with source-level idiomatization.

## Adding a fixup

Use this checklist for every idiom pass:

1. Generate Rust from a C fixture that already passes differential testing.
2. Add a test for the rewritten output shape and keep the differential test
   green.
3. Match only the narrow pattern you can prove.
4. Preserve a fallback to baseline Rust.
5. Keep the pass independent; other fixups should not be required to run first
   unless the dependency is explicit and tested.

See [writing-a-query-fixup.md](writing-a-query-fixup.md) for the AST-to-AST
pass recipe, and [passes.md](passes.md) for where to register the pass in the
pipeline.
