# Clang Blocks

With `-fblocks`, Clang's Blocks extension lets a block capture local values and
be called through a block pointer. An ordinary capture takes the value when the
block is created. A `__block` capture shares mutable storage with the enclosing
scope and other blocks that capture the same variable.

```c
typedef int (^counter)(void);
extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

counter make_counter(int start) {
    __block int count = start;
    return _Block_copy(^{ return ++count; });
}
```

Calling a block returned by `make_counter(10)` twice in sequence returns 11
then 12. `_Block_copy` keeps the block alive after `make_counter` returns. The
caller releases the copy with `_Block_release` when finished.

## Emit the block and its captures

Slate places the block's invoke function and descriptor in a C-compatible block
header, followed by its captures. Invoking the block passes its address as the
first argument to the generated function, so it can find the captures. The IR
marks `count` as `byref`, so the capture holds a pointer to a `Byref<i32>` cell.
The following Rust excerpt shortens the generated names.

```rust
#[repr(C)]
#[derive(Clone, Copy)]
struct Captures {
    count: *mut __slate_blocks::Byref<i32>,
}

extern "C-unwind" fn invoke_counter(block: *mut core::ffi::c_void) -> i32 {
    let captures = unsafe { __slate_blocks::captures::<Captures>(block) };
    let next = (unsafe { *__slate_blocks::value(captures.count) }) + 1;
    unsafe { *__slate_blocks::value(captures.count) = next };
    next
}
```

The cell has a forwarding pointer. When `_Block_copy` moves the cell to the
heap, the old cell forwards to the heap copy. Reads and writes from the
declaring scope and the copied block then use the same value. The block
descriptor tells the runtime which captures need copy and release handling.

`translate-project` includes the vendored `slate-blocks` runtime for generated
Blocks support on non-Apple targets. Apple targets use the Blocks runtime from
the system libraries.
[By-reference capture fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/block_byref.c)
