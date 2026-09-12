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
// DEFAULT: decl[0]: Declaration {
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
// DEFAULT-NEXT:                   "popped_in_branch",
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
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
// DEFAULT-NEXT:                   "unreachable_pop",
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
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
// DEFAULT-NEXT:                   "partial_push",
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
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
// DEFAULT-NEXT:                   "second_pop",
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 26,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: decl[0]: Declaration {
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
// A-NEXT:                   "popped_in_branch",
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 7,
// A-NEXT:           header: None,
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
// A-NEXT:                   "unreachable_pop",
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 17,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[2]: Declaration {
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
// A-NEXT:                   "partial_push",
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 24,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[3]: Declaration {
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
// A-NEXT:                   "second_pop",
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 26,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
