#include <complex.h>
#include <stdio.h>

int main(void) {
  volatile double real      = 0.5;
  volatile double imaginary = -0.25;
  double complex  a         = __builtin_complex(real, imaginary);
  double complex  b         = __builtin_complex(real, -imaginary);
  double complex  product;
  double complex  quotient;

  {
#pragma STDC CX_LIMITED_RANGE ON
    product = a * b;
  }

  {
#pragma STDC CX_LIMITED_RANGE OFF
    quotient = a / b;
  }

  printf("%.4f %.4f %.4f %.4f\n", creal(product), cimag(product),
         creal(quotient), cimag(quotient));
  return creal(product) == 0.3125 && cimag(product) == 0.0 &&
                 creal(quotient) == 0.6 && cimag(quotient) == -0.8
             ? 0
             : 1;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
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
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_volatile: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "real",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.5,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_volatile: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "imaginary",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.25,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Complex(
// DEFAULT-NEXT:                               Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_complex",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "real",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "imaginary",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Complex(
// DEFAULT-NEXT:                               Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "b",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_complex",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "real",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "imaginary",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Complex(
// DEFAULT-NEXT:                               Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "product",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Complex(
// DEFAULT-NEXT:                               Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "quotient",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "product",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "a",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "quotient",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Div,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "a",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%.4f %.4f %.4f %.4f\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "creal",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "product",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "cimag",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "product",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "creal",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "quotient",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "cimag",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "quotient",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: And,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: And,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "creal",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "product",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Double(
// DEFAULT-NEXT:                                                       0.3125,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "cimag",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "product",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Double(
// DEFAULT-NEXT:                                                       0.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "creal",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "quotient",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.6,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "cimag",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "quotient",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.8,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
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
