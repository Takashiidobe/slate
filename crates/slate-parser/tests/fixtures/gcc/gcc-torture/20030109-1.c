// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

void foo ()
{
  int x1, x2, x3;

  bar (&x2 - &x1, &x3 - &x2);
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "foo",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
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
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x3",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "bar",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "x2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "x1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "x3",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "x2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
