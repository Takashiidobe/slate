# Handoff: extending intrinsic lowering to ARM/AArch64/RISC-V

## Prerequisite reading

Read `handoffs/x86-intrinsics-lowering.md` first — it covers the whole
`call_llvm_intrinsic` lowering path this handoff extends: the
`slate-intrinsic-gen` sibling crate, the `intrinsics_table.rs` generated
table, the stdarch-mined override table, and the `std::simd::Simd<T,N>`
extern-boundary fix. Everything below assumes that context.

## Repo state right now

- `bb7fd8d6` "impl override table for intrinsics" is on `main`, containing
  all of the x86 stdarch-override + `Simd<T,N>` work.
- One further **uncommitted** fix on top, in `src/frontend/lowerer.rs`
  (`git diff --stat`: 13 lines): a pre-existing bitfield-storage regression
  (unrelated to ARM/RISC-V, found while investigating gcc-torture failures
  during this work) — `trust_cir` in `lower_record_def` was clobbering
  bitfield storage/wrapper field types. Fixed and verified
  (`gcc_torture_supported_tests_match_c` passes, full `lowering` profile
  7/7). Filed and closed as `slate-w42t`. This should get committed
  alongside or before ARM/RISC-V work — it's small and independent.
- `slate-intrinsic-gen` (`/home/takashi/Projects/slate-intrinsic-gen`,
  sibling of `slate`, still not a git repo — that's still a decision for
  the user) already extracts `AARCH64_INTRINSICS`, `ARM_INTRINSICS`, and
  `RISCV_INTRINSICS` tables into `intrinsics_table.rs` today — the LLVM-side
  extraction (`cpp/extract_intrinsics.cpp`, `overloaded_positions` walk) is
  already prefix-generic, not x86-specific. What's x86-only right now is
  purely the **stdarch override mining** (`mine_stdarch_overrides` in
  `slate-intrinsic-gen/src/main.rs`), which only scans `--stdarch-src`'s
  `x86`/`x86_64` subdirectories, and the lookup table it produces
  (`X86_STDARCH_OVERRIDES` in `intrinsics_table.rs`, consumed by
  `find_stdarch_override` in `src/frontend/lowerer/intrinsics.rs`).
- The `std::simd::Simd<T,N>` extern-boundary conversion in
  `lower_call_llvm_intrinsic` (`intrinsics.rs`) is **already
  architecture-agnostic** — it triggers off `Type::Array` shape and a
  supported `SimdElement` prim, not off the intrinsic name or arch. ARM and
  RISC-V calls get this for free with zero additional code.

## What's actually missing for ARM/RISC-V

### 1. Extend stdarch mining beyond x86/x86_64

`slate-intrinsic-gen/src/main.rs`'s `mine_stdarch_overrides` currently
hardcodes:
```rust
for dir in ["x86", "x86_64"] {
```
stdarch's `core_arch/src/` (confirmed present locally at
`$(rustc --print sysroot)/lib/rustlib/src/rust/library/stdarch/crates/core_arch/src`)
also has `arm`, `arm_shared`, `aarch64`, `riscv32`, `riscv64`,
`riscv_shared` — extending the loop to those dirs is the same mechanical
line-scanner, no new parsing logic needed.

**Also rename `X86_STDARCH_OVERRIDES` to a single combined
`STDARCH_OVERRIDES`** (or similar) rather than one table per arch — cheap
to just dump every mined entry from every dir into one array, since
`find_stdarch_override`'s matching is already scoped by the full
`llvm.<intrinsic_name>.` prefix (arch is implied by the prefix, e.g.
`llvm.aarch64.*`/`llvm.arm.*`/`llvm.riscv.*` vs `llvm.x86.*` — no
cross-arch collision risk). Splitting per-arch buys nothing and just adds
a second lookup site to keep in sync.

### 2. Verify RISC-V's overloaded/legacy-naming risk before assuming it's fine

From the original mining run (see the x86 handoff's coverage table):
RISC-V has **766/836 overloaded intrinsics** — the RVV (vector extension)
intrinsics are heavily overloaded, structurally similar to AVX512's
mask-intrinsic family. AVX512's `mask.expand` turned out to have a real
LLVM canonical-vs-legacy-ISel-selection divergence (see the x86 handoff's
"Open problem" + its resolution). It is **not safe to assume this
generalizes cleanly** — do one spot repro (a masked/tail-predicated RVV
load or store, e.g. `vle32_v_i32m1_m`-style) through the *whole* pipeline
(translate-project → cross-compile → run under emulation, see below)
before assuming the x86 fix's shape (stdarch override + `Simd<T,N>`)
covers RISC-V unchanged.

AArch64/ARM NEON intrinsics (504/318 overloaded, 100% resolved by the
extractor per the x86 handoff's table) are a much closer shape to ordinary
SSE/AVX (no legacy-vs-canonical split known yet) — lower risk, good first
target to validate the pipeline before tackling RISC-V's RVV surface.

### 3. Known, deliberately out-of-scope gap: AArch64 SVE

170 of 1420 overloaded AArch64 intrinsics are `llvm.aarch64.sve.*` and are
**unresolvable by the LLVM-side extractor** — `Intrinsic::getIntrinsicInfoTableEntries`'s
`IITDescriptor` walk in `cpp/extract_intrinsics.cpp` doesn't decode
`OneNthEltsVec`/`Subdivide2`/`Subdivide4` descriptor kinds (scalable-vector
narrowing/widening). The walker fails closed (`overloaded_positions: null`,
same as "not yet supported" — not a silent wrong answer). Low priority:
SVE only gets reached via explicit ACLE types (`svfloat32_t` etc.) in
source C, so ordinary NEON code is unaffected. Revisit only if SVE support
is explicitly requested.

## Testing — the actual blocker, checked this session

Two things need to be true before any of this can be verified end-to-end
(not just "generated Rust looks plausible"), and **neither is true on this
machine right now**:

1. **No cross-execution capability.** Checked directly:
   - `qemu-user`/`qemu-user-static` (needed to *run* cross-compiled
     ARM/RISC-V binaries directly on this x86_64 host) are available via
     pacman (`extra/qemu-user`, `extra/qemu-user-static`,
     `extra/qemu-user-binfmt`, `extra/qemu-user-static-binfmt`) but **not
     installed**. Only `qemu-system-riscv`/`qemu-base` are installed —
     full-system emulation, not what we want (too slow/heavy for a test
     loop; we want direct binary execution via binfmt, not booting a VM).
   - `rustup target list --installed` has `aarch64-unknown-linux-gnu`
     (glibc target, has `std` — good, ready to use) but for RISC-V only
     `riscv64gc-unknown-none-elf` (bare-metal, **no std** — useless for
     slate-generated programs that link `std`/`libc`). Need
     `rustup target add riscv64gc-unknown-linux-gnu`.
   - Both `rustc` (for the generated Rust) and `SLATE_CC`/clang (for the
     C baseline) need a real cross-glibc + cross-binutils to link against
     for their respective target triples — `aarch64-linux-gnu-gcc`/
     `riscv64-linux-gnu-gcc` packages from pacman provide this (sysroot +
     linker); confirmed available via `pacman -Ss`, not yet installed.
   - `~/llvm-project/build*/bin/clang --print-targets` already has
     `aarch64`/`arm`/`riscv32`/`riscv64` backends built in — no clang
     rebuild needed, just the sysroot/libc side.
2. **`tests/differential.rs` is fully disabled** (project memory: off
   since around 2026-08-21, `// Diff tests are disabled while working on
   lowering`). No automated harness exists to plug a new arch into even
   for x86 today — this is a separate, larger decision (re-enabling it,
   scoping what subset runs per-arch) that should probably happen before
   or alongside ARM/RISC-V work, not after, or ARM/RISC-V intrinsic fixes
   will suffer the same "verified once by hand, no regression coverage
   afterward" gap the x86 work has today.

**Recommended setup sequence** when picking this back up:
```bash
sudo pacman -S qemu-user-static qemu-user-static-binfmt \
    aarch64-linux-gnu-gcc riscv64-linux-gnu-gcc   # exact pkg names TBD, verify at pacman -Ss time
rustup target add aarch64-unknown-linux-gnu        # already installed, but confirm
rustup target add riscv64gc-unknown-linux-gnu
```
Then a manual smoke test analogous to the x86 handoff's "How to reproduce"
section: pick one NEON intrinsic fixture, `translate-project` it,
cross-compile both the clang baseline and the generated Rust for
`aarch64-unknown-linux-gnu` (`-C linker=aarch64-linux-gnu-gcc` or similar),
run both under `qemu-aarch64` (via binfmt or directly:
`qemu-aarch64 -L /usr/aarch64-linux-gnu ./binary`), diff stdout/exit code —
exactly the x86 pattern, just cross-executed.

## Suggested order of work

1. Land the uncommitted bitfield fix (`slate-w42t`) — small, independent,
   currently blocking a clean `main`.
2. Install qemu-user-static + cross-glibc packages, add the RISC-V glibc
   rustup target, do one manual NEON cross-execution smoke test by hand
   (no lowering code changes yet) to prove the execution environment
   itself works before touching the mining pipeline.
3. Extend `mine_stdarch_overrides` to `arm`/`arm_shared`/`aarch64`, rename
   the generated table to arch-agnostic `STDARCH_OVERRIDES`. Regenerate
   `intrinsics_table.rs`.
4. Pick one real ARM/AArch64 NEON intrinsic fixture (masked or otherwise
   "interesting" — not just a scalar one, since scalar ones already work
   for any arch via the existing generic path) and verify end-to-end via
   the cross-execution smoke test from step 2.
5. Only then attempt RISC-V RVV — check for AVX512-style legacy/canonical
   divergence before assuming the x86 fix generalizes (see "point 2"
   above). Extend mining to `riscv32`/`riscv64`/`riscv_shared` alongside.
6. Decide separately (probably its own conversation) whether/how to
   re-enable `tests/differential.rs`, and whether ARM/RISC-V differential
   coverage lives there or in a separate per-arch harness given the
   cross-execution requirement is qualitatively different from native x86
   execution.
