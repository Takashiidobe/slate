# Packed Records and Bit-fields

Slate preserves the target C layout of packed records, including member offsets,
alignment, and the bit positions used by bit-fields. It reads that layout from
IR and chooses Rust fields that fit it.

## Read and write a packed field

```c
struct __attribute__((packed)) Packed {
    char a;
    int b;
    char c;
};
```

```rust
#[repr(C, packed)]
struct Packed {
    a: i8,
    b: i32,
    c: i8,
}
```

On x86-64 Linux the fields start at bytes 0, 1, and 5. Both records have size 6
and alignment 1. The second field can sit at an address that would be unaligned
for a standalone `i32`. A direct assignment such as `p.b = value` or a by-value
read such as `let value = p.b` works on the packed field without forming an
aligned Rust reference to it.
[Packed struct fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/packed_struct.c)

## Keep bit-fields in their storage units

```c
struct __attribute__((packed)) MixedBits {
    unsigned char tag;
    unsigned int low : 3;
    signed int delta : 6;
    unsigned int : 0;
    unsigned long long wide : 35;
    unsigned int tail : 17;
};
```

In this layout, `low` and `delta` share the two bytes immediately after `tag`.
The unnamed zero-width field forces the next group to start on the next
four-byte boundary. Slate emits a byte of padding between the two groups and
uses the `bitfields` crate to define accessors over each group. The groups are
byte arrays, so the outer Rust struct can use `#[repr(C)]` and still achieve the
required alignment of one byte.

```rust
bits.__slate_bits_0.__set_low(7u32);
bits.__slate_bits_0.__set_delta(-17i32);
let delta = bits.__slate_bits_0.__get_delta();
```

A setter changes only its assigned bits, and the signed getter extends the sign
from the six-bit field to `i32`. The same C declaration can produce a different
layout on another target. Volatile bit-field accesses are a lowering barrier.
[Mixed bit-fields fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/packed_mixed_bitfields.c)
