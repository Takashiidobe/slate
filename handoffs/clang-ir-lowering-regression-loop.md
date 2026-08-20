# Handoff: clang-ir lowering regression loop

## Scope and state

Continue bead `slate-cevu.7`, "Update Slate for clang-ir 0.1.3 typed operation
refactor." It is still `in_progress` and must not be closed until the relevant
release lowering profile is back to 100%.

The repository was clean at `8eed8957` (`fix up long double parsing again`)
before this handoff was added. That commit contains the current repair pass in:

- `src/frontend/c_ast.rs`
- `src/frontend/lowerer.rs`
- `src/frontend/lowerer/arithmetic.rs`
- `src/frontend/lowerer/memory.rs`
- `src/frontend/lowerer/op_utils.rs`
- `src/frontend/lowerer/storage.rs`
- `src/frontend/lowerer/types.rs`
- `tools/macro-dump-plugin/MacroDump.cpp`

The crate compiles and `cargo fmt` passed during the repair session. Full
differential validation is not complete. Do not treat a clean `cargo check` as
completion: most failures are invalid generated Rust or runtime mismatches that
only fixture compilation and differential execution expose.

## How to continue

Start each session with:

```bash
bd prime
bd show slate-cevu.7
git status --short
```

Read `agents/lowerer.md` before changing the lowerer and
`agents/gcc-torture-triage.md` before working corpus failures.

Use this loop:

1. Take one fresh compiler error or runtime mismatch from an existing test
   artifact or log. Group obvious repetitions by generated Rust shape, not by C
   filename.
2. Locate the C fixture and inspect all three representations:

   ```bash
   cargo run --release -- translate-lowered tests/fixtures.gcc-torture/<name>.c
   cargo run --release -- emit-cir tests/fixtures.gcc-torture/<name>.c
   cargo run --release -- translate tests/fixtures.gcc-torture/<name>.c
   ```

   For c-testsuite, preserve its language mode:

   ```bash
   cargo run --release -- translate -std=gnu17 tests/fixtures.c-testsuite/case_NNNNN.c
   ```

3. Trace the bad Rust value back to the typed `clang_ir::model::Op`, its typed
   operands/results, and typed attributes. Do not recover old CIR behavior by
   parsing raw operation text.
4. Compare the current handler with the pre-refactor behavior when useful. Port
   the semantic cases, but express them through typed clang-ir APIs.
5. Make the smallest lowering fix and re-run only the affected fixture. For GCC
   torture, the supported single-case path is:

   ```bash
   SLATE_GCC_TORTURE_FIXTURE=<name> cargo nextest r --release \
     --test gcc_torture_suite \
     -E 'test(gcc_torture_unsupported_triage_report)' \
     --run-ignored ignored-only --nocapture
   ```

6. Grep the remaining captured errors for the next distinct generated-Rust
   shape and repeat. Do not repeatedly run the full suite while basic compile
   families remain.
7. After the targeted queue is exhausted, run the required gate:

   ```bash
   cargo fmt
   cargo clippy --release --all-targets
   cargo nextest r --release --profile lowering --no-fail-fast
   ```

Never use plain `cargo test`. Rebuild the macro plugin after changing it:

```bash
SLATE_CLANG=/home/takashi/llvm-project/build-cir/bin/clang \
  ./tools/macro-dump-plugin/build.sh
```

## Important stale-artifact trap

The last error scan compiled this existing directory directly:

```text
target/c-testsuite-suite/supported/batch_cargo
```

Its `src/bin/case_*.rs` files are stale. They are not the same as the newer
per-case outputs in:

```text
target/c-testsuite-suite/supported/*.generated.rs
target/c-testsuite-suite/supported/*_rs_cargo/src/main.rs
```

The stale batch reported these apparent failures:

- `case_00180`, `case_00201`, `case_00161`, and `case_00219`: invalid `.str`
  expressions.
- `case_00130`: array values cast with `as *mut ...`.
- `case_00042`: union fields accessed on an `i32` local.
- `case_00055`: enum storage mixed `u32` and `enum E`.

Do not implement fixes from that list without regenerating or translating the
individual source first. The current per-case output already shows the repaired
forms:

- `00130.generated.rs` uses `arr.as_mut_ptr()` and `v.as_mut_ptr()`.
- `00180.generated.rs` uses `c"abcdef"` instead of `.str`.
- `00042.generated.rs` declares and accesses `union anon_0`.
- `00055.generated.rs` stores `E` and casts it only when an integer value is
  required.

Use `diff` between the batch copy and the current generated file before trusting
any cached compiler diagnostic. Targeted per-case Cargo directories can be
checked without running the suite, for example:

```bash
cargo check --manifest-path \
  target/c-testsuite-suite/supported/00130_rs_cargo/Cargo.toml
```

The c-testsuite harness has no single-case environment filter. Use direct
translation plus the per-case Cargo directory for focused work. Only rebuild
the whole batch when it is time for a fresh broad test run.

## Repairs already present

### Typed operation lowering

The current commit restores much of the old lowerer's semantic coverage on top
of clang-ir's typed operations:

- constants, globals, loads, stores, copies, casts, pointer element/stride/diff
- function-pointer casts and calls
- comparison, select, unary/binary arithmetic, saturating and overflow ops
- arrays, vectors, aggregate constants, anonymous structs/unions
- bitfields, atomics, va_list operations, asm outputs
- memory intrinsics, math intrinsics, EH, stack/address/prefetch operations

Keep comparison kinds 6 and 7 separate. They are not equivalent:

- kind 6 is ordered-not-equal: `(lhs < rhs) || (lhs > rhs)`
- kind 7 is unordered: `(lhs != lhs) || (rhs != rhs)`

### Long double source facts

Long-double constants must come from Clang AST evaluation, joined to CIR by
source location. CIR's long-double attribute representation truncates bytes and
is not an acceptable fallback.

The macro-dump plugin now emits exact AST-evaluated long-double facts for:

- global/static initializer leaves as `GLOBAL_LONG_DOUBLE`
- non-`FloatingLiteral` expressions that Clang can constant-evaluate, emitted
  at expression/begin/end spelling and expansion locations

`src/frontend/c_ast.rs` joins those plugin facts into the existing floating
literal/global queues. `Lowerer::ast_floating_literal` resolves typed
`SourceLocation::File`, `Fused`, `Loc`, aliases, and unknown locations. This
location resolution was required because clang-ir can retain an op location as
an alias such as `Loc("#loc480")` while the relevant source point is inside a
fused location.

The invariant is deliberate:

- Constant-evaluable long-double expressions use exact AST value/bits.
- Nonconstant long-double expressions are lowered as runtime operations and do
  not need a constant fact.
- A `cir.const` long double with no matching AST fact emits a loud lowering TODO
  instead of using truncated CIR bytes, reparsing CIR text, substituting zero,
  or inferring a builtin such as infinity/NaN by name.

Do not restore a CIR-text fallback. Passing infinity or NaN as bytes has the
same correctness requirement as any other long-double value; builtin-name
inference merely hides the truncation bug.

The plugin was rebuilt against:

```text
/home/takashi/llvm-project/build-cir/bin/clang
```

`ieee__copysign1` and `ieee__copysign2` passed after this change. The plugin
emitted the expected 80-bit values for their global long-double arrays,
including signed zero, infinities, and NaNs.

## Targeted fixtures observed passing

These were individually exercised during the repair loop:

```text
930628-1
950714-1
20040709-1
pr58662
pr71554
float-floor
ieee__compare-fp-1
pr97073
pr34415
anon-1
pr92618
20121108-1
pr92904
20000314-2
builtin-prefetch-1
20030323-1
20060420-1
20041214-1
920501-8
920929-1
ieee__cdivchkld
bitfld-1
cbrt
pr58574
pr47538
call-trap-1
pr71626-1
pr71626-2
struct-ret-1
980223
ieee__copysign1
ieee__copysign2
```

Do not infer that neighboring fixtures pass. This list is useful for regression
checks after touching the same lowering families.

## Likely remaining fixes

The real remaining queue is not known until fresh artifacts are produced. The
highest-probability problems are semantic cases lost when old string-based
handlers were replaced with typed-op dispatch:

1. Missing dispatch arms or incomplete typed attribute extraction. A handler
   may compile but silently return because an attribute moved from string data
   to `Attribute::SymbolRef`, an alias, a dialect attribute, or a structured
   value.
2. Place/value confusion in casts, loads, and stores. Arrays must decay via an
   address expression such as `ArrayPtr`/`as_mut_ptr`, records need ABI
   coercion rather than primitive `as`, function pointers need normalized
   `Option<fn>` transmutation, and enum places need enum storage with integer
   casts only at value boundaries.
3. Alias expansion before record lookup. Anonymous struct/union aliases can
   have a mangled alias key different from the literal record name. Resolve the
   typed alias before deriving the Rust record name.
4. Source-location joins. When an AST fact exists but is not found, inspect the
   complete typed location/alias/fused-location chain before changing constant
   semantics.
5. Aggregate recursion. Constant arrays must recursively render elements using
   their declared type; do not reduce every element to `u8`. ABI coercion
   records and unions must remain records, not collapse to `i32`.
6. Runtime mismatches after generated Rust compiles. Check pointer stride,
   signedness, comparison predicate, volatile/atomic behavior, va_list copying,
   and bitfield storage offsets before blaming fixups.

When a failure is found, search for all generated occurrences of the same bad
Rust shape before changing code. Examples:

```bash
rg -n ' as \*mut| as \*const|\.str|cannot find value|expected .* found' target
rg -n 'todo!|lower: unhandled|without Clang AST value' target
```

Keep fixes in the raw lowerer. Do not add a backend rewrite to repair malformed
or semantically wrong raw lowering.

## Known cleanup after correctness

The last compile had dead-code warnings for location helpers and constant
helpers including `resolve_loc`, `resolve_expansion_source_point`,
`resolve_macro_group_loc`, `const_vector_expr`, `f128_from_text`, the
`CirComplexComponent::Uint` variant, and `long_double_from_text`. Some are
leftovers from removing raw CIR long-double fallback. Remove them only after
the fixture loop is stable; they are not the current objective.

## Finish criteria

Before closing `slate-cevu.7`:

1. Fresh generated artifacts, not cached batch files, compile.
2. Targeted regression fixtures still match C at runtime.
3. `cargo fmt` passes.
4. `cargo clippy --release --all-targets` passes or any pre-existing warning is
   explicitly accounted for.
5. `cargo nextest r --release --profile lowering --no-fail-fast` passes 100%.
6. Update and close the bead only when no required work remains.
7. Check `git status`; do not commit or push without explicit authorization.
