# C Types, Layout, and ABI

Slate keeps C type identity in `CTypes` while sema checks the program. Two C
types can have the same machine width and still differ for `_Generic` or
redeclaration checks. For example, `int` and `long` can both be 32 bits on a
target, yet a generic selection can distinguish them.

## Carry layout into Rust

**C input**

```c
#pragma pack(push, 1)
struct record { char tag; int value; };
#pragma pack(pop)
```

**Typed IR excerpt**

```text
type @type0 record = struct {
    field0 tag: i8;
    field1 value: i32;
} [size=5, align=1, offsets=[0, 1], underaligned=[1]];
```

The layout lists offsets `[0, 1]` and marks field `1` as underaligned.

On a target with a four-byte `int`, `value` begins at offset one and can be
underaligned. Sema applies the active packing when it lays out the record. The
IR records the resulting layout and field access properties. Rust lowering uses
them when choosing how to read the field. Creating a normal Rust reference to
that underaligned field would be invalid.

## Record how calls cross the ABI

Sema classifies parameters and returns using the target and calling convention.
A value can travel directly, be split into register pieces, or be passed
indirectly. Rust lowering uses those classifications to pick a native C
signature or an ABI bridge when the ordinary Rust declaration would pass the
value differently.

**C input**

```c
__attribute__((ms_abi)) int add(int a, int b) { return a + b; }
```

**Typed IR excerpt**

```text
fn %0 @add(%1 a: i32, %2 b: i32) -> i32 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
    return add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2));
}
```

The `abi=win64 ms_abi` annotation carries the requested convention even on this
Linux target.

Start with `sema/ctype/` for C type identity, `sema/types.rs` for layout, and
`sema/abi.rs` for call classification.
