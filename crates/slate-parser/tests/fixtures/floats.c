#include <stdio.h>

static double avg(double a, double b) {
  double c = (a + b) / 2.0;
  return c;
}

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f\n", x + 0.5f);
  printf("%f\n", avg(3.0, 4.0));
  printf("%.2f\n", y * 2.0);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Char {
// DEFAULT-NEXT:                                               signed: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           variadic: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "avg",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "2.0",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "c",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
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
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       FloatLiteral(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               spelling: "1.5f",
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: F,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "y",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       FloatLiteral(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               spelling: "2.25",
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       102,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "%f\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.5f",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: F,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       102,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "%f\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "avg",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   FloatLiteral(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           spelling: "3.0",
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   FloatLiteral(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           spelling: "4.0",
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       46,
// DEFAULT-NEXT:                                       50,
// DEFAULT-NEXT:                                       102,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "%.2f\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "y",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "2.0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:               line: 7,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
