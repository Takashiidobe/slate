void abort(void);
void exit(int);

long long simple_rand() {
  static unsigned long long seed = 47114711;
  unsigned long long this        = seed * 1103515245 + 12345;
  seed                           = this;
  return this >> 8;
}

unsigned long long int random_bitstring() {
  unsigned long long int x;
  int                    n_bits;
  long long              ran;
  int                    tot_bits = 0;

  x = 0;
  for (;;) {
    ran       = simple_rand();
    n_bits    = (ran >> 1) % 16;
    tot_bits += n_bits;

    if (n_bits == 0)
      return x;
    else {
      x <<= n_bits;
      if (ran & 1)
        x |= (1 << n_bits) - 1;

      if (tot_bits > 8 * sizeof(long long) + 6)
        return x;
    }
  }
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

int main(void) {
  long long int i;

  for (i = 0; i < 10000; i++) {
    unsigned long long x, y;
    x = random_bitstring();
    y = random_bitstring();

    if (sizeof(int) == sizeof(long long))
      goto save_time;

    {
      unsigned long long xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed long long xx = x, yy = y, r1, r2;
      if ((unsigned long long)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned long long)ABS(yy) ||
          (signed long long)(r1 * yy + r2) != xx)
        abort();
    }
  save_time: {
    unsigned int xx = x, yy = y, r1, r2;
    if (yy == 0)
      continue;
    r1 = xx / yy;
    r2 = xx % yy;
    if (r2 >= yy || r1 * yy + r2 != xx)
      abort();
  }
    {
      signed int xx = x, yy = y, r1, r2;
      if ((unsigned int)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned int)ABS(yy) ||
          (signed int)(r1 * yy + r2) != xx || ((xx < 0) != (r2 < 0) && r2))
        abort();
    }
    {
      unsigned short xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed short xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned short)ABS(yy) ||
          (signed short)(r1 * yy + r2) != xx)
        abort();
    }
    {
      unsigned char xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed char xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned char)ABS(yy) ||
          (signed char)(r1 * yy + r2) != xx)
        abort();
    }
  }

  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
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
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: LongLong,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "simple_rand",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           storage: Static,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "seed",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               47114711,
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "this",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "seed",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1103515245,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   12345,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "seed",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "this",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: ShiftRight,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "this",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               8,
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
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: LongLong,
// DEFAULT-NEXT:                   signed: false,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "random_bitstring",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "n_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "ran",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tot_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: None,
// DEFAULT-NEXT:                   condition: None,
// DEFAULT-NEXT:                   increment: None,
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "ran",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "simple_rand",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "n_bits",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Rem,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: ShiftRight,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "ran",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: AddAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "tot_bits",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "n_bits",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "n_bits",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Return(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: Some(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: ShiftLeftAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "n_bits",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: BitAnd,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "ran",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: BitOrAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "x",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "n_bits",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Greater,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "tot_bits",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Integer(
// DEFAULT-NEXT:                                                           8,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: SizeOfType {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: LongLong,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       6,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Return(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
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
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "i",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   10000,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "x",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "y",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "random_bitstring",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "y",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "random_bitstring",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: SizeOfType {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: SizeOfType {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: LongLong,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Goto(
// DEFAULT-NEXT:                                   "save_time",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: LongLong,
// DEFAULT-NEXT:                                                   signed: false,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: LongLong,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: And,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: LongLong,
// DEFAULT-NEXT:                                                               signed: false,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "xx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Ternary {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: LongLong,
// DEFAULT-NEXT:                                                           signed: false,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: LongLong,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "r1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Labeled(
// DEFAULT-NEXT:                           "save_time",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: false,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: And,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Int,
// DEFAULT-NEXT:                                                               signed: false,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "xx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Or,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: GreaterEqual,
// DEFAULT-NEXT:                                                   left: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Cast {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Int,
// DEFAULT-NEXT:                                                               signed: false,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Ternary {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: GreaterEqual,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           then_value: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           else_value: Unary {
// DEFAULT-NEXT:                                                               op: Minus,
// DEFAULT-NEXT:                                                               value: Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Int,
// DEFAULT-NEXT:                                                               signed: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Mul,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "r1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "xx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: And,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Less,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "xx",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: Less,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Short,
// DEFAULT-NEXT:                                                   signed: false,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Short,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Ternary {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Short,
// DEFAULT-NEXT:                                                           signed: false,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Short,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "r1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Char {
// DEFAULT-NEXT:                                                   signed: Some(
// DEFAULT-NEXT:                                                       false,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Char {
// DEFAULT-NEXT:                                                   signed: Some(
// DEFAULT-NEXT:                                                       true,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Div,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Rem,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "yy",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Ternary {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Identifier(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: Some(
// DEFAULT-NEXT:                                                               false,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Identifier(
// DEFAULT-NEXT:                                                           "yy",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: Some(
// DEFAULT-NEXT:                                                               true,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "r1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "yy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "r2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "xx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
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
// DEFAULT-NEXT:               line: 37,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
