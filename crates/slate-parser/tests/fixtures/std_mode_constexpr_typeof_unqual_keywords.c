int subject;
#ifdef C23_IS_KEYWORD
constexpr int constexpr_value = 5;
typeof_unqual(subject) typeof_unqual_derived;
#else
int constexpr = 5;
int typeof_unqual;
#endif
__typeof_unqual(subject) always_typeof_unqual_derived;

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23 C23_IS_KEYWORD
// SLATE-FILECHECK-STD C23 c23

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
// C17-NEXT:                       "constexpr",
// C17-NEXT:                   ),
// C17-NEXT:                   initializer: Some(
// C17-NEXT:                       Expr(
// C17-NEXT:                           IntegerLiteral(
// C17-NEXT:                               IntegerLiteral {
// C17-NEXT:                                   value: 5,
// C17-NEXT:                                   radix: Decimal,
// C17-NEXT:                                   suffix: IntegerSuffix {
// C17-NEXT:                                       unsigned: false,
// C17-NEXT:                                       size: None,
// C17-NEXT:                                   },
// C17-NEXT:                                   spelling: "5",
// C17-NEXT:                               },
// C17-NEXT:                           ),
// C17-NEXT:                       ),
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
// C17-NEXT:                       "typeof_unqual",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// C17-NEXT: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: TypeOfUnqual(
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
// C17-NEXT:                       "always_typeof_unqual_derived",
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
// GNU17-NEXT:                       "constexpr",
// GNU17-NEXT:                   ),
// GNU17-NEXT:                   initializer: Some(
// GNU17-NEXT:                       Expr(
// GNU17-NEXT:                           IntegerLiteral(
// GNU17-NEXT:                               IntegerLiteral {
// GNU17-NEXT:                                   value: 5,
// GNU17-NEXT:                                   radix: Decimal,
// GNU17-NEXT:                                   suffix: IntegerSuffix {
// GNU17-NEXT:                                       unsigned: false,
// GNU17-NEXT:                                       size: None,
// GNU17-NEXT:                                   },
// GNU17-NEXT:                                   spelling: "5",
// GNU17-NEXT:                               },
// GNU17-NEXT:                           ),
// GNU17-NEXT:                       ),
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// GNU17-NEXT:                       "typeof_unqual",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: TypeOfUnqual(
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
// GNU17-NEXT:                       "always_typeof_unqual_derived",
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
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               is_constexpr: true,
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "constexpr_value",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           IntegerLiteral(
// C23-NEXT:                               IntegerLiteral {
// C23-NEXT:                                   value: 5,
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: IntegerSuffix {
// C23-NEXT:                                       unsigned: false,
// C23-NEXT:                                       size: None,
// C23-NEXT:                                   },
// C23-NEXT:                                   spelling: "5",
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: TypeOfUnqual(
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
// C23-NEXT:                       "typeof_unqual_derived",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: TypeOfUnqual(
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
// C23-NEXT:                       "always_typeof_unqual_derived",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
