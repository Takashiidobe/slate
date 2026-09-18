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
pp ──▶ parser ──▶ AST ──▶ src/sema/ (validation + resolution + lowering) ──▶ typed IR
```

| Stage                  | Owns                                                                                                                 | Does not                               |
| ---------------------- | -------------------------------------------------------------------------------------------------------------------- | -------------------------------------- |
| Parser                 | syntax, source form, spans, provenance, typedef-name tracking needed to parse                                        | evaluate, resolve names, compute types |
| `src/sema/validate.rs` | existing early checks and diagnostics                                                                                | guarantee of full semantic validity    |
| `src/sema/`            | name resolution, types, conversions, constant evaluation, layout, semantic diagnostics, direct typed IR construction | produce an intermediate semantic AST   |

Early checks require no resolved names or types. Validation that depends on
resolution belongs to `src/sema/`; surviving the early pass does not prove
that a program is semantically valid.

The parser boundary is `ParserInput`: one expanded token buffer with comments
and pragmas anchored between tokens. `PPNode` code chunks are flattened once;
their text and physical-line boundaries do not participate in the grammar.
Declaration, function, record and statement parsing consume token positions
and delimiters. Expression parsing borrows the same tokens, including inside
GNU statement expressions. Original token spans and macro/header provenance
survive this boundary without re-lexing or synthesized declaration tokens.

Nested item parsers claim annotations within their token ranges. Remaining
pragmas inside an item precede that containing item; interleaved comments follow
it. Comments at item boundaries retain their position, including empty bodies.

All grammar entry points share one ordinary-name environment. Lookup walks the
scope stack from inside out and stops at the nearest typedef or ordinary binding;
tags and members do not enter this namespace. Scope guards restore enclosing
bindings on both success and errors for blocks, loops, function bodies and
prototype parameter lists. Grammar parsers share the environment rather than
copying typedef sets or cloning the whole parser. Object and typedef names bind
immediately after their complete declarator, before attributes, initializers and
later declarators; their own array bounds still see the enclosing binding.
Parameters bind after their complete declarators, and enumerators bind after
their defining enumerator, leaving initializer expressions unevaluated.
Each parameter list has its own scope, including nested function-pointer lists.
The defining function's parameter-scope bindings (including enumerators) are
retained for its body; prototype and nested parameter bindings do not leak.
In C99+, a for-initializer declaration stays visible through the condition,
increment and body, then the enclosing bindings are restored.

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
- `Span<T> { id, value, spelling, expansion, provenance, macro_origin }`: `spelling` is
  where the tokens are written (possibly inside a macro definition).
  `expansion` is where they appear in the including file. **Every**
  declaration, declarator, statement and expression node is spanned.
- `id: NodeId`, a globally unique id allocated when the `Span` is built
  (`Span::new`/`Span::cover`; `Span::with_value`/`Span::map` keep the
  original id since they relabel the same node). This is the node identity
  `Loc` can't provide: every token from one macro expansion shares an
  `expansion` `Loc`, but each gets a distinct `NodeId`, which is what lets
  `src/sema/` preserve identity rather than keying nodes by location (see
  [[ir-spec]]). `slate-parser parse --show-ids` prints it on every `Span`
  in the debug dump; `tests/fixtures/node_ids_macro_expansion.c` (enabled via
  `// SLATE-FILECHECK-SHOW-IDS <prefix>`) checks that nodes sharing an
  expansion `Loc` still get distinct ids.
- `Provenance { file, kind: System | User, line, system_header }`: stored on every
  `Span`, so source-grammar nodes inherit it automatically. `system_header` is
  the first system header entered from user code and is preserved through its
  transitive include subtree. It is `None` for main-file and user-header code.
  `line` is 0-based. Public repeated grammar nodes use the
  `Node = Span<NodeKind>` representation; payloads such as `FunctionDefinition`
  and `Declaration` inherit the enclosing `Decl` or `Stmt` span and carry no
  duplicate provenance. Debug output includes provenance only when
  `system_header` is `Some`.
- `macro_origin: Option<Rc<MacroOrigin>>` (`MacroOrigin { name, definition:
Provenance, parent: Option<Rc<MacroOrigin>> }`) identifies which macro
  produced a token, e.g. `int m = INT_MAX;` gives the folded literal's `Span`
  a `macro_origin` of `{ name: "INT_MAX", definition: <limits.h provenance> }`
  so Slate can pattern-match macro name + system header. `name`/`definition` are the
  outermost macro invoked at the use site; `parent` chains through whatever
  further macros its replacement expanded through (e.g. `INT_MAX` expanding
  through `__INT_MAX__`), innermost last. Set in `pp/expand.rs::expand_macros`
  on every replacement token, then flows into AST `Span`s for free because
  `Span::cover` propagates `macro_origin` when every covered token agrees on
  it (`None` on a node built from tokens with mixed origins). Not yet
  surfaced by any renderer/dump mode.

## Translation unit

```
TranslationUnit {
    items: Vec<ExternalItem>,
    tags: Vec<TagDefinition>,      // indexed by TagId, every tag definition in the TU
    flavor: CompilerFlavor,        // Gcc | Clang | Msvc personality
    target: TargetInfo,
    options: CompilerOptions,
}

ExternalItem =
    | FunctionDefinition
    | Declaration
    | StaticAssert
    | Asm(GnuAsm)                   // file-scope asm("...")
    | Pragma(Pragma)                // semantic preprocessor state change
    | CommentGroup
```

`options` retains grouped operation/layout settings and ordered compiler
arguments. `target` is the effective target after layout options, shared
with predefine generation. Semantic lowering resolves these inputs into
operation contracts and concrete numeric formats; later IR consumers do not
interpret the arguments. Long-double literal spelling remains unresolved
in the AST and is interpreted at the target precision by sema.

Semantic pragmas are preserved at their source position as `DeclKind::Pragma`
at file scope or `StmtKind::Pragma` inside a function. Pack, weak, visibility,
STDC floating-point, `float_control`, and `ms_struct` forms have typed payloads;
other pragma spellings use `PragmaKind::Opaque` so preprocessing information is
never discarded.

`_Pragma` operands are destringized after macro expansion, with spelling and
expansion locations retained. An operator inside a statement is emitted before
that containing statement; its span records the position within the expression.
Completed statements preceding the operator keep their order and typedef scope.
The preprocessor executes `push_macro` and `pop_macro` operators as well as
preserving their pragma nodes. Computed include operands are expanded before
header lookup; directly written header names are not macro-expanded.

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
| `Float(Decimal32 \| Decimal64 \| Decimal128)`                      | `_Decimal32`, `_Decimal64`, `_Decimal128`; literals `DF`/`DD`/`DL`     |
| `Complex(FloatKind)`, `Imaginary(FloatKind)`                       |                                                                        |
| `FixedPoint { kind, rank, saturated }`                             | `_Fract`/`_Accum`                                                      |
| `Atomic(TypeName)`                                                 | `_Atomic(T)` specifier form                                            |
| `TypeOf { unqual: bool, operand: TypeOfOperand }`                  | `typeof(expr)` / `typeof(type-name)`                                   |
| `TypedefName(String)`                                              | an identifier the parser knows is a typedef name                       |
| `Tag(TagSpecifier)`                                                | `struct`/`union`/`enum`                                                |
| `TargetBuiltin(String)`                                            | `__builtin_va_list` etc.                                               |
| `Vector { element, size }`                                         | GNU vector types                                                       |

Decimal floating types (C23 Annex H, and a GNU extension before C23) are
accepted in every standard mode and on every target:
`StandardFeatures::decimal_floating_point` is `Standard` from C23 and
`Extension` before it, never `Rejected`. Real compilers differ (clang
rejects them, gcc only enables them on some targets), but that's the
implementation's choice. The input is assumed to have compiled with the
real compiler, so whether it's usable is Slate's decision, not the
front end's. IR lowering does not handle them yet.

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
`declared_specifiers` stays `None`. C23 removed identifier lists, so
`StandardFeatures::identifier_list_definitions` is `Rejected` there and the
parser reports an error instead of rewriting; fixtures using K&R pin
`SLATE-FILECHECK-STD DEFAULT c17` because the default standard is C23.

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
    | Reference { kind: Struct | Union | Enum, name: String, fixed_type: Option<Box<TypeName>> }
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
- An enumerator's attributes are written between the name and the `=`, per C23
  6.7.2.2, in either the `[[...]]` or `__attribute__((...))` spelling. Neither
  clang nor gcc accepts them after the value, and neither do we. The IR drops
  attributes everywhere, so they stop at the AST.

## Statements

`Stmt = Span<StmtKind>`. Each control-flow body is a single `Box<Stmt>`.
Braced bodies retain an explicit `Block(Vec<Stmt>)` node spanning the braces;
unbraced bodies retain their statement node. `Null` represents `;`, separately
from an empty compound statement. Function bodies remain statement lists.

`TranslationUnit.standard` preserves the configured language standard for sema.
In C89/GNU89, only explicit compound statements introduce block scopes here;
selection/iteration statements and unbraced bodies add no implicit scopes.
In C99 and later (including GNU modes), each selection/iteration statement
has a scope, and each controlled body has a nested scope regardless of braces.
A braced body's `Block` supplies that body scope; do not add another implicit
scope around it. Condition and `for` clause declarations belong to the control
statement's scope, while body declarations remain within the body scope.
Parser typedef disambiguation and semantic name resolution follow these same
version-dependent rules. Enum/tag definitions in expressions make the
unbraced-body distinction observable even without declaration statements.

```
FunctionDefinition { body: Vec<Stmt>, ... }

StmtKind =
    | Decl(Declaration)
    | StaticAssert(StaticAssert)
    | NestedFunction(Box<FunctionDefinition>)
    | LocalLabelDecl(Vec<Span<String>>)  // GNU __label__
    | Comment(CommentGroup)
    | Pragma(Pragma)
    | Block(Vec<Stmt>)
    | Null
    | Expr(Expr)
    | If { condition, then_branch: Box<Stmt>, else_branch: Option<Box<Stmt>> }
    | Switch { discriminant, body: Box<Stmt> }
    | While { condition, body: Box<Stmt> }
    | DoWhile { body: Box<Stmt>, condition }
    | For { init: Option<Box<Stmt>>, condition: Option<Expr>, increment: Option<Expr>, body: Box<Stmt> }
    | Labeled { label: Span<String>, body: Box<Stmt> }        // goto target
    | SwitchLabel { label: SwitchLabel, body: Box<Stmt> }     // case/default
    | Goto(Span<String>)
    | ComputedGoto(Expr)                        // GNU goto *p
    | Continue
    | Break
    | Return(Expr)
    | ReturnVoid
    | Attribute(Vec<Attribute>)                // standalone [[fallthrough]];
    | Attributed { attributes, body: Box<Stmt> } // attributes on a non-null statement
    | Asm(GnuAsm)

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
  `Null` as `body`. A null statement `;` uses the same representation;
  an explicit empty compound statement remains `Block([])`.
- `switch` cases are found by walking the body. Cases may be nested inside
  other statements (Duff's device), so they are not collected by the parser.
  Case expressions and GNU range endpoints are parsed by the expression
  parser, not split at the first colon or ellipsis; nested ternaries and
  type expressions retain their structure.
- `For.init` is absent, a declaration statement, or an expression statement;
  the parser never puts another statement kind there.
- `Attributed` preserves attachment to its nested statement, including labels
  and control statements. It does not itself introduce a scope. Attributes
  before declarations remain on the declaration, and standalone attribute
  statements retain the existing `Attribute` form.

## Comments

```
CommentGroup { comment: Comment }
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
| `Member { base, field: Span<String>, arrow: bool }`                                            | `.` / `->`                                             |
| `Index { base, index }`                                                                        | as written                                             |
| `Cast { ty: TypeName, value }`                                                                 |                                                        |
| `CompoundLiteral { ty: TypeName, storage, initializer: InitializerList }`                      | C23 storage in compound literals                       |
| `SizeOfExpr(Expr)`, `SizeOfType(TypeName)`, `AlignOf(TypeName)`, `AlignOfExpr(Expr)`           |                                                        |
| `OffsetOf { ty: TypeName, member: MemberDesignator }`                                          | `offsetof`/`__builtin_offsetof`                        |
| `Generic { controlling: GenericControl, associations: Vec<GenericAssociation> }`               |                                                        |
| `VaArg { list, ty: TypeName }`                                                                 |                                                        |
| `TypesCompatible(TypeName, TypeName)`                                                          | GNU                                                    |
| `BitCast { ty: TypeName, value }`                                                              |                                                        |
| `LabelAddress(Span<String>)`                                                                   | GNU `&&label`                                          |
| `StatementExpression(CompoundStatement)`                                                       | GNU `({ ... })`, parsed                                |

```
GenericControl = Expr(Expr) | Type(TypeName)
GenericAssociation = Type { ty: TypeName, value: Expr } | Default(Expr)
MemberDesignator = Vec<Field(Span<String>) | Index(Expr)>
```

The AST keeps every `_Generic` association; sema picks one. The controlling
operand is lvalue-converted first (array and function types decay to pointers,
top-level qualifiers drop), so an association of array type can never be
selected, matching clang's `-Wunreachable-code-generic-assoc`.

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
    imaginary: bool,                         // GNU i/j suffix, anywhere among u/l
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
Designator = Field(Span<String>) | Index(Expr) | IndexRange { start: Expr, end: Expr }   // GNU range
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

## Post-C89 constructs in older standard modes

slate-parser accepts these in every standard mode. Checked with clang 22.1
and gcc 16.2 over `c89 gnu89 c99 c11 c17 gnu17 c23`, plain and
`-pedantic`. `ok` = silent, `warn` = only under `-pedantic`, `err` = hard
error. Where a compiler accepts a construct as an extension, slate-parser
records it as an extension (like `_BitInt`, see [ir-spec](ir-spec.md)) and
does not reject; input is assumed to have compiled with the real compiler.

| Construct                                                        | Introduced | clang before intro.         | gcc before intro.         | slate-parser                        |
| ---------------------------------------------------------------- | ---------- | --------------------------- | ------------------------- | ----------------------------------- |
| VLAs, `[*]` parameters                                           | C99        | warn                        | warn                      | extension                           |
| compound literals                                                | C99        | warn                        | warn                      | extension                           |
| designated initializers                                          | C99        | warn                        | warn                      | extension                           |
| declarations after statements                                    | C99        | warn                        | warn                      | extension                           |
| flexible array members                                           | C99        | warn                        | warn                      | extension                           |
| variadic macros                                                  | C99        | warn                        | warn                      | extension                           |
| `//` comments                                                    | C99        | `c89`: warn, `gnu89`: ok    | `c89`: err, `gnu89`: warn | extension                           |
| `for (int i…)` declaration                                       | C99        | warn                        | `c89`/`gnu89`: err        | gated by `control_statement_scopes` |
| `_Static_assert`, `_Generic`, `_Alignof`, `_Atomic`, `_Noreturn` | C11        | warn                        | warn                      | extension                           |
| `[[…]]` attributes                                               | C23        | warn (all modes before C23) | warn                      | extension                           |
| `0b` binary literals                                             | C23        | warn                        | warn                      | extension                           |
| digit separators (`1'000`)                                       | C23        | err                         | err                       | extension                           |

Digit separators are the one construct both compilers reject before C23
(both lex the `'` as the start of an unterminated character constant, so
there is no dedicated extension diagnostic). slate-parser still accepts
them in every mode: the input is assumed to have compiled, so rejecting
buys no fidelity. `0b` literals are likewise an extension.

## Migration

Where `src/ast.rs` does not match this spec yet. Each row is tracked under
the AST redesign epic.

| Current                                                                                                                                                                   | Target                                                                                        | Also fixes                                   |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | -------------------------------------------- |
| `TagSpecifier::Reference` stores an optional boxed fixed enum underlying type; `TagBody::Record` holds `FieldItem`                                                        | `MemberItem`, `EnumItem`                                                                      | opaque C23 enum declarations retain `: type` |
| `TypeSpecifier` variants use `Integer(IntegerType)`, `Floating(FloatingType)`, `Complex(Box<TypeSpecifier>)`, `Named`, `TypeOf`/`TypeOfUnqual` instead of the table above | variant names and shapes in the `TypeSpecifier` table                                         |                                              |
| `TranslationUnit.tags` is `Vec<Span<TagDefinition>>` ordered by id; reachability pruning leaves gaps, so look tags up with `TranslationUnit::tag`                         | indexed by `TagId` once pruning moves to the IR pipeline                                      |                                              |
| `Designator::Array`/`ArrayRange`                                                                                                                                          | `Index`/`IndexRange`                                                                          |                                              |
| bare `aligned` attribute reads `__BIGGEST_ALIGNMENT__` from the target macros in the parser                                                                               | argument-less `Aligned`, value chosen in `src/ir/sema`                                        |                                              |
| `sema.rs` returns errors only; rejects tag definitions in parameter lists                                                                                                 | returns structurally checked AST plus diagnostics; semantic validity checked by `src/ir/sema` |                                              |
| parser calls name-based `filter_translation_unit` before resolution                                                                                                       | IR pipeline prunes resolved symbol dependencies from explicit roots                           |                                              |

## Calling conventions and Microsoft declaration attributes

`Attribute::CallingConvention(CallingConvention)` preserves explicit `Cdecl`,
`Stdcall`, `Fastcall`, `Vectorcall`, `Thiscall`, `MsAbi`, `SysVAbi`,
`RegParm(Expr)`, and `Pcs(Aapcs | AapcsVfp)` requests. GNU attributes (including
wrapped names) and Microsoft calling-convention keywords share these nodes.
As with other attributes, declaration-specifier positions apply to the
whole declaration; nested declarator positions remain on `Attributed` or
`Pointer` nodes, and trailing positions remain in declarator attributes.
`regparm` keeps its expression unevaluated. Target support, conflicts, and
the effective ABI are sema responsibilities.

`__declspec(...)` accepts single-parenthesis attribute groups, including
space-separated entries. `dllimport` and `dllexport` become `DllImport` and
`DllExport`; `align(expr)` becomes `Aligned(Expr)`. Unrecognized entries
retain their name and argument tokens through `Unknown`.
