# Goto Lowering

_created 2026-09-06_

How C `goto`/labels turn into Rust, end to end: three phases across two
sides of the pipeline. Phase 1 (CIR) decides whether a function needs
flattening at all. Phase 2 (frontend lowering) turns a flattened CFG into a
`loop { match __state { .. } }` dispatch. Phase 3 (backend rewrite engine)
tries to structure that dispatch loop back down into ordinary Rust control
flow — `if`/`else`, labeled loops, `break`/`continue` — wherever the CFG
shape allows it.

This is a sketch: accurate as of 2026-09, but the rewrite-engine stages
(structure_goto) are an active epic (slate-04q.85) and may grow more stages.
See [Rewrite engine v2](rewrite-engine-v2.md) for how `NodeRule`s and the
worklist fit together, and [switch-lowering.md](switch-lowering.md) for the
sibling `cir.switch`/Duff's-device path, which shares the same dispatch
machinery for backward jumps.

## Why this needs a state machine at all

Rust has no `goto`. CIR's high-level (structured) form keeps `cir.goto`
targeting `cir.label` inside single-block function bodies, which is exactly
what Clang's AST->CIR emission produces for a function containing a
`goto`. Slate's structured lowering path (`lower_block` walking one block's
op list) has nowhere to put a jump that isn't a loop `break`/`continue`.

So goto-bearing functions get flattened to a real multi-block CFG before
lowering ever sees them, and multi-block CFGs get lowered as an explicit
state machine: a `__state` variable and a `loop { match __state { N => ..,
.. } }`, where each match arm is one basic block and falls through by
writing the next state and `continue`-ing the loop. That's `lower_dispatch`
in `src/frontend/lowerer/control_flow.rs`, and it is the only place
`cir.goto`, `cir.br`, `cir.brcond`, `cir.switch.flat`, and indirect
gotos/branches lower to.

## Phase 1: does this function need flattening? (`src/frontend/cir_input.rs`)

`emit_module` calls `emit_generic_with_args` once (structured CIR, no
flattening) and inspects each `cir.func` in the parsed module:

- `needs_goto_flattening` — the func body contains a `cir.goto` anywhere,
  nested regions included (`contains_goto`, recursive over regions/blocks).
- `needs_indirect_goto_flattening` — contains `cir.indirect_goto`
  (computed goto, `goto *ptr`).
- `needs_asm_goto_flattening` — a single-block function with no `cir.goto`
  but a nested `cir.label` inside some child region (e.g. inside a
  statement-expression) _and_ a `cir.asm` whose template has a `:l}`
  (asm-goto output) constraint.

Only functions matching one of these get re-emitted with `cir-opt` flags
that flatten the CFG, and only those functions are spliced back into the
module (`merge_flattened_functions`, matched by `sym_name`) — see the
doc comment on `emit_generic_with_args_flattened` for why this isn't done
unconditionally: flattening a goto-free function still produces a correct
CFG, but it downgrades native Rust loops/ifs to the uglier dispatch form,
so straight-line and structured functions are left as CIR emits them.

Two flag sets matter:

- `--cir-flatten-cfg --cir-goto-solver` (`emit_generic_with_args_flattened`)
  — used for real `cir.goto` and indirect-goto functions. The goto solver
  resolves `cir.goto`/`cir.label` into real `cir.br` edges between blocks.
- `--cir-flatten-cfg` alone (`emit_generic_with_args_cfg_flattened`) — used
  for nested asm-goto. Running the goto solver here would consume the
  `cir.label` ops that the AST-level asm-goto target resolution
  (`c_ast.rs`, matching `GCCAsmStmt` label operands to their `SourcePoint`)
  needs to still exist; see the 2026-08-30 log entry
  (`wiki/log/2026-08-30-16-23.md`) on why label identity has to survive to
  be joined against Clang AST source points.

There's also a direct-emit fallback: if plain `emit_generic_with_args`
fails with clang's "does not dominate this use" (an SSA value defined in one
structured region used somewhere that only dominates it once the CFG is
flattened), the whole module is re-emitted flattened+goto-solved rather than
per-function.

Indirect goto gets one more wrinkle upstream of all this: `slate-hos2.4`
bypasses `mem2reg` specifically for functions containing `cir.indirect_goto`
(see `wiki/log/2026-08-31-13-27.md`), because mem2reg introduces `undef` for
predecessors reachable only through a dynamic (computed) jump target — the
affected function is flattened without mem2reg while the rest of the module
keeps it.

## Phase 2: `lower_dispatch` (`src/frontend/lowerer/control_flow.rs`)

Triggered in `function_setup.rs` whenever a function body has more than one
block after phase 1 — that's the signal a flattening pass actually fired,
since single-block bodies never reach here otherwise. It:

1. Assigns each block an integer state (`block_to_state`), and additionally
   maps every `cir.label` name to the state of the block that defines it
   (`label_to_state`) — multiple labels can point at the same block.
2. Hoists every `cir.alloca` in the flattened body up front, before any arm,
   because a `let` inside one match arm wouldn't be visible from another
   arm on a later loop iteration.
3. Hoists cross-block-live SSA values (`cross_block_live_values`) into
   `let mut` temps for the same reason — a value defined in block N and
   used in block M can't stay a Rust binding scoped to block N's arm.
4. Materializes block arguments (CIR's SSA-with-block-params form) as
   `let mut` temps that predecessor blocks assign right before jumping,
   since Rust match arms don't take parameters.
5. Lowers each block's ops into one `match` arm body; a non-diverging arm
   gets `__state = i+1; continue 'dispatch` appended so fallthrough between
   adjacent blocks is explicit.
6. Wraps it all in `let mut __state = 0; 'dispatch: loop { match __state {
.. _ => break/unreachable!() } }`.

The terminator lowerers that only make sense inside this context —
`lower_goto`, `lower_br`, `lower_brcond`, `lower_switch_flat`,
`lower_indirect_br`/`lower_indirect_goto` — all read `self.dispatch`
(`DispatchCtx`, holding the loop label, state variable name, and the two
label/block maps) and turn a jump into `__state = <target>; continue
'dispatch`. `lower_goto` outside a dispatch context (`self.dispatch` is
`None`, i.e. structured lowering hit a bare `cir.goto`) is a no-op —
that path shouldn't be reachable given phase 1's flattening, and would
indicate a function that needed flattening but didn't get it.

`cir.brcond` and `cir.switch.flat` lower to an `if`/`match` that assigns
`__state` in each branch and then falls through to one shared
`continue 'dispatch` after — not per-branch continues — since every branch
target is a state in the same loop.

Indirect targets (`lower_indirect_target`) look up a precomputed
`indirect_target_values` map (address-of-label constant -> state literal
expression) built during earlier lowering of `cir.indirect_goto`'s operand;
if the address isn't a recognized label constant, it lowers to
`unreachable!()` rather than guessing.

At this point every goto-bearing function is correct: a plain `loop {
match .. }` state machine, differential-testable, but far from what a human
would write for e.g. a single forward `goto` past an `if`.

## Phase 3: structuring the dispatch loop back down

This is the `structure_goto` rewrite-engine module
(`src/backend/engine/rules/structure_goto.rs` +
`structure_goto/reducible.rs`), an epic staged as slate-04q.85. Each stage
is a `NodeRule` matching on the `'dispatch: loop { match __state { .. } }`
shape (`parse` in `structure_goto.rs` recognizes it structurally by label
prefix `__dispatch`/`__state`, not by name tracking).

**Stage 1 — normalize** (`StructureGoto`, priority 1). Two cleanups before
anything structural: jump-thread arms whose entire body is `__state = M;
continue` (pure forwarding — `thread` follows the forwarding chain, giving
up and returning the start state if it hits a cycle, since a forwarding
cycle is just `for(;;);` written with goto and threading it further buys
nothing), and drop arms unreachable from the (possibly-threaded) entry
state via a DFS over `successors`. Remaining states are compacted to a
dense `0..n` range. This alone shrank arm counts 2-4x across the
`goto_*` fixtures (see `wiki/log/2026-09-03-21-23.md`) before any of the
harder stages ran.

**Stage 2 — acyclic collapse** (part of `StructureReducible`, in
`reducible.rs`). If the normalized state graph has no back edges at all,
the dispatch loop is redundant — every state runs at most once. This
deletes it entirely and re-derives structured Rust from the DAG: parse each
arm into a prefix plus one terminator (`Term::Diverge` / `Jump` / `Branch` /
`Switch`), compute reverse postorder + Cooper-Harvey-Kennedy dominators,
and recursively emit — a successor with exactly one predecessor is inlined
directly, a join node (multiple predecessors) is wrapped in a labeled block
at its immediate dominator and reached from elsewhere via
`break '__joinN_k`. Every node emits exactly once (no tail duplication).
Divergence (needed to know an arm's control never falls to the bottom and
re-enters a state) must be _proven_ — `return`, `std::process::exit`,
`std::process::abort`, `unreachable!()`, `panic!()`, recursing through
`if`/`match`/`unsafe` — anything else bails rather than assumes.

**Stage 3 — natural loops** (also in `reducible.rs`). Cyclic but reducible
graphs: find natural loops per back edge (`natural_loop`, header + tails)
and emit them as real Rust loops around the same dominator-tree emission
from stage 2, so a simple `goto`-written `while`/`for` becomes a labeled
Rust loop instead of staying a whole-function dispatch.

**Stage 4 — irreducible fallback** (`reducible.rs`, `slate-04q.85.6`).
Genuinely irreducible control flow (an SCC with two or more entries — a
member with a predecessor from outside the SCC) can't be emitted with
stage 2/3's dominator-tree machinery directly. Rather than a second
emitter, the graph is made reducible: for each such SCC, insert one
synthetic header block terminated by `Switch(__state, entries)`, then
redirect _every_ edge into an entry (internal edges too, not just
external ones — an entry reached from two directions inside the SCC is
still a problem) through a fresh **per-edge** trampoline block that sets
`__state` and jumps to the header. Per-edge (not per-target) matters: a
trampoline shared across edges into the same entry would itself be
reachable from outside the SCC, so the header still wouldn't dominate it
and the SCC would stay irreducible — verified concretely on
`goto_irreducible` before committing to the approach. Once the header
dominates the whole SCC, its cycles have real back edges and stage 3's
loop emission runs unchanged, scoped just to that SCC with ordinary
straight-line prologue/epilogue around it. If the graph's _entry itself_
lands inside an irreducible SCC, the synthetic header becomes the new
function entry with a `__state = <original entry>` preamble.

**What still bails to a whole-function dispatch loop.** Any function whose
`__state` is assigned a non-literal value (i.e. the target isn't statically
known — this is exactly computed/indirect goto's jump-table pattern) is
left alone by every stage; `computed_goto_ops.c` and
`mem2reg_computed_goto.c` are untouched by design, not oversight. A
forwarding cycle in stage 1's `thread` also gives up rather than risk
collapsing two arms onto one state. That's stage 4's successor problem, not
solved here.

## Where to look for each half

| Question                                                                                 | File                                                                        |
| ---------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| Does this function need CFG flattening, and with which flags?                            | `src/frontend/cir_input.rs`                                                 |
| How does `cir.goto`/`cir.br`/`cir.brcond`/`cir.switch.flat` become `__state`/`continue`? | `src/frontend/lowerer/control_flow.rs` (`lower_dispatch` and friends)       |
| Why is mem2reg skipped for computed goto?                                                | `wiki/log/2026-08-31-13-27.md`                                              |
| How does asm-goto keep label identity through flattening?                                | `wiki/log/2026-08-30-16-23.md`, `src/frontend/c_ast.rs`                     |
| How is a dispatch loop parsed/normalized/dismantled after lowering?                      | `src/backend/engine/rules/structure_goto.rs`, `structure_goto/reducible.rs` |
| Fixture coverage for each shape                                                          | `tests/fixtures/goto_*.c`, `computed_goto_ops.c`, `mem2reg_computed_goto.c` |

## Related

- [Rewrite engine v2](rewrite-engine-v2.md) — the `NodeRule`/worklist
  machinery `structure_goto` is built on.
- [switch-lowering.md](switch-lowering.md) — `cir.switch` and Duff's device,
  which reuse `lower_dispatch`'s state-loop shape for backward jumps into a
  switch body.
- [passes.md](passes.md) — where `structure_goto`'s stages sit in the full
  pass catalog and ordering.
