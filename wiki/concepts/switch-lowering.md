# Switch lowering and recovery

How `cir.switch` becomes Rust, in two stages that don't share a code path:
`lower_switch` (`src/frontend/lowerer/control_flow.rs:128`) always emits one
mechanical trampoline shape, and `structure_dispatch`
(`src/backend/engine/rules/structure_dispatch.rs`) recovers an idiomatic
`match` from exactly that shape in the rewrite engine
([rewrite-engine-v2.md](rewrite-engine-v2.md)). Goto-shaped switches (`cir.switch.flat`
plus irreducible control flow) are a separate rule, `structure_goto`, and never
reach `structure_dispatch`.

## The CIR shape

`cir.switch`'s body is a region containing `cir.case` ops, each carrying a
`kind` (`Equal`, `Range`, `Default`) and either scalar values or a
`[start, end]` pair, plus its own nested region for the case body. Nothing
in CIR guarantees the cases are non-overlapping in source order or that a
case body ends before the next one starts — C fallthrough means a case
region can simply not terminate, and control flows into whatever text
follows it in the original switch body.

Two case-value encodings need special handling, both in `cir_ops.rs`:

- **`_BitInt` selectors** can't use range patterns at all — rustc restricts
  `a..=b` in pattern position to `char` and the built-in numeric primitives
  (E0029), and no trait impl on the crate's `BInt`/`BUint` types opens that
  door. Every bitint case becomes a _guard_ instead of a pattern:
  `__switch_sel if __switch_sel == const { BInt::<N>::from_decimal_str("...") }`.
  The `const { }` block matters on its own — `from_decimal_str` is `const fn`,
  and without wrapping the call, a naive guard would re-parse the decimal
  string from scratch on every switch evaluation at runtime.
- **`u128` values that don't fit `i128`** fall back to `Pattern::U128`/
  `Pattern::InclusiveRangeU128` — CIR's constant attributes are signed by
  default, so an unsigned value larger than `i128::MAX` has to be recovered
  from its decimal digit string rather than trusted as a signed int.

## Lowering: one shape, no cleverness

`lower_switch` doesn't attempt to reconstruct fallthrough as a CFG. It
can't, prior to the rewrite engine's arena existing — at lowering time there's
nothing yet to run a graph analysis over, and being conservative here is what
keeps lowering simple. Instead it always emits the same trampoline:

```rust
{
    let __switch_value0 = <selector>;
    let mut __switch_case0 = match __switch_value0 { <pat> => 0, <pat> => 1, ..., _ => <default index or -1> };
    '__switch0: loop {
        match __switch_case0 {
            0 => { <case 0 body>; __switch_case0 = 1; continue '__switch0; }
            1 => { <case 1 body>; break '__switch0; }
            ...
            _ => break '__switch0,
        }
    }
}
```

The selector is evaluated once into an integer "which case index" via one
`match`, and the `loop` + reassign-and-`continue` pattern is what encodes
fallthrough — falling off the end of arm N's body sets the state variable to
`N+1` and loops back around, rather than duplicating code or jumping directly.
This is deliberately the _only_ shape `lower_switch` produces: every
fallthrough edge in its output is `0 -> 1 -> ... -> N-1`, always in tail
position (the reassignment is always the statement right before the
`continue`), never conditional, never cyclic. That determinism is what makes
the rewrite side possible without a general CFG reconstruction — see below.

## The Duff's-device exception

One structural shape gets special-cased in lowering rather than left to the
trampoline: `duff_switch` (`cir_ops.rs:120`) detects a `switch` whose entire
body is a single `do { ... } while (cond)` loop, where the switch's own case
dispatches directly into a case inside that loop body (classic
[Duff's device](https://en.wikipedia.org/wiki/Duff%27s_device)). Structurally
this is _also_ just fallthrough, but fallthrough into a loop rather than
between straight-line arms — the trampoline shape above can't express jumping
into the middle of a Rust `loop`. Detecting it needs a specific pattern match
(single case in a region, whose body is a bare `do`, whose body starts with
case labels), which is where `duff_switch` and `lower_duff_switch`
(`control_flow.rs:232`) earn their keep: they translate it directly into a
bounded `if index <= N { ... }` cascade inside the loop, one `if` per case
threshold, instead of forcing a general jump target through the trampoline's
state machine.

## Rewrite: recovering `match` without a fallback path

`structure_dispatch` runs in the arena-based rewrite engine
([rewrite-engine-v2.md](rewrite-engine-v2.md)) and turns the trampoline back
into a plain `match`. The key move, recorded in
`wiki/log/2026-09-02-12-27.md`, is noticing that because `lower_switch` only
ever emits one shape, recovery is **shape verification, not graph analysis**:
`parse()` walks the scope/let/loop/match structure the trampoline is known to
have, confirms every fallthrough edge really is the tail-position
`assign-then-continue` pattern lowering guarantees, and bails (leaving the
trampoline untouched — a safe no-op, not a miscompile) the moment anything
doesn't match. There is no fallback interpretation to reach for, because
there is no second shape to fall back to.

Per arm, `parse` records three things: the source patterns (inverted out of
the selector `match`), the arm's stripped body, and a terminator —
`Exit` (breaks the outer label), `FallsThrough` (assigns `index+1` and
continues), or `Diverges` (anything else, e.g. `return`/`goto`/an infinite
loop). `rebuild()` then does the actual recovery, entirely arm-local:

- **Empty fallthrough arms fold into an or-pattern.** `case 3: case 4: foo();`
  lowers to two trampoline arms, one empty and falling through to the next;
  `rebuild` merges the empty arm's patterns onto its successor, recovering
  `3 | 4 => foo()`.
- **Non-empty fallthrough chains are tail-duplicated.** Where C fallthrough
  actually shares code between arms, Rust `match` has no equivalent — the
  body has to be copied into every arm that reaches it, gated at 24
  duplicated statements (`MAX_DUPLICATED_STMTS`) so a long shared tail just
  leaves the trampoline alone rather than blowing up the output. That count
  is taken at `structure_dispatch`'s own priority (1), which runs before
  `compound_assign`/`inline_temps` (priority 41-70) fold raw arena statements
  back into source shape — so it has to skip compiler-temp `let _vN = ...`
  bindings explicitly (`is_compiler_temp_let`) rather than counting
  `body.len()` directly. Counting the raw form meant `out += 1;` cost 4
  toward the budget (`let _v=1; let _v=out; let _v=_v+_v; out=_v;`) instead
  of 1, and switches with only a handful of real duplicated source lines —
  grouped fallthrough arms plus one non-tail `if (...) break;` was enough —
  blew the budget roughly 4x faster than intended and silently kept the
  trampoline. See `wiki/log/2026-09-02-20-41.md` and
  `tests/fixtures/switch_torture.c`.
- **The default arm moves to the end unconditionally.** Case _values_ are
  disjoint by construction, so an arm's dispatch index never depends on
  where `default:` sat in the source — `default:` in the middle of a switch
  recovers correctly with no special case, and `case 2: default:` sharing a
  label folds into the wildcard arm the same way any other shared label does.
- **The label survives only if something still needs it.** `scan()` checks
  whether any arm contains a non-tail `break '__switchN` (a `break` inside a
  nested `if`, say) — if so the recovered `match` is wrapped in
  `'__switchN: { match ... }`; if every exit was already in tail position,
  the label is dropped entirely and the output is a bare `match`.

One correctness bug already surfaced and got fixed here
(`wiki/log/2026-09-02-12-40.md`): naive tail duplication copied a
fallthrough arm's `let _vN = ...` temp declarations verbatim into both arms,
and `inline_temps` refuses to touch a name that isn't declared exactly once
in the function — so the duplicated copy kept every raw temp while the
original arm got cleaned up, and one arm of the recovered `match` read
noticeably worse than the other. Not a miscompile (conservative rules don't
silently break code), but visibly asymmetric output. The fix renames every
`_vN` in the copy to a fresh index before splicing it in
(`max_temp`/`rename_decls`/`freshen`), so both arms go through the same
later cleanup passes identically.

What this rule explicitly does **not** attempt: recovering `c2rust`-style
shared-tail emission (`'s_34: { match ...; shared_tail }`) once the
duplication gate is exceeded — past that gate the trampoline is left as-is,
correct but unrecovered, rather than reached for a second, riskier shape.
Goto-shaped switches — anything that needed `cir.switch.flat` or the
lowerer's irreducible-control-flow state-machine fallback
(`lowerer-internals.md`'s `lower_dispatch`) — never reach this rule at all;
they're `structure_goto`'s problem, kept on a fully separate code path so
the two recovery strategies never have to agree on a shared intermediate
shape.

## Related

- [rewrite-engine-v2.md](rewrite-engine-v2.md) — the arena/worklist engine
  `structure_dispatch` runs in.
- [lowerer-internals.md](lowerer-internals.md) — `control_flow.rs`'s role in
  the lowerer module split, and the state-machine fallback for irreducible
  control flow that goto-shaped switches fall into instead.
- `wiki/log/2026-09-02-12-27.md` — the session that shipped fallthrough
  recovery via arm-path folding.
- `wiki/log/2026-09-02-12-40.md` — the duplicated-temp bug and fix.
- `wiki/log/2026-09-02-14-51.md` — why bitint cases are guards with inline
  `const { }` blocks, not hoisted named consts.
