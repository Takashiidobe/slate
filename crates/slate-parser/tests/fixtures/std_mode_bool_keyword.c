#if __STDC_VERSION__ >= 202311L
bool flag = 1;
#else
int bool = 1;
#endif

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C17
// C17: decl[0]: Declaration {
// C17-NEXT:       declaration: Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: Integer(
// C17-NEXT:                   Ranked {
// C17-NEXT:                       rank: Int,
// C17-NEXT:                       signed: true,
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclarator {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "bool",
// C17-NEXT:                   ),
// C17-NEXT:                   initializer: Some(
// C17-NEXT:                       Expr(
// C17-NEXT:                           IntegerLiteral(
// C17-NEXT:                               IntegerLiteral {
// C17-NEXT:                                   value: 1,
// C17-NEXT:                                   radix: Decimal,
// C17-NEXT:                                   suffix: IntegerSuffix {
// C17-NEXT:                                       unsigned: false,
// C17-NEXT:                                       size: None,
// C17-NEXT:                                   },
// C17-NEXT:                                   spelling: "1",
// C17-NEXT:                               },
// C17-NEXT:                           ),
// C17-NEXT:                       ),
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:       provenance: Provenance {
// C17-NEXT:           file: FileId(
// C17-NEXT:               3,
// C17-NEXT:           ),
// C17-NEXT:           kind: User,
// C17-NEXT:           line: 3,
// C17-NEXT:           header: None,
// C17-NEXT:       },
// C17-NEXT:   }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: decl[0]: Declaration {
// C23-NEXT:       declaration: Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Bool,
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclarator {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "flag",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           IntegerLiteral(
// C23-NEXT:                               IntegerLiteral {
// C23-NEXT:                                   value: 1,
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: IntegerSuffix {
// C23-NEXT:                                       unsigned: false,
// C23-NEXT:                                       size: None,
// C23-NEXT:                                   },
// C23-NEXT:                                   spelling: "1",
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:       provenance: Provenance {
// C23-NEXT:           file: FileId(
// C23-NEXT:               3,
// C23-NEXT:           ),
// C23-NEXT:           kind: User,
// C23-NEXT:           line: 1,
// C23-NEXT:           header: None,
// C23-NEXT:       },
// C23-NEXT:   }
// SLATE-FILECHECK-END C23
