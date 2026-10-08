# AST Spec

<!-- toc -->
- [Pipeline and responsibilities](#pipeline-and-responsibilities)
- [Parser input and name
  environment](#parser-input-and-name-environment)
- [Invariants](#invariants)
- [Locations and provenance](#locations-and-provenance)
- [Translation unit](#translation-unit)
- [Pragmas](#pragmas)
- [Declarations](#declarations)
  - [`Declaration`](#declaration)
  - [`DeclarationSpecifiers`](#declarationspecifiers)
  - [`TypeSpecifier`](#typespecifier)
  - [MS and GNU keywords](#ms-and-gnu-keywords)
  - [`vector_size` and `mode`](#vector_size-and-mode)
  - [Floating keywords](#floating-keywords)
  - [`Declarator`](#declarator)
  - [`TypeName`](#typename)
  - [`FunctionDefinition`](#functiondefinition)
- [Tags](#tags)
- [Statements](#statements)
- [Comments](#comments)
- [Expressions](#expressions)
- [Literals](#literals)
- [Initializers](#initializers)
- [Attributes and asm](#attributes-and-asm)
- [Calling conventions and
  `__declspec`](#calling-conventions-and-__declspec)
- [Early validation](#early-validation)
- [Post-C89 constructs in older
  modes](#post-c89-constructs-in-older-modes)
- [Migration](#migration)
<!-- /toc -->

The AST is the syntactic form of one preprocessed translation unit, close to
the C grammar (specifiers + declarator lists), like clang's parser output
before Sema. Update this page with any `src/ast.rs` or parser change.

- Printed syntax: [ast-grammar](ast-grammar.md).
- Exhaustive match sites: [ast-enum-touchpoints](ast-enum-touchpoints.md).
- What it lowers into: [ir-spec](ir-spec.md).
- [Migration](#migration) lists where `src/ast.rs` differs from this spec.

## Pipeline and responsibilities

```
pp ──▶ parser ──▶ AST ──▶ src/sema/ (validation + resolution + lowering) ──▶ typed IR
```

| Stage | Owns | Does not |
| --- | --- | --- |
| Parser | syntax, source form, spans, provenance, typedef-name tracking | evaluate, resolve names, compute types |
| `src/sema/validate.rs` | early checks needing no names or types | guarantee semantic validity |
| `src/sema/` | names, types, conversions, constants, layout, semantic diagnostics, typed IR | produce a semantic AST |

## Parser input and name environment

- `ParserInput` is one expanded token buffer with comments and pragmas
  anchored between tokens. `PPNode` chunks are flattened once; their text
  and line boundaries don't affect the grammar. Spans and provenance
  survive without re-lexing.
- Nested item parsers claim annotations in their token range. Leftover
  pragmas inside an item precede it; interleaved comments follow it.
- One ordinary-name environment for all grammar entry points. Lookup stops
  at the nearest typedef or ordinary binding; tags and members are
  separate. Scope guards restore bindings on success and error (blocks,
  loops, bodies, prototype lists).
- Objects and typedefs bind after their complete declarator, before
  attributes, initializers, and later declarators; their own array bounds
  see the enclosing binding. Parameters bind after their declarators;
  enumerators after their definition.
- Each parameter list has its own scope. The defining function's parameter
  bindings (including enumerators) carry into its body; prototype and
  nested lists don't leak.
- C99+: a `for` declaration is visible through condition, increment, body.

## Invariants

- **Single configuration.** One `-D`/target set; no conditional nodes.
- **Source form preserved.** No evaluation, folding, or rewriting: `2[a]`
  stays, `A = 1 << 3` keeps the expression, `if (x) y;` ≠
  `if (x) { y; }`.
- **Nothing resolved.** Names stay names; the parser only tracks typedef
  names (needed for `T * x;`).
- **Nothing dropped.** Every declarator, initializer, specifier, and
  attribute appears in the AST, or the parser errors.
- **Source order** after preprocessing.
- **No analysis nodes.** No reachability; `__builtin_unreachable()` is a
  call.

## Locations and provenance

- `Loc { file, offset, length }`: byte range.
- `Span<T> { id, value, spelling, expansion, provenance, macro_origin }`.
  `spelling` is where tokens are written (possibly in a macro definition);
  `expansion` is where they appear in the including file. Every
  declaration, declarator, statement, expression, and attribute is spanned.
- `id: NodeId` is unique per node. `Span::new` / `Span::cover` allocate;
  `with_value` / `map` keep the id; `derive` copies the location with a
  fresh id. Tokens from one expansion share a `Loc` but get distinct ids
  ([node identity](ir/pipeline.md#node-identity-and-metadata)).
  `parse --show-ids` prints them; checked by
  `clang/linux/x86_64/node_ids_macro_expansion.c`
  (`// SLATE-FILECHECK-SHOW-IDS <prefix>`).
- `Provenance { file, kind: System | User, line, system_header }` on every
  `Span`. `system_header` is the first system header entered from user
  code, kept through its include subtree; `None` for main-file and user
  headers. `line` is 0-based. Payloads (`FunctionDefinition`,
  `Declaration`) inherit the enclosing `Decl`/`Stmt` span. Debug output
  shows provenance only when `system_header` is `Some`.
- `macro_origin: Option<Rc<MacroOrigin { name, definition, inner }>>`:
  `name`/`definition` are the outermost macro at the use site (`INT_MAX`);
  `inner` chains through its replacement (`__INT_MAX__`). Set in
  `pp/expand.rs::expand_macros`; `Span::cover` keeps it only when all
  covered tokens agree. Not printed by any dump. How both are computed:
  [preprocessor](preprocessor.md#provenance).

## Translation unit

```
TranslationUnit {
    items: Vec<ExternalItem>,
    tags: Vec<TagDefinition>,      // indexed by TagId
    dialect: Dialect,              // flavor, standard, features, effective target, options
}

ExternalItem =
    | FunctionDefinition
    | Declaration
    | StaticAssert
    | Asm(GnuAsm)                   // file-scope asm("...")
    | Pragma(Pragma)
    | Attribute(Vec<Span<Attribute>>)  // file-scope `[[attr]];`
    | CommentGroup
```

- `dialect.options` keeps grouped settings and ordered arguments; `target`
  is the effective target. Sema resolves them into operation contracts and
  formats; IR consumers never read arguments.
- Long-double literal spelling stays unresolved; sema interprets it at the
  target precision.
- No `Typedef`, `Record`, or `Enum` item: `typedef` is a storage class and
  tag definitions live in the specifier that wrote them.

## Pragmas

- Kept in place as `DeclKind::Pragma` / `StmtKind::Pragma`. Pack, weak,
  visibility, STDC floating-point, `float_control`, `ms_struct` are typed;
  others are `PragmaKind::Opaque`.
- STDC pragmas are typed only with uppercase `ON`/`OFF`/`DEFAULT`.
  `float_control` forms other than `({push|pop})` and
  `({precise|except}[, {on|off}][, push])` are `FloatControl::Malformed`
  for sema to reject per flavor.
- `_Pragma` operands are destringized after expansion, keeping locations.
  MS `__pragma(tokens)` (gated like MS keywords) uses its expanded operand
  (`__pragma(pack(push, _CRT_PACKING))`). An operator inside a statement
  is emitted before that statement; its span records the position.
- `push_macro` / `pop_macro` are executed and kept as nodes.
- Computed include operands are expanded; written header names are not.

## Declarations

### `Declaration`

```
Declaration {
    specifiers: DeclarationSpecifiers,
    declarators: Vec<InitDeclarator>,   // may be empty: `struct S { int x; };`
    provenance,
}

InitDeclarator {
    declarator: Declarator,
    asm_label: Option<AsmLabel>,        // `asm("sym")`, register variables
    attributes: Vec<Span<Attribute>>,   // after the declarator
    initializer: Option<Initializer>,
    provenance,
}
```

| Source | AST |
| --- | --- |
| `int a, *b = &a;` | 1 `Declaration`, 2 `InitDeclarator`s |
| `typedef struct { int a; } T, *PT;` | storage `Typedef`, tag definition in the specifier, declarators `T`, `*PT` |
| `static struct S { int x; } s = { 1 };` | storage `Static`, tag definition, `s` with initializer |
| `struct S;` / `struct S { int x; };` | no declarators |
| `int x, f(void);` | declarators `x`, `f(void)` |

Used at file scope, in blocks, in `for` initializers, and (as
`FieldDeclaration`) in records.

### `DeclarationSpecifiers`

```
DeclarationSpecifiers {
    storage: Option<StorageClass>,      // Typedef | Extern | Static | Auto | Register
    thread_local: bool,
    function: FunctionSpecifiers,       // inline, noreturn
    constexpr: bool,
    qualifiers: Qualifiers,             // const, volatile, restrict, _Atomic
    ty: TypeSpecifier,
    attributes: Vec<Span<Attribute>>,
}
```

The parser accepts conflicting specifiers where unambiguous; sema rejects.

### `TypeSpecifier`

The base type; never pointers, arrays, or functions (those are declarators).

| Variant | Source |
| --- | --- |
| `Void`, `Bool` | `void`, `_Bool`/`bool` |
| `Char { signed: Option<bool> }` | `char` / `signed char` / `unsigned char`; MS `__int8` |
| `Int { rank: Short \| Int \| Long \| LongLong \| Int128, signed }` | incl. `__int128_t`/`__uint128_t`; MS `__int16`/`__int32`/`__int64` |
| `BitInt { width: Expr, signed }` | `_BitInt(N)`, width unevaluated |
| `Float(FloatKind)` | `float`, `double`, `long double`, `_Float16`, `__fp16`, `_Float128`, … |
| `Float(Decimal32 \| Decimal64 \| Decimal128)` | `_Decimal32/64/128`; literals `DF`/`DD`/`DL` |
| `Complex(FloatKind)`, `Imaginary(FloatKind)` | |
| `FixedPoint { kind, rank, signed, saturated }` | `_Fract`/`_Accum`, any order, signed unless `unsigned` |
| `Atomic(TypeName)` | `_Atomic(T)` |
| `TypeOf { unqual: bool, operand: TypeOfOperand }` | `typeof(expr)` / `typeof(type-name)` |
| `TypedefName(Span<String>)` | known typedef name; `NodeId` keys the reference |
| `Tag(TagSpecifier)` | `struct`/`union`/`enum` |
| `TargetBuiltin(String)` | `__builtin_va_list` |
| `Inferred` | `__auto_type`, C23 `auto` |
| `Vector { element, size }` | GNU vectors |
| `Mode { base, mode }` | `__attribute__((mode(M)))`, name as spelled |

Decimal floating types are accepted in every mode and target
(`StandardFeatures::decimal_floating_point`: `Standard` from C23, else
`Extension`). IR lowering does not handle them yet.

### MS and GNU keywords

Gate: `microsoft_extensions` ([MS modes](compiler-flags.md#ms-modes)).
Elsewhere these are identifiers.

- `__intN` / `_intN`: `__int8/16/32` alias `char`/`short`/`int`; `__int64`
  is a `long long` width (`__int64 unsigned int`, `long __int64` parse).
- `__forceinline`: `is_inline` + `AlwaysInline` attribute.
- `__ptr32`, `__ptr64`, `__sptr`, `__uptr`: `Qualifiers::is_ptr32` etc.,
  after `*` or in specifier position; sema resolves them against the target
  ([MS mixed-size pointers](ir/places-pointers.md#ms-mixed-size-pointers)).
- `__unaligned`: `Qualifiers::is_unaligned`, a real qualifier for
  compatibility and discard warnings. Sets `_Alignof` and declared-object
  alignment to 1; never changes member layout. clang applies it to any
  type, cl.exe only to pointers.

gcc flavor, x86, gnu modes only: `__seg_fs` / `__seg_gs` are qualifiers
(`Qualifiers::is_seg_fs` / `is_seg_gs`). Under clang they are macros for
`address_space(257/256)`. Sema ignores both; `_Generic` doesn't distinguish
them.

### `vector_size` and `mode`

- Not written as specifiers. Each `vector_size`, `ext_vector_type`, or
  `mode` attribute in specifier position wraps the specifier, in attribute
  order, for all declarators.
- On or inside a declarator (`int (__attribute__((mode(QI))) x)`) it stays
  in the declarator's attributes and sema wraps a copy for that declarator
  (`TypeResolver::resolve_declarator`). Parameters and type names fold all
  attributes.
- `mode(SI) vector_size(8)` is a vector of the mode type;
  `vector_size(16) mode(DI)` keeps 16 bytes with 64-bit lanes.

Mode resolution (as clang):

- Integer modes (`QI`/`byte`, `HI`, `SI`, `DI`, `TI`, `word`/`pointer`)
  pick the first of `signed char`, `short`, `int`, `long`, `long long`,
  `__int128` with that width (`DI` = `long` on LP64, `long long` on LLP64
  and 32-bit).
- Signedness from the base: plain `char` per target, `_Bool` unsigned,
  enums via underlying type (result is the integer, not the enum).
- `SF` = `float`, `DF` = `double`, `XF` = `long double` only if x87, `TF` =
  `long double` if binary128 else `__float128` on non-MSVC x86; otherwise
  errors.
- Errors: integer mode on a floating base or vice versa; mode on a
  pointer, array, or function declarator. Complex, vector (`V4SI`), `HF`
  modes are unsupported.

### Floating keywords

- gcc flavor (all modes): `_Float32`, `_Float64`, `_Float32x`,
  `_Float64x`, `_Float128` are keywords; `__float80` on x86. Elsewhere
  identifiers (glibc typedefs them for clang).
- `_Float32/64/32x/64x` are distinct from `float`/`double`/`long double`.
  `_Float128` = `__float128`. `__float80` = x87 `long double`. `_Float64x`
  is f80 on x86, f128 on aarch64; it and `_Float128` error where no format
  exists (armv7).
- Suffixes `f32`/`f64`/`f32x`/`f64x` give those types; `w` gives
  `__float80`.
- gcc strict `-std=cNN` implies `-fno-asm`: `_Fract`/`_Accum`/`_Sat` are
  keywords only in gnu modes.

### `Declarator`

Read inside-out from the name.

```
Declarator =
    | Name(String)
    | Abstract                                    // type names, unnamed parameters
    | Grouped(Box<Declarator>)                    // parentheses
    | Pointer { qualifiers, attributes, inner }
    | Array { inner, size: ArraySize, qualifiers, is_static }   // `int a[static const 3]`
    | Function { inner, parameters: ParameterList }
    | Attributed { inner, attributes }

ArraySize = Unspecified | Expr(Expr) | Star      // [], [n], [*]

ParameterList =
    | Prototype { parameters: Vec<ParameterDeclaration>, variadic: bool }
    | IdentifierList { parameters: Vec<ParameterDeclaration> }   // K&R definition
    | Void                                        // (void)
    | Empty                                       // (); meaning per standard, decided by sema

ParameterDeclaration { specifiers, declarator: Declarator, attributes, comments: Vec<Span<CommentGroup>>, provenance }
```

`int (*fp)(int)` is
`Function { inner: Grouped(Pointer { inner: Name("fp") }), parameters: [int] }`.

`Attributed` holds attributes written inside a declarator:

| Source | AST |
| --- | --- |
| `(__attribute__((x)) *p)`, `(__cdecl *p)` | `Grouped(Attributed { inner: Pointer {..} })` |
| `a [[x]]` (after the identifier) | `Attributed { inner: Name("a") }` |
| `f(void) [[x]]`, `v[2] [[x]]` (after a suffix) | `Attributed { inner: Function {..} }` / `Array {..}` |

Only `[[...]]` is read after the identifier or a suffix; GNU attributes
there end the declarator and become `InitDeclarator.attributes`. `[[` never
starts an array bound.

K&R definitions (`f(a, b) int a; char b; { ... }`):

- `IdentifierList` in identifier order, each with its declared type
  (undeclared = implicit `int`). No promotion in the parser.
- Sema gives an unprototyped type with default-promoted parameters, passes
  promoted types in the ABI slots, and binds each body name to a local of
  the declared type converted from the slot.
- Rejected in C23 (`identifier_list_definitions`); K&R fixtures pin
  `SLATE-FILECHECK-STD DEFAULT c17`.

### `TypeName`

```
TypeName { specifiers: DeclarationSpecifiers, declarator: Declarator }   // abstract declarator
```

Used by casts, `sizeof`, `_Alignof`, compound literals, `va_arg`,
`offsetof`, `_Generic`, `typeof`. `_Alignof`, `__alignof`, `__alignof__`
are always the operator; `alignof` only with `keyword_alignof` (C23).
`_Countof` is the operator only with `keyword_countof` (gcc and clang in
every mode); msvc leaves it an identifier.

### `FunctionDefinition`

```
FunctionDefinition {
    specifiers: DeclarationSpecifiers,
    declarator: Declarator,                 // outermost derived layer is Function
    attributes: Vec<Span<Attribute>>,
    body: CompoundStatement,
    provenance,
}
```

- `int *f(void) { ... }`: specifiers `int`, declarator
  `Function { inner: Pointer { inner: Name("f") }, parameters: Void }`.
  The return type is derived by sema.
- GNU nested functions are `FunctionDefinition`s in block item position.

## Tags

```
TagSpecifier =
    | Reference { kind: Struct | Union | Enum, name: Span<String>, fixed_type: Option<Box<TypeName>> }
    | Definition(TagId)

TagDefinition { id: TagId, kind: TagKind, name: Option<String>, attributes, body: TagBody, provenance }

TagBody =
    | Record(Vec<MemberItem>)
    | Enum { fixed_type: Option<TypeName>, enumerators: Vec<EnumItem> }   // C23 `enum E : int`

MemberItem = Field(FieldDeclaration) | StaticAssert | CommentGroup
FieldDeclaration { specifiers, declarators: Vec<FieldDeclarator>, provenance }   // empty: anonymous member
FieldDeclarator { declarator: Declarator, bit_width: Option<Expr>, attributes, provenance }
EnumItem = Enumerator { name, value: Option<Expr>, attributes, provenance } | CommentGroup
```

- A stray `;` in a record body (`struct { int x; ; }`) leaves no
  `MemberItem`. gcc and clang accept it (their pedantic warning is
  not emitted, slate-parser-6x05.38.26); msvc rejects it.
- Definitions are stored once in `TranslationUnit.tags` and referenced by
  `TagId`, so anonymous tags have identity (`struct { int y; } g1, g2;`
  share one). Nested, block, and parameter-list definitions get their own
  `TagId`; scope is sema's.
- `Reference` is by name; its `NodeId` keys the reference or forward
  declaration. Resolution is sema's.
- Enumerator values are unevaluated; an omitted value is `None`.
- Enumerator attributes go between the name and `=` (C23 6.7.2.2), either
  spelling; not after the value. The IR drops them.

## Statements

`Stmt = Span<StmtKind>`.

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
    | Attribute(Vec<Span<Attribute>>)          // standalone [[fallthrough]];
    | Attributed { attributes, body: Box<Stmt> }
    | Asm(GnuAsm)
    | MsAsm(MsAsm)                              // MSVC __asm

SwitchLabel = Case(Expr) | CaseRange { start: Expr, end: Expr } | Default
```

- Control-flow bodies are one `Box<Stmt>`; braced bodies are `Block`.
  `Null` is `;`; an empty compound is `Block([])`.
- Scopes: C89/GNU89, only compound statements. C99+, each
  selection/iteration statement has a scope and each body a nested one
  (a braced body's `Block` is that scope). Condition and `for` clause
  declarations belong to the control statement. Parser typedef
  disambiguation and name resolution follow the same rules.
- `Labeled` (goto) and `SwitchLabel` (case/default) are separate variants.
  Both nest their target: `case 1: case 2: x;` is
  `SwitchLabel(Case 1, SwitchLabel(Case 2, Expr x))`. A label with nothing
  after it gets `Null`.
- Cases are found by walking the body (Duff's device). Case expressions and
  range endpoints use the expression parser.
- `For.init` is absent, a declaration, or an expression statement.
- `Attributed` keeps attachment without adding a scope. Attributes before a
  declaration stay on the declaration.

## Comments

```
CommentGroup { comment: Comment, attach: Leading | Trailing | Detached, doc: bool }
Comment { text: Vec<String>, loc: Loc }
```

Taxonomy and rationale: [comment-placement](comment-placement.md).

- The preprocessor records each comment's layout from the source text
  (`pp::CommentLayout`): own line, after code on the same line, or on a
  directive line; its column; and what follows it (a comment on the same or
  next line, code, or a blank line / directive / `}` / end of file).
  Conditional directives (`#if` … `#endif`) are looked through, so a comment
  directly above `#ifndef X` still leads the declaration inside.
- Comments after code on a line are emitted after that line's code, so they
  sit after the item they trail.
- Comments on conditional-directive lines (`#endif /* X */`) are dropped;
  comments on other directive lines (`#define X 1 /* doc */`) are `Detached`.
- Consecutive comments form one group unless a blank line separates them. A
  `Trailing` group only absorbs following own-line comments at the same
  column (a continuation). `text` keeps each comment's raw text; `loc` spans
  first to last. Never spans files.
- `attach`: `Trailing` if the group starts after code on its line; `Leading`
  if it is on its own lines and code follows directly; otherwise `Detached`.
  The owner of a `Leading` group is the next sibling; of a `Trailing` group,
  the previous sibling (the enclosing item if there is none).
- Comments inside a declaration, statement, or field (expressions,
  initializers, multi-line calls) become `Trailing` siblings after it.
- `doc`: the first comment starts with `/**`, `/*!`, `///`, or `//!`
  (not `/**/`, `/***`, `////`).
- Groups appear as `ExternalItem`, `BlockItem`, `MemberItem`, `EnumItem`,
  and `ParameterDeclaration.comments`. A parameter takes the comments inside
  its tokens and the `Trailing` comments after its comma.

## Expressions

`Expr = Box<Span<ExprKind>>`.

| Variant | Source |
| --- | --- |
| `Identifier(String)` | unresolved name |
| `IntegerLiteral`, `FloatLiteral`, `CharLiteral` | [Literals](#literals) |
| `StringLiteral(StringLiteral)` | adjacent literals concatenated |
| `Paren(Expr)` | kept for source form |
| `Unary { op: Plus \| Minus \| BitNot \| Not \| AddrOf \| Deref \| PreInc \| PreDec, operand }` | |
| `Postfix { op: PostInc \| PostDec, operand }` | |
| `Binary { op, left, right }` | arithmetic, shifts, comparisons, bitwise, `&&`, `\|\|` |
| `Assign { op, target, value }` | `=` and compound |
| `Conditional { condition, then_value: Option<Expr>, else_value }` | `None`: GNU `a ?: b` |
| `Comma { left, right }` | |
| `Call { callee, arguments }` | |
| `Member { base, field: Span<String>, arrow: bool }` | `.` / `->` |
| `Index { base, index }` | as written |
| `Cast { ty: TypeName, value }` | |
| `CompoundLiteral { ty: TypeName, storage, initializer: InitializerList }` | C23 storage |
| `SizeOfExpr`, `SizeOfType`, `AlignOf`, `AlignOfExpr` | |
| `CountOfExpr`, `CountOfType { ty: TypeName }` | C2y `_Countof`; parsed like `sizeof` |
| `OffsetOf { ty: TypeName, member: MemberDesignator }` | `offsetof`, `__builtin_offsetof` |
| `Generic { controlling: GenericControl, associations }` | |
| `VaArg { list, ty: TypeName }` | |
| `TypesCompatible(TypeName, TypeName)` | GNU |
| `BitCast { ty: TypeName, value }` | |
| `ConvertVector { ty: TypeName, value }` | `__builtin_convertvector` |
| `LabelAddress(Span<String>)` | GNU `&&label` |
| `StatementExpression(CompoundStatement)` | GNU `({ ... })` |

```
GenericControl = Expr(Expr) | Type(TypeName)
GenericAssociation = Type { ty: TypeName, value: Expr } | Default(Expr)
MemberDesignator = Vec<Field(Span<String>) | Index(Expr)>
```

- `_Generic` keeps all associations; sema selects. An expression operand is
  lvalue-converted (decay, drop top-level qualifiers), so array-type
  associations never match. A type-name operand (C2y; gcc and clang accept
  earlier) matches as written: `_Generic(const int, int: 1, const int: 2)`
  is 2.
- Whether an identifier is a type (`_Generic`, `sizeof(x)`, `(x)(y)`) comes
  from the typedef-name set.

## Literals

Lexical content is decoded; C types are assigned by sema.

```
IntegerLiteral {
    value: BigUint,
    radix: Decimal | Hex | Octal | Binary,
    suffix: IntegerSuffix { unsigned: bool, size: None | Long | LongLong | BitInt },
    spelling: String,
    imaginary: bool,                         // GNU i/j, anywhere among u/l
}

FloatLiteral {
    spelling: String,
    radix: Decimal | Hex,
    suffix: None | F | L | F16 | F32 | F64 | F128 | F32x | F64x | Q | W | DecimalF32 | DecimalF64 | DecimalF128,
    fixed_suffix: Option<{ kind: Fract | Accum, rank: Short | Default | Long | LongLong, unsigned: bool }>,
    imaginary: bool,
}

CharLiteral {
    encoding: Plain | Utf8 | Utf16 | Utf32 | Wide,
    code_units: Vec<u32>,                    // multichar keeps every unit
    spelling: String,
}

StringLiteral {
    encoding: Plain | Utf8 | Utf16 | Utf32 | Wide,   // after concatenation
    code_units: Vec<u32>,                    // no terminating NUL
    pieces: Vec<Span<String>>,               // each source literal's spelling
}
```

## Initializers

```
Initializer = Expr(Expr) | List(InitializerList)
InitializerList { items: Vec<InitializerItem>, trailing_comma: bool }
InitializerItem { designators: Vec<Designator>, value: Initializer }
Designator = Field(Span<String>) | Index(Expr) | IndexRange { start: Expr, end: Expr }   // GNU range
```

Designator indices are unevaluated.

## Attributes and asm

- `Attribute` is a closed set of known GNU/C23 attributes with parsed
  arguments. `Unknown { name, arguments }` covers unknown names and modeled
  spellings the flavor doesn't register for the target
  (`src/attribute_support.rs`, e.g. `dllimport` off Windows); malformed
  ones are `Invalid`. Each keeps its span and placement (specifiers,
  declarators, init-declarators, tag definitions, statements).
- `GnuAsm`: decoded template, operands with constraints, clobbers, labels.
  Asm string escapes decode per literal before concatenation and template analysis.
- `MsAsm`: instructions of optional label, prefixes, mnemonic, MASM
  operands. Registers, numbers, operators decoded; names left to sema.
  Exception: `TYPE int` in the msvc flavor parses to `TypeKeyword`.
  `Parser::mark_ms_asm_lines` first drops `;` comments and inserts
  `Newline` tokens. See [msvc-asm](msvc-asm.md).

## Calling conventions and `__declspec`

- `Attribute::CallingConvention`: `Cdecl`, `Stdcall`, `Fastcall`,
  `Vectorcall`, `Thiscall`, `MsAbi`, `SysVAbi`, `RegParm(Expr)`,
  `Pcs(Aapcs | AapcsVfp)`, from GNU attributes or MS keywords. Specifier
  positions apply to the declaration; nested positions stay on
  `Attributed`/`Pointer`; trailing ones in declarator attributes.
  `regparm` stays unevaluated. Sema checks support and conflicts and puts
  x86 conventions in the function type
  ([calling conventions](ir/calls-abi.md#calling-conventions)).
- `__declspec(...)`: single parentheses, space-separated entries.
  `dllimport`/`dllexport` → `DllImport`/`DllExport`; `align(expr)` →
  `Aligned(Expr)`. Registration per flavor
  (`attribute_support::declspec_registered`): clang only its own set
  (dllimport/dllexport only on Windows, no `__name__` unwrapping); gcc
  treats it as `__attribute__((x))` like mingw; msvc accepts every modeled
  name. Unregistered names become `IgnoredDeclspec { name, arguments }`
  with no effect. Registered unmodeled names are `Unknown`.

## Early validation

- `src/sema/validate.rs` removes invalid items and reports structural
  errors that need no name or type resolution. Surviving items aren't
  guaranteed valid; the rest of `src/sema/` checks the remainder before
  lowering.
- The parser keeps all declarations; pruning is
  [reachability pruning](ir/pipeline.md#reachability-pruning).

## Post-C89 constructs in older modes

Accepted in every mode as extensions (input is assumed to compile with the
real compiler). clang 22.1 and gcc 16.2 over `c89 gnu89 c99 c11 c17 gnu17
c23`, plain and `-pedantic`; `warn` = only under `-pedantic`.

| Construct | Since | clang before | gcc before | slate |
| --- | --- | --- | --- | --- |
| VLAs, `[*]` parameters | C99 | warn | warn | extension |
| compound literals | C99 | warn | warn | extension |
| designated initializers | C99 | warn | warn | extension |
| declarations after statements | C99 | warn | warn | extension |
| flexible array members | C99 | warn | warn | extension |
| variadic macros | C99 | warn | warn | extension |
| `//` comments | C99 | `c89` warn, `gnu89` ok | `c89` err, `gnu89` warn | extension |
| `for (int i…)` | C99 | warn | `c89`/`gnu89` err | gated by `control_statement_scopes` |
| `_Static_assert`, `_Generic`, `_Alignof`, `_Atomic`, `_Noreturn` | C11 | warn | warn | extension |
| `_BitInt` | C23 | warn | warn | extension ([`_BitInt`](ir/types.md#_bitint)) |
| `[[…]]` attributes | C23 | warn | warn | extension |
| `0b` literals | C23 | warn | warn | extension |
| `0o`/`0O` literals (radix `Octal`, also in `#if`) | C2y | warn | warn | extension; msvc rejects (`octal_prefix`) |
| `_Countof` | C2y | warn | warn | extension; msvc identifier (`keyword_countof`) |
| digit separators (`1'000`) | C23 | char constant | char constant | gated by `digit_separators` |

Digit separators are gated because pre-C23 `'` starts a character constant
in valid code: gcc.dg's `#define m(x) 0` / `m(1'2)+(3'4)` is 0 in C11 and
34 in C23. The lexer continues a pp-number through `'` only with
`StandardFeatures::digit_separators`. A separator consumes the next
character, so the `e`/`p` sign rule doesn't apply across it: `0x0'e-0xe` is
`0x0'e`, `-`, `0xe`.

## Migration

Where `src/ast.rs` differs from this spec.

| Current | Target |
| --- | --- |
| `TagSpecifier::Reference` holds an optional boxed fixed enum type; `TagBody::Record` holds `FieldItem` | `MemberItem`, `EnumItem`; opaque C23 enum declarations keep `: type` |
| `TypeSpecifier` uses `Integer(IntegerType)`, `Floating(FloatingType)`, `Complex(Box<TypeSpecifier>)`, `Named`, `TypeOf`/`TypeOfUnqual` | names and shapes in the `TypeSpecifier` table |
| `TranslationUnit.tags` is `Vec<Span<TagDefinition>>` with gaps after pruning; use `TranslationUnit::tag` | indexed by `TagId` once pruning moves to the IR pipeline |
| `Designator::Array` / `ArrayRange` | `Index` / `IndexRange` |
| bare `aligned` reads `__BIGGEST_ALIGNMENT__` in the parser | argument-less `Aligned`, value chosen in sema |
| `validate.rs` returns errors only; rejects tag definitions in parameter lists | returns checked AST plus diagnostics |
| parser calls `filter_translation_unit` before resolution | IR pipeline prunes resolved dependencies from explicit roots |
