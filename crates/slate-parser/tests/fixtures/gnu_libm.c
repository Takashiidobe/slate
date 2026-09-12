#define _GNU_SOURCE
#include <complex.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>

static int gnu_sincos_extensions(void) {
  double      sine     = 0.0;
  double      cosine   = 0.0;
  float       sine_f   = 0.0f;
  float       cosine_f = 0.0f;
  long double sine_l   = 0.0L;
  long double cosine_l = 0.0L;
  int         total    = 0;

  sincos(0.5, &sine, &cosine);
  sincosf(0.5f, &sine_f, &cosine_f);
  sincosl(0.5L, &sine_l, &cosine_l);

  total += fabs(sine - sin(0.5)) == 0.0;
  total += fabs(cosine - cos(0.5)) == 0.0;
  total += fabsf(sine_f - sinf(0.5f)) == 0.0f;
  total += fabsf(cosine_f - cosf(0.5f)) == 0.0f;
  total += fabsl(sine_l - sinl(0.5L)) == 0.0L;
  total += fabsl(cosine_l - cosl(0.5L)) == 0.0L;
  return total;
}

static int gnu_exponential_extensions(void) {
  int sign  = 0;
  int total = 0;

  total += exp10(2.0) == 100.0;
  total += exp10f(2.0f) == 100.0f;
  total += exp10l(2.0L) == 100.0L;
  total += lgamma_r(0.5, &sign) == lgamma(0.5);
  total += sign == 1;
  total += significand(12.0) == 1.5;
  total += significandf(12.0f) == 1.5f;
  total += drem(7.0, 3.0) == 1.0;
  total += dremf(7.0f, 3.0f) == 1.0f;
  total += scalb(1.5, 3.0) == 12.0;
  total += scalbf(1.5f, 3.0f) == 12.0f;
  return total;
}

static int gnu_bessel_extensions(void) {
  int total = 0;

  total += j0(1.0) == j0(1.0) && j0(1.0) > 0.7;
  total += j1(1.0) > 0.4;
  total += jn(2, 1.0) > 0.1;
  total += y0(1.0) > 0.08;
  total += y1(1.0) < 0.0;
  total += yn(2, 1.0) < 0.0;
  total += j0f(1.0f) > 0.7f;
  total += y0f(1.0f) > 0.08f;
  return total;
}

static int gnu_fenv_extensions(void) {
  int total = 0;

  total += feenableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) != 0;
  total += fedisableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) == 0;
  return total;
}

static int gnu_constant_extensions(void) {
  int total = 0;

  total += (double)M_PIl == M_PI;
  total += (double)M_El == M_E;
  total += (double)M_SQRT2l == M_SQRT2;
  total += (double)M_LN2l == M_LN2;
  return total;
}

int main(void) {
  printf("%d %d %d %d %d\n", gnu_sincos_extensions(),
         gnu_exponential_extensions(), gnu_bessel_extensions(),
         gnu_fenv_extensions(), gnu_constant_extensions());
  return 0;
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
// DEFAULT-NEXT:           name: "gnu_sincos_extensions",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "sine",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.0,
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
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "cosine",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.0,
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
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "sine_f",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
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
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "cosine_f",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
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
// DEFAULT-NEXT:                               LongDouble,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "sine_l",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
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
// DEFAULT-NEXT:                               LongDouble,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "cosine_l",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "sincos",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "sine",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "cosine",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "sincosf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           0.5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "sine_f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "cosine_f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "sincosl",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: LongDouble(
// DEFAULT-NEXT:                                           "0.5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "sine_l",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "cosine_l",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabs",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "sine",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "sin",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Double(
// DEFAULT-NEXT:                                                               0.5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabs",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "cosine",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "cos",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Double(
// DEFAULT-NEXT:                                                               0.5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabsf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "sine_f",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "sinf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Single(
// DEFAULT-NEXT:                                                               0.5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabsf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "cosine_f",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "cosf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Single(
// DEFAULT-NEXT:                                                               0.5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabsl",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "sine_l",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "sinl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: LongDouble(
// DEFAULT-NEXT:                                                               "0.5",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: LongDouble(
// DEFAULT-NEXT:                                           "0.0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fabsl",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "cosine_l",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "cosl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: LongDouble(
// DEFAULT-NEXT:                                                               "0.5",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: LongDouble(
// DEFAULT-NEXT:                                           "0.0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 6,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_exponential_extensions",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "sign",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "exp10",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   2.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           100.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "exp10f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   2.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           100.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "exp10l",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: LongDouble(
// DEFAULT-NEXT:                                                   "2.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: LongDouble(
// DEFAULT-NEXT:                                           "100.0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "lgamma_r",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       AddrOf(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "sign",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "lgamma",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "sign",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "significand",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   12.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           1.5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "significandf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   12.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           1.5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "drem",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   7.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   3.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           1.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "dremf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   7.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   3.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           1.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "scalb",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   3.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           12.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "scalbf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   1.5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   3.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           12.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 28,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_bessel_extensions",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: And,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "j0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Double(
// DEFAULT-NEXT:                                                       1.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "j0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Double(
// DEFAULT-NEXT:                                                       1.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Greater,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "j0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Double(
// DEFAULT-NEXT:                                                       1.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.7,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Greater,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "j1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Greater,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "jn",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Greater,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "y0",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.08,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "y1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "yn",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Greater,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "j0f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           0.7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Greater,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "y0f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   1.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           0.08,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 46,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
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
// DEFAULT-NEXT:           name: "gnu_fenv_extensions",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "feenableexcept",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitAnd,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "fegetexcept",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       4,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "fedisableexcept",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitAnd,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "fegetexcept",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       4,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 60,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_constant_extensions",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Cast {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Double,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "3.141592653589793238462643383279502884",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           3.141592653589793,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Cast {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Double,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "2.718281828459045235360287471352662498",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           2.718281828459045,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Cast {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Double,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "1.414213562373095048801688724209698079",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           1.4142135623730951,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Cast {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Double,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.693147180559945309417232121458176568",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           0.6931471805599453,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 70,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%d %d %d %d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_sincos_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_exponential_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_bessel_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_fenv_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_constant_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 80,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Comment {
// DEFAULT-NEXT:       text: "// LOWERING: #![feature(f128)]",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 2270,
// DEFAULT-NEXT:           length: 30,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 87,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Comment {
// DEFAULT-NEXT:       text: "// REWRITES: #![feature(f128)]",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 2302,
// DEFAULT-NEXT:           length: 30,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 89,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
