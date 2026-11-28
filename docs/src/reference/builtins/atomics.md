# Atomics

- `__c11_atomic_*` covers initialization, loads, stores, exchanges,
  compare-exchange, fetch operations, and fences used by Clang's
  `<stdatomic.h>`.
- `__atomic_*` covers GNU loads, stores, exchanges, compare-exchange,
  read-modify-write operations, flag operations, and fences. This includes the
  generic pointer forms and typed `_n` forms where defined.
- `__scoped_atomic_*` carries an explicit synchronization scope. Supported
  operations and scopes depend on the target.
- `__sync_*` covers legacy fetch and update operations, compare-and-swap, locks,
  and synchronization. Recognized size-suffixed spellings take their width from
  the pointer operand.
- `__atomic_always_lock_free`, `__atomic_is_lock_free`, and
  `__c11_atomic_is_lock_free` query the target's lock-free widths. A runtime
  query can call `libatomic`.

For the resulting operations and ordering policies, see
[Atomics, Builtins, and Inline Assembly](../../parsing/ir/special-operations.md).

The `__atomic_fetch_OP` and `__sync_fetch_and_OP` forms return the old value.
The `__atomic_OP_fetch` and `__sync_OP_and_fetch` forms return the new value.
The legacy `__sync_*` family generally uses sequentially consistent ordering,
with special orderings for lock operations.

## `__atomic_fetch_add`

```c
unsigned increment(unsigned *ptr) {
    return __atomic_fetch_add(ptr, 1, __ATOMIC_RELAXED);
}
```

The update returns the old value with relaxed ordering.

```rust,ignore
fn increment(ptr: *mut u32) -> u32 {
    unsafe {
        core::sync::atomic::AtomicU32::from_ptr(ptr as *mut u32)
            .fetch_add(1u32, core::sync::atomic::Ordering::Relaxed)
    }
}
```

## `__atomic_always_lock_free`

```c
int is_lock_free(void) { return __atomic_always_lock_free(4, 0); }
```

Slate folds the query using the selected target's lock-free widths.

```rust,ignore
fn is_lock_free() -> i32 {
    true as i32
}
```

## `__atomic_load_n`

```c
int example(int *p) {
    return __atomic_load_n(p, __ATOMIC_ACQUIRE);
}
```

Slate loads the 32-bit value with acquire ordering.

```rust,ignore
fn example(p: *mut i32) -> i32 {
    unsafe {
        core::sync::atomic::AtomicI32::from_ptr(p as *mut i32)
            .load(core::sync::atomic::Ordering::Acquire)
    }
}
```

## `__atomic_compare_exchange_n`

```c
_Bool example(int *p, int *expected, int value) {
    return __atomic_compare_exchange_n(p, expected, value, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE);
}
```

This strong compare-exchange uses acquire-release ordering on success and acquire ordering on failure. On failure, it writes the observed value back through `expected`.

```rust,ignore
fn example(p: *mut i32, expected: *mut i32, value: i32) -> bool {
    {
        let __t0 = expected;
        let __t1 = value;
        unsafe {
            match core::sync::atomic::AtomicI32::from_ptr(p as *mut i32)
                .compare_exchange(
                    *__t0,
                    __t1,
                    core::sync::atomic::Ordering::AcqRel,
                    core::sync::atomic::Ordering::Acquire,
                ) {
                Ok(_) => true,
                Err(__t2) => {
                    *__t0 = __t2;
                    false
                },
            }
        }
    }
}
```

## `__atomic_thread_fence`

```c
void example(void) {
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}
```

Slate emits a sequentially consistent thread fence.

```rust,ignore
fn example() {
    core::sync::atomic::fence(core::sync::atomic::Ordering::SeqCst);
}
```

## `__sync_fetch_and_add`

```c
int example(int *p) {
    return __sync_fetch_and_add(p, 1);
}
```

The legacy builtin returns the old value and uses sequentially consistent ordering.

```rust,ignore
fn example(p: *mut i32) -> i32 {
    unsafe {
        core::sync::atomic::AtomicI32::from_ptr(p as *mut i32)
            .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
    }
}
```
