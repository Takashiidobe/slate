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
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Or(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       And(
// DEFAULT-NEXT:                           Not(
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "A",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Defined(
// DEFAULT-NEXT:                               "X",
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                       "x_defined",
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
// DEFAULT-NEXT:                               line: 4,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Or(
// DEFAULT-NEXT:                           Defined(
// DEFAULT-NEXT:                               "A",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           And(
// DEFAULT-NEXT:                               Not(
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "A",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "X",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                       "x_undefined",
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
// DEFAULT-NEXT:                               line: 6,
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
// DEFAULT-NEXT:                   And(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "FLAG",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Not(
// DEFAULT-NEXT:                           Or(
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "A",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               And(
// DEFAULT-NEXT:                                   Not(
// DEFAULT-NEXT:                                       Defined(
// DEFAULT-NEXT:                                           "A",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "X",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                       "flag_without_x",
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
// DEFAULT-NEXT:                               line: 9,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Or(
// DEFAULT-NEXT:                           Defined(
// DEFAULT-NEXT:                               "A",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           And(
// DEFAULT-NEXT:                               Not(
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "A",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "X",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                       "x_missing",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           4,
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
// DEFAULT-NEXT:                   "x_undefined",
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
// DEFAULT-NEXT:           line: 6,
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
// DEFAULT-NEXT:                   "x_missing",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       4,
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
// A-NEXT:                   Or(
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                       And(
// A-NEXT:                           Not(
// A-NEXT:                               Defined(
// A-NEXT:                                   "A",
// A-NEXT:                               ),
// A-NEXT:                           ),
// A-NEXT:                           Defined(
// A-NEXT:                               "X",
// A-NEXT:                           ),
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
// A-NEXT:                                       "x_defined",
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
// A-NEXT:                               line: 4,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:               (
// A-NEXT:                   Not(
// A-NEXT:                       Or(
// A-NEXT:                           Defined(
// A-NEXT:                               "A",
// A-NEXT:                           ),
// A-NEXT:                           And(
// A-NEXT:                               Not(
// A-NEXT:                                   Defined(
// A-NEXT:                                       "A",
// A-NEXT:                                   ),
// A-NEXT:                               ),
// A-NEXT:                               Defined(
// A-NEXT:                                   "X",
// A-NEXT:                               ),
// A-NEXT:                           ),
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
// A-NEXT:                                       "x_undefined",
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
// A-NEXT:                               line: 6,
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
// A-NEXT:                   And(
// A-NEXT:                       Defined(
// A-NEXT:                           "FLAG",
// A-NEXT:                       ),
// A-NEXT:                       Not(
// A-NEXT:                           Or(
// A-NEXT:                               Defined(
// A-NEXT:                                   "A",
// A-NEXT:                               ),
// A-NEXT:                               And(
// A-NEXT:                                   Not(
// A-NEXT:                                       Defined(
// A-NEXT:                                           "A",
// A-NEXT:                                       ),
// A-NEXT:                                   ),
// A-NEXT:                                   Defined(
// A-NEXT:                                       "X",
// A-NEXT:                                   ),
// A-NEXT:                               ),
// A-NEXT:                           ),
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
// A-NEXT:                                       "flag_without_x",
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
// A-NEXT:                               line: 9,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[2]: Conditional(
// A-NEXT:       Conditional {
// A-NEXT:           branches: [
// A-NEXT:               (
// A-NEXT:                   Not(
// A-NEXT:                       Or(
// A-NEXT:                           Defined(
// A-NEXT:                               "A",
// A-NEXT:                           ),
// A-NEXT:                           And(
// A-NEXT:                               Not(
// A-NEXT:                                   Defined(
// A-NEXT:                                       "A",
// A-NEXT:                                   ),
// A-NEXT:                               ),
// A-NEXT:                               Defined(
// A-NEXT:                                   "X",
// A-NEXT:                               ),
// A-NEXT:                           ),
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
// A-NEXT:                                       "x_missing",
// A-NEXT:                                   ),
// A-NEXT:                                   size: Expression(
// A-NEXT:                                       IntLit(
// A-NEXT:                                           4,
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
// A-NEXT:                   "x_defined",
// A-NEXT:               ),
// A-NEXT:               size: Expression(
// A-NEXT:                   IntLit(
// A-NEXT:                       1,
// A-NEXT:                   ),
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 4,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN FLAG
// FLAG: polyvariant:
// FLAG-NEXT: decl[0]: Conditional(
// FLAG-NEXT:       Conditional {
// FLAG-NEXT:           branches: [
// FLAG-NEXT:               (
// FLAG-NEXT:                   Or(
// FLAG-NEXT:                       Defined(
// FLAG-NEXT:                           "A",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       And(
// FLAG-NEXT:                           Not(
// FLAG-NEXT:                               Defined(
// FLAG-NEXT:                                   "A",
// FLAG-NEXT:                               ),
// FLAG-NEXT:                           ),
// FLAG-NEXT:                           Defined(
// FLAG-NEXT:                               "X",
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   ),
// FLAG-NEXT:                   [
// FLAG-NEXT:                       Declaration {
// FLAG-NEXT:                           declaration: Declaration {
// FLAG-NEXT:                               specifiers: DeclarationSpecifiers {
// FLAG-NEXT:                                   ty: Integer(
// FLAG-NEXT:                                       Ranked {
// FLAG-NEXT:                                           rank: Int,
// FLAG-NEXT:                                           signed: true,
// FLAG-NEXT:                                       },
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                               declarator: Array {
// FLAG-NEXT:                                   inner: Name(
// FLAG-NEXT:                                       "x_defined",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                                   size: Expression(
// FLAG-NEXT:                                       IntLit(
// FLAG-NEXT:                                           1,
// FLAG-NEXT:                                       ),
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                           },
// FLAG-NEXT:                           provenance: Provenance {
// FLAG-NEXT:                               file: FileId(
// FLAG-NEXT:                                   2,
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               kind: User,
// FLAG-NEXT:                               line: 4,
// FLAG-NEXT:                           },
// FLAG-NEXT:                       },
// FLAG-NEXT:                   ],
// FLAG-NEXT:               ),
// FLAG-NEXT:               (
// FLAG-NEXT:                   Not(
// FLAG-NEXT:                       Or(
// FLAG-NEXT:                           Defined(
// FLAG-NEXT:                               "A",
// FLAG-NEXT:                           ),
// FLAG-NEXT:                           And(
// FLAG-NEXT:                               Not(
// FLAG-NEXT:                                   Defined(
// FLAG-NEXT:                                       "A",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               Defined(
// FLAG-NEXT:                                   "X",
// FLAG-NEXT:                               ),
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   ),
// FLAG-NEXT:                   [
// FLAG-NEXT:                       Declaration {
// FLAG-NEXT:                           declaration: Declaration {
// FLAG-NEXT:                               specifiers: DeclarationSpecifiers {
// FLAG-NEXT:                                   ty: Integer(
// FLAG-NEXT:                                       Ranked {
// FLAG-NEXT:                                           rank: Int,
// FLAG-NEXT:                                           signed: true,
// FLAG-NEXT:                                       },
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                               declarator: Array {
// FLAG-NEXT:                                   inner: Name(
// FLAG-NEXT:                                       "x_undefined",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                                   size: Expression(
// FLAG-NEXT:                                       IntLit(
// FLAG-NEXT:                                           2,
// FLAG-NEXT:                                       ),
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                           },
// FLAG-NEXT:                           provenance: Provenance {
// FLAG-NEXT:                               file: FileId(
// FLAG-NEXT:                                   2,
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               kind: User,
// FLAG-NEXT:                               line: 6,
// FLAG-NEXT:                           },
// FLAG-NEXT:                       },
// FLAG-NEXT:                   ],
// FLAG-NEXT:               ),
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// FLAG-NEXT: decl[1]: Conditional(
// FLAG-NEXT:       Conditional {
// FLAG-NEXT:           branches: [
// FLAG-NEXT:               (
// FLAG-NEXT:                   And(
// FLAG-NEXT:                       Defined(
// FLAG-NEXT:                           "FLAG",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       Not(
// FLAG-NEXT:                           Or(
// FLAG-NEXT:                               Defined(
// FLAG-NEXT:                                   "A",
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               And(
// FLAG-NEXT:                                   Not(
// FLAG-NEXT:                                       Defined(
// FLAG-NEXT:                                           "A",
// FLAG-NEXT:                                       ),
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                                   Defined(
// FLAG-NEXT:                                       "X",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               ),
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   ),
// FLAG-NEXT:                   [
// FLAG-NEXT:                       Declaration {
// FLAG-NEXT:                           declaration: Declaration {
// FLAG-NEXT:                               specifiers: DeclarationSpecifiers {
// FLAG-NEXT:                                   ty: Integer(
// FLAG-NEXT:                                       Ranked {
// FLAG-NEXT:                                           rank: Int,
// FLAG-NEXT:                                           signed: true,
// FLAG-NEXT:                                       },
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                               declarator: Array {
// FLAG-NEXT:                                   inner: Name(
// FLAG-NEXT:                                       "flag_without_x",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                                   size: Expression(
// FLAG-NEXT:                                       IntLit(
// FLAG-NEXT:                                           3,
// FLAG-NEXT:                                       ),
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                           },
// FLAG-NEXT:                           provenance: Provenance {
// FLAG-NEXT:                               file: FileId(
// FLAG-NEXT:                                   2,
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               kind: User,
// FLAG-NEXT:                               line: 9,
// FLAG-NEXT:                           },
// FLAG-NEXT:                       },
// FLAG-NEXT:                   ],
// FLAG-NEXT:               ),
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// FLAG-NEXT: decl[2]: Conditional(
// FLAG-NEXT:       Conditional {
// FLAG-NEXT:           branches: [
// FLAG-NEXT:               (
// FLAG-NEXT:                   Not(
// FLAG-NEXT:                       Or(
// FLAG-NEXT:                           Defined(
// FLAG-NEXT:                               "A",
// FLAG-NEXT:                           ),
// FLAG-NEXT:                           And(
// FLAG-NEXT:                               Not(
// FLAG-NEXT:                                   Defined(
// FLAG-NEXT:                                       "A",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               Defined(
// FLAG-NEXT:                                   "X",
// FLAG-NEXT:                               ),
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   ),
// FLAG-NEXT:                   [
// FLAG-NEXT:                       Declaration {
// FLAG-NEXT:                           declaration: Declaration {
// FLAG-NEXT:                               specifiers: DeclarationSpecifiers {
// FLAG-NEXT:                                   ty: Integer(
// FLAG-NEXT:                                       Ranked {
// FLAG-NEXT:                                           rank: Int,
// FLAG-NEXT:                                           signed: true,
// FLAG-NEXT:                                       },
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                               declarator: Array {
// FLAG-NEXT:                                   inner: Name(
// FLAG-NEXT:                                       "x_missing",
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                                   size: Expression(
// FLAG-NEXT:                                       IntLit(
// FLAG-NEXT:                                           4,
// FLAG-NEXT:                                       ),
// FLAG-NEXT:                                   ),
// FLAG-NEXT:                               },
// FLAG-NEXT:                           },
// FLAG-NEXT:                           provenance: Provenance {
// FLAG-NEXT:                               file: FileId(
// FLAG-NEXT:                                   2,
// FLAG-NEXT:                               ),
// FLAG-NEXT:                               kind: User,
// FLAG-NEXT:                               line: 12,
// FLAG-NEXT:                           },
// FLAG-NEXT:                       },
// FLAG-NEXT:                   ],
// FLAG-NEXT:               ),
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:   )
// FLAG-NEXT: concrete:
// FLAG-NEXT: decl[0]: Declaration {
// FLAG-NEXT:       declaration: Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarator: Array {
// FLAG-NEXT:               inner: Name(
// FLAG-NEXT:                   "x_undefined",
// FLAG-NEXT:               ),
// FLAG-NEXT:               size: Expression(
// FLAG-NEXT:                   IntLit(
// FLAG-NEXT:                       2,
// FLAG-NEXT:                   ),
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               2,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 6,
// FLAG-NEXT:       },
// FLAG-NEXT:   }
// FLAG-NEXT: decl[1]: Declaration {
// FLAG-NEXT:       declaration: Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarator: Array {
// FLAG-NEXT:               inner: Name(
// FLAG-NEXT:                   "flag_without_x",
// FLAG-NEXT:               ),
// FLAG-NEXT:               size: Expression(
// FLAG-NEXT:                   IntLit(
// FLAG-NEXT:                       3,
// FLAG-NEXT:                   ),
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               2,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 9,
// FLAG-NEXT:       },
// FLAG-NEXT:   }
// FLAG-NEXT: decl[2]: Declaration {
// FLAG-NEXT:       declaration: Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarator: Array {
// FLAG-NEXT:               inner: Name(
// FLAG-NEXT:                   "x_missing",
// FLAG-NEXT:               ),
// FLAG-NEXT:               size: Expression(
// FLAG-NEXT:                   IntLit(
// FLAG-NEXT:                       4,
// FLAG-NEXT:                   ),
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               2,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 12,
// FLAG-NEXT:       },
// FLAG-NEXT:   }
// SLATE-FILECHECK-END FLAG
// SLATE-FILECHECK-BEGIN X
// X: polyvariant:
// X-NEXT: decl[0]: Conditional(
// X-NEXT:       Conditional {
// X-NEXT:           branches: [
// X-NEXT:               (
// X-NEXT:                   Or(
// X-NEXT:                       Defined(
// X-NEXT:                           "A",
// X-NEXT:                       ),
// X-NEXT:                       And(
// X-NEXT:                           Not(
// X-NEXT:                               Defined(
// X-NEXT:                                   "A",
// X-NEXT:                               ),
// X-NEXT:                           ),
// X-NEXT:                           Defined(
// X-NEXT:                               "X",
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   ),
// X-NEXT:                   [
// X-NEXT:                       Declaration {
// X-NEXT:                           declaration: Declaration {
// X-NEXT:                               specifiers: DeclarationSpecifiers {
// X-NEXT:                                   ty: Integer(
// X-NEXT:                                       Ranked {
// X-NEXT:                                           rank: Int,
// X-NEXT:                                           signed: true,
// X-NEXT:                                       },
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                               declarator: Array {
// X-NEXT:                                   inner: Name(
// X-NEXT:                                       "x_defined",
// X-NEXT:                                   ),
// X-NEXT:                                   size: Expression(
// X-NEXT:                                       IntLit(
// X-NEXT:                                           1,
// X-NEXT:                                       ),
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                           },
// X-NEXT:                           provenance: Provenance {
// X-NEXT:                               file: FileId(
// X-NEXT:                                   2,
// X-NEXT:                               ),
// X-NEXT:                               kind: User,
// X-NEXT:                               line: 4,
// X-NEXT:                           },
// X-NEXT:                       },
// X-NEXT:                   ],
// X-NEXT:               ),
// X-NEXT:               (
// X-NEXT:                   Not(
// X-NEXT:                       Or(
// X-NEXT:                           Defined(
// X-NEXT:                               "A",
// X-NEXT:                           ),
// X-NEXT:                           And(
// X-NEXT:                               Not(
// X-NEXT:                                   Defined(
// X-NEXT:                                       "A",
// X-NEXT:                                   ),
// X-NEXT:                               ),
// X-NEXT:                               Defined(
// X-NEXT:                                   "X",
// X-NEXT:                               ),
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   ),
// X-NEXT:                   [
// X-NEXT:                       Declaration {
// X-NEXT:                           declaration: Declaration {
// X-NEXT:                               specifiers: DeclarationSpecifiers {
// X-NEXT:                                   ty: Integer(
// X-NEXT:                                       Ranked {
// X-NEXT:                                           rank: Int,
// X-NEXT:                                           signed: true,
// X-NEXT:                                       },
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                               declarator: Array {
// X-NEXT:                                   inner: Name(
// X-NEXT:                                       "x_undefined",
// X-NEXT:                                   ),
// X-NEXT:                                   size: Expression(
// X-NEXT:                                       IntLit(
// X-NEXT:                                           2,
// X-NEXT:                                       ),
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                           },
// X-NEXT:                           provenance: Provenance {
// X-NEXT:                               file: FileId(
// X-NEXT:                                   2,
// X-NEXT:                               ),
// X-NEXT:                               kind: User,
// X-NEXT:                               line: 6,
// X-NEXT:                           },
// X-NEXT:                       },
// X-NEXT:                   ],
// X-NEXT:               ),
// X-NEXT:           ],
// X-NEXT:       },
// X-NEXT:   )
// X-NEXT: decl[1]: Conditional(
// X-NEXT:       Conditional {
// X-NEXT:           branches: [
// X-NEXT:               (
// X-NEXT:                   And(
// X-NEXT:                       Defined(
// X-NEXT:                           "FLAG",
// X-NEXT:                       ),
// X-NEXT:                       Not(
// X-NEXT:                           Or(
// X-NEXT:                               Defined(
// X-NEXT:                                   "A",
// X-NEXT:                               ),
// X-NEXT:                               And(
// X-NEXT:                                   Not(
// X-NEXT:                                       Defined(
// X-NEXT:                                           "A",
// X-NEXT:                                       ),
// X-NEXT:                                   ),
// X-NEXT:                                   Defined(
// X-NEXT:                                       "X",
// X-NEXT:                                   ),
// X-NEXT:                               ),
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   ),
// X-NEXT:                   [
// X-NEXT:                       Declaration {
// X-NEXT:                           declaration: Declaration {
// X-NEXT:                               specifiers: DeclarationSpecifiers {
// X-NEXT:                                   ty: Integer(
// X-NEXT:                                       Ranked {
// X-NEXT:                                           rank: Int,
// X-NEXT:                                           signed: true,
// X-NEXT:                                       },
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                               declarator: Array {
// X-NEXT:                                   inner: Name(
// X-NEXT:                                       "flag_without_x",
// X-NEXT:                                   ),
// X-NEXT:                                   size: Expression(
// X-NEXT:                                       IntLit(
// X-NEXT:                                           3,
// X-NEXT:                                       ),
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                           },
// X-NEXT:                           provenance: Provenance {
// X-NEXT:                               file: FileId(
// X-NEXT:                                   2,
// X-NEXT:                               ),
// X-NEXT:                               kind: User,
// X-NEXT:                               line: 9,
// X-NEXT:                           },
// X-NEXT:                       },
// X-NEXT:                   ],
// X-NEXT:               ),
// X-NEXT:           ],
// X-NEXT:       },
// X-NEXT:   )
// X-NEXT: decl[2]: Conditional(
// X-NEXT:       Conditional {
// X-NEXT:           branches: [
// X-NEXT:               (
// X-NEXT:                   Not(
// X-NEXT:                       Or(
// X-NEXT:                           Defined(
// X-NEXT:                               "A",
// X-NEXT:                           ),
// X-NEXT:                           And(
// X-NEXT:                               Not(
// X-NEXT:                                   Defined(
// X-NEXT:                                       "A",
// X-NEXT:                                   ),
// X-NEXT:                               ),
// X-NEXT:                               Defined(
// X-NEXT:                                   "X",
// X-NEXT:                               ),
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   ),
// X-NEXT:                   [
// X-NEXT:                       Declaration {
// X-NEXT:                           declaration: Declaration {
// X-NEXT:                               specifiers: DeclarationSpecifiers {
// X-NEXT:                                   ty: Integer(
// X-NEXT:                                       Ranked {
// X-NEXT:                                           rank: Int,
// X-NEXT:                                           signed: true,
// X-NEXT:                                       },
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                               declarator: Array {
// X-NEXT:                                   inner: Name(
// X-NEXT:                                       "x_missing",
// X-NEXT:                                   ),
// X-NEXT:                                   size: Expression(
// X-NEXT:                                       IntLit(
// X-NEXT:                                           4,
// X-NEXT:                                       ),
// X-NEXT:                                   ),
// X-NEXT:                               },
// X-NEXT:                           },
// X-NEXT:                           provenance: Provenance {
// X-NEXT:                               file: FileId(
// X-NEXT:                                   2,
// X-NEXT:                               ),
// X-NEXT:                               kind: User,
// X-NEXT:                               line: 12,
// X-NEXT:                           },
// X-NEXT:                       },
// X-NEXT:                   ],
// X-NEXT:               ),
// X-NEXT:           ],
// X-NEXT:       },
// X-NEXT:   )
// X-NEXT: concrete:
// X-NEXT: decl[0]: Declaration {
// X-NEXT:       declaration: Declaration {
// X-NEXT:           specifiers: DeclarationSpecifiers {
// X-NEXT:               ty: Integer(
// X-NEXT:                   Ranked {
// X-NEXT:                       rank: Int,
// X-NEXT:                       signed: true,
// X-NEXT:                   },
// X-NEXT:               ),
// X-NEXT:           },
// X-NEXT:           declarator: Array {
// X-NEXT:               inner: Name(
// X-NEXT:                   "x_defined",
// X-NEXT:               ),
// X-NEXT:               size: Expression(
// X-NEXT:                   IntLit(
// X-NEXT:                       1,
// X-NEXT:                   ),
// X-NEXT:               ),
// X-NEXT:           },
// X-NEXT:       },
// X-NEXT:       provenance: Provenance {
// X-NEXT:           file: FileId(
// X-NEXT:               2,
// X-NEXT:           ),
// X-NEXT:           kind: User,
// X-NEXT:           line: 4,
// X-NEXT:       },
// X-NEXT:   }
// SLATE-FILECHECK-END X
