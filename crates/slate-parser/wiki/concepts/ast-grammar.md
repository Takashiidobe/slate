# AST Grammar

The grammar of the AST dump printed by `slate-parser parse source.c`. It is
the reference for what each AST node can be and which choices it has.
[AST Spec](ast-spec.md) explains what the nodes mean and why the AST has this
shape.

The types in `src/ast.rs` and the literal and operator types in
`src/const_expr.rs` are the source of truth. The dump is their Rust `Debug`
form (derived, or `custom_debug` where fields are skipped) as laid out by
`src/render.rs`. **Any change to those types, their `Debug` output or the
renderer must update this file in the same change.**

## Notation

- `a = ... ;` defines `a`; `|` separates choices; `[ x ]` is optional;
  `{ x }` is zero or more; `( ... )` groups; `(* ... *)` is a comment.
- Node shapes are written as they print:
  - `Name { f: t, g: t }` is a struct or struct variant.
  - `Name(t)` is a tuple variant.
  - A bare `Name` is a unit variant.
  - A field written `g?: t` is omitted when it is empty, `None`, `false` or
    default. When it is printed it is `t`, so `g?: Some(t)` means the field
    is absent or `Some(..)`, and `g?: true` means absent or `true`.
- `opt<t>` is `"Some(" t ")" | "None"`, `vec<t>` is `"[" [ t { "," t } ] "]"`,
  and `span<t>` is a [spanned node](#spans) holding `t`.
- `Box` is transparent. `expr` is `span<ExprKind>` and `stmt` is
  `span<StmtKind>`.
- Layout: the renderer prints one field or element per line, indented 4
  spaces per level, with a trailing `,` after each. The productions use the
  flat form; whitespace and trailing commas are layout.

## Lexical

```ebnf
int    = digits ;
bool   = "true" | "false" ;
string = '"' { char | escape } '"' ;        (* Rust {:?} quoting *)
char   = "'" ( char | escape ) "'" ;
FileId = "FileId(" int ")" ;
TagId  = "TagId(" int ")" ;
```

## Dump

```ebnf
dump      = { tag_line } { decl_line } ;
tag_line  = "tag[" int "]" [ " #" int ] ": " span<TagDefinition> ;
decl_line = "decl[" int "]" [ " #" int ] ": " span<DeclKind> ;
```

- All tag definitions print first, ordered by `TagId`; the index in
  `tag[N]` is the `TagId`. Declarations follow in source order, `decl[N]`
  being the position.
- The `TranslationUnit`'s `dialect` (flavor, standard, target, options) is
  not printed.
- Without `--show-comments`, `Comment` items are removed from declarations,
  function bodies, records and enums before printing.

## Spans

```ebnf
span<t>    = [ "#" int " " ] ( t | "Spanned {" "value:" t ","
                                   "provenance:" Provenance "}" ) ;
Provenance = "Provenance {" "file:" FileId "," "kind:" ( "System" | "User" ) ","
             "line:" int "," "system_header:" opt<FileId> "}" ;
```

- `#N` is the `NodeId`, printed only with `--show-ids`. The line label then
  repeats it: `decl[2] #2869: #2869 Declaration(..)`.
- A span prints bare unless it came from a system header, in which case it
  is wrapped in `Spanned` with its provenance. Locations (`spelling`,
  `expansion`) and macro origins are never printed.

## Declarations

```ebnf
DeclKind = "Comment(" CommentGroup ")"
         | "Function(" FunctionDefinition ")"
         | "Declaration(" Declaration ")"
         | "StaticAssert(" StaticAssert ")"
         | "Asm(" GnuAsm ")"
         | "Pragma(" Pragma ")" ;

FunctionDefinition = FunctionDefinition {
                       specifiers: DeclarationSpecifiers,
                       declarator: Declarator,
                       attributes?: vec<span<Attribute>>,
                       body?: vec<stmt> } ;
Declaration        = Declaration {
                       specifiers: DeclarationSpecifiers,
                       declarators?: vec<span<InitDeclaratorKind>> } ;
InitDeclaratorKind = InitDeclaratorKind {
                       declarator: Declarator,
                       asm_label?: Some(span<AsmLabel>),
                       attributes?: vec<span<Attribute>>,
                       initializer?: Some(Initializer) } ;
StaticAssert       = StaticAssert { condition: expr, message?: Some(string) } ;

CommentGroup = CommentGroup { comment: Comment } ;
Comment      = Comment { text: vec<string>, loc: Loc } ;
Loc          = Loc { file: FileId, offset: int, length: int } ;
```

- There is no typedef, record or enum declaration: `typedef` is a storage
  class, and tag bodies are in `tag[..]` lines, referenced by `TagId`.
- A `FunctionDefinition` with an empty body prints without `body`.

## Specifiers

```ebnf
DeclarationSpecifiers = DeclarationSpecifiers {
                          ty: TypeSpecifier,
                          qualifiers?: Qualifiers,
                          storage?: StorageClass,
                          is_thread_local?: true,
                          is_inline?: true,
                          is_noreturn?: true,
                          is_constexpr?: true,
                          attributes?: vec<span<Attribute>> } ;
Qualifiers   = Qualifiers { is_const?: true, is_volatile?: true,
                            is_restrict?: true, is_atomic?: true,
                            is_unaligned?: true, is_ptr32?: true,
                            is_ptr64?: true, is_sptr?: true,
                            is_uptr?: true, is_seg_fs?: true,
                            is_seg_gs?: true } ;
StorageClass = "Typedef" | "Extern" | "Static" | "Auto" | "Register" ;
TypeName     = TypeName { specifiers: DeclarationSpecifiers,
                          declarator: Declarator } ;
```

`Qualifiers` with every field false prints as the bare word `Qualifiers`.

## Type specifiers

```ebnf
TypeSpecifier = "Void" | "Bool"
              | "Integer(" IntegerType ")"
              | "Floating(" FloatingType ")"
              | "Complex(" TypeSpecifier ")"
              | "Imaginary(" TypeSpecifier ")"
              | "Atomic(" TypeName ")"
              | "Vector(" VectorType ")"
              | "Mode(" ModeType ")"
              | "FixedPoint(" FixedPointType ")"
              | "TypeOf(" TypeOfOperand ")"
              | "TypeOfUnqual(" TypeOfOperand ")"
              | "TargetBuiltin(" string ")"
              | "Inferred"
              | "Named(" span<string> ")"
              | "Tag(" TagSpecifier ")" ;

IntegerType  = Char { signed: opt<bool> }
             | Ranked { rank: IntegerRank, signed: bool }
             | BitInt { width: expr, signed: bool } ;
IntegerRank  = "Short" | "Int" | "Long" | "LongLong" | "Int128" ;
FloatingType = "BFloat16" | "Float" | "Float16" | "Fp16"
             | "Float32" | "Float64" | "Float32x" | "Float64x"
             | "Double" | "LongDouble" | "Float128" | "Float128Ext" | "Float80"
             | "Decimal32" | "Decimal64" | "Decimal128" ;

VectorType     = VectorType { element: TypeSpecifier, size: VectorSize } ;
VectorSize     = "Bytes(" expr ")" | "Lanes(" expr ")" ;
ModeType       = ModeType { base: TypeSpecifier, mode: string } ;
FixedPointType = FixedPointType {
                   kind: ( "Fract" | "Accum" ),
                   rank: ( "Default" | "Short" | "Long" | "LongLong" ),
                   signed: bool,
                   saturated: bool } ;
TypeOfOperand  = "Expression(" expr ")" | "Type(" TypeName ")" ;

TagSpecifier = Reference { kind: TagKind, name: span<string>,
                           fixed_type?: Some(TypeName) }
             | "Definition(" TagId ")" ;
TagKind      = "Struct" | "Union" | "Enum" ;
```

- `Char { signed: None }` is plain `char`, distinct from `signed char` and
  `unsigned char`.
- `Named` is a typedef name; `TargetBuiltin` is a compiler-provided type
  name such as `__builtin_va_list`. `Inferred` is `__auto_type`, or C23
  `auto` with no type specifier (alone or beside another storage class).
- `Definition(TagId(N))` is where a tag body was written; the body is
  `tag[N]`. `Reference` names a tag without a body.

## Declarators

```ebnf
Declarator = "Abstract"
           | "Name(" string ")"
           | "Grouped(" Declarator ")"
           | Attributed { inner: Declarator, attributes: vec<span<Attribute>> }
           | Pointer { qualifiers: Qualifiers, attributes?: vec<span<Attribute>>,
                       inner: Declarator }
           | Array { inner: Declarator, size: ArraySize,
                     qualifiers?: Qualifiers, is_static?: true }
           | Function { inner: Declarator, parameters: ParameterList } ;
ArraySize  = "Unspecified" | "Expression(" expr ")" | "Star" ;

ParameterList            = Prototype { parameters: vec<span<ParameterDeclarationKind>>,
                                       variadic?: true }
                         | IdentifierList { parameters: vec<span<ParameterDeclarationKind>> }
                         | "Void" | "Empty" ;
ParameterDeclarationKind = ParameterDeclarationKind {
                             specifiers: DeclarationSpecifiers,
                             declarator: Declarator,
                             attributes?: vec<span<Attribute>> } ;
```

- Declarators keep the written nesting, not the derivation order:
  `int *a[3]` (an array of pointers) is
  `Array { inner: Pointer { inner: Name("a") } }`, and `int (*fp)(int)` is
  `Function { inner: Grouped(Pointer { inner: Name("fp") }) }`. `Grouped` is
  a parenthesized declarator.
- `Void` is `(void)`; `Empty` is `()`, an unprototyped list.
- `IdentifierList` is a K&R definition; its parameters keep the declared
  types, and promotion happens in sema.

## Tags

```ebnf
TagDefinition = TagDefinition { id: TagId, kind: TagKind, name: opt<string>,
                                attributes?: vec<span<Attribute>>, body: TagBody } ;
TagBody       = "Record(" vec<span<FieldItemKind>> ")"
              | Enum { fixed_type?: Some(TypeName),
                       enumerators: vec<span<EnumItemKind>> } ;

FieldItemKind       = "Comment(" CommentGroup ")" | "Field(" FieldDecl ")" ;
FieldDecl           = FieldDecl { specifiers: DeclarationSpecifiers,
                                  declarators?: vec<span<FieldDeclaratorKind>> } ;
FieldDeclaratorKind = FieldDeclaratorKind { declarator: Declarator,
                                            bit_width?: Some(expr),
                                            attributes?: vec<span<Attribute>> } ;

EnumItemKind = "Comment(" CommentGroup ")" | "Enumerator(" Enumerator ")" ;
Enumerator   = Enumerator { name: string, attributes?: vec<span<Attribute>>,
                            value: opt<expr> } ;
```

A `FieldDecl` without declarators is an anonymous struct or union member.

## Statements

```ebnf
StmtKind = "Null" | "Break" | "Continue" | "ReturnVoid"
         | "Comment(" CommentGroup ")"
         | "Return(" expr ")"
         | "Expr(" expr ")"
         | "Decl(" Declaration ")"
         | "StaticAssert(" StaticAssert ")"
         | "Attribute(" vec<span<Attribute>> ")"
         | Attributed { attributes: vec<span<Attribute>>, body: stmt }
         | "Block(" vec<stmt> ")"
         | If { condition: expr, then_branch: stmt, else_branch: opt<stmt> }
         | While { condition: expr, body: stmt }
         | DoWhile { body: stmt, condition: expr }
         | For { init: opt<stmt>, condition: opt<expr>,
                 increment: opt<expr>, body: stmt }
         | Switch { discriminant: expr, body: stmt }
         | Labeled { label: span<string>, body: stmt }
         | SwitchLabel { label: SwitchLabel, body: stmt }
         | "LocalLabelDecl(" vec<span<string>> ")"
         | "Goto(" span<string> ")"
         | "ComputedGoto(" expr ")"
         | "Asm(" GnuAsm ")"
         | "MsAsm(" MsAsm ")"
         | "NestedFunction(" FunctionDefinition ")"
         | "Pragma(" Pragma ")" ;
SwitchLabel = "Case(" expr ")"
            | CaseRange { start: expr, end: expr }
            | "Default" ;
```

- `Attribute` is an attribute declaration (`[[fallthrough]];`);
  `Attributed` is attributes applied to the statement that follows.
- `For.init` is an expression or declaration statement.
- `LocalLabelDecl` is GNU `__label__`; `ComputedGoto` is `goto *e`.

## Expressions

```ebnf
ExprKind = "Identifier(" string ")"
         | "IntegerLiteral(" IntegerLiteral ")"
         | "FloatLiteral(" FloatLiteral ")"
         | "CharLiteral(" CharLiteral ")"
         | "StringLiteral(" StringLiteral ")"
         | "BoolLiteral(" bool ")"
         | "NullPtrLiteral"
         | "Paren(" expr ")"
         | Unary { op: UnaryOp, operand: expr }
         | Postfix { op: PostfixOp, operand: expr }
         | Binary { op: BinaryOp, left: expr, right: expr }
         | Assign { op: AssignOp, target: expr, value: expr }
         | Conditional { condition: expr, then_value?: Some(expr),
                         else_value: expr }
         | Comma { left: expr, right: expr }
         | Call { callee: expr, arguments: vec<expr> }
         | Member { base: expr, field: span<string>, arrow?: true }
         | Index { base: expr, index: expr }
         | Cast { ty: TypeName, value: expr }
         | CompoundLiteral { ty: TypeName, initializer: vec<InitializerItem> }
         | "SizeOfExpr(" expr ")"
         | SizeOfType { ty: TypeName }
         | "AlignOfExpr(" expr ")"
         | AlignOf { ty: TypeName }
         | OffsetOf { ty: TypeName, member: expr }
         | Generic { controlling: GenericControl,
                     associations: vec<GenericAssociation> }
         | VaArg { list: expr, ty: TypeName }
         | TypesCompatible { left_ty: TypeName, right_ty: TypeName }
         | BitCast { ty: TypeName, value: expr }
         | ConvertVector { ty: TypeName, value: expr }
         | "LabelAddress(" span<string> ")"
         | "StatementExpression(" vec<stmt> ")" ;

GenericControl     = "Expr(" expr ")" | Type { ty: TypeName } ;
GenericAssociation = Type { ty: TypeName, value: expr } | "Default(" expr ")" ;

UnaryOp   = "Plus" | "Minus" | "BitNot" | "Not" | "AddrOf" | "Deref"
          | "PreIncrement" | "PreDecrement" | "Real" | "Imag" ;
PostfixOp = "Increment" | "Decrement" ;
BinaryOp  = "Add" | "Sub" | "Mul" | "Div" | "Rem"
          | "Less" | "LessEqual" | "Greater" | "GreaterEqual"
          | "Equal" | "NotEqual"
          | "BitAnd" | "BitXor" | "BitOr" | "And" | "Or"
          | "ShiftLeft" | "ShiftRight" ;
AssignOp  = "Assign" | "AddAssign" | "SubAssign" | "MulAssign" | "DivAssign"
          | "RemAssign" | "BitAndAssign" | "BitOrAssign" | "BitXorAssign"
          | "ShiftLeftAssign" | "ShiftRightAssign" ;
```

- `Conditional` without `then_value` is GNU `a ?: b`.
- `Member.arrow` is `->`; its absence is `.`.
- `OffsetOf.member` is the member designator written as an expression
  (identifiers, `Member` and `Index`).
- `Real`/`Imag` are GNU `__real__`/`__imag__`; `And`/`Or` are `&&`/`||`.

## Literals

```ebnf
IntegerLiteral = IntegerLiteral { value: digits, radix: Radix,
                                  suffix: IntegerSuffix, spelling: string,
                                  imaginary?: true } ;
Radix          = "Decimal" | "Hex" | "Octal" | "Binary" ;
IntegerSuffix  = IntegerSuffix { unsigned: bool,
                                 size: ( "None" | "Long" | "LongLong" | "BitInt" ) } ;

FloatLiteral = FloatLiteral { spelling: string, radix: ( "Decimal" | "Hex" ),
                              suffix: FloatSuffix, fixed_suffix?: FixedPointLiteralSuffix,
                              imaginary?: true } ;
FixedPointLiteralSuffix = FixedPointLiteralSuffix { kind: ( "Fract" | "Accum" ),
                                                   rank: FixedPointRank,
                                                   unsigned: bool } ;
FloatSuffix  = "None" | "F" | "L" | "BF16" | "F16" | "F32" | "F64" | "F128"
             | "F32x" | "F64x" | "Q" | "W"
             | "DecimalF32" | "DecimalF64" | "DecimalF128" ;

CharLiteral   = CharLiteral { encoding: Encoding, code_units: vec<int>,
                              spelling: string } ;
StringLiteral = StringLiteral { encoding: Encoding, code_units: vec<int>,
                                pieces: vec<span<string>> } ;
Encoding      = "Plain" | "Utf8" | "Utf16" | "Utf32" | "Wide" ;
```

- An integer literal's `value` is the decoded magnitude in decimal; its type
  is not resolved in the AST.
- A float literal keeps only its spelling; sema interprets it at the target
  precision.
- `pieces` are the adjacent string tokens that were concatenated, in order.
  `code_units` is the decoded concatenation.

## Initializers

```ebnf
Initializer     = "Expr(" expr ")" | "List(" vec<InitializerItem> ")" ;
InitializerItem = InitializerItem { designators: vec<Designator>,
                                    value: Initializer } ;
Designator      = "Array(" expr ")"
                | ArrayRange { start: expr, end: expr }
                | "Field(" span<string> ")" ;
```

`ArrayRange` is GNU `[a ... b]`.

## Pragmas

```ebnf
Pragma     = Pragma { kind: PragmaKind } ;
PragmaKind = Pack { action: StackAction, label?: opt<string>,
                    alignment: opt<expr> }
           | Weak { name: string, alias: opt<string> }
           | Visibility { action: StackAction, visibility: opt<string> }
           | Stdc { option: ( "FenvAccess" | "FpContract" | "CxLimitedRange" ),
                    value: ( "On" | "Off" | "Default" ) }
           | "FloatControl(" FloatControl ")"
           | MsStruct { action: ( "On" | "Off" | "Reset" ) }
           | "Opaque(" string ")" ;
FloatControl = Set { option: ( "Precise" | "Except" ), enabled: bool,
                     push: bool }
             | "Push" | "Pop" | "Malformed" ;
StackAction = "Push" | "Pop" | "Show" | "Set" ;
```

`Opaque` holds the text of any pragma without a typed form, including a
recognized pragma given an argument it does not accept -- `#pragma ms_struct
push` and `#pragma STDC FP_CONTRACT maybe` both land there.

`Pack`'s `label` is the MSVC named slot: `#pragma pack(push, lbl, 1)` fills
both `label` and `alignment`, `#pragma pack(pop, lbl)` only `label`.

## Inline assembly

```ebnf
GnuAsm      = GnuAsm { qualifiers?: vec<span<AsmQualifier>>,
                       template: span<string>,
                       operands?: Some(AsmOperands) } ;
AsmQualifier = "Volatile" | "Inline" | "Goto" ;
AsmOperands = AsmOperands { pieces: vec<AsmTemplatePiece>,
                            outputs?: vec<AsmOperand>,
                            inputs?: vec<AsmOperand>,
                            clobbers?: vec<span<AsmClobber>>,
                            labels?: vec<span<string>> } ;
AsmTemplatePiece = "Text(" string ")"
                 | Operand { index: int, modifier?: Some(char) }
                 | "Label(" int ")"
                 | "Percent" | "UniqueId"
                 | "DialectAlternatives(" vec<vec<AsmTemplatePiece>> ")" ;
AsmOperand  = AsmOperand { name?: Some(span<string>),
                           constraint: span<AsmConstraint>, expr: expr } ;
AsmConstraint            = AsmConstraint { alternatives: vec<AsmConstraintAlternative> } ;
AsmConstraintAlternative = AsmConstraintAlternative {
                             modifiers?: vec<AsmConstraintModifier>,
                             location: AsmConstraintLocation } ;
AsmConstraintModifier    = "Overwrite" | "ReadWrite" | "EarlyClobber"
                         | "Commutative" | "Pic" ;
AsmConstraintLocation    = "HardRegister(" Register ")"
                         | "Matching(" int ")"
                         | "Letters(" string ")" ;
AsmClobber = "Memory" | "Cc" | "Unwind" | "Register(" Register ")" ;
AsmLabel   = "Symbol(" string ")" | "Register(" Register ")" ;

Register     = "X86(" RegisterInfo<X86Width> ")"
             | "Aarch64(" RegisterInfo<AArch64Width> ")"
             | "Other(" string ")" ;
RegisterInfo<w> = RegisterInfo { spelling: string, number: int,
                                 canonical: string, width: opt<w> } ;
X86Width     = "Low8" | "High8" | "Bits16" | "Bits32" | "Bits64" ;
AArch64Width = "Bits8" | "Bits16" | "Bits32" | "Bits64" | "Bits128"
             | "ScalableVector" | "ScalablePredicate" ;
```

- `operands` is absent for basic asm (`asm("...")` without colons).
- `pieces` is the template split into text and `%` references; `Label(N)`
  is a reference to a goto label, as an index into `labels`.
- `DialectAlternatives` is a bare `{att|intel|...}` in an x86 template; the
  escapes `%{`, `%|` and `%}` are literal text. On other targets braces and
  `|` are always text.
- `AsmLabel` is a declarator's `asm("name")`: a symbol name, or a register
  for a GNU register variable.

```ebnf
MsAsm            = MsAsm { instructions: vec<span<MsAsmInstruction>> } ;
MsAsmInstruction = MsAsmInstruction { label?: Some(span<string>),
                                      prefixes?: vec<span<string>>,
                                      mnemonic?: Some(span<string>),
                                      operands?: vec<span<MsAsmExpr>> } ;
MsAsmExpr = "Register(" Register ")"
          | "SegmentRegister(" MsAsmSegment ")"
          | "St(" int ")"
          | "Number(" int ")"
          | "Name(" string ")"
          | Member { base: span<MsAsmExpr>, field: span<string> }
          | Index { base: span<MsAsmExpr>, index: span<MsAsmExpr> }
          | "Bracket(" span<MsAsmExpr> ")"
          | Binary { op: MsAsmBinaryOp, lhs: span<MsAsmExpr>, rhs: span<MsAsmExpr> }
          | "Negate(" span<MsAsmExpr> ")"
          | Ptr { size: MsAsmSize, operand: span<MsAsmExpr> }
          | Segment { segment: MsAsmSegment, operand: span<MsAsmExpr> }
          | Operator { operator: MsAsmOperator, operand: span<MsAsmExpr> }
          | "TypeKeyword(" MsAsmTypeKeyword ")" ;       (* only as the operand of TYPE, msvc flavor *)
MsAsmTypeKeyword = "Char" | "Short" | "Int" | "Long" | "Int64" | "Float" | "Double"
                 | "Signed" | "Unsigned" ;
MsAsmSegment  = "Es" | "Cs" | "Ss" | "Ds" | "Fs" | "Gs" ;
MsAsmBinaryOp = "Add" | "Sub" | "Mul" | "Div" ;
MsAsmSize     = "Byte" | "Word" | "Dword" | "Fword" | "Qword" | "Tbyte"
              | "Mmword" | "Xmmword" | "Ymmword" | "Zmmword" | "Oword"
              | "Real4" | "Real8" | "Real10" ;
MsAsmOperator = "Offset" | "Type" | "Length" | "Size" | "Short" ;
```

- `MsAsm` is one MSVC `__asm` statement, after merging (see
  [MSVC inline asm](msvc-asm.md)). An instruction with only `label` is a
  label on its own line; `a: b: nop` gives a label-only instruction for `a`.
- `Register` is always `X86`, and `spelling` keeps the source case. `St(n)`
  is `st(n)`; a bare `st` is `St(0)`. `Name` is anything else: a C name, an
  asm label or a field, resolved by sema.
- `Number` is already folded from its MASM radix suffix or C spelling.
- `Index` is MASM's `x[4]`, a byte offset, not a C subscript. A `(...)`
  group prints as its contents. `[eax]T.f` prints as `Binary` `Add` of the
  bracket and `T.f`.

## Attributes

```ebnf
Attribute = (* no arguments *)
            "Packed" | "LifetimeBound" | "Overloadable" | "GnuInline"
          | "NoThrow" | "SelectAny" | "ThreadLocal" | "NoAlias"
          | "RestrictReturn" | "OptimizeNone" | "Weak" | "Used" | "Retain"
          | "NoInline" | "AlwaysInline" | "NoReturn" | "Malloc"
          | "ReturnsNonNull" | "WarnUnusedResult" | "Cold" | "Flatten"
          | "Hot" | "Leaf" | "NoIpa" | "NoClone" | "Naked" | "Interrupt"
          | "NoSplitStack" | "ReturnsTwice" | "DllImport" | "DllExport"
          | "WeakImport" | "MsStruct" | "NoMips16" | "TransparentUnion"
          | "GccStruct" | "Common" | "NoCommon" | "Pure" | "Const"
          | "MayAlias" | "MaybeUnused" | "Fallthrough"
            (* expression arguments *)
          | "AddressSpace(" expr ")" | "Aligned(" expr ")"
          | "VectorSize(" expr ")" | "AllocAlign(" expr ")"
          | "ExtVectorType(" expr ")"
          | "AssumeAligned(" vec<expr> ")" | "AllocSize(" vec<expr> ")"
          | PassObjectSize { size_type: expr, dynamic: bool }
          | "AlignAs(" AlignAsOperand ")"
            (* string arguments *)
          | "CodeSeg(" string ")" | "Mode(" string ")"
          | "Visibility(" string ")" | "Section(" string ")"
          | "Annotate(" string ")" | "Target(" string ")"
          | "Alias(" string ")" | "WeakRef(" string ")"
          | "Cleanup(" string ")" | "Ifunc(" string ")"
          | "TlsModel(" string ")" | "ScalarStorageOrder(" string ")"
          | "Optimize(" vec<string> ")" | "CpuDispatch(" vec<string> ")"
          | "CpuSpecific(" vec<string> ")" | "TargetClones(" vec<string> ")"
          | "Availability(" vec<string> ")" | "Format(" vec<string> ")"
          | "FormatArg(" vec<string> ")"
          | "Deprecated(" opt<string> ")" | "NoDiscard(" opt<string> ")"
            (* integer arguments *)
          | "Constructor(" opt<int> ")" | "Destructor(" opt<int> ")"
          | "Sentinel(" opt<int> ")" | "NonNull(" vec<int> ")"
            (* other *)
          | "CallingConvention(" CallingConvention ")"
          | Unknown { name: string, arguments: vec<string> }
          | IgnoredDeclspec { name: string, arguments: vec<string> }
          | Invalid { name: string, arguments: vec<string> } ;

AlignAsOperand    = Type { ty: TypeName } | "Expr(" expr ")" ;
CallingConvention = "Cdecl" | "Stdcall" | "Fastcall" | "Vectorcall"
                  | "Thiscall" | "MsAbi" | "SysVAbi" | "PreserveMost"
                  | "PreserveAll" | "PreserveNone"
                  | "RegParm(" expr ")"
                  | "Pcs(" ( "Aapcs" | "AapcsVfp" ) ")" ;
```

- Every attribute syntax (GNU `__attribute__`, C23 `[[...]]`, keywords such
  as `_Alignas` and `__cdecl`) produces an `Attribute`; which syntax was
  used is not kept.
- `Unknown` is an attribute the parser does not model, or one the flavor
  does not register for the target (`src/attribute_support.rs`), kept by
  name with its argument token spellings. `IgnoredDeclspec` is a
  `__declspec` entry the flavor does not register for the target, which
  clang ignores as "not supported" even when the name is a GNU attribute.
  `Invalid` is a known attribute whose
  arguments did not fit its form.
