# Reading a Module

An IR module is one translation unit for one target. Read the target header
first, then the types, globals, and functions. References use IDs such as `%1`
for bindings and `@type0` for type definitions.

## Read a function

**C input**

```c
int mix(const int *p, int i, unsigned char n) {
    return p[i] + n;
}
```

**Typed IR excerpt**

```text
fn %0 @mix(%1 p: ptr<const i32>, %2 i: i32, %3 n: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    return add<i32, overflow=ub>(
        read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%1), read<i32>(%2)))),
        reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3)))
    );
}
```

Read the return expression from the inside out. `%1` names the pointer
parameter. `ptr_offset` advances it by `i` elements and `deref` identifies the
memory to read. Sema promotes the `u8` parameter before the addition. The outer
`add<i32, overflow=ub>` gives the computation width and overflow rule. Rust
lowering can emit that sequence without repeating C subscript or promotion
rules.

## Read the target header

```text
target "x86_64-unknown-linux-gnu" {
    endian = little;
    pointer [size=8, align=8];
    stack_alignment = 16;
    long_double = f80;
    ...
}
```

The header describes the C target, including scalar storage sizes and
alignments. A `long double` operation can therefore use `f80` even when Slate
itself is running on a machine with a different format. Linkage and function
fallthrough are likewise explicit in the function header.
