void abort(void);
void exit(int);

long simple_rand() {
  static unsigned long seed = 47114711;
  unsigned long this        = seed * 1103515245 + 12345;
  seed                      = this;
  return this >> 8;
}

unsigned long int random_bitstring() {
  unsigned long int x;
  int               n_bits;
  long              ran;
  int               tot_bits = 0;

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

      if (tot_bits > 8 * sizeof(long) + 6)
        return x;
    }
  }
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

int main(void) {
  long int i;

  for (i = 0; i < 1000; i++) {
    unsigned long x, y;
    x = random_bitstring();
    y = random_bitstring();

    if (sizeof(int) == sizeof(long))
      goto save_time;

    {
      unsigned long xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed long xx = x, yy = y, r1, r2;
      if ((unsigned long)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned long)ABS(yy) ||
          (signed long)(r1 * yy + r2) != xx)
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
      if (ABS(r2) >= (unsigned int)ABS(yy) || (signed int)(r1 * yy + r2) != xx)
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
// DEFAULT-NEXT:                   rank: Long,
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
// DEFAULT-NEXT:                                   rank: Long,
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
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 47114711,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "47114711",
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
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
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "seed",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1103515245,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1103515245",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 12345,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "12345",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "seed",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Identifier(
// DEFAULT-NEXT:                           "this",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Binary {
// DEFAULT-NEXT:                       op: ShiftRight,
// DEFAULT-NEXT:                       left: Identifier(
// DEFAULT-NEXT:                           "this",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 8,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "8",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                   rank: Long,
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
// DEFAULT-NEXT:                                   rank: Long,
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
// DEFAULT-NEXT:                                   rank: Long,
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
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 0,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "0",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: None,
// DEFAULT-NEXT:                   condition: None,
// DEFAULT-NEXT:                   increment: None,
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "ran",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "simple_rand",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "n_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Binary {
// DEFAULT-NEXT:                                   op: Rem,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: ShiftRight,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "ran",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 16,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "16",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: AddAssign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "tot_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "n_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "n_bits",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Return(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: Some(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: ShiftLeftAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "n_bits",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
// DEFAULT-NEXT:                                           op: BitAnd,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "ran",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: BitOrAssign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Binary {
// DEFAULT-NEXT:                                                       op: Sub,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "n_bits",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 1,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "1",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
// DEFAULT-NEXT:                                           op: Greater,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "tot_bits",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 8,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "8",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: SizeOfType {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Long,
// DEFAULT-NEXT:                                                               signed: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 6,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "6",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Return(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "x",
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
// DEFAULT-NEXT:                                   rank: Long,
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
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1000,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1000",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Long,
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
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "random_bitstring",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "y",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "random_bitstring",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: SizeOfType {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Long,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:                                                   rank: Long,
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "yy",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "r1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "yy",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                                   rank: Long,
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Long,
// DEFAULT-NEXT:                                                           signed: false,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "xx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 0,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "0",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               operand: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Conditional {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       operand: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Long,
// DEFAULT-NEXT:                                                       signed: false,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           operand: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Long,
// DEFAULT-NEXT:                                                       signed: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Binary {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Labeled {
// DEFAULT-NEXT:                           label: "save_time",
// DEFAULT-NEXT:                           body: Block(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Int,
// DEFAULT-NEXT:                                                       signed: false,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "xx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "yy",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "r1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "r2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 0,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "0",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Continue,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Expr(
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
// DEFAULT-NEXT:                                   Expr(
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
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
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
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "abort",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Int,
// DEFAULT-NEXT:                                                           signed: false,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "xx",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 0,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "0",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               operand: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Conditional {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       operand: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Int,
// DEFAULT-NEXT:                                                       signed: false,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           operand: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Int,
// DEFAULT-NEXT:                                                       signed: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Binary {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "yy",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "r1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "yy",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Conditional {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       operand: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Short,
// DEFAULT-NEXT:                                                       signed: false,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           operand: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Short,
// DEFAULT-NEXT:                                                       signed: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Binary {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "yy",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Continue,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "r2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "r1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "yy",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "r2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "x",
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
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "y",
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
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "yy",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Conditional {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       operand: Paren(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "r2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Char {
// DEFAULT-NEXT:                                                       signed: Some(
// DEFAULT-NEXT:                                                           false,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           operand: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "yy",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Char {
// DEFAULT-NEXT:                                                       signed: Some(
// DEFAULT-NEXT:                                                           true,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Paren(
// DEFAULT-NEXT:                                                   Binary {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "xx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "abort",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
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
