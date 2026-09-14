# AST Spec

_created 2026-09-13 — evergreen: update in the same change as any `src/ast.rs` or parser change_

What the parser produces and what it means. The AST is the **syntactic**
representation of one preprocessed translation unit. It follows the C
grammar closely (declaration specifiers + declarator lists, labeled
statements, type names), in the same spirit as clang's parser output before
Sema.

- For where each enum is matched exhaustively, see [[ast-enum-touchpoints]].
- For what the AST lowers into, see [[ir-spec]].
- The **Target design** sections are the spec. **Migration** at the end lists
  where `src/ast.rs` does not match it yet; each row is a bead under the AST
  redesign epic.

## Pipeline and responsibilities

```
pp ──▶ parser ──▶ AST ──▶ sema.rs (structural checks) ──▶ src/ir/sema (resolution + semantic checks) ──▶ IR
```

| Stage         | Owns                                                                                   | Does not                                                     |
| ------------- | -------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| Parser        | syntax, source form, spans, provenance, typedef-name tracking needed to parse          | evaluate, resolve names, compute types                       |
| `sema.rs`     | structural checks, diagnostics, removal of structurally invalid items                  | type resolution, conversions, guarantee of semantic validity |
| `src/ir/sema` | name resolution, types, conversions, constant evaluation, layout, semantic diagnostics | re-validate syntax                                           |

Early checks require no resolved names or types. Validation that depends on
resolution belongs to `src/ir/sema`; surviving the early pass does not prove
that a program is semantically valid.

## Invariants

- **Single configuration.** Preprocessing ran for one set of `-D` defines
  and target predefines. No conditional nodes. See
  [[architecture_single_configuration]].
- **Source form is preserved.** The parser never evaluates, folds, hoists
  or rewrites. `2[a]` stays `2[a]`. `A = 1 << 3` keeps the expression.
  `if (x) y;` and `if (x) { y; }` are distinguishable.
- **Nothing is resolved.** Identifiers, typedef names and tag references are
  names. The only name knowledge in the parser is the typedef-name set, which
  C's grammar requires to parse `T * x;`.
- **Nothing is dropped silently.** Every declarator, initializer, specifier
  and attribute written in the source appears in the AST, or the parser
  reports an error.
- **Source order.** Items appear in source order after preprocessing.
- **No analysis nodes.** Reachability is not recorded in the AST.
  `__builtin_unreachable()` is an ordinary call expression.

## Locations and provenance

- `Loc { file, offset, length }`: byte range.
- `Span<T> { value, spelling, expansion }`: `spelling` is where the tokens are
  written (possibly inside a macro definition). `expansion` is where they
  appear in the including file. **Every** declaration, declarator, statement
  and expression node is spanned.
- `Provenance { file, kind: System | User, line, header }`: `header` is the
  outermost header the main file directly included (`None` for the main
  file). `line` is 0-based. Carried by every declaration-level node
  (`Declaration`, `InitDeclarator`, `FunctionDefinition`, `TagDefinition`,
  `FieldDeclaration`, `Enumerator`, `ParameterDeclaration`, `CommentGroup`).
- Macro-expansion identity (which macro produced a token) is not in `Span`
  yet; tracked by `slate-parser-lh7.1.9`.

## Translation unit

```
TranslationUnit {
    items: Vec<ExternalItem>,
    tags: Vec<TagDefinition>,      // indexed by TagId, every tag definition in the TU
    flavor: CompilerFlavor,        // Gcc | Clang | Msvc personality
}

ExternalItem =
    | FunctionDefinition
    | Declaration
    | StaticAssert
    | Asm(GnuAsm)                   // file-scope asm("...")
    | CommentGroup
```

There is no `Typedef`, `Record` or `Enum` item. `typedef` is a storage
class, and tag definitions live in the specifiers that wrote them (see
[Tags](#tags)).

## Declarations

### `Declaration`

One C declaration: shared specifiers, then a list of declarators.

```
Declaration {
    specifiers: DeclarationSpecifiers,
    declarators: Vec<InitDeclarator>,   // may be empty: `struct S { int x; };`
    provenance,
}

InitDeclarator {
    declarator: Declarator,
    asm_label: Option<AsmLabel>,        // `asm("sym")`, register variables
    attributes: Vec<Attribute>,         // attributes after the declarator
    initializer: Option<Initializer>,
    provenance,
}
```

| Source                                  | AST                                                                                  |
| --------------------------------------- | ------------------------------------------------------------------------------------ |
| `int a, *b = &a;`                       | 1 `Declaration`, 2 `InitDeclarator`s                                                 |
| `typedef struct { int a; } T, *PT;`     | storage `Typedef`, tag definition in the type specifier, declarators `T`, `*PT`      |
| `static struct S { int x; } s = { 1 };` | storage `Static`, tag definition, declarator `s` with initializer                    |
| `struct S;` / `struct S { int x; };`    | no declarators                                                                       |
| `int x, f(void);`                       | declarators `x` and `f(void)`; that one is a function is visible from the declarator |

The same `Declaration` node is used at file scope, in blocks, in `for`
initializers, and (with `FieldDeclaration`, below) in records.

### `DeclarationSpecifiers`

```
DeclarationSpecifiers {
    storage: Option<StorageClass>,      // Typedef | Extern | Static | Auto | Register
    thread_local: bool,
    function: FunctionSpecifiers,       // inline, noreturn
    constexpr: bool,
    qualifiers: Qualifiers,             // const, volatile, restrict, _Atomic (qualifier form)
    ty: TypeSpecifier,
    attributes: Vec<Attribute>,
}
```

Multiple storage classes, conflicting specifiers etc. are **syntax the
parser accepts** where unambiguous and `sema.rs` rejects.

### `TypeSpecifier`

The base type named by the specifiers. It never contains pointers, arrays
or functions; those come only from declarators.

| Variant                                                            | Source                                                                 |
| ------------------------------------------------------------------ | ---------------------------------------------------------------------- |
| `Void`, `Bool`                                                     | `void`, `_Bool`/`bool`                                                 |
| `Char { signed: Option<bool> }`                                    | `char` / `signed char` / `unsigned char`                               |
| `Int { rank: Short \| Int \| Long \| LongLong \| Int128, signed }` | including `__int128_t`/`__uint128_t`                                   |
| `BitInt { width: Expr, signed }`                                   | `_BitInt(N)`, width unevaluated                                        |
| `Float(FloatKind)`                                                 | `float`, `double`, `long double`, `_Float16`, `__fp16`, `_Float128`, … |
| `Complex(FloatKind)`, `Imaginary(FloatKind)`                       |                                                                        |
| `FixedPoint { kind, rank, saturated }`                             | `_Fract`/`_Accum`                                                      |
| `Atomic(TypeName)`                                                 | `_Atomic(T)` specifier form                                            |
| `TypeOf { unqual: bool, operand: TypeOfOperand }`                  | `typeof(expr)` / `typeof(type-name)`                                   |
| `TypedefName(String)`                                              | an identifier the parser knows is a typedef name                       |
| `Tag(TagSpecifier)`                                                | `struct`/`union`/`enum`                                                |
| `TargetBuiltin(String)`                                            | `__builtin_va_list` etc.                                               |
| `Vector { element, size }`                                         | GNU vector types                                                       |

### `Declarator`

Read inside-out from the name. Each layer derives a type from the one
outside it.

```
Declarator =
    | Name(String)
    | Abstract                                    // no name: type names, unnamed parameters
    | Grouped(Box<Declarator>)                    // parentheses, kept for source form
    | Pointer { qualifiers, attributes, inner }
    | Array { inner, size: ArraySize, qualifiers, is_static }   // `int a[static const 3]` in params
    | Function { inner, parameters: ParameterList }
    | Attributed { inner, attributes }
```

`int (*fp)(int)` is
`Function { inner: Grouped(Pointer { inner: Name("fp") }), parameters: [int] }`:
`fp` is a pointer to a function taking `int`, returning the specifier type.

```
ArraySize = Unspecified | Expr(Expr) | Star      // [], [n], [*]

ParameterList =
    | Prototype { parameters: Vec<ParameterDeclaration>, variadic: bool }
    | Void                                        // (void)
    | Empty                                       // () — meaning depends on standard, decided by sema

ParameterDeclaration {
    specifiers,
    declarator: Declarator,
    declared_specifiers,   // Some(written specifiers) when a K&R rewrite promoted
                            // `specifiers` (default argument promotion); None otherwise
    attributes,
    provenance,
}
```

K&R definitions (`f(a, b) int a; char b; { ... }`) are rewritten into
`ParameterList::Prototype` at parse time; there is no separate K&R
representation. Callers of an unprototyped function apply default argument
promotions (small integers to `int`, `float` to `double`), so a rewritten
parameter's `specifiers` is the promoted, ABI-visible type; `declared_specifiers`
recovers the type as written, for the function-body local. An
implicit-int parameter (no declaration at all) is already `int`, so
`declared_specifiers` stays `None`.

### `TypeName`

A type written without declaring anything: casts, `sizeof(T)`, `_Alignof`,
compound literals, `va_arg`, `offsetof`, `_Generic` associations, `typeof`.

```
TypeName { specifiers: DeclarationSpecifiers, declarator: Declarator }   // declarator is abstract
```

### `FunctionDefinition`

```
FunctionDefinition {
    specifiers: DeclarationSpecifiers,
    declarator: Declarator,                 // outermost derived layer is Function
    attributes: Vec<Attribute>,
    body: CompoundStatement,
    provenance,
}
```

`int *f(void) { ... }` is specifiers `int`, declarator
`Function { inner: Pointer { inner: Name("f") }, parameters: Void }`.
The return type is not precomputed; `src/ir/sema` derives it the same way it
derives every other declared type.

GNU nested functions are `FunctionDefinition`s in block item position.

## Tags

```
TagSpecifier =
    | Reference { kind: Struct | Union | Enum, name: String, attributes }
    | Definition(TagId)

TagDefinition {
    id: TagId,
    kind: TagKind,
    name: Option<String>,
    attributes: Vec<Attribute>,
    body: TagBody,
    provenance,
}

TagBody =
    | Record(Vec<MemberItem>)
    | Enum { fixed_type: Option<TypeName>, enumerators: Vec<EnumItem> }   // C23 `enum E : int`

MemberItem = Field(FieldDeclaration) | StaticAssert | CommentGroup

FieldDeclaration {
    specifiers: DeclarationSpecifiers,
    declarators: Vec<FieldDeclarator>,      // empty for an anonymous struct/union member
    provenance,
}

FieldDeclarator { declarator: Declarator, bit_width: Option<Expr>, attributes, provenance }

EnumItem = Enumerator { name, value: Option<Expr>, attributes, provenance } | CommentGroup
```

- A tag definition is stored once in `TranslationUnit.tags` and referenced by
  `TagId` from the specifier that wrote it. Anonymous tags therefore have
  identity: in `struct { int y; } g1, g2;` both declarators share one
  specifier holding `Definition(TagId(n))`.
- A tag definition written inside another (`struct A { struct B { int x; } b; }`),
  in a block, or in a parameter list gets its own `TagId` the same way. The
  parser does not decide scope; `src/ir/sema` does.
- `Reference` is by name only. Resolving `struct S` to a definition (or to a
  forward declaration) is name resolution, done in `src/ir/sema`.
- Enumerator values are unevaluated expressions. An omitted value is `None`;
  "previous + 1" is sema's rule, not the parser's.

## Statements

`Stmt = Span<StmtKind>`. This section is the target shape; see the
Migration table for where `src/ast.rs` still differs (notably: `if`/loop
bodies are `Vec<Stmt>`, not `Box<Stmt>`, so brace presence is not yet
preserved).

```
CompoundStatement { items: Vec<BlockItem> }

BlockItem =
    | Declaration(Declaration)
    | StaticAssert
    | NestedFunction(FunctionDefinition)
    | LocalLabelDeclaration(Vec<String>)        // GNU __label__
    | CommentGroup
    | Stmt(Stmt)

StmtKind =
    | Compound(CompoundStatement)
    | Expr(Expr)
    | Null                                      // `;`
    | If { condition, then_branch: Box<Stmt>, else_branch: Option<Box<Stmt>> }
    | Switch { discriminant, body: Box<Stmt> }
    | While { condition, body: Box<Stmt> }
    | DoWhile { body: Box<Stmt>, condition }
    | For { init: ForInit, condition: Option<Expr>, step: Option<Expr>, body: Box<Stmt> }
    | Labeled { label: String, body: Box<Stmt> }              // goto target
    | SwitchLabel { label: SwitchLabel, body: Box<Stmt> }     // case/default
    | Goto(String)
    | IndirectGoto(Expr)                        // GNU goto *p
    | Continue
    | Break
    | Return(Option<Expr>)
    | Attributed { attributes, body: Box<Stmt> }   // [[fallthrough]]; → body is Null
    | Asm(GnuAsm)

ForInit = Declaration(Declaration) | Expr(Option<Expr>)

SwitchLabel = Case(Expr) | CaseRange { start: Expr, end: Expr } | Default
```

- `Labeled` and `SwitchLabel` are separate variants (not one `Label` sum
  type) because a `goto` label and a `switch` case/default are different
  things spelled with the same `name:` syntax; keeping them apart avoids a
  `Label::Named` arm that every case/default match has to rule out.
- Each nests its target statement as `body` rather than appearing as a
  flat list item, so `case 1: case 2: x;` is
  `SwitchLabel(Case 1, body: SwitchLabel(Case 2, body: Expr x))`, and a
  label at the end of a block or before nothing parseable gets an empty
  `Block([])` as `body` (implemented this way already; not yet `Null`
  since `Stmt::Null` doesn't exist — see Migration).
- `switch` cases are found by walking the body. Cases may be nested inside
  other statements (Duff's device), so they are not collected by the parser.

## Comments

```
CommentGroup { comment: Comment, provenance }
Comment { text: Vec<String>, loc: Loc }
```

- Consecutive comments with no code between them form **one** group, including
  across blank lines, coalesced into a single `Comment`. `text` holds the raw
  text of each original comment in order (so line/block style and per-comment
  boundaries are still visible); `loc` spans from the start of the first
  comment to the end of the last, covering any blank lines between them. A
  group never spans files.
- Groups appear where items can: `ExternalItem`, `BlockItem`, `MemberItem`,
  `EnumItem`. Comments inside expressions or declarators are not preserved.

## Expressions

`Expr = Box<Span<ExprKind>>`: one expression type, spanned at every node.

| Variant                                                                                        | Source                                                 |
| ---------------------------------------------------------------------------------------------- | ------------------------------------------------------ |
| `Identifier(String)`                                                                           | unresolved name                                        |
| `IntegerLiteral(IntegerLiteral)`                                                               | see [Literals](#literals)                              |
| `FloatLiteral(FloatLiteral)`                                                                   |                                                        |
| `CharLiteral(CharLiteral)`                                                                     |                                                        |
| `StringLiteral(StringLiteral)`                                                                 | adjacent literals concatenated                         |
| `Paren(Expr)`                                                                                  | parentheses, kept for source form                      |
| `Unary { op: Plus \| Minus \| BitNot \| Not \| AddrOf \| Deref \| PreInc \| PreDec, operand }` |                                                        |
| `Postfix { op: PostInc \| PostDec, operand }`                                                  |                                                        |
| `Binary { op, left, right }`                                                                   | arithmetic, shifts, comparisons, bitwise, `&&`, `\|\|` |
| `Assign { op, target, value }`                                                                 | `=` and compound assignments                           |
| `Conditional { condition, then_value: Option<Expr>, else_value }`                              | `?:`; `then_value: None` is GNU `a ?: b`               |
| `Comma { left, right }`                                                                        |                                                        |
| `Call { callee, arguments }`                                                                   |                                                        |
| `Member { base, field, arrow: bool }`                                                          | `.` / `->`                                             |
| `Index { base, index }`                                                                        | as written                                             |
| `Cast { ty: TypeName, value }`                                                                 |                                                        |
| `CompoundLiteral { ty: TypeName, storage, initializer: InitializerList }`                      | C23 storage in compound literals                       |
| `SizeOfExpr(Expr)`, `SizeOfType(TypeName)`, `AlignOf(TypeName)`, `AlignOfExpr(Expr)`           |                                                        |
| `OffsetOf { ty: TypeName, member: MemberDesignator }`                                          | `offsetof`/`__builtin_offsetof`                        |
| `Generic { controlling: GenericControl, associations: Vec<GenericAssociation> }`               |                                                        |
| `VaArg { list, ty: TypeName }`                                                                 |                                                        |
| `TypesCompatible(TypeName, TypeName)`                                                          | GNU                                                    |
| `BitCast { ty: TypeName, value }`                                                              |                                                        |
| `LabelAddress(String)`                                                                         | GNU `&&label`                                          |
| `StatementExpression(CompoundStatement)`                                                       | GNU `({ ... })`, parsed                                |

```
GenericControl = Expr(Expr) | Type(TypeName)
GenericAssociation = Type { ty: TypeName, value: Expr } | Default(Expr)
MemberDesignator = Vec<Field(String) | Index(Expr)>
```

Whether an identifier in `_Generic`, `sizeof(x)` or `(x)(y)` is a type is
decided by the typedef-name set, the same as everywhere else in C parsing.

## Literals

The parser decodes lexical content (digits, escapes) but does not assign C
types. Suffixes and prefixes are kept so `src/ir/sema` can.

```
IntegerLiteral {
    value: BigUint,                          // magnitude; never overflows
    radix: Decimal | Hex | Octal | Binary,
    suffix: IntegerSuffix { unsigned: bool, size: None | Long | LongLong | BitInt },
    spelling: String,
}

FloatLiteral {
    spelling: String,
    radix: Decimal | Hex,
    suffix: None | F | L | F16 | F32 | F64 | F128 | F32x | F64x | Q | DecimalF32 | DecimalF64 | DecimalF128,
    imaginary: bool,
}

CharLiteral {
    encoding: Plain | Utf8 | Utf16 | Utf32 | Wide,
    code_units: Vec<u32>,                    // multichar literals keep every unit
    spelling: String,
}

StringLiteral {
    encoding: Plain | Utf8 | Utf16 | Utf32 | Wide,   // after concatenation rules
    code_units: Vec<u32>,                    // decoded, without the terminating NUL
    pieces: Vec<Span<String>>,               // each source literal's raw spelling
}
```

## Initializers

```
Initializer = Expr(Expr) | List(InitializerList)
InitializerList { items: Vec<InitializerItem>, trailing_comma: bool }
InitializerItem { designators: Vec<Designator>, value: Initializer }
Designator = Field(String) | Index(Expr) | IndexRange { start: Expr, end: Expr }   // GNU range
```

Designator indices are expressions, not evaluated integers.

## Attributes and asm

`Attribute` is a closed set of known GNU/C23 attributes with parsed
arguments. Unknown attributes are `Unknown { name, arguments }`, malformed
ones `Invalid`. Attribute _placement_ is preserved: specifiers, declarators,
init-declarators, tag definitions, statements. `GnuAsm` holds the parsed
template, operands with constraints, clobbers and labels.

## Validation (`sema.rs`)

`sema.rs` runs on the AST and returns the AST with invalid items removed,
plus diagnostics for structural errors it can establish without name or
type resolution. It does not annotate types or guarantee that surviving
items are semantically valid. `src/ir/sema` checks constraints that require
resolved scopes, types, conversions, or layout for the configured flavor
and standard, and reports failures before those items can lower to IR.

For the IR pipeline, declaration pruning happens after name resolution,
using resolved dependencies and explicit translation/linkage/attribute
roots. The parser preserves declarations for that resolution. See
[IR validation and pruning](ir-spec.md#validation-and-declaration-pruning).

## Migration

Where `src/ast.rs` does not match this spec yet. Each row is tracked under
the AST redesign epic.

| Current                                                                                                                                                                                       | Target                                                                                        | Also fixes                                                             |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| `InitDeclarator` and `FieldDeclarator` carry no provenance; `Stmt::Decl` has none at all                                                                                                      | provenance on every declarator                                                                |                                                                        |
| `CType::Tag(TagSpecifier)` inside `CType`; `Reference` has no attributes; `TagBody::Record` holds `FieldItem`; `TagBody::Enum` has no `fixed_type`                                            | `TypeSpecifier` (`lh7.3.5`), `MemberItem`, `EnumItem`                                         |                                                                        |
| `TranslationUnit.tags` is `Vec<Span<TagDefinition>>` ordered by id; reachability pruning leaves gaps, so look tags up with `TranslationUnit::tag`                                             | indexed by `TagId` once pruning moves to the IR pipeline                                      |                                                                        |
| `CType` mixes specifiers with derived types; `FunctionDecl.ret_type` pre-applied                                                                                                              | `TypeSpecifier` + `Declarator` everywhere; `FunctionDefinition` with a declarator             | two type encodings                                                     |
| `FunctionDecl.parameters: []` for both `(void)` and `()`                                                                                                                                      | `ParameterList::{Void, Empty}`                                                                |                                                                        |
| `ExprKind` casts, `sizeof`, `_Alignof`, `offsetof`, `va_arg`, compound literals, `GenericControl::Type` and `GenericAssociation::Type` hold `ty: Box<CType>` + `declarator`; no `AlignOfExpr` | `TypeName`; `AlignOfExpr` (`lh7.3.5`)                                                         |                                                                        |
| `Designator::Array`/`ArrayRange`                                                                                                                                                              | `Index`/`IndexRange`                                                                          |                                                                        |
| bare `aligned` attribute reads `__BIGGEST_ALIGNMENT__` from the target macros in the parser                                                                                                   | argument-less `Aligned`, value chosen in `src/ir/sema`                                        |                                                                        |
| `if`/loop bodies are `Vec<Stmt>`; a braced `{ ... }` body is flattened into that `Vec` the same as a brace-less single statement                                                              | `Box<Stmt>` bodies, so a `Block` body is visible in the AST                                   | braces not preserved                                                   |
| `sema.rs` returns errors only; rejects tag definitions in parameter lists                                                                                                                     | returns structurally checked AST plus diagnostics; semantic validity checked by `src/ir/sema` |                                                                        |
| parser calls name-based `filter_translation_unit` before resolution                                                                                                                           | IR pipeline prunes resolved symbol dependencies from explicit roots                           |                                                                        |
