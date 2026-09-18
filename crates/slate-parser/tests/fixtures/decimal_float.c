typedef _Decimal64 money;

_Decimal32 small = 1.5DF;
_Decimal64 medium = 2.5dd;
_Decimal128 large = 3.5DL;
money total;

struct ledger {
  _Decimal32 fee;
  _Decimal128 balance;
};

_Decimal64 scale(_Decimal64 value, _Decimal32 factor);

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: tag[{{[0-9]+}}]: TagDefinition {
// C89-NEXT:       id: TagId(
// C89-NEXT:           [[#TAG0:]],
// C89-NEXT:       ),
// C89-NEXT:       kind: Struct,
// C89-NEXT:       name: Some(
// C89-NEXT:           "ledger",
// C89-NEXT:       ),
// C89-NEXT:       body: Record(
// C89-NEXT:           [
// C89-NEXT:               Field(
// C89-NEXT:                   FieldDecl {
// C89-NEXT:                       specifiers: DeclarationSpecifiers {
// C89-NEXT:                           ty: Floating(
// C89-NEXT:                               Decimal32,
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                       declarators: [
// C89-NEXT:                           FieldDeclaratorKind {
// C89-NEXT:                               declarator: Name(
// C89-NEXT:                                   "fee",
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                       ],
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:               Field(
// C89-NEXT:                   FieldDecl {
// C89-NEXT:                       specifiers: DeclarationSpecifiers {
// C89-NEXT:                           ty: Floating(
// C89-NEXT:                               Decimal128,
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                       declarators: [
// C89-NEXT:                           FieldDeclaratorKind {
// C89-NEXT:                               declarator: Name(
// C89-NEXT:                                   "balance",
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                       ],
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       ),
// C89-NEXT:   }
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Floating(
// C89-NEXT:                   Decimal64,
// C89-NEXT:               ),
// C89-NEXT:               storage: Typedef,
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "money",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Floating(
// C89-NEXT:                   Decimal32,
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "small",
// C89-NEXT:                   ),
// C89-NEXT:                   initializer: Some(
// C89-NEXT:                       Expr(
// C89-NEXT:                           FloatLiteral(
// C89-NEXT:                               FloatLiteral {
// C89-NEXT:                                   spelling: "1.5DF",
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: DecimalF32,
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Floating(
// C89-NEXT:                   Decimal64,
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "medium",
// C89-NEXT:                   ),
// C89-NEXT:                   initializer: Some(
// C89-NEXT:                       Expr(
// C89-NEXT:                           FloatLiteral(
// C89-NEXT:                               FloatLiteral {
// C89-NEXT:                                   spelling: "2.5dd",
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: DecimalF64,
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Floating(
// C89-NEXT:                   Decimal128,
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "large",
// C89-NEXT:                   ),
// C89-NEXT:                   initializer: Some(
// C89-NEXT:                       Expr(
// C89-NEXT:                           FloatLiteral(
// C89-NEXT:                               FloatLiteral {
// C89-NEXT:                                   spelling: "3.5DL",
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: DecimalF128,
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Named(
// C89-NEXT:                   "money",
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "total",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Tag(
// C89-NEXT:                   Definition(
// C89-NEXT:                       TagId(
// C89-NEXT:                           [[#TAG0]],
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Floating(
// C89-NEXT:                   Decimal64,
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Function {
// C89-NEXT:                       inner: Name(
// C89-NEXT:                           "scale",
// C89-NEXT:                       ),
// C89-NEXT:                       parameters: Prototype {
// C89-NEXT:                           parameters: [
// C89-NEXT:                               ParameterDeclarationKind {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Floating(
// C89-NEXT:                                           Decimal64,
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                                   declarator: Name(
// C89-NEXT:                                       "value",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                               ParameterDeclarationKind {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Floating(
// C89-NEXT:                                           Decimal32,
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                                   declarator: Name(
// C89-NEXT:                                       "factor",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                           ],
// C89-NEXT:                       },
// C89-NEXT:                   },
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C23
// C23: tag[{{[0-9]+}}]: TagDefinition {
// C23-NEXT:       id: TagId(
// C23-NEXT:           [[#TAG0:]],
// C23-NEXT:       ),
// C23-NEXT:       kind: Struct,
// C23-NEXT:       name: Some(
// C23-NEXT:           "ledger",
// C23-NEXT:       ),
// C23-NEXT:       body: Record(
// C23-NEXT:           [
// C23-NEXT:               Field(
// C23-NEXT:                   FieldDecl {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Floating(
// C23-NEXT:                               Decimal32,
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           FieldDeclaratorKind {
// C23-NEXT:                               declarator: Name(
// C23-NEXT:                                   "fee",
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Field(
// C23-NEXT:                   FieldDecl {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Floating(
// C23-NEXT:                               Decimal128,
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           FieldDeclaratorKind {
// C23-NEXT:                               declarator: Name(
// C23-NEXT:                                   "balance",
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       ),
// C23-NEXT:   }
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Floating(
// C23-NEXT:                   Decimal64,
// C23-NEXT:               ),
// C23-NEXT:               storage: Typedef,
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "money",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Floating(
// C23-NEXT:                   Decimal32,
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "small",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           FloatLiteral(
// C23-NEXT:                               FloatLiteral {
// C23-NEXT:                                   spelling: "1.5DF",
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: DecimalF32,
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Floating(
// C23-NEXT:                   Decimal64,
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "medium",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           FloatLiteral(
// C23-NEXT:                               FloatLiteral {
// C23-NEXT:                                   spelling: "2.5dd",
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: DecimalF64,
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Floating(
// C23-NEXT:                   Decimal128,
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "large",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           FloatLiteral(
// C23-NEXT:                               FloatLiteral {
// C23-NEXT:                                   spelling: "3.5DL",
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: DecimalF128,
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Named(
// C23-NEXT:                   "money",
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "total",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Tag(
// C23-NEXT:                   Definition(
// C23-NEXT:                       TagId(
// C23-NEXT:                           [[#TAG0]],
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Floating(
// C23-NEXT:                   Decimal64,
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "scale",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Prototype {
// C23-NEXT:                           parameters: [
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Floating(
// C23-NEXT:                                           Decimal64,
// C23-NEXT:                                       ),
// C23-NEXT:                                   },
// C23-NEXT:                                   declarator: Name(
// C23-NEXT:                                       "value",
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Floating(
// C23-NEXT:                                           Decimal32,
// C23-NEXT:                                       ),
// C23-NEXT:                                   },
// C23-NEXT:                                   declarator: Name(
// C23-NEXT:                                       "factor",
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
