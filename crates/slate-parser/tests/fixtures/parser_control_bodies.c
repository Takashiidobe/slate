void bodies(int x) {
  if (x) { x++; } else x--;
  if (x) x++; else { x--; }
  while (x) { x--; }
  while (x) x--;
  do { x++; } while (x);
  do x++; while (x);
  for (; x; x--) { x++; }
  for (; x; x--) x++;
  switch (x) { case 0: x++; break; }
  switch (x) default: x++;
  if (x) {} else ;
}
typedef int T;
int condition_typedef(void) {
  if (sizeof(enum { T = 1 })) (void)sizeof(T);
  return sizeof(T);
}
int body_typedef(void) {
  if (1) (void)sizeof(enum { T = 1 });
  return sizeof(T);
}
int do_typedef(void) {
  do (void)sizeof(enum { T = 1 }); while (sizeof(T));
  return sizeof(T);
}
int nested_typedef(void) {
  if (sizeof(enum { T = 1 })) {
    typedef int T;
    T x;
  }
  return sizeof(T);
}

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99

// SLATE-FILECHECK-BEGIN C89
// C89: tag[0]: TagDefinition {
// C89-NEXT:       id: TagId(
// C89-NEXT:           0,
// C89-NEXT:       ),
// C89-NEXT:       kind: Enum,
// C89-NEXT:       name: None,
// C89-NEXT:       body: Enum {
// C89-NEXT:           enumerators: [
// C89-NEXT:               Enumerator(
// C89-NEXT:                   Enumerator {
// C89-NEXT:                       name: "T",
// C89-NEXT:                       value: Some(
// C89-NEXT:                           IntegerLiteral(
// C89-NEXT:                               IntegerLiteral {
// C89-NEXT:                                   value: 1,
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: IntegerSuffix {
// C89-NEXT:                                       unsigned: false,
// C89-NEXT:                                       size: None,
// C89-NEXT:                                   },
// C89-NEXT:                                   spelling: "1",
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   }
// C89-NEXT: tag[1]: TagDefinition {
// C89-NEXT:       id: TagId(
// C89-NEXT:           1,
// C89-NEXT:       ),
// C89-NEXT:       kind: Enum,
// C89-NEXT:       name: None,
// C89-NEXT:       body: Enum {
// C89-NEXT:           enumerators: [
// C89-NEXT:               Enumerator(
// C89-NEXT:                   Enumerator {
// C89-NEXT:                       name: "T",
// C89-NEXT:                       value: Some(
// C89-NEXT:                           IntegerLiteral(
// C89-NEXT:                               IntegerLiteral {
// C89-NEXT:                                   value: 1,
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: IntegerSuffix {
// C89-NEXT:                                       unsigned: false,
// C89-NEXT:                                       size: None,
// C89-NEXT:                                   },
// C89-NEXT:                                   spelling: "1",
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   }
// C89-NEXT: tag[2]: TagDefinition {
// C89-NEXT:       id: TagId(
// C89-NEXT:           2,
// C89-NEXT:       ),
// C89-NEXT:       kind: Enum,
// C89-NEXT:       name: None,
// C89-NEXT:       body: Enum {
// C89-NEXT:           enumerators: [
// C89-NEXT:               Enumerator(
// C89-NEXT:                   Enumerator {
// C89-NEXT:                       name: "T",
// C89-NEXT:                       value: Some(
// C89-NEXT:                           IntegerLiteral(
// C89-NEXT:                               IntegerLiteral {
// C89-NEXT:                                   value: 1,
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: IntegerSuffix {
// C89-NEXT:                                       unsigned: false,
// C89-NEXT:                                       size: None,
// C89-NEXT:                                   },
// C89-NEXT:                                   spelling: "1",
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   }
// C89-NEXT: tag[3]: TagDefinition {
// C89-NEXT:       id: TagId(
// C89-NEXT:           3,
// C89-NEXT:       ),
// C89-NEXT:       kind: Enum,
// C89-NEXT:       name: None,
// C89-NEXT:       body: Enum {
// C89-NEXT:           enumerators: [
// C89-NEXT:               Enumerator(
// C89-NEXT:                   Enumerator {
// C89-NEXT:                       name: "T",
// C89-NEXT:                       value: Some(
// C89-NEXT:                           IntegerLiteral(
// C89-NEXT:                               IntegerLiteral {
// C89-NEXT:                                   value: 1,
// C89-NEXT:                                   radix: Decimal,
// C89-NEXT:                                   suffix: IntegerSuffix {
// C89-NEXT:                                       unsigned: false,
// C89-NEXT:                                       size: None,
// C89-NEXT:                                   },
// C89-NEXT:                                   spelling: "1",
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   }
// C89-NEXT: decl[0]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Void,
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "bodies",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Prototype {
// C89-NEXT:                   parameters: [
// C89-NEXT:                       ParameterDeclarationKind {
// C89-NEXT:                           specifiers: DeclarationSpecifiers {
// C89-NEXT:                               ty: Integer(
// C89-NEXT:                                   Ranked {
// C89-NEXT:                                       rank: Int,
// C89-NEXT:                                       signed: true,
// C89-NEXT:                                   },
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                           declarator: Name(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ],
// C89-NEXT:               },
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               If {
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   then_branch: Block(
// C89-NEXT:                       [
// C89-NEXT:                           Expr(
// C89-NEXT:                               Postfix {
// C89-NEXT:                                   op: Increment,
// C89-NEXT:                                   operand: Identifier(
// C89-NEXT:                                       "x",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: Some(
// C89-NEXT:                       Expr(
// C89-NEXT:                           Postfix {
// C89-NEXT:                               op: Decrement,
// C89-NEXT:                               operand: Identifier(
// C89-NEXT:                                   "x",
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               If {
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   then_branch: Expr(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Increment,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: Some(
// C89-NEXT:                       Block(
// C89-NEXT:                           [
// C89-NEXT:                               Expr(
// C89-NEXT:                                   Postfix {
// C89-NEXT:                                       op: Decrement,
// C89-NEXT:                                       operand: Identifier(
// C89-NEXT:                                           "x",
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                               ),
// C89-NEXT:                           ],
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               While {
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   body: Block(
// C89-NEXT:                       [
// C89-NEXT:                           Expr(
// C89-NEXT:                               Postfix {
// C89-NEXT:                                   op: Decrement,
// C89-NEXT:                                   operand: Identifier(
// C89-NEXT:                                       "x",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               While {
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   body: Expr(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Decrement,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               DoWhile {
// C89-NEXT:                   body: Block(
// C89-NEXT:                       [
// C89-NEXT:                           Expr(
// C89-NEXT:                               Postfix {
// C89-NEXT:                                   op: Increment,
// C89-NEXT:                                   operand: Identifier(
// C89-NEXT:                                       "x",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               DoWhile {
// C89-NEXT:                   body: Expr(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Increment,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               For {
// C89-NEXT:                   init: None,
// C89-NEXT:                   condition: Some(
// C89-NEXT:                       Identifier(
// C89-NEXT:                           "x",
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:                   increment: Some(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Decrement,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   body: Block(
// C89-NEXT:                       [
// C89-NEXT:                           Expr(
// C89-NEXT:                               Postfix {
// C89-NEXT:                                   op: Increment,
// C89-NEXT:                                   operand: Identifier(
// C89-NEXT:                                       "x",
// C89-NEXT:                                   ),
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               For {
// C89-NEXT:                   init: None,
// C89-NEXT:                   condition: Some(
// C89-NEXT:                       Identifier(
// C89-NEXT:                           "x",
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:                   increment: Some(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Decrement,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   body: Expr(
// C89-NEXT:                       Postfix {
// C89-NEXT:                           op: Increment,
// C89-NEXT:                           operand: Identifier(
// C89-NEXT:                               "x",
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               Switch {
// C89-NEXT:                   discriminant: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   body: Block(
// C89-NEXT:                       [
// C89-NEXT:                           SwitchLabel {
// C89-NEXT:                               label: Case(
// C89-NEXT:                                   IntegerLiteral(
// C89-NEXT:                                       IntegerLiteral {
// C89-NEXT:                                           value: 0,
// C89-NEXT:                                           radix: Decimal,
// C89-NEXT:                                           suffix: IntegerSuffix {
// C89-NEXT:                                               unsigned: false,
// C89-NEXT:                                               size: None,
// C89-NEXT:                                           },
// C89-NEXT:                                           spelling: "0",
// C89-NEXT:                                       },
// C89-NEXT:                                   ),
// C89-NEXT:                               ),
// C89-NEXT:                               body: Expr(
// C89-NEXT:                                   Postfix {
// C89-NEXT:                                       op: Increment,
// C89-NEXT:                                       operand: Identifier(
// C89-NEXT:                                           "x",
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                           Break,
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               Switch {
// C89-NEXT:                   discriminant: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   body: SwitchLabel {
// C89-NEXT:                       label: Default,
// C89-NEXT:                       body: Expr(
// C89-NEXT:                           Postfix {
// C89-NEXT:                               op: Increment,
// C89-NEXT:                               operand: Identifier(
// C89-NEXT:                                   "x",
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                       ),
// C89-NEXT:                   },
// C89-NEXT:               },
// C89-NEXT:               If {
// C89-NEXT:                   condition: Identifier(
// C89-NEXT:                       "x",
// C89-NEXT:                   ),
// C89-NEXT:                   then_branch: Block(
// C89-NEXT:                       [],
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: Some(
// C89-NEXT:                       Null,
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[1]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:               storage: Typedef,
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "T",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[2]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "condition_typedef",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Void,
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               If {
// C89-NEXT:                   condition: SizeOfType {
// C89-NEXT:                       ty: TypeName {
// C89-NEXT:                           specifiers: DeclarationSpecifiers {
// C89-NEXT:                               ty: Tag(
// C89-NEXT:                                   Definition(
// C89-NEXT:                                       TagId(
// C89-NEXT:                                           0,
// C89-NEXT:                                       ),
// C89-NEXT:                                   ),
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                           declarator: Abstract,
// C89-NEXT:                       },
// C89-NEXT:                   },
// C89-NEXT:                   then_branch: Expr(
// C89-NEXT:                       Cast {
// C89-NEXT:                           ty: TypeName {
// C89-NEXT:                               specifiers: DeclarationSpecifiers {
// C89-NEXT:                                   ty: Void,
// C89-NEXT:                               },
// C89-NEXT:                               declarator: Abstract,
// C89-NEXT:                           },
// C89-NEXT:                           value: SizeOfExpr(
// C89-NEXT:                               Paren(
// C89-NEXT:                                   Identifier(
// C89-NEXT:                                       "T",
// C89-NEXT:                                   ),
// C89-NEXT:                               ),
// C89-NEXT:                           ),
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: None,
// C89-NEXT:               },
// C89-NEXT:               Return(
// C89-NEXT:                   SizeOfExpr(
// C89-NEXT:                       Paren(
// C89-NEXT:                           Identifier(
// C89-NEXT:                               "T",
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[3]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "body_typedef",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Void,
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               If {
// C89-NEXT:                   condition: IntegerLiteral(
// C89-NEXT:                       IntegerLiteral {
// C89-NEXT:                           value: 1,
// C89-NEXT:                           radix: Decimal,
// C89-NEXT:                           suffix: IntegerSuffix {
// C89-NEXT:                               unsigned: false,
// C89-NEXT:                               size: None,
// C89-NEXT:                           },
// C89-NEXT:                           spelling: "1",
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   then_branch: Expr(
// C89-NEXT:                       Cast {
// C89-NEXT:                           ty: TypeName {
// C89-NEXT:                               specifiers: DeclarationSpecifiers {
// C89-NEXT:                                   ty: Void,
// C89-NEXT:                               },
// C89-NEXT:                               declarator: Abstract,
// C89-NEXT:                           },
// C89-NEXT:                           value: SizeOfType {
// C89-NEXT:                               ty: TypeName {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Tag(
// C89-NEXT:                                           Definition(
// C89-NEXT:                                               TagId(
// C89-NEXT:                                                   1,
// C89-NEXT:                                               ),
// C89-NEXT:                                           ),
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                                   declarator: Abstract,
// C89-NEXT:                               },
// C89-NEXT:                           },
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: None,
// C89-NEXT:               },
// C89-NEXT:               Return(
// C89-NEXT:                   SizeOfExpr(
// C89-NEXT:                       Paren(
// C89-NEXT:                           Identifier(
// C89-NEXT:                               "T",
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[4]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "do_typedef",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Void,
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               DoWhile {
// C89-NEXT:                   body: Expr(
// C89-NEXT:                       Cast {
// C89-NEXT:                           ty: TypeName {
// C89-NEXT:                               specifiers: DeclarationSpecifiers {
// C89-NEXT:                                   ty: Void,
// C89-NEXT:                               },
// C89-NEXT:                               declarator: Abstract,
// C89-NEXT:                           },
// C89-NEXT:                           value: SizeOfType {
// C89-NEXT:                               ty: TypeName {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Tag(
// C89-NEXT:                                           Definition(
// C89-NEXT:                                               TagId(
// C89-NEXT:                                                   2,
// C89-NEXT:                                               ),
// C89-NEXT:                                           ),
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                                   declarator: Abstract,
// C89-NEXT:                               },
// C89-NEXT:                           },
// C89-NEXT:                       },
// C89-NEXT:                   ),
// C89-NEXT:                   condition: SizeOfExpr(
// C89-NEXT:                       Paren(
// C89-NEXT:                           Identifier(
// C89-NEXT:                               "T",
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:               Return(
// C89-NEXT:                   SizeOfExpr(
// C89-NEXT:                       Paren(
// C89-NEXT:                           Identifier(
// C89-NEXT:                               "T",
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[5]: Function(
// C89-NEXT:       FunctionDefinition {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarator: Function {
// C89-NEXT:               inner: Name(
// C89-NEXT:                   "nested_typedef",
// C89-NEXT:               ),
// C89-NEXT:               parameters: Void,
// C89-NEXT:           },
// C89-NEXT:           body: [
// C89-NEXT:               If {
// C89-NEXT:                   condition: SizeOfType {
// C89-NEXT:                       ty: TypeName {
// C89-NEXT:                           specifiers: DeclarationSpecifiers {
// C89-NEXT:                               ty: Tag(
// C89-NEXT:                                   Definition(
// C89-NEXT:                                       TagId(
// C89-NEXT:                                           3,
// C89-NEXT:                                       ),
// C89-NEXT:                                   ),
// C89-NEXT:                               ),
// C89-NEXT:                           },
// C89-NEXT:                           declarator: Abstract,
// C89-NEXT:                       },
// C89-NEXT:                   },
// C89-NEXT:                   then_branch: Block(
// C89-NEXT:                       [
// C89-NEXT:                           Decl(
// C89-NEXT:                               Declaration {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Integer(
// C89-NEXT:                                           Ranked {
// C89-NEXT:                                               rank: Int,
// C89-NEXT:                                               signed: true,
// C89-NEXT:                                           },
// C89-NEXT:                                       ),
// C89-NEXT:                                       storage: Typedef,
// C89-NEXT:                                   },
// C89-NEXT:                                   declarators: [
// C89-NEXT:                                       InitDeclaratorKind {
// C89-NEXT:                                           declarator: Name(
// C89-NEXT:                                               "T",
// C89-NEXT:                                           ),
// C89-NEXT:                                       },
// C89-NEXT:                                   ],
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                           Decl(
// C89-NEXT:                               Declaration {
// C89-NEXT:                                   specifiers: DeclarationSpecifiers {
// C89-NEXT:                                       ty: Named(
// C89-NEXT:                                           "T",
// C89-NEXT:                                       ),
// C89-NEXT:                                   },
// C89-NEXT:                                   declarators: [
// C89-NEXT:                                       InitDeclaratorKind {
// C89-NEXT:                                           declarator: Name(
// C89-NEXT:                                               "x",
// C89-NEXT:                                           ),
// C89-NEXT:                                       },
// C89-NEXT:                                   ],
// C89-NEXT:                               },
// C89-NEXT:                           ),
// C89-NEXT:                       ],
// C89-NEXT:                   ),
// C89-NEXT:                   else_branch: None,
// C89-NEXT:               },
// C89-NEXT:               Return(
// C89-NEXT:                   SizeOfExpr(
// C89-NEXT:                       Paren(
// C89-NEXT:                           Identifier(
// C89-NEXT:                               "T",
// C89-NEXT:                           ),
// C89-NEXT:                       ),
// C89-NEXT:                   ),
// C89-NEXT:               ),
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C99
// C99: tag[0]: TagDefinition {
// C99-NEXT:       id: TagId(
// C99-NEXT:           0,
// C99-NEXT:       ),
// C99-NEXT:       kind: Enum,
// C99-NEXT:       name: None,
// C99-NEXT:       body: Enum {
// C99-NEXT:           enumerators: [
// C99-NEXT:               Enumerator(
// C99-NEXT:                   Enumerator {
// C99-NEXT:                       name: "T",
// C99-NEXT:                       value: Some(
// C99-NEXT:                           IntegerLiteral(
// C99-NEXT:                               IntegerLiteral {
// C99-NEXT:                                   value: 1,
// C99-NEXT:                                   radix: Decimal,
// C99-NEXT:                                   suffix: IntegerSuffix {
// C99-NEXT:                                       unsigned: false,
// C99-NEXT:                                       size: None,
// C99-NEXT:                                   },
// C99-NEXT:                                   spelling: "1",
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   }
// C99-NEXT: tag[1]: TagDefinition {
// C99-NEXT:       id: TagId(
// C99-NEXT:           1,
// C99-NEXT:       ),
// C99-NEXT:       kind: Enum,
// C99-NEXT:       name: None,
// C99-NEXT:       body: Enum {
// C99-NEXT:           enumerators: [
// C99-NEXT:               Enumerator(
// C99-NEXT:                   Enumerator {
// C99-NEXT:                       name: "T",
// C99-NEXT:                       value: Some(
// C99-NEXT:                           IntegerLiteral(
// C99-NEXT:                               IntegerLiteral {
// C99-NEXT:                                   value: 1,
// C99-NEXT:                                   radix: Decimal,
// C99-NEXT:                                   suffix: IntegerSuffix {
// C99-NEXT:                                       unsigned: false,
// C99-NEXT:                                       size: None,
// C99-NEXT:                                   },
// C99-NEXT:                                   spelling: "1",
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   }
// C99-NEXT: tag[2]: TagDefinition {
// C99-NEXT:       id: TagId(
// C99-NEXT:           2,
// C99-NEXT:       ),
// C99-NEXT:       kind: Enum,
// C99-NEXT:       name: None,
// C99-NEXT:       body: Enum {
// C99-NEXT:           enumerators: [
// C99-NEXT:               Enumerator(
// C99-NEXT:                   Enumerator {
// C99-NEXT:                       name: "T",
// C99-NEXT:                       value: Some(
// C99-NEXT:                           IntegerLiteral(
// C99-NEXT:                               IntegerLiteral {
// C99-NEXT:                                   value: 1,
// C99-NEXT:                                   radix: Decimal,
// C99-NEXT:                                   suffix: IntegerSuffix {
// C99-NEXT:                                       unsigned: false,
// C99-NEXT:                                       size: None,
// C99-NEXT:                                   },
// C99-NEXT:                                   spelling: "1",
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   }
// C99-NEXT: tag[3]: TagDefinition {
// C99-NEXT:       id: TagId(
// C99-NEXT:           3,
// C99-NEXT:       ),
// C99-NEXT:       kind: Enum,
// C99-NEXT:       name: None,
// C99-NEXT:       body: Enum {
// C99-NEXT:           enumerators: [
// C99-NEXT:               Enumerator(
// C99-NEXT:                   Enumerator {
// C99-NEXT:                       name: "T",
// C99-NEXT:                       value: Some(
// C99-NEXT:                           IntegerLiteral(
// C99-NEXT:                               IntegerLiteral {
// C99-NEXT:                                   value: 1,
// C99-NEXT:                                   radix: Decimal,
// C99-NEXT:                                   suffix: IntegerSuffix {
// C99-NEXT:                                       unsigned: false,
// C99-NEXT:                                       size: None,
// C99-NEXT:                                   },
// C99-NEXT:                                   spelling: "1",
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   }
// C99-NEXT: decl[0]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Void,
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "bodies",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Prototype {
// C99-NEXT:                   parameters: [
// C99-NEXT:                       ParameterDeclarationKind {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Integer(
// C99-NEXT:                                   Ranked {
// C99-NEXT:                                       rank: Int,
// C99-NEXT:                                       signed: true,
// C99-NEXT:                                   },
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Name(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ],
// C99-NEXT:               },
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               If {
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   then_branch: Block(
// C99-NEXT:                       [
// C99-NEXT:                           Expr(
// C99-NEXT:                               Postfix {
// C99-NEXT:                                   op: Increment,
// C99-NEXT:                                   operand: Identifier(
// C99-NEXT:                                       "x",
// C99-NEXT:                                   ),
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: Some(
// C99-NEXT:                       Expr(
// C99-NEXT:                           Postfix {
// C99-NEXT:                               op: Decrement,
// C99-NEXT:                               operand: Identifier(
// C99-NEXT:                                   "x",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                       ),
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               If {
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   then_branch: Expr(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Increment,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: Some(
// C99-NEXT:                       Block(
// C99-NEXT:                           [
// C99-NEXT:                               Expr(
// C99-NEXT:                                   Postfix {
// C99-NEXT:                                       op: Decrement,
// C99-NEXT:                                       operand: Identifier(
// C99-NEXT:                                           "x",
// C99-NEXT:                                       ),
// C99-NEXT:                                   },
// C99-NEXT:                               ),
// C99-NEXT:                           ],
// C99-NEXT:                       ),
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               While {
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   body: Block(
// C99-NEXT:                       [
// C99-NEXT:                           Expr(
// C99-NEXT:                               Postfix {
// C99-NEXT:                                   op: Decrement,
// C99-NEXT:                                   operand: Identifier(
// C99-NEXT:                                       "x",
// C99-NEXT:                                   ),
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               While {
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   body: Expr(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Decrement,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               DoWhile {
// C99-NEXT:                   body: Block(
// C99-NEXT:                       [
// C99-NEXT:                           Expr(
// C99-NEXT:                               Postfix {
// C99-NEXT:                                   op: Increment,
// C99-NEXT:                                   operand: Identifier(
// C99-NEXT:                                       "x",
// C99-NEXT:                                   ),
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               DoWhile {
// C99-NEXT:                   body: Expr(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Increment,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               For {
// C99-NEXT:                   init: None,
// C99-NEXT:                   condition: Some(
// C99-NEXT:                       Identifier(
// C99-NEXT:                           "x",
// C99-NEXT:                       ),
// C99-NEXT:                   ),
// C99-NEXT:                   increment: Some(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Decrement,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   body: Block(
// C99-NEXT:                       [
// C99-NEXT:                           Expr(
// C99-NEXT:                               Postfix {
// C99-NEXT:                                   op: Increment,
// C99-NEXT:                                   operand: Identifier(
// C99-NEXT:                                       "x",
// C99-NEXT:                                   ),
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               For {
// C99-NEXT:                   init: None,
// C99-NEXT:                   condition: Some(
// C99-NEXT:                       Identifier(
// C99-NEXT:                           "x",
// C99-NEXT:                       ),
// C99-NEXT:                   ),
// C99-NEXT:                   increment: Some(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Decrement,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   body: Expr(
// C99-NEXT:                       Postfix {
// C99-NEXT:                           op: Increment,
// C99-NEXT:                           operand: Identifier(
// C99-NEXT:                               "x",
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               Switch {
// C99-NEXT:                   discriminant: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   body: Block(
// C99-NEXT:                       [
// C99-NEXT:                           SwitchLabel {
// C99-NEXT:                               label: Case(
// C99-NEXT:                                   IntegerLiteral(
// C99-NEXT:                                       IntegerLiteral {
// C99-NEXT:                                           value: 0,
// C99-NEXT:                                           radix: Decimal,
// C99-NEXT:                                           suffix: IntegerSuffix {
// C99-NEXT:                                               unsigned: false,
// C99-NEXT:                                               size: None,
// C99-NEXT:                                           },
// C99-NEXT:                                           spelling: "0",
// C99-NEXT:                                       },
// C99-NEXT:                                   ),
// C99-NEXT:                               ),
// C99-NEXT:                               body: Expr(
// C99-NEXT:                                   Postfix {
// C99-NEXT:                                       op: Increment,
// C99-NEXT:                                       operand: Identifier(
// C99-NEXT:                                           "x",
// C99-NEXT:                                       ),
// C99-NEXT:                                   },
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           Break,
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:               Switch {
// C99-NEXT:                   discriminant: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   body: SwitchLabel {
// C99-NEXT:                       label: Default,
// C99-NEXT:                       body: Expr(
// C99-NEXT:                           Postfix {
// C99-NEXT:                               op: Increment,
// C99-NEXT:                               operand: Identifier(
// C99-NEXT:                                   "x",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                       ),
// C99-NEXT:                   },
// C99-NEXT:               },
// C99-NEXT:               If {
// C99-NEXT:                   condition: Identifier(
// C99-NEXT:                       "x",
// C99-NEXT:                   ),
// C99-NEXT:                   then_branch: Block(
// C99-NEXT:                       [],
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: Some(
// C99-NEXT:                       Null,
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[1]: Declaration(
// C99-NEXT:       Declaration {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:               storage: Typedef,
// C99-NEXT:           },
// C99-NEXT:           declarators: [
// C99-NEXT:               InitDeclaratorKind {
// C99-NEXT:                   declarator: Name(
// C99-NEXT:                       "T",
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[2]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "condition_typedef",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               If {
// C99-NEXT:                   condition: SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Tag(
// C99-NEXT:                                   Definition(
// C99-NEXT:                                       TagId(
// C99-NEXT:                                           0,
// C99-NEXT:                                       ),
// C99-NEXT:                                   ),
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:                   then_branch: Expr(
// C99-NEXT:                       Cast {
// C99-NEXT:                           ty: TypeName {
// C99-NEXT:                               specifiers: DeclarationSpecifiers {
// C99-NEXT:                                   ty: Void,
// C99-NEXT:                               },
// C99-NEXT:                               declarator: Abstract,
// C99-NEXT:                           },
// C99-NEXT:                           value: SizeOfExpr(
// C99-NEXT:                               Paren(
// C99-NEXT:                                   Identifier(
// C99-NEXT:                                       "T",
// C99-NEXT:                                   ),
// C99-NEXT:                               ),
// C99-NEXT:                           ),
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: None,
// C99-NEXT:               },
// C99-NEXT:               Return(
// C99-NEXT:                   SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Named(
// C99-NEXT:                                   "T",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[3]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "body_typedef",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               If {
// C99-NEXT:                   condition: IntegerLiteral(
// C99-NEXT:                       IntegerLiteral {
// C99-NEXT:                           value: 1,
// C99-NEXT:                           radix: Decimal,
// C99-NEXT:                           suffix: IntegerSuffix {
// C99-NEXT:                               unsigned: false,
// C99-NEXT:                               size: None,
// C99-NEXT:                           },
// C99-NEXT:                           spelling: "1",
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   then_branch: Expr(
// C99-NEXT:                       Cast {
// C99-NEXT:                           ty: TypeName {
// C99-NEXT:                               specifiers: DeclarationSpecifiers {
// C99-NEXT:                                   ty: Void,
// C99-NEXT:                               },
// C99-NEXT:                               declarator: Abstract,
// C99-NEXT:                           },
// C99-NEXT:                           value: SizeOfType {
// C99-NEXT:                               ty: TypeName {
// C99-NEXT:                                   specifiers: DeclarationSpecifiers {
// C99-NEXT:                                       ty: Tag(
// C99-NEXT:                                           Definition(
// C99-NEXT:                                               TagId(
// C99-NEXT:                                                   1,
// C99-NEXT:                                               ),
// C99-NEXT:                                           ),
// C99-NEXT:                                       ),
// C99-NEXT:                                   },
// C99-NEXT:                                   declarator: Abstract,
// C99-NEXT:                               },
// C99-NEXT:                           },
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: None,
// C99-NEXT:               },
// C99-NEXT:               Return(
// C99-NEXT:                   SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Named(
// C99-NEXT:                                   "T",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[4]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "do_typedef",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               DoWhile {
// C99-NEXT:                   body: Expr(
// C99-NEXT:                       Cast {
// C99-NEXT:                           ty: TypeName {
// C99-NEXT:                               specifiers: DeclarationSpecifiers {
// C99-NEXT:                                   ty: Void,
// C99-NEXT:                               },
// C99-NEXT:                               declarator: Abstract,
// C99-NEXT:                           },
// C99-NEXT:                           value: SizeOfType {
// C99-NEXT:                               ty: TypeName {
// C99-NEXT:                                   specifiers: DeclarationSpecifiers {
// C99-NEXT:                                       ty: Tag(
// C99-NEXT:                                           Definition(
// C99-NEXT:                                               TagId(
// C99-NEXT:                                                   2,
// C99-NEXT:                                               ),
// C99-NEXT:                                           ),
// C99-NEXT:                                       ),
// C99-NEXT:                                   },
// C99-NEXT:                                   declarator: Abstract,
// C99-NEXT:                               },
// C99-NEXT:                           },
// C99-NEXT:                       },
// C99-NEXT:                   ),
// C99-NEXT:                   condition: SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Named(
// C99-NEXT:                                   "T",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:               },
// C99-NEXT:               Return(
// C99-NEXT:                   SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Named(
// C99-NEXT:                                   "T",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[5]: Function(
// C99-NEXT:       FunctionDefinition {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarator: Function {
// C99-NEXT:               inner: Name(
// C99-NEXT:                   "nested_typedef",
// C99-NEXT:               ),
// C99-NEXT:               parameters: Void,
// C99-NEXT:           },
// C99-NEXT:           body: [
// C99-NEXT:               If {
// C99-NEXT:                   condition: SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Tag(
// C99-NEXT:                                   Definition(
// C99-NEXT:                                       TagId(
// C99-NEXT:                                           3,
// C99-NEXT:                                       ),
// C99-NEXT:                                   ),
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:                   then_branch: Block(
// C99-NEXT:                       [
// C99-NEXT:                           Decl(
// C99-NEXT:                               Declaration {
// C99-NEXT:                                   specifiers: DeclarationSpecifiers {
// C99-NEXT:                                       ty: Integer(
// C99-NEXT:                                           Ranked {
// C99-NEXT:                                               rank: Int,
// C99-NEXT:                                               signed: true,
// C99-NEXT:                                           },
// C99-NEXT:                                       ),
// C99-NEXT:                                       storage: Typedef,
// C99-NEXT:                                   },
// C99-NEXT:                                   declarators: [
// C99-NEXT:                                       InitDeclaratorKind {
// C99-NEXT:                                           declarator: Name(
// C99-NEXT:                                               "T",
// C99-NEXT:                                           ),
// C99-NEXT:                                       },
// C99-NEXT:                                   ],
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                           Decl(
// C99-NEXT:                               Declaration {
// C99-NEXT:                                   specifiers: DeclarationSpecifiers {
// C99-NEXT:                                       ty: Named(
// C99-NEXT:                                           "T",
// C99-NEXT:                                       ),
// C99-NEXT:                                   },
// C99-NEXT:                                   declarators: [
// C99-NEXT:                                       InitDeclaratorKind {
// C99-NEXT:                                           declarator: Name(
// C99-NEXT:                                               "x",
// C99-NEXT:                                           ),
// C99-NEXT:                                       },
// C99-NEXT:                                   ],
// C99-NEXT:                               },
// C99-NEXT:                           ),
// C99-NEXT:                       ],
// C99-NEXT:                   ),
// C99-NEXT:                   else_branch: None,
// C99-NEXT:               },
// C99-NEXT:               Return(
// C99-NEXT:                   SizeOfType {
// C99-NEXT:                       ty: TypeName {
// C99-NEXT:                           specifiers: DeclarationSpecifiers {
// C99-NEXT:                               ty: Named(
// C99-NEXT:                                   "T",
// C99-NEXT:                               ),
// C99-NEXT:                           },
// C99-NEXT:                           declarator: Abstract,
// C99-NEXT:                       },
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// SLATE-FILECHECK-END C99
