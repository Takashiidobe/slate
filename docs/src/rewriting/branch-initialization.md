# Branch Initialization

Slate can turn two branch assignments into one Rust initializer. Each branch
computes the value it would have assigned, and only the selected branch runs.

```c
int choose(int flag, int a, int b) {
    int result;
    if (flag)
        result = a;
    else
        result = b;
    return result;
}
```

```rust
fn choose(flag: i32, a: i32, b: i32) -> i32 {
    let result: i32 = if flag != 0 { a } else { b };
    result
}
```

Lowering can emit this shape directly. The Rust AST rule `branch_init::fold` can
also recover it from a declaration followed by an `if` whose two arms end in
assignments to that binding. Earlier statements in an arm stay there and execute
before that arm yields its value.

## When the assignments stay

The Rust rule requires an uninitialized binding, which may have a placeholder
introduced by lowering. An initializer written in the C source is kept. The
condition and statements before the final assignment must also leave the binding
alone, and the assigned expression cannot read the old value.

For example, in `int result = 5; if (flag) result += a; else result = b;`, the
first arm needs the original `5`. With a missing `else`, there is no second
assignment to use as the value of that arm. A source initializer that both
branches overwrite still keeps its original evaluation.
[Branch initialization fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/branch_init_let_if.c)
