# IR Shape

_design discussion — agreed choices marked Decided; not an implemented API_

Companion to [IR Spec](ir-spec.md). This document describes the fields of
individual IR nodes and where their associated information lives. Shapes
are pseudocode, not Rust implementation declarations. Proposals here remain
open until agreed. Decided sections record the agreed shape choices.

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
| Object size, alignment, representation | Storage metadata on the type; object/field overrides | Yes when stored |
| Declared constness | Type metadata | Required where qualification constrains access |
| Storage class, storage duration, linkage | Variable declaration/object | Yes where applicable |
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

**Decided:** use explicit numeric variants, with C type information retained
as metadata for contextual Rust lowering.

```text
NumericType =
    I8 | I16 | I32 | I64 | I128
  | U8 | U16 | U32 | U64 | U128
  | F16 | F32 | F64 | F80 | F128
  | BitInt { width: NonZeroU32, signed: bool }
```

The printer uses `i32`, `u64`, `f80`, etc. `BitInt` represents `_BitInt(N)`
with its resolved width and signedness. Its width includes the sign bit
when signed, and is not restricted to native Rust integer widths. The
target controls which widths are supported.

Decimal floating-point types (`_Decimal*`, called `BF*` in this discussion)
are supported too. Their detailed variants and representation will be
worked out later; the list above covers the binary numeric variants.

`Bool` is a separate type with two values, not a one-bit integer.
Enums retain a separate identity and refer to an underlying integer type;
their full shape will be discussed separately. Complex, vector, and
fixed-point types likewise need their own shapes.

The numeric variant itself contains no literal value, variable name, C
rank, typedef name, range proof, or overflow policy. A type wraps the
variant with metadata, including `c_type`, storage, and constness.

## Target resolution and type metadata

**Decided:** IR semantic analysis (`src/ir/sema`), given the target and
compiler configuration, resolves C numeric types to concrete variants and
computes their widths and storage representation. For an explicit variant
such as `U64`, the numeric width is already fixed; sema resolves how that
type is represented on the target. For a C type such as `long`, sema first
chooses the appropriate concrete variant. It also evaluates `_BitInt(N)`'s
width. Rust lowering does not repeat these decisions.

This is the target-aware IR sema stage described in the IR spec, not the
later pass that derives ranges, aliasing, and other analysis facts.

**Decided:** storage information is metadata on the type, alongside
constness and the original C type. Proposed field shape after resolution:

```text
Type {
    kind: NumericType,
    metadata: TypeMetadata,
}

TypeMetadata {
    c_type: Option<CTypeOriginId>,
    storage: StorageMetadata,
    constness: Const | NonConst,
    origin: Option<OriginId>,
}

StorageMetadata {
    size_bytes: u64,
    alignment_bytes: u32,
    encoding: ResolvedNumericEncoding,
}
```

`ResolvedNumericEncoding` stands for the required mapping between the value
domain and object storage, including padding where applicable; its concrete
shape is open. The target module owns byte order. Storage metadata is
required when emitting storage-sensitive code, even though it is called
metadata. It describes how a value of the type is stored, not whether a
particular expression has an allocated object.

Field packing or explicit object alignment is recorded at the field/object
as an override of the type's storage metadata. Resolved calling semantics
remain on function signatures and calls.

Value width is not necessarily object size. In particular, do not derive
storage size or alignment merely by dividing `bits` by eight, or treat
`f80` as a promise of a ten-byte object. Equal numeric variants must not
erase distinct storage metadata or resolved calling requirements.

This keeps pure arithmetic independent of storage while giving Rust enough
information to represent objects and foreign signatures correctly.

## Contextual type uses

**Proposed:** a parameter, return type, field, local, or expression references
its own contextual `Type`. Types may be interned by their full contents;
interning only by `NumericType` must not merge different metadata.

```text
Type {
    kind: U64,
    metadata: {
        c_type: size_t,
        storage: { size_bytes: 8, alignment_bytes: 8, encoding: ... },
        constness: Const,
        origin: ...,
    },
}
```

This illustrates a `const size_t` type on the initial LP64 target.
`CTypeOriginId` references an interned description of the original C type
and typedef chain, including declaration identities for aliases. The same
mechanism can later describe nested pointer and aggregate type uses.
`OriginId` references spelling/expansion locations and macro/header context
as described in the IR spec. These are optional to consume.

For example, on the initial LP64 target, `size_t n` and
`unsigned long flags` can both have numeric kind `U64`.
Their type metadata distinguishes `size_t` from `unsigned long`. A Rust
rewriter can use the alias and header identity to recognize `size_t`;
ordinary arithmetic does not need that information. A typedef's name alone
does not prove it is the standard library type.

Constness records declared qualification, not inferred Rust binding
mutability. A non-const C local may become an immutable Rust binding when
analysis establishes that it is never written after initialization. Reads
and conversions carry the resulting value type; they do not blindly copy
every qualifier from the source object.

## Variables and computed values

**Proposed:** both variables and computed values carry a type. Only objects
have storage duration and object identity. Distinguish this from type
storage metadata, which describes representation.

```text
Variable {
    id: VariableId,
    name: String,
    ty: Type,
    storage_duration: Automatic | Static | Thread,
    linkage: None | Internal | External,
    declaration_metadata: { c_storage_class: ... },
    initializer: Option<Initializer>,
}

Parameter {
    id: VariableId,
    name: Option<String>,
    ty: Type,
    metadata: ...,
}

Value {
    id: NodeId,
    ty: Type,
    kind: Constant(...) | Read(Place) | Add(...) | ...,
}
```

These are partial shapes, not complete variable, parameter, or expression
definitions. `Initializer` includes expression and structured aggregate
initialization as described in the IR spec. An expression initializer can
be a constant, a read of another variable, or any supported computation.
Reads refer to resolved variable IDs through places; source names are
retained for printing. `int b = a` initializes `b` from the current value
of `a`; it does not make `b` an alias that follows later assignments to `a`.

A variable retains its own declared, resolved type even when its initializer
is another variable. Any required conversion is explicit in the initializer.
No initializer means no explicit initializer was supplied; storage duration
still determines whether semantic zero initialization is required. Later
assignments are write operations, not replacements of the declaration's
initializer field.

A parameter always carries its resolved type, including in an unnamed
prototype parameter. It has no declaration-time value or initializer: the
caller supplies an argument value. Parameter attributes and the remaining
function shape will be specified with functions.

Original C storage classes are declaration
metadata once their effects have been resolved into storage duration,
linkage, and any other required semantics. A computed value such as `a + b`
does not acquire a C storage class merely because its operands have one.
Its type still carries storage metadata describing its representation if
stored. When lowering needs an actual temporary object, that object carries
its own storage duration and lifetime.

## Numeric constants and operations

**Decided:** a constant node combines a type and a value. A `BitInt` type
contains width and signedness; the constant's payload supplies its value.
It remains a distinct type from ordinary integers even at equal width, so
IR sema can apply the appropriate rules before materializing conversions.

The logical constant shape is:

```text
Constant {
    ty: Type,
    value: NumericPayload,
}

NumericPayload = IntegerBits(BitVector) | FloatBits(BitVector)
```

For example, with the integer payload printed as a number:

```text
Constant {
    ty: Type { kind: BitInt { width: 17, signed: true }, metadata: ... },
    value: 3,
}
```

The same shape applies to ordinary integers and floats: a constant carries
`I32`, `U64`, `F64`, etc. together with its payload. In the uniform `Value`
wrapper above, `Value.ty` is the constant's type and `Value.kind` contains
the constant payload; do not duplicate the type in both places. These are
two views of the same typed node. Node identity and source origin follow
the common expression-node convention.

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

1. Does the proposed variable/value split capture the intended distinction
   between declaration storage classes and type storage metadata?
