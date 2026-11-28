# Microsoft intrinsics

Availability depends on the MSVC target and architecture. Clang mode with
Microsoft extensions also recognizes shared Microsoft intrinsics.

- `__cpuid` and `__cpuidex` query x86 CPU identification.
- `_BitScanForward` and `_BitScanReverse` locate a set bit and report whether
  the input contained one. The `64` forms take 64-bit operands.
- `_rotl`, `_rotr`, `_rotl64`, and `_rotr64` rotate integer bits.
- `__ll_lshift`, `__ll_rshift`, and `__ull_rshift` shift 64-bit values.
- `_mul128` and `_umul128` return the low half of a wide product and write its
  high half through a pointer.
- `__mulh` and `__umulh` return the high half of a wide product.
- `__shiftleft128` and `__shiftright128` shift across two 64-bit operands on
  x86-64.
- `__movsb`, `__movsw`, `__movsd`, and the corresponding `__stos*` names cover
  x86 copy and fill operations at the selected element width.
- `_bittest`, `_bittestandset`, `_bittestandreset`, and `_bittestandcomplement`
  test or modify a selected bit. The interlocked forms use atomic updates where
  supported.
- `_Interlocked*` covers exchange, compare-exchange, increment, decrement, fetch
  arithmetic, and bitwise updates. Recognized width and ordering suffixes depend
  on the operation and target.

See [Builtin Inventories](inventories.md#msvc) for individual names and
architecture-specific probe results.

## `_rotl`

```c
unsigned rotate(unsigned value, int count) { return _rotl(value, count); }
```

Translate with `--flavor=msvc --target=x86_64-pc-windows-msvc`. Slate lowers the
rotation to Rust integer operations.

```rust,ignore
fn rotate(value: u32, count: i32) -> u32 {
    value.rotate_left(count as u32)
}
```

## `_InterlockedIncrement`

```c
long increment(volatile long *value) { return _InterlockedIncrement(value); }
```

Translate with `--flavor=msvc --target=x86_64-pc-windows-msvc`. This updates a
Windows `long` atomically and returns the incremented value.

```rust,ignore
fn increment(value: *mut i32) -> i32 {
    unsafe {
        core::sync::atomic::AtomicI32::from_ptr(value as *mut i32)
            .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
            .wrapping_add(1)
    }
}
```

## `__cpuid`

```c
void example(int *out) {
    __cpuid(out, 0);
}
```

Translate with `--flavor=msvc --target=x86_64-pc-windows-msvc`. The result is written to four consecutive integers.

```rust,ignore
fn example(out: *mut i32) {
    unsafe { __cpuid(out as *mut i32, 0i32 as i32) };
}
```

## `_BitScanForward`

```c
unsigned char example(unsigned long *index, unsigned long mask) {
    return _BitScanForward(index, mask);
}
```

Translate with `--flavor=msvc --target=x86_64-pc-windows-msvc`. If the mask has a set bit, Slate stores its lowest set-bit index and returns a nonzero value.

```rust,ignore
fn example(index: *mut u32, mask: u32) -> u8 {
    {
        let __t0: *mut u32 = index;
        let __t1 = mask;
        if __t1 == 0 {
            0u8
        } else {
            {
                unsafe { __t0.write(__t1.trailing_zeros()) };
                1u8
            }
        }
    }
}
```

## `_umul128`

```c
unsigned long long example(unsigned long long a, unsigned long long b, unsigned long long *high) {
    return _umul128(a, b, high);
}
```

Translate with `--flavor=msvc --target=x86_64-pc-windows-msvc`. Slate returns the low 64 bits and writes the high 64 bits through `high`.

```rust,ignore
fn example(a: u64, b: u64, high: *mut u64) -> u64 {
    unsafe { _umul128(a as u64, b as u64, high as *mut u64) as u64 }
}
```
