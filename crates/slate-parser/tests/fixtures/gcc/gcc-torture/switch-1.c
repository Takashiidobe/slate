/* Copyright (C) 2003  Free Software Foundation.

   Test that switch statements suitable using case bit tests are
   implemented correctly.

   Written by Roger Sayle, 01/25/2001.  */

extern void abort(void);

int foo(int x) {
  switch (x) {
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  }
  return 31;
}

int main() {
  int i, r;

  for (i = -1; i < 66; i++) {
    r = foo(i);
    if (i == 4) {
      if (r != 30)
        abort();
    } else if (i == 6) {
      if (r != 30)
        abort();
    } else if (i == 9) {
      if (r != 30)
        abort();
    } else if (i == 11) {
      if (r != 30)
        abort();
    } else if (r != 31)
      abort();
  }
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comment: Comment {
// DEFAULT-NEXT:               text: [
// DEFAULT-NEXT:                   "/* Copyright (C) 2003  Free Software Foundation.\n\n   Test that switch statements suitable using case bit tests are\n   implemented correctly.\n\n   Written by Roger Sayle, 01/25/2001.  */",
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:               loc: Loc {
// DEFAULT-NEXT:                   file: FileId(
// DEFAULT-NEXT:                       3,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   offset: 0,
// DEFAULT-NEXT:                   length: 184,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "foo",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Switch {
// DEFAULT-NEXT:                   discriminant: Identifier(
// DEFAULT-NEXT:                       "x",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 4,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "4",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: SwitchLabel {
// DEFAULT-NEXT:                               label: Case(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 6,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "6",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               body: SwitchLabel {
// DEFAULT-NEXT:                                   label: Case(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 9,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "9",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: SwitchLabel {
// DEFAULT-NEXT:                                       label: Case(
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 11,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "11",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       body: Return(
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 30,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "30",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 31,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "31",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 9,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
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
// DEFAULT-NEXT:                                   "r",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:                               value: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   operand: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                                   value: 66,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "66",
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
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "r",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "foo",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 4,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "4",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: NotEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "r",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 30,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "30",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: Some(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 6,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "6",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           If {
// DEFAULT-NEXT:                                               condition: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 30,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "30",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               then_branch: [
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "abort",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               else_branch: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: Some(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: Equal,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 9,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "9",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       If {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: NotEqual,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "r",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 30,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "30",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           then_branch: [
// DEFAULT-NEXT:                                                               Expr(
// DEFAULT-NEXT:                                                                   Call {
// DEFAULT-NEXT:                                                                       callee: Identifier(
// DEFAULT-NEXT:                                                                           "abort",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       arguments: [],
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           else_branch: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: Some(
// DEFAULT-NEXT:                                                       [
// DEFAULT-NEXT:                                                           If {
// DEFAULT-NEXT:                                                               condition: Binary {
// DEFAULT-NEXT:                                                                   op: Equal,
// DEFAULT-NEXT:                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                       "i",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 11,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "11",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               then_branch: [
// DEFAULT-NEXT:                                                                   If {
// DEFAULT-NEXT:                                                                       condition: Binary {
// DEFAULT-NEXT:                                                                           op: NotEqual,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "r",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 30,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "30",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       then_branch: [
// DEFAULT-NEXT:                                                                           Expr(
// DEFAULT-NEXT:                                                                               Call {
// DEFAULT-NEXT:                                                                                   callee: Identifier(
// DEFAULT-NEXT:                                                                                       "abort",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   arguments: [],
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       else_branch: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               else_branch: Some(
// DEFAULT-NEXT:                                                                   [
// DEFAULT-NEXT:                                                                       If {
// DEFAULT-NEXT:                                                                           condition: Binary {
// DEFAULT-NEXT:                                                                               op: NotEqual,
// DEFAULT-NEXT:                                                                               left: Identifier(
// DEFAULT-NEXT:                                                                                   "r",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                       value: 31,
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                           size: None,
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       spelling: "31",
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           then_branch: [
// DEFAULT-NEXT:                                                                               Expr(
// DEFAULT-NEXT:                                                                                   Call {
// DEFAULT-NEXT:                                                                                       callee: Identifier(
// DEFAULT-NEXT:                                                                                           "abort",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       arguments: [],
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ],
// DEFAULT-NEXT:                                                                           else_branch: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:               line: 20,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
