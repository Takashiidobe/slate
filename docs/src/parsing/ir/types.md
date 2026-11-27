# Types and Layout

The IR uses concrete types for computation and storage. The effective C target
determines the integer widths, floating formats, and layout. Written C type
names can still be carried as source context.

## Compare two targets

**C input**

```c
long word;
#pragma pack(push, 1)
struct record { char tag; int value; };
#pragma pack(pop)
struct record sample;
```

**Linux x86-64**

```text
global %0 word: i64 [storage=static] [linkage=external];
```

**Windows x64**

```text
global %0 word: i32 [storage=static] [linkage=external];
```

The same C declaration needs 64 bits on this Linux target and 32 on this Windows
target. Consumers work from the concrete type instead of resolving the spelling
`long` again.

## Read a packed record

```text
type @type0 record = struct {
    field0 tag: i8;
    field1 value: i32;
} [size=5, align=1, offsets=[0, 1], underaligned=[1]];
```

The offset, total size, and alignment give lowering the storage shape to
preserve. The underaligned field marker also affects how the generated Rust may
access `value`. A normal Rust reference would require its usual alignment, even
though the field is only one byte into the record.

## Keep the shape of a vector

**C input**

```c
typedef int lanes __attribute__((vector_size(16)));
lanes add_lanes(lanes a, lanes b) { return a + b; }
```

**Typed IR excerpt**

```text
type @type0 lanes = vector<i32, 4>;
return add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%2) [c="lanes"], read<vector<i32, 4>>(%3) [c="lanes"]) [c="lanes"];
```

The IR records the lane type and count, elementwise behavior, and overflow
policy. A vector is more than a block of bytes of the same size. Those
properties tell a consumer how to compute its result. Other type families carry
similar distinctions, such as the floating format of a complex value's
components.
