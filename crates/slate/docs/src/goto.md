# Goto

Rust has no general `goto`, so Slate lowers goto-bearing C functions through a
temporary control-flow state machine and then recovers structured Rust when
the graph permits it.

## Flattening and lowering

`src/frontend/cir_input.rs` leaves ordinary structured functions in their
native CIR form. Functions containing direct or indirect gotos are selectively
re-emitted with CFG flattening; nested asm-goto uses CFG flattening without the
goto solver so its labels remain available to the AST source-location join.

`FunctionLowerer::lower_dispatch` (`src/frontend/lowerer/control_flow.rs`)
turns the flattened CFG into:

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

Each arm is one basic block. Cross-block values and block arguments are
hoisted into mutable locals, and branches assign the next state before
continuing the dispatch loop. This form is always correct, including for
computed goto and irreducible control flow.

## Structure recovery

`structure_goto` in the worklist engine handles literal-state dispatch loops
in four stages: it threads forwarding arms and drops unreachable states; it
collapses acyclic graphs into structured branches; it emits natural loops with
real `loop`/`break`/`continue`; and it makes irreducible SCCs reducible by
inserting per-edge trampolines and a scoped dispatch header. Computed goto and
other non-literal state assignments remain as whole-function dispatch loops.

A graph shape that cannot be proven safe is left in the baseline dispatch form.

## Switch

Rust's `match` does not fall through between arms, so `lower_switch` uses a
separate state variable and labeled loop. A case without an explicit `break`
assigns the next case index and continues the loop. `structure_dispatch`
recovers this deterministic trampoline into a Rust `match` when its shape is
unchanged; goto-shaped switches use `structure_goto` instead.
