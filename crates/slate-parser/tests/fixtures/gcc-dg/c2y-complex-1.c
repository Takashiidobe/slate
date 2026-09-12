/* Test C2Y complex increment and decrement.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort(void);
extern void exit(int);

_Complex float       a, ax;
_Complex double      b, bx;
_Complex long double c, cx;

int
main() {
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();

  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();

  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();

  exit(0);
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* Test C2Y complex increment and decrement.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 48,
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
// DEFAULT-NEXT: decl[1]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-do run } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 49,
// DEFAULT-NEXT:           length: 19,
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
// DEFAULT-NEXT: decl[2]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-options \"-std=c2y -pedantic-errors\" } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 69,
// DEFAULT-NEXT:           length: 48,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "abort",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "exit",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       Float,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "a",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       Float,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "ax",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       Double,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "b",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       Double,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "bx",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       LongDouble,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       LongDouble,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "cx",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[11]: Function(
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
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "ax",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_crealf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_crealf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "ax",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_crealf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_crealf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "ax",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_crealf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_crealf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_crealf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_crealf",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "ax",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "ax",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_crealf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "ax",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ax",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_crealf",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "ax",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagf",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ax",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_crealf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimag",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "bx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creal",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimag",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimag",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creal",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimag",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimag",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "bx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creal",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimag",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimag",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creal",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimag",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimag",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "bx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creal",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimag",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creal",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimag",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creal",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimag",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_creal",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "bx",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimag",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "bx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creal",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimag",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               0.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "bx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "bx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_creal",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "bx",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimag",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "bx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creal",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimag",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "cx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creall",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagl",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: LongDouble(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagl",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creall",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagl",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "cx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creall",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagl",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: LongDouble(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagl",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_creall",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagl",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "cx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creall",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagl",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: LongDouble(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_creall",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagl",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creall",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagl",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_creall",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "cx",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_cimagl",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "cx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creall",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_signbit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_cimagl",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_complex",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: LongDouble(
// DEFAULT-NEXT:                                               "0.0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: LongDouble(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "cx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: PreDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "cx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_signbit",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "__builtin_creall",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "cx",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_signbit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_cimagl",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Not,
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_signbit",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_creall",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Unary {
// DEFAULT-NEXT:                               op: Not,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_signbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__builtin_cimagl",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "exit",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 11,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
