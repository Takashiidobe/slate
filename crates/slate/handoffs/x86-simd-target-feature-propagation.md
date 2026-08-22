# Handoff: vector abs lowering + target_feature propagation (slate-3f8g.4.9)

## Bug 1: vector `cir.abs` lowered to scalar `.abs()`

- `_mm_abs_epi8` etc. hit `lower_abs` (`src/frontend/lowerer/arithmetic.rs`)
  with a vector `result_ty`, but the fallback path called `.abs()` on the
  whole `[T; N]` array (no such method) instead of per-lane.
- Fix: detect vector via `parse_cir_vector_type`, expand into an `ArrayLit`
  of per-lane `wrapping_abs()` (int elems) or `abs()` (float elems), same
  shape as the existing vector-binop lowering (`lower_binary`).
- Element-kind check has to go through `named_scalar_type` (made
  `pub(super)`): CIR vector element types show up as `CirType::Named("s8i")`
  etc., not `CirType::Int { .. }` directly.

## Bug 2: no `#[target_feature]` propagation at all

- C `__attribute__((target("ssse3")))` / `-mavx2` etc. never made it to the
  generated Rust. Any call to a real (non-array-arithmetic) LLVM x86
  intrinsic — `pshufb`, `crc32`, anything gated above SSE2 baseline —
  produced Rust that failed with `rustc-LLVM ERROR: Cannot select`.
  Pre-existing, affected the *existing* `x86_instruction_intrinsics.c`
  fixture too (sse4.2 crc32), not something the new fixture introduced.
- CIR carries this as trailing op attrs on `cir.func`: `"cir.target-cpu"`
  and `"cir.target-features"` (comma list, `+feat`/`-feat`). Was previously
  unparsed — reachable only via `Function.raw` (untyped escape hatch).

### Structured fix (per user ask: frontend should model this, not slate)

- `clang-ir` crate (`~/Projects/clang-ir/clang-ir/src/model/function.rs`):
  added typed `Function.target_cpu: Option<String>` and
  `Function.target_features: Vec<TargetFeature>` (`{name, enabled}`),
  parsed in `Function::from_op`. Re-exported `TargetFeature` from
  `clang_ir::model`.
- slate (`src/frontend/lowerer.rs`):
  - `target_feature_functions(module)`: baseline = intersection of enabled
    features across every function that has any; per-function extras =
    enabled features not in baseline.
  - `rustc_target_feature_name`: maps LLVM `crc32` → rustc `sse4.2` (rustc
    has no separate `crc32` x86 target-feature name; dropping it silently
    was wrong — it left `_mm_crc32_u8` with zero added features and no
    attribute at all).
  - Functions with extras get `Attr::TargetFeature("a,b,c")` (new variant,
    `src/backend/rust_ast.rs` + `codegen.rs`, renders
    `#[target_feature(enable = "a,b,c")]`) and are folded into the existing
    `unsafe_functions` set — reuses the call-site `unsafe {}` wrapping
    that mechanism already drives, no new call-site logic needed.

## Fixture

- `tests/fixtures/x86_simd_vector_intrinsics.c`: SSE2 add/storeu, SSSE3
  abs+shuffle, AVX2 add/storeu (each ISA-gated fn tagged
  `__attribute__((target(...)))` to match real usage).

## Verification (differential harness is disabled project-wide right now)

- Manual: `cargo run -- translate`, drop output into a scratch cargo
  project (`vendor/{aligned,bitint,num-complex}` deps, matching
  `tests/support/mod.rs`'s manifest), `cargo build` + run, diff stdout/exit
  vs `clang -mssse3 -mavx2 ...`. Byte-identical for both the new fixture
  and `x86_instruction_intrinsics.c`.
- `cargo nextest r --release --profile lowering`: 7/7 pass, no regressions.
- `cargo fmt` / `cargo clippy --release`: clean.
