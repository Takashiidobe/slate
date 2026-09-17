// SLATE-FILECHECK-DEFINES AST
// SLATE-FILECHECK-STD AST gnu23

int attributed(int x) {
    [[vendor::hint]] if (x) [[vendor::hint]] return 1; else return 2;
    [[vendor::hint]] while (x) { x--; }
    [[vendor::hint]] for (; x; x--) ;
    [[vendor::hint]] goto done;
    [[vendor::hint]] done: x++;
    switch (x) {
    [[vendor::hint]] case 1: x++;
    [[fallthrough]];
    default: break;
    }
    [[maybe_unused]] int local = x;
    return local;
}

// SLATE-FILECHECK-BEGIN AST
// AST: decl[0]: Function(
// AST-NEXT:       FunctionDefinition {
// AST-NEXT:           specifiers: DeclarationSpecifiers {
// AST-NEXT:               ty: Integer(
// AST-NEXT:                   Ranked {
// AST-NEXT:                       rank: Int,
// AST-NEXT:                       signed: true,
// AST-NEXT:                   },
// AST-NEXT:               ),
// AST-NEXT:           },
// AST-NEXT:           declarator: Function {
// AST-NEXT:               inner: Name(
// AST-NEXT:                   "attributed",
// AST-NEXT:               ),
// AST-NEXT:               parameters: Prototype {
// AST-NEXT:                   parameters: [
// AST-NEXT:                       ParameterDeclarationKind {
// AST-NEXT:                           specifiers: DeclarationSpecifiers {
// AST-NEXT:                               ty: Integer(
// AST-NEXT:                                   Ranked {
// AST-NEXT:                                       rank: Int,
// AST-NEXT:                                       signed: true,
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                           declarator: Name(
// AST-NEXT:                               "x",
// AST-NEXT:                           ),
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:               },
// AST-NEXT:           },
// AST-NEXT:           body: [
// AST-NEXT:               Attributed {
// AST-NEXT:                   attributes: [
// AST-NEXT:                       Unknown {
// AST-NEXT:                           name: "vendor::hint",
// AST-NEXT:                           arguments: [],
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:                   body: If {
// AST-NEXT:                       condition: Identifier(
// AST-NEXT:                           "x",
// AST-NEXT:                       ),
// AST-NEXT:                       then_branch: Attributed {
// AST-NEXT:                           attributes: [
// AST-NEXT:                               Unknown {
// AST-NEXT:                                   name: "vendor::hint",
// AST-NEXT:                                   arguments: [],
// AST-NEXT:                               },
// AST-NEXT:                           ],
// AST-NEXT:                           body: Return(
// AST-NEXT:                               IntegerLiteral(
// AST-NEXT:                                   IntegerLiteral {
// AST-NEXT:                                       value: 1,
// AST-NEXT:                                       radix: Decimal,
// AST-NEXT:                                       suffix: IntegerSuffix {
// AST-NEXT:                                           unsigned: false,
// AST-NEXT:                                           size: None,
// AST-NEXT:                                       },
// AST-NEXT:                                       spelling: "1",
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                           ),
// AST-NEXT:                       },
// AST-NEXT:                       else_branch: Some(
// AST-NEXT:                           Return(
// AST-NEXT:                               IntegerLiteral(
// AST-NEXT:                                   IntegerLiteral {
// AST-NEXT:                                       value: 2,
// AST-NEXT:                                       radix: Decimal,
// AST-NEXT:                                       suffix: IntegerSuffix {
// AST-NEXT:                                           unsigned: false,
// AST-NEXT:                                           size: None,
// AST-NEXT:                                       },
// AST-NEXT:                                       spelling: "2",
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                           ),
// AST-NEXT:                       ),
// AST-NEXT:                   },
// AST-NEXT:               },
// AST-NEXT:               Attributed {
// AST-NEXT:                   attributes: [
// AST-NEXT:                       Unknown {
// AST-NEXT:                           name: "vendor::hint",
// AST-NEXT:                           arguments: [],
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:                   body: While {
// AST-NEXT:                       condition: Identifier(
// AST-NEXT:                           "x",
// AST-NEXT:                       ),
// AST-NEXT:                       body: Block(
// AST-NEXT:                           [
// AST-NEXT:                               Expr(
// AST-NEXT:                                   Postfix {
// AST-NEXT:                                       op: Decrement,
// AST-NEXT:                                       operand: Identifier(
// AST-NEXT:                                           "x",
// AST-NEXT:                                       ),
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                           ],
// AST-NEXT:                       ),
// AST-NEXT:                   },
// AST-NEXT:               },
// AST-NEXT:               Attributed {
// AST-NEXT:                   attributes: [
// AST-NEXT:                       Unknown {
// AST-NEXT:                           name: "vendor::hint",
// AST-NEXT:                           arguments: [],
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:                   body: For {
// AST-NEXT:                       init: None,
// AST-NEXT:                       condition: Some(
// AST-NEXT:                           Identifier(
// AST-NEXT:                               "x",
// AST-NEXT:                           ),
// AST-NEXT:                       ),
// AST-NEXT:                       increment: Some(
// AST-NEXT:                           Postfix {
// AST-NEXT:                               op: Decrement,
// AST-NEXT:                               operand: Identifier(
// AST-NEXT:                                   "x",
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                       ),
// AST-NEXT:                       body: Null,
// AST-NEXT:                   },
// AST-NEXT:               },
// AST-NEXT:               Attributed {
// AST-NEXT:                   attributes: [
// AST-NEXT:                       Unknown {
// AST-NEXT:                           name: "vendor::hint",
// AST-NEXT:                           arguments: [],
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:                   body: Goto(
// AST-NEXT:                       "done",
// AST-NEXT:                   ),
// AST-NEXT:               },
// AST-NEXT:               Attributed {
// AST-NEXT:                   attributes: [
// AST-NEXT:                       Unknown {
// AST-NEXT:                           name: "vendor::hint",
// AST-NEXT:                           arguments: [],
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:                   body: Labeled {
// AST-NEXT:                       label: "done",
// AST-NEXT:                       body: Expr(
// AST-NEXT:                           Postfix {
// AST-NEXT:                               op: Increment,
// AST-NEXT:                               operand: Identifier(
// AST-NEXT:                                   "x",
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                       ),
// AST-NEXT:                   },
// AST-NEXT:               },
// AST-NEXT:               Switch {
// AST-NEXT:                   discriminant: Identifier(
// AST-NEXT:                       "x",
// AST-NEXT:                   ),
// AST-NEXT:                   body: Block(
// AST-NEXT:                       [
// AST-NEXT:                           Attributed {
// AST-NEXT:                               attributes: [
// AST-NEXT:                                   Unknown {
// AST-NEXT:                                       name: "vendor::hint",
// AST-NEXT:                                       arguments: [],
// AST-NEXT:                                   },
// AST-NEXT:                               ],
// AST-NEXT:                               body: SwitchLabel {
// AST-NEXT:                                   label: Case(
// AST-NEXT:                                       IntegerLiteral(
// AST-NEXT:                                           IntegerLiteral {
// AST-NEXT:                                               value: 1,
// AST-NEXT:                                               radix: Decimal,
// AST-NEXT:                                               suffix: IntegerSuffix {
// AST-NEXT:                                                   unsigned: false,
// AST-NEXT:                                                   size: None,
// AST-NEXT:                                               },
// AST-NEXT:                                               spelling: "1",
// AST-NEXT:                                           },
// AST-NEXT:                                       ),
// AST-NEXT:                                   ),
// AST-NEXT:                                   body: Expr(
// AST-NEXT:                                       Postfix {
// AST-NEXT:                                           op: Increment,
// AST-NEXT:                                           operand: Identifier(
// AST-NEXT:                                               "x",
// AST-NEXT:                                           ),
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               },
// AST-NEXT:                           },
// AST-NEXT:                           Attribute(
// AST-NEXT:                               [
// AST-NEXT:                                   Fallthrough,
// AST-NEXT:                               ],
// AST-NEXT:                           ),
// AST-NEXT:                           SwitchLabel {
// AST-NEXT:                               label: Default,
// AST-NEXT:                               body: Break,
// AST-NEXT:                           },
// AST-NEXT:                       ],
// AST-NEXT:                   ),
// AST-NEXT:               },
// AST-NEXT:               Decl(
// AST-NEXT:                   Declaration {
// AST-NEXT:                       specifiers: DeclarationSpecifiers {
// AST-NEXT:                           ty: Integer(
// AST-NEXT:                               Ranked {
// AST-NEXT:                                   rank: Int,
// AST-NEXT:                                   signed: true,
// AST-NEXT:                               },
// AST-NEXT:                           ),
// AST-NEXT:                           attributes: [
// AST-NEXT:                               MaybeUnused,
// AST-NEXT:                           ],
// AST-NEXT:                       },
// AST-NEXT:                       declarators: [
// AST-NEXT:                           InitDeclaratorKind {
// AST-NEXT:                               declarator: Name(
// AST-NEXT:                                   "local",
// AST-NEXT:                               ),
// AST-NEXT:                               initializer: Some(
// AST-NEXT:                                   Expr(
// AST-NEXT:                                       Identifier(
// AST-NEXT:                                           "x",
// AST-NEXT:                                       ),
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                       ],
// AST-NEXT:                   },
// AST-NEXT:               ),
// AST-NEXT:               Return(
// AST-NEXT:                   Identifier(
// AST-NEXT:                       "local",
// AST-NEXT:                   ),
// AST-NEXT:               ),
// AST-NEXT:           ],
// AST-NEXT:       },
// AST-NEXT:   )
// SLATE-FILECHECK-END AST
