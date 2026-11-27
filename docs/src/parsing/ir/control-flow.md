# Control Flow and Evaluation Order

The IR keeps structured C control flow. It also spells out where an effect runs
inside a condition, increment, or branch. Rust lowering needs both the shape and
the effect order.

## Keep the short circuit inside the loop

**C input**

```c
int ready(void);
int scan(int n) {
    int i;
    for (i = 0; i < n && ready(); ++i) {}
    return i;
}
```

**IR excerpt**

```ir
for %4
    init:
        write<i32>(%3, const<i32>(0));
    condition: {
        let %5: bool [synthetic];
        if lt<i32>(read<i32>(%3), read<i32>(%2))
            write<bool>(%5, ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0)));
        else
            write<bool>(%5, const<bool>(false));
        yield read<bool>(%5);
    }
    ...
```

The `for` retains separate initializer, condition, increment, and body regions.
The condition yields a boolean. The call to `ready` is inside the true arm of
`i < n`, so it runs only when the left operand succeeds. Moving that call ahead
of the branch could add an observable effect. Keeping the loop structure also
gives range analysis a place to look for a counted loop.

## Mark an unsequenced effect

**C input**

```c
int take(int a, int b);
int g(int i) { return take(i++, i); }
```

**IR excerpt**

```ir
let %7: i32 [synthetic, unsequenced] = read<i32>(%4);
let %8: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
write<i32, unsequenced>(%4, read<i32>(%8));
```

The C call has undefined behavior because its unsequenced arguments modify and
read the same `i`. Slate's normalization prints an order and marks the affected
operations `unsequenced`. That marker lets consumers distinguish the emitted
order from a sequencing guarantee in the C source. A temporary used for an
ordinary, sequenced effect lacks that marker.
