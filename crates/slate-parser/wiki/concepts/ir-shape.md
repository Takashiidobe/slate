# IR Shape

_discussion draft — numeric types proposed; not an implemented API_

Companion to [IR Spec](ir-spec.md). This document describes the fields of
individual IR nodes and where their associated information lives. Shapes
are pseudocode, not Rust implementation declarations. Proposals here remain
open until agreed; they do not silently replace decisions in the spec.

The reference will cover numeric types, enums, structs, unions, functions
(attributes, parameters, return type, body), and statements including
switch, for, while, and do/while. This first discussion defines numeric
types and their immediate supporting information. Other node shapes are
not specified yet.

## Information ownership

**Proposed:** distinguish required semantics, optional source context, and
derived analysis. Required information may live in referenced tables; a
side table does not by itself mean the information is optional.

| Information | Owner | Required for correct emission? |
| --- | --- | --- |
| Numeric value domain | Canonical numeric type | Yes |
| Object size and alignment | Resolved target representation; object/field overrides | Yes when stored |
| Calling convention and ABI classification | Resolved function signature/call contract | Yes at calls and FFI boundaries |
| Original C type and typedef chain | Origin of an individual type use | No, after semantic and representation decisions are resolved |
| Source spelling, literal radix/suffix, macro/header origin | Node/type-use origin metadata | No |
| Overflow and conversion behavior | Operation | Yes |
| Volatile/atomic access behavior | Memory access operation, with required storage properties on the object/type | Yes |
| Proven range, known bits, no-overflow proof | Analysis facts for a value or operation | No |

Optional context can improve Rust names and idioms. It must not be needed
to reconstruct arithmetic, storage, or calling semantics. This refines the
spec's broad wording that C-specific information is optional metadata.

## Numeric type

**Proposed:** the numeric type describes a value domain. Keep integers and
floating-point formats explicit rather than a bag of flags such as
`is_float`, `is_signed`, and `is_long`.

```text
NumericType =
    Integer { bits: NonZeroU32, signedness: Signed | Unsigned }
  | Float { format: FloatFormat }

FloatFormat = Binary16 | Binary32 | Binary64 | X87Extended80 | Binary128
```

Integer `bits` is the value representation width, including the sign bit
for a signed integer. Signed values use two's-complement interpretation.
The width is not restricted to native Rust integer widths, so bit-precise
integers can retain their actual domain. The target controls which widths
are supported. The printer renders these as `i32`, `u64`, `i17`, etc.

Float format identifies the numeric representation, not just storage size.
The initial format set above is a proposal; decimal and other extension
formats need distinct identities if supported. Do not silently approximate
an unsupported format with a native Rust float. Rounding/evaluation behavior
belongs to the operation or an explicit semantic environment it references.

`Bool` is a separate type with two values, not `Integer { bits: 1, ... }`.
Enums retain a separate identity and refer to an underlying integer type;
their full shape will be discussed separately. Complex, vector, and
fixed-point types likewise need their own shapes.

The type contains no literal value, variable name, source location, C rank,
typedef name, range proof, or overflow policy. C promotions and usual
arithmetic conversions have already been resolved into operations.

## Type identity and representation

**Proposed:** intern value-domain types in a module table and reference them
by `TypeId`. Identical integer domains share a type even when different C
spellings produced them.

```text
module.types[TypeId] = NumericType

NumericRepresentation {
    value_type: TypeId,
    size_bytes: u64,
    abi_alignment_bytes: u32,
    encoding: ResolvedNumericEncoding,
}
```

`ResolvedNumericEncoding` stands for the required mapping between the value
domain and object storage, including padding where applicable; its concrete
shape is open. The target module owns byte order. Representations are
resolved by the frontend, not inferred by the Rust emitter from C names.
An object refers to its representation; field packing or explicit object
alignment is recorded at the field/object rather than changing every use
of the numeric type.

Value width is not necessarily object size. In particular, do not derive
storage size or alignment merely by dividing `bits` by eight, or treat
`f80` as a promise of a ten-byte object. If two C types have the same value
domain but different storage or ABI requirements, sharing `TypeId` must not
erase those requirements. Their object representations and resolved call
contracts preserve the distinction.

This keeps pure arithmetic independent of storage while giving Rust enough
information to represent objects and foreign signatures correctly.

## Source context belongs to a type use

**Proposed:** attach C context to each use of a type, not to the canonical
numeric type. A parameter, return type, field, local, or expression can have
its own origin even when all share one `TypeId`.

```text
TypeUse {
    ty: TypeId,
    origin: Option<TypeOriginId>,
}

TypeOrigin {
    c_type: CTypeOriginId,
    source: Option<OriginId>,
}
```

`CTypeOriginId` references an interned description of the original C type
and typedef chain, including declaration identities for aliases. The same
mechanism can later describe nested pointer and aggregate type uses.
`OriginId` references spelling/expansion locations and macro/header context
as described in the IR spec. These are optional to consume.

For example, on the initial LP64 target, `size_t n` and
`unsigned long flags` can both use `Integer { bits: 64, signedness: Unsigned }`.
Their type-use origins distinguish `size_t` from `unsigned long`. A Rust
rewriter can use the alias and header identity to recognize `size_t`;
ordinary arithmetic does not need that information. A typedef's name alone
does not prove it is the standard library type.

## Numeric constants and operations

**Proposed:** a constant value belongs to an expression node, separate from
the type. The surrounding expression provides its node ID and type use.

```text
NumericConstant = IntegerBits(BitVector) | FloatBits(BitVector)
```

Integer bits match the resolved integer width; the type supplies signedness.
Float bits encode the value in its resolved format and preserve distinctions
such as signed zero and NaN payloads. Object padding is not part of this
value encoding. Keep original radix, suffix, and spelling in node origin
metadata after typing and conversion; do not use a host `i64` or `f64` as
the universal constant representation.

An addition references its operand/result types and carries its required
overflow behavior. A conversion carries its actual conversion semantics;
why C inserted it is optional origin information. A proof that a particular
addition cannot overflow belongs to analysis facts keyed by its node ID.
It does not change the integer type shared by every other expression.

## Questions for this discussion

1. Does this split work: small numeric value-domain types, required target
   representation information, and optional C context on each type use?
2. Should representation be a separate referenced record as proposed, or
   part of the canonical type key? The former shares arithmetic types;
   the latter simplifies storage-type lookup but duplicates equal domains.
3. Is the proposed float-format set sufficient for the first implementation?
   Other formats should be explicit support decisions.
