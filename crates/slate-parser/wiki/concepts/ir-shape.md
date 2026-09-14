# IR Shape

_design discussion — agreed choices marked Decided; not an implemented API_

Companion to [IR Spec](ir-spec.md). This document describes the fields of
individual IR nodes and where their associated information lives. Shapes
are pseudocode, not Rust implementation declarations. Proposals here remain
open until agreed. Decided sections record the agreed shape choices.

This reference covers numeric types, typed constants and bindings, string
literal objects, loops, jumps, switches, enums, structs, and unions.
The control-flow/aggregate shapes and use of IDs are accepted. Function
and attribute shapes below are the next proposals for discussion.

## Information ownership

**Proposed:** distinguish required semantics, optional source context, and
derived analysis. Required information may live in referenced tables; a
side table does not by itself mean the information is optional.

| Information                                                | Owner                                                                        | Required for correct emission?                               |
| ---------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------------------------------------------------------ |
| Numeric value domain                                       | Canonical numeric type                                                       | Yes                                                          |
| Object size, alignment, representation                     | Storage metadata on the type; object/field overrides                         | Yes when stored                                              |
| Declared constness                                         | Type metadata                                                                | Required where qualification constrains access               |
| Storage class, storage duration, linkage                   | Variable declaration/object                                                  | Yes where applicable                                         |
| Calling convention and ABI classification                  | Resolved function signature/call contract                                    | Yes at calls and FFI boundaries                              |
| Original C type and typedef chain                          | Origin of an individual type use                                             | No, after semantic and representation decisions are resolved |
| Source spelling, literal radix/suffix, macro/header origin | Node/type-use origin metadata                                                | No                                                           |
| Overflow and conversion behavior                           | Operation                                                                    | Yes                                                          |
| Volatile/atomic access behavior                            | Memory access operation, with required storage properties on the object/type | Yes                                                          |
| Proven range, known bits, no-overflow proof                | Analysis facts for a value or operation                                      | No                                                           |

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

**Decided:** semantic analysis (`src/sema/`), given the target and
compiler configuration, resolves C numeric types to concrete variants and
computes their widths and storage representation. For an explicit variant
such as `U64`, the numeric width is already fixed; sema resolves how that
type is represented on the target. For a C type such as `long`, sema first
chooses the appropriate concrete variant. It also evaluates `_BitInt(N)`'s
width. Rust lowering does not repeat these decisions.

This is the target-aware IR sema stage described in the IR spec, not the
later pass that derives ranges, aliasing, and other analysis facts.

Sema constructs typed IR directly; there is no separate semantic AST.
The first implemented slice uses `Value { ty, node: Span<ValueKind> }`
with constants and addition, preserving spelling/expansion spans, node IDs,
header provenance, and macro origins. Numeric types currently carry only
width and integer signedness. The complete type/storage metadata below
remains the design target; see [implemented numeric seed](ir-spec.md#implemented-numeric-seed)
for current scope and dump examples.

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
caller supplies an argument value. The [function proposal](#functions-callable-type-and-named-entity)
below refines this partial parameter shape into signature types, parameter
metadata, and definition bindings.

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

**Decided:** keep statement regions with typed expression trees. The
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

| Event                          | Destination         |
| ------------------------------ | ------------------- |
| Condition true                 | Body entry          |
| Condition false                | `Exit(id)`          |
| Body completes normally        | `LoopStep(id)`      |
| `continue` targeting this loop | `LoopStep(id)`      |
| Step completes normally        | `LoopCondition(id)` |
| `break` targeting this loop    | `Exit(id)`          |

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

**Decided:** named and anonymous tags have stable identity in module
tables. Types refer to definitions by ID, allowing forward declarations
and recursive pointers without copying the definition at each use.

```text
TypeKind = ... | Enum(EnumId) | Struct(RecordId) | Union(RecordId)
           | Function(FunctionType)

module.enums[EnumId] = EnumDefinition
module.records[RecordId] = StructDefinition | UnionDefinition
```

Names and aliases are useful for output but do not establish type identity.
Distinct anonymous declarations remain distinct types; multiple declarators
sharing one tag definition share its ID. Contextual type metadata can still
carry typedef names and qualifiers at each use.

Names are retained through semantic lowering. IDs join references to the
correct definition; names and typedef metadata guide readable Rust output.
Rust lowering can choose an output name once per ID and reuse it at every
reference. Ownership of children remains structural even when nodes have
IDs for references or metadata.

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

## Functions: callable type and named entity

**Proposed:** separate a callable type from the named function that has it.
A function pointer needs a signature but has no function body or symbol
definition. Keep concrete source-level parameter and result types rather
than rewriting the signature into machine registers or hidden ABI operands.

```text
FunctionType {
    parameters: Prototype { fixed: Vec<Type>, variadic: bool }
              | Unprototyped,
    result: Type,
    abi: ResolvedCallingConvention,
}

Function {
    id: FunctionId,
    name: String,
    ty: FunctionType,
    parameter_metadata: Vec<ParameterMetadata>,
    result_metadata: ResultMetadata,
    symbol: FunctionSymbol,
    implementation: FunctionImplementation,
    attributes: FunctionAttributes,
    metadata: { origins: ..., c_storage_class: ..., annotations: ... },
}

FunctionImplementation =
    Declaration
  | Definition(FunctionDefinition)
  | Alias { target: FunctionId }
  | Resolver { resolver: FunctionId }
```

`result: Void` means no returned value. No-return behavior is a function
contract, not a replacement of the C signature's result type with `Void`
or `Never`. A declaration is distinct from a definition with an empty body.
Aliases retain symbol identity and point to another function; they are not
synthetic wrapper bodies. `Resolver` reserves the distinct symbol behavior
of an indirect-function resolver rather than treating it as an ordinary
function call on every invocation.

The function's source name and emitted linkage name may differ. Redeclarations
of the same entity join on `FunctionId`; sema checks compatibility and
merges their effective information rather than selecting the last spelling.
Definition parameter names identify its bindings; names in other prototypes
are retained as origin context. A function's ID and name remain available
when its address is used as a value.

### Parameters, return information, and bodies

```text
ParameterMetadata {
    name: Option<String>,
    origin: ...,
    contracts: Vec<ParameterContract>,
    hints: Vec<ParameterHint>,
}

ResultMetadata {
    origin: ...,
    contracts: Vec<ResultContract>,
    hints: Vec<ResultHint>,
}

FunctionDefinition {
    parameters: Vec<ParameterBinding>,
    entry: Vec<Stmt>,
    body: Scope,
}

ParameterBinding {
    parameter: ParamIndex,
    variable: VariableId,
    name: Option<String>,
    ty: Type,
}

Return { value: Option<Value> }
```

`ParamIndex` is a zero-based signature position, not a source parameter
name. Prototype parameter metadata is positional, matching the fixed type
list. Function types omit names and binding IDs, but their contextual
parameter/result types still carry `c_type`, storage, and qualifiers.

Definition bindings are the parameter objects read and written by the body.
Their type can retain top-level qualification that is irrelevant to the
callable type. Array/function parameter adjustment is already resolved in
the signature; original array form and bounds remain contextual information
or explicit contracts where they constrain valid calls. A bound does not
automatically turn a pointer parameter into a Rust slice.

`entry` executes once per call and contains any required incoming-value
conversions or parameter-bound evaluations. It is not re-entered by a goto
to a label in the body. The body owns nested scopes, locals, and statements;
references inside it use resolved variable, function, type, and label IDs.
Return values include explicit conversions to the result type. Returning a
record remains a typed aggregate return, not a synthetic output pointer.

For old-style definitions, retain known incoming parameter types separately
from local binding types when default promotions require an entry conversion.
An unprototyped declaration does not mean zero parameters and is not a
variadic prototype. Its complete definition-entry shape is an open detail;
do not fabricate a zero-argument signature to fit it into the prototype form.

Body fallthrough must have explicit function-end semantics. Void functions
can return without a value and `main` gets its implicit zero return. For
other non-void functions, do not automatically infer a no-return contract
from a missing return statement. The precise missing-result representation
is an open item below, including whether a caller uses that result.

### Symbol and ABI information

```text
FunctionSymbol {
    link_name: String,
    linkage: Internal | External,
    visibility: Default | Hidden | Protected,
    binding: Strong | Weak,
    emission: ResolvedFunctionEmission,
    section: Option<String>,
    alignment_bytes: Option<u32>,
    retention: { compiler_used: bool, linker_retain: bool },
}
```

`ResolvedFunctionEmission` records whether this translation unit supplies
the linkable definition, only a local inline body, or a reference, together
with required import/export behavior. Its final variant list must reflect
the supported target object formats. A body alone does not determine this.
Resolve the language-mode-dependent effects of C `inline`, `extern inline`,
and related attributes here. Inline performance preferences stay separate.

The ABI belongs to the callable type and call sites, including indirect
calls. IR sema resolves the target calling convention and any signature
adaptations Rust cannot express directly. Ordinary struct passing uses the
existing concrete type/layout and the corresponding Rust foreign ABI.
Register assignments, stack slots, and hidden return pointers need not
become parameters in this source-to-source IR. If an unusual ABI requires
a wrapper, preserve that requirement explicitly for emission rather than
assuming `extern "C"` covers every signature.

### Attributes by meaning and attachment

**Decided:** attribute storage is extensible. Preserve arbitrary GNU/vendor
attribute names and arguments, even when no dedicated semantic handler
exists. A closed enum of known attributes must not limit what reaches IR.
Known attributes additionally produce typed effects for Rust lowering.

Proposed occurrence shape, shared by functions, variables, parameters,
results, types, fields, and statements:

```text
AttributeOccurrence {
    id: AttributeId,
    namespace: Option<String>,
    name: String,
    arguments: Option<TokenTree>,
    interpreted_arguments: Option<Vec<AttributeArgument>>,
    resolution: Uninterpreted | Resolved | IgnoredByCompiler,
    metadata: { syntax: ..., spelling: ..., placement: ..., origin: ... },
}

AttributeArgument =
    Constant(Constant)
  | String(StringContents)
  | Type(Type)
  | Symbol(SymbolId)
  | Parameter(ParamIndex)
  | Identifier(String)
  | List(Vec<AttributeArgument>)
```

`TokenTree` retains tokens, nested delimiters, and argument separators after
preprocessing. It is the fallback for arbitrary argument grammars, not a
comma-split list of strings. `None` distinguishes no argument clause from
an empty parenthesized clause. Source spelling remains available separately.
Known handlers interpret arguments according to that attribute's grammar;
an identifier is not automatically an ordinary C variable reference.

Keep occurrences in order, including repeated names and differing arguments.
Do not store them in a map that overwrites duplicates. Their containing
node identifies the semantic attachment; placement metadata retains the
original attribute syntax and declaration position. Compatible GNU and
namespaced spellings may normalize to the same recognized name while
retaining their original spelling. Unknown names are preserved verbatim.

The parser need only understand the attribute envelope and balanced argument
tokens to retain an unfamiliar attribute. Adding semantic support for a
new attribute should require a handler, not changing the generic attribute
container or losing the attribute on every other node type.

For functions, keep both the open occurrence list and the resolved groups:

```text
FunctionAttributes {
    occurrences: Vec<AttributeOccurrence>,
    control: {
        normal_return: MayReturn | NoNormalReturn,
        returns_twice: bool,
    },
    contracts: Vec<FunctionContract>,
    execution: Vec<ExecutionRequirement>,
    hints: Vec<FunctionHint>,
}
```

Parameter, result, and type metadata expose the same occurrence list beside
their resolved contracts. Effects that move to a signature, symbol, or other
owner retain their source `AttributeId` references, including when several
declarations contribute to one effective contract. `Resolved` means the
semantic handler consumed the attribute; it does not mean Rust emission
has implemented that effect.

For example, an unfamiliar `__attribute__((vendor_hint("mode", (x, y))))`
retains its name, string spelling, and nested argument tree with
`resolution: Uninterpreted`. A recognized `alloc_size(1, 2)` retains its
occurrence and additionally exposes a resolved relationship such as
`Product(Argument(0), Argument(1))`. Rust-specific handlers can inspect
preserved extension attributes without requiring C source re-parsing.

Unknown behavior is conservative: an ordinary declaration may return and
may have side effects. A no-return contract is not a claim that the function
cannot unwind or otherwise transfer control nonlocally. `returns_twice`
must remain visible to control-flow handling. Detailed nonlocal-exit and
unwinding support is a separate design choice, not a default `nothrow` flag.

| Attribute information                                           | Resolved owner                                    | Intended use                                                                 |
| --------------------------------------------------------------- | ------------------------------------------------- | ---------------------------------------------------------------------------- |
| Calling convention                                              | `FunctionType.abi`                                | Correct direct and indirect calls                                            |
| Symbol name, visibility, weak/import/export, section            | `FunctionSymbol`                                  | Correct symbol/linker behavior                                               |
| Alias or resolver                                               | `FunctionImplementation`                          | Preserve symbol indirection                                                  |
| No-return, returns-twice                                        | `FunctionAttributes.control`                      | Correct control-flow treatment                                               |
| Constructor/destructor registration and priority                | Execution requirements                            | Preserve startup/shutdown execution                                          |
| Target features, interrupt, naked                               | Execution requirements                            | Preserve target-specific execution requirements                              |
| Nonnull/access/alignment requirements                           | Parameter/result contracts                        | Preserve declared contracts and guide analysis                               |
| Allocation size, alignment, allocator/deallocator relationships | Result/function contracts                         | Guide allocation and ownership reasoning                                     |
| Pure/const effect contracts                                     | Function contracts                                | Describe declared effects without claiming Rust purity                       |
| Inline preference, hot/cold, optimization preferences           | Hints                                             | Optional output/code-generation guidance after semantic effects are resolved |
| Deprecated, nodiscard, format, annotations                      | Hints/origin context at the applicable attachment | Diagnostics and contextual rewriting                                         |

Function, parameter, result, type, and call-site attachment are distinct.
An attribute constraining a pointee stays on that type use or access
contract; it is not a property of every numeric type with the same width.
Parameter-number operands resolve to `ParamIndex`. Relationships such as
an allocation size equal to the product of two arguments remain structured,
for example `Product(Argument(0), Argument(1))`; related functions use IDs.

Contracts supplied by declarations are kept distinct from facts proved by
analysis. Nonnull alone does not establish reference validity or lifetime;
allocation information alone does not establish a unique Rust owner.
Body-derived effect summaries belong to the analysis side table. Attributes
on function-pointer types must remain available at indirect calls even
when no particular function definition is known.

Preserving an attribute and implementing its semantics are separate
capabilities. Uninterpreted occurrences remain available through IR and do
not by themselves prevent parsing or IR retention. `IgnoredByCompiler` is
used only when established for the configured compiler/target; lack of a
handler does not establish that an attribute is ignorable.

For a known behavior-affecting attribute, IR sema resolves the effect or
reports the unsupported semantic feature. Rust emission can use an
extension handler or report an unhandled requirement; it must not silently
drop an ABI or execution effect. Hints can remain contextual metadata.
Malformed attribute syntax is diagnosed separately from an unfamiliar
attribute name. This permits broad attribute coverage while making the
supported semantic subset explicit.

### Calls and attribute visibility

```text
Call {
    callee: Direct(FunctionId) | Indirect(Value),
    signature: FunctionType,
    arguments: Vec<Value>,
    contracts: EffectiveCallContracts,
    requirements: Vec<CallRequirement>,
}
```

The enclosing expression supplies the call's result type and node identity.
Arguments contain explicit conversions; variadic and unprototyped calls
retain the actual promoted argument types in those values. The call's
signature and effective contracts reflect what sema resolved at that call
site, including applicable function-pointer attributes. Later redeclarations
must not retroactively change already-resolved argument evaluation or typing.
Repeated signature/contract data may be interned behind references.

Calls evaluate their callee and arguments according to the resolved
sequencing, exactly once. Requirements such as a supported must-tail call
belong on the call, not on every invocation of its target. Builtins whose
semantics require dedicated IR operations should lower to those operations;
a familiar function name by itself is not sufficient builtin identity.

### Concrete example

For `int add(int a, int b) { return a + b; }`, abbreviated:

```text
Function {
    id: add_id,
    name: "add",
    ty: {
        parameters: Prototype { fixed: [I32, I32], variadic: false },
        result: I32,
        abi: C_for_target,
    },
    parameter_metadata: [{ name: "a", ... }, { name: "b", ... }],
    implementation: Definition {
        parameters: [
            { parameter: 0, variable: a_id, name: "a", ty: I32 },
            { parameter: 1, variable: b_id, name: "b", ty: I32 },
        ],
        entry: [],
        body: { Return(Add(Read(a_id), Read(b_id), overflow=ub)) },
    },
    symbol: { link_name: "add", linkage: External, ... },
    attributes: { control: { normal_return: MayReturn, returns_twice: false }, ... },
    ...,
}
```

`I32` abbreviates its contextual `Type`, including `c_type=int` and storage
metadata. The signature provides callable types, while definition bindings
provide names and IDs for local uses. Extern declarations omit the definition;
function pointers refer to the callable type without copying this entity.

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

For functions, `src/ops/globals.rs::Func`, `src/ops/calls.rs::Call`, and
`src/types.rs::Func` informed the separation of symbols, callable types,
bodies, and parameter/result attributes. This proposal retains concrete
source-level signatures and typed attribute groups for Rust translation.
GCC documents [mode-dependent inline semantics](https://gcc.gnu.org/onlinedocs/gcc/Inline.html)
and [attribute meanings](https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html).

For source semantics, the [C11 draft, sections 6.8.4–6.8.6](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
describes selection, loops, and jumps. GNU extensions include
[inclusive case ranges](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html)
and [labels as values](https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html).
These references are background; the shapes above are our design proposals.

## Questions for this discussion

1. Does the proposed variable/value split capture the intended distinction
   between declaration storage classes and type storage metadata?
2. Does the function-type / named-function / definition-binding split expose
   the right information without making ordinary function lowering cumbersome?
3. Which contracts and execution requirements need concrete variants first?
4. Old-style definition entry, missing non-void results, GNU nested-function
   captures/static chains, and nonlocal exits need dedicated follow-up shapes.
