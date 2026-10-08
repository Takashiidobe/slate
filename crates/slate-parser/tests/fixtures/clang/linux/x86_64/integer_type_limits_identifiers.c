int _Maxof(int x) { return x; }
int _Minof(int x) { return -x; }
int legacy(int n) { return _Maxof(n) + _Minof(n); }

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23

// SLATE-FILECHECK-BEGIN C23
// C23: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "_Maxof",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Prototype {
// C23-NEXT:                   parameters: [
// C23-NEXT:                       ParameterDeclarationKind {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Name(
// C23-NEXT:                               "x",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                   ],
// C23-NEXT:               },
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Return(
// C23-NEXT:                   Identifier(
// C23-NEXT:                       "x",
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "_Minof",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Prototype {
// C23-NEXT:                   parameters: [
// C23-NEXT:                       ParameterDeclarationKind {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Name(
// C23-NEXT:                               "x",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                   ],
// C23-NEXT:               },
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Return(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: Minus,
// C23-NEXT:                       operand: Identifier(
// C23-NEXT:                           "x",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "legacy",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Prototype {
// C23-NEXT:                   parameters: [
// C23-NEXT:                       ParameterDeclarationKind {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Name(
// C23-NEXT:                               "n",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                   ],
// C23-NEXT:               },
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Return(
// C23-NEXT:                   Binary {
// C23-NEXT:                       op: Add,
// C23-NEXT:                       left: Call {
// C23-NEXT:                           callee: Identifier(
// C23-NEXT:                               "_Maxof",
// C23-NEXT:                           ),
// C23-NEXT:                           arguments: [
// C23-NEXT:                               Identifier(
// C23-NEXT:                                   "n",
// C23-NEXT:                               ),
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                       right: Call {
// C23-NEXT:                           callee: Identifier(
// C23-NEXT:                               "_Minof",
// C23-NEXT:                           ),
// C23-NEXT:                           arguments: [
// C23-NEXT:                               Identifier(
// C23-NEXT:                                   "n",
// C23-NEXT:                               ),
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: decl[{{[0-9]+}}]: Function(
// GNU23-NEXT:       FunctionDefinition {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Integer(
// GNU23-NEXT:                   Ranked {
// GNU23-NEXT:                       rank: Int,
// GNU23-NEXT:                       signed: true,
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           },
// GNU23-NEXT:           declarator: Function {
// GNU23-NEXT:               inner: Name(
// GNU23-NEXT:                   "_Maxof",
// GNU23-NEXT:               ),
// GNU23-NEXT:               parameters: Prototype {
// GNU23-NEXT:                   parameters: [
// GNU23-NEXT:                       ParameterDeclarationKind {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Name(
// GNU23-NEXT:                               "x",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                       },
// GNU23-NEXT:                   ],
// GNU23-NEXT:               },
// GNU23-NEXT:           },
// GNU23-NEXT:           body: [
// GNU23-NEXT:               Return(
// GNU23-NEXT:                   Identifier(
// GNU23-NEXT:                       "x",
// GNU23-NEXT:                   ),
// GNU23-NEXT:               ),
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// GNU23-NEXT: decl[{{[0-9]+}}]: Function(
// GNU23-NEXT:       FunctionDefinition {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Integer(
// GNU23-NEXT:                   Ranked {
// GNU23-NEXT:                       rank: Int,
// GNU23-NEXT:                       signed: true,
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           },
// GNU23-NEXT:           declarator: Function {
// GNU23-NEXT:               inner: Name(
// GNU23-NEXT:                   "_Minof",
// GNU23-NEXT:               ),
// GNU23-NEXT:               parameters: Prototype {
// GNU23-NEXT:                   parameters: [
// GNU23-NEXT:                       ParameterDeclarationKind {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Name(
// GNU23-NEXT:                               "x",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                       },
// GNU23-NEXT:                   ],
// GNU23-NEXT:               },
// GNU23-NEXT:           },
// GNU23-NEXT:           body: [
// GNU23-NEXT:               Return(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: Minus,
// GNU23-NEXT:                       operand: Identifier(
// GNU23-NEXT:                           "x",
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// GNU23-NEXT: decl[{{[0-9]+}}]: Function(
// GNU23-NEXT:       FunctionDefinition {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Integer(
// GNU23-NEXT:                   Ranked {
// GNU23-NEXT:                       rank: Int,
// GNU23-NEXT:                       signed: true,
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           },
// GNU23-NEXT:           declarator: Function {
// GNU23-NEXT:               inner: Name(
// GNU23-NEXT:                   "legacy",
// GNU23-NEXT:               ),
// GNU23-NEXT:               parameters: Prototype {
// GNU23-NEXT:                   parameters: [
// GNU23-NEXT:                       ParameterDeclarationKind {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Name(
// GNU23-NEXT:                               "n",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                       },
// GNU23-NEXT:                   ],
// GNU23-NEXT:               },
// GNU23-NEXT:           },
// GNU23-NEXT:           body: [
// GNU23-NEXT:               Return(
// GNU23-NEXT:                   Binary {
// GNU23-NEXT:                       op: Add,
// GNU23-NEXT:                       left: Call {
// GNU23-NEXT:                           callee: Identifier(
// GNU23-NEXT:                               "_Maxof",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                           arguments: [
// GNU23-NEXT:                               Identifier(
// GNU23-NEXT:                                   "n",
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                       right: Call {
// GNU23-NEXT:                           callee: Identifier(
// GNU23-NEXT:                               "_Minof",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                           arguments: [
// GNU23-NEXT:                               Identifier(
// GNU23-NEXT:                                   "n",
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// SLATE-FILECHECK-END GNU23
