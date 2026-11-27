# Constant Evaluation

Sema evaluates constants that determine types, layout, or control flow. Examples
include enum values, fixed array extents, case labels, designators, and static
assertions. This uses the selected C types and target rules.

**C input**

```c
enum { count = sizeof(long) };
int table[count];
```

**IR excerpt**

```ir
type @type0 = enum : u32 {
    %0 count = const<i32>(8);
} [size=4, align=4];
global %2 table: array<i32, 8> [storage=static] [align=16] [linkage=external];
```

The enum value is `const<i32>(8)` and the array type contains the resolved
extent `8`.

Here the target's size for C `long` determines the fixed extent. The IR contains
that extent, so Rust lowering never has to infer it from the name `long` or the
host running Slate.

A compiler query can depend on information that becomes available later. For
eligible `__builtin_constant_p` operands at optimized levels, Slate can preserve
a query in IR so it can be answered after inlining. A constant required by a C
declaration must be resolved when sema checks it. See
[Translation Unit Configuration](../configuration.md#why-optimization-levels-matter).

Start with `sema/fold.rs` for evaluation and `sema/typer.rs` for the type and
conversion facts it reads.
