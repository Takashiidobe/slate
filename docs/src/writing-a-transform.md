# Transforms

Transforms simplify typed C IR before [fact analysis](writing-a-query.md) and
Rust lowering. Use `emit-slate-ir` to see what a pass changes.

```bash
slate emit-slate-ir input.c
slate emit-slate-ir --passes=const-fold input.c
slate translate input.c
```

The first command prints IR with no optional transforms. Use a comma-separated
pass list or `--passes=all` to inspect more than one pass. Each Rust block below
shows the function emitted by `slate translate`, which runs the full pipeline.
The rewrite blocks isolate the named pass and use shorthand IR. Uppercase
letters stand for expressions or statement lists, and `continue L` and `break L`
name their exact targets.

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

### Purpose

Replace direct gotos with labeled blocks and loops where the jump targets fit
structured control flow.

### Example

**C input**

```c
int run(int skip, int x) {
    if (skip)
        goto done;
    x += 1;
done:
    return x * 2;
}
```

**Emitted Rust**

```rust
fn run(skip: i32, mut x: i32) -> i32 {
    if skip == 0 {
        x += 1;
    }
    x * 2
}
```

### Rewrite rules

```text
if C { goto L }; A; L: B  →  block K { if C { break K }; A }; B
L: A; if C { goto L }; B  →  do { A } while C; B
```

For a forward edge, `K` is fresh and the block ends exactly where `L` began. For
a backward edge, the second rule requires a single trailing conditional jump to
`L`. The general backward rule wraps the range in a loop, turns its back edges
into continues, and adds an exit where the original range fell through. In the
example, later passes turn the forward block into an `if`.

### Caveats

Account for every edge to the label before replacing it. A jump into the middle
from outside the candidate range can bypass the new structured entry. The pass
also checks for VLA or cleanup declarations and case labels owned by an outer
switch. An address such as `&&L` makes the function ineligible because the label
is also a value. Residual jumps go to dispatch during lowering.

## truth-canonicalize

### Purpose

Remove redundant truth tests and recover short-circuit expressions from boolean
IR.

### Example

**C input**

```c
int inverted(int a, int b) {
    return !(a < b);
}
```

**Emitted Rust**

```rust
fn inverted(a: i32, b: i32) -> i32 {
    (a >= b) as i32
}
```

### Rewrite rules

```text
ne(from_bool(b), 0)            → b
eq(from_bool(b), 0)            → not(b)
not(not(b))                    → b
not(lt_int(a, b))              → ge_int(a, b)
if c { 1 } else { 0 }          → from_bool(c)
if c { 1 } else { from_bool(y) } → from_bool(c || y)
```

`from_bool` maps false to 0 and true to 1. The first two identities follow from
those two values. The last identity evaluates `y` only when `c` is false. The
pass can also recover a short circuit from branches writing the same synthetic
boolean temporary. A source variable written by both arms keeps its branch
writes.

### Caveats

Ordered floating point comparisons need a separate rule. With `a = NaN`,
`!(a < b)` is true and `a >= b` is false, so the pass keeps the negation.
Negating equality can still flip `==` and `!=`. Rewrites that would remove an
explicit conversion or comparison, or a protected macro value, are blocked.

## inline-temps

### Purpose

Move a synthetic temporary’s initializer into its only read so lowering can emit
the expression directly.

### Example

**C input**

```c
int decrement(int n) {
    n--;
    return n;
}
```

**Emitted Rust**

```rust
fn decrement(mut n: i32) -> i32 {
    n -= 1;
    n
}
```

### Rewrite rules

```text
temp t = E; S; use(read(t))  →  S; use(E)
temp a = read(n); temp b = a - 1; write(n, b)
                            →  write(n, read(n) - 1)
```

The pass substitutes the initializer once, keeping its type and node ID.

### Caveats

The read must be in one of the next 32 sibling statements or the list's tail
value. It must be a direct value use outside macro arguments, address operands,
and statement expressions. Captures, a second read, or an unsequenced temporary
keep the binding.

Moving `E` reorders it with `S` and any evaluation before the read in the
consumer. For ordinary reads and writes, the conflicts to check are
`reads(E) ∩ writes(S)`, `writes(E) ∩ reads(S)`, and `writes(E) ∩ writes(S)`. For
example, sinking `t = read(n)` past `write(n, 7)` would change which value
reaches the consumer. The pass also blocks opaque statements and control flow.
An effectful initializer must execute on the same paths, so moving a call under
the right operand of `&&` would be invalid when the left operand is false.

## const-fold

### Purpose

Fold implicit integer conversions, signed negations, and comparisons of
constants into typed literals.

### Example

**C input**

```c
signed char narrow(void) {
    signed char c = 200;
    return c;
}
```

**Emitted Rust**

```rust
fn narrow() -> i8 {
    let c: i8 = -56;
    c
}
```

### Rewrite rules

```text
implicit_int_convert<T>(const(v)) → const<T>(convert_T(v))
r = v mod 2^width(T), with 0 <= r < 2^width(T)
convert_T(v) = r - 2^width(T) if T is signed and r >= 2^(width(T)-1)
convert_T(v) = r              otherwise
```

For the eight-bit signed destination, `r = 200` and `200 - 256 = -56`. The pass
also folds signed literal negation and eligible integer constant comparisons,
such as `ne(1, 0) → true`.

### Caveats

The match is limited to these IR operations. An explicit cast such as
`(signed char)200`, an arithmetic expression such as `60 * 60`, or a conversion
of a wrapped macro constant stays in the IR.

## do-while-zero

### Purpose

Turn a do-while loop with a constant false test into a block that runs once.

### Example

**C input**

```c
int once(int stop, int x) {
    do {
        if (stop)
            continue;
        x += 1;
    } while (0);
    return x;
}
```

**Emitted Rust**

```rust
fn once(stop: i32, mut x: i32) -> i32 {
    if stop == 0 {
        x += 1;
    }
    x
}
```

### Rewrite rules

```text
do_L { A } while false  →  block { A }                 if A has no jumps to L
do_L { A } while false  →  block L { A[continue L ↦ break L] } otherwise
```

A continue targeting `L` reaches the false test, so it has the same destination
as a break targeting `L`. Inner loop jumps keep their targets. The later
`labeled-blocks` pass can turn the exit in this example into a plain `if`.

### Caveats

The test must consist of a false boolean constant or a recognized false integer
comparison, with no evaluation statements. For example, `while (tick(), 0)`
still calls `tick()` whenever execution reaches its test. Kept expansions retain
the loop shape for their macro template. The same loop becomes eligible when
Slate emits the expansion inline.

## labeled-blocks

### Purpose

Recover if/else branches and switch arms from labeled blocks and their exits.

### Example

**C input**

```c
int choose(int c, int x) {
    if (c)
        goto other;
    x += 1;
    goto done;
other:
    x += 2;
done:
    return x;
}
```

**Emitted Rust**

```rust
fn choose(c: i32, mut x: i32) -> i32 {
    if c == 0 {
        x += 1;
    } else {
        x += 2;
    }
    x
}
```

### Rewrite rules

```text
block L { if C { A; break L }; B; break L }
    → if C { A } else { B }
```

Both branches reach the end of `L`, and each executes the same body as before.
This pass can also sink a common tail into the sole break that reaches it, and
can move case siblings into switch arms.

### Caveats

If two breaks share the tail, sinking would duplicate it. If the true branch
ends in `continue`, the two branches have different destinations. A tail move
also needs to preserve scope and name resolution. For example, moving a call to
`helper()` into a block that declares a local named `helper` would change what
the emitted Rust call resolves to. Cleanup, VLA, and SEH scopes can also block a
move.

## dead-temporaries

### Purpose

Remove unread synthetic result slots while keeping the expressions that produced
their values.

### Example

**C input**

```c
int f(void);
int g(void);

void discard(int c) {
    c ? f() : g();
}
```

**Emitted Rust**

```rust
unsafe extern "C" {
    fn f() -> i32;
    fn g() -> i32;
}

fn discard(c: i32) {
    if c != 0 {
        unsafe { f() };
    } else {
        unsafe { g() };
    }
}
```

### Rewrite rules

```text
temp t; if C { write(t, E) } else { write(t, F) }
    → if C { eval(E) } else { eval(F) }
temp t = E → eval(E)
```

The result slot can disappear when it is a synthetic scalar with zero reads and
all references are direct writes. Every initializer or written value is still
evaluated at the original point. The example must call exactly one of `f()` and
`g()`, even though their results are discarded.

### Caveats

Changing the C statement to `return c ? f() : g();` adds a read of the result
slot. A VLA extent can also refer to a temporary through its type, even when the
statement list has no read. Both uses keep the slot. Source declarations and
storage in kept expansions stay in place.

## decl-init

### Purpose

Combine an uninitialized declaration with its first assignment so lowering can
emit an initialized Rust binding.

### Example

**C input**

```c
int choose(int c) {
    int x;
    if (c)
        x = 4;
    else
        x = 9;
    return x;
}
```

**Emitted Rust**

```rust
fn choose(c: i32) -> i32 {
    let x: i32 = if c != 0 {
        4
    } else {
        9
    };
    x
}
```

### Rewrite rules

```text
let x; S; write(x, E) → S; let x = E
let x; if C { A; write(x, E) } else { B; write(x, F) }
    → let x = if C { A; E } else { B; F }
```

The first rule moves the declaration to the write. The second keeps each branch
prefix in its original arm. In both rules the initializer runs where the first
write's value would have run.

### Caveats

The declaration must have no source initializer. In the straight-line rule `S`
and `E` must leave `x` untouched, with no control flow or lifetime boundary that
prevents sinking. The branch rule needs an adjacent conditional with one final
write per branch and no intervening use of `x` in the test or either arm. For
example, `int x; prepare(&x); x = compute();` needs the storage before `prepare`
runs.

## truth-locals

### Purpose

Store an integer local as a boolean when all its definitions produce 0 or 1 and
its uses favor truth tests.

### Example

**C input**

```c
int classify(int a, int b) {
    int lt = a < b;
    if (lt)
        return 4;
    return 5;
}
```

**Emitted Rust**

```rust
fn classify(a: i32, b: i32) -> i32 {
    let lt: bool = a < b;
    if lt {
        return 4;
    }
    5
}
```

### Rewrite rules

```text
all definitions of x ∈ {0, 1}
encode(0) = false, encode(1) = true
ne(read_int(x), 0) → read_bool(x)
eq(read_int(x), 0) → not(read_bool(x))
other read_int(x)  → from_bool<int>(read_bool(x))
```

The encoding preserves every possible stored value. A definition qualifies if it
is 0, 1, a conversion from a boolean, a conditional with two qualifying arms, or
a copy from another proven local. The pass solves the proof across copies. Truth
uses must outnumber integer reads, since each remaining integer read needs a
conversion back.

### Caveats

A write of 2 would collapse two distinct nonzero values into true, changing a
later integer read. A call that writes through `&x` defeats the proof and needs
the original storage type. Updates, captures, volatile or atomic accesses, and
references in macro or OpenMP regions also keep the integer local.

## Add a transform fixture

Put a small C input in `crates/slate/tests/fixtures.transforms/<passes>/`. The
directory name selects the passes for the generated IR checks. Include an
example that rewrites and one that reaches the guard you want to preserve. Use a
Slate differential fixture when you need to check the Rust output or runtime
behavior. See [Testing](testing.md).
