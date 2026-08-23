# x86/ARM/RISC-V intrinsic (`call_llvm_intrinsic`) lowering

CIR represents `<immintrin.h>`/`<arm_neon.h>`/`__builtin_ia32_*` calls as
`cir.call_llvm_intrinsic`. The op carries fully concrete, resolved operand and
result types at every call site, even for LLVM-level *overloaded* intrinsics
(one `.td` definition, many concrete instantiations) — so slate never needs to
resolve LLVM's type-DAG overloading for codegen, only for computing the link
name.

## Design

For each `call_llvm_intrinsic` op, declare a local
`extern "unadjusted" { #[link_name = "llvm.<name>[.<mangled suffix>]"] fn ...; }`
(mirrors what `core::arch::x86_64`/stdarch does internally) and call it
directly. Requires nightly `#![feature(link_llvm_intrinsics, abi_unadjusted)]`.
The shim name is a hash of `{sanitized}__{ret_type:?}__{param_types:?}`, so
distinct concrete overload instantiations of the same intrinsic name get
distinct shim declarations for free.

Vector-typed operand/result positions at the shim boundary use
`std::simd::Simd<T,N>` (`#[feature(portable_simd, simd_ffi)]`), converted at
the call boundary only (`Simd::from_array`/`.to_array()`) — the rest of the
generated function keeps using plain arrays. Plain `[T; N]` arrays lower to
LLVM array types, not LLVM vector types, and nothing coerces one into the
other at an extern/FFI boundary; a hand-rolled `#[repr(simd)]` newtype over an
array is a confirmed dead end (triggers a MIR-validation ICE outside
`core`/`std` bootstrap privileges).

`<N x i1>` masks are CIR's `[bitint::BInt<1,1,1>; N]` (byte-packed booleans).
`memory.rs` has `bitint_vector_lane_bits`/`packed_mask_int_type`/
`pack_bitint_vector_expr`/`unpack_bitint_vector_expr` (generalized to any lane
bit-width) for the general bitcast case, but `call_llvm_intrinsic`'s own mask
arguments need scalar packing at the shim boundary specifically — see below.

## The `slate-intrinsic-gen` sibling crate

`/home/takashi/Projects/slate-intrinsic-gen` (sibling of `slate`, not a git
repo yet — deliberate, user decision pending). Shells out to a small C++
program (`cpp/extract_intrinsics.cpp`) linked against the real LLVM build,
calling `Intrinsic::isOverloaded`/`getType`/`getAttributes`/
`getIntrinsicInfoTableEntries` as black boxes rather than re-parsing `.td`
DAG-based overload resolution (`LLVMMatchType`, `llvm_anyvector_ty`) by hand —
robust against LLVM version drift since it asks LLVM itself, not a
hand-copied algorithm. `overloaded_positions` (which return/param positions
are free type variables) comes from a linear walk of the pre-decoded
`IITDescriptor` array LLVM's own `IntrinsicEmitter` uses.

Coverage against one LLVM build (24.0.0git): x86 100% (17/17 overloaded
resolved), arm 100% (318/318), riscv 100% (766/766), aarch64 88%
(1250/1420 — the unresolved 170 are all `llvm.aarch64.sve.*`, using IIT
descriptor kinds the walker doesn't decode; fails closed, not silently
wrong; SVE only reached via explicit ACLE types like `svfloat32_t`, low
priority).

Generates `intrinsics_table.rs` (machine-generated, regenerate rather than
hand-edit):

```bash
cd /home/takashi/Projects/slate-intrinsic-gen && cargo build --release
./target/release/slate-intrinsic-gen \
  --llvm-build ~/llvm-project/build --llvm-src ~/llvm-project \
  --prefix x86 --prefix aarch64 --prefix arm --prefix riscv \
  --stdarch-src "$(rustc --print sysroot)/lib/rustlib/src/rust/library/stdarch/crates/core_arch/src" \
  --out /home/takashi/Projects/slate/src/frontend/lowerer/intrinsics_table.rs
```

Extending mining to a new arch dir (`arm`/`aarch64`/`riscv*`) is the same
mechanical line-scanner as x86 — no new parsing logic. Rename any
per-arch override table to one combined table if adding more; matching is
already scoped by intrinsic-name prefix (`llvm.aarch64.*` vs `llvm.x86.*`),
so no cross-arch collision risk from merging.

## Legacy vs. canonical naming divergence (AVX512 masks)

LLVM's canonical mangled name (from `Intrinsic::getName()`) and the X86
backend's actual instruction-selection patterns can diverge for historical
intrinsic families. Confirmed for AVX512 `mask.expand`: the verifier accepts
`llvm.x86.avx512.mask.expand.v16i32` (modern canonical form), but the backend
has no ISel pattern for it (`Cannot select` at codegen time, not a
verification failure) — only the pre-unification legacy name
(`llvm.x86.avx512.mask.expand.d.512`) actually selects. No "ask LLVM" API
(`getIntrinsicInfoTableEntries` included) detects this ahead of time; it's a
backend implementation gap, not a naming/type-system fact.

**Resolution: mine stdarch's own `#[link_name = "llvm...."]` attributes** as
the source of truth for "what name and Rust type shape actually works" —
`mine_stdarch_overrides` in `slate-intrinsic-gen` scans stdarch's
`core_arch/src/{x86,x86_64,arm,arm_shared,aarch64,riscv32,riscv64,
riscv_shared}` dirs for `#[link_name]` extern-fn declarations, pairing each
with its mined Rust param/return type spellings into a `STDARCH_OVERRIDES`
table. `find_stdarch_override` (`intrinsics.rs`) looks up a call site's
concrete resolved types against this table by rendering them into stdarch's
own naming convention (`stdarch_shape`); a unique match wins over the
computed canonical name. This is strictly additive — falls back to the
canonical `mangled_link_name` computation when no override matches, so the
already-correct non-overloaded and non-AVX512-mask overloaded paths (crc32,
pause/rdtsc/fences, `llvm.ctpop.v4i32`) are unaffected.

RISC-V's RVV (vector extension) intrinsics are heavily overloaded (766/836)
and structurally similar to AVX512's mask family — **do not assume the x86
fix generalizes unchanged**; spot-check one masked/tail-predicated RVV
load/store through the full pipeline before trusting the override-table
approach for RISC-V. AArch64/ARM NEON (504/318 overloaded, 100% resolved by
the extractor) is a much closer shape to ordinary SSE/AVX and is the safer
first cross-arch target.

## `target_feature` propagation

C `__attribute__((target("ssse3")))` / `-mavx2` arrive on `cir.func` as
untyped trailing attrs (`cir.target-cpu`, `cir.target-features`), previously
unparsed. Fixed at the frontend layer per design preference (model it in
`clang-ir`, not slate): `Function.target_cpu`/`Function.target_features`
(typed, `~/Projects/clang-ir/clang-ir/src/model/function.rs`). Slate computes
a whole-module baseline (intersection of enabled features across every
function that declares any) plus per-function extras, and emits
`#[target_feature(enable = "...")]` only for the extras — folded into the
existing `unsafe_functions` call-site-wrapping mechanism, no new call-site
logic needed. `rustc_target_feature_name` maps LLVM feature spellings with no
1:1 rustc equivalent (e.g. `crc32` → `sse4.2`) rather than dropping them
silently.

Vector `cir.abs` (e.g. `_mm_abs_epi8`) needs per-lane expansion
(`wrapping_abs()`/`abs()` inside an `ArrayLit`, same shape as vector binops)
— the scalar `.abs()` fallback doesn't apply to `[T; N]`.

## Cross-execution gap for ARM/RISC-V

Verifying ARM/RISC-V intrinsic lowering end-to-end (not just "generated Rust
looks plausible") needs a cross-compile + cross-execute loop this machine
didn't have as of the initial extension effort: `qemu-user-static` (binfmt
direct-binary execution, not full-system emulation), a RISC-V glibc rustup
target (`riscv64gc-unknown-linux-gnu`, not the bare-metal `-none-elf` one),
and cross-glibc/cross-binutils for both `SLATE_CC` and the generated Rust's
linker. `tests/differential.rs` being disabled compounds this — no automated
regression harness exists for any arch, x86 included, only manual verification
per change.
