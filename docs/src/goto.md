# Switch and Goto

Slate preserves which statements run after a C `switch` or `goto`, including
case fallthrough and jumps into a cycle at different labels.

## Switch and fallthrough

A Rust `match` evaluates the C selector once and chooses the case to enter.
Several labels can share an arm, and `_` represents `default` or an empty arm
when there is no default. Here entering case `1` executes both additions.
Entering case `2` executes only the second.

```c
int score(int x) {
    int out = 0;
    switch (x) {
    case 1:
        out += 10;
    case 2:
        out += 20;
        break;
    case 3:
    case 4:
        out += 40;
        break;
    default:
        out += 90;
    }
    return out;
}
```

```rust
fn score(x: i32) -> i32 {
    let mut out = 0;
    match x {
        1 => {
            out += 10;
            out += 20;
        }
        2 => out += 20,
        3 | 4 => out += 40,
        _ => out += 90,
    }
    out
}
```

Each Rust arm ends at the end of its body. Slate includes the statements reached
by fallthrough in the first arm, in their original order. A C `break` exits the
switch, while a `continue` inside a switch nested in a loop continues the
enclosing loop.
[Switch fallthrough fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/switch_fallthrough.c)

## Structured goto

A forward jump that skips statements can become `break 'done` in a labeled
block. In this shape, the C label `done` marks the destination after the block.

```c
if (skip)
    goto done;
work();
done:
finish();
```

```rust
'done: {
    if skip != 0 {
        break 'done;
    }
    work();
}
finish();
```

A backward jump to the head of a region can become a loop, with `continue`
returning to the head and `break` taking the original exit edge. The
[Structure Goto](transforms/structure-goto.md) transform recovers these shapes
in the C IR. Later passes may simplify the labeled block to an `if`.

## Cycles with multiple entry points

This example can enter the same cycle at either `a` or `b`. Both labels are
reachable from outside the cycle, so neither can serve as the single loop
header. This is an irreducible control-flow region.

```c
int run(int choose_b) {
    int x = 0;
    if (choose_b)
        goto b;
a:
    x += 1;
    if (x < 3)
        goto b;
    goto done;
b:
    x += 2;
    if (x < 4)
        goto a;
done:
    return x;
}
```

Slate introduces a common entry with a state variable. The initial branch sets
the state for `a` or `b`. Inside the loop, each jump sets the next state and
each exit breaks the loop. The following Rust uses names for the generated state
numbers.

```rust
fn run(choose_b: i32) -> i32 {
    const A: usize = 2;
    const B: usize = 1;
    let mut x = 0;
    let mut state = if choose_b != 0 { B } else { A };
    loop {
        match state {
            B => {
                x += 2;
                if x >= 4 {
                    break;
                }
                state = A;
            }
            _ => {
                x += 1;
                if x >= 3 {
                    break;
                }
                state = B;
            }
        }
    }
    x
}
```

The dispatcher matches a state introduced by Slate to choose the next block. The
switch example matches a value from the C program to choose a case. Here `x`
lives outside the loop so both arms and the code at `done` can access it. The
dispatcher can stay local to the cycle, leaving straight-line code before and
after it.
[Irreducible goto fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/goto_irreducible.c)
