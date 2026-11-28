# Size, alignment, and layout

- `_Alignas(N)` and `alignas(N)` request alignment in bytes. With a type
  argument, they request that type's alignment.
- `aligned(N)` requests alignment for an object, type declaration, or field.
  Bare `aligned` requests the target's biggest alignment. GCC mode also models
  it in a type name such as a `sizeof` operand.
- `packed` reduces alignment requirements on a record or field. See
  [Packed Records and Bit-fields](../../lowering/packed-records.md).
- `ms_struct` selects Microsoft record layout.
- `gcc_struct` selects GNU record layout. Both layout override spellings are
  gated to x86 in GCC mode.
- `mode(MODE)` changes the declared type to a supported machine mode.
- `vector_size(BYTES)` constructs a vector with the specified total size.
- `ext_vector_type(LANES)` constructs a Clang vector with the specified number
  of elements.
- `neon_vector_type(LANES)` constructs a Clang NEON vector type.
- `neon_polyvector_type(LANES)` constructs the corresponding Clang polynomial
  vector type.
- `arm_sve_vector_bits(BITS)` constructs a fixed-length SVE vector from an SVE
  type in GCC mode on AArch64.
- `transparent_union` lets a call supply a compatible member type directly.
  Calls that need an implicit union conversion can fail.
- `address_space(N)` accepts a Clang address space annotation. This attribute
  does not change Slate's type layout.
- `scalar_storage_order("ORDER")` accepts the GCC annotation. Slate does not
  change the byte order of fields based on it.
- `may_alias` is accepted in GCC and Clang modes. It does not change record
  layout or produce a Rust aliasing attribute.

## `packed` and `aligned`

```c
struct __attribute__((packed)) Header {
    unsigned char kind;
    unsigned int len;
};

unsigned int header_size(void) { return sizeof(struct Header); }
```

```rust
#[repr(C, packed)]
#[derive(Clone, Copy)]
struct Header {
    kind: u8,
    len: u32,
}

fn header_size() -> u32 {
    core::mem::size_of::<Header>() as u32
}
```

A record declared with `struct __attribute__((aligned(32))) Aligned` instead
emits `#[repr(C, align(32))]` when that alignment is representable.

## `mode`

```c
typedef unsigned int Byte __attribute__((mode(QI)));
Byte one(void) { return 1; }
```

The `QI` mode selects an eight-bit integer. The unsigned return type becomes
`u8`.

```rust
fn one() -> u8 {
    1
}
```

## `vector_size`

```c
typedef int Vec4 __attribute__((vector_size(16)));
Vec4 sum(Vec4 a, Vec4 b) { return a + b; }
```

Sixteen bytes hold four 32-bit lanes here. Slate emits a Rust SIMD type.

```rust
fn sum(
    a: core::simd::Simd<i32, 4>,
    b: core::simd::Simd<i32, 4>,
) -> core::simd::Simd<i32, 4> {
    return a + b;
}
```
