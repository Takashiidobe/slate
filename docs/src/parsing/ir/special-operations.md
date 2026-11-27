# Atomics, Builtins, and Inline Assembly

C extensions can carry semantics that a plain Rust arithmetic operation or
function call cannot express. Slate resolves those semantics before Rust
emission and records each operation's contract in IR.

## Atomic read and update

**C input**

```c
int bump(_Atomic int *p) {
    return __c11_atomic_fetch_add(p, 1, __ATOMIC_ACQ_REL);
}
```

**Typed IR excerpt**

```text
let %2: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(
    deref(read<ptr<atomic i32>>(%1)),
    add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
```

The operation modifies one atomic place, returns the old value, and uses
acquire-release ordering. The increment wraps. Keeping these properties on one
update lets Rust emit a read-modify-write operation with matching ordering.
Splitting it into a load and store would allow another thread to intervene.

## A builtin with two results

**C input**

```c
int fits(int a, int b, int *out) {
    return !__builtin_add_overflow(a, b, out);
}
```

**Typed IR excerpt**

```text
return from_bool<i32, reason=return>(not<bool>(overflow_add<bool>(read<i32>(%1), read<i32>(%2), deref(read<ptr<i32>>(%3)))));
```

`overflow_add<bool>` stores the sum through the output place and produces a
boolean overflow result. The caller negates that boolean and converts it to C's
`int` result. Lowering can preserve both effects without relying on a function
named `__builtin_add_overflow` in the Rust program.

## A selected assembly operand

**C input**

```c
int bit(void) {
    int out;
    __asm__("movl %1, %0" : "=r"(out) : "i"(2));
    return out;
}
```

**Typed IR excerpt**

```text
fn %0 @bit() -> i32 [linkage=external] [fallthrough=ub_if_used] {
    let %1 out: i32 [storage=automatic];
    asm "movl %1, %0" [dialect=att] [options=pure,nomem,nostack] {
        template: "movl " %1 ", " %0;
        lateout 0 "r" [reg] width 32 place<i32>(%1);
        in 1 "i" [imm | sym] -> imm width 32 const<i32>(2);
    }
    return read<i32>(%1);
}
```

The C `"i"` constraint permits an immediate or symbol in this case. Sema selects
`imm` and supplies `const<i32>(2)`. The output records its place and register
class. Rust lowering uses the selected operand forms to build `asm!`, with the C
constraints already interpreted for the target.
