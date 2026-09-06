# Directive-translate cfg atom classification

_created 2026-09-06_

`plan_configs` (`src/frontend/directive_translate.rs`) enumerates one clang
compile per branch of every `#if` chain, pinning that chain's atoms with
`-D`/`-U` (`chain_atoms`, `pin_args`). This is linear in branch count per
chain, which is fine when chains are independent single-purpose toggles.
It breaks down on configuration-heavy sources (zstd's `lib/common/*.h`)
where an atom tested in one chain is itself computed by an unrelated
`#ifndef`/`#define` cascade elsewhere in the TU — every atom in that cascade
currently looks like an independent branch axis, when most of them have
exactly one possible value once the target/compiler is fixed.

## Evidence (zstd)

- `lib/common/mem.h:131` — `MEM_FORCE_MEMORY_ACCESS` is `#ifndef`-guarded,
  defaulting from `__GNUC__` alone. Comment: "can be defined externally, on
  command line for example" — it is only free if a *build* overrides it.
- `lib/common/portability_macros.h:87-149` — `DYNAMIC_BMI2` →
  `ZSTD_ASM_SUPPORTED` → `ZSTD_ENABLE_ASM_X86_64_BMI2` is a derivation chain,
  each link `#ifndef`-guarded and computed only from `__clang__`, `__GNUC__`,
  `__BMI2__`, `__x86_64__`, `_M_IX86`, `ZSTD_MEMORY_SANITIZER`. None of these
  is a real degree of freedom.
- `lib/common/threading.h:19,71` — `ZSTD_MULTITHREAD` has no unconditional
  in-tree fallback anywhere. That absence is the signature of a genuinely
  free atom (confirmed by the file's own doc comment).

## Three buckets

- **Fixed** — atoms `known_cfg`/`pred_to_cfg` already resolve per compile
  (target arch/os/vendor/endian, compiler family). One value per
  (target, compiler) pin.
- **Derived** — atoms whose *every* definition site in the TU is
  `#ifndef X ... #define X <expr over fixed/derived atoms only> ... #endif`.
  Fold to a concrete value by the same fixed-point pass, no branching.
- **Free** — the residual: no unconditional fallback, or a fallback that
  still bottoms out on an undefined atom. These are the only real branch
  axes, and empirically there are a handful per file, not the dozens that
  appear textually in `#if` lines.

## Algorithm

Before `plan_configs` counts variants, walk the TU's `#ifndef X` / `#define
X <expr>` pairs and classify atoms by fixed-point evaluation: fixed atoms
are known immediately; an atom becomes derived once every atom in its
guarded `<expr>` is itself fixed or derived; whatever is left when the walk
stabilizes is free. Substitute the evaluated value for every fixed/derived
atom directly into `pin_args` as `-DX=<value>` (no variant created); restrict
`chain_atoms`, branch enumeration, and cross-product clustering to free
atoms only. On `portability_macros.h` this collapses the whole file to zero
extra variants per target; `threading.h` keeps its real branch on
`ZSTD_MULTITHREAD`/`DEBUGLEVEL`.

This composes with clustering coupled chains (connected components over a
def/use graph, for the case where two *free* atoms are cross-referenced):
clustering only needs to run over the free-atom residual, which is far
smaller than the raw atom count in the source text.

## Feature macros belong in the fixed bucket

`known_cfg` (`src/frontend/preprocess.rs:1414`) maps `__x86_64__`,
`__aarch64__`, etc. to `target_arch`, but not CPU *feature* macros
(`__BMI2__`, `__AVX2__`, `__SSE4_2__`, `__ARM_NEON`, ...). Without an entry,
`__BMI2__` is opaque to the classifier and stalls `DYNAMIC_BMI2`'s
derivation at "unknown atom" instead of folding it away. Adding these,
mapped to `cfg(target_feature = "bmi2")` etc. (a real Rust cfg key), moves
them into the fixed bucket the same way `known_cfg` already handles arch/os
— tracked as `slate-c16q.2`.

**Caveat**: `DYNAMIC_BMI2` itself is a *runtime*-dispatch flag — its C use
selects between "always call the generic path" and "compile both paths,
pick at runtime via CPUID." Translating that selection to a static
`#[cfg(target_feature = "bmi2")]` fork is a semantic mismatch: it bakes in
whatever feature set slate's target happens to have, discarding runtime
dispatch across unknown hardware. That case needs translation to
`is_x86_feature_detected!` at the call site, not a cfg fork, and is
explicitly out of scope for `slate-c16q`.

## Tracking

`slate-c16q` (epic), `slate-c16q.1` (classifier), `slate-c16q.2` (known_cfg
feature-macro entries). See also
[cfg translation demo](../log/2026-09-06-15-34.md) and
`tests/fixtures.cfg/README.md` for the existing target/os/arch cfg surface
this extends.
