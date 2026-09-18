#ifdef __STDC_VERSION__
int stdc_version = __STDC_VERSION__;
#else
int no_stdc_version;
#endif

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
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
// C89-NEXT:                       "no_stdc_version",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
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
// C99-NEXT:                   declarator: Name(
// C99-NEXT:                       "stdc_version",
// C99-NEXT:                   ),
// C99-NEXT:                   initializer: Some(
// C99-NEXT:                       Expr(
// C99-NEXT:                           IntegerLiteral(
// C99-NEXT:                               IntegerLiteral {
// C99-NEXT:                                   value: 199901,
// C99-NEXT:                                   radix: Decimal,
// C99-NEXT:                                   suffix: IntegerSuffix {
// C99-NEXT:                                       unsigned: false,
// C99-NEXT:                                       size: Long,
// C99-NEXT:                                   },
// C99-NEXT:                                   spelling: "199901L",
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ),
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C11
// C11: decl[{{[0-9]+}}]: Declaration(
// C11-NEXT:       Declaration {
// C11-NEXT:           specifiers: DeclarationSpecifiers {
// C11-NEXT:               ty: Integer(
// C11-NEXT:                   Ranked {
// C11-NEXT:                       rank: Int,
// C11-NEXT:                       signed: true,
// C11-NEXT:                   },
// C11-NEXT:               ),
// C11-NEXT:           },
// C11-NEXT:           declarators: [
// C11-NEXT:               InitDeclaratorKind {
// C11-NEXT:                   declarator: Name(
// C11-NEXT:                       "stdc_version",
// C11-NEXT:                   ),
// C11-NEXT:                   initializer: Some(
// C11-NEXT:                       Expr(
// C11-NEXT:                           IntegerLiteral(
// C11-NEXT:                               IntegerLiteral {
// C11-NEXT:                                   value: 201112,
// C11-NEXT:                                   radix: Decimal,
// C11-NEXT:                                   suffix: IntegerSuffix {
// C11-NEXT:                                       unsigned: false,
// C11-NEXT:                                       size: Long,
// C11-NEXT:                                   },
// C11-NEXT:                                   spelling: "201112L",
// C11-NEXT:                               },
// C11-NEXT:                           ),
// C11-NEXT:                       ),
// C11-NEXT:                   ),
// C11-NEXT:               },
// C11-NEXT:           ],
// C11-NEXT:       },
// C11-NEXT:   )
// SLATE-FILECHECK-END C11
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
// C17-NEXT:                       "stdc_version",
// C17-NEXT:                   ),
// C17-NEXT:                   initializer: Some(
// C17-NEXT:                       Expr(
// C17-NEXT:                           IntegerLiteral(
// C17-NEXT:                               IntegerLiteral {
// C17-NEXT:                                   value: 201710,
// C17-NEXT:                                   radix: Decimal,
// C17-NEXT:                                   suffix: IntegerSuffix {
// C17-NEXT:                                       unsigned: false,
// C17-NEXT:                                       size: Long,
// C17-NEXT:                                   },
// C17-NEXT:                                   spelling: "201710L",
// C17-NEXT:                               },
// C17-NEXT:                           ),
// C17-NEXT:                       ),
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// SLATE-FILECHECK-END C17
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
// C23-NEXT:                       "stdc_version",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           IntegerLiteral(
// C23-NEXT:                               IntegerLiteral {
// C23-NEXT:                                   value: 202311,
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: IntegerSuffix {
// C23-NEXT:                                       unsigned: false,
// C23-NEXT:                                       size: Long,
// C23-NEXT:                                   },
// C23-NEXT:                                   spelling: "202311L",
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
