# Extending intrinsic lowering to ARM/AArch64/RISC-V

Builds on [x86-intrinsic-lowering.md](x86-intrinsic-lowering.md) — the whole
`call_llvm_intrinsic` path, `slate-intrinsic-gen`, `intrinsics_table.rs`, and
the `std::simd::Simd<T,N>` extern-boundary conversion are already
architecture-agnostic. What's x86-only today is purely the stdarch
override-mining step.

## What's already generic vs. what's x86-only

- LLVM-side extraction (`cpp/extract_intrinsics.cpp`, the
  `overloaded_positions` walk) is prefix-generic already — arch is just
  another `--prefix` flag.
- The `Simd<T,N>` extern-boundary conversion triggers off `Type::Array` shape
  and a supported `SimdElement` prim, not off intrinsic name or arch — ARM
  and RISC-V calls get it for free with zero additional code.
- `mine_stdarch_overrides` hardcodes scanning stdarch's `x86`/`x86_64`
  subdirectories only. stdarch's `core_arch/src/` also has `arm`,
  `arm_shared`, `aarch64`, `riscv32`, `riscv64`, `riscv_shared` — extending
  the loop to those dirs is the same mechanical line-scanner, no new parsing
  logic. Rename the generated `X86_STDARCH_OVERRIDES` to an arch-agnostic
  combined `STDARCH_OVERRIDES` table while doing this — `find_stdarch_
  override`'s matching is already scoped by the full `llvm.<name>.` prefix,
  so there's no cross-arch collision risk from merging, and a single lookup
  site is cheaper to keep in sync than one table per arch.

## Risk profile per target

- **AArch64/ARM NEON** (504/318 overloaded, 100% resolved by the extractor):
  structurally close to ordinary SSE/AVX, no known legacy-vs-canonical
  naming split. Lower risk, the recommended first cross-arch target to
  validate the pipeline.
- **RISC-V RVV** (766/836 overloaded): heavily overloaded, structurally
  similar to AVX512's mask-intrinsic family, which turned out to have a real
  LLVM canonical-vs-legacy-ISel divergence (see the x86 doc's "legacy vs.
  canonical naming" section). **Do not assume this generalizes cleanly** — do
  one spot repro (a masked/tail-predicated RVV load/store) through the whole
  pipeline before trusting the x86 fix's shape unchanged.
- **AArch64 SVE** (170/1420 overloaded AArch64 intrinsics, `llvm.aarch64.
  sve.*`): deliberately out of scope. Unresolvable by the LLVM-side extractor
  — `IITDescriptor` kinds `OneNthEltsVec`/`Subdivide2`/`Subdivide4`
  (scalable-vector narrowing/widening) aren't decoded. Fails closed
  (`overloaded_positions: null`), not a silent wrong answer. Only reached via
  explicit ACLE types (`svfloat32_t`) in source C — revisit only if
  explicitly requested.

## Blocker: no cross-execution capability

Verifying ARM/RISC-V lowering end-to-end (not just "generated Rust looks
plausible") needs, none of which were present as of the initial extension
attempt:

- `qemu-user`/`qemu-user-static` + binfmt registration, for **direct binary
  execution** on the x86_64 host — not `qemu-system-riscv` (full-system
  emulation is too slow/heavy for a test loop).
- `riscv64gc-unknown-linux-gnu` rustup target (glibc, has `std`) — not the
  bare-metal `riscv64gc-unknown-none-elf` target, which is useless for
  programs linking `std`/`libc`. `aarch64-unknown-linux-gnu` is typically
  already installed.
- Cross-glibc + cross-binutils (`aarch64-linux-gnu-gcc`/
  `riscv64-linux-gnu-gcc` packages) for both `SLATE_CC`'s C baseline and the
  generated Rust's linker.
- clang's backend support (`aarch64`/`arm`/`riscv32`/`riscv64` in
  `--print-targets`) is typically already present — no clang rebuild needed,
  just the sysroot/libc side.

Separately, `tests/differential.rs` being disabled means there's no
automated harness to plug a new arch into even for x86 today — re-enabling
it (and scoping what subset runs per-arch) is a prerequisite decision, not an
afterthought, or ARM/RISC-V fixes inherit the same "verified once by hand, no
regression coverage afterward" gap x86 has.

## Suggested order of work

1. Install `qemu-user-static` + cross-glibc packages, add the RISC-V glibc
   rustup target, do one manual NEON cross-execution smoke test by hand (no
   lowering code changes) to prove the execution environment itself works.
2. Extend `mine_stdarch_overrides` to `arm`/`arm_shared`/`aarch64`, rename
   the table to the arch-agnostic `STDARCH_OVERRIDES`, regenerate
   `intrinsics_table.rs`.
3. Pick one real NEON intrinsic fixture (masked/"interesting", not scalar —
   scalar already works for any arch via the existing generic path) and
   verify via the cross-execution smoke test.
4. Only then attempt RISC-V RVV — check for an AVX512-style legacy/canonical
   divergence before assuming the x86 fix generalizes. Extend mining to
   `riscv32`/`riscv64`/`riscv_shared` alongside.
5. Decide separately whether/how to re-enable `tests/differential.rs`, and
   whether ARM/RISC-V differential coverage lives there or in a
   cross-execution-specific harness.
