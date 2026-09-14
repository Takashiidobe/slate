#define X 1
#ifdef A
#undef X
#define X 2
#endif
int redefined[X];
#ifdef A
#ifdef B
#undef X
#define X 5
#endif
#endif
int nested[X];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES B B
// SLATE-FILECHECK-DEFINES AB A B

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration(
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
// DEFAULT-NEXT:                           "redefined",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
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
// DEFAULT-NEXT:                           "nested",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
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
// A: decl[0]: Declaration(
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
// A-NEXT:                           "redefined",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntegerLiteral(
// A-NEXT:                               IntegerLiteral {
// A-NEXT:                                   value: 2,
// A-NEXT:                                   radix: Decimal,
// A-NEXT:                                   suffix: IntegerSuffix {
// A-NEXT:                                       unsigned: false,
// A-NEXT:                                       size: None,
// A-NEXT:                                   },
// A-NEXT:                                   spelling: "2",
// A-NEXT:                               },
// A-NEXT:                           ),
// A-NEXT:                       ),
// A-NEXT:                   },
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[1]: Declaration(
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
// A-NEXT:                           "nested",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntegerLiteral(
// A-NEXT:                               IntegerLiteral {
// A-NEXT:                                   value: 2,
// A-NEXT:                                   radix: Decimal,
// A-NEXT:                                   suffix: IntegerSuffix {
// A-NEXT:                                       unsigned: false,
// A-NEXT:                                       size: None,
// A-NEXT:                                   },
// A-NEXT:                                   spelling: "2",
// A-NEXT:                               },
// A-NEXT:                           ),
// A-NEXT:                       ),
// A-NEXT:                   },
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN B
// B: decl[0]: Declaration(
// B-NEXT:       Declaration {
// B-NEXT:           specifiers: DeclarationSpecifiers {
// B-NEXT:               ty: Integer(
// B-NEXT:                   Ranked {
// B-NEXT:                       rank: Int,
// B-NEXT:                       signed: true,
// B-NEXT:                   },
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:           declarators: [
// B-NEXT:               InitDeclaratorKind {
// B-NEXT:                   declarator: Array {
// B-NEXT:                       inner: Name(
// B-NEXT:                           "redefined",
// B-NEXT:                       ),
// B-NEXT:                       size: Expression(
// B-NEXT:                           IntegerLiteral(
// B-NEXT:                               IntegerLiteral {
// B-NEXT:                                   value: 1,
// B-NEXT:                                   radix: Decimal,
// B-NEXT:                                   suffix: IntegerSuffix {
// B-NEXT:                                       unsigned: false,
// B-NEXT:                                       size: None,
// B-NEXT:                                   },
// B-NEXT:                                   spelling: "1",
// B-NEXT:                               },
// B-NEXT:                           ),
// B-NEXT:                       ),
// B-NEXT:                   },
// B-NEXT:               },
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:   )
// B-NEXT: decl[1]: Declaration(
// B-NEXT:       Declaration {
// B-NEXT:           specifiers: DeclarationSpecifiers {
// B-NEXT:               ty: Integer(
// B-NEXT:                   Ranked {
// B-NEXT:                       rank: Int,
// B-NEXT:                       signed: true,
// B-NEXT:                   },
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:           declarators: [
// B-NEXT:               InitDeclaratorKind {
// B-NEXT:                   declarator: Array {
// B-NEXT:                       inner: Name(
// B-NEXT:                           "nested",
// B-NEXT:                       ),
// B-NEXT:                       size: Expression(
// B-NEXT:                           IntegerLiteral(
// B-NEXT:                               IntegerLiteral {
// B-NEXT:                                   value: 1,
// B-NEXT:                                   radix: Decimal,
// B-NEXT:                                   suffix: IntegerSuffix {
// B-NEXT:                                       unsigned: false,
// B-NEXT:                                       size: None,
// B-NEXT:                                   },
// B-NEXT:                                   spelling: "1",
// B-NEXT:                               },
// B-NEXT:                           ),
// B-NEXT:                       ),
// B-NEXT:                   },
// B-NEXT:               },
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:   )
// SLATE-FILECHECK-END B
// SLATE-FILECHECK-BEGIN AB
// AB: decl[0]: Declaration(
// AB-NEXT:       Declaration {
// AB-NEXT:           specifiers: DeclarationSpecifiers {
// AB-NEXT:               ty: Integer(
// AB-NEXT:                   Ranked {
// AB-NEXT:                       rank: Int,
// AB-NEXT:                       signed: true,
// AB-NEXT:                   },
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:           declarators: [
// AB-NEXT:               InitDeclaratorKind {
// AB-NEXT:                   declarator: Array {
// AB-NEXT:                       inner: Name(
// AB-NEXT:                           "redefined",
// AB-NEXT:                       ),
// AB-NEXT:                       size: Expression(
// AB-NEXT:                           IntegerLiteral(
// AB-NEXT:                               IntegerLiteral {
// AB-NEXT:                                   value: 2,
// AB-NEXT:                                   radix: Decimal,
// AB-NEXT:                                   suffix: IntegerSuffix {
// AB-NEXT:                                       unsigned: false,
// AB-NEXT:                                       size: None,
// AB-NEXT:                                   },
// AB-NEXT:                                   spelling: "2",
// AB-NEXT:                               },
// AB-NEXT:                           ),
// AB-NEXT:                       ),
// AB-NEXT:                   },
// AB-NEXT:               },
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:   )
// AB-NEXT: decl[1]: Declaration(
// AB-NEXT:       Declaration {
// AB-NEXT:           specifiers: DeclarationSpecifiers {
// AB-NEXT:               ty: Integer(
// AB-NEXT:                   Ranked {
// AB-NEXT:                       rank: Int,
// AB-NEXT:                       signed: true,
// AB-NEXT:                   },
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:           declarators: [
// AB-NEXT:               InitDeclaratorKind {
// AB-NEXT:                   declarator: Array {
// AB-NEXT:                       inner: Name(
// AB-NEXT:                           "nested",
// AB-NEXT:                       ),
// AB-NEXT:                       size: Expression(
// AB-NEXT:                           IntegerLiteral(
// AB-NEXT:                               IntegerLiteral {
// AB-NEXT:                                   value: 5,
// AB-NEXT:                                   radix: Decimal,
// AB-NEXT:                                   suffix: IntegerSuffix {
// AB-NEXT:                                       unsigned: false,
// AB-NEXT:                                       size: None,
// AB-NEXT:                                   },
// AB-NEXT:                                   spelling: "5",
// AB-NEXT:                               },
// AB-NEXT:                           ),
// AB-NEXT:                       ),
// AB-NEXT:                   },
// AB-NEXT:               },
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:   )
// SLATE-FILECHECK-END AB
