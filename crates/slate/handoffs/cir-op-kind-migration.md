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
  Label, Unreachable, Trap, While, DoWhile.
- Of those, most had their `CirOpKind` variant/parse-arm/legacy-match-arm
  deleted outright (single reference = the dead `lower_op` arm). Kept
  alive (real classification call sites elsewhere in the codebase —
  `analysis.rs`, `types.rs`, `bitfields.rs`, `asm.rs`, `control_flow.rs`,
  `cir_ops.rs`): `Call`, `GetMember`, `VaStart`, `VaCopy`, `VaArg`, `Asm`,
  `Stackrestore`, `CallLlvmIntrinsic`, `Const`, `GetGlobal`,
  `GetBitfield`, `SetBitfield`, `GetElement`, all control-transfer ops,
  `While`, `Do`.
- Converted to the "pass the whole `Instruction`, not just discriminant
  fields" shape (Phase 2.5, see below): `Rotate`, `Shift`, `Select`,
  `Copy`, `Cmp`, `Cast`, `PtrStride`, `PtrDiff`, `Load`, `Store`, `Const`,
  `GetGlobal`. Everything else migrated is still call-through
  (`self.lower_x(op)`), not yet converted.
- Not attempted: `For`, `Switch`, `Ternary`, `Scope`, `CleanupScope` (new
  families — expect the `If`/`While` one-line-intercept pattern to work
  unchanged, `Switch` has more attr-decoding surface so verify against the
  full fixture suite, not just a clean build).
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
`GetBitfield`/`SetBitfield`/`GetElement` still need this audit before
converting.

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
