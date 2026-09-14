void abort(void);
void exit(int);

int main(void) {
  int   i, j, k, l;
  float x[8][2][8][2];

  for (i = 0; i < 8; i++)
    for (j = i; j < 8; j++)
      for (k = 0; k < 2; k++)
        for (l = 0; l < 2; l++) {
          if ((i == j) && (k == l))
            x[i][k][j][l] = 0.8;
          else
            x[i][k][j][l] = 0.8;
          if (x[i][k][j][l] < 0.0)
            abort();
        }

  exit(0);
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

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
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "j",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "k",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "l",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Array {
// DEFAULT-NEXT:                                           inner: Array {
// DEFAULT-NEXT:                                               inner: Name(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               size: Expression(
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 8,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "8",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           size: Expression(
// DEFAULT-NEXT:                                               IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 2,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "2",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       size: Expression(
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 8,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "8",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 8,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "8",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "j",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           condition: Some(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Less,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "j",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 8,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "8",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Postfix {
// DEFAULT-NEXT:                                   op: Increment,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "j",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               For {
// DEFAULT-NEXT:                                   init: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "k",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 0,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   condition: Some(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Less,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "k",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   increment: Some(
// DEFAULT-NEXT:                                       Postfix {
// DEFAULT-NEXT:                                           op: Increment,
// DEFAULT-NEXT:                                           operand: Identifier(
// DEFAULT-NEXT:                                               "k",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       For {
// DEFAULT-NEXT:                                           init: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "l",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           condition: Some(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "l",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 2,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "2",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           increment: Some(
// DEFAULT-NEXT:                                               Postfix {
// DEFAULT-NEXT:                                                   op: Increment,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "l",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           body: [
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: And,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "i",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "j",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "k",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "l",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Index {
// DEFAULT-NEXT:                                                                   base: Index {
// DEFAULT-NEXT:                                                                       base: Index {
// DEFAULT-NEXT:                                                                           base: Index {
// DEFAULT-NEXT:                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                   "x",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               index: Identifier(
// DEFAULT-NEXT:                                                                                   "i",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           index: Identifier(
// DEFAULT-NEXT:                                                                               "k",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       index: Identifier(
// DEFAULT-NEXT:                                                                           "j",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Identifier(
// DEFAULT-NEXT:                                                                       "l",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: FloatLiteral(
// DEFAULT-NEXT:                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                       spelling: "0.8",
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: Some(
// DEFAULT-NEXT:                                                       [
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Index {
// DEFAULT-NEXT:                                                                       base: Index {
// DEFAULT-NEXT:                                                                           base: Index {
// DEFAULT-NEXT:                                                                               base: Index {
// DEFAULT-NEXT:                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                       "x",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   index: Identifier(
// DEFAULT-NEXT:                                                                                       "i",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               index: Identifier(
// DEFAULT-NEXT:                                                                                   "k",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           index: Identifier(
// DEFAULT-NEXT:                                                                               "j",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       index: Identifier(
// DEFAULT-NEXT:                                                                           "l",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.8",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: Less,
// DEFAULT-NEXT:                                                       left: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Index {
// DEFAULT-NEXT:                                                                   base: Index {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "x",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       index: Identifier(
// DEFAULT-NEXT:                                                                           "i",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Identifier(
// DEFAULT-NEXT:                                                                       "k",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "j",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "l",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.0",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "abort",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
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
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
