# Goto

C supports a lot of structured programming structures, like if, else,
for, while, etc.

`goto` is the exception. Rust only has goto to break out of loops, but
otherwise can't lower it. Thankfully there's a theorem called the
structured programming theorem, that states any unstructured program can
be represented with structured programming constructs, which basically
turns any goto with labels into an interpreter.

## How to flatten

`src/cir/flatten.rs` decides per function, before anything else runs,
whether it needs the flattened form at all. A function is left in its
structured, region-based form unless it's still single-block _and_ contains
a `cir.goto` (`needs_flattening`) functions without `goto`, and functions
CIR already emits as multi-block on their own (computed goto, `asm goto`,
functions with top-level labels), never pay for the extra `cir-opt
--cir-flatten-cfg --cir-goto-solver` invocation or the state-machine
lowering path at all. Only the functions that actually need it get
re-parsed in flattened form and spliced back into the module.

## The dispatch loop

`FunctionLowerer::lower_dispatch` (`control_flow.rs`) turns a flattened
function body into:

```rust
let mut __state0: i32 = 0;
'__dispatch0: loop {
    match __state0 {
        0 => { /* block 0 */ __state0 = 1; continue '__dispatch0; }
        1 => { /* block 1 */ ... }
        _ => { break '__dispatch0; }
    }
}
```

one match arm per basic block, indexed by an integer state variable. Every
alloca in the function is hoisted to the top (so it's visible from every
arm), and any SSA value live across a block boundary is promoted to a
`let mut` for the same reason (`cross_block_live_values` in `analysis.rs`).
`goto`/`br` become `__stateN = <target>; continue`; `brcond` becomes an
`if`/`else` choosing between two such assignments; `switch.flat` becomes a
nested match picking the target state; computed goto and `indirect_br`
assign a runtime-computed state instead of a constant one, which is why
those functions are marked `dynamic` and never eligible for restructuring
(below). This shape can represent _any_ control-flow graph, including
irreducible ones that's the whole point of going through it at the cost
of being unstructured, table-driven Rust instead of `if`/`loop`.

`tests/fixtures/goto_irreducible.c` is an example that has no
recoverable structure:

```c
if (choose_b) goto b;
a: x = x + 1; if (x < 3) goto b; goto done;
b: x = x + 2; if (x < 4) goto a;
done: printf("%d\n", x);
```

`a` and `b` jump into each other, and the entry point can land on either one
first (from the initial `if`, or by falling through from the top) a
two-node cycle with two distinct entries, the textbook irreducible graph.

## Recovering structure

Most real `goto` usage isn't actually irreducible. It's a disguised
`break`, `continue`, or early-exit that CIR only flattened because it
couldn't tell. `src/backend/facts/goto.rs` parses the dispatch-loop shape
back out of the _lowered_ Rust AST (`DispatchLoop`, one `DispatchState` per
arm, each ending in a `Transfer::{Goto, Branch, Switch, Return, Diverge}`)
and classifies it with the same graph tools a relooper would use
dominators, natural loops, strongly-connected components
(`is_reducible`, `cyclic_sccs`). `src/backend/query/control_flow.rs` then
tries, in order:

- acyclic plain `if`/`else`/sequence, no loop at all;
- a single self-loop `loop { ..; if !cond { break } }`, i.e. recovering
  a `while`;
- a single irreducible cycle localized within an otherwise-structured
  function a smaller `loop { match __blockN }` peeled down to just the
  cycle, with everything before and after it fully structured;
- anything else a genuinely irreducible whole function like
  `goto_irreducible.c`, or a `dynamic` state assignment from computed goto
  stays exactly as the dispatch loop `lower_dispatch` produced. That's still
  correct; it's just not idiomatic.

This runs as `Pass::Goto`, the first fixup pass in the pipeline (see
[Rewriting](./writing-a-rewrite.md)), so later passes only ever have to deal
with whichever of those four shapes the function ended up in. Like every
fixup, it's independently verified by differential testing and optional in
spirit disabling it just leaves more functions as dispatch loops.

## Switch

Rust's `match` doesn't fall through between arms, but C's `switch` does, so
`lower_switch` builds its own miniature version of the same trick: a
`__switch_caseN` integer plus a labeled `loop { match __switch_caseN { .. } }`,
where a case without an explicit `break` bumps the index and `continue`s
into the next arm instead of falling through the match. For
`tests/fixtures/switch_fallthrough.c`'s `case 1:` (no `break`, falls into
`case 2:`), the lowered arm for state `0` ends with
`__switch_case0 = 1; continue '__switch0;` instead of `break`.

Duff's device a `switch` whose body is a single `do`/`while` with
`case` labels interleaved inside the loop body doesn't fit that shape at
all (the cases aren't parallel alternatives; they're entry points into
_different offsets of the same loop iteration_), so it's detected
separately (`duff_switch` in `cir_ops.rs`) and lowered by
`lower_duff_switch` into a cascade of `if __switch_case0 <= K { .. }` guards
inside the `do`-loop body one per case, in original order, each falling
through into the next followed by resetting the index to `0` and
re-checking the loop condition. This reproduces the "jump into the middle of
a loop and fall through the rest of it" behavior Duff's device depends on
without needing `goto` in the output at all.
