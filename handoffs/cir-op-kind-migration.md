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
