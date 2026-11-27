# Name Resolution

The parser tracks enough names to distinguish types from expressions. Sema
resolves each use to the C entity it actually names. IR refers to that entity by
ID, so a Rust name can change without changing what an expression reads.

**C input**

```c
int x = 3;
int read_x(int test) {
    int value = x;
    if (test) {
        int x = 7;
        return x;
    }
    return value;
}
```

**Typed IR excerpt**

```text
global %0 x: i32 [storage=static] = const<i32>(3) [linkage=external];
fn %1 @read_x(%2 test: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    let %3 value: i32 [storage=automatic] = read<i32>(%0);
    if ne<i32>(read<i32>(%2), const<i32>(0))
        {
            let %4 x: i32 [storage=automatic] = const<i32>(7);
            return read<i32>(%4);
        }
    return read<i32>(%3);
}
```

The global is `%0` and the shadowing local is `%4`. The two reads name those IDs
directly.

The initializer of `value` reads the file-scope `x`. The inner return reads the
block-local `x`. They have separate `BindingId`s. Later stages can look up the
uses of either binding without comparing their printed names.

`names::resolve_items` builds a `NameResolution` for the translation unit.
`TypeResolver::with_names` uses it to look up the bindings behind references and
declarations. The checker also checks linkage and merges compatible
redeclaration types. For example, `extern int values[];` and `int values[4];`
can describe one object. Lowering reads the resolved declaration type instead of
merging the declarations again.

Start with `sema/names.rs` for binding resolution and `sema/types.rs` for types
and redeclarations.
