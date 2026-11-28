# Allocation, bounds, and cleanup

- `alloc_size(N)` and `alloc_size(N, M)` record which arguments specify the
  allocation size. Slate checks the attribute's applicability and retains it as
  metadata.
- `alloc_align(N)` records which argument specifies allocation alignment.
- `assume_aligned(N)` and `assume_aligned(N, OFFSET)` retain a return alignment
  annotation. Slate does not turn it into a Rust assumption.
- `malloc` accepts the allocation annotation on a function. `malloc(FREE)` and
  `malloc(FREE, N)` record the deallocator and which of its parameters receives
  the pointer.
- `cleanup(FUNCTION)` runs a cleanup function for an automatic object at scope
  exit. The function receives the object's address.
- `uninitialized` suppresses initialization of an automatic object when
  `-ftrivial-auto-var-init` would otherwise initialize it. GCC also accepts
  `[[gnu::uninitialized]]`.
- `counted_by(MEMBER)` associates a field with its element count. Slate resolves
  the member for `__builtin_counted_by_ref`.
- `counted_by_or_null(MEMBER)` accepts Clang's nullable element-count annotation
  and validates the member reference.
- `sized_by(MEMBER)` accepts Clang's byte-size annotation and validates the
  member reference.
- `sized_by_or_null(MEMBER)` accepts Clang's nullable byte-size annotation and
  validates the member reference.
- `pass_object_size(N)` distinguishes Clang overloads and can break an overload
  tie. Slate does not pass the hidden size argument.
- `pass_dynamic_object_size(N)` uses the same overload handling. Slate does not
  pass the hidden dynamic size argument.

## `cleanup`

```c
int total;
static void add_to_total(int *x) { total += *x; }

void record(int n) {
    int x __attribute__((cleanup(add_to_total))) = n;
}
```

Slate emits a guard that invokes the cleanup function with the address of `x`
when the scope exits. The Rust excerpt shows `record`.

```rust,ignore
fn record(n: i32) {
    let mut x: i32 = n;
    let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(x) as *mut _;
    let __slate_cleanup_0 = ::slate_support::Guard(
        &mut || add_to_total(__slate_cleanup_ptr_0 as *mut _),
    );
}
```

## `malloc`, `alloc_size`, and `alloc_align`

```c
extern void *reserve(unsigned long bytes, unsigned long alignment)
    __attribute__((malloc, alloc_size(1), alloc_align(2)));

void *request(void) { return reserve(128, 16); }
```

The indices are one-based. They describe the size and alignment arguments. Slate
records the annotations without adding Rust allocation or alignment checks at
the call.

```rust
unsafe extern "C" {
    fn reserve(arg0: u64, arg1: u64) -> *mut ();
}

fn request() -> *mut () {
    unsafe { reserve(128u64, 16u64) }
}
```

## `counted_by`

```c
struct Packet {
    unsigned n;
    int data[] __attribute__((counted_by(n)));
};

unsigned *count(struct Packet *p) {
    return __builtin_counted_by_ref(p->data);
}
```

Slate resolves `n` as the counter for `data`. The generated Rust takes the
address of that field. This excerpt shows the function.

```rust,ignore
fn count(p: &Packet) -> *mut u32 {
    core::ptr::addr_of!((*p).n) as *mut u32
}
```
