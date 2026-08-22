# Handoff: `_Float16` support

Tracked by `slate-<id>` (see `bd show` for the live tree).

## Scope decision

Investigated the full TS 18661-3 `_FloatN`/`_FloatNx` family
(`_Float16`/`_Float32`/`_Float64`/`_Float128`, `_Float32x`/`_Float64x`/`_Float128x`)
plus the glibc `_FloatN_t` typedefs. Only `_Float16` is in scope going forward:

- **`_Float32x`/`_Float64x`/`_Float128x`** — tested directly against the
  project's CIR-enabled Clang (`clang version 24.0.0git`,
  `x86_64-unknown-linux-gnu`): all three are rejected with `unknown type
  name`. Clang has never implemented TS 18661-3's "extended" family at all
  (GCC-only extension) — this isn't a target gap, it's an upstream gap.
  Nothing to lower; no valid input can contain these spellings. Not worth
  building target-dependent resolution machinery for.
- **`_Float32`/`_Float64`/`_Float128`** (the plain TS spellings) — also
  rejected as `unknown type name` on this same toolchain/target. Only
  `_Float16` and the older GNU `__float128` spelling are recognized here.
  Clang folks have mentioned wanting to pick this up at some point but it's
  TBD upstream — revisit if/when a newer Clang enables them, or if a fixture
  needs one on a target where they *are* enabled (untested).
- **`_FloatN_t`** (e.g. `_Float32_t`) — these are plain typedefs in glibc's
  `bits/floatn-common.h`, not compiler keywords. `parse_c_type`'s
  `lookup_typedef` fallback (`c_ast.rs:2110`) already resolves arbitrary
  typedefs to their underlying spelling, so once the base type is supported
  these resolve for free. `libc-shim/` doesn't currently declare
  `bits/floatn-common.h` at all — that's a stdlib header gap, not a lowering
  gap, and is out of scope here unless a fixture needs it.

## `_Float16` itself: confirmed to work end-to-end

- Clang recognizes `_Float16` as a keyword on x86_64 and emits `!cir.f16`
  (`CirType::Fp16`, already present in the vendored `clang-ir-types` crate —
  the CIR parser already handles the `f16` type spelling, nothing needed
  there).
- Arithmetic on `_Float16` operands gets promoted to `float` by Clang before
  emitting `cir.fadd`/etc. (`cir.cast floating %x : !cir.f16 -> !cir.float`,
  op, `cir.cast floating` back down) — the arithmetic ops themselves never
  see `!cir.f16` directly on this target. `_Float16` only shows up in
  `cir.alloca`/`cir.load`/`cir.store`/function signatures/casts. Much
  smaller lowering surface than `long double`/`f128` needed — no muladd or
  quad-precision special-casing required.
- Unlike `float`, `_Float16` does **not** get promoted under C's default
  argument promotions when passed to a variadic function — confirmed via a
  probe (`f(1, x)` where `x` is `_Float16`, called into a `...` parameter):
  Clang passes `!cir.f16` through unpromoted. Whatever variadic/va_list
  lowering path handles scalar float args needs to accept f16 as-is, not
  assume "float always widens to double in varargs".
- Rust nightly's `f16` (same unstable track as the already-adopted `f128`)
  compiles clean as an `extern "C"` parameter/return type on this target —
  no ABI lint, plain `half` in the emitted LLVM IR, verified with a
  standalone `rustc --emit=llvm-ir` probe. So the existing `Feature::F128` /
  nightly-gate pattern (`#![feature(f128)]`) is the right template: add
  `Prim::F16` + `Feature::F16` gating `#![feature(f16)]`, no wrapper struct
  needed (contrast with `LongDouble`, which exists only because x87 80-bit
  has no native Rust representation at all).

## Bug found while investigating (not just a gap)

`CirType::Fp16` already existed as a variant in the CIR parser (used
elsewhere for e.g. ARM `__fp16` raw-bits contexts) but Slate's own lowering
silently mapped it to `Type::Prim(Prim::I32)`
(`src/frontend/lowerer/types.rs:68`) and `cir_type_to_ctype`'s fallback
silently mapped it to `CType::Int { signed: true, bits: 32 }`
(`types.rs:690` fallthrough). Any `_Float16` fixture hitting either path
today would silently miscompile to an `i32`-typed value rather than erroring
— worth fixing regardless of the rest of this handoff.

## Sites requiring changes (grepped exhaustively for `F128`/`Fp128` as the
mirror pattern)

- `src/backend/rust_ast.rs` — add `Prim::F16` (spelling `"f16"`, parse arm),
  add `Feature::F16` (spelling `"f16"`).
- `src/frontend/c_ast.rs:2100-2109` (`parse_c_type`) — add
  `s == "_Float16"` → `CType::Float { bits: 16 }`.
- `src/frontend/lowerer/types.rs`:
  - line 68: fix `CirType::Fp16 => Type::Prim(Prim::I32)` →
    `Type::Prim(Prim::F16)`.
  - `cir_type_to_ctype` (~line 687-690): add `CirType::Fp16 => return
    CType::Float { bits: 16 }`.
  - `type_mentions_f128` (~line 445): generalize to a predicate-parameterized
    `type_mentions` helper shared with a new `type_mentions_f16`, rather than
    duplicating the whole recursive walk a second time.
- `src/frontend/lowerer.rs`:
  - `cir_type_mentions_f128`/`attr_mentions_f128` (~line 591-648): same
    generalize-to-shared-helper treatment, mirrored into `required_features`
    for `Feature::F16`.
  - `type_alignment` (~line 1055): `F128 | LongDouble => 16` — add a
    separate `F16 => 2` arm (not lumped in).
  - `rust_type()` (~line 3159): mirror the `uses_f128`/`type_mentions_f128`
    call with `uses_f16`/`type_mentions_f16`, and wire
    `insert_crate_feature(&mut items, Feature::F16)` next to the existing
    F128 one (~line 2091).
  - `CType::Float { .. } => Type::Prim(Prim::F64)` fallback (~line 3996):
    add an explicit `CType::Float { bits: 16 } => Type::Prim(Prim::F16)` arm
    before it — this fallback is exactly the second silent-miscompile bug
    (would currently map `_Float16` to `f64`, not `i32`, via this path).
- `src/backend/facts/heap_ownership.rs:369` (`type_size_bytes`) — add
  `Prim::F16 => Some(2)`.
- `src/backend/facts/values.rs:697` — add `Prim::F16` to the
  `BTreeSet::new()` arm alongside F32/F64/F128.
- `src/backend/query/sort_search.rs:671` (`prim_layout`) — add a separate
  `Prim::F16 => 2` arm.
- `src/backend/query/context.rs:6398` / `src/frontend/lowerer/constants.rs:142`
  (`default_value`/`default_value_for_type`) — add
  `Type::Prim(Prim::F16) => Expr::HexFloat("0.0f16".into())`, same shape as
  the F128 arm right above it.

No changes needed in `src/frontend/lowerer/calls.rs`'s long-double shim-call
path or `runtime_support.rs`'s `long_double_shim_type_tag` — those exist
specifically for x87/quad ABI shimming, and `_Float16` doesn't need a C
shim (real hardware `half` arithmetic works fine once promoted to
`float`/`double`, same as Rust's native `f16` support already confirmed
above).

## Fixture plan

Straight differential fixture in `tests/fixtures/`: a function taking/
returning `_Float16`, doing arithmetic, and (separately) a variadic call
passing an unpromoted `_Float16` arg, to exercise the promotion-skip
behavior specifically. Run under the `lowering` profile per
`SLATE_DIFF_FIXTURE`.
