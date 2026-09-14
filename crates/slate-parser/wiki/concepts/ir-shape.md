# IR Shape

_design discussion — agreed choices marked Decided; not an implemented API_

Companion to [IR Spec](ir-spec.md). This document describes the fields of
individual IR nodes and where their associated information lives. Shapes
are pseudocode, not Rust implementation declarations. Proposals here remain
open until agreed. Decided sections record the agreed shape choices.

This reference covers numeric types, typed constants and bindings, string
literal objects, loops, jumps, switches, enums, structs, and unions.
Functions (attributes, parameters, return type, and body) remain a separate
discussion. Newly proposed shapes below are design choices for review.

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
Enums retain a separate identity and refer to an underlying integer type
(see [Enums](#enums)). Complex, vector, and
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

## String literal objects

**Decided:** retain both the string and its exact bytes so Rust lowering
can select the useful view directly. A string literal represents an array
object with static storage; an address derived from it is a separate value.
Its character element type is resolved for the target, retaining the C
character type as metadata.

Proposed field shape:

```text
StringLiteral {
    id: ObjectId,
    ty: Type { kind: Array { element: Type, length: N }, metadata: ... },
    storage_duration: Static,
    writable: false,
    contents: StringContents,
    metadata: { source_pieces: ..., encoding_prefix: ..., origin: ... },
}

StringContents {
    code_units: U8Sequence | U16Sequence | U32Sequence,
    bytes: ByteSequence,
    text: Option<String>,
}
```

The array type's storage metadata carries its target size and alignment.
The element type carries its own storage representation and `c_type`,
distinguishing ordinary `char`, UTF character types, and `wchar_t` even
when their numeric representations match. Code-unit widths and encodings
are resolved by IR sema for the target; additional target representations
must be modeled explicitly if needed.

### Three views of the same contents

- `code_units` contains the decoded character units, including the final
  terminating zero. Array length `N` counts these units, not Unicode
  characters or storage bytes. Embedded zeros are preserved.
- `bytes` contains the exact target representation of those units,
  including the terminator, in target byte order. Rust lowering can use
  this view without re-encoding the source literal. Its length agrees with
  the array's resolved storage size.
- `text` contains the corresponding Unicode text when decoding is valid
  and lossless under the resolved encoding. It excludes only the one
  implicit final terminator; explicit embedded or trailing zeros remain.
  Invalid or undecodable sequences yield `None`, never replacement
  characters or truncated contents. Rust lowering then uses units or bytes.

IR sema produces these views together; they must describe the same
contents. `text` is a convenient decoded string, not the original spelling
with C escape syntax. Original literal pieces and prefixes remain origin
metadata, including after adjacent literals are concatenated.

For ordinary one-byte characters on the initial target:

```text
source:     "a\0b"
code_units: [97, 0, 98, 0]
bytes:      [97, 0, 98, 0]
text:       Some("a\0b")
length:     4
```

The `text` line uses escaped notation to display an actual embedded zero.
For wide or UTF-16 literals, the text's UTF-8 bytes need not equal the
object's target bytes. Rust lowering chooses a view compatible with the
required element representation and use; the views are not interchangeable
memory layouts. It also checks interior-zero constraints when choosing a
C-string representation.

### Object and initialization behavior

Using the literal in pointer context produces the address of its first
element. The object remains an array, available for size computations and
array initialization. `char s[] = "abc"` creates a separate writable array;
it does not turn `s` into a pointer to the literal. Initializer lowering
uses the destination's resolved extent and initialization rules, rather
than assuming every destination copies every byte of the literal object.

`writable: false` describes the literal object's access contract. It does
not rewrite the original C element type to `const char`. Object identity
is represented independently of content equality; sharing content buffers
must not itself decide whether literal object addresses are merged.

## Structured control flow

**Proposed:** keep statement regions with typed expression trees. The
generated `clang-ir-types` bindings informed the region split: CIR `For`
has condition/body/step regions, `While` and `Do` have condition/body
regions, and `Switch` contains potentially nested `Case` regions. The
following shapes add resolved IDs and explicit normal-exit destinations.

Common node IDs and origin metadata are implicit in shapes that do not
show them. `LoopId`, `SwitchId`, and `CaseId` identify their corresponding
nodes. A region is a statement container, not automatically a C scope:

```text
Region {
    statements: Vec<Stmt>,
    normal_exit: Continuation,
}

EvalBlock {
    statements: Vec<Stmt>,
    result: Value,
}

Continuation =
    After(NodeId)
  | LoopCondition(LoopId)
  | LoopStep(LoopId)
  | CaseEntry(CaseId)
  | Exit(LoopId | SwitchId)

Scope {
    id: ScopeId,
    body: Region,
    lifetime: ScopeLifetimeInfo,
}
```

`normal_exit` is followed only when execution reaches the end of a region.
`return`, `goto`, `break`, and `continue` transfer control immediately and
do not also execute that exit. `After(node)` resumes at the node's lexical
continuation, preserving enclosing loop tests and other structured flow.
These references do not duplicate statements or require SSA block arguments.

An `EvalBlock` executes its statements and then evaluates its result once,
when that block is reached. Conditions have a `Bool` result with C truth
conversions already explicit. This preserves side effects in loop headers.
Scope lifetime information covers object lifetimes and applicable cleanup
actions; region boundaries alone neither create nor end local lifetimes.
Edges that cross scopes must honor this information. Cleanup details can be
extended independently of the loop shapes.

### C-style for

```text
For {
    id: LoopId,
    scope: ScopeId,
    init: Region,
    condition: EvalBlock,
    step: Region,
    body: Region,
}
```

The init region can contain declarations or expression statements. It runs
once on normal entry, with `normal_exit = LoopCondition(id)`. Initializer
declarations belong to the loop's scope, covering its header and body.
An absent C condition becomes constant `true`; absent init and step become
empty regions. Their original omission is optional source metadata.

| Event | Destination |
| --- | --- |
| Condition true | Body entry |
| Condition false | `Exit(id)` |
| Body completes normally | `LoopStep(id)` |
| `continue` targeting this loop | `LoopStep(id)` |
| Step completes normally | `LoopCondition(id)` |
| `break` targeting this loop | `Exit(id)` |

This retains a C for loop without forcing it into a Rust iterator range.
Any later iterator rewrite must establish the required facts about bound
evaluation, mutation, and step behavior.

### While and do/while

```text
While {
    id: LoopId,
    scope: ScopeId,
    condition: EvalBlock,
    body: Region,
}

DoWhile {
    id: LoopId,
    scope: ScopeId,
    body: Region,
    condition: EvalBlock,
}
```

`While` enters through its condition. `DoWhile` enters through its body.
For both, body normal completion and `continue` reach `LoopCondition(id)`;
condition true reaches the body, and condition false reaches `Exit(id)`.
`break` reaches `Exit(id)` directly. A do/while condition is not executed
on a break path. The scope table retains any nested compound-body scope.

### Break, continue, labels, and goto

```text
Break { target: LoopId | SwitchId }
Continue { target: LoopId }

Label {
    id: LabelId,
    name: Option<String>,
    scope: ScopeId,
}

Goto { target: LabelId }
LabelAddress { target: LabelId }
IndirectGoto { destination: Value }
```

Labels mark positions in region statement lists. A label is not a scope
and does not own the following statements. Label IDs resolve forward
references and GNU local-label shadowing; the spelling is for printing.
Direct gotos may cross region boundaries within the enclosing function.
Taking a label's address produces a typed value; an indirect goto evaluates
its destination once. A computed target set, if known, is an analysis fact.

`continue` inside a switch within a loop names that loop; `break` names the
switch. Their destination does not depend on re-searching enclosing nodes.
Entering a label bypasses statements preceding it, including initializers.
IR sema checks forbidden scope entries and records scope/lifetime effects;
Rust lowering must not execute bypassed initializers to satisfy Rust syntax.

### Switch, cases, and fallthrough

```text
Switch {
    id: SwitchId,
    scope: ScopeId,
    discriminant: EvalBlock,
    body: Region,
    cases: Vec<CaseId>,
    default: Option<CaseId>,
}

Case {
    id: CaseId,
    owner: SwitchId,
    selectors: Vec<CaseSelector>,
    body: Region,
}

CaseSelector =
    Equal(IntegerConstant)
  | Range { low: IntegerConstant, high: IntegerConstant }
  | Default
```

Evaluate the discriminant once with integer promotions already resolved.
Case constants have the discriminant's comparison type; ranges are
inclusive and need not be expanded into individual values. IR sema checks
overlaps and duplicate defaults. Adjacent labels with a common entry can
share a `Case` with multiple selectors, including a default selector.

`cases` indexes the case nodes owned by this switch, including cases nested
in its body but excluding those owned by nested switches. Bodies are stored
once in the statement tree, not copied into this index. `default` indexes
the one case containing `Default`; its absence means a nonmatching value
goes to `Exit(id)`. Dispatch enters the selected case directly, not the
beginning of the switch body.

**Use explicit exits, not a per-case fallthrough boolean.** In a simple
switch, a case body's normal exit is `CaseEntry(next_case)` or `Exit(id)`.
A `break` remains a separate statement, so a conditional break can coexist
with a normal fallthrough path. A default can appear anywhere in the body.

```text
case 1:
    if stop { Break { target: switch_id } }
    work();
    normal_exit: CaseEntry(case_2)

case 2:
    more_work();
    normal_exit: Exit(switch_id)
```

For nested cases, normal completion may instead resume with `After(case)`
inside an enclosing statement. That continuation retains intervening
conditions, loop steps, and scope exits. Do not replace it with a direct
jump to the next case by source order. This supports Duff's-device-style
entry into a loop body and labels reachable by goto before the first case.
Switch cases do not introduce scopes; explicit `Scope` nodes preserve the
actual C scopes. Normal completion of the outer switch body exits the switch.

The full representation always works; a derived simple-arm view can expose
top-level cases to Rust `match` lowering. Enum-case coverage and whether
fallthrough is reachable are analysis facts, not promises that a switch
has no other possible input values. An explicit source `fallthrough`
annotation is origin metadata; the executable edge determines behavior.

## Enum and record type identity

**Proposed:** named and anonymous tags have stable identity in module
tables. Types refer to definitions by ID, allowing forward declarations
and recursive pointers without copying the definition at each use.

```text
TypeKind = ... | Enum(EnumId) | Struct(RecordId) | Union(RecordId)

module.enums[EnumId] = EnumDefinition
module.records[RecordId] = StructDefinition | UnionDefinition
```

Names and aliases are useful for output but do not establish type identity.
Distinct anonymous declarations remain distinct types; multiple declarators
sharing one tag definition share its ID. Contextual type metadata can still
carry typedef names and qualifiers at each use.

The earlier `StorageMetadata` shape described numeric storage. Aggregate
type storage metadata instead references the layout below. Shared layout
data lives once with its type definition; references are still type-owned
storage metadata, not optional analysis facts.

### Enums

```text
EnumDefinition {
    id: EnumId,
    name: Option<String>,
    underlying: Option<Type>,
    enumerators: Option<Vec<Enumerator>>,
    metadata: { storage: Option<StorageMetadata>, origin: ... },
}

Enumerator {
    id: EnumeratorId,
    name: String,
    value: Constant,
    metadata: { origin: ..., attributes: ... },
}
```

`underlying` is the resolved integer representation, selected by IR sema
using the target, compiler mode, and any explicit underlying type. Every
complete enum has it and matching storage metadata. A supported incomplete
enum may lack both; an explicit known underlying type can supply storage
even before enumerators are defined. `enumerators: None` means no definition
is available, not an empty list of constants.

Enumerators retain names, source order, duplicate numeric values, and their
resolved constant types. Do not assume an enumerator expression's type is
always the enum type. Implicit values are evaluated by sema, and arithmetic
uses explicit conversions while retaining enum origin for contextual
lowering. The enumerator list is not a closed validity set for stored values.

This lets Rust lowering choose integer constants, a newtype, flags, or a
Rust enum when justified. It must not assume that every value names exactly
one enumerator, nor infer a flags representation from the type name alone.

### Structs and unions

```text
StructDefinition {
    id: RecordId,
    name: Option<String>,
    fields: Option<Vec<Field>>,
    metadata: RecordMetadata,
}

UnionDefinition {
    id: RecordId,
    name: Option<String>,
    fields: Option<Vec<Field>>,
    metadata: RecordMetadata,
}

Field {
    id: FieldId,
    name: Option<String>,
    ty: Type,
    kind: Ordinary | AnonymousMember | BitField { width: u32 }
        | FlexibleArray,
    metadata: { origin: ..., attributes: ... },
}

RecordMetadata {
    storage: Option<RecordStorage>,
    origin: ...,
    attributes: ...,
}

RecordStorage {
    size_bytes: u64,
    alignment_bytes: u32,
    fields: Map<FieldId, FieldStorage>,
    bit_field_units: Vec<BitFieldUnit>,
}

FieldStorage =
    Object { offset_bytes: u64, alignment_bytes: u32 }
  | Bits { slices: Vec<BitSlice>, signed: bool }
  | NoStorage

BitFieldUnit {
    id: UnitId,
    offset_bytes: u64,
    storage_type: Type,
    alignment_bytes: u32,
}

BitSlice {
    unit: UnitId,
    unit_bit_offset: u32,
    value_bit_offset: u32,
    width: u32,
}
```

`fields: None` and absent storage mean an incomplete record. A complete
record has both fields and resolved storage, even for an accepted empty
record extension. Fields remain in declaration order. Their types carry
constness and C type metadata; the record stores physical placement and
effective member alignment after target packing/alignment rules.

For structs, ordinary fields have distinct storage placements. For unions,
members overlap the same object storage; they are alternative views, not
a struct's consecutive fields. Overall size includes tail padding and
alignment requirements. A union has no runtime tag or type-level active
member field. Initializers and member accesses name the selected `FieldId`;
later analysis may track which member was written.

Anonymous aggregate members keep a field ID and their nested record type.
Name resolution expands promoted member names into projection paths through
those IDs, so `p->x` does not require Rust lowering to repeat anonymous
member lookup. Synthesized Rust names belong to emission.

Flexible array members use an explicit flexible-array type/field kind,
not a zero-length fixed array. Store their element type and offset;
`RecordStorage.size_bytes` describes the base object, not an arbitrary
runtime tail. Any known extra allocation extent belongs to the object or
analysis facts. Zero-length array extensions remain distinguishable.

### Bit-fields and padding

Keep logical fields separate from storage units, following the useful
distinction in the local CIR bindings. A bit-field's declared type and
width cannot be recovered from the unit holding it. Several fields can
share a unit; `BitSlice` explicitly maps value bits to unit bits, including
multiple slices if required by a supported layout. Offsets count from the
least significant bit of the decoded unit value; target byte order is
handled by the unit's storage representation.

Unnamed fields remain represented. A zero-width field has `NoStorage`;
its layout effect is already reflected in following offsets. Reads and
writes project by field ID and use the resolved layout. Access operations
also retain volatile/atomic and effective-alignment requirements; a byte
layout alone does not authorize a particular machine access width.

Padding is storage information, not a user field. Record size, member
placements, and unused portions of bit-field units describe it without
inventing padding fields in the logical field list. Rust lowering can
introduce physical padding or backing fields when needed. Layout-affecting
attributes have resolved effects in storage metadata; original spelling
can remain contextual metadata. Other behavior-affecting attributes must
likewise be resolved before an emitter can ignore their source spelling.

## Reference points

The local generated bindings consulted for these proposals are
`~/Projects/clang-ir/clang-ir-types/src/ops/control_flow.rs` (`For`, `While`,
`Do`, `Case`, `Switch`, `Goto`, `Yield`), `src/types.rs` (`Struct`, `Union`,
`BitField`), and `src/attrs.rs` (`BitFieldDecl`, `BitfieldInfo`, `RecordLayout`).
They motivate regions and the logical-field/storage-unit distinction.
This design uses typed IDs instead of symbolic lookup and omits CIR's
implicit parent-dependent yield behavior and C++-specific record fields.
The generated type enum has no dedicated C enum variant; the enum shape
above is a Slate-specific proposal.

For source semantics, the [C11 draft, sections 6.8.4–6.8.6](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
describes selection, loops, and jumps. GNU extensions include
[inclusive case ranges](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html)
and [labels as values](https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html).
These references are background; the shapes above are our design proposals.

## Questions for this discussion

1. Does the proposed variable/value split capture the intended distinction
   between declaration storage classes and type storage metadata?
2. Do structured regions with explicit normal exits provide the desired
   balance for switches, including cases nested inside loops?
3. Are logical fields plus type-owned storage layouts sufficient for the
   record information Rust lowering should consume?
