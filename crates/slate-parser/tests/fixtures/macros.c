#define VALUE 7
#define TRANSITIVE VALUE
#define ID(x) x
#define JOIN(a, b) a ## b

int ordinary = TRANSITIVE;

#define SELF SELF
int recursive __attribute__((slate_literal(SELF)));

#ifdef SELECT
#define CHOICE 1
int selected __attribute__((slate_literal(CHOICE)));
#else
#define CHOICE 2
int selected __attribute__((slate_literal(CHOICE)));
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES SELECT SELECT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
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
// DEFAULT-NEXT:               "ordinary",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           7,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "recursive",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Unknown {
// DEFAULT-NEXT:                   name: "slate_literal",
// DEFAULT-NEXT:                   arguments: [
// DEFAULT-NEXT:                       "SELF",
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "SELECT",
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
// DEFAULT-NEXT:                                   "selected",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               attributes: [
// DEFAULT-NEXT:                                   Unknown {
// DEFAULT-NEXT:                                       name: "slate_literal",
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           "1",
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
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
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "SELECT",
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
// DEFAULT-NEXT:                                   "selected",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               attributes: [
// DEFAULT-NEXT:                                   Unknown {
// DEFAULT-NEXT:                                       name: "slate_literal",
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           "2",
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 15,
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
// DEFAULT-NEXT:               "ordinary",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           7,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "recursive",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Unknown {
// DEFAULT-NEXT:                   name: "slate_literal",
// DEFAULT-NEXT:                   arguments: [
// DEFAULT-NEXT:                       "SELF",
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "selected",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Unknown {
// DEFAULT-NEXT:                   name: "slate_literal",
// DEFAULT-NEXT:                   arguments: [
// DEFAULT-NEXT:                       "2",
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 15,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: polyvariant:
// SELECT-NEXT: decl[0]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Name(
// SELECT-NEXT:               "ordinary",
// SELECT-NEXT:           ),
// SELECT-NEXT:           initializer: Some(
// SELECT-NEXT:               Expr(
// SELECT-NEXT:                   Const(
// SELECT-NEXT:                       Integer(
// SELECT-NEXT:                           7,
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               ),
// SELECT-NEXT:           ),
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               2,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 5,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[1]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Name(
// SELECT-NEXT:               "recursive",
// SELECT-NEXT:           ),
// SELECT-NEXT:           attributes: [
// SELECT-NEXT:               Unknown {
// SELECT-NEXT:                   name: "slate_literal",
// SELECT-NEXT:                   arguments: [
// SELECT-NEXT:                       "SELF",
// SELECT-NEXT:                   ],
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               2,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 8,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[2]: Conditional(
// SELECT-NEXT:       Conditional {
// SELECT-NEXT:           branches: [
// SELECT-NEXT:               (
// SELECT-NEXT:                   Defined(
// SELECT-NEXT:                       "SELECT",
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   [
// SELECT-NEXT:                       Declaration {
// SELECT-NEXT:                           declaration: Declaration {
// SELECT-NEXT:                               specifiers: DeclarationSpecifiers {
// SELECT-NEXT:                                   ty: Integer(
// SELECT-NEXT:                                       Ranked {
// SELECT-NEXT:                                           rank: Int,
// SELECT-NEXT:                                           signed: true,
// SELECT-NEXT:                                       },
// SELECT-NEXT:                                   ),
// SELECT-NEXT:                               },
// SELECT-NEXT:                               declarator: Name(
// SELECT-NEXT:                                   "selected",
// SELECT-NEXT:                               ),
// SELECT-NEXT:                               attributes: [
// SELECT-NEXT:                                   Unknown {
// SELECT-NEXT:                                       name: "slate_literal",
// SELECT-NEXT:                                       arguments: [
// SELECT-NEXT:                                           "1",
// SELECT-NEXT:                                       ],
// SELECT-NEXT:                                   },
// SELECT-NEXT:                               ],
// SELECT-NEXT:                           },
// SELECT-NEXT:                           provenance: Provenance {
// SELECT-NEXT:                               file: FileId(
// SELECT-NEXT:                                   2,
// SELECT-NEXT:                               ),
// SELECT-NEXT:                               kind: User,
// SELECT-NEXT:                               line: 12,
// SELECT-NEXT:                           },
// SELECT-NEXT:                       },
// SELECT-NEXT:                   ],
// SELECT-NEXT:               ),
// SELECT-NEXT:               (
// SELECT-NEXT:                   Not(
// SELECT-NEXT:                       Defined(
// SELECT-NEXT:                           "SELECT",
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   [
// SELECT-NEXT:                       Declaration {
// SELECT-NEXT:                           declaration: Declaration {
// SELECT-NEXT:                               specifiers: DeclarationSpecifiers {
// SELECT-NEXT:                                   ty: Integer(
// SELECT-NEXT:                                       Ranked {
// SELECT-NEXT:                                           rank: Int,
// SELECT-NEXT:                                           signed: true,
// SELECT-NEXT:                                       },
// SELECT-NEXT:                                   ),
// SELECT-NEXT:                               },
// SELECT-NEXT:                               declarator: Name(
// SELECT-NEXT:                                   "selected",
// SELECT-NEXT:                               ),
// SELECT-NEXT:                               attributes: [
// SELECT-NEXT:                                   Unknown {
// SELECT-NEXT:                                       name: "slate_literal",
// SELECT-NEXT:                                       arguments: [
// SELECT-NEXT:                                           "2",
// SELECT-NEXT:                                       ],
// SELECT-NEXT:                                   },
// SELECT-NEXT:                               ],
// SELECT-NEXT:                           },
// SELECT-NEXT:                           provenance: Provenance {
// SELECT-NEXT:                               file: FileId(
// SELECT-NEXT:                                   2,
// SELECT-NEXT:                               ),
// SELECT-NEXT:                               kind: User,
// SELECT-NEXT:                               line: 15,
// SELECT-NEXT:                           },
// SELECT-NEXT:                       },
// SELECT-NEXT:                   ],
// SELECT-NEXT:               ),
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SELECT-NEXT: concrete:
// SELECT-NEXT: decl[0]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Name(
// SELECT-NEXT:               "ordinary",
// SELECT-NEXT:           ),
// SELECT-NEXT:           initializer: Some(
// SELECT-NEXT:               Expr(
// SELECT-NEXT:                   Const(
// SELECT-NEXT:                       Integer(
// SELECT-NEXT:                           7,
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               ),
// SELECT-NEXT:           ),
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               2,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 5,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[1]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Name(
// SELECT-NEXT:               "recursive",
// SELECT-NEXT:           ),
// SELECT-NEXT:           attributes: [
// SELECT-NEXT:               Unknown {
// SELECT-NEXT:                   name: "slate_literal",
// SELECT-NEXT:                   arguments: [
// SELECT-NEXT:                       "SELF",
// SELECT-NEXT:                   ],
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               2,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 8,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[2]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Name(
// SELECT-NEXT:               "selected",
// SELECT-NEXT:           ),
// SELECT-NEXT:           attributes: [
// SELECT-NEXT:               Unknown {
// SELECT-NEXT:                   name: "slate_literal",
// SELECT-NEXT:                   arguments: [
// SELECT-NEXT:                       "1",
// SELECT-NEXT:                   ],
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               2,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 12,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SLATE-FILECHECK-END SELECT
