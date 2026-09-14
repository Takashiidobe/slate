#define WIDTH 4
#pragma push_macro("WIDTH")
#undef WIDTH
#define WIDTH 8
int inner[WIDTH];
#pragma pop_macro("WIDTH")
int outer[WIDTH];
#pragma push_macro("FRESH")
#define FRESH 1
#pragma pop_macro("FRESH")
#ifdef FRESH
int fresh;
#endif
#pragma pop_macro("WIDTH")
int unmatched_pop[WIDTH];
#ifdef WIDE
#pragma push_macro("WIDTH")
#define WIDTH 16
int wide[WIDTH];
#pragma pop_macro("WIDTH")
#endif
int after[WIDTH];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIDE WIDE

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
// DEFAULT-NEXT:                           "inner",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 8,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "8",
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
// DEFAULT-NEXT:                           "outer",
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
// DEFAULT-NEXT: decl[2]: Declaration(
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
// DEFAULT-NEXT:                           "unmatched_pop",
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
// DEFAULT-NEXT: decl[3]: Declaration(
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
// DEFAULT-NEXT:                           "after",
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
// SLATE-FILECHECK-BEGIN WIDE
// WIDE: decl[0]: Declaration(
// WIDE-NEXT:       Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarators: [
// WIDE-NEXT:               InitDeclaratorKind {
// WIDE-NEXT:                   declarator: Array {
// WIDE-NEXT:                       inner: Name(
// WIDE-NEXT:                           "inner",
// WIDE-NEXT:                       ),
// WIDE-NEXT:                       size: Expression(
// WIDE-NEXT:                           IntegerLiteral(
// WIDE-NEXT:                               IntegerLiteral {
// WIDE-NEXT:                                   value: 8,
// WIDE-NEXT:                                   radix: Decimal,
// WIDE-NEXT:                                   suffix: IntegerSuffix {
// WIDE-NEXT:                                       unsigned: false,
// WIDE-NEXT:                                       size: None,
// WIDE-NEXT:                                   },
// WIDE-NEXT:                                   spelling: "8",
// WIDE-NEXT:                               },
// WIDE-NEXT:                           ),
// WIDE-NEXT:                       ),
// WIDE-NEXT:                   },
// WIDE-NEXT:               },
// WIDE-NEXT:           ],
// WIDE-NEXT:       },
// WIDE-NEXT:   )
// WIDE-NEXT: decl[1]: Declaration(
// WIDE-NEXT:       Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarators: [
// WIDE-NEXT:               InitDeclaratorKind {
// WIDE-NEXT:                   declarator: Array {
// WIDE-NEXT:                       inner: Name(
// WIDE-NEXT:                           "outer",
// WIDE-NEXT:                       ),
// WIDE-NEXT:                       size: Expression(
// WIDE-NEXT:                           IntegerLiteral(
// WIDE-NEXT:                               IntegerLiteral {
// WIDE-NEXT:                                   value: 4,
// WIDE-NEXT:                                   radix: Decimal,
// WIDE-NEXT:                                   suffix: IntegerSuffix {
// WIDE-NEXT:                                       unsigned: false,
// WIDE-NEXT:                                       size: None,
// WIDE-NEXT:                                   },
// WIDE-NEXT:                                   spelling: "4",
// WIDE-NEXT:                               },
// WIDE-NEXT:                           ),
// WIDE-NEXT:                       ),
// WIDE-NEXT:                   },
// WIDE-NEXT:               },
// WIDE-NEXT:           ],
// WIDE-NEXT:       },
// WIDE-NEXT:   )
// WIDE-NEXT: decl[2]: Declaration(
// WIDE-NEXT:       Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarators: [
// WIDE-NEXT:               InitDeclaratorKind {
// WIDE-NEXT:                   declarator: Array {
// WIDE-NEXT:                       inner: Name(
// WIDE-NEXT:                           "unmatched_pop",
// WIDE-NEXT:                       ),
// WIDE-NEXT:                       size: Expression(
// WIDE-NEXT:                           IntegerLiteral(
// WIDE-NEXT:                               IntegerLiteral {
// WIDE-NEXT:                                   value: 4,
// WIDE-NEXT:                                   radix: Decimal,
// WIDE-NEXT:                                   suffix: IntegerSuffix {
// WIDE-NEXT:                                       unsigned: false,
// WIDE-NEXT:                                       size: None,
// WIDE-NEXT:                                   },
// WIDE-NEXT:                                   spelling: "4",
// WIDE-NEXT:                               },
// WIDE-NEXT:                           ),
// WIDE-NEXT:                       ),
// WIDE-NEXT:                   },
// WIDE-NEXT:               },
// WIDE-NEXT:           ],
// WIDE-NEXT:       },
// WIDE-NEXT:   )
// WIDE-NEXT: decl[3]: Declaration(
// WIDE-NEXT:       Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarators: [
// WIDE-NEXT:               InitDeclaratorKind {
// WIDE-NEXT:                   declarator: Array {
// WIDE-NEXT:                       inner: Name(
// WIDE-NEXT:                           "wide",
// WIDE-NEXT:                       ),
// WIDE-NEXT:                       size: Expression(
// WIDE-NEXT:                           IntegerLiteral(
// WIDE-NEXT:                               IntegerLiteral {
// WIDE-NEXT:                                   value: 16,
// WIDE-NEXT:                                   radix: Decimal,
// WIDE-NEXT:                                   suffix: IntegerSuffix {
// WIDE-NEXT:                                       unsigned: false,
// WIDE-NEXT:                                       size: None,
// WIDE-NEXT:                                   },
// WIDE-NEXT:                                   spelling: "16",
// WIDE-NEXT:                               },
// WIDE-NEXT:                           ),
// WIDE-NEXT:                       ),
// WIDE-NEXT:                   },
// WIDE-NEXT:               },
// WIDE-NEXT:           ],
// WIDE-NEXT:       },
// WIDE-NEXT:   )
// WIDE-NEXT: decl[4]: Declaration(
// WIDE-NEXT:       Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarators: [
// WIDE-NEXT:               InitDeclaratorKind {
// WIDE-NEXT:                   declarator: Array {
// WIDE-NEXT:                       inner: Name(
// WIDE-NEXT:                           "after",
// WIDE-NEXT:                       ),
// WIDE-NEXT:                       size: Expression(
// WIDE-NEXT:                           IntegerLiteral(
// WIDE-NEXT:                               IntegerLiteral {
// WIDE-NEXT:                                   value: 4,
// WIDE-NEXT:                                   radix: Decimal,
// WIDE-NEXT:                                   suffix: IntegerSuffix {
// WIDE-NEXT:                                       unsigned: false,
// WIDE-NEXT:                                       size: None,
// WIDE-NEXT:                                   },
// WIDE-NEXT:                                   spelling: "4",
// WIDE-NEXT:                               },
// WIDE-NEXT:                           ),
// WIDE-NEXT:                       ),
// WIDE-NEXT:                   },
// WIDE-NEXT:               },
// WIDE-NEXT:           ],
// WIDE-NEXT:       },
// WIDE-NEXT:   )
// SLATE-FILECHECK-END WIDE
