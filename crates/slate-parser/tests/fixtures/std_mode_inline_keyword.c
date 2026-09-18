#ifdef INLINE_IS_KEYWORD
inline int inline_function(void) { return 0; }
#else
int inline;
#endif
__inline int always_inline_function(void) { return 1; }

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89 INLINE_IS_KEYWORD
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C99 INLINE_IS_KEYWORD
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C23 INLINE_IS_KEYWORD
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "inline",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:               is_inline: true,
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "always_inline_function",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Void,
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               Return(
// C89-NEXT:                   IntegerLiteral(
// C89-NEXT:                       IntegerLiteral {
// C89-NEXT:                           value: 1,
// C89-NEXT:                           radix: Decimal,
// C89-NEXT:                           suffix: IntegerSuffix {
// C89-NEXT:                               unsigned: false,
// C89-NEXT:                               size: None,
// C89-NEXT:                           },
// C89-NEXT:                           spelling: "1",
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: decl[{{[0-9]+}}]: Function(
// GNU89-NEXT:       FunctionDefinition {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:               is_inline: true,
// GNU89-NEXT:           },
// GNU89-NEXT:           declarator: Function {
// GNU89-NEXT:               inner: Name(
// GNU89-NEXT:                   "inline_function",
// GNU89-NEXT:               ),
// GNU89-NEXT:               parameters: Void,
// GNU89-NEXT:           },
// GNU89-NEXT:           body: [
// GNU89-NEXT:               Return(
// GNU89-NEXT:                   IntegerLiteral(
// GNU89-NEXT:                       IntegerLiteral {
// GNU89-NEXT:                           value: 0,
// GNU89-NEXT:                           radix: Decimal,
// GNU89-NEXT:                           suffix: IntegerSuffix {
// GNU89-NEXT:                               unsigned: false,
// GNU89-NEXT:                               size: None,
// GNU89-NEXT:                           },
// GNU89-NEXT:                           spelling: "0",
// GNU89-NEXT:                       },
// GNU89-NEXT:                   ),
// GNU89-NEXT:               ),
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Function(
// GNU89-NEXT:       FunctionDefinition {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:               is_inline: true,
// GNU89-NEXT:           },
// GNU89-NEXT:           declarator: Function {
// GNU89-NEXT:               inner: Name(
// GNU89-NEXT:                   "always_inline_function",
// GNU89-NEXT:               ),
// GNU89-NEXT:               parameters: Void,
// GNU89-NEXT:           },
// GNU89-NEXT:           body: [
// GNU89-NEXT:               Return(
// GNU89-NEXT:                   IntegerLiteral(
// GNU89-NEXT:                       IntegerLiteral {
// GNU89-NEXT:                           value: 1,
// GNU89-NEXT:                           radix: Decimal,
// GNU89-NEXT:                           suffix: IntegerSuffix {
// GNU89-NEXT:                               unsigned: false,
// GNU89-NEXT:                               size: None,
// GNU89-NEXT:                           },
// GNU89-NEXT:                           spelling: "1",
// GNU89-NEXT:                       },
// GNU89-NEXT:                   ),
// GNU89-NEXT:               ),
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// SLATE-FILECHECK-END GNU89
// SLATE-FILECHECK-BEGIN C99
// C99: decl[{{[0-9]+}}]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:               is_inline: true,
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "inline_function",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               Return(
// C99-NEXT:                   IntegerLiteral(
// C99-NEXT:                       IntegerLiteral {
// C99-NEXT:                           value: 0,
// C99-NEXT:                           radix: Decimal,
// C99-NEXT:                           suffix: IntegerSuffix {
// C99-NEXT:                               unsigned: false,
// C99-NEXT:                               size: None,
// C99-NEXT:                           },
// C99-NEXT:                           spelling: "0",
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[{{[0-9]+}}]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:               is_inline: true,
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "always_inline_function",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               Return(
// C99-NEXT:                   IntegerLiteral(
// C99-NEXT:                       IntegerLiteral {
// C99-NEXT:                           value: 1,
// C99-NEXT:                           radix: Decimal,
// C99-NEXT:                           suffix: IntegerSuffix {
// C99-NEXT:                               unsigned: false,
// C99-NEXT:                               size: None,
// C99-NEXT:                           },
// C99-NEXT:                           spelling: "1",
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C23
// C23: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               is_inline: true,
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "inline_function",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Void,
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Return(
// C23-NEXT:                   IntegerLiteral(
// C23-NEXT:                       IntegerLiteral {
// C23-NEXT:                           value: 0,
// C23-NEXT:                           radix: Decimal,
// C23-NEXT:                           suffix: IntegerSuffix {
// C23-NEXT:                               unsigned: false,
// C23-NEXT:                               size: None,
// C23-NEXT:                           },
// C23-NEXT:                           spelling: "0",
// C23-NEXT:                       },
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               is_inline: true,
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "always_inline_function",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Void,
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Return(
// C23-NEXT:                   IntegerLiteral(
// C23-NEXT:                       IntegerLiteral {
// C23-NEXT:                           value: 1,
// C23-NEXT:                           radix: Decimal,
// C23-NEXT:                           suffix: IntegerSuffix {
// C23-NEXT:                               unsigned: false,
// C23-NEXT:                               size: None,
// C23-NEXT:                           },
// C23-NEXT:                           spelling: "1",
// C23-NEXT:                       },
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
