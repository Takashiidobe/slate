#define INNER 1
#ifdef A
#undef INNER
#define INNER 2
#endif
#define OUTER INNER
#define WRAP(value) (value + INNER)
int nested[OUTER];
int wrapped[WRAP(1)];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A

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
// DEFAULT-NEXT:                               line: 7,
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
// DEFAULT-NEXT:                               line: 7,
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
// DEFAULT-NEXT:                                       "wrapped",
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
// DEFAULT-NEXT:                               line: 8,
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
// DEFAULT-NEXT:                                       "wrapped",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 8,
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
// DEFAULT-NEXT:           line: 7,
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
// DEFAULT-NEXT:                   "wrapped",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       2,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
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
// A-NEXT:                               line: 7,
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
// A-NEXT:                               line: 7,
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
// A-NEXT:                                       "wrapped",
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
// A-NEXT:                               line: 8,
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
// A-NEXT:                                       "wrapped",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           3,
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                               },
// A-NEXT:                           },
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   2,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 8,
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
// A-NEXT:           line: 7,
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
// A-NEXT:                   "wrapped",
// A-NEXT:               ),
// A-NEXT:               size: Expression(
// A-NEXT:                   IntLit(
// A-NEXT:                       3,
// A-NEXT:                   ),
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 8,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
