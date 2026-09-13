# AST Spec

_created 2026-09-13 — evergreen: update whenever `src/ast.rs` or `src/const_expr.rs` changes_

What the parser's output means, as consumed by IR sema (`src/ir/sema`).
Type definitions live in `src/ast.rs` and `src/const_expr.rs`; this page
records the semantics and invariants that aren't visible from the types.
For where each enum is matched exhaustively see
[[ast-enum-touchpoints]]; for what the AST lowers into see [[ir-spec]].

## Invariants

- **Single configuration.** Preprocessing has already run for one set of
  `-D` defines and target predefines. There are no conditional nodes; every
  `#if` is resolved. See [[architecture_single_configuration]].
- **Target-independent shape, target-dependent content.** Types are C
  spellings (`Integer(Ranked { rank: Long, .. })`), never widths. But the
  predefines (`src/predefines/clang_x86_64_linux_gnu.h`) have already
  selected branches in headers, so the set of declarations is target-specific.
- **Not resolved.** Identifiers, typedef names, and tag references are
  strings. Nothing is typed. Implicit conversions are absent.
- **Source order.** `TranslationUnit.decls` and statement lists are in
  source order after preprocessing, including declarations from headers.
- **Unreachable code is marked, not removed.** `reachability::mark_unreachable`
  wraps statements after an unconditional jump (until the next label/case/
  default) in `Stmt::Unreachable`.

## Locations and provenance

- `Loc { file, offset, length }`: byte range in a file.
- `Span<T> { value, spelling, expansion }`: `spelling` is where the tokens
  are written (possibly inside a macro definition); `expansion` is where
  they appear after expansion in the including file. Neither records _which_
  macro expanded.
- `Provenance { file, kind, line, header }`: `kind` is `System`/`User`;
  `header` is the outermost header the main file directly included (`None`
  for the main file). This is what lets libc declarations be recognized.
- Spans exist on `Decl`, `Stmt`, `Expr`, and `FieldItem`. **`ConstExpr`
  nodes have no spans**, so sub-expressions have no location.

## TranslationUnit

```
TranslationUnit { decls: Vec<Span<Decl>>, flavor: CompilerFlavor }
```

`flavor` (`Gcc`/`Clang`/`Msvc`) is the dialect personality; it changes which
extensions parse and some semantics.

## Declarations

### `Decl`

| Variant                                       | Meaning                                                             |
| --------------------------------------------- | ------------------------------------------------------------------- |
| `Function(FunctionDecl)`                      | a function **definition** (has a body)                              |
| `Declaration { declaration, provenance }`     | any other file-scope declarator: object, function prototype, extern |
| `Typedef { name, ty, asm_label, attributes }` | `ty` is the fully applied type                                      |
| `Record(RecordDecl)`                          | a file-scope struct/union **definition** (with body)                |
| `Enum(EnumDecl)`                              | a file-scope enum **definition**                                    |
| `StaticAssert`                                | `_Static_assert`                                                    |
| `Asm`                                         | file-scope `asm("...")`                                             |
| `Comment`                                     | preserved source comment                                            |

A function prototype like `int printf(const char *restrict, ...);` is a
`Declaration` with a `Declarator::Function`, not a `Function`.

### Two type encodings

The same C type is encoded in two ways depending on the node:

- **Applied**: a single `CType` with pointers/arrays/functions already
  folded in. Used by `FunctionDecl.ret_type`, `Typedef.ty`, `CType` inside
  casts' `ty` _when the declarator is `Abstract`_.
  `int *f(void)` → `ret_type: Pointer { pointee: Integer(Int) }`.
- **Specifier + declarator**: a base `CType` plus a `Declarator` that must be
  read inside-out. Used by `Declaration`, `Parameter`, `FieldDecl`, and every
  `ConstExpr` that names a type (`Cast`, `SizeOfType`, `AlignOf`,
  `OffsetOf`, `VaArg`, `CompoundLiteral`, `BitCast`, `TypesCompatible`).
  `int (*fp)(int)` → `ty: Int`, `declarator: Function { inner: Grouped(Pointer { inner: Name("fp") }), parameters: [int] }`
  = pointer to function(int) returning int.

IR sema must fold every specifier+declarator pair into one type.

### `Declarator` (inside-out)

Apply from the outside of the tree toward `Name`/`Abstract`, each layer
wrapping the type built so far:

| Variant                                    | Wraps current type `T` into                       |
| ------------------------------------------ | ------------------------------------------------- |
| `Name(n)` / `Abstract`                     | end; `T` is the declared type (named / unnamed)   |
| `Pointer { qualifiers, inner }`            | `T *qualifiers`, continue with `inner`            |
| `Array { inner, size }`                    | array of `T`, continue with `inner`               |
| `Function { inner, parameters, variadic }` | function returning `T`                            |
| `Grouped(inner)`                           | parentheses; no type change, only changes binding |
| `Attributed { inner, attributes }`         | attributes on this declarator level               |

`ArraySize`: `Unspecified` (`[]`), `Expression(e)`, `Star` (`[*]` VLA in
prototypes).

### Qualifiers

- `CType::Qualified { qualifiers, ty }` wraps a qualified base type:
  `const char` in a parameter is `Qualified { is_const, Integer(Char) }`.
- Pointer qualifiers (`*const`, `*restrict`) are on `Declarator::Pointer` /
  `CType::Pointer`.
- `DeclarationSpecifiers.qualifiers` also exists; which qualifiers land there
  vs in `CType::Qualified` has not been audited. Sema should treat both as
  qualifiers of the base type.

### `DeclarationSpecifiers`

`ty` (base type), `qualifiers`, `storage` (`None`/`Typedef`/`Extern`/`Static`/
`Auto`/`Register`), `is_thread_local`, `is_inline`, `is_noreturn`,
`is_constexpr`.

### `FunctionDecl`

`ret_type` (applied), `name`, `parameters`, `variadic`, `body`,
`provenance`, `qualifiers`, `storage`, `is_inline`, `is_noreturn`,
`attributes`. The body has already been through `mark_unreachable`.

`parameters: []` with `variadic: false` is used both for `f(void)` and for
unprototyped `f()`; distinguishing them has not been audited.

### `Parameter`

`ty` (base) + optional `declarator` (`None` for a bare type like `int`),
`attributes`.

### Records and enums

- File-scope tag definitions are hoisted into their own `Decl::Record` /
  `Decl::Enum`, and the declarations that used them refer to the tag with
  `CType::Tagged { kind, name, body: None }`.
- Tag definitions inside function bodies stay inline:
  `CType::Tagged { name: Some("T"), body: Some(TagBody::Fields(..)) }`.
- `RecordDecl { kind, name, fields: Vec<Span<FieldItem>>, attributes }`.
  `FieldItem` is `Field(FieldDecl)` or `Comment`.
- `FieldDecl { declaration, bit_width: Option<Span<Expr>> }`. An anonymous
  member has `declarator: Abstract`.
- `EnumDecl { name, enumerators }`. `Enumerator.value` is an already-evaluated
  `Expr::IntLit` when written explicitly, `None` when implicit
  (previous + 1, starting at 0).

## Types: `CType`

| Variant                                     | Meaning                                                                                               |
| ------------------------------------------- | ----------------------------------------------------------------------------------------------------- |
| `Void`, `Bool`                              |                                                                                                       |
| `Integer(Char { signed: Option<bool> })`    | `None` = plain `char` (signedness is target-defined)                                                  |
| `Integer(Ranked { rank, signed })`          | `Short`/`Int`/`Long`/`LongLong`/`Int128`                                                              |
| `Integer(BitInt { width, signed })`         | `_BitInt(N)`; `width` is an unevaluated `ConstExpr`                                                   |
| `Floating(..)`                              | `Float`, `Double`, `LongDouble`, `Float16`, `Fp16`, `BFloat16`, `Float64x`, `Float128`, `Float128Ext` |
| `Complex(T)`, `Imaginary(T)`                | `_Complex` / `_Imaginary` of `T`                                                                      |
| `Atomic(T)`                                 | `_Atomic(T)`                                                                                          |
| `Vector(VectorType)`                        | GNU vector types                                                                                      |
| `FixedPoint(..)`                            | `_Fract`/`_Accum`                                                                                     |
| `TypeOf(op)`, `TypeOfUnqual(op)`            | `typeof(expr or type)`; not evaluated                                                                 |
| `TargetBuiltin(name)`                       | target builtin type names (e.g. `__builtin_va_list`)                                                  |
| `Named(name)`                               | **an unresolved identifier used as a type**, normally a typedef name                                  |
| `Tagged { kind, name, body }`               | struct/union/enum reference or inline definition                                                      |
| `Qualified`, `Pointer`, `Array`, `Function` | applied forms (see above)                                                                             |

`__int128_t`/`__uint128_t` are parsed directly to `Ranked { Int128 }`, not
`Named`.

## Statements: `Stmt`

| Variant                                                      | Meaning                                                                                          |
| ------------------------------------------------------------ | ------------------------------------------------------------------------------------------------ |
| `Expr(e)`                                                    | expression statement                                                                             |
| `Decl(Declaration)`                                          | one local declarator                                                                             |
| `Block(stmts)`                                               | `{ ... }` — **also used for multi-declarator local declarations**; see gaps                      |
| `Return(e)` / `ReturnVoid`                                   |                                                                                                  |
| `If`, `While`, `DoWhile`, `For`                              | `For.init` is a boxed `Stmt` (declaration or expression)                                         |
| `Switch { discriminant, body }`                              | `body` is flat; `Case`/`CaseRange`/`Default` are **marker statements** inside it, not containers |
| `Case(e)`, `CaseRange { start, end }`, `Default`             | markers; fallthrough is implicit                                                                 |
| `Labeled(name)`                                              | a label **marker**; the labeled statement is the next statement                                  |
| `Goto(name)`, `ComputedGoto(e)`                              |                                                                                                  |
| `Break`, `Continue`                                          |                                                                                                  |
| `LocalLabelDecl(names)`                                      | GNU `__label__`                                                                                  |
| `NestedFunction(FunctionDecl)`                               | GNU nested function                                                                              |
| `StaticAssert`, `Attribute(attrs)`, `Asm(GnuAsm)`, `Comment` |                                                                                                  |
| `Unreachable(stmt)`                                          | wrapper added by `mark_unreachable`                                                              |

## Expressions

### `Expr` (statement-level wrapper)

In practice the parser produces only three variants:

- `Expr::Const(ConstExpr)`: **every** expression in statements,
  initializers, conditions and bit-field widths.
- `Expr::IntLit(i64)`: already-evaluated enumerator values.
- `Expr::StatementExpression(stmts)`: a GNU `({ ... })` that _starts_ a
  statement, with parsed statements.

`Expr::Identifier`, `Unary`, `Binary`, `SizeOf`, and the string-literal
variants are still matched by `sema.rs`/`reachability.rs` but not constructed
by the parser.

### `ConstExpr` (the real expression tree)

| Variant                                                                           | Meaning                                                                                                         |
| --------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- | --- | --------- |
| `Integer(i64)`                                                                    | integer **or character** literal value; suffix and char-ness are dropped                                        |
| `WideInteger(WideInt)`                                                            | integer literal too large for `i64`; carries width + signedness from the suffix                                 |
| `Float(FloatLiteral)`                                                             | value in its suffix's format (`Single`/`Double`/`LongDouble` f80 bits/`Quad`/`Half`/decimal), `imaginary` flag  |
| `StringLit`, `Utf8StringLit`, `Utf16StringLit`, `Utf32StringLit`, `WideStringLit` | adjacent literals concatenated; **content is the raw spelling with escapes undecoded** (`"%d\n"` holds `\` `n`) |
| `Identifier(name)`                                                                | unresolved: variable, function, enumerator                                                                      |
| `Unary { op }`                                                                    | `+ - ~ !`                                                                                                       |
| `Binary { op }`                                                                   | arithmetic, comparison, bitwise, `&&`/`                                                                         |     | `, shifts |
| `Assign { op, target, value }`                                                    | `=` and all compound assignments                                                                                |
| `PreIncrement`, `PreDecrement`, `PostIncrement`, `PostDecrement`                  |                                                                                                                 |
| `AddrOf`, `Deref`                                                                 | `&e`, `*e`                                                                                                      |
| `Member { base, field }`, `Arrow { base, field }`                                 | `.` / `->`                                                                                                      |
| `Index { base, index }`                                                           | `base[index]` (operands not normalized; `2[a]` stays as written)                                                |
| `Call { callee, arguments }`                                                      |                                                                                                                 |
| `Ternary`, `Elvis`                                                                | `c ? a : b`, GNU `c ?: b`                                                                                       |
| `Comma(a, b)`                                                                     |                                                                                                                 |
| `Cast { ty, declarator, value }`                                                  | explicit cast                                                                                                   |
| `CompoundLiteral { ty, declarator, initializer }`                                 | `(T){ ... }`                                                                                                    |
| `SizeOf(e)`, `SizeOfType`, `AlignOf`, `OffsetOf`                                  | unevaluated                                                                                                     |
| `Generic { controlling, associations }`                                           | `_Generic`; association `type_name` is the joined token spelling (`"unsigned int"`), `None` for `default`       |
| `TypesCompatible`, `BitCast`, `VaArg`                                             | GNU builtins                                                                                                    |
| `LabelAddr(name)`                                                                 | GNU `&&label`                                                                                                   |
| `StatementExpression(tokens)`                                                     | GNU `({ ... })` nested inside an expression; **raw unparsed tokens**                                            |

## Initializers

`Initializer::Expr(e)` or `Initializer::List(items)`; each
`InitializerItem { designators, value }` where `Designator` is
`Field(name)`, `Array(i64)`, or GNU `ArrayRange { start, end }`.

## Attributes and asm

`Attribute` is a closed set of known GNU/C23 attributes with parsed
arguments; unknown ones are `Unknown { name, arguments }`, malformed ones
`Invalid`. `GnuAsm` holds a parsed template, operands with constraints, and
clobbers; `AsmLabel` is `asm("sym")` / register variables.

## Known gaps

Things IR sema cannot recover from the AST as it stands. Tracked as bugs
under `slate-parser-lh7.1`.

| Gap                                                                                                                                               | Effect                                                                    |
| ------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| File-scope `struct { .. } a, b;` drops `b`, and `a`'s `Tagged { name: None, body: None }` can't be linked to its hoisted anonymous `Decl::Record` | lost declaration; anonymous tag identity unrecoverable                    |
| Local `int a, b;` is `Stmt::Block([Decl a, Decl b])`                                                                                              | indistinguishable from a real block, so `a`/`b` would get the wrong scope |
| Integer suffixes (`u`, `l`, `ll`) dropped from `ConstExpr::Integer`; char literals become `Integer`                                               | literal type (`unsigned`, `long`, `int` for chars) unrecoverable          |
| String literal escapes not decoded                                                                                                                | length and contents must be re-lexed by consumers                         |
| `_Generic(a, ..)` with an identifier controlling expression becomes `Identifier("<type-name>")`                                                   | controlling expression lost                                               |
| `ConstExpr` has no spans or macro-expansion identity                                                                                              | no per-subexpression location or macro provenance                         |
| Nested `({ ... })` is raw tokens                                                                                                                  | consumer must re-parse                                                    |
| Fields of inline (local) tag definitions get `Provenance::default()`                                                                              | wrong provenance                                                          |
