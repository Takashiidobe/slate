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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "x_undefined",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "x_missing",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
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
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclarator {
// A-NEXT:                   declarator: Array {
// A-NEXT:                       inner: Name(
// A-NEXT:                           "x_defined",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntLit(
// A-NEXT:                               1,
// A-NEXT:                           ),
// A-NEXT:                       ),
// A-NEXT:                   },
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 4,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN FLAG
// FLAG: decl[0]: Declaration {
// FLAG-NEXT:       declaration: Declaration {
// FLAG-NEXT:           specifiers: DeclarationSpecifiers {
// FLAG-NEXT:               ty: Integer(
// FLAG-NEXT:                   Ranked {
// FLAG-NEXT:                       rank: Int,
// FLAG-NEXT:                       signed: true,
// FLAG-NEXT:                   },
// FLAG-NEXT:               ),
// FLAG-NEXT:           },
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclarator {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "x_undefined",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntLit(
// FLAG-NEXT:                               2,
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               3,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 6,
// FLAG-NEXT:           header: None,
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
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclarator {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "flag_without_x",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntLit(
// FLAG-NEXT:                               3,
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               3,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 9,
// FLAG-NEXT:           header: None,
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
// FLAG-NEXT:           declarators: [
// FLAG-NEXT:               InitDeclarator {
// FLAG-NEXT:                   declarator: Array {
// FLAG-NEXT:                       inner: Name(
// FLAG-NEXT:                           "x_missing",
// FLAG-NEXT:                       ),
// FLAG-NEXT:                       size: Expression(
// FLAG-NEXT:                           IntLit(
// FLAG-NEXT:                               4,
// FLAG-NEXT:                           ),
// FLAG-NEXT:                       ),
// FLAG-NEXT:                   },
// FLAG-NEXT:               },
// FLAG-NEXT:           ],
// FLAG-NEXT:       },
// FLAG-NEXT:       provenance: Provenance {
// FLAG-NEXT:           file: FileId(
// FLAG-NEXT:               3,
// FLAG-NEXT:           ),
// FLAG-NEXT:           kind: User,
// FLAG-NEXT:           line: 12,
// FLAG-NEXT:           header: None,
// FLAG-NEXT:       },
// FLAG-NEXT:   }
// SLATE-FILECHECK-END FLAG
// SLATE-FILECHECK-BEGIN X
// X: decl[0]: Declaration {
// X-NEXT:       declaration: Declaration {
// X-NEXT:           specifiers: DeclarationSpecifiers {
// X-NEXT:               ty: Integer(
// X-NEXT:                   Ranked {
// X-NEXT:                       rank: Int,
// X-NEXT:                       signed: true,
// X-NEXT:                   },
// X-NEXT:               ),
// X-NEXT:           },
// X-NEXT:           declarators: [
// X-NEXT:               InitDeclarator {
// X-NEXT:                   declarator: Array {
// X-NEXT:                       inner: Name(
// X-NEXT:                           "x_defined",
// X-NEXT:                       ),
// X-NEXT:                       size: Expression(
// X-NEXT:                           IntLit(
// X-NEXT:                               1,
// X-NEXT:                           ),
// X-NEXT:                       ),
// X-NEXT:                   },
// X-NEXT:               },
// X-NEXT:           ],
// X-NEXT:       },
// X-NEXT:       provenance: Provenance {
// X-NEXT:           file: FileId(
// X-NEXT:               3,
// X-NEXT:           ),
// X-NEXT:           kind: User,
// X-NEXT:           line: 4,
// X-NEXT:           header: None,
// X-NEXT:       },
// X-NEXT:   }
// SLATE-FILECHECK-END X
