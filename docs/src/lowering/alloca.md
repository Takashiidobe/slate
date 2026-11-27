# Dynamic Stack Allocation

`alloca` requests storage whose size can be decided at runtime. Slate gives
ordinary `alloca` allocations the lifetime of the function call and preserves
alignment and spacing for the compiler flavor and target where its layout model
applies. Two live allocations can hold separate values, including when the
function calls another function that uses `alloca`.

```c
#include <alloca.h>

int two_buffers(void) {
    unsigned char *a = alloca(16);
    unsigned char *b = alloca(16);
    a[0] = 10;
    b[0] = 20;
    return a[0] + b[0];
}
```

The x86-64 Clang lowering uses a function-local mark and requests 16-byte
alignment. This excerpt simplifies the generated pointer casts.

```rust
fn two_buffers() -> i32 {
    let _mark = __slate_alloca::Mark::new();
    let a = __slate_alloca::alloca(16, 16).cast::<u8>();
    let b = __slate_alloca::alloca(16, 16).cast::<u8>();
    unsafe {
        *a = 10;
        *b = 20;
        i32::from(*a) + i32::from(*b)
    }
}
```

## Lifetime and layout

The helper allocates from a thread-local arena backed by heap blocks. The mark
saves the current block and position, then restores them when the function
returns. When an inner function returns, later allocations can reuse its space
without disturbing the caller's live buffers.

The arena subtracts the requested size plus target-specific padding, then rounds
the new position down to the required alignment. Within a block, two 16-byte
allocations are 16 bytes apart for x86-64 Clang and 32 bytes apart for x86-64
GCC at its default alignment. For `__builtin_alloca_with_align`, Slate converts
the explicit alignment from bits to bytes, uses at least the target alignment,
and skips the ordinary padding.

The backing blocks are on the heap, so addresses relative to native stack locals
and limits imposed by stack exhaustion can differ. Spacing can also differ
across arena blocks. With GCC `-mavx`, Slate models the alignment, while native
spacing can depend on the incoming stack address.
[Clang layout fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/alloca_layout.c)
·
[GCC layout fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/gcc/alloca_layout_gcc.c)
