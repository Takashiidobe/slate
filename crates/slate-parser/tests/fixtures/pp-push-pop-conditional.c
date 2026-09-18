#define X 1
#pragma push_macro("X")
#undef X
#define X 2
#ifdef A
#pragma pop_macro("X")
#endif
int popped_in_branch[X];
#define Y 1
#ifdef A
#pragma push_macro("Y")
#endif
#undef Y
#define Y 2
#ifndef A
#pragma pop_macro("Y")
#endif
int unreachable_pop[Y];
#ifdef A
#pragma push_macro("Y")
#undef Y
#define Y 3
#endif
#pragma pop_macro("Y")
int partial_push[Y];
#pragma pop_macro("Y")
int second_pop[Y];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[{{[0-9]+}}]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Opaque(
// DEFAULT-NEXT:               "push_macro ( \"X\" )",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:                           "popped_in_branch",
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
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Opaque(
// DEFAULT-NEXT:               "pop_macro ( \"Y\" )",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:                           "unreachable_pop",
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
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Opaque(
// DEFAULT-NEXT:               "pop_macro ( \"Y\" )",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:                           "partial_push",
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
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Opaque(
// DEFAULT-NEXT:               "pop_macro ( \"Y\" )",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:                           "second_pop",
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
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "push_macro ( \"X\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "pop_macro ( \"X\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// A-NEXT:                           "popped_in_branch",
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
// A-NEXT: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "push_macro ( \"Y\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// A-NEXT:                           "unreachable_pop",
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
// A-NEXT: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "push_macro ( \"Y\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "pop_macro ( \"Y\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// A-NEXT:                           "partial_push",
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
// A-NEXT: decl[{{[0-9]+}}]: Pragma(
// A-NEXT:       Pragma {
// A-NEXT:           kind: Opaque(
// A-NEXT:               "pop_macro ( \"Y\" )",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// A-NEXT:                           "second_pop",
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
