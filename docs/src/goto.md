# Goto

C supports a lot of structured programming structures, like if, else,
for, while, etc.

`goto` is the exception. Rust only has goto to break out of loops, but
otherwise can't lower it. Thankfully there's a theorem called the
structured programming theorem, that states any unstructured program can
be represented with structured programming constructs, which basically
turns any goto with labels into an interpreter.

## How to flatten

`src/cir/flatten.rs` decides per function whether it needs the flattened form at all.
A function is left in its structured, region-based form unless it's still
single-block _and_ contains a `cir.goto` (`needs_flattening`).
Functions without `goto`, and functions
CIR already emits as multi-block on their own (computed goto, `asm goto`,
functions with top-level labels), never pay for the extra `cir-opt
--cir-flatten-cfg --cir-goto-solver` invocation or the state-machine
lowering path at all. Only the functions that actually need it get
re-parsed in flattened form.

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

which basically interprets an unstructured program with structured
programming constructs. The con is that this is fairly inefficient (not
to mention ugly). We'll talk about how to fix that in the next section,
but note that not all gotos can be recovered into nicer if/else
structure.

Look at `tests/fixtures/goto_irreducible.c`, which is like this:

```c
if (choose_b) goto b;
a: x = x + 1; if (x < 3) goto b; goto done;
b: x = x + 2; if (x < 4) goto a;
done: printf("%d\n", x);
```

Draw it out:

```
        +---------+ entry +---------+
        |                           |
   !choose_b                    choose_b
        |                           |
        v                           v
  +-----------+   x<3          +-----------+
  |     a     | -------------> |     b     |
  | x = x + 1 |                | x = x + 2 |
  +-----------+ <------------- +-----------+
        |            x<4             |
        |                            |
     (else)                       (else)
        |                            |
        v                            v
        +---------> done <----------+
```

Since neither `a` nor `b` dominates the other, there's no single loop
header a relooper could wrap a `while`/`loop` around while keeping the
cycle's stack usage at O(1).

## Recovering structure

Thankfully, most real `goto` usage can be repaired.
`goto`s that are like `break`, `continue`, or early-exits can be
recovered into structured programming constructs.

Slate uses a relooper algorithm to check:

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
with whichever of those four shapes the function ended up in.

## Switch

Rust's `match` doesn't fall through between arms, but C's `switch` does, so
`lower_switch` builds its own miniature version of the same trick: a
`__switch_caseN` integer plus a labeled `loop { match __switch_caseN { .. } }`,
where a case without an explicit `break` bumps the index and `continue`s
into the next arm instead of falling through the match. For
`tests/fixtures/switch_fallthrough.c`'s `case 1:` (no `break`, falls into
`case 2:`), the lowered arm for state `0` ends with
`__switch_case0 = 1; continue '__switch0;` instead of `break`.
