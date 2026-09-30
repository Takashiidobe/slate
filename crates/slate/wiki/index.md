# Index

- [Architecture](concepts/architecture.md)
- [Differential fixtures](concepts/differential-fixtures.md)
- [Lowerer internals](concepts/lowerer-internals.md)
- [Passes](concepts/passes.md)
- [Cross-target toolchains](concepts/cross-target-toolchains.md)
- [gcc-torture triage](concepts/gcc-torture-triage.md)
- [Slate overview](concepts/slate-overview.md)
- [x86/ARM/RISC-V intrinsic lowering](concepts/x86-intrinsic-lowering.md)
- [ARM/RISC-V intrinsic extension](concepts/arm-riscv-intrinsic-extension.md)
- [clang-ir typed migration](concepts/clang-ir-typed-migration.md)
- [Typed-op function-body lowering](concepts/typed-op-lowering.md)
- [gcc-torture clang-ir refactor loop](concepts/gcc-torture-clang-ir-refactor-loop.md)
- [`_Float16` and FENV-constrained float](concepts/float16-and-fenv.md)
- [va_list lowering](concepts/va-list-lowering.md)
- [Pointer capability lattice](concepts/pointer-capability-lattice.md)
- [long-double-f80](concepts/long-double-f80.md)
- [predicate tokenizer no longer collapses #if conditions to opaque](concepts/predicate-tokenizer-no-longer-collapses-if-conditions-to-opaque.md)
- [Directive-driven differential fixtures use profile-scoped FileCheck](concepts/directive-driven-differential-fixtures-use-profile-scoped-filecheck.md)
- [Port differential source assertions to fixture FileCheck](concepts/port-differential-source-assertions-to-fixture-filecheck.md)
- [Target-qualified FileCheck is independent of fixture flavor](concepts/target-qualified-filecheck-is-independent-of-fixture-flavor.md)
- [FileCheck follows generated artifact ownership](concepts/filecheck-follows-generated-artifact-ownership.md)
- [Remove obsolete snapshot test infrastructure](concepts/remove-obsolete-snapshot-test-infrastructure.md)
- [clang-ir type projection accessors](concepts/clang-ir-type-projection-accessors.md)
- [Retire generic operation helpers from semantic lowering](concepts/retire-generic-operation-helpers-from-semantic-lowering.md)
- [Lowerer phase file boundaries](concepts/lowerer-phase-file-boundaries.md)
- [Reusable CIR normalization ownership](concepts/reusable-cir-normalization-ownership.md)
- [Typed module alias ownership](concepts/typed-module-alias-ownership.md)
- [ClangIR -emit-cir memory blowup on large, type-recursive TUs](concepts/clangir-emit-cir-memory-blowup.md)
- [C++ translation pain points (pre-implementation scoping)](concepts/cxx-translation-pain-points.md)
- [C++ exceptions lowering](concepts/cxx-exceptions-lowering.md)
- [C++ stdlib shim lowering](concepts/cxx-stdlib-shim-lowering.md)
- [Hybrid C++ migration with residual fallback](concepts/cxx-hybrid-migration.md)
- [Rewrite engine v2: ground-up replacement for src/backend/query + salsa](concepts/rewrite-engine-v2.md)
- [Porting a pass to the worklist engine (slate-y0qs.3 fast path)](concepts/pass-porting-workflow.md)

## Historical

Point-in-time design records for decisions that have since been superseded.
Kept for context, not as current guidance -- see the concept doc that
superseded each one for what actually applies now.

- [Salsa Migration](historical/salsa-migration.md) -- superseded by
  [Rewrite engine v2](concepts/rewrite-engine-v2.md), which dropped salsa
  for hand-rolled fact caches.
- [setjmp/longjmp lowering](historical/setjmp-longjmp-lowering.md) -- describes
  the removed `SetjmpRecovery` query pass; raw setjmp/longjmp lowering remains
  in the frontend.
- [Writing a Query-Driven Fixup](historical/writing-a-query-driven-fixup.md) --
  the retired `src/backend/query/` engine; see
  [Rewrite engine v2](concepts/rewrite-engine-v2.md).
- [Fixups](historical/fixups.md) -- query-engine matcher/`EditSet` mechanics,
  retired with `src/backend/query/`.
- [Facts](historical/facts.md) -- salsa-backed analysis layer, retired.
- libc-shim and its oracles -- [libc-shim](historical/libc-shim.md),
  [Bionic ABI](historical/android-bionic-libc-shim-abi.md),
  [Android NDK](historical/android-ndk-oracle.md),
  [FreeBSD](historical/freebsd-libc-oracle.md),
  [macOS SDK](historical/macos-sdk-oracle.md),
  [MSVC sysroot](historical/msvc-reference-sysroot.md) and the MSVC/Darwin/Linux
  libc surface audits: removed with `libc-shim/` (slate-p58o.7.9); the
  slate-parser frontend reads target headers from slate-sysroots.
- [ptr_len signature worklist with canonical forwarding](concepts/ptr-len-signature-worklist-with-canonical-forwarding.md)
- [ptr_len owned Vec signature lifting](concepts/ptr-len-owned-vec-signature-lifting.md)
- [Re-enable rewrite FileCheck for worklist baseline](concepts/re-enable-rewrite-filecheck-for-worklist-baseline.md)
- [Local libc-call rewrite table with multi-hop lifted-arg chase](concepts/local-libc-call-rewrite-table-with-multi-hop-lifted-arg-chase.md)
- [Fix length-lattice multi-buffer asymmetry (by_callee/accepted single-candidate bug)](concepts/fix-length-lattice-multi-buffer-asymmetry-by-callee-accepted-single-candidate-bug.md)
- [Local libc table: strcmp via Ordering, mem\* via intrinsic, dead-let drop](concepts/local-libc-table-strcmp-via-ordering-mem-via-intrinsic-dead-let-drop.md)
- [memcmp libc rewrite + byte-array slice lifting; ctype backed out for locale](concepts/memcmp-libc-rewrite-byte-array-slice-lifting-ctype-backed-out-for-locale.md)
- [Remove retired backend facts and Salsa](concepts/remove-retired-backend-facts-and-salsa.md)
- [Bridge fixed arrays into pointer length signatures](concepts/bridge-fixed-arrays-into-pointer-length-signatures.md)
- [prototype FileCheck updater](concepts/prototype-filecheck-updater.md)
- [Skip unavailable target FileCheck generation](concepts/skip-unavailable-target-filecheck-generation.md)
- [Fix four differential failures exposed by FileCheck generation](concepts/fix-four-differential-failures-exposed-by-filecheck-generation.md)
- [atoi/atol/atoll/atof const-fold lift (Tier A)](concepts/atoi-atol-atoll-atof-const-fold-lift-tier-a.md)
- [atoi/atol/atoll Tier B prelude helper + handwritten filecheck](concepts/atoi-atol-atoll-tier-b-prelude-helper-handwritten-filecheck.md)
- [Goto Lowering](concepts/goto-lowering.md)
