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
// DEFAULT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "not_third",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FIRST
// FIRST: decl[{{[0-9]+}}]: Declaration(
// FIRST-NEXT:       Declaration {
// FIRST-NEXT:           specifiers: DeclarationSpecifiers {
// FIRST-NEXT:               ty: Integer(
// FIRST-NEXT:                   Ranked {
// FIRST-NEXT:                       rank: Int,
// FIRST-NEXT:                       signed: true,
// FIRST-NEXT:                   },
// FIRST-NEXT:               ),
// FIRST-NEXT:           },
// FIRST-NEXT:           declarators: [
// FIRST-NEXT:               InitDeclaratorKind {
// FIRST-NEXT:                   declarator: Name(
// FIRST-NEXT:                       "first",
// FIRST-NEXT:                   ),
// FIRST-NEXT:               },
// FIRST-NEXT:           ],
// FIRST-NEXT:       },
// FIRST-NEXT:   )
// SLATE-FILECHECK-END FIRST
// SLATE-FILECHECK-BEGIN SECOND
// SECOND: decl[{{[0-9]+}}]: Declaration(
// SECOND-NEXT:       Declaration {
// SECOND-NEXT:           specifiers: DeclarationSpecifiers {
// SECOND-NEXT:               ty: Integer(
// SECOND-NEXT:                   Ranked {
// SECOND-NEXT:                       rank: Int,
// SECOND-NEXT:                       signed: true,
// SECOND-NEXT:                   },
// SECOND-NEXT:               ),
// SECOND-NEXT:           },
// SECOND-NEXT:           declarators: [
// SECOND-NEXT:               InitDeclaratorKind {
// SECOND-NEXT:                   declarator: Name(
// SECOND-NEXT:                       "second",
// SECOND-NEXT:                   ),
// SECOND-NEXT:               },
// SECOND-NEXT:           ],
// SECOND-NEXT:       },
// SECOND-NEXT:   )
// SLATE-FILECHECK-END SECOND
// SLATE-FILECHECK-BEGIN THIRD
// THIRD: decl[{{[0-9]+}}]: Declaration(
// THIRD-NEXT:       Declaration {
// THIRD-NEXT:           specifiers: DeclarationSpecifiers {
// THIRD-NEXT:               ty: Integer(
// THIRD-NEXT:                   Ranked {
// THIRD-NEXT:                       rank: Int,
// THIRD-NEXT:                       signed: true,
// THIRD-NEXT:                   },
// THIRD-NEXT:               ),
// THIRD-NEXT:           },
// THIRD-NEXT:           declarators: [
// THIRD-NEXT:               InitDeclaratorKind {
// THIRD-NEXT:                   declarator: Name(
// THIRD-NEXT:                       "fallback",
// THIRD-NEXT:                   ),
// THIRD-NEXT:               },
// THIRD-NEXT:           ],
// THIRD-NEXT:       },
// THIRD-NEXT:   )
// SLATE-FILECHECK-END THIRD
