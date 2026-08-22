# Handoff: x86/ARM/RISC-V intrinsic (`call_llvm_intrinsic`) lowering

## Scope and state

This is a fresh feature area, not a bug-fix loop. Slate could not translate any
C code that hits an intrinsic (`<immintrin.h>`, `<arm_neon.h>`, `__builtin_ia32_*`,
etc.) — CIR represents these as `cir.call_llvm_intrinsic`, and slate had no
lowering handler for that op at all, so it fell through to `emit_todo` and
produced un-compilable output.

This session built the handler, a companion offline table-generator crate, and
found + fixed three unrelated pre-existing bugs it exposed along the way. The
non-overloaded case (single concrete signature per intrinsic name, e.g.
`_mm_crc32_u8`, `__builtin_ia32_pause`) is **fully working and differentially
verified** (matches real clang+glibc output byte-for-byte). The overloaded
case (one LLVM intrinsic definition, many concrete instantiations, e.g. AVX512
`mask.expand`) is **not yet working** — the last thing done this session was
diagnosing exactly why, and landing on a plan the user wants implemented next.
That plan is not started.

### Repo state right now

- Commit `6a8b252f` "start generating intrinsics" is already on `main`,
  1 commit ahead of `origin/main`, **not pushed**. It contains the bulk of
  this session's work: the `call_llvm_intrinsic` handler, the `ExternFnDecl`
  attrs field, the vector/mask cast fix in `memory.rs`, the shared-types CIR
  field-type fix, and `main.rs` plumbing for merged CIR aliases.
- Working tree has further **uncommitted** changes on top of that commit:
  `src/frontend/lowerer.rs` (adds `mod intrinsics_table;`) and
  `src/frontend/lowerer/intrinsics.rs` (adds LLVM-name mangling for overloaded
  intrinsics). Diff is small (~65 lines), shown in full further down.
- `src/frontend/lowerer/intrinsics_table.rs` is **untracked**, machine-
  generated, ~4780 lines, not yet `git add`ed. Regenerate it rather than hand-
  edit it (see "slate-intrinsic-gen" section).
- No fixture exists yet in `tests/fixtures/` for the overloaded/vector case.
  `tests/fixtures/x86_instruction_intrinsics.c` exists and covers the
  non-overloaded case (crc32/pause/lfence/mfence/sfence/rdtsc/rdtscp) — this
  was the existing fixture found at the start of the session, not new.
- Recall from project memory: `tests/differential.rs` is currently fully
  commented out (`// Diff tests are disabled while working on lowering`) and
  the `rewrites` nextest profile is off. Only ad hoc
  `cargo run -- translate` / `translate-project` + manual `rustc`/`cargo build`
  runs were used to verify this session's work — there is no automated way to
  re-run these checks right now.

## New sibling crate: `slate-intrinic-gen`

Wait — correct spelling: **`/home/takashi/Projects/slate-intrinsic-gen`**,
a sibling of `slate` (i.e. `/home/takashi/Projects/slate-intrinsic-gen`, NOT
inside the `slate` repo). Deliberately kept separate per user instruction, and
distinct from the older, unrelated `/home/takashi/Projects/intrinsicgen` crate
(vendor-XML/JSON-based, used only for generating C headers — it has no LLVM
intrinsic name data and was explicitly ruled out as the source for this work,
see "Why not intrinsicgen" below).

**Not a git repo yet** — nobody has run `git init` in it. That's a decision
for the user, not made this session.

Layout:
```
slate-intrinsic-gen/
  Cargo.toml            # binary crate, deps: serde, serde_json
  cpp/extract_intrinsics.cpp   # the actual extraction logic, in C++
  src/main.rs            # shells out to llvm-config + a compiler, builds and
                          # runs the C++ extractor, renders the Rust table
```

### What it does and why it's C++

Confirmed early this session (with the user pushing back on doing this via
`tblgen-rs`/raw TableGen parsing): LLVM's `.td` intrinsic definitions
(`IntrinsicsX86.td` etc.) use a DAG-based overload-resolution scheme
(`LLVMMatchType`, `llvm_anyvector_ty`, `ImmArg<ArgIndex<N>>`) that only LLVM's
own C++ `IntrinsicEmitter`/`CodeGenIntrinsics` backend correctly interprets.
Reimplementing that resolution via generic TableGen record-walking (the
`tblgen-rs` approach used successfully for `clang-ir-types-gen` against CIR's
own, much simpler, flat `.td` dialect) would mean re-deriving real, gnarly
compiler logic. Decision: **shell out to a small C++ program linked against
the user's real LLVM build**, and call LLVM's own already-correct public API
functions as black boxes instead of re-parsing `.td` text. This is robust
against LLVM version drift in exactly the areas that matter (type resolution,
mangling) since it asks LLVM itself, not a hand-copied algorithm.

`cpp/extract_intrinsics.cpp` links against `LLVMCore`+`LLVMSupport` (found via
`llvm-config --cxxflags --ldflags --libs core support --system-libs`) and, for
a given target prefix (`x86`/`aarch64`/`arm`/`riscv`), walks every
`Intrinsic::ID` whose `Intrinsic::getName(id)` starts with `llvm.<prefix>.`.
For each:

- `Intrinsic::isOverloaded(id)` — is this a polymorphic/"any"-typed intrinsic?
- If **not** overloaded: `Intrinsic::getType(ctx, id, {})` gives the concrete
  `FunctionType` directly (empty overload-types list is valid since there's
  nothing to resolve), and `Intrinsic::getAttributes(ctx, id, FT)` gives real
  parameter attributes — specifically `Attribute::ImmArg`, which marks
  arguments that must remain LLVM-level compile-time constants (this is
  **not** derivable from CIR alone; it's the one fact from the `.td` that's
  genuinely load-bearing and not already implied by the concrete call-site
  types CIR provides). Emits `{"name", "overloaded": false, "ret": "<llvm type
  string>", "params": [{"type", "immarg"}]}`.
- If **overloaded**: emits `{"overloaded": true, "ret": null, "params": null}`
  plus (added mid-session, see below) `"overloaded_positions"`.

Run against the user's real LLVM build (`~/llvm-project/build`, commit
`84542052ebd4`, a bleeding-edge/dev tree, LLVM version string `24.0.0git` —
**this version mismatch matters**, see the "AVX512 legacy naming" finding
below), coverage was:

| prefix   | total | overloaded | resolved (non-overloaded + resolved-overloaded) |
|----------|-------|------------|---------------------------------------------------|
| x86      | 1703  | 17         | 100% (17/17 overloaded resolved)                   |
| arm      | 504   | 318        | 100% (318/318)                                     |
| riscv    | 836   | 766        | 100% (766/766)                                     |
| aarch64  | 1727  | 1420       | 88% (1250/1420 resolved; 170 unresolved, all SVE)   |

The 170 unresolved AArch64 entries are all `llvm.aarch64.sve.*` (Scalable
Vector Extension — narrowing/widening ops using `OneNthEltsVec`/`Subdivide2`/
`Subdivide4` IIT descriptor kinds the walker doesn't handle, see below). SVE
needs explicit ACLE usage (`svfloat32_t` etc.) in source C to even reach —
low priority. The walker **fails closed** (marks `overloaded_positions: null`,
same as "not yet supported") rather than guessing, so this is an honest gap,
not a silent wrong answer.

### Overloaded intrinsics: `overloaded_positions` (added mid-session)

Real overloaded intrinsics (e.g. `int_x86_avx512_mask_expand` — declared once
in `.td` as `[llvm_anyvector_ty]` return, `[LLVMMatchType<0>, LLVMMatchType<0>,
LLVMScalarOrSameVectorWidth<0, llvm_i1_ty>]` params) need to know, ahead of
time, **which argument/return positions are free type variables** — not to
resolve concrete types (CIR already gives us those at each call site) but to
compute LLVM's required mangled symbol suffix (see next section). This is
exposed via LLVM's `Intrinsic::getIntrinsicInfoTableEntries(id, table)`
(`llvm/include/llvm/IR/Intrinsics.h`), which returns a flat, pre-decoded
`ArrayRef<IITDescriptor>` — the same decoded form LLVM's own
`IntrinsicEmitter` walks, so no TableGen DAG logic needs reimplementing here
either.

Verified empirically (see `iit_probe.cpp`, not checked in, was a scratch
probe) that this descriptor list is a linear walk: position 0 = return type,
positions 1..N = params in order, and each position consumes either 1 raw
descriptor slot (`Overloaded`, `Match`, `Integer`, `Pointer` [opaque/modern
LLVM — no pointee sub-descriptor], scalar kinds, etc.) or 1 + a recursive
sub-descriptor (`Vector`, `SameVecWidth` — element type follows; `Struct` —
N member types follow). `Kind == Overloaded` marks a position as a genuinely
free type variable and carries its own `getOverloadIndex()`; `Kind == Match`
marks a position tied to an already-defined free variable (doesn't create a
new one). Example, confirmed against `mask.expand`:

```
[0] Overloaded overloadIndex=0     <- return, FREE
[1] Match overloadIndex=0          <- arg0, tied to return
[2] Match overloadIndex=0          <- arg1, tied to return
[3] SameVecWidth  [4] Integer bits=1   <- arg2 (mask), 2 slots, NOT free
```

→ `overloaded_positions: [0]` (only the return type varies; both data args
and the mask are pinned to it).

`consumeOne()`/`overloadedPositions()` in `extract_intrinsics.cpp` implement
this walk. Unhandled `Kind`s (the SVE ones: `OneNthEltsVec`, `Subdivide2`,
`Subdivide4`, and a few others never observed in practice) hit the `default:`
branch and return `{0, nullopt}`, which propagates up as "give up on this
intrinsic" (`overloadedPositions` returns `std::nullopt`, so `params`/
`overloaded_positions` come out `null` in the JSON — matches the pre-existing
"not yet supported" convention).

### Generated table shape

`src/main.rs` in `slate-intrinsic-gen` shells out to a C++/C compiler (`$CXX`
or `c++`) to build the extractor against `--llvm-build <dir>` (needs
`<dir>/bin/llvm-config`), runs it once per `--prefix`, and renders a single,
dependency-free `.rs` file:

```rust
pub struct IntrinsicParam { pub llvm_type: &'static str, pub immarg: bool }
pub struct IntrinsicSignature {
    pub name: &'static str,                              // "llvm.x86.sse42.crc32.32.8"
    pub overloaded: bool,
    pub ret: Option<&'static str>,                        // None for overloaded
    pub params: Option<&'static [IntrinsicParam]>,        // None for overloaded
    pub overloaded_positions: Option<&'static [u32]>,     // Some(&[0]) etc; None if unresolved
}
pub static X86_INTRINSICS: &[IntrinsicSignature] = &[ ... ];
pub static AARCH64_INTRINSICS: &[IntrinsicSignature] = &[ ... ];
pub static ARM_INTRINSICS: &[IntrinsicSignature] = &[ ... ];
pub static RISCV_INTRINSICS: &[IntrinsicSignature] = &[ ... ];
```

Regenerate command actually used this session (adjust paths for whoever's
running it):

```bash
cd /home/takashi/Projects/slate-intrinsic-gen
cargo build --release
./target/release/slate-intrinsic-gen \
  --llvm-build ~/llvm-project/build \
  --llvm-src ~/llvm-project \
  --prefix x86 --prefix aarch64 --prefix arm --prefix riscv \
  --out /home/takashi/Projects/slate/src/frontend/lowerer/intrinsics_table.rs
```

Confirmed this generated file compiles standalone (`rustc --crate-type lib`)
with zero external deps, and is wired into slate via `mod intrinsics_table;`
in `src/frontend/lowerer.rs` (uncommitted change, see diff below).

## What's wired into slate right now

### `Op::CallLlvmIntrinsic` dispatch

`src/frontend/lowerer.rs`, in the big `match op { ... }` inside `fn
lower_op`, added:
```rust
Op::CallLlvmIntrinsic(value) => return self.lower_call_llvm_intrinsic(&value),
```
(This variant — `clang_ir_types::ops::calls::CallLlvmIntrinsic { result:
Option<ValueId>, result_ty: Option<Type>, intrinsic_name: String, arg_ops:
Vec<ValueId>, loc }` where `ValueId = String` — already existed in the
generated `clang-ir-types` crate; slate just never had a match arm for it, so
it silently fell through to `emit_todo("instruction without lowering")`.)

### `lower_call_llvm_intrinsic` (`src/frontend/lowerer/intrinsics.rs`)

Core design decision, confirmed by direct testing rather than assumed: CIR's
`call_llvm_intrinsic` op **already carries fully concrete, resolved operand
and result types** at every call site (confirmed via `cargo run -- emit-cir`,
e.g. `"cir.call_llvm_intrinsic"(%69, %70) <{intrinsic_name =
"x86.sse42.crc32.32.8"}> : (!u32i, !u8i) -> !u32i`), **even for LLVM-level
overloaded intrinsics** (confirmed for `llvm.x86.avx512.mask.expand`: CIR
gives the bare/unmangled `intrinsic_name` plus the real resolved
`!cir.vector<16 x !s32i>` types at that call site). So slate never needs to
resolve LLVM's type-DAG overloading itself for *codegen* purposes — only for
computing the *link name* (see mangling section).

Approach: for each `call_llvm_intrinsic` op, declare a local, per-signature
`extern "unadjusted" { #[link_name = "llvm.<name>[.<mangled suffix>]"] fn
__slate_intrinsic_<sanitized>_<sig-hash>(...) -> ...; }` (this mirrors exactly
what `core::arch::x86_64`/stdarch does internally — confirmed by reading
`~/.rustup/toolchains/nightly-.../lib/rustlib/src/rust/library/stdarch/
crates/core_arch/src/x86/sse42.rs` etc., which is present locally and was
used repeatedly this session as a design oracle) and call it directly. This
requires nightly `#![feature(link_llvm_intrinsics, abi_unadjusted)]`,
consistent with slate already emitting other nightly `#![feature(...)]`
attrs.

Dedup key for the shim: `format!("{sanitized}__{ret_type:?}__{param_types:?}")`
hashed into the generated fn name, so distinct concrete signatures of the same
`intrinsic_name` (i.e. different overload instantiations) get **distinct**
shim declarations automatically — no special-casing needed, this falls out of
using the CIR-resolved types directly.

New `Lowerer` fields (in `src/frontend/lowerer.rs`, part of the committed
`6a8b252f`): `llvm_intrinsic_shims: BTreeMap<String, ExternFnDecl>`, flushed
into `Item::ExternBlock { abi: "unadjusted".into(), decls: ... }` at program
end, mirroring the pre-existing `long_double_shims` pattern exactly.

Void-return handling: CIR still binds a result id even for `void`-returning
intrinsics (e.g. `_mm_pause`/`__builtin_ia32_lfence`), so the code checks the
*Rust* return type is `Some` (i.e. not `CLibType::VOID`) before calling
`materialize_expr`, else just pushes an expression statement. See the
`match &op.result { Some(result) if ret_type.is_some() => ..., _ => ... }`
block.

### `ExternFnDecl` gained an `attrs: Vec<Attr>` field

Needed for `#[link_name = "..."]` — the struct previously had no way to
rename the linked symbol away from the Rust fn name (fine for ordinary C
externs, useless for `llvm.*` symbols which aren't valid Rust identifiers —
dots). This is a **repo-wide, 8-call-site change** (part of committed
`6a8b252f`): `src/backend/rust_ast.rs` (struct def), `src/backend/codegen.rs`
(prints `#[...]` before `fn` in `ExternDecl::Fn` — mirrors how
`ExternDecl::Static`'s attrs were already printed), and 7 construction sites
across `src/frontend/lowerer.rs`, `src/frontend/lowerer/calls.rs`,
`src/frontend/lowerer/runtime_support.rs` (x2), and
`src/backend/query/program_recipe.rs` all got `attrs: Vec::new()`.

## Pre-existing bugs found and fixed this session

All three were found by trying to get a *real, running* differential test to
pass for the AVX512 `mask.expand` overload case (not just eyeballing generated
Rust) — each one blocked compilation/execution and was root-caused before
being fixed, all landed in the already-committed `6a8b252f`.

### 1. `BInt<1,1,1>` byte-padding vs. bit-packing (`memory.rs`)

CIR represents `<N x i1>` masks (e.g. a `__mmask16` bitcast to a boolean
vector, which is what the `_mm512_mask_expand_epi32` header wrapper's body
does internally) as `[bitint::BInt<1,1,1>; N]` — an array of N **byte**-sized
1-bit ints (`BInt<BITS,LIMBS,BYTES>` stores `bytes: [u8; BYTES]`, and
`bitint_storage_bytes(1) == 1`, so each element is a whole byte, same as
Rust's own `bool`). The pre-existing generic `lower_cast` fallback in
`src/frontend/lowerer/memory.rs` did a blind
`Expr::Transmute { from: u16, to: [BInt<1,1,1>;16], ... }` for **any**
scalar↔array cast mismatch, with no size check — `u16` is 2 bytes, `[BInt<1,
1,1>;16]` is 16 bytes, so this was always an invalid, panicking-at-runtime (or
rather compile-error, `E0512`) transmute whenever a real program actually hit
this path (nothing did until this session's intrinsics work started calling
these wrapper functions).

Fix: added `bitint_vector_lane_bits`, `packed_mask_int_type`,
`pack_bitint_vector_expr`, `unpack_bitint_vector_expr` to `memory.rs`
(deliberately generalized to any lane bit-width via `lane_bits`, not
hardcoded to 1 — the user specifically asked "is this only for i1?" and the
fix was widened in response, even though in practice only i1 masks occur for
real x86/ARM/RISC-V signatures; wider narrow-bitint vectors don't occur
because non-mask vector elements are always byte-multiple widths and bypass
`bitint_type` entirely via `scalar_int_type`). `lower_cast`'s final fallback
now special-cases "one side is `[BInt<K,..>; N]`, the other is a plain
`u8/u16/u32/u64/u128` of exactly `K*N` bits" and emits an explicit bit-by-bit
pack/unpack `Expr` tree (fully unrolled at lowering time, `N` is always a
small compile-time constant) instead of the invalid transmute.

**Note: this fix is now believed to be solving the wrong layer for the
specific case of masks passed to `call_llvm_intrinsic`** — see "Open
problem" section below. It's still correct and needed for the *general*
CIR bitcast case (a `<N x i1>`-typed SSA value used for something other than
directly feeding an intrinsic call), just not sufficient by itself to make
`call_llvm_intrinsic`'s own mask arguments work end-to-end.

### 2. GNU vector-typed struct fields mis-typed as scalars (`lowerer.rs`)

The `__may_alias__` packed-struct trick clang's headers use for unaligned
stores (`_mm512_storeu_si512`/`_mm512_storeu_pd`):
```c
struct __storeu_si512 { __m512i __v; } __attribute__((__packed__, __may_alias__));
```
was being lowered with `__v: i64` (should be `[i64; 8]`) for the `si512`
case and `__v: i32` (should be `[f64; 8]`!) for the `pd` case — both wrong
type AND wrong width. Root cause: the Clang-AST-fact layer (`CType` in
`src/frontend/c_ast.rs`) has **no representation for GNU vector types at
all** (`__attribute__((vector_size(N)))`), and `parse_c_type`'s only
fallback for an unrecognized type string is a crude substring-based
scalar-int guess (`int_bits`: checks for `"char"`/`"short"`/`"__int128"`/
`"long"` substrings, defaults to 32). `__m512i` desugars to
`"__attribute__((__vector_size__(8 * sizeof(long long)))) long long"` —
contains `"long"` → guessed 64-bit int. `__m512d` desugars similarly with
`"double"` — contains none of those substrings → defaulted to 32-bit int.
Both numbers are coincidences of the substring match, not the real type.

**First attempted fix was wrong and was reverted** (worth noting so nobody
re-tries it): initially wrote a `parse_vector_size_type` string-parser for
`c_ast.rs` to properly decode the `__attribute__((vector_size(N)))` spelling.
The user asked "doesn't CIR already provide this?" — checked, and yes: CIR's
own record definition for `__storeu_si512` is
`!cir.struct<"__storeu_si512" {data !cir.vector<8 x !s64i>}>` — completely
correct, real vector type, no parsing needed. The actual bug was that
`lower_record_def` (`src/frontend/lowerer.rs`) **already receives** correct
CIR-derived field types (`cir_field_types: Option<&[Type]>` parameter) but
was deliberately restricted to only trust them for `CType::FuncPtr` fields:
```rust
.filter(|_| matches!(field.ty, CType::FuncPtr { .. }))
```
Real fix (in `6a8b252f`, superseding the reverted c_ast.rs approach): widen
that filter to also trust the CIR type whenever it says "array" and the
AST-derived guess didn't already agree:
```rust
let trust_cir = matches!(field.ty, CType::FuncPtr { .. })
    || (matches!(cir_ty, Some(Type::Array { .. })) && !matches!(field.ty, CType::Array(..)));
```
This only mattered for `lower_record_def`'s **direct** callers (which already
had `self.cir_record_field_types(record)` available via a `Lowerer` method).
The `translate-project` shared-types path (`lower_shared_types`, used for
records shared across translation units, written to a standalone `types.rs`)
is a **free function with no CIR module access at all** — it unconditionally
passed `None` for `cir_field_types`. Fixed by: extracting the CIR
struct-field lookup into a new free function `cir_record_field_types_from_
aliases(record, aliases: &BTreeMap<String, CirType>, va_list_boxed)` (the
original `Lowerer::cir_record_field_types` method now just delegates to it),
changing `lower_shared_types`'s signature to take
`aliases: &BTreeMap<String, CirType>`, and — the actually-invasive part —
threading a **merged** aliases map (accumulated from every translation
unit's `module.generic.type_aliases` while iterating `loaded_modules`/
`variants`/etc.) through `src/main.rs` at all **4** call sites of
`lower_shared_types` (each of the 4 `translate-project*` command variants has
its own near-duplicate loop structure; each needed its own
`let mut merged_aliases: BTreeMap<String, cir::CirType> = BTreeMap::new();`
declared before the loop and `merged_aliases.extend(<module>.generic.
type_aliases.clone());` inside it). All 4 sites were located and fixed via
`grep -n "lower_shared_types"` — if this pattern needs touching again, that's
still the right way to enumerate all call sites.

Verified: regenerated `/tmp/overload_crate` (a `translate-project` output for
a probe fixture, not checked in — see "How to reproduce" below) and confirmed
`types.rs` now has `pub __v: [i64; 8]` / `pub __v: [f64; 8]`.

### 3. Overloaded intrinsics need a mangled `#[link_name]`, not the bare CIR name

This is the fix represented by the **currently uncommitted** diff in
`intrinsics.rs`/`lowerer.rs` (shown in full above, under "Repo state"). CIR's
`intrinsic_name` for an overloaded intrinsic is the unmangled family name
(`x86.avx512.mask.expand`, confirmed via `emit-cir` — no `.512`/`.d`/`.v16i32`
suffix at all). Declaring two different Rust extern fns with that identical
bare `#[link_name]` (one shim per concrete overload, per the dedup-by-
signature design above) works fine for **one** overload per compilation unit,
but as soon as **two** overloads of the same intrinsic are used together,
rustc's LLVM codegen backend — which declares the LLVM global under the
*literal* link_name string, doesn't auto-mangle — hits a real ICE:
```
Cannot match `[8 x double]` (expected) with [16 x i32] (found) in
declare [8 x double] @llvm.x86.avx512.mask.expand([8 x double], [8 x double], [8 x { [1 x i8] }]) unnamed_addr
```
because both shims declared the exact same LLVM global name with conflicting
types.

Fix added: `mangled_link_name(intrinsic_name, ret_type, param_types) ->
String` in `intrinsics.rs`, consulting the new `overloaded_positions` table
field — for each free position, mangle the concrete CIR-resolved type
(`mangle_llvm_type`: `Type::Prim(..)` → `iN`/`fN`, `Type::Array{elem,len}` →
`v{len}{mangle(elem)}`, `Type::Ptr` → `p0`, bitint → `iN`) and append
`.{mangled}` per free position, joined, matching LLVM's own
`Intrinsic::getName(ID, Tys)` mangling scheme exactly (verified directly: a
standalone C++ probe calling `Intrinsic::getName(id, {i32v})` /
`{f64v}` for `mask.expand` produced exactly `llvm.x86.avx512.mask.expand.
v16i32` / `.v8f64`, matching what `mangled_link_name` now computes). Falls
back to the bare name (previous, pre-existing behavior) if the intrinsic
isn't in the table, isn't overloaded, or a needed position can't be mangled —
purely additive, doesn't affect the already-working non-overloaded path at
all (verified: re-ran the crc32/pause/rdtsc fixture after this change,
`llvm.x86.rdtsc` etc. link names unchanged).

**This computed name is correct per LLVM's own canonicalization — and still
not sufficient to make AVX512 `mask.expand` actually compile+run.** See next
section; this is the open problem the user wants picked up next.

## Open problem: computed "canonical" mangled names don't always match what the X86 backend can actually select

This was diagnosed via a series of minimal standalone repros (none checked
in — all were `/tmp/*.rs`, rebuild from the snippets below if needed to
re-verify against a different LLVM/rustc pairing) run directly with `rustc`
(this environment's `rustc` is nightly, `1.99.0-nightly (14cae6813
2026-07-08)`, and is a **different build** from the `~/llvm-project` checkout
used for `SLATE_CLANG`/`slate-intrinsic-gen` — this version skew is almost
certainly related to what was found):

1. Declaring the shim with `[i32; 16]` (plain Rust array) for the `__m512i`
   data arguments — which is what slate's *general* `CirType::Vector →
   Type::Array` mapping produces everywhere else in the codebase, used
   unchanged by the new intrinsics code too — fails differently depending on
   whether it's the array-shape problem or the naming problem being tested;
   see below, both were live bugs simultaneously and had to be separated.

2. **Root architectural gap, independent of naming**: plain Rust arrays
   (`[T; N]`) lower to LLVM **array** types, not LLVM **vector** types.
   `<N x T>` (a real LLVM vector, what every SIMD intrinsic's signature
   actually declares) is a different type constructor entirely; nothing
   coerces one into the other at an extern/FFI call boundary. This affects
   **every** vector-typed CIR value slate produces, not just intrinsic
   arguments — confirmed the *outer* C-level `__m512i` type itself gets
   represented as `[i64; 8]` throughout slate's existing output, e.g. real
   `expand_i32(arg12: [i64; 8], ...)` wrapper functions in already-generated
   code. This has presumably never mattered before because nothing
   previously round-tripped a slate-generated array-shaped SIMD value through
   a real `extern "unadjusted"`/`#[link_name="llvm.*"]` call boundary — the
   only intrinsics slate could lower before this session were scalar-only
   (crc32/rdtsc/etc., confirmed still working after all of this session's
   changes).

3. `std::simd::Simd<T, N>` (portable_simd, nightly, needs
   `#![feature(portable_simd, simd_ffi)]` in addition to the two features
   already used) is confirmed to be the right fix for (2): it's a real
   `#[repr(simd)]` type that does lower to a genuine LLVM vector. Verified
   working standalone for: a **non-x86, fully generic** overloaded intrinsic
   (`llvm.ctpop.v4i32` — computed/"canonical" mangled name, `Simd<i32,4>`,
   worked first try, correct output); and AVX512 **without a mask argument**
   (`llvm.x86.avx512.pmul.dq.512`, `Simd<i64,8>` args, worked, correct
   output). So `Simd<T,N>` is not itself the blocker.

4. A **hand-rolled `#[repr(simd)] struct i32x16([i32; 16]);`** (mimicking
   stdarch's own internal simd newtype, textually copy-pasted from stdarch's
   actual source for this exact intrinsic, same link_name, same everything)
   triggered a **different rustc ICE** (`rustc_mir_transform::validate`
   panic, i.e. crashed during MIR validation, before even reaching LLVM
   codegen) — this specific repr(simd)-newtype-over-array shape appears to
   only really work when compiled as part of `core`/`std` itself (bootstrap-
   stage privileges), not reproducible verbatim in ordinary user-crate code
   on this nightly. **Do not pursue hand-rolled `#[repr(simd)]` newtypes as
   a path forward** — dead end, confirmed by direct repro, not guessed.

5. **The actual, isolated root cause**: with `Simd<T,N>` (not the repr(simd)
   newtype) for the data args and a **plain scalar `u16`** for the mask
   (exactly matching stdarch's own declaration —
   `fn vpexpandd(a: i32x16, src: i32x16, mask: u16) -> i32x16;` — rustc's
   `#[link_name="llvm.*"]` codegen path implicitly bitcasts a scalar `iN`
   argument into `<N x i1>` when the real intrinsic signature calls for it;
   this is a real, working, intentional rustc feature, not a coincidence —
   confirmed `std::simd::Mask<T,N>` does **not** help and hits the exact same
   failure as the scalar), calling with the **computed "canonical" mangled
   name** (`llvm.x86.avx512.mask.expand.v16i32`) produces:
   ```
   rustc-LLVM ERROR: Cannot select: intrinsic %llvm.x86.avx512.mask.expand
   ```
   an LLVM `Cannot select` — an **instruction-selection** failure, i.e. LLVM
   accepted the module (verifier passed, so the name/types are *structurally*
   valid) but the X86 backend has no ISel pattern that lowers this
   particular declared intrinsic call to real instructions.

   Swapping **only** the link_name to stdarch's own legacy form —
   `llvm.x86.avx512.mask.expand.d.512` (element-type-code + total-bit-width,
   the pre-unification naming scheme) — with everything else identical
   (`Simd<i32,16>` args, scalar `u16` mask) **compiled and ran correctly**,
   producing semantically correct output (`[1, 0, 2, 0, 0, ...]` for mask
   `0b101` over `a=[1..16]`).

   **Conclusion**: for this specific AVX512 mask-intrinsic family (and
   plausibly others sharing the same historical naming split — not yet
   surveyed), LLVM's `Intrinsic::getName()`/canonical mangling and the X86
   backend's actual instruction-selection patterns have **diverged** — the
   verifier/parser accept the modern canonical name, but only the legacy
   name has ISel support wired up. This is not something `getIntrinsicInfoTableEntries`
   or any other "ask LLVM" API this session tried can detect ahead of time;
   it's a backend implementation gap, not a naming/type-system fact.

## Next step the user wants (not started)

User's explicit direction, verbatim intent: **prefer calling
`core::arch::<target>::` functions directly by name** where a matching
public wrapper exists (more idiomatic, and — as finding 5 above proves —
provably correct, since stdarch's own declarations are what's actually
tested against real ISel). **Fall back** to the raw
`extern "unadjusted"`/mangled-`#[link_name]` mechanism built this session
for anything `core::arch` doesn't wrap. And since some of `core::arch`'s own
naming/linking choices are "weird" (confirmed: the AVX512 legacy-suffix case
above, and there will be others), **build an override layer** — a table
mapping `(intrinsic_name, concrete signature)` → either a `core::arch`
function path to call, or an explicit legacy `#[link_name]` string to use
instead of the computed canonical one.

Concretely, this likely means:

1. **Mine stdarch's `#[link_name = "llvm...."]` attributes** (the actual
   source of truth for "what name and what Rust type shape actually works"),
   pairing each with its enclosing public wrapper function name and its
   parameter/return Rust types. Source is available locally and was used
   as a design oracle throughout this session:
   `~/.rustup/toolchains/nightly-x86_64-unknown-linux-gnu/lib/rustlib/src/
   rust/library/stdarch/crates/core_arch/src/{x86,x86_64,arm,aarch64,
   riscv64}/**/*.rs`. This gives, per LLVM intrinsic name (keyed off the
   *actual working* link_name string, which may be the legacy form): the
   public Rust function name, its exact parameter/return types (already
   correctly `Simd<T,N>`/scalar as needed — no guessing), and its required
   `#[target_feature]`.
2. Decide where this mined data lives: most likely a second generated table
   in `slate-intrinsic-gen` (or folded into the existing one) keyed by CIR's
   `intrinsic_name` + a way to disambiguate by concrete signature when one
   CIR name maps to multiple stdarch functions (e.g. `_mm512_mask_expand_
   epi32` vs `_mm256_mask_expand_epi32` — different C intrinsics, quite
   possibly the *same* CIR `intrinsic_name` with different concrete
   `!cir.vector` widths at the call site, exactly like the `overloaded_
   positions` case already handled).
3. In `lower_call_llvm_intrinsic`: look up the stdarch mapping first by
   `(intrinsic_name, concrete signature)`; if found, emit a direct call to
   the `core::arch::<target>::<fn>` path (no local shim/extern block needed
   at all — simpler generated output, and provably correct since it's
   calling code that's already compiled and tested as part of `std`). If not
   found, fall back to today's `mangled_link_name` + local shim mechanism —
   which remains correct for cases stdarch doesn't cover, and is now known
   to be reliable for anything that isn't hitting a legacy-vs-canonical
   naming split like AVX512 masks (i.e. was proven correct for crc32,
   pause/rdtsc/fences, and for the non-AVX512-mask overloaded case
   `llvm.ctpop.v4i32`).
4. The override layer for "weird" cases (AVX512 legacy suffixes being the
   first known instance) is subsumed by (1)/(2) if the stdarch-mining
   approach is used as the *primary* path rather than a secondary fallback
   table — stdarch's `#[link_name]` already **is** the override table, for
   every intrinsic stdarch bothers to wrap. A separate override table would
   only be needed for intrinsics with no `core::arch` wrapper at all AND a
   legacy-vs-canonical naming split — narrower scope, lower priority, revisit
   only if it turns out to matter in practice.

## How to reproduce / pick this back up

```bash
cd /home/takashi/Projects/slate
cargo build --release   # picks up uncommitted mangling diff on top of 6a8b252f
```

Non-overloaded, fully-working smoke test (crc32/pause/rdtsc/fences):
```bash
mkdir -p /tmp/crc32_src && cp tests/fixtures/x86_instruction_intrinsics.c /tmp/crc32_src/main.c
cargo run --release -- translate-project /tmp/crc32_src /tmp/crc32_crate
# add #![feature(link_llvm_intrinsics, abi_unadjusted)] to line 1 of generated main.rs (translate/translate-project
# don't auto-add these features yet -- another loose end worth fixing once the design settles)
# -C target-feature=+sse4.2 needed via .cargo/config.toml [build] rustflags, or -C flag directly
```
Compare against `clang -msse4.2 tests/fixtures/x86_instruction_intrinsics.c`
— confirmed byte-identical stdout and exit code this session.

Overloaded/broken smoke test (AVX512 `mask.expand`, both `_epi32` and `_pd`
overloads together) — fixture used this session, not checked in, recreate at
`/tmp/overload_src/main.c`:
```c
#include <immintrin.h>
#include <stdio.h>
__attribute__((target("avx512f")))
__m512i expand_i32(__m512i src, __mmask16 k, __m512i a) { return _mm512_mask_expand_epi32(src, k, a); }
__attribute__((target("avx512f")))
__m512d expand_f64(__m512d src, __mmask8 k, __m512d a) { return _mm512_mask_expand_pd(src, k, a); }
int main(void) { /* ... exercise both, printf results ... */ return 0; }
```
```bash
cargo run --release -- translate-project /tmp/overload_src /tmp/overload_crate
# check /tmp/overload_crate/src/main.rs and src/types.rs
```
This currently produces correct extern declarations (mangled names, `[i32;
16]`/`[f64;8]` array types, correct `__storeu_si512`/`__storeu_pd` struct
field types) but will fail at LLVM ISel for the reasons in "Open problem"
above until the `core::arch` direct-call approach lands.

`slate-intrinsic-gen` regeneration command is under "Generated table shape"
above. Note `~/llvm-project/build` (used for the extractor) and whatever
`SLATE_CLANG`/`SLATE_CIR_OPT` point at for slate's own C→CIR step should
ideally be the *same* LLVM build/commit to avoid version-skew surprises
(unverified whether they currently are — worth checking).
