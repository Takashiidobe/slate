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
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "redefined",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 5,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "A",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "redefined",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 5,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "nested",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 12,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   And(
// DEFAULT-NEXT:                       Not(
// DEFAULT-NEXT:                           And(
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "A",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "B",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "nested",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 12,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   And(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "B",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "nested",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 12,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "redefined",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       1,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "nested",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       1,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: polyvariant:
// A-NEXT: decl[0]: Conditional(
// A-NEXT:       Conditional {
// A-NEXT:           branches: [
// A-NEXT:               (
// A-NEXT:                   Not(
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Declaration {
// A-NEXT:                           declaration: Declaration {
// A-NEXT:                               specifiers: DeclarationSpecifiers {
// A-NEXT:                                   ty: Integer(
// A-NEXT:                                       Ranked {
// A-NEXT:                                           rank: Int,
// A-NEXT:                                           signed: true,
// A-NEXT:                                       },
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                               declarator: Array {
// A-NEXT:                                   inner: Name(
// A-NEXT:                                       "redefined",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           1,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 5,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:               (
// A-NEXT:                   Defined(
// A-NEXT:                       "A",
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Declaration {
// A-NEXT:                           declaration: Declaration {
// A-NEXT:                               specifiers: DeclarationSpecifiers {
// A-NEXT:                                   ty: Integer(
// A-NEXT:                                       Ranked {
// A-NEXT:                                           rank: Int,
// A-NEXT:                                           signed: true,
// A-NEXT:                                       },
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                               declarator: Array {
// A-NEXT:                                   inner: Name(
// A-NEXT:                                       "redefined",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           2,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 5,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[1]: Conditional(
// A-NEXT:       Conditional {
// A-NEXT:           branches: [
// A-NEXT:               (
// A-NEXT:                   Not(
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Declaration {
// A-NEXT:                           declaration: Declaration {
// A-NEXT:                               specifiers: DeclarationSpecifiers {
// A-NEXT:                                   ty: Integer(
// A-NEXT:                                       Ranked {
// A-NEXT:                                           rank: Int,
// A-NEXT:                                           signed: true,
// A-NEXT:                                       },
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                               declarator: Array {
// A-NEXT:                                   inner: Name(
// A-NEXT:                                       "nested",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           1,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 12,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:               (
// A-NEXT:                   And(
// A-NEXT:                       Not(
// A-NEXT:                           And(
// A-NEXT:                               Defined(
// A-NEXT:                                   "A",
// A-NEXT:                               ),
// A-NEXT:                               Defined(
// A-NEXT:                                   "B",
// A-NEXT:                               ),
// A-NEXT:                           ),
// A-NEXT:                       ),
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Declaration {
// A-NEXT:                           declaration: Declaration {
// A-NEXT:                               specifiers: DeclarationSpecifiers {
// A-NEXT:                                   ty: Integer(
// A-NEXT:                                       Ranked {
// A-NEXT:                                           rank: Int,
// A-NEXT:                                           signed: true,
// A-NEXT:                                       },
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                               declarator: Array {
// A-NEXT:                                   inner: Name(
// A-NEXT:                                       "nested",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           2,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 12,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:               (
// A-NEXT:                   And(
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                       Defined(
// A-NEXT:                           "B",
// A-NEXT:                       ),
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Declaration {
// A-NEXT:                           declaration: Declaration {
// A-NEXT:                               specifiers: DeclarationSpecifiers {
// A-NEXT:                                   ty: Integer(
// A-NEXT:                                       Ranked {
// A-NEXT:                                           rank: Int,
// A-NEXT:                                           signed: true,
// A-NEXT:                                       },
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                               declarator: Array {
// A-NEXT:                                   inner: Name(
// A-NEXT:                                       "nested",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           5,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 12,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: concrete:
// A-NEXT: decl[0]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Integer(
// A-NEXT:                   Ranked {
// A-NEXT:                       rank: Int,
// A-NEXT:                       signed: true,
// A-NEXT:                   },
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Array {
// A-NEXT:               inner: Name(
// A-NEXT:                   "redefined",
// A-NEXT:               ),
// A-NEXT:               size: Expression(
// A-NEXT:                   IntLit(
// A-NEXT:                       2,
// A-NEXT:                   ),
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 5,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[1]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Integer(
// A-NEXT:                   Ranked {
// A-NEXT:                       rank: Int,
// A-NEXT:                       signed: true,
// A-NEXT:                   },
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Array {
// A-NEXT:               inner: Name(
// A-NEXT:                   "nested",
// A-NEXT:               ),
// A-NEXT:               size: Expression(
// A-NEXT:                   IntLit(
// A-NEXT:                       2,
// A-NEXT:                   ),
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 12,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN B
// B: polyvariant:
// B-NEXT: decl[0]: Conditional(
// B-NEXT:       Conditional {
// B-NEXT:           branches: [
// B-NEXT:               (
// B-NEXT:                   Not(
// B-NEXT:                       Defined(
// B-NEXT:                           "A",
// B-NEXT:                       ),
// B-NEXT:                   ),
// B-NEXT:                   [
// B-NEXT:                       Declaration {
// B-NEXT:                           declaration: Declaration {
// B-NEXT:                               specifiers: DeclarationSpecifiers {
// B-NEXT:                                   ty: Integer(
// B-NEXT:                                       Ranked {
// B-NEXT:                                           rank: Int,
// B-NEXT:                                           signed: true,
// B-NEXT:                                       },
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                               declarator: Array {
// B-NEXT:                                   inner: Name(
// B-NEXT:                                       "redefined",
// B-NEXT:                                   ),
// B-NEXT:                                   size: Expression(
// B-NEXT:                                       IntLit(
// B-NEXT:                                           1,
// B-NEXT:                                       ),
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                           },
// B-NEXT:                           provenance: Provenance {
// B-NEXT:                               file: FileId(
// B-NEXT:                                   2,
// B-NEXT:                               ),
// B-NEXT:                               kind: User,
// B-NEXT:                               line: 5,
// B-NEXT:                           },
// B-NEXT:                       },
// B-NEXT:                   ],
// B-NEXT:               ),
// B-NEXT:               (
// B-NEXT:                   Defined(
// B-NEXT:                       "A",
// B-NEXT:                   ),
// B-NEXT:                   [
// B-NEXT:                       Declaration {
// B-NEXT:                           declaration: Declaration {
// B-NEXT:                               specifiers: DeclarationSpecifiers {
// B-NEXT:                                   ty: Integer(
// B-NEXT:                                       Ranked {
// B-NEXT:                                           rank: Int,
// B-NEXT:                                           signed: true,
// B-NEXT:                                       },
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                               declarator: Array {
// B-NEXT:                                   inner: Name(
// B-NEXT:                                       "redefined",
// B-NEXT:                                   ),
// B-NEXT:                                   size: Expression(
// B-NEXT:                                       IntLit(
// B-NEXT:                                           2,
// B-NEXT:                                       ),
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                           },
// B-NEXT:                           provenance: Provenance {
// B-NEXT:                               file: FileId(
// B-NEXT:                                   2,
// B-NEXT:                               ),
// B-NEXT:                               kind: User,
// B-NEXT:                               line: 5,
// B-NEXT:                           },
// B-NEXT:                       },
// B-NEXT:                   ],
// B-NEXT:               ),
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:   )
// B-NEXT: decl[1]: Conditional(
// B-NEXT:       Conditional {
// B-NEXT:           branches: [
// B-NEXT:               (
// B-NEXT:                   Not(
// B-NEXT:                       Defined(
// B-NEXT:                           "A",
// B-NEXT:                       ),
// B-NEXT:                   ),
// B-NEXT:                   [
// B-NEXT:                       Declaration {
// B-NEXT:                           declaration: Declaration {
// B-NEXT:                               specifiers: DeclarationSpecifiers {
// B-NEXT:                                   ty: Integer(
// B-NEXT:                                       Ranked {
// B-NEXT:                                           rank: Int,
// B-NEXT:                                           signed: true,
// B-NEXT:                                       },
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                               declarator: Array {
// B-NEXT:                                   inner: Name(
// B-NEXT:                                       "nested",
// B-NEXT:                                   ),
// B-NEXT:                                   size: Expression(
// B-NEXT:                                       IntLit(
// B-NEXT:                                           1,
// B-NEXT:                                       ),
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                           },
// B-NEXT:                           provenance: Provenance {
// B-NEXT:                               file: FileId(
// B-NEXT:                                   2,
// B-NEXT:                               ),
// B-NEXT:                               kind: User,
// B-NEXT:                               line: 12,
// B-NEXT:                           },
// B-NEXT:                       },
// B-NEXT:                   ],
// B-NEXT:               ),
// B-NEXT:               (
// B-NEXT:                   And(
// B-NEXT:                       Not(
// B-NEXT:                           And(
// B-NEXT:                               Defined(
// B-NEXT:                                   "A",
// B-NEXT:                               ),
// B-NEXT:                               Defined(
// B-NEXT:                                   "B",
// B-NEXT:                               ),
// B-NEXT:                           ),
// B-NEXT:                       ),
// B-NEXT:                       Defined(
// B-NEXT:                           "A",
// B-NEXT:                       ),
// B-NEXT:                   ),
// B-NEXT:                   [
// B-NEXT:                       Declaration {
// B-NEXT:                           declaration: Declaration {
// B-NEXT:                               specifiers: DeclarationSpecifiers {
// B-NEXT:                                   ty: Integer(
// B-NEXT:                                       Ranked {
// B-NEXT:                                           rank: Int,
// B-NEXT:                                           signed: true,
// B-NEXT:                                       },
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                               declarator: Array {
// B-NEXT:                                   inner: Name(
// B-NEXT:                                       "nested",
// B-NEXT:                                   ),
// B-NEXT:                                   size: Expression(
// B-NEXT:                                       IntLit(
// B-NEXT:                                           2,
// B-NEXT:                                       ),
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                           },
// B-NEXT:                           provenance: Provenance {
// B-NEXT:                               file: FileId(
// B-NEXT:                                   2,
// B-NEXT:                               ),
// B-NEXT:                               kind: User,
// B-NEXT:                               line: 12,
// B-NEXT:                           },
// B-NEXT:                       },
// B-NEXT:                   ],
// B-NEXT:               ),
// B-NEXT:               (
// B-NEXT:                   And(
// B-NEXT:                       Defined(
// B-NEXT:                           "A",
// B-NEXT:                       ),
// B-NEXT:                       Defined(
// B-NEXT:                           "B",
// B-NEXT:                       ),
// B-NEXT:                   ),
// B-NEXT:                   [
// B-NEXT:                       Declaration {
// B-NEXT:                           declaration: Declaration {
// B-NEXT:                               specifiers: DeclarationSpecifiers {
// B-NEXT:                                   ty: Integer(
// B-NEXT:                                       Ranked {
// B-NEXT:                                           rank: Int,
// B-NEXT:                                           signed: true,
// B-NEXT:                                       },
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                               declarator: Array {
// B-NEXT:                                   inner: Name(
// B-NEXT:                                       "nested",
// B-NEXT:                                   ),
// B-NEXT:                                   size: Expression(
// B-NEXT:                                       IntLit(
// B-NEXT:                                           5,
// B-NEXT:                                       ),
// B-NEXT:                                   ),
// B-NEXT:                               },
// B-NEXT:                           },
// B-NEXT:                           provenance: Provenance {
// B-NEXT:                               file: FileId(
// B-NEXT:                                   2,
// B-NEXT:                               ),
// B-NEXT:                               kind: User,
// B-NEXT:                               line: 12,
// B-NEXT:                           },
// B-NEXT:                       },
// B-NEXT:                   ],
// B-NEXT:               ),
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:   )
// B-NEXT: concrete:
// B-NEXT: decl[0]: Declaration {
// B-NEXT:       declaration: Declaration {
// B-NEXT:           specifiers: DeclarationSpecifiers {
// B-NEXT:               ty: Integer(
// B-NEXT:                   Ranked {
// B-NEXT:                       rank: Int,
// B-NEXT:                       signed: true,
// B-NEXT:                   },
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:           declarator: Array {
// B-NEXT:               inner: Name(
// B-NEXT:                   "redefined",
// B-NEXT:               ),
// B-NEXT:               size: Expression(
// B-NEXT:                   IntLit(
// B-NEXT:                       1,
// B-NEXT:                   ),
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:       },
// B-NEXT:       provenance: Provenance {
// B-NEXT:           file: FileId(
// B-NEXT:               2,
// B-NEXT:           ),
// B-NEXT:           kind: User,
// B-NEXT:           line: 5,
// B-NEXT:       },
// B-NEXT:   }
// B-NEXT: decl[1]: Declaration {
// B-NEXT:       declaration: Declaration {
// B-NEXT:           specifiers: DeclarationSpecifiers {
// B-NEXT:               ty: Integer(
// B-NEXT:                   Ranked {
// B-NEXT:                       rank: Int,
// B-NEXT:                       signed: true,
// B-NEXT:                   },
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:           declarator: Array {
// B-NEXT:               inner: Name(
// B-NEXT:                   "nested",
// B-NEXT:               ),
// B-NEXT:               size: Expression(
// B-NEXT:                   IntLit(
// B-NEXT:                       1,
// B-NEXT:                   ),
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:       },
// B-NEXT:       provenance: Provenance {
// B-NEXT:           file: FileId(
// B-NEXT:               2,
// B-NEXT:           ),
// B-NEXT:           kind: User,
// B-NEXT:           line: 12,
// B-NEXT:       },
// B-NEXT:   }
// SLATE-FILECHECK-END B
// SLATE-FILECHECK-BEGIN AB
// AB: polyvariant:
// AB-NEXT: decl[0]: Conditional(
// AB-NEXT:       Conditional {
// AB-NEXT:           branches: [
// AB-NEXT:               (
// AB-NEXT:                   Not(
// AB-NEXT:                       Defined(
// AB-NEXT:                           "A",
// AB-NEXT:                       ),
// AB-NEXT:                   ),
// AB-NEXT:                   [
// AB-NEXT:                       Declaration {
// AB-NEXT:                           declaration: Declaration {
// AB-NEXT:                               specifiers: DeclarationSpecifiers {
// AB-NEXT:                                   ty: Integer(
// AB-NEXT:                                       Ranked {
// AB-NEXT:                                           rank: Int,
// AB-NEXT:                                           signed: true,
// AB-NEXT:                                       },
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                               declarator: Array {
// AB-NEXT:                                   inner: Name(
// AB-NEXT:                                       "redefined",
// AB-NEXT:                                   ),
// AB-NEXT:                                   size: Expression(
// AB-NEXT:                                       IntLit(
// AB-NEXT:                                           1,
// AB-NEXT:                                       ),
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                           },
// AB-NEXT:                           provenance: Provenance {
// AB-NEXT:                               file: FileId(
// AB-NEXT:                                   2,
// AB-NEXT:                               ),
// AB-NEXT:                               kind: User,
// AB-NEXT:                               line: 5,
// AB-NEXT:                           },
// AB-NEXT:                       },
// AB-NEXT:                   ],
// AB-NEXT:               ),
// AB-NEXT:               (
// AB-NEXT:                   Defined(
// AB-NEXT:                       "A",
// AB-NEXT:                   ),
// AB-NEXT:                   [
// AB-NEXT:                       Declaration {
// AB-NEXT:                           declaration: Declaration {
// AB-NEXT:                               specifiers: DeclarationSpecifiers {
// AB-NEXT:                                   ty: Integer(
// AB-NEXT:                                       Ranked {
// AB-NEXT:                                           rank: Int,
// AB-NEXT:                                           signed: true,
// AB-NEXT:                                       },
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                               declarator: Array {
// AB-NEXT:                                   inner: Name(
// AB-NEXT:                                       "redefined",
// AB-NEXT:                                   ),
// AB-NEXT:                                   size: Expression(
// AB-NEXT:                                       IntLit(
// AB-NEXT:                                           2,
// AB-NEXT:                                       ),
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                           },
// AB-NEXT:                           provenance: Provenance {
// AB-NEXT:                               file: FileId(
// AB-NEXT:                                   2,
// AB-NEXT:                               ),
// AB-NEXT:                               kind: User,
// AB-NEXT:                               line: 5,
// AB-NEXT:                           },
// AB-NEXT:                       },
// AB-NEXT:                   ],
// AB-NEXT:               ),
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:   )
// AB-NEXT: decl[1]: Conditional(
// AB-NEXT:       Conditional {
// AB-NEXT:           branches: [
// AB-NEXT:               (
// AB-NEXT:                   Not(
// AB-NEXT:                       Defined(
// AB-NEXT:                           "A",
// AB-NEXT:                       ),
// AB-NEXT:                   ),
// AB-NEXT:                   [
// AB-NEXT:                       Declaration {
// AB-NEXT:                           declaration: Declaration {
// AB-NEXT:                               specifiers: DeclarationSpecifiers {
// AB-NEXT:                                   ty: Integer(
// AB-NEXT:                                       Ranked {
// AB-NEXT:                                           rank: Int,
// AB-NEXT:                                           signed: true,
// AB-NEXT:                                       },
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                               declarator: Array {
// AB-NEXT:                                   inner: Name(
// AB-NEXT:                                       "nested",
// AB-NEXT:                                   ),
// AB-NEXT:                                   size: Expression(
// AB-NEXT:                                       IntLit(
// AB-NEXT:                                           1,
// AB-NEXT:                                       ),
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                           },
// AB-NEXT:                           provenance: Provenance {
// AB-NEXT:                               file: FileId(
// AB-NEXT:                                   2,
// AB-NEXT:                               ),
// AB-NEXT:                               kind: User,
// AB-NEXT:                               line: 12,
// AB-NEXT:                           },
// AB-NEXT:                       },
// AB-NEXT:                   ],
// AB-NEXT:               ),
// AB-NEXT:               (
// AB-NEXT:                   And(
// AB-NEXT:                       Not(
// AB-NEXT:                           And(
// AB-NEXT:                               Defined(
// AB-NEXT:                                   "A",
// AB-NEXT:                               ),
// AB-NEXT:                               Defined(
// AB-NEXT:                                   "B",
// AB-NEXT:                               ),
// AB-NEXT:                           ),
// AB-NEXT:                       ),
// AB-NEXT:                       Defined(
// AB-NEXT:                           "A",
// AB-NEXT:                       ),
// AB-NEXT:                   ),
// AB-NEXT:                   [
// AB-NEXT:                       Declaration {
// AB-NEXT:                           declaration: Declaration {
// AB-NEXT:                               specifiers: DeclarationSpecifiers {
// AB-NEXT:                                   ty: Integer(
// AB-NEXT:                                       Ranked {
// AB-NEXT:                                           rank: Int,
// AB-NEXT:                                           signed: true,
// AB-NEXT:                                       },
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                               declarator: Array {
// AB-NEXT:                                   inner: Name(
// AB-NEXT:                                       "nested",
// AB-NEXT:                                   ),
// AB-NEXT:                                   size: Expression(
// AB-NEXT:                                       IntLit(
// AB-NEXT:                                           2,
// AB-NEXT:                                       ),
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                           },
// AB-NEXT:                           provenance: Provenance {
// AB-NEXT:                               file: FileId(
// AB-NEXT:                                   2,
// AB-NEXT:                               ),
// AB-NEXT:                               kind: User,
// AB-NEXT:                               line: 12,
// AB-NEXT:                           },
// AB-NEXT:                       },
// AB-NEXT:                   ],
// AB-NEXT:               ),
// AB-NEXT:               (
// AB-NEXT:                   And(
// AB-NEXT:                       Defined(
// AB-NEXT:                           "A",
// AB-NEXT:                       ),
// AB-NEXT:                       Defined(
// AB-NEXT:                           "B",
// AB-NEXT:                       ),
// AB-NEXT:                   ),
// AB-NEXT:                   [
// AB-NEXT:                       Declaration {
// AB-NEXT:                           declaration: Declaration {
// AB-NEXT:                               specifiers: DeclarationSpecifiers {
// AB-NEXT:                                   ty: Integer(
// AB-NEXT:                                       Ranked {
// AB-NEXT:                                           rank: Int,
// AB-NEXT:                                           signed: true,
// AB-NEXT:                                       },
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                               declarator: Array {
// AB-NEXT:                                   inner: Name(
// AB-NEXT:                                       "nested",
// AB-NEXT:                                   ),
// AB-NEXT:                                   size: Expression(
// AB-NEXT:                                       IntLit(
// AB-NEXT:                                           5,
// AB-NEXT:                                       ),
// AB-NEXT:                                   ),
// AB-NEXT:                               },
// AB-NEXT:                           },
// AB-NEXT:                           provenance: Provenance {
// AB-NEXT:                               file: FileId(
// AB-NEXT:                                   2,
// AB-NEXT:                               ),
// AB-NEXT:                               kind: User,
// AB-NEXT:                               line: 12,
// AB-NEXT:                           },
// AB-NEXT:                       },
// AB-NEXT:                   ],
// AB-NEXT:               ),
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:   )
// AB-NEXT: concrete:
// AB-NEXT: decl[0]: Declaration {
// AB-NEXT:       declaration: Declaration {
// AB-NEXT:           specifiers: DeclarationSpecifiers {
// AB-NEXT:               ty: Integer(
// AB-NEXT:                   Ranked {
// AB-NEXT:                       rank: Int,
// AB-NEXT:                       signed: true,
// AB-NEXT:                   },
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:           declarator: Array {
// AB-NEXT:               inner: Name(
// AB-NEXT:                   "redefined",
// AB-NEXT:               ),
// AB-NEXT:               size: Expression(
// AB-NEXT:                   IntLit(
// AB-NEXT:                       2,
// AB-NEXT:                   ),
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:       },
// AB-NEXT:       provenance: Provenance {
// AB-NEXT:           file: FileId(
// AB-NEXT:               2,
// AB-NEXT:           ),
// AB-NEXT:           kind: User,
// AB-NEXT:           line: 5,
// AB-NEXT:       },
// AB-NEXT:   }
// AB-NEXT: decl[1]: Declaration {
// AB-NEXT:       declaration: Declaration {
// AB-NEXT:           specifiers: DeclarationSpecifiers {
// AB-NEXT:               ty: Integer(
// AB-NEXT:                   Ranked {
// AB-NEXT:                       rank: Int,
// AB-NEXT:                       signed: true,
// AB-NEXT:                   },
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:           declarator: Array {
// AB-NEXT:               inner: Name(
// AB-NEXT:                   "nested",
// AB-NEXT:               ),
// AB-NEXT:               size: Expression(
// AB-NEXT:                   IntLit(
// AB-NEXT:                       5,
// AB-NEXT:                   ),
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:       },
// AB-NEXT:       provenance: Provenance {
// AB-NEXT:           file: FileId(
// AB-NEXT:               2,
// AB-NEXT:           ),
// AB-NEXT:           kind: User,
// AB-NEXT:           line: 12,
// AB-NEXT:       },
// AB-NEXT:   }
// SLATE-FILECHECK-END AB
