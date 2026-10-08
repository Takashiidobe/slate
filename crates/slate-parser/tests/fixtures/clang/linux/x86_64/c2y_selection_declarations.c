void selections(int n) {
    if (int x = n) n = x;
    else n = x + 1;
    if (int x = n; x > 0) n = x;
    switch (int x = n) { default: n = x; break; }
    switch (int x = n; x + 1) { default: n = x; break; }
}

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-ERROR GNU23

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × selection statement declarations require C2y
// C23: ╰─▶ selection statement declarations require C2y
// C23: ╭─[tests/fixtures/clang/linux/x86_64/c2y_selection_declarations.c:2:9]
// C23: 1 │ void selections(int n) {
// C23: 2 │     if (int x = n) n = x;
// C23: ·         ───
// C23: 3 │     else n = x + 1;
// C23: ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: Error:   × selection statement declarations require C2y
// GNU23: ╰─▶ selection statement declarations require C2y
// GNU23: ╭─[tests/fixtures/clang/linux/x86_64/c2y_selection_declarations.c:2:9]
// GNU23: 1 │ void selections(int n) {
// GNU23: 2 │     if (int x = n) n = x;
// GNU23: ·         ───
// GNU23: 3 │     else n = x + 1;
// GNU23: ╰────
// SLATE-FILECHECK-END GNU23
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Void,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "selections",
// C2Y-NEXT:               ),
// C2Y-NEXT:               parameters: Prototype {
// C2Y-NEXT:                   parameters: [
// C2Y-NEXT:                       ParameterDeclarationKind {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Name(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: None,
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: Some(
// C2Y-NEXT:                       Expr(
// C2Y-NEXT:                           Assign {
// C2Y-NEXT:                               op: Assign,
// C2Y-NEXT:                               target: Identifier(
// C2Y-NEXT:                                   "n",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               value: Binary {
// C2Y-NEXT:                                   op: Add,
// C2Y-NEXT:                                   left: Identifier(
// C2Y-NEXT:                                       "x",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   right: IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 1,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "1",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Binary {
// C2Y-NEXT:                           op: Greater,
// C2Y-NEXT:                           left: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           right: IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 0,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "0",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               SwitchDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   discriminant: None,
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Identifier(
// C2Y-NEXT:                                           "x",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Break,
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               SwitchDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   discriminant: Some(
// C2Y-NEXT:                       Binary {
// C2Y-NEXT:                           op: Add,
// C2Y-NEXT:                           left: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           right: IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 1,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "1",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Identifier(
// C2Y-NEXT:                                           "x",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Break,
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Void,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "selections",
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               parameters: Prototype {
// GNU2Y-NEXT:                   parameters: [
// GNU2Y-NEXT:                       ParameterDeclarationKind {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Name(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: None,
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: Some(
// GNU2Y-NEXT:                       Expr(
// GNU2Y-NEXT:                           Assign {
// GNU2Y-NEXT:                               op: Assign,
// GNU2Y-NEXT:                               target: Identifier(
// GNU2Y-NEXT:                                   "n",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               value: Binary {
// GNU2Y-NEXT:                                   op: Add,
// GNU2Y-NEXT:                                   left: Identifier(
// GNU2Y-NEXT:                                       "x",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   right: IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 1,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "1",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Binary {
// GNU2Y-NEXT:                           op: Greater,
// GNU2Y-NEXT:                           left: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           right: IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 0,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "0",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               SwitchDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   discriminant: None,
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Identifier(
// GNU2Y-NEXT:                                           "x",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Break,
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               SwitchDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   discriminant: Some(
// GNU2Y-NEXT:                       Binary {
// GNU2Y-NEXT:                           op: Add,
// GNU2Y-NEXT:                           left: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           right: IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 1,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "1",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Identifier(
// GNU2Y-NEXT:                                           "x",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Break,
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
