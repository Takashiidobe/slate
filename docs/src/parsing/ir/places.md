# Places, Values, and Pointers

A place identifies storage. A value represents the result of a computation. The
IR separates the two so a consumer can see which part computes an address and
which part loads or stores the object.

## Follow a compound assignment

**C input**

```c
void bump(volatile int *p, int i) {
    p[i] += 1;
}
```

**Typed IR excerpt**

```text
fn %0 @bump(%1 p: ptr<volatile i32>, %2 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
    let %3: ptr<volatile i32> [synthetic] = ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(
        read<ptr<volatile i32>>(%1), read<i32>(%2));
    let %4: i32 [synthetic] = read<i32, volatile>(deref(read<ptr<volatile i32>>(%3)));
    let %5: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
    write<i32, volatile>(deref(read<ptr<volatile i32>>(%3)), read<i32>(%5));
}
```

`ptr_offset` computes the address in units of `i32` elements. The synthetic
binding `%3` saves that address once so the load and store use the same place.
`deref` names the place, and the `read` and `write` explicitly perform volatile
accesses. Reading the pointer binding itself is an ordinary read.

This distinction lets lowering emit a pointer offset and volatile load and store
without creating a Rust reference or evaluating the address twice. Analyses can
also tell the difference between updating a pointer binding and updating memory
through it.
