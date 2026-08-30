# Recovering from a clang-ir typed-operation refactor

When `clang-ir`'s typed-operation model changes underneath Slate (e.g. the
0.1.3 refactor, `slate-cevu.7`), the lowerer can compile clean while being
semantically wrong — most failures are invalid generated Rust or runtime
mismatches that only fixture compilation and differential execution expose,
never `cargo check`. Treat "compiles" as zero signal for this class of
change.

## Loop

1. Take one fresh compiler error or runtime mismatch from an existing test
   artifact. Group by generated-Rust _shape_, not by C filename — many
   filenames can share one root cause.
2. Inspect all three representations of the fixture:
   `translate-lowered` (raw lowering), `emit-cir` (ground truth), `translate`
   (with fixups). For c-testsuite, preserve its language mode
   (`translate -std=gnu17`).
3. Trace the bad Rust value back to the typed `clang_ir::model::Op`, its
   typed operands/results, and typed attributes. Never recover old CIR
   behavior by parsing raw operation text — that's the exact anti-pattern the
   refactor is migrating away from.
4. Port the semantic cases from the pre-refactor handler, expressed through
   typed clang-ir APIs, not the old string matching.
5. Make the smallest fix, re-run only the affected fixture. For gcc-torture,
   the single-case path:
   ```bash
   SLATE_GCC_TORTURE_FIXTURE=<name> cargo nextest r --release \
     --test gcc_torture_suite \
     -E 'test(gcc_torture_unsupported_triage_report)' \
     --run-ignored ignored-only --nocapture
   ```
6. Grep remaining captured errors for the next distinct generated-Rust shape.
   Don't re-run the full suite while basic compile families still repeat.
7. Once the targeted queue is exhausted: `cargo fmt`, `cargo clippy --release
--all-targets`, `cargo nextest r --release --profile lowering
--no-fail-fast`.

## Stale-artifact trap

Batch-compiled directories under `target/*-suite/*/batch_cargo/src/bin/` go
stale the moment a fixture is re-translated — they are **not** the same as
the newer per-case outputs (`*.generated.rs`, `*_rs_cargo/src/main.rs`).
Diagnosing from a stale batch produces phantom failures that the current
per-case output has already fixed. Always `diff` the batch copy against the
current generated file, or check the per-case Cargo directory directly
(`cargo check --manifest-path target/.../NNNNN_rs_cargo/Cargo.toml`) before
trusting any cached compiler diagnostic. There's no single-case environment
filter for c-testsuite specifically — use direct translation plus the
per-case directory.

## Comparison-kind trap

Comparison kinds 6 and 7 are not equivalent and must stay separate in any
port: kind 6 is ordered-not-equal (`(lhs < rhs) || (lhs > rhs)`), kind 7 is
unordered (`(lhs != lhs) || (rhs != rhs)`).

## Long double: AST facts, never CIR-text fallback

Long-double constants must come from Clang AST evaluation joined to CIR by
source location — CIR's own long-double attribute representation truncates
bytes and is not an acceptable fallback under any refactor pressure. The
macro-dump plugin emits exact AST-evaluated long-double facts (global/static
initializer leaves, plus any non-`FloatingLiteral` expression Clang can
constant-evaluate) which `c_ast.rs` joins into the floating-literal/global
queues; `Lowerer::ast_floating_literal` resolves the typed
`SourceLocation::{File,Fused,Loc}`/alias/unknown-location chain, since CIR can
retain a location as an alias pointing at a fused location.

The invariant: constant-evaluable long-double expressions use exact AST
value/bits; nonconstant ones lower as runtime ops needing no constant fact; a
`cir.const` long double with **no** matching AST fact emits a loud lowering
TODO — never a CIR-text fallback, zero substitution, and never infer a
builtin (infinity/NaN) by name, since that just hides the same truncation bug
under a different name.

## Where semantic loss tends to hide after this class of refactor

1. A handler compiles but silently no-ops because an attribute moved from
   string data to `Attribute::SymbolRef`, an alias, a dialect attribute, or a
   structured value.
2. Place/value confusion in casts, loads, stores — arrays decaying via an
   address expression vs. `as_mut_ptr`, records needing ABI coercion instead
   of primitive `as`, function pointers needing `Option<fn>` normalization,
   enums needing integer casts only at value boundaries.
3. Anonymous struct/union aliases needing expansion before record-name
   lookup (mangled alias key differs from the literal record name).
4. Source-location joins: when an AST fact exists but isn't found, walk the
   complete typed location/alias/fused-location chain before touching
   constant semantics.
5. Aggregate recursion: constant arrays must recursively render elements at
   their declared type, not collapse every element to `u8`; ABI-coercion
   records and unions must stay records, never collapse to a scalar.
6. Runtime-only mismatches after the generated Rust compiles: pointer
   stride, signedness, comparison predicate, volatile/atomic behavior,
   va_list copying, bitfield storage offsets — check these before blaming
   backend fixups for what's actually a raw-lowering bug.

Fix regressions of this kind in the raw lowerer. Never patch them with a
backend rewrite pass — that just relocates a lowering bug instead of fixing
it, and backend rewrites are meant for idiomization, not correctness repair.

Useful greps across a fresh batch of generated output:

```bash
rg -n ' as \*mut| as \*const|\.str|cannot find value|expected .* found' target
rg -n 'todo!|lower: unhandled|without Clang AST value' target
```
