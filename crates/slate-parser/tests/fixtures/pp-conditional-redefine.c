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
// DEFAULT-NEXT:                           "redefined",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               1,
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
// DEFAULT-NEXT:           line: 5,
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
// DEFAULT-NEXT:                           "nested",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               1,
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
// A-NEXT:                           "redefined",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntLit(
// A-NEXT:                               2,
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
// A-NEXT:           line: 5,
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
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclarator {
// A-NEXT:                   declarator: Array {
// A-NEXT:                       inner: Name(
// A-NEXT:                           "nested",
// A-NEXT:                       ),
// A-NEXT:                       size: Expression(
// A-NEXT:                           IntLit(
// A-NEXT:                               2,
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
// A-NEXT:           line: 12,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN B
// B: decl[0]: Declaration {
// B-NEXT:       declaration: Declaration {
// B-NEXT:           specifiers: DeclarationSpecifiers {
// B-NEXT:               ty: Integer(
// B-NEXT:                   Ranked {
// B-NEXT:                       rank: Int,
// B-NEXT:                       signed: true,
// B-NEXT:                   },
// B-NEXT:               ),
// B-NEXT:           },
// B-NEXT:           declarators: [
// B-NEXT:               InitDeclarator {
// B-NEXT:                   declarator: Array {
// B-NEXT:                       inner: Name(
// B-NEXT:                           "redefined",
// B-NEXT:                       ),
// B-NEXT:                       size: Expression(
// B-NEXT:                           IntLit(
// B-NEXT:                               1,
// B-NEXT:                           ),
// B-NEXT:                       ),
// B-NEXT:                   },
// B-NEXT:               },
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:       provenance: Provenance {
// B-NEXT:           file: FileId(
// B-NEXT:               3,
// B-NEXT:           ),
// B-NEXT:           kind: User,
// B-NEXT:           line: 5,
// B-NEXT:           header: None,
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
// B-NEXT:           declarators: [
// B-NEXT:               InitDeclarator {
// B-NEXT:                   declarator: Array {
// B-NEXT:                       inner: Name(
// B-NEXT:                           "nested",
// B-NEXT:                       ),
// B-NEXT:                       size: Expression(
// B-NEXT:                           IntLit(
// B-NEXT:                               1,
// B-NEXT:                           ),
// B-NEXT:                       ),
// B-NEXT:                   },
// B-NEXT:               },
// B-NEXT:           ],
// B-NEXT:       },
// B-NEXT:       provenance: Provenance {
// B-NEXT:           file: FileId(
// B-NEXT:               3,
// B-NEXT:           ),
// B-NEXT:           kind: User,
// B-NEXT:           line: 12,
// B-NEXT:           header: None,
// B-NEXT:       },
// B-NEXT:   }
// SLATE-FILECHECK-END B
// SLATE-FILECHECK-BEGIN AB
// AB: decl[0]: Declaration {
// AB-NEXT:       declaration: Declaration {
// AB-NEXT:           specifiers: DeclarationSpecifiers {
// AB-NEXT:               ty: Integer(
// AB-NEXT:                   Ranked {
// AB-NEXT:                       rank: Int,
// AB-NEXT:                       signed: true,
// AB-NEXT:                   },
// AB-NEXT:               ),
// AB-NEXT:           },
// AB-NEXT:           declarators: [
// AB-NEXT:               InitDeclarator {
// AB-NEXT:                   declarator: Array {
// AB-NEXT:                       inner: Name(
// AB-NEXT:                           "redefined",
// AB-NEXT:                       ),
// AB-NEXT:                       size: Expression(
// AB-NEXT:                           IntLit(
// AB-NEXT:                               2,
// AB-NEXT:                           ),
// AB-NEXT:                       ),
// AB-NEXT:                   },
// AB-NEXT:               },
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:       provenance: Provenance {
// AB-NEXT:           file: FileId(
// AB-NEXT:               3,
// AB-NEXT:           ),
// AB-NEXT:           kind: User,
// AB-NEXT:           line: 5,
// AB-NEXT:           header: None,
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
// AB-NEXT:           declarators: [
// AB-NEXT:               InitDeclarator {
// AB-NEXT:                   declarator: Array {
// AB-NEXT:                       inner: Name(
// AB-NEXT:                           "nested",
// AB-NEXT:                       ),
// AB-NEXT:                       size: Expression(
// AB-NEXT:                           IntLit(
// AB-NEXT:                               5,
// AB-NEXT:                           ),
// AB-NEXT:                       ),
// AB-NEXT:                   },
// AB-NEXT:               },
// AB-NEXT:           ],
// AB-NEXT:       },
// AB-NEXT:       provenance: Provenance {
// AB-NEXT:           file: FileId(
// AB-NEXT:               3,
// AB-NEXT:           ),
// AB-NEXT:           kind: User,
// AB-NEXT:           line: 12,
// AB-NEXT:           header: None,
// AB-NEXT:       },
// AB-NEXT:   }
// SLATE-FILECHECK-END AB
