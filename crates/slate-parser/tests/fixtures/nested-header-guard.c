#ifndef OUTER_GUARD
#define OUTER_GUARD

#ifndef INNER_GUARD
#define INNER_GUARD
#define NESTED_VALUE 1
#endif

#ifdef SOME_FEATURE
#define FEATURE_VALUE 1
#else
#define FEATURE_VALUE 2
#endif

#endif

int nested = NESTED_VALUE;
int feature = FEATURE_VALUE;

#ifndef LEVEL_A
#define LEVEL_A
#ifndef LEVEL_B
#define LEVEL_B
#ifndef LEVEL_C
#define LEVEL_C
#define TRIPLE_NESTED 3
#endif
#endif
#endif

int triple = TRIPLE_NESTED;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES FEATURE SOME_FEATURE

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
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "nested",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 16,
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
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "feature",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "triple",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 30,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FEATURE
// FEATURE: decl[0]: Declaration {
// FEATURE-NEXT:       declaration: Declaration {
// FEATURE-NEXT:           specifiers: DeclarationSpecifiers {
// FEATURE-NEXT:               ty: Integer(
// FEATURE-NEXT:                   Ranked {
// FEATURE-NEXT:                       rank: Int,
// FEATURE-NEXT:                       signed: true,
// FEATURE-NEXT:                   },
// FEATURE-NEXT:               ),
// FEATURE-NEXT:           },
// FEATURE-NEXT:           declarators: [
// FEATURE-NEXT:               InitDeclarator {
// FEATURE-NEXT:                   declarator: Name(
// FEATURE-NEXT:                       "nested",
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:                   initializer: Some(
// FEATURE-NEXT:                       Expr(
// FEATURE-NEXT:                           Integer(
// FEATURE-NEXT:                               1,
// FEATURE-NEXT:                           ),
// FEATURE-NEXT:                       ),
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:               },
// FEATURE-NEXT:           ],
// FEATURE-NEXT:       },
// FEATURE-NEXT:       provenance: Provenance {
// FEATURE-NEXT:           file: FileId(
// FEATURE-NEXT:               3,
// FEATURE-NEXT:           ),
// FEATURE-NEXT:           kind: User,
// FEATURE-NEXT:           line: 16,
// FEATURE-NEXT:           header: None,
// FEATURE-NEXT:       },
// FEATURE-NEXT:   }
// FEATURE-NEXT: decl[1]: Declaration {
// FEATURE-NEXT:       declaration: Declaration {
// FEATURE-NEXT:           specifiers: DeclarationSpecifiers {
// FEATURE-NEXT:               ty: Integer(
// FEATURE-NEXT:                   Ranked {
// FEATURE-NEXT:                       rank: Int,
// FEATURE-NEXT:                       signed: true,
// FEATURE-NEXT:                   },
// FEATURE-NEXT:               ),
// FEATURE-NEXT:           },
// FEATURE-NEXT:           declarators: [
// FEATURE-NEXT:               InitDeclarator {
// FEATURE-NEXT:                   declarator: Name(
// FEATURE-NEXT:                       "feature",
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:                   initializer: Some(
// FEATURE-NEXT:                       Expr(
// FEATURE-NEXT:                           Integer(
// FEATURE-NEXT:                               1,
// FEATURE-NEXT:                           ),
// FEATURE-NEXT:                       ),
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:               },
// FEATURE-NEXT:           ],
// FEATURE-NEXT:       },
// FEATURE-NEXT:       provenance: Provenance {
// FEATURE-NEXT:           file: FileId(
// FEATURE-NEXT:               3,
// FEATURE-NEXT:           ),
// FEATURE-NEXT:           kind: User,
// FEATURE-NEXT:           line: 17,
// FEATURE-NEXT:           header: None,
// FEATURE-NEXT:       },
// FEATURE-NEXT:   }
// FEATURE-NEXT: decl[2]: Declaration {
// FEATURE-NEXT:       declaration: Declaration {
// FEATURE-NEXT:           specifiers: DeclarationSpecifiers {
// FEATURE-NEXT:               ty: Integer(
// FEATURE-NEXT:                   Ranked {
// FEATURE-NEXT:                       rank: Int,
// FEATURE-NEXT:                       signed: true,
// FEATURE-NEXT:                   },
// FEATURE-NEXT:               ),
// FEATURE-NEXT:           },
// FEATURE-NEXT:           declarators: [
// FEATURE-NEXT:               InitDeclarator {
// FEATURE-NEXT:                   declarator: Name(
// FEATURE-NEXT:                       "triple",
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:                   initializer: Some(
// FEATURE-NEXT:                       Expr(
// FEATURE-NEXT:                           Integer(
// FEATURE-NEXT:                               3,
// FEATURE-NEXT:                           ),
// FEATURE-NEXT:                       ),
// FEATURE-NEXT:                   ),
// FEATURE-NEXT:               },
// FEATURE-NEXT:           ],
// FEATURE-NEXT:       },
// FEATURE-NEXT:       provenance: Provenance {
// FEATURE-NEXT:           file: FileId(
// FEATURE-NEXT:               3,
// FEATURE-NEXT:           ),
// FEATURE-NEXT:           kind: User,
// FEATURE-NEXT:           line: 30,
// FEATURE-NEXT:           header: None,
// FEATURE-NEXT:       },
// FEATURE-NEXT:   }
// SLATE-FILECHECK-END FEATURE
