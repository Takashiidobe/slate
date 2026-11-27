# Inspecting the IR Pipeline

Use the IR dump to locate a translation problem. Check sema's result first, then
compare the output of any transform that touches the operation.

## Compare before and after a transform

**C input**

```c
signed char narrow(void) {
    signed char c = 200;
    return c;
}
```

```bash
slate emit-slate-ir input.c
slate emit-slate-ir --passes=const-fold input.c
```

**Before transforms**

```text
let %1 c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(const<i32>(200));
```

**After Const Fold**

```text
let %1 c: i8 [storage=automatic] = const<i8>(-56);
```

Both forms initialize the signed byte with the same C value. The explicit
truncation shows which conversion sema chose. The folded IR shows what the
transform did with that choice. If the width or conversion is already wrong in
the first dump, investigate sema. If the first dump is correct and the second
changes the value, investigate the pass.

## Follow facts into Rust

Fact analysis reads the transformed IR and gives lowering a read-only view for
the current translation unit. For example, a proven pointer length can change a
Rust function to take a slice while leaving the pointer operations in the
analyzed IR. Inspect the Rust signature with `slate translate input.c` when
checking how lowering uses a fact.

`slate-parser ir input.c` dumps parser IR directly. The Slate `emit-slate-ir`
command can select transforms and runs through Slate's macro handling. Use the
parser command when investigating the original macro structure. See
[Analyses](../../writing-a-query.md) and
[Transforms](../../writing-a-transform.md).
