int subject;
#ifdef TYPEOF_IS_KEYWORD
typeof(subject) typeof_derived;
#else
int typeof;
#endif
__typeof(subject) always_typeof_derived;

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89 TYPEOF_IS_KEYWORD
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17 TYPEOF_IS_KEYWORD
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23 TYPEOF_IS_KEYWORD
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
// C89-NEXT:                       "subject",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// C89-NEXT:                       "typeof",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: TypeOf(
// C89-NEXT:                   Expression(
// C89-NEXT:                       Identifier(
// C89-NEXT:                           "subject",
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "always_typeof_derived",
// C89-NEXT:                   ),
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
// GNU89-NEXT:                       "subject",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: TypeOf(
// GNU89-NEXT:                   Expression(
// GNU89-NEXT:                       Identifier(
// GNU89-NEXT:                           "subject",
// GNU89-NEXT:                       ),
// GNU89-NEXT:                   ),
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "typeof_derived",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: TypeOf(
// GNU89-NEXT:                   Expression(
// GNU89-NEXT:                       Identifier(
// GNU89-NEXT:                           "subject",
// GNU89-NEXT:                       ),
// GNU89-NEXT:                   ),
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "always_typeof_derived",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// SLATE-FILECHECK-END GNU89
// SLATE-FILECHECK-BEGIN C17
// C17: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: Integer(
// C17-NEXT:                   Ranked {
// C17-NEXT:                       rank: Int,
// C17-NEXT:                       signed: true,
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclaratorKind {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "subject",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// C17-NEXT: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: Integer(
// C17-NEXT:                   Ranked {
// C17-NEXT:                       rank: Int,
// C17-NEXT:                       signed: true,
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclaratorKind {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "typeof",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// C17-NEXT: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: TypeOf(
// C17-NEXT:                   Expression(
// C17-NEXT:                       Identifier(
// C17-NEXT:                           "subject",
// C17-NEXT:                       ),
// C17-NEXT:                   ),
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclaratorKind {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "always_typeof_derived",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: Integer(
// GNU17-NEXT:                   Ranked {
// GNU17-NEXT:                       rank: Int,
// GNU17-NEXT:                       signed: true,
// GNU17-NEXT:                   },
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "subject",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: TypeOf(
// GNU17-NEXT:                   Expression(
// GNU17-NEXT:                       Identifier(
// GNU17-NEXT:                           "subject",
// GNU17-NEXT:                       ),
// GNU17-NEXT:                   ),
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "typeof_derived",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: TypeOf(
// GNU17-NEXT:                   Expression(
// GNU17-NEXT:                       Identifier(
// GNU17-NEXT:                           "subject",
// GNU17-NEXT:                       ),
// GNU17-NEXT:                   ),
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "always_typeof_derived",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// SLATE-FILECHECK-END GNU17
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
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "subject",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: TypeOf(
// C23-NEXT:                   Expression(
// C23-NEXT:                       Identifier(
// C23-NEXT:                           "subject",
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "typeof_derived",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: TypeOf(
// C23-NEXT:                   Expression(
// C23-NEXT:                       Identifier(
// C23-NEXT:                           "subject",
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "always_typeof_derived",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
