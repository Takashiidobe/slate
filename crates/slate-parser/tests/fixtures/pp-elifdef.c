#ifdef FIRST
int first;
#elifdef SECOND
int second;
#elifndef THIRD
int not_third;
#else
int fallback;
#endif
#if 0
#ifdef DEAD
#elifdef ALSO_DEAD
#endif
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES FIRST FIRST
// SLATE-FILECHECK-DEFINES SECOND SECOND
// SLATE-FILECHECK-DEFINES THIRD THIRD

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "FIRST",
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "first",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 1,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   And(
// DEFAULT-NEXT:                       Not(
// DEFAULT-NEXT:                           Defined(
// DEFAULT-NEXT:                               "FIRST",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "SECOND",
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "second",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 3,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   And(
// DEFAULT-NEXT:                       Not(
// DEFAULT-NEXT:                           Or(
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "FIRST",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               And(
// DEFAULT-NEXT:                                   Not(
// DEFAULT-NEXT:                                       Defined(
// DEFAULT-NEXT:                                           "FIRST",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "SECOND",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Not(
// DEFAULT-NEXT:                           Defined(
// DEFAULT-NEXT:                               "THIRD",
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "not_third",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Or(
// DEFAULT-NEXT:                           Or(
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "FIRST",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               And(
// DEFAULT-NEXT:                                   Not(
// DEFAULT-NEXT:                                       Defined(
// DEFAULT-NEXT:                                           "FIRST",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "SECOND",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           And(
// DEFAULT-NEXT:                               Not(
// DEFAULT-NEXT:                                   Or(
// DEFAULT-NEXT:                                       Defined(
// DEFAULT-NEXT:                                           "FIRST",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       And(
// DEFAULT-NEXT:                                           Not(
// DEFAULT-NEXT:                                               Defined(
// DEFAULT-NEXT:                                                   "FIRST",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Defined(
// DEFAULT-NEXT:                                               "SECOND",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Not(
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "THIRD",
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "fallback",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "not_third",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FIRST
// FIRST: polyvariant:
// FIRST-NEXT: decl[0]: Conditional(
// FIRST-NEXT:       Conditional {
// FIRST-NEXT:           branches: [
// FIRST-NEXT:               (
// FIRST-NEXT:                   Defined(
// FIRST-NEXT:                       "FIRST",
// FIRST-NEXT:                   ),
// FIRST-NEXT:                   [
// FIRST-NEXT:                       Declaration {
// FIRST-NEXT:                           declaration: Declaration {
// FIRST-NEXT:                               specifiers: DeclarationSpecifiers {
// FIRST-NEXT:                                   ty: Integer(
// FIRST-NEXT:                                       Ranked {
// FIRST-NEXT:                                           rank: Int,
// FIRST-NEXT:                                           signed: true,
// FIRST-NEXT:                                       },
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               },
// FIRST-NEXT:                               declarator: Name(
// FIRST-NEXT:                                   "first",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           },
// FIRST-NEXT:                           provenance: Provenance {
// FIRST-NEXT:                               file: FileId(
// FIRST-NEXT:                                   2,
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               kind: User,
// FIRST-NEXT:                               line: 1,
// FIRST-NEXT:                           },
// FIRST-NEXT:                       },
// FIRST-NEXT:                   ],
// FIRST-NEXT:               ),
// FIRST-NEXT:               (
// FIRST-NEXT:                   And(
// FIRST-NEXT:                       Not(
// FIRST-NEXT:                           Defined(
// FIRST-NEXT:                               "FIRST",
// FIRST-NEXT:                           ),
// FIRST-NEXT:                       ),
// FIRST-NEXT:                       Defined(
// FIRST-NEXT:                           "SECOND",
// FIRST-NEXT:                       ),
// FIRST-NEXT:                   ),
// FIRST-NEXT:                   [
// FIRST-NEXT:                       Declaration {
// FIRST-NEXT:                           declaration: Declaration {
// FIRST-NEXT:                               specifiers: DeclarationSpecifiers {
// FIRST-NEXT:                                   ty: Integer(
// FIRST-NEXT:                                       Ranked {
// FIRST-NEXT:                                           rank: Int,
// FIRST-NEXT:                                           signed: true,
// FIRST-NEXT:                                       },
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               },
// FIRST-NEXT:                               declarator: Name(
// FIRST-NEXT:                                   "second",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           },
// FIRST-NEXT:                           provenance: Provenance {
// FIRST-NEXT:                               file: FileId(
// FIRST-NEXT:                                   2,
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               kind: User,
// FIRST-NEXT:                               line: 3,
// FIRST-NEXT:                           },
// FIRST-NEXT:                       },
// FIRST-NEXT:                   ],
// FIRST-NEXT:               ),
// FIRST-NEXT:               (
// FIRST-NEXT:                   And(
// FIRST-NEXT:                       Not(
// FIRST-NEXT:                           Or(
// FIRST-NEXT:                               Defined(
// FIRST-NEXT:                                   "FIRST",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               And(
// FIRST-NEXT:                                   Not(
// FIRST-NEXT:                                       Defined(
// FIRST-NEXT:                                           "FIRST",
// FIRST-NEXT:                                       ),
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                                   Defined(
// FIRST-NEXT:                                       "SECOND",
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           ),
// FIRST-NEXT:                       ),
// FIRST-NEXT:                       Not(
// FIRST-NEXT:                           Defined(
// FIRST-NEXT:                               "THIRD",
// FIRST-NEXT:                           ),
// FIRST-NEXT:                       ),
// FIRST-NEXT:                   ),
// FIRST-NEXT:                   [
// FIRST-NEXT:                       Declaration {
// FIRST-NEXT:                           declaration: Declaration {
// FIRST-NEXT:                               specifiers: DeclarationSpecifiers {
// FIRST-NEXT:                                   ty: Integer(
// FIRST-NEXT:                                       Ranked {
// FIRST-NEXT:                                           rank: Int,
// FIRST-NEXT:                                           signed: true,
// FIRST-NEXT:                                       },
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               },
// FIRST-NEXT:                               declarator: Name(
// FIRST-NEXT:                                   "not_third",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           },
// FIRST-NEXT:                           provenance: Provenance {
// FIRST-NEXT:                               file: FileId(
// FIRST-NEXT:                                   2,
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               kind: User,
// FIRST-NEXT:                               line: 5,
// FIRST-NEXT:                           },
// FIRST-NEXT:                       },
// FIRST-NEXT:                   ],
// FIRST-NEXT:               ),
// FIRST-NEXT:               (
// FIRST-NEXT:                   Not(
// FIRST-NEXT:                       Or(
// FIRST-NEXT:                           Or(
// FIRST-NEXT:                               Defined(
// FIRST-NEXT:                                   "FIRST",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               And(
// FIRST-NEXT:                                   Not(
// FIRST-NEXT:                                       Defined(
// FIRST-NEXT:                                           "FIRST",
// FIRST-NEXT:                                       ),
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                                   Defined(
// FIRST-NEXT:                                       "SECOND",
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           ),
// FIRST-NEXT:                           And(
// FIRST-NEXT:                               Not(
// FIRST-NEXT:                                   Or(
// FIRST-NEXT:                                       Defined(
// FIRST-NEXT:                                           "FIRST",
// FIRST-NEXT:                                       ),
// FIRST-NEXT:                                       And(
// FIRST-NEXT:                                           Not(
// FIRST-NEXT:                                               Defined(
// FIRST-NEXT:                                                   "FIRST",
// FIRST-NEXT:                                               ),
// FIRST-NEXT:                                           ),
// FIRST-NEXT:                                           Defined(
// FIRST-NEXT:                                               "SECOND",
// FIRST-NEXT:                                           ),
// FIRST-NEXT:                                       ),
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               Not(
// FIRST-NEXT:                                   Defined(
// FIRST-NEXT:                                       "THIRD",
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           ),
// FIRST-NEXT:                       ),
// FIRST-NEXT:                   ),
// FIRST-NEXT:                   [
// FIRST-NEXT:                       Declaration {
// FIRST-NEXT:                           declaration: Declaration {
// FIRST-NEXT:                               specifiers: DeclarationSpecifiers {
// FIRST-NEXT:                                   ty: Integer(
// FIRST-NEXT:                                       Ranked {
// FIRST-NEXT:                                           rank: Int,
// FIRST-NEXT:                                           signed: true,
// FIRST-NEXT:                                       },
// FIRST-NEXT:                                   ),
// FIRST-NEXT:                               },
// FIRST-NEXT:                               declarator: Name(
// FIRST-NEXT:                                   "fallback",
// FIRST-NEXT:                               ),
// FIRST-NEXT:                           },
// FIRST-NEXT:                           provenance: Provenance {
// FIRST-NEXT:                               file: FileId(
// FIRST-NEXT:                                   2,
// FIRST-NEXT:                               ),
// FIRST-NEXT:                               kind: User,
// FIRST-NEXT:                               line: 7,
// FIRST-NEXT:                           },
// FIRST-NEXT:                       },
// FIRST-NEXT:                   ],
// FIRST-NEXT:               ),
// FIRST-NEXT:           ],
// FIRST-NEXT:       },
// FIRST-NEXT:   )
// FIRST-NEXT: concrete:
// FIRST-NEXT: decl[0]: Declaration {
// FIRST-NEXT:       declaration: Declaration {
// FIRST-NEXT:           specifiers: DeclarationSpecifiers {
// FIRST-NEXT:               ty: Integer(
// FIRST-NEXT:                   Ranked {
// FIRST-NEXT:                       rank: Int,
// FIRST-NEXT:                       signed: true,
// FIRST-NEXT:                   },
// FIRST-NEXT:               ),
// FIRST-NEXT:           },
// FIRST-NEXT:           declarator: Name(
// FIRST-NEXT:               "first",
// FIRST-NEXT:           ),
// FIRST-NEXT:       },
// FIRST-NEXT:       provenance: Provenance {
// FIRST-NEXT:           file: FileId(
// FIRST-NEXT:               2,
// FIRST-NEXT:           ),
// FIRST-NEXT:           kind: User,
// FIRST-NEXT:           line: 1,
// FIRST-NEXT:       },
// FIRST-NEXT:   }
// SLATE-FILECHECK-END FIRST
// SLATE-FILECHECK-BEGIN SECOND
// SECOND: polyvariant:
// SECOND-NEXT: decl[0]: Conditional(
// SECOND-NEXT:       Conditional {
// SECOND-NEXT:           branches: [
// SECOND-NEXT:               (
// SECOND-NEXT:                   Defined(
// SECOND-NEXT:                       "FIRST",
// SECOND-NEXT:                   ),
// SECOND-NEXT:                   [
// SECOND-NEXT:                       Declaration {
// SECOND-NEXT:                           declaration: Declaration {
// SECOND-NEXT:                               specifiers: DeclarationSpecifiers {
// SECOND-NEXT:                                   ty: Integer(
// SECOND-NEXT:                                       Ranked {
// SECOND-NEXT:                                           rank: Int,
// SECOND-NEXT:                                           signed: true,
// SECOND-NEXT:                                       },
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               },
// SECOND-NEXT:                               declarator: Name(
// SECOND-NEXT:                                   "first",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           },
// SECOND-NEXT:                           provenance: Provenance {
// SECOND-NEXT:                               file: FileId(
// SECOND-NEXT:                                   2,
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               kind: User,
// SECOND-NEXT:                               line: 1,
// SECOND-NEXT:                           },
// SECOND-NEXT:                       },
// SECOND-NEXT:                   ],
// SECOND-NEXT:               ),
// SECOND-NEXT:               (
// SECOND-NEXT:                   And(
// SECOND-NEXT:                       Not(
// SECOND-NEXT:                           Defined(
// SECOND-NEXT:                               "FIRST",
// SECOND-NEXT:                           ),
// SECOND-NEXT:                       ),
// SECOND-NEXT:                       Defined(
// SECOND-NEXT:                           "SECOND",
// SECOND-NEXT:                       ),
// SECOND-NEXT:                   ),
// SECOND-NEXT:                   [
// SECOND-NEXT:                       Declaration {
// SECOND-NEXT:                           declaration: Declaration {
// SECOND-NEXT:                               specifiers: DeclarationSpecifiers {
// SECOND-NEXT:                                   ty: Integer(
// SECOND-NEXT:                                       Ranked {
// SECOND-NEXT:                                           rank: Int,
// SECOND-NEXT:                                           signed: true,
// SECOND-NEXT:                                       },
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               },
// SECOND-NEXT:                               declarator: Name(
// SECOND-NEXT:                                   "second",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           },
// SECOND-NEXT:                           provenance: Provenance {
// SECOND-NEXT:                               file: FileId(
// SECOND-NEXT:                                   2,
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               kind: User,
// SECOND-NEXT:                               line: 3,
// SECOND-NEXT:                           },
// SECOND-NEXT:                       },
// SECOND-NEXT:                   ],
// SECOND-NEXT:               ),
// SECOND-NEXT:               (
// SECOND-NEXT:                   And(
// SECOND-NEXT:                       Not(
// SECOND-NEXT:                           Or(
// SECOND-NEXT:                               Defined(
// SECOND-NEXT:                                   "FIRST",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               And(
// SECOND-NEXT:                                   Not(
// SECOND-NEXT:                                       Defined(
// SECOND-NEXT:                                           "FIRST",
// SECOND-NEXT:                                       ),
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                                   Defined(
// SECOND-NEXT:                                       "SECOND",
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           ),
// SECOND-NEXT:                       ),
// SECOND-NEXT:                       Not(
// SECOND-NEXT:                           Defined(
// SECOND-NEXT:                               "THIRD",
// SECOND-NEXT:                           ),
// SECOND-NEXT:                       ),
// SECOND-NEXT:                   ),
// SECOND-NEXT:                   [
// SECOND-NEXT:                       Declaration {
// SECOND-NEXT:                           declaration: Declaration {
// SECOND-NEXT:                               specifiers: DeclarationSpecifiers {
// SECOND-NEXT:                                   ty: Integer(
// SECOND-NEXT:                                       Ranked {
// SECOND-NEXT:                                           rank: Int,
// SECOND-NEXT:                                           signed: true,
// SECOND-NEXT:                                       },
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               },
// SECOND-NEXT:                               declarator: Name(
// SECOND-NEXT:                                   "not_third",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           },
// SECOND-NEXT:                           provenance: Provenance {
// SECOND-NEXT:                               file: FileId(
// SECOND-NEXT:                                   2,
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               kind: User,
// SECOND-NEXT:                               line: 5,
// SECOND-NEXT:                           },
// SECOND-NEXT:                       },
// SECOND-NEXT:                   ],
// SECOND-NEXT:               ),
// SECOND-NEXT:               (
// SECOND-NEXT:                   Not(
// SECOND-NEXT:                       Or(
// SECOND-NEXT:                           Or(
// SECOND-NEXT:                               Defined(
// SECOND-NEXT:                                   "FIRST",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               And(
// SECOND-NEXT:                                   Not(
// SECOND-NEXT:                                       Defined(
// SECOND-NEXT:                                           "FIRST",
// SECOND-NEXT:                                       ),
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                                   Defined(
// SECOND-NEXT:                                       "SECOND",
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           ),
// SECOND-NEXT:                           And(
// SECOND-NEXT:                               Not(
// SECOND-NEXT:                                   Or(
// SECOND-NEXT:                                       Defined(
// SECOND-NEXT:                                           "FIRST",
// SECOND-NEXT:                                       ),
// SECOND-NEXT:                                       And(
// SECOND-NEXT:                                           Not(
// SECOND-NEXT:                                               Defined(
// SECOND-NEXT:                                                   "FIRST",
// SECOND-NEXT:                                               ),
// SECOND-NEXT:                                           ),
// SECOND-NEXT:                                           Defined(
// SECOND-NEXT:                                               "SECOND",
// SECOND-NEXT:                                           ),
// SECOND-NEXT:                                       ),
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               Not(
// SECOND-NEXT:                                   Defined(
// SECOND-NEXT:                                       "THIRD",
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           ),
// SECOND-NEXT:                       ),
// SECOND-NEXT:                   ),
// SECOND-NEXT:                   [
// SECOND-NEXT:                       Declaration {
// SECOND-NEXT:                           declaration: Declaration {
// SECOND-NEXT:                               specifiers: DeclarationSpecifiers {
// SECOND-NEXT:                                   ty: Integer(
// SECOND-NEXT:                                       Ranked {
// SECOND-NEXT:                                           rank: Int,
// SECOND-NEXT:                                           signed: true,
// SECOND-NEXT:                                       },
// SECOND-NEXT:                                   ),
// SECOND-NEXT:                               },
// SECOND-NEXT:                               declarator: Name(
// SECOND-NEXT:                                   "fallback",
// SECOND-NEXT:                               ),
// SECOND-NEXT:                           },
// SECOND-NEXT:                           provenance: Provenance {
// SECOND-NEXT:                               file: FileId(
// SECOND-NEXT:                                   2,
// SECOND-NEXT:                               ),
// SECOND-NEXT:                               kind: User,
// SECOND-NEXT:                               line: 7,
// SECOND-NEXT:                           },
// SECOND-NEXT:                       },
// SECOND-NEXT:                   ],
// SECOND-NEXT:               ),
// SECOND-NEXT:           ],
// SECOND-NEXT:       },
// SECOND-NEXT:   )
// SECOND-NEXT: concrete:
// SECOND-NEXT: decl[0]: Declaration {
// SECOND-NEXT:       declaration: Declaration {
// SECOND-NEXT:           specifiers: DeclarationSpecifiers {
// SECOND-NEXT:               ty: Integer(
// SECOND-NEXT:                   Ranked {
// SECOND-NEXT:                       rank: Int,
// SECOND-NEXT:                       signed: true,
// SECOND-NEXT:                   },
// SECOND-NEXT:               ),
// SECOND-NEXT:           },
// SECOND-NEXT:           declarator: Name(
// SECOND-NEXT:               "second",
// SECOND-NEXT:           ),
// SECOND-NEXT:       },
// SECOND-NEXT:       provenance: Provenance {
// SECOND-NEXT:           file: FileId(
// SECOND-NEXT:               2,
// SECOND-NEXT:           ),
// SECOND-NEXT:           kind: User,
// SECOND-NEXT:           line: 3,
// SECOND-NEXT:       },
// SECOND-NEXT:   }
// SLATE-FILECHECK-END SECOND
// SLATE-FILECHECK-BEGIN THIRD
// THIRD: polyvariant:
// THIRD-NEXT: decl[0]: Conditional(
// THIRD-NEXT:       Conditional {
// THIRD-NEXT:           branches: [
// THIRD-NEXT:               (
// THIRD-NEXT:                   Defined(
// THIRD-NEXT:                       "FIRST",
// THIRD-NEXT:                   ),
// THIRD-NEXT:                   [
// THIRD-NEXT:                       Declaration {
// THIRD-NEXT:                           declaration: Declaration {
// THIRD-NEXT:                               specifiers: DeclarationSpecifiers {
// THIRD-NEXT:                                   ty: Integer(
// THIRD-NEXT:                                       Ranked {
// THIRD-NEXT:                                           rank: Int,
// THIRD-NEXT:                                           signed: true,
// THIRD-NEXT:                                       },
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               },
// THIRD-NEXT:                               declarator: Name(
// THIRD-NEXT:                                   "first",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           },
// THIRD-NEXT:                           provenance: Provenance {
// THIRD-NEXT:                               file: FileId(
// THIRD-NEXT:                                   2,
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               kind: User,
// THIRD-NEXT:                               line: 1,
// THIRD-NEXT:                           },
// THIRD-NEXT:                       },
// THIRD-NEXT:                   ],
// THIRD-NEXT:               ),
// THIRD-NEXT:               (
// THIRD-NEXT:                   And(
// THIRD-NEXT:                       Not(
// THIRD-NEXT:                           Defined(
// THIRD-NEXT:                               "FIRST",
// THIRD-NEXT:                           ),
// THIRD-NEXT:                       ),
// THIRD-NEXT:                       Defined(
// THIRD-NEXT:                           "SECOND",
// THIRD-NEXT:                       ),
// THIRD-NEXT:                   ),
// THIRD-NEXT:                   [
// THIRD-NEXT:                       Declaration {
// THIRD-NEXT:                           declaration: Declaration {
// THIRD-NEXT:                               specifiers: DeclarationSpecifiers {
// THIRD-NEXT:                                   ty: Integer(
// THIRD-NEXT:                                       Ranked {
// THIRD-NEXT:                                           rank: Int,
// THIRD-NEXT:                                           signed: true,
// THIRD-NEXT:                                       },
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               },
// THIRD-NEXT:                               declarator: Name(
// THIRD-NEXT:                                   "second",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           },
// THIRD-NEXT:                           provenance: Provenance {
// THIRD-NEXT:                               file: FileId(
// THIRD-NEXT:                                   2,
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               kind: User,
// THIRD-NEXT:                               line: 3,
// THIRD-NEXT:                           },
// THIRD-NEXT:                       },
// THIRD-NEXT:                   ],
// THIRD-NEXT:               ),
// THIRD-NEXT:               (
// THIRD-NEXT:                   And(
// THIRD-NEXT:                       Not(
// THIRD-NEXT:                           Or(
// THIRD-NEXT:                               Defined(
// THIRD-NEXT:                                   "FIRST",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               And(
// THIRD-NEXT:                                   Not(
// THIRD-NEXT:                                       Defined(
// THIRD-NEXT:                                           "FIRST",
// THIRD-NEXT:                                       ),
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                                   Defined(
// THIRD-NEXT:                                       "SECOND",
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           ),
// THIRD-NEXT:                       ),
// THIRD-NEXT:                       Not(
// THIRD-NEXT:                           Defined(
// THIRD-NEXT:                               "THIRD",
// THIRD-NEXT:                           ),
// THIRD-NEXT:                       ),
// THIRD-NEXT:                   ),
// THIRD-NEXT:                   [
// THIRD-NEXT:                       Declaration {
// THIRD-NEXT:                           declaration: Declaration {
// THIRD-NEXT:                               specifiers: DeclarationSpecifiers {
// THIRD-NEXT:                                   ty: Integer(
// THIRD-NEXT:                                       Ranked {
// THIRD-NEXT:                                           rank: Int,
// THIRD-NEXT:                                           signed: true,
// THIRD-NEXT:                                       },
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               },
// THIRD-NEXT:                               declarator: Name(
// THIRD-NEXT:                                   "not_third",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           },
// THIRD-NEXT:                           provenance: Provenance {
// THIRD-NEXT:                               file: FileId(
// THIRD-NEXT:                                   2,
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               kind: User,
// THIRD-NEXT:                               line: 5,
// THIRD-NEXT:                           },
// THIRD-NEXT:                       },
// THIRD-NEXT:                   ],
// THIRD-NEXT:               ),
// THIRD-NEXT:               (
// THIRD-NEXT:                   Not(
// THIRD-NEXT:                       Or(
// THIRD-NEXT:                           Or(
// THIRD-NEXT:                               Defined(
// THIRD-NEXT:                                   "FIRST",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               And(
// THIRD-NEXT:                                   Not(
// THIRD-NEXT:                                       Defined(
// THIRD-NEXT:                                           "FIRST",
// THIRD-NEXT:                                       ),
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                                   Defined(
// THIRD-NEXT:                                       "SECOND",
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           ),
// THIRD-NEXT:                           And(
// THIRD-NEXT:                               Not(
// THIRD-NEXT:                                   Or(
// THIRD-NEXT:                                       Defined(
// THIRD-NEXT:                                           "FIRST",
// THIRD-NEXT:                                       ),
// THIRD-NEXT:                                       And(
// THIRD-NEXT:                                           Not(
// THIRD-NEXT:                                               Defined(
// THIRD-NEXT:                                                   "FIRST",
// THIRD-NEXT:                                               ),
// THIRD-NEXT:                                           ),
// THIRD-NEXT:                                           Defined(
// THIRD-NEXT:                                               "SECOND",
// THIRD-NEXT:                                           ),
// THIRD-NEXT:                                       ),
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               Not(
// THIRD-NEXT:                                   Defined(
// THIRD-NEXT:                                       "THIRD",
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           ),
// THIRD-NEXT:                       ),
// THIRD-NEXT:                   ),
// THIRD-NEXT:                   [
// THIRD-NEXT:                       Declaration {
// THIRD-NEXT:                           declaration: Declaration {
// THIRD-NEXT:                               specifiers: DeclarationSpecifiers {
// THIRD-NEXT:                                   ty: Integer(
// THIRD-NEXT:                                       Ranked {
// THIRD-NEXT:                                           rank: Int,
// THIRD-NEXT:                                           signed: true,
// THIRD-NEXT:                                       },
// THIRD-NEXT:                                   ),
// THIRD-NEXT:                               },
// THIRD-NEXT:                               declarator: Name(
// THIRD-NEXT:                                   "fallback",
// THIRD-NEXT:                               ),
// THIRD-NEXT:                           },
// THIRD-NEXT:                           provenance: Provenance {
// THIRD-NEXT:                               file: FileId(
// THIRD-NEXT:                                   2,
// THIRD-NEXT:                               ),
// THIRD-NEXT:                               kind: User,
// THIRD-NEXT:                               line: 7,
// THIRD-NEXT:                           },
// THIRD-NEXT:                       },
// THIRD-NEXT:                   ],
// THIRD-NEXT:               ),
// THIRD-NEXT:           ],
// THIRD-NEXT:       },
// THIRD-NEXT:   )
// THIRD-NEXT: concrete:
// THIRD-NEXT: decl[0]: Declaration {
// THIRD-NEXT:       declaration: Declaration {
// THIRD-NEXT:           specifiers: DeclarationSpecifiers {
// THIRD-NEXT:               ty: Integer(
// THIRD-NEXT:                   Ranked {
// THIRD-NEXT:                       rank: Int,
// THIRD-NEXT:                       signed: true,
// THIRD-NEXT:                   },
// THIRD-NEXT:               ),
// THIRD-NEXT:           },
// THIRD-NEXT:           declarator: Name(
// THIRD-NEXT:               "fallback",
// THIRD-NEXT:           ),
// THIRD-NEXT:       },
// THIRD-NEXT:       provenance: Provenance {
// THIRD-NEXT:           file: FileId(
// THIRD-NEXT:               2,
// THIRD-NEXT:           ),
// THIRD-NEXT:           kind: User,
// THIRD-NEXT:           line: 7,
// THIRD-NEXT:       },
// THIRD-NEXT:   }
// SLATE-FILECHECK-END THIRD
