# Transforms

Transforms simplify typed C IR before [fact analysis](writing-a-query.md) and
Rust lowering. Use `emit-slate-ir` to see what a pass changes.

```bash
slate emit-slate-ir input.c
slate emit-slate-ir --passes=const-fold input.c
slate translate input.c
```

The first command prints IR with no optional transforms. Use a comma-separated
pass list or `--passes=all` to inspect more than one pass. The Rust examples
below show the resulting shapes with signatures and ABI details omitted. Later
passes and Rust rewrites can simplify them further.

## What the macro slots see

Sema emits the expanded C program. It also records which statement regions came
from a macro expansion and which values came from the macro body or its
arguments. Slate uses that structure to decide which expansions it can emit as
Rust macro calls.

- **Before macro keep.** All recorded macro regions are still present.
  `inline-temps` runs here so the keep decision sees eligible temporary chains
  already simplified. A temporary move must respect expansion boundaries and
  argument holes.
- **After macro strip.** Slate removes the wrappers for expansions it will emit
  inline. Their statements and values are now ordinary IR and can be rewritten.
  Wrappers and argument holes for kept expansions still need to match the Rust
  macro template. `inline-temps` runs again for code exposed by stripping.
- **After transforms.** Fact analysis reads the resulting IR, so lowering
  receives facts about the bindings and values that survived the passes.

`PASSES` in `crates/slate-analysis/src/transform/mod.rs` defines the order
within each slot. The sections below follow the order after macro strip.

## structure-goto

C `if (bad) goto done; work(); done: finish();` can lower to:

```rust
'done: {
    if bad != 0 {
        break 'done;
    }
    work();
}
finish();
```

A forward jump over a statement range becomes a break from the block enclosing
that range. A backward jump to the start of a range becomes a continue in a
loop. A sole trailing `if c { goto L }` can become the test of a do-while loop.

The pass needs to account for every edge into the region. Cleanup or VLA
declarations and switch cases owned by an enclosing switch can block a rewrite.
A function with computed gotos, label addresses, or asm goto stays out of this
pass. Lowering handles residual jumps with dispatch.

## truth-canonicalize

C `return !(a < b);` for integer operands can lower to
`return (a >= b) as i32;`. The comparison stays boolean until the return
converts it to the C result type.

In IR, `ne(from_bool(b), 0) → b`, `eq(from_bool(b), 0) → not(b)`, and
`not(not(b)) → b`. A conditional with arms `1` and `0` becomes
`from_bool(condition)`. When sema uses a synthetic boolean temporary for
`a || f()`, the pass can recover `a != 0 || f() != 0`, keeping the call on the
right of the short circuit.

Negated equality can flip even for floats. Negated ordered float comparisons
stay as written because NaN makes `!(a < b)` differ from `a >= b`. Explicit
conversions and comparisons and protected macro values can block the rewrite.

## inline-temps

For C `n--; return n;`, sema can save the old value of `n` in a synthetic
temporary for the decrement. Rust can use `n -= 1; return n;` once the
subtraction reads `n` directly.

`temp t = E; ...; use(read(t)) → ...; use(E)` moves the same initializer to its
sole read. The types must agree, and the read must be in the next 32 sibling
statements or the list's tail value. Multiple reads, captures, macro argument
holes, nested statement bodies, and opaque control flow block the move. The
producer's effects must commute with intervening evaluation. An effectful
producer must also stay out of a conditional arm or short circuit operand that
might never run.

## const-fold

C `signed char c = 200;` on the usual eight-bit target can lower to
`let c: i8 = -56;`.

An implicit integer conversion of a constant becomes a constant of the
destination type. For a narrowing to eight bits, `200 mod 256 = 200`, whose
signed interpretation is `200 - 256 = -56`. The pass also folds signed negation
of literals and eligible comparisons of integer constants.

Explicit casts, general arithmetic such as `60 * 60`, and shifts stay in the IR.
A kept expansion or wrapped macro value can also block folding.

## do-while-zero

C `do { if (stop) break; work(); } while (0);` can lower to a Rust labeled block
with `break 'once` in the conditional arm.

`do B while false → block B` runs the body once. If the body breaks or continues
to that loop, the pass emits a labeled block and makes either jump an exit from
the block. A continue would have reached the false test and exited anyway. Jumps
targeting inner loops keep their targets.

The test must have no evaluation statements and must be false as a boolean
constant or a recognized constant integer comparison. Kept macro expansions
retain their loop. The pass can simplify the same loop from a macro that Slate
chose to emit inline.

## labeled-blocks

C `if (c) goto other; f(); goto done; other: g(); done: return;` can lower to
`if c != 0 { g(); } else { f(); }` followed by the function exit.

After goto structuring, `if c { A; break L }; B; break L` can become
`if c { A } else { B }; break L`. Both arms reach the same exit. The pass can
also move a tail into the sole break that reaches it, remove redundant blocks,
and recover switch arms from labeled control flow.

A shared tail stays in place when moving it would require duplication. The pass
keeps early continues and rejects tail moves that would change name resolution
or cross a cleanup, VLA, or SEH scope.

## dead-temporaries

For C `c ? f() : g();`, Rust can emit `if c != 0 { f(); } else { g(); }` without
a local for the unused result.

For an unread synthetic scalar `t`, `write(t, E) → eval(E)` and the declaration
disappears. Any initializer also becomes an evaluation. This preserves both
which call runs and where it runs.

A read of the result, an indirect use, or a type capture such as a VLA extent
keeps the storage. The pass keeps source locals and storage inside retained
macro regions.

## decl-init

C `int x; x = compute(); return x;` can lower to `let x = compute(); return x;`.

`let x; ...; write(x, E) → ...; let x = E` sinks the declaration to its first
ordinary write without moving `E`. For an adjacent conditional whose two arms
end in a write to `x`, it can build one conditional initializer and keep each
arm's preceding statements inside that arm.

An explicit initializer, an earlier read or address use, or a control flow or
lifetime boundary that prevents sinking keeps the declaration separate. For the
conditional case, its test and arm prefixes must leave `x` untouched.

## truth-locals

C `int found = 0; if (hit) found = 1; if (found) report();` can lower to
`let mut found = false;`, an assignment of `true`, and `if found`.

When every definition of an automatic integer is proven to store 0 or 1, the
pass can store that value as a boolean. It replaces `x != 0` with `x` and
`x == 0` with `!x`. An integer read converts the boolean back to the original
integer type. Copies between eligible locals carry the proof.

Truth uses must outnumber integer reads. An unknown definition, a stored value
outside 0 and 1, an address use, a capture, a volatile access, or a reference
inside a macro or OpenMP region keeps the integer representation.

## Add a transform fixture

Put a small C input in `crates/slate/tests/fixtures.transforms/<passes>/`. The
directory name selects the passes for the generated IR checks. Include an
example that rewrites and one that reaches the guard you want to preserve. Use a
Slate differential fixture when you need to check the Rust output or runtime
behavior. See [Testing](testing.md).
