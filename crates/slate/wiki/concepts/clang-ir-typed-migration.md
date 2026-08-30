# clang-ir typed-CIR migration

Tracked by epic `slate-cevu` (phases 1–3 below) and completed by the
follow-on `slate-jedr`/`slate-3hkk` epics (see "Completion" below). Moved
slate's CIR handling off generic, raw-text `Operation`/`Attribute` matching
and onto the `clang-ir` crate's typed AST (`ast::{Attribute, Type}`) and typed
instruction model (`model::Instruction`/`model::Op`). As of `slate-jedr.5`/
`slate-3hkk` landing, the migration is **done**: the frontend/lowerer no
longer has any generic escape hatch at all, and a build-time gate enforces
that going forward.

## Phase 1 — typed `Attribute`/`Type` layer

Slate imports clang-ir's typed AST and model directly. `Attribute`, `Type`,
`model::Module`, and generated operations have no Slate compatibility wrapper
or separate representation needing reconciliation.

`lower_const` (storage.rs) now pulls `op.attr("value")`, resolves `#name`
aliases via `resolve_attr`, and matches structurally on
`Attr::{CirInt,CirFloat,CirBool,ConstVector,ConstComplex,GlobalView,
Dialect{...}}` instead of prefix/suffix-matching `#cir.xxx<...>` text. This
deleted ~20 raw-text `parse_cir_*`/`cir_*_text` helpers in `constants.rs`
outright once `lower_const` stopped calling them (verified zero remaining
callers via whole-repo `rg`, not just the lowerer tree, before each
deletion).

**Migration debugging methodology** (repeatable for any typed-migration bug):

1. `cargo run --release -- translate-lowered <fixture>.c` — raw lowering, no
   fixups, fastest way to see the bug.
2. `cargo run --release -- emit-cir <fixture>.c` — ground-truth CIR text.
3. Grep the `clang-ir` crate itself (`~/Projects/clang-ir/src/{parser,model,
ast}/*.rs`) for how the attribute/type is actually represented. Don't guess
   from the shape of the old raw-text code.
4. Fix, rebuild, re-diff, re-run `cargo nextest r --release --profile
lowering` (never plain `cargo test`).

Six real, previously-latent bugs surfaced this way once the lowerer compiled
again after months of not running at all — a warning that "compiles clean"
and "correct" are unrelated claims for CIR-attribute code:

- **`attr_symbol_ref` read `@name` refs as `attr_str`.** `@name` parses to
  `Attribute::SymbolRef(String)`, not `Attribute::Str` — `attr_str` silently
  returned `None`. Broke direct calls (fell through to the indirect-call
  path), weak-alias resolution, and global variable references.
- **`void` function definitions got `-> core::ffi::c_void`** instead of `()`
  — the definition path (unlike the declaration path) didn't filter the void
  sentinel out of the return type.
- **clang-ir crate: struct-type parser missing the `padded` keyword.** Real
  CIR struct grammar is `packed padded incomplete { members }`, each keyword
  optional in that order; the Rust parser only recognized `incomplete`/
  `packed`. Any struct printed with `padded` (most bitfield-containing
  structs) silently parsed as **zero members**. Fixed in the `clang-ir` crate
  (separate repo, local path dependency — changes apply immediately, no
  version bump, but commit it separately).
- **Anonymous-struct record name derived from the mangled alias key without
  expanding the alias first**, in two `memory.rs` call sites newly rewritten
  this migration (the old raw-text version expanded-then-extracted; the
  rewrite "simplified away" that step without realizing it was load-bearing).
  For named structs the alias-key strip and the literal name coincide; for
  anonymous ones they don't (`"anon2E0"` vs the real `"anon.0"`).
- **Bitfield shift expression used the storage field's raw type, not the
  logical field's type** — needs an explicit same-width `as` cast before the
  shift whenever storage and logical signedness differ.
- **`bitfield_info` attribute alias not resolved before matching** — CIR
  hoists repeated `bitfield_info` literals behind a `#name` alias exactly like
  it does for `#true`/`#false`; unresolved, every multi-field bitfield storage
  group fell back to the raw-shift path, silently corrupting sibling fields on
  write. Fixed by threading `&Module` into `collect_bitfield_storages` so it
  can call `module.resolve_attr` before matching, mirroring the `Lowerer`-level
  fix already in place for post-construction code.

Also found: **x86-64 SysV ABI struct-coercion anonymous types collapse to
`i32`** (`record_struct_type` returns `Type::Prim(Prim::I32)` whenever
`s.name` is `None` — true for compiler-synthesized ABI-coercion struct types
with no source-level name at all, not just ordinary anonymous structs). The
naming convention to fix it already exists (`is_abi_coercion_record_name`,
`anon_local_records`) — the type-resolution fallback just needs to derive a
synthetic name from the enclosing alias key instead of defaulting to `i32`.

## Phase 2 — `CirOpKind` → `model::Instruction` dispatch

`lower_op` (`lowerer.rs`) dispatches through
`clang_ir::model::instruction::lower_op(op)` before falling back to the
legacy `match op.kind()`. Migrate one op family at a time:

1. Check `~/Projects/clang-ir/src/model/instruction.rs`'s `Instruction` enum
   - `try_lower` for which mnemonics collapse into a variant and what it
     extracts.
2. Add one match arm calling through to the existing `lower_x` helper —
   existing helpers almost always already read what they need from
   `op.operands`/`op.regions`/`op.attr(...)`; don't assume a rewrite is
   needed (control-flow ops with region fields work with the same one-arm
   pattern as everything else — an earlier session wrongly assumed `If`
   needed special handling).
3. Delete the `CirOpKind` variant/parse-arm/legacy-match-arm only after
   confirming zero remaining references **repo-wide**, not just in
   `lowerer/`.
4. Verify: `cargo build --release`, `cargo nextest r --release --profile
lowering` (+`rewrites` for high-traffic ops like Load/Store/Const),
   `cargo fmt`, `cargo clippy --release --all-targets`.

Blocked on crate-side modeling (re-check before re-deriving): `ComplexMul`/
`ComplexDiv`/`ComplexConj`, `SwitchFlat`. `DivOverflow`/`RemOverflow` deleted
outright as genuinely dead (no div/rem overflow builtin exists in C).

### Phase 2.5 — pass the whole `Instruction`, not just a discriminant

Early per-family migrations only used the typed `Instruction` to classify,
then called `lower_x(op: &Op)` which re-derives everything by hand —
including attrs the crate already decoded into enums (`CmpOpKind`,
`MemOrder`, `AtomicFetchKind`, ...). Convention going forward: helpers take
the matched `Instruction` by value, re-narrowed with `let Instruction::
TheVariant { fields, .. } = instr else { unreachable!() };` as the first
line, and swap any hand-rolled attr decode for the crate's typed field.
`unreachable!()` isn't compiler-checked, but every op family has fixture
coverage, so a dispatch/destructure mismatch fails loudly on the first
fixture that hits it — accepted, since differential fixture testing is
already the whole correctness model here, not a new risk category.

**Convert a family only once it has zero remaining `CirOpKind` references.**
A legacy fallback arm existing does _not_ by itself mean the family is unsafe
to convert — check whether the hand-written helper's own guards are provably
identical to `try_lower`'s preconditions. If they're looser and silently
produce a degenerate default on malformed input (old `Const`/`GetGlobal`:
missing attr → fabricated zero/empty-string), tighten the helper's guard to
match `try_lower` _first_, then convert — that's a deliberate behavior
tightening (runtime check → dispatch-validated precondition), not just a
refactor. `GetBitfield`/`SetBitfield` looked provably-dead by this check but
weren't — a third failure mode (missing-attr vs. present-but-unresolved-alias,
see the bitfield alias bug above) that guard-parity alone doesn't catch;
cross-check against real emitted CIR, not just a synthetic minimal fixture,
before trusting "provably dead."

Bare/zero-payload ops (`Break`, `Continue`, `Unreachable`, `Trap`) and
second-level dispatch functions (`lower_binary_family`, etc.) aren't worth
this conversion.

## Phase 3 (in progress) — fully self-sufficient `Instruction`

Remaining gap (`slate-cevu.6`): per-operand types and the untyped block/
region traversal engine still require `&Op` alongside the typed
`Instruction`. Goal is for `lower_*` helpers to eventually drop `op` entirely.
`clang-ir-types 0.1.4`'s generated `Op::for_each_result` and
`FunctionLowerer::value_types` (typed SSA-result lookup) are steps toward
this; runtime operand-type queries now go through the SSA lookup rather than
re-deriving from raw operand strings.

## cir-opt invocation ownership

`frontend::toolchain` invokes clang-ir's flag-capable normalization API once
per emission. `frontend::cir_input` parses that generic-form result directly;
it performs a second flattened emission only when goto detection or Clang's
dominance failure requires the selective CFG fallback.

## Completion (`slate-jedr`, `slate-3hkk`)

Phase 3's stated goal — `lower_*` helpers dropping `op` entirely — landed,
then went further: the frontend/lowerer no longer imports or traverses
generic `Operation` values or `Function`/`Global` raw escape hatches at all
(`slate-jedr.5`). Linkage, visibility, aliases, sections, lifecycle
priorities, `noreturn`, module assembly, call-operand types, and
`get_member` base types all now come from clang-ir's typed model/generated
ops. Generic operations remain **only** under `src/cir` normalization/
flattening, and generic type/attribute alias resolution remains structural
(not semantic) — those two are the deliberate, permanent boundary, not
leftover migration debt.

Follow-on work in the same pass:

- **`slate-jedr.6`**: reorganized the lowerer by typed pipeline phase —
  `module_index.rs` (TU indexing), `function_setup.rs` (per-function setup),
  `dispatch.rs` (the single typed traversal), `record_analysis.rs` (anonymous
  record discovery/reconciliation). `types.rs` now owns structural type
  lowering only; `lowerer.rs` is state + module/global/record emission
  coordination.
- **`slate-3hkk.1`**: `frontend::toolchain`'s clang-ir `Toolchain` accepts
  caller-supplied `cir-opt` flags while always adding generic-form/
  debug-location flags; slate's own duplicate `cir-opt` subprocess/error
  plumbing was deleted in favor of this API (see "cir-opt invocation
  ownership" above).
- **Typed module alias ownership**: `clang_ir::model::Module` gained
  normalized type/attribute alias tables and alias-chain resolution; Slate's
  semantic consumers no longer touch `Module::generic` for this.
- **Compact typed model ownership**: `Module::generic` and `Function`/
  `Global` raw operation clones were removed from clang-ir's default typed
  model entirely. Generic parsing stays explicit and separate for slate's
  selective flattening boundary; conversion preserves alias tables, typed
  bodies, and metadata, keeping only genuinely-unconverted top-level ops in
  `Module::other` (with test coverage proving typed functions/globals aren't
  duplicated there).
- **Frontend-owned Clang configuration**: slate's Clang/CIR tool paths,
  `SLATE_*` environment policy, target and libc-shim arguments,
  preprocessing queries, and CIR emission all moved under
  `frontend::toolchain`; Clang AST extraction shares the same path/target
  helpers instead of duplicating them. No slate policy moved into the
  `clang-ir` crate itself.
- **Removed the slate CIR facade**: selective goto/label detection,
  flattened-function merging, and dominance-error fallback moved to
  `frontend::cir_input`. `src/cir` and its facade exports were deleted
  outright; the public API and CLI import clang-ir AST/model types directly.
- **Fallible typed model conversion**: `clang-ir` typed `Module` conversion
  now returns structured errors for missing/invalid function/global
  attributes, an absent `builtin.module`, or `cir.*` ops that fail generated
  schema conversion, with recursive validation so malformed body ops can't
  silently become `Op::Other` — only genuinely-unknown non-CIR dialect
  operations do.
- **Build-time enforcement gate**: a regression gate over frontend lowering
  rejects generic `Operation`/`GenericModule` access, retained `.generic`
  trees, or mnemonic dispatch, so the boundary above can't silently regress.
  Slate's obsolete `clang-ir-types` patch was removed — `clang-ir` is now the
  sole, direct CIR dependency.
