#ifdef RESTRICT_IS_KEYWORD
int *restrict restrict_qualified;
#else
int restrict;
#endif
char *__restrict always_restrict;

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C99 RESTRICT_IS_KEYWORD
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C23 RESTRICT_IS_KEYWORD
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "restrict",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Char {
// C89-NEXT:                       signed: None,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Pointer {
// C89-NEXT:                       qualifiers: Qualifiers {
// C89-NEXT:                           is_restrict: true,
// C89-NEXT:                       },
// C89-NEXT:                       inner: Name(
// C89-NEXT:                           "always_restrict",
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "restrict",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Char {
// GNU89-NEXT:                       signed: None,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Pointer {
// GNU89-NEXT:                       qualifiers: Qualifiers {
// GNU89-NEXT:                           is_restrict: true,
// GNU89-NEXT:                       },
// GNU89-NEXT:                       inner: Name(
// GNU89-NEXT:                           "always_restrict",
// GNU89-NEXT:                       ),
// GNU89-NEXT:                   },
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// SLATE-FILECHECK-END GNU89
// SLATE-FILECHECK-BEGIN C99
// C99: decl[{{[0-9]+}}]: Declaration(
// C99-NEXT:       Declaration {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarators: [
// C99-NEXT:               InitDeclaratorKind {
// C99-NEXT:                   declarator: Pointer {
// C99-NEXT:                       qualifiers: Qualifiers {
// C99-NEXT:                           is_restrict: true,
// C99-NEXT:                       },
// C99-NEXT:                       inner: Name(
// C99-NEXT:                           "restrict_qualified",
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[{{[0-9]+}}]: Declaration(
// C99-NEXT:       Declaration {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Char {
// C99-NEXT:                       signed: None,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarators: [
// C99-NEXT:               InitDeclaratorKind {
// C99-NEXT:                   declarator: Pointer {
// C99-NEXT:                       qualifiers: Qualifiers {
// C99-NEXT:                           is_restrict: true,
// C99-NEXT:                       },
// C99-NEXT:                       inner: Name(
// C99-NEXT:                           "always_restrict",
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C23
// C23: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Pointer {
// C23-NEXT:                       qualifiers: Qualifiers {
// C23-NEXT:                           is_restrict: true,
// C23-NEXT:                       },
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "restrict_qualified",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Char {
// C23-NEXT:                       signed: None,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Pointer {
// C23-NEXT:                       qualifiers: Qualifiers {
// C23-NEXT:                           is_restrict: true,
// C23-NEXT:                       },
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "always_restrict",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
