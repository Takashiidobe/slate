void abort(void);
void exit(int);

long double C  = 2;
long double U  = 1;
long double Y2 = 3;
long double Y1 = 1;
long double X, Y, Z, T, R, S;
int         main(void) {
  X  = (C + U) * Y2;
  Y  = C - U - U;
  Z  = C + U + U;
  T  = (C - U) * Y1;
  X  = X - (Z + U);
  R  = Y * Y1;
  S  = Z * Y2;
  T  = T - Y;
  Y  = (U - Y) + R;
  Z  = S - (Z + U + U);
  R  = (Y2 + U) * Y1;
  Y1 = Y2 * Y1;
  R  = R - Y2;
  Y1 = Y1 - 0.5L;
  if (Z != 6)
    abort();
  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Void,
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
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
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   LongDouble,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "C",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 2,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "2",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   LongDouble,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "U",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   LongDouble,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Y2",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 3,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "3",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   LongDouble,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Y1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   LongDouble,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "X",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Y",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Z",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "T",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "R",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "S",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Function(
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
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "X",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "C",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Y",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "C",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Identifier(
// DEFAULT-NEXT:                                   "U",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "U",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Z",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "C",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Identifier(
// DEFAULT-NEXT:                                   "U",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "U",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "T",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Sub,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "C",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "X",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "X",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "Z",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "R",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "Y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "S",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "Z",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "T",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "T",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Y",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Sub,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "Y",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "R",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Z",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "S",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "Z",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "U",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "R",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "Y2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "U",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Y1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Mul,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "Y2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "R",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "R",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "Y2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "Y1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "Y1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: FloatLiteral(
// DEFAULT-NEXT:                               FloatLiteral {
// DEFAULT-NEXT:                                   spelling: "0.5L",
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: L,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: NotEqual,
// DEFAULT-NEXT:                       left: Identifier(
// DEFAULT-NEXT:                           "Z",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 6,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "6",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "abort",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 8,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
