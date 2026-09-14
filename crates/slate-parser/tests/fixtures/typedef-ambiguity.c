int B;

#ifdef AS_CAST
typedef int A;
int cast_main() {
  (A)(B);
  return 0;
}
#else
int A(int value);
int call_main() {
  (A)(B);
  return 0;
}
#endif
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES CAST AS_CAST

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
// DEFAULT-NEXT:                       "B",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
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
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Name(
// DEFAULT-NEXT:                                       "value",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "call_main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Paren(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "A",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "B",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN CAST
// CAST: decl[0]: Declaration {
// CAST-NEXT:       declaration: Declaration {
// CAST-NEXT:           specifiers: DeclarationSpecifiers {
// CAST-NEXT:               ty: Integer(
// CAST-NEXT:                   Ranked {
// CAST-NEXT:                       rank: Int,
// CAST-NEXT:                       signed: true,
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:           },
// CAST-NEXT:           declarators: [
// CAST-NEXT:               InitDeclarator {
// CAST-NEXT:                   declarator: Name(
// CAST-NEXT:                       "B",
// CAST-NEXT:                   ),
// CAST-NEXT:               },
// CAST-NEXT:           ],
// CAST-NEXT:       },
// CAST-NEXT:       provenance: Provenance {
// CAST-NEXT:           file: FileId(
// CAST-NEXT:               3,
// CAST-NEXT:           ),
// CAST-NEXT:           kind: User,
// CAST-NEXT:           line: 0,
// CAST-NEXT:           header: None,
// CAST-NEXT:       },
// CAST-NEXT:   }
// CAST-NEXT: decl[1]: Declaration {
// CAST-NEXT:       declaration: Declaration {
// CAST-NEXT:           specifiers: DeclarationSpecifiers {
// CAST-NEXT:               ty: Integer(
// CAST-NEXT:                   Ranked {
// CAST-NEXT:                       rank: Int,
// CAST-NEXT:                       signed: true,
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:               storage: Typedef,
// CAST-NEXT:           },
// CAST-NEXT:           declarators: [
// CAST-NEXT:               InitDeclarator {
// CAST-NEXT:                   declarator: Name(
// CAST-NEXT:                       "A",
// CAST-NEXT:                   ),
// CAST-NEXT:               },
// CAST-NEXT:           ],
// CAST-NEXT:       },
// CAST-NEXT:       provenance: Provenance {
// CAST-NEXT:           file: FileId(
// CAST-NEXT:               3,
// CAST-NEXT:           ),
// CAST-NEXT:           kind: User,
// CAST-NEXT:           line: 3,
// CAST-NEXT:           header: None,
// CAST-NEXT:       },
// CAST-NEXT:   }
// CAST-NEXT: decl[2]: Function(
// CAST-NEXT:       FunctionDefinition {
// CAST-NEXT:           specifiers: DeclarationSpecifiers {
// CAST-NEXT:               ty: Integer(
// CAST-NEXT:                   Ranked {
// CAST-NEXT:                       rank: Int,
// CAST-NEXT:                       signed: true,
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:           },
// CAST-NEXT:           declarator: Function {
// CAST-NEXT:               inner: Name(
// CAST-NEXT:                   "cast_main",
// CAST-NEXT:               ),
// CAST-NEXT:               parameters: Empty,
// CAST-NEXT:           },
// CAST-NEXT:           body: [
// CAST-NEXT:               Expr(
// CAST-NEXT:                   Cast {
// CAST-NEXT:                       ty: TypeName {
// CAST-NEXT:                           specifiers: DeclarationSpecifiers {
// CAST-NEXT:                               ty: Named(
// CAST-NEXT:                                   "A",
// CAST-NEXT:                               ),
// CAST-NEXT:                           },
// CAST-NEXT:                           declarator: Abstract,
// CAST-NEXT:                       },
// CAST-NEXT:                       value: Paren(
// CAST-NEXT:                           Identifier(
// CAST-NEXT:                               "B",
// CAST-NEXT:                           ),
// CAST-NEXT:                       ),
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:               Return(
// CAST-NEXT:                   IntegerLiteral(
// CAST-NEXT:                       IntegerLiteral {
// CAST-NEXT:                           value: 0,
// CAST-NEXT:                           radix: Decimal,
// CAST-NEXT:                           suffix: IntegerSuffix {
// CAST-NEXT:                               unsigned: false,
// CAST-NEXT:                               size: None,
// CAST-NEXT:                           },
// CAST-NEXT:                           spelling: "0",
// CAST-NEXT:                       },
// CAST-NEXT:                   ),
// CAST-NEXT:               ),
// CAST-NEXT:           ],
// CAST-NEXT:           provenance: Provenance {
// CAST-NEXT:               file: FileId(
// CAST-NEXT:                   3,
// CAST-NEXT:               ),
// CAST-NEXT:               kind: User,
// CAST-NEXT:               line: 4,
// CAST-NEXT:               header: None,
// CAST-NEXT:           },
// CAST-NEXT:       },
// CAST-NEXT:   )
// SLATE-FILECHECK-END CAST
