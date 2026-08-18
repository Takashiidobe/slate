# Handoff: CirOpKind -> clang-ir model::Instruction migration (slate-cevu.4)

Phase 2 of `slate-cevu`. Migrate slate's `CirOpKind` dispatch
(`src/frontend/lowerer.rs::lower_op`) onto clang-ir's typed
`model::Instruction`, one op family at a time, each verified against real
fixtures before any `CirOpKind` variant is deleted.

## Status

- Migrated (dispatch goes through the typed `match
  clang_ir::model::instruction::lower_op(op) { ... }` before the legacy
  `match op.kind()`): Binary, Unary, MathUnary, Shift, Rotate, Cmp, Select,
  Load, Store, Copy, Const, GetGlobal, Cast, GetBitfield, SetBitfield,
  GetElement, PtrStride, PtrDiff, AddOverflow/SubOverflow/MulOverflow,
  Call, GetMember, ExtractMember, Vec* (Splat/Extract/Create/Cmp/Insert/
  Shuffle), IsFpClass, ObjSize, IsConstant, Copysign, FMaxNum, FMinNum,
  Fmuladd, Fma, Modf, Complex{Create,Real,Imag,RealPtr,ImagPtr,Add,Sub},
  VaStart/VaEnd/VaCopy/VaArg, EhSetjmp, FrameAddress, ReturnAddress,
  Prefetch, InlineAsm, StackSave/StackRestore, MemChr, CallLlvmIntrinsic,
  BlockAddress, Assume, MemCpy/MemMove/MemSet, ClearCache, the 6 atomic
  ops, If, Return, Yield, Condition, Break, Continue, Br, BrCond, Goto,
  Label, Unreachable, Trap, While, DoWhile, For, Switch, Ternary, Scope,
  CleanupScope.
- Of those, most had their `CirOpKind` variant/parse-arm/legacy-match-arm
  deleted outright (single reference = the dead `lower_op` arm). Kept
  alive (real classification call sites elsewhere in the codebase, or a
  fallback arm that's load-bearing — see gotchas): `Call`, `GetMember`,
  `VaStart`, `VaCopy`, `VaArg`, `Asm`, `Stackrestore`, `CallLlvmIntrinsic`,
  `Const`, `GetGlobal`, all control-transfer ops, `While`, `Do`, `For`,
  `Switch`, `Ternary`, `CleanupScope`.
- Converted to the "pass the whole `Instruction`, not just discriminant
  fields" shape (Phase 2.5, see below): `Rotate`, `Shift`, `Select`,
  `Copy`, `Cmp`, `Cast`, `PtrStride`, `PtrDiff`, `Load`, `Store`, `Const`,
  `GetGlobal`, `GetElement`, `GetBitfield`, `SetBitfield`. Everything else
  migrated is still call-through (`self.lower_x(op)`), not yet converted.
- Blocked on the `clang-ir` crate (being fixed upstream, re-check before
  re-deriving): `DivOverflow`/`RemOverflow` deleted outright (dead — no
  div/rem overflow builtin exists in C). `ComplexMul`/`ComplexDiv`/
  `ComplexConj` and `SwitchFlat` have no crate-side modeling yet.
  `IndirectBr`/`IndirectGoto` left alone (uncertain crate-side support).
- Out of scope (zero `CirOpKind` variants, no C++ support): vtable ops,
  C++ EH ops.

## How to migrate one family

1. Check `~/Projects/clang-ir/src/model/instruction.rs`'s `Instruction`
   enum + `try_lower` to see which mnemonics collapse into the variant and
   exactly what it extracts.
2. Add one match arm in `lower_op` (`lowerer.rs`) on
   `clang_ir::model::instruction::lower_op(op)`, `instr @
   Instruction::TheVariant { .. } => { self.lower_x(op, instr); return; }`.
3. Existing `lower_*(op: &Op)` helpers almost always already read what
   they need from `op.operands`/`op.regions`/`op.attr(...)` — including
   control-flow ops with `Body`/region fields. Don't assume a rewrite is
   needed; check first (session 4 wrongly assumed `If` needed one).
4. Convert the helper to take `instr: Instruction` by value instead of
   `op` alone once its family has zero remaining `CirOpKind` references
   (see Phase 2.5 below) — re-narrow with `let Instruction::TheVariant {
   fields, .. } = instr else { unreachable!() };` as the first line.
5. Delete the `CirOpKind` variant/parse-arm/legacy-match-arm only after
   `rg -n 'CirOpKind::TheVariant\b'` is empty except the `ir.rs`
   declaration — check the *whole repo*, not just `lowerer/`.
6. Verify: `cargo build --release`, `cargo nextest r --release --profile
   lowering` (add `rewrites` too for high-traffic ops like Load/Store/
   Const), `cargo fmt`, `cargo clippy --release --all-targets`.
7. Update `slate-cevu.4`'s beads notes; leave the ticket open.

## Phase 2.5: pass `Instruction`, not just a discriminant

Sessions 1-6 only used the typed `Instruction` to classify, then called
`lower_x(op: &Op)` which re-derives everything by hand — including attrs
the crate already decoded into enums (`CmpOpKind`, `MemOrder`,
`AtomicFetchKind`, ...). New convention: helpers take the whole matched
`Instruction` by value, and while converting a family, also swap any
hand-rolled attr decode (e.g. `attr_int(op, "kind")`) for the crate's
already-typed field. `unreachable!()` re-narrowing isn't compiler-checked,
but every op family has fixture coverage so a dispatch/destructure
mismatch fails loudly on the first fixture that hits it — accepted
tradeoff, not a new risk category for this codebase (differential fixture
testing is already the whole correctness model, not compile-time
exhaustiveness).

Bare/zero-payload ops (`Break`, `Continue`, `Unreachable`, `Trap`) aren't
worth converting. Family-dispatch fns (`lower_binary_family`,
`lower_overflow_arith`, etc.) that re-dispatch to a different per-case
helper are a different shape (2nd-level dispatch) and don't need this
either.

**Only convert once a family has zero remaining `CirOpKind` references.**
A "legacy match op.kind() fallback arm" existing for a family does NOT by
itself mean the family is unsafe to convert — check whether the
hand-written helper's own early-return guards are provably identical to
`try_lower`'s preconditions (if so, the fallback is dead code, safe to
delete and convert — this was true for `Load`/`Store`). If the helper's
guards are looser than `try_lower`'s and silently produce a degenerate
default on malformed input (true of old `Const`/`GetGlobal` — missing
attr → fabricated zero value / empty-string global name), tighten the
helper's own guard to match `try_lower` first, *then* convert — moving the
precondition from a runtime check to "only reachable once dispatch already
validated it" is a deliberate behavior tightening, not just a refactor.

`GetBitfield`/`SetBitfield` looked provably-dead by this same guard-parity
check but weren't — a *third* failure mode, distinct from missing-attr:
`try_lower` can fail to decode an attr that's fully *present* (see the
alias gotcha below). Fixed at the crate level, then converted safely.

## Other gotchas

- A missing-operand/result `Instruction::try_lower` failure falls through
  silently to the `Other(_)` catch-all once the `CirOpKind` variant is
  deleted — same as the old code's early-return guards, not a behavior
  change, but confirm the guard parity per-family (see Phase 2.5 above).
- Zero comments in dispatch code — this repo's convention; migration
  rationale goes in this doc / beads notes / commit messages, not inline.
- Dead-arm removal (zero fixture/doc references, confirmed against real
  clang source) only needs a clean build to verify. Removing a
  parse-arm *spelling* (e.g. `"cir.indirect_br"` alias, superseded by
  `"cir.indirect_goto"`) changes real parse output, so that needs the
  `lowering` profile, same bar as any other behavior change.
- **Attribute alias gotcha**: MLIR's generic printer hoists large/repeated
  attributes (e.g. `#cir.bitfield_info<...>`) into a module-level `#name =
  ...` alias, referenced elsewhere as bare `#name`. `Instruction::try_lower`
  only sees a bare `&Operation`, not the owning `Module`, so it can't
  resolve that alias — a structural `TryFrom<&Attribute>` decode (like
  `BitfieldInfo::try_from`) fails on the *unresolved* alias even though the
  op is well-formed and the attr is genuinely present. This silently
  degraded `GetBitfield`/`SetBitfield` to the `Other(_)`/`todo!` path for
  any bitfield op whose attr got aliased — caught by a full `lowering`
  profile run, not by `cargo check`, since it's real ops failing to lower,
  not a compile error. **Fixed in the crate** (`~/Projects/clang-ir`, local
  path dep — see session 9): `Module::resolve_named_attrs`, called at the
  end of `parse_module`, eagerly resolves every `Attribute::Named`
  reachable from the parsed ops against the module's alias table right
  after parsing, so `op.attr(...)` never returns an unresolved alias again.
  Lesson: guard-parity between a hand-written helper and `try_lower` isn't
  enough on its own — also sanity-check against real emitted CIR (not just
  a synthetic minimal fixture) before trusting "provably dead."
- `Attr`/`CirType` (`src/cir/ir.rs`) are `pub use clang_ir::ast::{Attribute
  as Attr, ..., Type as CirType}` — literally the same types `Instruction`'s
  typed fields use, not a separate representation. An earlier version of
  this doc (session 8) wrongly said they needed reconciling; they don't.
  See `slate-cevu.6` for the actual remaining gap (per-operand types and
  the untyped block/region traversal engine).

## Session log

1. Binary family done (reference pattern).
2. Unary, MathUnary, + 18 single-mnemonic families wired into the typed
   match.
3. Remaining ~41 non-control-flow `Instruction` variants wired in.
4. Corrected a wrong assumption that control flow needs special handling;
   `If` migrated using the same one-arm pattern as everything else.
5. Control-transfer family (`Return`/`Break`/`Continue`/`Br`/etc.)
   migrated. Deleted two confirmed-dead things unrelated to the migration:
   `DivOverflow`/`RemOverflow` variants, and the `"cir.indirect_br"`
   parse-arm spelling.
6. Started Phase 2.5 (instruction-passing shape) with `Rotate` as the
   reference conversion.
7. Converted `Cmp`/`Cast`/`PtrStride`/`PtrDiff` to Phase 2.5.
   Migrated `While`/`DoWhile` as the first new control-flow family.
   Wrongly flagged `Load`/`Store`/`Const`/`GetGlobal` as universally
   unsafe to convert (too coarse — corrected next session).
8. Audited `Load`/`Store`/`Const`/`GetGlobal` individually: `Load`/`Store`
   were provably-dead-fallback, converted directly. `Const`/`GetGlobal`
   had real (if degenerate) fallback behavior on malformed input, so
   tightened them to require the typed `Instruction` (making the missing-
   attr fallback structurally unreachable) before converting. Deleted all
   four now-dead legacy match arms.
9. Migrated `For`/`Switch`/`Ternary`/`Scope`/`CleanupScope` (additive,
   op-only — their fallback arms do real work on malformed input, not
   provably dead). Converted `GetElement` (provably dead) directly. Tried
   the same for `GetBitfield`/`SetBitfield`, which regressed a real gcc
   torture fixture (`20040709-1.c`, bitfields + long double) —
   `BitfieldInfo::try_from` was failing on a real, well-formed op because
   its attr had been hoisted into an MLIR attribute alias `try_lower`
   couldn't resolve (see the alias gotcha above). Root-caused and fixed in
   the `clang-ir` crate itself rather than working around it in slate, then
   re-converted `GetBitfield`/`SetBitfield` on top of the fix. Scoped a
   follow-up epic child (`slate-cevu.6`) for making `Instruction` fully
   self-sufficient (per-operand types, typed block/region traversal) so
   `lower_*` helpers can eventually drop `op` entirely — out of scope for
   this ticket, doesn't block continuing per-family migration.
