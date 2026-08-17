# Handoff: clang-ir typed-CIR migration (slate-cevu.3, Phase 1)

## Where this fits

Beads ticket `slate-cevu.3` ("Phase 1: swap slate's generic CIR layer onto
clang-ir's typed ast layer"), child of epic `slate-cevu`. Still `in_progress`.
`slate-cevu.4` (Phase 2: migrate `CirOpKind` dispatch onto clang-ir's
`model::Instruction` layer) is blocked on `.3` and not started.

Prior sessions got the crate wired in and did a first pass of typed-migration
on `src/cir/ir.rs` / `src/frontend/lowerer.rs` / `src/frontend/lowerer/{analysis,constants,types}.rs`,
but left the lowerer submodules (`storage.rs`, `memory.rs`, `intrinsics.rs`,
`asm.rs`) not compiling (~166 `cargo build --release` errors) and left several
`parse_cir_*` raw-text-reimplements-CIR-grammar functions in `constants.rs`
still wired up instead of the typed `Attribute`/`Type` matching the ticket
calls for.

## What this session did

### 1. Got the whole workspace compiling again (0 errors)

Fixed the ~166 compile errors across `storage.rs`, `memory.rs`, `intrinsics.rs`,
`asm.rs`, `analysis.rs`, `op_utils.rs`, `bitfields.rs`. All mechanical, per the
patterns the prior session's ticket notes already established:

- `op.results.first()` now yields `Option<&(String, CirType)>` — destructure
  as `let Some((result, cir_ty)) = ... else { return };` instead of
  `let Some(result) = ...`.
- `op.ty` (the old flattened type-string field) is gone. Use
  `op_result_type(op)` / `op_result_types(op)` / `op_operand_types(op)`
  (all in `cir_ops.rs`, already typed) instead of re-deriving from text.
- `is_cir_va_list_type` / `is_cir_va_list_value_type` now take
  `(ty: &CirType, aliases: &BTreeMap<String, CirType>)` — two args, not one.
- `self.aliases` / `self.parent.aliases` is `BTreeMap<String, CirType>`
  (type aliases), separate from `self.attr_aliases` / `self.parent.attr_aliases`
  (`BTreeMap<String, Attr>`, attribute aliases like `#true`/`#bfi_field2`).
  Don't cross the streams — this exact confusion is Bug 6 below, in code the
  *previous* session didn't get to.
- `translate_asm_template` / `rust_asm_register_modifier` (`asm.rs`) were
  changed to take `&[rust_ast::Type]` (Rust types) rather than raw CIR type
  strings, computed once in `lower_extended_asm` via
  `self.parent.rust_type(ty)` before calling down into the free functions
  (which have no `self` access for alias resolution).

### 2. Rewrote `lower_const` (storage.rs) against typed `Attribute`

This was the other big piece the ticket called out. `lower_const` now pulls
`op.attr("value")`, resolves `#name` aliases via `self.parent.resolve_attr`,
and matches structurally on `Attr::{CirInt,CirFloat,CirBool,ConstVector,
ConstComplex,GlobalView,Dialect{dialect:"cir",mnemonic:"ptr",raw:Some("null")}}`
instead of prefix/suffix-matching `#cir.xxx<...>` text.

### 3. Deleted the now-fully-dead raw-text CIR parsing engine

Once `lower_const` no longer called them, ~20 `parse_cir_*` / `cir_*_text`
helpers in `constants.rs` had zero remaining callers (verified via
`rg -n '\bfn_name\(' --glob '*.rs' src/` before each deletion — this is a
whole-repo grep, not just the lowerer tree, so nothing outside `lowerer/` was
depending on them either): `parse_cir_int`, `parse_cir_uint128`,
`parse_cir_bool`, `parse_cir_fp`, `parse_cir_fp_expr`, `cir_fp_text`,
`cir_int_digits`, `parse_cir_scalar_expr`, `parse_cir_const_vector`,
`parse_cir_const_complex`, `parse_cir_complex_component`,
`parse_cir_global_view`, `parse_cir_global_view_indices`,
`parse_cir_global_views`, `parse_cir_global_view_array`, `parse_cir_f128_expr`,
`long_double_raw_expr`, `parse_cir_int_ptr`, `parse_cir_const_array`,
`parse_cir_const_array_elems`, `parse_cir_block_addr_labels`,
`is_cir_aggregate_init`, `bitfield_info_number`. Also
`split_top_level_arrow` / `split_top_level` (the local, `constants.rs`-private
one — **not** the one still used elsewhere like `c_ast.rs`/`function_abi.rs`,
those keep their own) / `split_record_member_types`, which had become an
isolated dead island once the above were gone.

Replaced with two small typed helpers (`constants.rs`): `const_vector_expr(elements: &[Attr])`
and `f128_from_text(text: &str)`, both built on the typed helpers the prior
session already added (`scalar_attr_expr`, `complex_component_from_attr`,
`fp_text_value`, `long_double_from_text`).

**Not part of this deletion, still alive and unrelated**: `cir_complex_inner`
(`runtime_support.rs`), `parse_rust_array_type`, `op_type_return` (`types.rs`)
show up as `dead_code` warnings but were already unused *before* this session
(confirmed via `git diff` — I never touched those files) — left alone per the
"don't clean up pre-existing dead code unless asked" rule. Worth a separate
tiny cleanup pass if anyone cares.

### 4. `cargo fmt` + `cargo clippy --release --all-targets` clean

Modulo the 3 pre-existing warnings above. **Note**: fmt was run once after the
initial "get it compiling" pass; the bugfix commits described below (section
5) have *not* been re-formatted. Run `cargo fmt` again before calling this
done.

### 5. Ran `cargo nextest r --release --profile lowering` repeatedly and found/fixed 6 real, previously-latent bugs

The whole lowerer hadn't compiled since the "get lowerer to compile at least"
checkpoint commit, so none of this had ever actually been exercised at
runtime. Once it built, the `lowering` profile immediately started surfacing
real semantic bugs — some from this session's mechanical migration, most
inherited from the *previous* session's already-compiling code that just
happened to compile clean while being silently wrong (the exact "attr_str
returns None for structured attributes, compiles fine, wrong behavior" trap
the ticket notes warned about — turned out to generalize beyond
`initial_value`).

**Methodology that worked every time** (repeat this for whatever's next):
1. `cargo run --release -- translate-lowered <fixture>.c` — raw lowering, no
   fixups, fastest way to see the bug.
2. `cargo run --release -- emit-cir <fixture>.c` — ground-truth CIR text for
   the same input.
3. Grep the **clang-ir crate itself** (`~/Projects/clang-ir/src/{parser,model,ast}/*.rs`)
   for how the attribute/type in question is actually represented — do not
   guess from the shape of the old raw-text code. Every bug below was found
   this way.
4. Fix, rebuild, re-diff the two outputs, then re-run
   `cargo nextest r --release --profile lowering` (always this exact
   profile — never plain `cargo test` / `cargo nextest run`, see CLAUDE.md)
   to find the next failure.
5. For a couple of these I also compiled the generated Rust standalone
   (`cargo new /tmp/scratch`, add `libc`/`bitfields`/`aligned` deps as
   needed, `cargo build`) and diffed its exit code against the real C
   binary's — worth doing for anything nonobvious, cheap insurance.

**Bug 1 — `attr_symbol_ref` read `@name` refs as `attr_str`.**
`op_utils.rs`'s `attr_symbol_ref` did `attr_str(op, key).strip_prefix('@')...`.
But `@name` references parse to `Attribute::SymbolRef(String)` in clang-ir,
not `Attribute::Str` — `attr_str` (`Attr::as_str`) only matches `Str` and
silently returned `None` for every symbol ref. Broke: direct function calls
(`calls.rs::lower_call`'s `"callee"` attr — every direct call fell through to
the *indirect*-call path, treating operand `[0]` as a function pointer, e.g.
`test1(19)` lowered to `19.unwrap()()`); weak-alias resolution (`"aliasee"`,
7 call sites in `lowerer.rs`); global variable references
(`memory.rs::lower_get_global`'s `"name"` attr — every global read/write
resolved to an empty name, rendered as `*_tmp`).
Fix: `attr_symbol_ref` → `op.attr(key).and_then(Attr::as_symbol_ref)`. Also
had a second, independent `attr_str(op, "callee")` call in `analysis.rs`
(va_list-ABI detection) that needed the same fix directly.

**Bug 2 — `void` functions got `-> core::ffi::c_void` instead of `()`.**
`lowerer.rs::lower_func` always wrapped the computed return type in
`Some(...)`. `extern_fn_signature` (declarations) already correctly filtered
out the void sentinel type; the function-*definition* path didn't. Fixed by
adding the same `.filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID))`.

**Bug 3 — clang-ir crate: struct-type parser missing the `padded` keyword.**
Real Clang CIR struct-type grammar (confirmed against
`~/llvm-project/clang/lib/CIR/Dialect/IR/CIRTypes.cpp`, both `parse` and
`print`) is `packed padded incomplete { members }`, each keyword optional, in
that fixed order. clang-ir's Rust parser (`parser/types.rs::parse_cir_record`)
only recognized `incomplete`/`packed`. Any struct printed with `padded` (i.e.
most bitfield-containing structs, since clang marks them padded once it adds
CIR's separate `pad` member for tail padding) hit the loop's `_ => break`,
left the parser sitting on the `padded` token instead of `{`, so
`self.eat(&Tok::LBrace)` failed and **the entire member list silently parsed
as zero members** (swallowed into unstructured "trailing" text instead).
Fixed in `~/Projects/clang-ir` (a separate repo, pulled in by local path — no
version bump needed, changes are picked up immediately): added
`padded: bool` to `ast::ty::StructType`, taught the parser loop to recognize
it. `cargo test --lib` in clang-ir itself still green. **This is a fix in the
clang-ir crate, not slate — remember to check/commit that repo separately.**

**Bug 4 — record name derived from the mangled alias key instead of the struct's own literal name, for two call sites in `memory.rs`.**
`bitfield_storage_member` and `record_name_from_op` did
`cir_ptr_pointee(ty).and_then(cir_record_name)` directly on the
*unexpanded* `CirType::Named(alias)` pointee. `cir_record_name`'s fallback for
`Named` is `name.strip_prefix("rec_")` — for a **named** struct this
coincides with the real name (`rec_Foo` → `Foo` = the struct literal's own
`s.name`), but for **anonymous** structs CIR mangles the alias key
differently from the literal name it embeds (e.g. alias key `rec_anon2E0`
strips to `"anon2E0"`, but the struct literal's actual `s.name` is
`"anon.0"` — `sanitize_ident` gives different results for these:
`"anon2E0"` vs `"anon_0"`). Every other place in the codebase that needs a
record name already expands the alias first specifically to avoid this
(`cir_type_to_ctype`, `anon_alias_key`, `any_alias_key` — the last two even
have a comment: *"resolve records through the alias table so anon fields
keep their dotted name"*). These two call sites just didn't, because they
were newly rewritten this session from the old raw-text version (which
*did* expand-then-extract, via `self.parent.aliases.get(ty_text)...`) and I
"simplified away" that step without realizing it was load-bearing.
Fix: `.map(|ty| self.parent.expand_alias(ty)).and_then(cir_record_name)`.
`record_name_from_op` had to gain a `&self` param (was a bare associated fn)
to reach `expand_alias`. Symptom before the fix:
`self.parent.records.get(&record_name)` silently failed, so bitfield member
access fell through to *plain* (non-bitfield) field access using the raw C
field name — e.g. `20040705-1.c`'s `b.i = ...` instead of routing through the
storage-group wrapper.

**Bug 5 — bitfield read/write shift expression used the storage field's raw
Rust type, not the logical field's type.**
`memory.rs::truncate_bitfield_expr` did `(storage_expr << sh) >> sh` where
`storage_expr` has the storage unit's Rust type (e.g. `u64`) but the
surrounding code (`materialize_expr`, the final assignment) expects the
*logical* bitfield's type (e.g. `i64`, when the C field is signed). Any time
storage and logical signedness differ this was a straight type error
(`expected i64, found u64`). Fixed by inserting an explicit same-width
`as <logical_type>` cast before the shift — same-width `as` between ints is a
bit-preserving reinterpret in Rust, matching C's implementation-defined (but
in practice universal) signed reinterpretation. Verified against `pr103417.c`
(reads only, both fields share one `u32`).

**Bug 6 — `bitfield_info` attribute alias not resolved in `bitfields.rs`.**
`collect_bitfield_storages` (which decides whether a bitfield *group*
sharing one storage unit needs a real multi-field wrapper struct, generated
via the `bitfields` crate, vs. the simple raw-shift path that's only valid
for a single field occupying the *entire* storage unit) reads
`op.attr("bitfield_info")` and matches it directly against
`Attr::BitfieldInfo{..}`. But CIR commonly hoists repeated `bitfield_info`
literals (same field, multiple access sites/functions) behind a `#name`
alias (e.g. `#bfi_field2`), same as it does for `#true`/`#false`. Un-resolved,
this always fell into `_ => None`, so `bitfield_storages` ended up
effectively empty and **every** multi-field-sharing-one-storage-unit bitfield
struct (e.g. `991118-1.c`'s `struct tmp3 { long long pad:11; long long field:53; }`)
fell back to the raw-shift path — which is simply wrong for a field that
doesn't start at bit 0 (silently reads/writes the wrong bits, corrupting
sibling fields on write). Fixed: `bitfield_info()` now takes `&Module` and
calls `module.resolve_attr(...)` first — this method already exists on
clang-ir's `Module` (mirrors `Lowerer::resolve_attr`, which the prior session
added for the same reason at the `Lowerer` level; this is the module-level
equivalent, needed here because `collect_bitfield_storages` runs *before* a
`Lowerer` exists). Threaded `&Module` down through `collect_function_bitfields`
(previously took `aliases: &BTreeMap<String, CirType>`, kept that as a local
inside the function body via `module.type_aliases`).
Verified: `991118-1.c` now emits a real `mod __slate_bitfields { ... }` with
`#[bitfields::bitfield]` wrapper structs; the generated Rust compiles
standalone and its exit code matches the real C binary (both 0) for the two
values the test exercises.

## What's NOT done — pick up here

### Bug 7 (in progress, root cause identified, fix not yet written)

**x86-64 SysV ABI struct-coercion anonymous types collapse to `i32`.**

Repro: `tests/fixtures.gcc-torture/pr30185.c` (also breaks `20021118-1.c`,
`20000717-5.c`, `20000717-1.c` per the last nextest run — same root cause,
check for others too). `struct S { char a; long long b; }` passed/returned
by value gets ABI-coerced by clang into a **genuinely unnamed**
(`StructType.name == None` — not the "has a name but it's `anon.N`" case from
Bug 4, an actual anonymous struct body) CIR struct type, e.g.:

```
!rec_anon_struct = !cir.struct<{data !s8i, data !s64i}>
```

Root cause: `types.rs::record_struct_type(s: &StructType) -> Type`:

```rust
fn record_struct_type(s: &StructType) -> Type {
    let Some(name) = s.name.as_deref() else {
        return Type::Prim(Prim::I32);   // <-- collapses anonymous coercion structs to i32
    };
    ...
}
```

Silently returns `Type::Prim(Prim::I32)` when `s.name` is `None`. This
cascades everywhere the type is used: `foo`'s Rust return type becomes `i32`
instead of `S`; the ABI-coercion alloca frame tuple elements
(`__SlateAllocaFrame0(Aligned<A8, i32>, ...)`) get scalar `i32` instead of a
real 2-field struct; `cir.get_member` accesses on the coercion alloca (which
use CIR's empty `name=""` attr, correctly routed by
`lower_get_member`/`memory.rs` to the `__slate_anon_{index}` synthetic-name
convention already used for anonymous nested struct members) then fail to
type-check because the target isn't actually a struct.

**Why this is fixable, and probably a small fix**: the naming *convention*
for these coercion structs already exists and is already consumed elsewhere —
`is_abi_coercion_record_name(name: &str) -> bool` (`types.rs` ~483, matches
`"anon_struct"`, `"anon_struct0"`, `"anon_struct1"`, ...) is used in
`memory.rs::lower_cast` to detect a cast between a coercion-struct pointer and
the "real" struct type and register a `coerce_alloca_real_type` redirect.
Deriving `"anon_struct"` from the CIR text `cir_record_name` on the
*unexpanded* `CirType::Named("rec_anon_struct")` (i.e. via the
`strip_prefix("rec_")` fallback, same mechanism as Bug 4 but this time it's
*correct* because there's no literal name to conflict with) gives exactly
`"anon_struct"` — matching. So the record for this type is expected to be
keyed by the **alias key**, stripped of `rec_`, not by `s.name` (which
doesn't exist).

There's also already a mechanism that appears to build the actual `Record`
(fields) for this case independent of `record_struct_type`:
`anon_local_records(module: &Module)` (`types.rs` ~940, called once in
`lowerer.rs::lower_with_project`) walks `cir.alloca` / `cir.get_member` /
`cir.global` / `cir.call_llvm_intrinsic` ops, and for any pointee type that's
an "anon alias" per `anon_alias_key` (`types.rs` ~538 —
`cir_record_name(expanded).or_else(|| name.strip_prefix("rec_"))?`, then
checks the resulting name starts with `"anon."` or `"anon_"` — **this should
already match our `"rec_anon_struct"` → `"anon_struct"` case**, since
`cir_record_name(expanded)` on `expanded = CirType::Struct(s)` with
`s.name == None` returns `None`, falling through to the `strip_prefix("rec_")`
branch) — builds a `crate::frontend::c_ast::Record` directly from the CIR
struct literal's own `s.members` (not from Clang AST, since there's no
AST node for a compiler-synthesized coercion type), naming unnamed/empty
`cir.get_member` fields `__slate_anon_{i}` (matching the access-site
convention). **I was mid-way through confirming this actually produces a
correct populated record** (as opposed to `required_record_defs`'s *other*
fallback, which produces an *empty*-fields record when the Clang-AST-side
`candidates` map has no entry — need to make sure `anon_local_records`'s
result actually reaches `self.records`/`self.parent.records`, i.e. check
where `anon_records` from `lower_with_project` gets merged into the main
records map, and that it wins/isn't shadowed by `required_record_defs`'s
empty fallback for the same key) when I got interrupted for this handoff.

**Next steps, in order**:
1. Confirm `anon_local_records`'s output actually lands in `self.records`
   with the right fields for `"anon_struct"` before touching anything else
   — grep `lower_with_project` for where `anon_records`/`anon_record_names`
   get merged in, relative to `required_record_defs`'s own insertion, and
   check there's no field-clobbering/empty-record-wins ordering bug.
2. Fix `record_struct_type` (types.rs ~128) to fall back to a synthetic name
   derived from the enclosing alias key (stripped of `"rec_"`) instead of
   `Type::Prim(Prim::I32)` when `s.name` is `None`. It's called from two
   places in `rust_type_with_aliases`:
   - `CirType::Named(name) => ... Some(CirType::Struct(s)) => record_struct_type(s)`
     — `name` (the alias key) is in scope here, easy to thread through.
   - `CirType::Struct(s) => record_struct_type(s)` — reached when a bare
     `CirType::Struct` shows up with no enclosing `Named` wrapper. Check
     whether this is actually reachable for a *truly* anonymous struct in
     practice (CIR seems to always wrap struct types behind a `!rec_X`
     alias, so this branch may only ever see already-named structs in
     which case it's moot — but verify, don't assume).
   Simplest signature change: `record_struct_type(s: &StructType, alias_key: Option<&str>) -> Type`,
   pass `Some(name)` from the `Named` branch, `None` from the bare-`Struct`
   branch (or investigate whether that branch needs the enclosing alias
   threaded down too, if it turns out to be reachable for anon structs).
3. Rebuild, `cargo run --release -- translate-lowered tests/fixtures.gcc-torture/pr30185.c`,
   confirm `fn foo(...) -> S` (not `-> i32`) and the alloca-frame tuple
   elements are a real 2-field struct, not `Aligned<A8, i32>`.
4. Compile the generated Rust standalone, diff exit code against the real C
   binary (same pattern as Bugs 5/6 above).
5. Re-run `cargo nextest r --release --profile lowering`. Expect this to
   either go fully green, or surface one more distinct bug — every failure
   in this session so far has been a *different* root cause, not a symptom
   of a previous one, so don't assume the next failure (if any) is related
   to Bug 7.

### After `lowering` is green

- Run `cargo nextest r --release --profile rewrites` — fixup-pass code
  hasn't been touched or exercised at all this session; the acceptance
  criteria for `slate-cevu.3` names both `lowering` and `rewrites` (and
  `libc` "as applicable"). Given how many latent bugs `lowering` alone
  turned up, don't assume `rewrites` is clean without actually running it.
- `cargo fmt` (edits since the last fmt run haven't been reformatted).
- `cargo clippy --release --all-targets` (re-check after fmt; was clean
  modulo the 3 pre-existing warnings before this bugfix round).
- The ticket's acceptance criteria also has a clause this session did **not**
  touch at all: *"cir-opt is invoked exactly once per compilation unit, not
  twice"* — `src/cir/emit.rs` / `src/cir/flatten.rs` need to hand
  already-normalized generic-form text directly to clang-ir's parser instead
  of letting clang-ir's own `Toolchain` re-invoke `cir-opt`. The ticket
  description mentions "the pre-normalized-text entry point from the
  loc/signature task" — check whether clang-ir already exposes this (look
  for a `parse_generic`/`from_text`-shaped entry point that skips its own
  `cir-opt` invocation) before assuming it needs to be added there too.
  This is a distinct, not-yet-started sub-task, separate from everything
  above.

## Files touched this session

**slate** (`/home/takashi/Projects/slate`):
`src/frontend/lowerer.rs`,
`src/frontend/lowerer/{storage,memory,intrinsics,asm,analysis,op_utils,constants,bitfields}.rs`.
No commit made — conservative git policy, nothing committed this session,
`git status` will show all of the above as modified.

**clang-ir** (`/home/takashi/Projects/clang-ir`, separate repo, pulled in by
local path in slate's `Cargo.toml` so changes are picked up immediately with
no version bump): `src/ast/ty.rs`, `src/parser/types.rs`. Also uncommitted.
Its own `cargo test --lib` is green. Worth reviewing/committing this one on
its own terms since it's a different project with its own history.

## Beads

`slate-cevu.3` is still `in_progress` — do not close it, Bug 7 is unresolved
and the `rewrites`/`libc` profiles and the cir-opt-once acceptance clause
haven't been checked at all. Update its notes with a summary of this session
(or just point at this file) before the next session picks it up, so the
in-issue notes don't drift out of sync with what's actually true.
