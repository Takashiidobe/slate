# va_list handoff — gcc-torture epic (slate-os0h.3.1.43 / .44 / .40 / .46)

## Where things stand

Committed: `7c967396` "fix up failing va list fixtures" (already pushed by the
user, not by this session). `.beads/issues.jsonl` has an uncommitted diff from
`bd update --notes` calls made after that commit — just the notes text below,
no code changes; safe to commit or leave.

Full investigation notes (what changed, why, file-by-file) are on the bd
tickets themselves — read those first, this doc doesn't repeat them:

```
bd show slate-os0h.3.1.43   # va_list storage shapes (globals/fields/arrays/pointers)
bd show slate-os0h.3.1.44   # va_copy + escaping lifetimes
bd show slate-os0h.3.1.40   # aggregate variadic args (untouched this session)
bd show slate-os0h.3.1.46   # SIMD vector construction (untouched, unrelated domain)
```

`cargo nextest r --release --profile lowering` (whole profile) is green.

### Promoted this session
`stdarg-2`, `va-arg-13` (3.1.43) · `va-arg-10`, `va-arg-14` (3.1.44) ·
`20000519-1`, `20071213-1`, `stdarg-4`, `va-arg-9`, `va-arg-20` (incidental,
not in any ticket's case list — `gcc_torture_unsupported_tests_still_fail`
flagged them as a side effect of the same change)

### Still open
- **3.1.43**: `pr64979`, `va-arg-trap-1`, `va-arg-21`
- **3.1.44**: `stdarg-1`
- **3.1.40**: everything (claimed but no code touched this session)
- **3.1.46**: everything (claimed but not started — separate domain, see below)

## The core mechanism now in place

Read `docs/gcc-torture-triage.md` and `docs/lowerer.md` before touching
anything below — this section assumes that context.

- `module_requires_native_va_list` (`src/frontend/lowerer/analysis.rs`)
  decides, **once per translation unit**, whether every va_list use in the
  file can be `__SlateVaArgs` (boxed: `Rc<Vec<__SlateVaArg>>` + `index`,
  `Clone` = independent cursor over shared storage) or must stay
  `core::ffi::VaList` (native, real ABI). It's a whole-file switch, not
  per-function: native is forced only if something in the file needs genuine
  C ABI — a pub-exported/`c_abi_functions` function touching va_list, or a
  call to a function *not defined in this module* with a va_list argument
  (e.g. real `vprintf`/`vfprintf`). No fixture in the corpus mixes both needs
  in one file, so the whole-file simplification hasn't been tested against
  that case.
- `va_list_shaped_type` / `c_va_list_shaped_type`
  (`src/frontend/lowerer/types.rs`, `src/frontend/lowerer.rs`) recognize the
  bare record, `ptr<record>` (decayed by-value `va_list`), `array<record,N>`
  (storage, including real C arrays of `va_list`), and `ptr<array<record,N>>`
  (a genuine `va_list*`) uniformly, recursing through `cir.ptr`/`cir.array`
  layers. This is the type-side fix for globals/fields/arrays.
- `va_target_place` (`src/frontend/lowerer/memory.rs`) plus propagation
  through `cir.cast` and `cir.load` (`memory.rs`, `storage.rs`) is the
  place-side fix: va_start/va_arg/va_copy operands are always the result of
  an intervening cast (array decay) or load (parameter shadow-copy), so the
  underlying storage place (from `get_member`/`get_element`/`get_global`/
  `alloca`, via the pre-existing `place_expr`) has to be threaded through
  those intervening ops. `va_places: BTreeMap<String, Expr>` on
  `FunctionLowerer` records this propagation.
- `__SlateVaArgs`'s `args` field is `Option<Rc<Vec<__SlateVaArg>>>` with a
  `const fn empty()`, not `Rc::new(vec![])` directly — `Rc::new` isn't
  `const` in this toolchain, and va_list-shaped globals/uninitialized locals
  need a const-evaluable default.
- `default_value_expr` and record `Copy`-derivation
  (`src/backend/codegen.rs`) were adjusted for `__SlateVaArgs` being the
  first non-`Copy` field type ever embedded in a record/array: array
  defaults use `core::array::from_fn` instead of `[x; N]`, and
  `record_fields_are_copy` skips `#[derive(Copy)]` (keeps `Clone`) for any
  record with a `__SlateVaArgs`-shaped field. `record_fields_are_copy` only
  checks *direct* field types — a struct containing another struct that
  itself embeds a `va_list` won't be detected. Not exercised by any current
  fixture; fix if it comes up (needs a module-wide fixed-point pass over all
  records, not a per-record check).
- The old per-function `boxed_va_args`/`function_forwards_va_list` machinery
  was removed — replaced by the single module-wide `self.va_list_boxed` flag
  on `Lowerer`, read via `self.rust_type(...)` (CIR-string path) and
  `self.c_type_to_type(...)`/`self.c_record_field_type(...)` (C-AST path,
  used for struct field definitions specifically).
- `__SlateVaArg`/`__SlateVaArgs` are still emitted as a raw string literal
  (`SupportModule` in `src/frontend/lowerer.rs`, search `__SlateVaArg`) —
  **the user said they'll convert this to proper AST themselves** (matching
  how the `LongDouble` prelude is already built), so don't do it unless
  asked again.

## The known gap: `va_list*` pointer places (pr64979, va-arg-trap-1, stdarg-1)

This is the design work flagged for next turn. Diagnosed but not fixed.

**Symptom** (from `cargo run --release -- translate-lowered
tests/fixtures.gcc-torture.unsupported/pr64979.c`): `bar(int x, va_list *ap)`
gets a real `ap: *mut __SlateVaArgs` parameter (correct — `va_list_shaped_type`
already resolves `ptr<array<record,N>>` to `Type::Ptr{inner: VaList-ish}`
correctly). But inside `bar`'s body, `va_arg(*ap, int)` lowers to
`ap.next_arg::<i32>()` instead of `(*ap).next_arg::<i32>()` — calling the
method directly on the raw pointer instead of dereferencing it first. Same
family of bug: `if (ap)` needs `!ap.is_null()`, not `ap` used as a `bool`.

**Root cause**: `lower_load`'s new va-list guard
(`src/frontend/lowerer/storage.rs`, search `va_target_place`) fires whenever
the *loaded type* is va-list-shaped — but that's true both when loading a
by-value `va_list` (where propagating the underlying place is correct: the
loaded value *is* the same storage) and when loading a genuine `va_list*`
*pointer value* (where the load produces a pointer, and using its place
directly — instead of dereferencing the pointer it holds — skips a level of
indirection). The guard doesn't currently distinguish these two cases.

**Recommended fix, not yet attempted**: narrow the `lower_load` (and
`lower_cast`) va-list guard to only fire when the loaded/cast type is the
*bare* value shape (`Type::VaList` or `is_boxed_va_args_type(..)`), not when
it's `Type::Ptr{ inner: <va-list-ish> }`. Let the pointer-value case fall
through to the ordinary load/cast path (producing a plain pointer value in
`self.values`, same as any other pointer load). Then change
`va_target_place` (`memory.rs`) to fall back to the **existing**
`place_or_deref_expr` (already used elsewhere for exactly "no direct place →
deref the pointer operand") instead of bare `place_expr`, so a real
`va_list*` value that doesn't resolve to a tracked place gets dereferenced
automatically before `.next_arg()`/assignment. Concretely:

```rust
// va_target_place, memory.rs — sketch, not applied
pub(super) fn va_target_place(&self, ptr: &str) -> Option<Expr> {
    self.va_places.get(ptr).cloned().or_else(|| Some(self.place_or_deref_expr(ptr)))
}
```
(needs checking that `place_or_deref_expr`'s unconditional-`Some` return
doesn't change behavior for callers that currently rely on `va_target_place`
returning `None` to skip lowering silently — check `lower_va_start`/
`lower_va_arg`/`lower_va_copy`'s `let Some(place) = ... else { return }`
guards.)

Also check `call_arg_expr` (`values.rs`) — it currently only special-cases
`is_boxed_va_args_type(ty)` (by-value clone-forward). A call passing a
genuine `va_list*` argument (`bar(x, ap)` where `ap: *mut __SlateVaArgs`)
should already fall through correctly to the ordinary pointer-argument path
below it, but this hasn't been verified against pr64979/stdarg-1 yet — do
that first since the fix might already be closer than it looks.

`va-arg-trap-1`'s failure (`cannot find value _1 in this scope`) looked
different in the one look taken at it — possibly a separate, unrelated
lowering bug (a function returning `&ap` from a helper, i.e. `va_list
*foo(void) { ...; return &ap; }`, not wired through correctly). Root-cause
it independently once the pr64979 pointer-place fix lands; it may or may not
share a cause.

## The hard one: `va-arg-21` (malloc'd va_list + real `vprintf`)

Not investigated deeply — flagged as needing a design decision, not just a
lowering fix. The case: `va_list *ap_array[3]` populated via `malloc`,
pointer-array arithmetic (`ap_ptr++`), and a **real** `vprintf(s,
**ap_ptr)`call on that storage.

The conflict: this needs genuine `vprintf` ABI interop (forcing native
`core::ffi::VaList` per `module_requires_native_va_list`'s existing rule),
*and* it needs the va_list to live in `malloc`'d raw memory with pointer
arithmetic over it — `core::ffi::VaList<'a>` has no defined size/layout
contract Slate can rely on and can't be reconstructed from an arbitrary raw
buffer safely. `__SlateVaArgs` (the boxed representation) could live in
`malloc`'d memory fine, but then the real `vprintf` call can't consume it.

Two directions, neither started:
1. **Shim `vprintf` against `__SlateVaArgs`**: reimplement the subset of
   printf format-string interpretation needed (parse `%s`/`%d`/etc., pull
   typed args from the boxed vec) instead of calling the real libc
   `vprintf`. This would let va-arg-21 (and anything else forcing native
   today solely because of a vprintf/vfprintf call) box like everything
   else, sidestepping the native-ABI/malloc conflict entirely. Check
   `try_format_call_shims`/`try_long_double_call_shim` in
   `src/frontend/lowerer/calls.rs` first — there's existing shim
   infrastructure for long-double format args, but **no** existing
   general format-string interpreter; this would be new work, not an
   extension of something half-built.
2. **Give native `va_list*` a real raw-memory story**: transmute/reinterpret
   `malloc`'d bytes as `core::ffi::VaListImpl` and hope the byte layout
   matches `__va_list_tag` on the target ABI (x86-64 SysV: it should, by
   construction, but this is exactly the kind of "morally UB, works in
   practice" hack the project generally avoids — flag it explicitly if
   pursued, don't do it quietly).

Option 1 is very likely the better fit for the "solutions should scale, not
be filled with edge cases" instruction the epic is being worked under —
option 2 is a one-off hack for one test case; a vprintf shim is broadly
reusable and could also simplify/replace the existing supported
`vprintf-1.c`/`va_list_vfprintf.c` fixtures' reliance on real libc.
**This needs a decision from the user before implementation** — it's a
meaningfully different amount of work than everything else in this ticket
and changes the "when do we need native va_list at all" calculus for the
whole file-wide switch described above.

## 3.1.40 and 3.1.46 — not started this session

3.1.40 (aggregate variadic args — `__SlateVaArg::read` size/layout
reconstruction bugs for structs) and 3.1.46 (GNU vector construction/splats,
a SIMD domain, unrelated to variadics) were claimed (`bd update --claim`) at
the start of the session but received no code changes. Re-read their bd
notes/description fresh before starting — 3.1.40 in particular already has
prior-session notes describing a partially-diagnosed `__SlateVaArg::read`
bug (size mismatch reconstructing a struct extracted from a va_list) that's
a different bug class from everything fixed this session.

## Suggested skills for the next session

- None of the general-purpose Claude Code skills (dataviz, cloudflare, etc.)
  apply here — this is pure Rust/MLIR compiler work internal to the repo.
- If picking up the `vprintf` shim direction: no dedicated skill exists for
  "printf format string interpretation," but `docs/writing-a-fixup.md` and
  `docs/facts.md` are required reading before adding any new backend
  rewrite/shim pass — the format-shim work should follow that pattern
  rather than growing `calls.rs` ad hoc.
- `docs/gcc-torture-triage.md` and `docs/lowerer.md` remain the two
  load-bearing docs for this whole epic; re-read both fresh each session,
  they're short.
