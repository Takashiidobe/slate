#ifdef A
#define X
#endif
#ifdef X
int x_defined[1];
#else
int x_undefined[2];
#endif
#if defined(FLAG) && !defined X
int flag_without_x[3];
#endif
#ifndef X
int x_missing[4];
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES FLAG FLAG
// SLATE-FILECHECK-DEFINES X X

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "x_undefined",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 2,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "2",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "x_missing",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 4,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "4",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: decl[{{[0-9]+}}]: Declaration(
// A-NEXT:       Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Integer(
// A-NEXT:                   Ranked {
// A-NEXT:                       rank: Int,
// A-NEXT:                       signed: true,
// A-NEXT:                   },
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclaratorKind {
// A-NEXT:                   declarator: Array {
// A-NEXT:                       inner: Name(
// A-NEXT:                           "x_defined",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntegerLiteral(
// A-NEXT:                               IntegerLiteral {
// A-NEXT:                                   value: 1,
// A-NEXT:                                   radix: Decimal,
// A-NEXT:                                   suffix: IntegerSuffix {
// A-NEXT:                                       unsigned: false,
// A-NEXT:                                       size: None,
// A-NEXT:                                   },
// A-NEXT:                                   spelling: "1",
// A-NEXT:                               },
// A-NEXT:                           ),
// A-NEXT:                       ),
// A-NEXT:                   },
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN FLAG
// FLAG: decl[{{[0-9]+}}]: Declaration(
// FLAG-NEXT:       Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclaratorKind {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "x_undefined",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntegerLiteral(
// FLAG-NEXT:                               IntegerLiteral {
// FLAG-NEXT:                                   value: 2,
// FLAG-NEXT:                                   radix: Decimal,
// FLAG-NEXT:                                   suffix: IntegerSuffix {
// FLAG-NEXT:                                       unsigned: false,
// FLAG-NEXT:                                       size: None,
// FLAG-NEXT:                                   },
// FLAG-NEXT:                                   spelling: "2",
// FLAG-NEXT:                               },
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// FLAG-NEXT: decl[{{[0-9]+}}]: Declaration(
// FLAG-NEXT:       Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclaratorKind {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "flag_without_x",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntegerLiteral(
// FLAG-NEXT:                               IntegerLiteral {
// FLAG-NEXT:                                   value: 3,
// FLAG-NEXT:                                   radix: Decimal,
// FLAG-NEXT:                                   suffix: IntegerSuffix {
// FLAG-NEXT:                                       unsigned: false,
// FLAG-NEXT:                                       size: None,
// FLAG-NEXT:                                   },
// FLAG-NEXT:                                   spelling: "3",
// FLAG-NEXT:                               },
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// FLAG-NEXT: decl[{{[0-9]+}}]: Declaration(
// FLAG-NEXT:       Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclaratorKind {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "x_missing",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntegerLiteral(
// FLAG-NEXT:                               IntegerLiteral {
// FLAG-NEXT:                                   value: 4,
// FLAG-NEXT:                                   radix: Decimal,
// FLAG-NEXT:                                   suffix: IntegerSuffix {
// FLAG-NEXT:                                       unsigned: false,
// FLAG-NEXT:                                       size: None,
// FLAG-NEXT:                                   },
// FLAG-NEXT:                                   spelling: "4",
// FLAG-NEXT:                               },
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// SLATE-FILECHECK-END FLAG
// SLATE-FILECHECK-BEGIN X
// X: decl[{{[0-9]+}}]: Declaration(
// X-NEXT:       Declaration {
// X-NEXT:           specifiers: DeclarationSpecifiers {
// X-NEXT:               ty: Integer(
// X-NEXT:                   Ranked {
// X-NEXT:                       rank: Int,
// X-NEXT:                       signed: true,
// X-NEXT:                   },
// X-NEXT:               ),
// X-NEXT:           },
// X-NEXT:           declarators: [
// X-NEXT:               InitDeclaratorKind {
// X-NEXT:                   declarator: Array {
// X-NEXT:                       inner: Name(
// X-NEXT:                           "x_defined",
// X-NEXT:                       ),
// X-NEXT:                       size: Expression(
// X-NEXT:                           IntegerLiteral(
// X-NEXT:                               IntegerLiteral {
// X-NEXT:                                   value: 1,
// X-NEXT:                                   radix: Decimal,
// X-NEXT:                                   suffix: IntegerSuffix {
// X-NEXT:                                       unsigned: false,
// X-NEXT:                                       size: None,
// X-NEXT:                                   },
// X-NEXT:                                   spelling: "1",
// X-NEXT:                               },
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   },
// X-NEXT:               },
// X-NEXT:           ],
// X-NEXT:       },
// X-NEXT:   )
// SLATE-FILECHECK-END X
