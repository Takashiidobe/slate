# Builtins and Inline Assembly

A compiler builtin can choose a type, fold a value, or request a machine
operation. Sema uses the flavor and target to resolve the call. The IR carries
the result as a constant, explicit operation, or intrinsic with resolved
operands. Rust lowering can then select the corresponding Rust operation or
intrinsic binding.

**C input**

```c
int same_type(void) { return __builtin_types_compatible_p(int, long); }
```

**Typed IR excerpt**

```text
fn %0 @same_type() -> i32 [linkage=external] [fallthrough=ub_if_used] {
    return const<i32>(0);
}
```

The builtin resolves to an integer constant. Rust lowering receives the answer
in IR.

For inline assembly, sema interprets input and output constraints for the
target. It selects operand forms and resolves immediates or symbols when
required. Rust lowering uses those decisions when it constructs `asm!`. A C
input constrained by `"i"`, for example, needs a known immediate operand. At an
optimized level, a constant supplied by an inlined caller can make that
possible. See
[Translation Unit Configuration](../configuration.md#why-optimization-levels-matter).

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

The output selects `reg`. The input constraint allows an immediate or a symbol,
and sema selects `imm` with the resolved value `2`.

Start with `sema/builtins.rs` for builtin typing, `sema/asm.rs` for GNU asm
constraints, and `sema/asm_constants.rs` for operand specialization.
