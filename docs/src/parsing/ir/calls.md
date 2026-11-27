# Calls and ABI

A call records the function signature and the ABI information the backend needs
to pass arguments and receive the result. The ABI can differ from the target
default, even when the parameter types look ordinary.

## Call a Windows ABI function on a Linux target

**C input**

```c
__attribute__((ms_abi)) int add(int a, int b);
int apply(int x) { return add(x, 1); }
```

**Typed IR excerpt**

```text
fn %2 @add(%5 a: i32, %6 b: i32) -> i32 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar];
fn %3 @apply(%4 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    return call<i32, signature=fn ms_abi(i32, i32) -> i32, abi=win64 ms_abi(scalar, scalar) -> scalar>(
        %2, read<i32>(%4), const<i32>(1));
}
```

The declaration says `abi=win64 ms_abi`, and the call carries the same
convention. This lets lowering select the matching Rust ABI both where the
function is declared and where it is called. Relying on the Linux default would
put arguments in the wrong registers.

Other calls can carry an ABI classification for passing an aggregate indirectly
or splitting it into register pieces. The consumer uses that classification to
decide whether the Rust signature can express the C call directly or needs a
bridge. See [C Types, Layout, and ABI](../sema/types-layout-abi.md).
