# Handoff: CirOpKind -> clang-ir model::Instruction migration (slate-cevu.4)

Phase 2 of `slate-cevu`. Migrate slate's own `CirOpKind` dispatch (in
`src/frontend/lowerer.rs::lower_op`) onto clang-ir's typed
`model::Instruction` one op family at a time. Not a single cutover — each
family is its own commit, verified against real fixtures before the old
`CirOpKind` variants are deleted.

The Binary family (`add`/`sub`/`mul`/`div`/`rem`/`and`/`or`/`xor`/`fadd`/
`fsub`/`fmul`/`fdiv` -> `Instruction::Binary`) is done; use it as the
reference pattern (`src/frontend/lowerer/arithmetic.rs::lower_binary_family`,
wired in at the top of `lower_op` in `lowerer.rs`).

Also done (session 2): Unary (`inc`/`dec`/`minus`/`not`/`fneg` ->
`Instruction::Unary`, `lower_unary_family`), MathUnary (17 mnemonics ->
`Instruction::MathUnary`, `lower_math_unary_family`), plus 18 single-mnemonic
families dispatched straight from `lower_op`'s typed match with no separate
family fn: Shift, Rotate, Cmp, Select, Load, Store, Copy, Const, GetGlobal,
Cast, GetBitfield, SetBitfield, GetElement, PtrStride, PtrDiff, AddOverflow,
SubOverflow, MulOverflow. `lower_op` now does one
`match clang_ir::model::instruction::lower_op(op) { ... }` covering all
migrated families before falling into the legacy `match op.kind()`.

Load/Store/Const/GetGlobal/GetBitfield/SetBitfield/GetElement keep their
`CirOpKind` variant, parse arm, *and* legacy match arm — grep showed other
call sites outside `lower_op` (`analysis.rs`, `op_utils.rs`, `bitfields.rs`,
`types.rs`, `control_flow.rs`) still classify ops via `op.kind()` directly,
and the legacy arm is still reachable as the malformed-op fallback per the
gotcha below. Only variants with exactly one reference repo-wide (the dead
`lower_op` arm) were deleted this slice.

Also done (session 3, same sitting as session 2): every remaining
non-control-flow `Instruction` variant that clang-ir models — Call,
GetMember, ExtractMember, the Vec* family (Splat/Extract/Create/Cmp/Insert/
Shuffle), IsFpClass, ObjSize, IsConstant, Copysign, FMaxNum, FMinNum,
Fmuladd, Fma, Modf, the Complex* value ops (Create/Real/Imag/RealPtr/
ImagPtr/Add/Sub — Mul/Div/Conj aren't in clang-ir's Instruction enum, still
legacy), VaStart/VaEnd/VaCopy/VaArg, EhSetjmp, FrameAddress/ReturnAddress,
Prefetch, InlineAsm, StackSave/StackRestore, MemChr, CallLlvmIntrinsic,
BlockAddress, Assume, MemCpy/MemMove/MemSet, ClearCache, and the six atomic
ops (Fetch/Xchg/Fence/CmpXchg/TestAndSet/Clear). All dispatch straight from
the same typed match in `lower_op`, no new family fns needed (each
Instruction variant maps to exactly one CirOpKind already).

Of those, Call/GetMember/VaStart/VaCopy/VaArg/Asm/Stackrestore/
CallLlvmIntrinsic keep their `CirOpKind` variant + parse arm + legacy match
arm (same reasoning as the Load/Store group above — grep found other
classification call sites: `lower_op` noreturn/varargs checks,
`analysis.rs`, `types.rs`, `bitfields.rs`, `asm.rs`, `control_flow.rs`'s
stackrestore detection). The other 41 variants had exactly one reference
(the dead `lower_op` arm) and were fully deleted.

**Correction (session 4)**: the paragraph above claiming structured control
flow "requires rewriting the statement-lowering functions" was **wrong** —
tested and disproven by actually migrating `If`. Reasoning error: I looked
at `Instruction::If { condition, then_body: Body, else_body: Body }` and
assumed slate's `lower_if` would need to consume that typed `Body` tree.
It doesn't. `lower_if(op: &Op)` (and every other legacy `lower_*` control-flow
helper) reads straight from `&Op` — `op.operands`/`op.regions` — same as
every value-op helper. The typed match arm never has to touch `then_body`/
`else_body` at all:

```rust
clang_ir::model::Instruction::If { .. } => {
    self.lower_if(op);
    return;
}
```

This is the exact same one-line interception pattern as every family
above. `CirOpKind::If` had exactly one repo-wide reference (the dead
`lower_op` arm) and was deleted outright (enum variant + parse arm + match
arm). Verified: `cargo build --release` clean, `lowering` 7/7 and
`rewrites` 105/105 green, `cargo fmt`/`cargo clippy` clean.

**Implication**: the *entire* control-flow family (`While`/`DoWhile`/`For`/
`Switch`/`Case`/`Ternary`/`Try`/`Scope`/`CleanupScope`/`Return`/`Yield`/
`Condition`/`Break`/`Continue`/`Br`/`BrCond`/`Goto`/`Label`/`IndirectGoto`/
`Unreachable`/`Trap`) is likely just as mechanical as everything already
done — each already has an existing `lower_*(op: &Op)` helper that ignores
the typed `Body`, so the same one-arm-per-variant pattern should apply
uniformly. Don't assume any given family needs special handling just
because its typed variant has region/body fields — check whether the
*existing* slate helper actually consumes those fields (it almost never
does; slate re-derives everything from `&Op` itself). The only real
per-family risk is whatever `try_lower`'s `?`-chain can fail on (e.g. `If`
requires both `region(op,0)` and `region(op,1)` to be present — CIR always
emits both regions structurally, even for an empty else, so this hasn't
been observed to matter, but note it before deleting a variant on a family
where that isn't already guaranteed).

Still untouched: vtables (`VtableGetVptr`/`VtableGetVirtualFnAddr`/
`BaseClassAddr`/`DerivedClassAddr`) and C++ exception-handling ops
(`EhLongjmp`/`BeginCatch`/`EndCatch`/`InitCatchParam`/`Resume`) — these
have zero `CirOpKind` variants today (slate doesn't support C++), so
there's nothing to migrate; wiring them up would be adding new op support,
out of scope for this ticket.

**Blocked on the clang-ir crate itself** (confirmed by grepping
`instruction.rs`, not just assumed): `DivOverflow`/`RemOverflow` (the
`add.overflow`/`sub.overflow`/`mul.overflow` mnemonic match in `try_lower`
literally doesn't include `div`/`rem` — this is unrelated to `Binary`'s
`no_signed_wrap`/`no_unsigned_wrap`/`saturated` flags, which only cover
plain wraparound UB tracking on add/sub/mul, not the two-result
overflow-checked-division builtin shape slate's `cir.div.overflow`/
`cir.rem.overflow` ops use), `ComplexMul`/`ComplexDiv`/`ComplexConj` (no
`complex.mul`/`complex.div`/`complex.conj` mnemonic arm or type variant
exists in this crate version at all — only `Create`/`Real`/`Imag`/
`RealPtr`/`ImagPtr`/`Add`/`Sub` are modeled), and `SwitchFlat` (no
`switch.flat` arm). `IndirectBr`/`IndirectGoto` is ambiguous: clang-ir does
have `Instruction::IndirectGoto` on mnemonic `"indirect_goto"`, but per the
user (session 5) it may no longer be supported/emitted correctly on the
crate side — left alone for now rather than risk it. All of these are
being addressed upstream in the clang-ir crate in parallel; re-check when
that lands rather than re-deriving this from scratch.

Session 5: migrated the control-transfer op family — `Return`, `Yield`,
`Condition`, `Break`, `Continue`, `Br`, `BrCond`, `Goto`, `Label`,
`Unreachable`, `Trap` — same one-arm-per-variant pattern as `If`. All 11
keep their `CirOpKind` variant/parse-arm/legacy-match-arm alive (each has
2+ other classification call sites in `control_flow.rs`'s loop/switch/
dispatch state-machine building — grep confirmed none of them are down to
a single reference), so this slice was purely additive: the typed match
now dispatches these directly, the legacy match still exists as the
fallback + external-classification path. `Yield`/`Condition`/`Label` were
already no-ops in the legacy arm (values consumed elsewhere, e.g. by the
parent `Switch`/`Ternary`/`Case`/label-dispatch machinery, not at
top-level `lower_op`); the typed intercepts preserve that by also
returning without calling anything.

Verified: `cargo build --release` clean, `lowering` 7/7 and `rewrites`
105/105 green, `cargo fmt`/`cargo clippy --release --all-targets` clean.

Still not attempted (need their own slice — these are the ones with real
`Body`/`InstBlock` trees the `lower_*` helpers *do* consume through
`op.regions` + `self.lower_region_ops`, same call-through pattern as `If`,
just untested yet): `While`, `Do`/`DoWhile`, `For`, `Switch`, `Ternary`,
`Scope`, `CleanupScope`. Expect these to work with the identical
one-arm-per-variant pattern — `If` already proved the `Body` fields don't
need to be touched — but `Switch` in particular carries a `Vec<SwitchCase>`
with more attribute-decoding surface than `If`'s two plain regions, so
verify it against the full fixture suite (not just a clean build) before
deleting `CirOpKind::Switch`.

## Why this is safe to do incrementally

`clang_ir::model::instruction::ValueId` is just a type alias for `String` —
the exact same SSA-name strings slate's `Op::operands`/`Op::results` already
use. So matching on the typed `Instruction` instead of the generic `Op` is
purely a dispatch-classification change; it does **not** require rewiring
`self.values`, `self.slot_types`, `operand_expr`, `materialize_expr`, or
anything else keyed by SSA name. Low risk per family.

## Steps for one op family

1. **Pick a family.** Look at `~/Projects/clang-ir/src/model/instruction.rs`'s
   `Instruction` enum and its `try_lower` match (around line 973 on) to see
   which `cir.*` mnemonics collapse into one typed variant. Good next
   candidates: `Unary` (`inc`/`dec`/`minus`/`not`/`fneg`), `Shift`/`Rotate`,
   `Cmp`, `Select`. Bigger/riskier ones (`Call`, `GetMember`, atomics) should
   wait until the pattern is well-proven.

2. **Read the variant's fields** in `instruction.rs` and its `try_lower` arm
   to see exactly what's extracted (which attrs, which operand positions,
   any flags) — this tells you what the existing hand-rolled `CirOpKind`
   arms are duplicating.

3. **Add one interception point** at the top of `lower_op` in
   `src/frontend/lowerer.rs`, before the big `match op.kind()`:
   ```rust
   if let clang_ir::model::Instruction::TheVariant { field, .. } =
       clang_ir::model::instruction::lower_op(op)
   {
       self.lower_the_family(op, field, ...);
       return;
   }
   ```
   Add one `if let` per migrated family, all before the legacy match.

4. **Write the family dispatch fn** in the relevant lowerer submodule (e.g.
   `arithmetic.rs`, `values.rs`) that matches on the typed fields and
   delegates to the **existing, unchanged** `lower_*` helper functions for
   each concrete case. Don't rewrite those helpers' internals in the same
   slice — they already correctly handle vector shapes, const-folding, etc.
   off `&Op`; keep the diff to classification only.

5. **Preserve exact prior behavior**, including any asymmetric special-casing
   (e.g. Binary's `saturated` flag only matters for Add/Sub, because that's
   the only pair with real saturating builtins in C — verify *why* an
   asymmetry exists before "fixing" it away).

6. **Delete the now-dead `CirOpKind` variants**, but only after:
   - `rg -n 'CirOpKind::TheVariant\b'` across the whole repo (not just
     `lowerer/`) comes back empty except for the `ir.rs` declaration/parse
     arm itself.
   - Remove the variant from the `CirOpKind` enum and its `"cir.foo" =>
     CirOpKind::Foo` line in `CirOpKind::parse` (`src/cir/ir.rs`). Anything
     not explicitly matched already falls into the `Other(String)` catch-all,
     so this is safe once nothing references it.
   - Remove the corresponding arms from the legacy `match op.kind()` in
     `lower_op`.

7. **Verify**: `cargo build --release`, then
   `cargo nextest r --release --profile lowering` (frontend/lowering change
   — that profile alone is sufficient, no need for `rewrites`/`libc` unless
   the family touches backend/fixup code too). Then `cargo fmt` and
   `cargo clippy --release --all-targets`.

8. Update `slate-cevu.4`'s beads notes with what family moved, matching this
   file's level of detail, and leave the ticket open — closing happens once
   the ~150 remaining variants are gone, family by family.

## Gotchas hit so far

- `Instruction::Binary`'s `try_lower` returns `None` (falls back to
  `Instruction::Other(op.clone())`) if a required operand/result is missing
  — the typed `if let` just won't match in that case, and execution falls
  through to the legacy `match op.kind()`. Once the corresponding
  `CirOpKind` variant is deleted, a malformed op like that silently no-ops
  via the `Other(_)` catch-all arm, same as the old code's early-return
  guards did. Not a behavior change, but worth knowing before you delete a
  variant.
- Don't add comments explaining the migration inline in the dispatch code —
  this repo's convention is zero comments; put rationale here or in commit
  messages/beads notes instead.

## Dead/stale CirOpKind cleanup (session 5, not part of the migration proper)

Two things got deleted outright rather than migrated, both confirmed dead
by checking the real source (clang's CIR codegen / the user's own
knowledge of the crate history), not just by "no fixture hits it":

- `DivOverflow`/`RemOverflow` (`cir.div.overflow`/`cir.rem.overflow`):
  never emitted by real clang. `__builtin_{add,sub,mul}_overflow` are the
  only overflow-checking builtins C has — no div/rem equivalent exists in
  the language, and `CIRGenBuiltin.cpp` only instantiates
  `emitOverflowOp<...>` for `Add`/`Sub`/`MulOverflowOp`. The whole
  `CirOpKind` enum was added in one shot in `e3685719` from an assumed op
  vocabulary rather than derived from real output, and this pair never
  had a fixture, test, or doc reference. Deleted enum variant + parse arm
  + legacy match arm; `lower_overflow_arith` stayed (still used by
  `Add`/`Sub`/`MulOverflow`).
- The `"cir.indirect_br"` parse-arm spelling (not the whole `IndirectBr`
  `CirOpKind` — that's real, backs GCC computed-goto support via
  `lower_indirect_br`/`indirect_target_values`/`block_diverges`). CIR
  renamed this op to `indirect_goto` in an earlier version; slate's
  `parse()` had `"cir.indirect_br" | "cir.indirect_goto"` as a
  backwards-compat shim. Removed the `"cir.indirect_br"` alternative,
  confirmed via `lowering` profile (7/7 still green) that nothing depends
  on the old spelling.

General pattern for this kind of cleanup: dead-arm removal (like
`DivOverflow`/`RemOverflow`, or any future one with zero fixture/doc
references) doesn't need a full verify pass since there's no reachable
behavior to break — just build clean is enough. But removing a
*spelling/shim* like `indirect_br` changes what real parse output matches
against, so that one does need the `lowering` profile to actually confirm
nothing hits the removed path, same bar as any other behavior change.

## Phase 2.5: stop just using `Instruction` as a discriminant (session 6+)

Everything migrated so far (sessions 1-5) only used the typed `Instruction`
to *classify* the op, then immediately threw the typed fields away and
called the existing `lower_*(op: &Op)` helper, which re-derives everything
by hand from `op.operands`/`op.attr(...)` — including attrs the crate
already decoded into real enums (`CmpOpKind`, `MemOrder`,
`AtomicFetchKind`, `SyncScopeKind`, `FpClassFlags`, `AssumeBundleKind`,
`AsmFlavor`, ...). That was the right call while burning through ~90
variants for dispatch coverage, but it means slate is still carrying
hand-rolled duplicate decoders (e.g. `atomic.rs`'s `atomic_rmw_op(binop:
i64)` / `rust_ordering(mem_order: i64)` reimplement `AtomicFetchKind`/
`MemOrder` from raw ordinals, independently of and never verified against
the crate's own `try_from` impls) — worth retiring per family as this
phase proceeds, not just leaving as a permanent parallel-decode.

**New convention going forward, agreed with the user**: `lower_*` helpers
take the whole matched `Instruction` (by value — it's freshly constructed
per call by `clang_ir::model::instruction::lower_op(op)`, never borrowed,
so no extra clone from moving it) instead of a loose parameter list per
field. The dispatch arm becomes uniform regardless of arity:

```rust
instr @ Instruction::Rotate { .. } => {
    self.lower_rotate(instr);
    return;
}
```

and the helper re-narrows itself as the first line:

```rust
pub(super) fn lower_rotate(&mut self, instr: Instruction) {
    let Instruction::Rotate { result, ty, value, amount, left } = instr else {
        unreachable!()
    };
    ...
}
```

**Why this over destructuring at the call site and passing a loose field
list** (which is what `lower_rotate` looked like before this session, and
what `Cmp`/`Select`/`Load`/etc. still look like as of session 6 — not yet
converted): the field-list form makes every field name appear twice
(pattern + parameter list), which is fine for 2-field ops but genuinely
bad for the wide ones (`InlineAsm` has 8 fields, `AtomicCmpXchg` has 12).
The `unreachable!()` re-match isn't compiler-checked the way the
field-list form is — a copy-paste error wiring `Rotate`'s dispatch arm to
call `lower_shift` would compile fine under this convention and only
panic the first time a fixture hits it. Accepted anyway per the user:
every op family here has fixture coverage, so a dispatch/destructure
mismatch fails immediately and loudly, not silently — this codebase's
whole correctness model is differential fixture testing already (see
CLAUDE.md), not compile-time exhaustiveness, so this isn't introducing a
new category of risk, just moving where an existing kind of bug would
surface.

**Also motivates the eventual field-decoder cleanup above**: with the
whole `Instruction` in hand inside each `lower_*`, there's no reason left
to re-derive `kind`/`binop`/`mem_order`/etc. from raw attrs — the typed
fields are right there. Do that swap per family as you convert it to this
call convention, not as a separate pass — e.g. when converting
`lower_cmp`, replace its `attr_int(op, "kind")` raw-int match with the
`kind: CmpOpKind` field that's already on `Instruction::Cmp`, in the same
edit, and delete whatever local decode table becomes dead as a result.

**Bare/zero-payload ops aren't worth converting**: `Break`, `Continue`,
`Unreachable`, `Trap` (and any other unit-like variant) already take no
arguments — leave those as direct `self.lower_break()` etc. calls, no
`Instruction` plumbing needed.

**Done under this convention so far**: `Rotate` (`arithmetic.rs`), as the
reference example — converted per direct user request, one family, to
validate the pattern before doing the rest. `lower_shift` is the natural
next one (identical shape to `Rotate`: `Shift { result, ty, value,
amount, left }`, same `rotate_left`/`rotate_right`-style method-name
branch). After that, work through the rest of the already-migrated
dispatch arms (`Cmp`, `Select`, `Load`, `Store`, `Copy`, `Const`,
`GetGlobal`, `Cast`, `GetBitfield`, `SetBitfield`, `GetElement`,
`PtrStride`, `PtrDiff`, the three `*Overflow`s, and everything from
sessions 3-5) converting each to this shape — and while doing each one,
also fold in the "use the crate's decoded attr enum instead of the local
one" cleanup from the section above wherever that family has one.

**Where this doesn't apply cleanly**: the family-dispatch fns
(`lower_binary_family`, `lower_unary_family`, `lower_math_unary_family`)
already take destructured discriminant fields (`bop: BinaryOp`, `uop:
UnaryOp`, `kind: MathUnaryKind`) rather than a loose value list, because
they immediately re-dispatch to a *different* per-case helper rather than
doing the op's own lowering — that's a different shape (a second-level
dispatch, not a leaf lowering), so it doesn't need this treatment, only
the leaf `lower_*` functions those call into.
