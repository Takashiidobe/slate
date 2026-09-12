#define VERSION 3
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PRESENT
#define ALIAS UNDEFINED_NAME
#if VERSION >= 2
int version_ok;
#endif
#if MAX(1, VERSION) == 3
int max_ok;
#endif
#if defined PRESENT && defined(ALIAS)
int defined_ok;
#endif
#ifdef WIDE
#define WIDTH 64
#else
#define WIDTH 32
#endif
#if WIDTH == 64
int wide;
#else
int narrow;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIDE WIDE

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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "version_ok",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
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
// DEFAULT-NEXT:               "max_ok",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
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
// DEFAULT-NEXT:               "defined_ok",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 11,
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "narrow",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 21,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIDE
// WIDE: decl[0]: Declaration {
// WIDE-NEXT:       declaration: Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarator: Name(
// WIDE-NEXT:               "version_ok",
// WIDE-NEXT:           ),
// WIDE-NEXT:       },
// WIDE-NEXT:       provenance: Provenance {
// WIDE-NEXT:           file: FileId(
// WIDE-NEXT:               3,
// WIDE-NEXT:           ),
// WIDE-NEXT:           kind: User,
// WIDE-NEXT:           line: 5,
// WIDE-NEXT:       },
// WIDE-NEXT:   }
// WIDE-NEXT: decl[1]: Declaration {
// WIDE-NEXT:       declaration: Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarator: Name(
// WIDE-NEXT:               "max_ok",
// WIDE-NEXT:           ),
// WIDE-NEXT:       },
// WIDE-NEXT:       provenance: Provenance {
// WIDE-NEXT:           file: FileId(
// WIDE-NEXT:               3,
// WIDE-NEXT:           ),
// WIDE-NEXT:           kind: User,
// WIDE-NEXT:           line: 8,
// WIDE-NEXT:       },
// WIDE-NEXT:   }
// WIDE-NEXT: decl[2]: Declaration {
// WIDE-NEXT:       declaration: Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarator: Name(
// WIDE-NEXT:               "defined_ok",
// WIDE-NEXT:           ),
// WIDE-NEXT:       },
// WIDE-NEXT:       provenance: Provenance {
// WIDE-NEXT:           file: FileId(
// WIDE-NEXT:               3,
// WIDE-NEXT:           ),
// WIDE-NEXT:           kind: User,
// WIDE-NEXT:           line: 11,
// WIDE-NEXT:       },
// WIDE-NEXT:   }
// WIDE-NEXT: decl[3]: Declaration {
// WIDE-NEXT:       declaration: Declaration {
// WIDE-NEXT:           specifiers: DeclarationSpecifiers {
// WIDE-NEXT:               ty: Integer(
// WIDE-NEXT:                   Ranked {
// WIDE-NEXT:                       rank: Int,
// WIDE-NEXT:                       signed: true,
// WIDE-NEXT:                   },
// WIDE-NEXT:               ),
// WIDE-NEXT:           },
// WIDE-NEXT:           declarator: Name(
// WIDE-NEXT:               "wide",
// WIDE-NEXT:           ),
// WIDE-NEXT:       },
// WIDE-NEXT:       provenance: Provenance {
// WIDE-NEXT:           file: FileId(
// WIDE-NEXT:               3,
// WIDE-NEXT:           ),
// WIDE-NEXT:           kind: User,
// WIDE-NEXT:           line: 19,
// WIDE-NEXT:       },
// WIDE-NEXT:   }
// SLATE-FILECHECK-END WIDE
