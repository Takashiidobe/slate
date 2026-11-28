# Vector and target intrinsics

- `__builtin_shufflevector` selects lanes. Rust lowering requires a supported
  vector shape and constant lane selection.
- `__builtin_convertvector` converts lanes to the destination element type.
- `__builtin_reduce_*` covers add, multiply, and, or, xor, and supported minimum
  and maximum reductions.
- `__builtin_elementwise_*` includes popcount, min, max, fused multiply-add, and
  square root.
- `__builtin_ia32_*` includes mapped x86 intrinsics and explicit expansions for
  selected shuffles, extracts, inserts, comparisons, blends, and masked loads
  and stores. Immediate operands must fold to constants.
- `__rdtsc` reads the x86 time-stamp counter.
- `__builtin_neon_*` includes Clang NEON lane operations, reductions, saturating
  arithmetic, loads and stores, rounding, conversions, and table lookups.
  Polymorphic forms depend on the constant NEON type code.
- GCC AArch64 `sv*` intrinsics from `arm_sve.h` use matching Rust
  `core::arch::aarch64` functions for mapped names and types. The
  [SVE mapping](https://github.com/Takashiidobe/slate/blob/main/crates/slate/src/frontend/lowerer/sve_table.rs)
  lists the individual entries.

See [Builtin Inventories](inventories.md) for individual architecture builtin
names and the LLVM alias catalog.

## `__builtin_shufflevector`

```c
typedef int Vec4 __attribute__((vector_size(16)));
Vec4 reverse(Vec4 x) { return __builtin_shufflevector(x, x, 3, 2, 1, 0); }
```

Slate emits a SIMD swizzle with a constant lane selection.

```rust,ignore
fn reverse(x: core::simd::Simd<i32, 4>) -> core::simd::Simd<i32, 4> {
    return core::simd::simd_swizzle!(x, x, [3, 2, 1, 0]);
}
```

A runtime lane index fails during semantic analysis.

```c
typedef int Vec4 __attribute__((vector_size(16)));
Vec4 select(Vec4 x, int lane) { return __builtin_shufflevector(x, x, lane, 1, 2, 3); }
```

```text
nonconstant or unknown identifier
```

## `__rdtsc`

```c
unsigned long long timestamp(void) { return __rdtsc(); }
```

On x86-64, Slate calls the LLVM intrinsic that reads the processor's time-stamp
counter.

```rust,ignore
unsafe extern "llvm-intrinsic" {
    #[link_name = "llvm.x86.rdtsc"]
    fn __slate_intrinsic_9e72945d5c6a2d09() -> u64;
}

fn timestamp() -> u64 {
    unsafe { __slate_intrinsic_9e72945d5c6a2d09() }
}
```

## `__builtin_convertvector`

```c
typedef int Int4 __attribute__((vector_size(16)));
typedef float Float4 __attribute__((vector_size(16)));
Float4 example(Int4 value) {
    return __builtin_convertvector(value, Float4);
}
```

Slate casts each lane from `int` to `float`.

```rust,ignore
fn example(value: core::simd::Simd<i32, 4>) -> core::simd::Simd<f32, 4> {
    return core::simd::num::SimdInt::cast::<f32>(value);
}
```

## `__builtin_reduce_add`

```c
typedef int Int4 __attribute__((vector_size(16)));
int example(Int4 value) {
    return __builtin_reduce_add(value);
}
```

Slate lowers this horizontal sum to the LLVM reduction for four `i32` lanes.

```rust,ignore
unsafe extern "llvm-intrinsic" {
    #[link_name = "llvm.vector.reduce.add.v4i32"]
    fn __slate_intrinsic_98bc3c92f4c5619d(arg0: core::simd::Simd<i32, 4>) -> i32;
}

fn example(value: core::simd::Simd<i32, 4>) -> i32 {
    unsafe { __slate_intrinsic_98bc3c92f4c5619d(value) }
}
```

## `__builtin_elementwise_fma`

```c
typedef float Float4 __attribute__((vector_size(16)));
Float4 example(Float4 x, Float4 y, Float4 z) {
    return __builtin_elementwise_fma(x, y, z);
}
```

Slate performs a fused multiply-add on each lane.

```rust,ignore
fn example(
    x: core::simd::Simd<f32, 4>,
    y: core::simd::Simd<f32, 4>,
    z: core::simd::Simd<f32, 4>,
) -> core::simd::Simd<f32, 4> {
    return unsafe { __slate_intrinsic_8cfacf8a53c42a36(x, y, z) };
}
```
