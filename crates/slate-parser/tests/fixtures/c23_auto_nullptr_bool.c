void f(void) {
#if __STDC_VERSION__ >= 202311L
  auto z = 1.0;
#else
  auto int z = 1;
#endif
  int *p = nullptr;
  _Bool t = true;
  _Bool u = false;
}

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C17
// C17: decl[0]: Function(
// C17-NEXT:       FunctionDecl {
// C17-NEXT:           ret_type: Void,
// C17-NEXT:           name: "f",
// C17-NEXT:           body: [
// C17-NEXT:               Decl(
// C17-NEXT:                   Declaration {
// C17-NEXT:                       specifiers: DeclarationSpecifiers {
// C17-NEXT:                           ty: Integer(
// C17-NEXT:                               Ranked {
// C17-NEXT:                                   rank: Int,
// C17-NEXT:                                   signed: true,
// C17-NEXT:                               },
// C17-NEXT:                           ),
// C17-NEXT:                           storage: Auto,
// C17-NEXT:                       },
// C17-NEXT:                       declarators: [
// C17-NEXT:                           InitDeclarator {
// C17-NEXT:                               declarator: Name(
// C17-NEXT:                                   "z",
// C17-NEXT:                               ),
// C17-NEXT:                               initializer: Some(
// C17-NEXT:                                   Expr(
// C17-NEXT:                                       IntegerLiteral(
// C17-NEXT:                                           IntegerLiteral {
// C17-NEXT:                                               value: 1,
// C17-NEXT:                                               radix: Decimal,
// C17-NEXT:                                               suffix: IntegerSuffix {
// C17-NEXT:                                                   unsigned: false,
// C17-NEXT:                                                   size: None,
// C17-NEXT:                                               },
// C17-NEXT:                                               spelling: "1",
// C17-NEXT:                                           },
// C17-NEXT:                                       ),
// C17-NEXT:                                   ),
// C17-NEXT:                               ),
// C17-NEXT:                           },
// C17-NEXT:                       ],
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:               Decl(
// C17-NEXT:                   Declaration {
// C17-NEXT:                       specifiers: DeclarationSpecifiers {
// C17-NEXT:                           ty: Integer(
// C17-NEXT:                               Ranked {
// C17-NEXT:                                   rank: Int,
// C17-NEXT:                                   signed: true,
// C17-NEXT:                               },
// C17-NEXT:                           ),
// C17-NEXT:                       },
// C17-NEXT:                       declarators: [
// C17-NEXT:                           InitDeclarator {
// C17-NEXT:                               declarator: Pointer {
// C17-NEXT:                                   qualifiers: Qualifiers,
// C17-NEXT:                                   inner: Name(
// C17-NEXT:                                       "p",
// C17-NEXT:                                   ),
// C17-NEXT:                               },
// C17-NEXT:                               initializer: Some(
// C17-NEXT:                                   Expr(
// C17-NEXT:                                       Identifier(
// C17-NEXT:                                           "nullptr",
// C17-NEXT:                                       ),
// C17-NEXT:                                   ),
// C17-NEXT:                               ),
// C17-NEXT:                           },
// C17-NEXT:                       ],
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:               Decl(
// C17-NEXT:                   Declaration {
// C17-NEXT:                       specifiers: DeclarationSpecifiers {
// C17-NEXT:                           ty: Bool,
// C17-NEXT:                       },
// C17-NEXT:                       declarators: [
// C17-NEXT:                           InitDeclarator {
// C17-NEXT:                               declarator: Name(
// C17-NEXT:                                   "t",
// C17-NEXT:                               ),
// C17-NEXT:                               initializer: Some(
// C17-NEXT:                                   Expr(
// C17-NEXT:                                       Identifier(
// C17-NEXT:                                           "true",
// C17-NEXT:                                       ),
// C17-NEXT:                                   ),
// C17-NEXT:                               ),
// C17-NEXT:                           },
// C17-NEXT:                       ],
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:               Decl(
// C17-NEXT:                   Declaration {
// C17-NEXT:                       specifiers: DeclarationSpecifiers {
// C17-NEXT:                           ty: Bool,
// C17-NEXT:                       },
// C17-NEXT:                       declarators: [
// C17-NEXT:                           InitDeclarator {
// C17-NEXT:                               declarator: Name(
// C17-NEXT:                                   "u",
// C17-NEXT:                               ),
// C17-NEXT:                               initializer: Some(
// C17-NEXT:                                   Expr(
// C17-NEXT:                                       Identifier(
// C17-NEXT:                                           "false",
// C17-NEXT:                                       ),
// C17-NEXT:                                   ),
// C17-NEXT:                               ),
// C17-NEXT:                           },
// C17-NEXT:                       ],
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           ],
// C17-NEXT:           provenance: Provenance {
// C17-NEXT:               file: FileId(
// C17-NEXT:                   3,
// C17-NEXT:               ),
// C17-NEXT:               kind: User,
// C17-NEXT:               line: 0,
// C17-NEXT:               header: None,
// C17-NEXT:           },
// C17-NEXT:       },
// C17-NEXT:   )
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: decl[0]: Function(
// C23-NEXT:       FunctionDecl {
// C23-NEXT:           ret_type: Void,
// C23-NEXT:           name: "f",
// C23-NEXT:           body: [
// C23-NEXT:               Decl(
// C23-NEXT:                   Declaration {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: TargetBuiltin(
// C23-NEXT:                               "__auto_type",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           InitDeclarator {
// C23-NEXT:                               declarator: Name(
// C23-NEXT:                                   "z",
// C23-NEXT:                               ),
// C23-NEXT:                               initializer: Some(
// C23-NEXT:                                   Expr(
// C23-NEXT:                                       FloatLiteral(
// C23-NEXT:                                           FloatLiteral {
// C23-NEXT:                                               spelling: "1.0",
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: None,
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Decl(
// C23-NEXT:                   Declaration {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Integer(
// C23-NEXT:                               Ranked {
// C23-NEXT:                                   rank: Int,
// C23-NEXT:                                   signed: true,
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           InitDeclarator {
// C23-NEXT:                               declarator: Pointer {
// C23-NEXT:                                   qualifiers: Qualifiers,
// C23-NEXT:                                   inner: Name(
// C23-NEXT:                                       "p",
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                               initializer: Some(
// C23-NEXT:                                   Expr(
// C23-NEXT:                                       NullPtrLiteral,
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Decl(
// C23-NEXT:                   Declaration {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Bool,
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           InitDeclarator {
// C23-NEXT:                               declarator: Name(
// C23-NEXT:                                   "t",
// C23-NEXT:                               ),
// C23-NEXT:                               initializer: Some(
// C23-NEXT:                                   Expr(
// C23-NEXT:                                       BoolLiteral(
// C23-NEXT:                                           true,
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Decl(
// C23-NEXT:                   Declaration {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Bool,
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           InitDeclarator {
// C23-NEXT:                               declarator: Name(
// C23-NEXT:                                   "u",
// C23-NEXT:                               ),
// C23-NEXT:                               initializer: Some(
// C23-NEXT:                                   Expr(
// C23-NEXT:                                       BoolLiteral(
// C23-NEXT:                                           false,
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:           provenance: Provenance {
// C23-NEXT:               file: FileId(
// C23-NEXT:                   3,
// C23-NEXT:               ),
// C23-NEXT:               kind: User,
// C23-NEXT:               line: 0,
// C23-NEXT:               header: None,
// C23-NEXT:           },
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
