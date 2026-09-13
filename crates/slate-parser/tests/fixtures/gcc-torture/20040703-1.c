/* PR 16341 */
/* { dg-require-effective-target int32plus } */

void abort(void);
void exit(int);

#define PART_PRECISION (sizeof(cpp_num_part) * 8)

typedef unsigned int   cpp_num_part;
typedef struct cpp_num cpp_num;
struct cpp_num {
  cpp_num_part high;
  cpp_num_part low;
  int          unsignedp; /* True if value should be treated as unsigned.  */
  int          overflow;  /* True if the most recent calculation overflowed.  */
};

static int num_positive(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    return (num.high & (cpp_num_part)1 << (precision - 1)) == 0;
  }

  return (num.low & (cpp_num_part)1 << (precision - 1)) == 0;
}

static cpp_num num_trim(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    if (precision < PART_PRECISION)
      num.high &= ((cpp_num_part)1 << precision) - 1;
  } else {
    if (precision < PART_PRECISION)
      num.low &= ((cpp_num_part)1 << precision) - 1;
    num.high = 0;
  }

  return num;
}

/* Shift NUM, of width PRECISION, right by N bits.  */
static cpp_num num_rshift(cpp_num num, unsigned int precision, unsigned int n) {
  cpp_num_part sign_mask;
  int          x = num_positive(num, precision);

  if (num.unsignedp || x)
    sign_mask = 0;
  else
    sign_mask = ~(cpp_num_part)0;

  if (n >= precision)
    num.high = num.low = sign_mask;
  else {
    /* Sign-extend.  */
    if (precision < PART_PRECISION)
      num.high = sign_mask, num.low |= sign_mask << precision;
    else if (precision < 2 * PART_PRECISION)
      num.high |= sign_mask << (precision - PART_PRECISION);

    if (n >= PART_PRECISION) {
      n        -= PART_PRECISION;
      num.low   = num.high;
      num.high  = sign_mask;
    }

    if (n) {
      num.low  = (num.low >> n) | (num.high << (PART_PRECISION - n));
      num.high = (num.high >> n) | (sign_mask << (PART_PRECISION - n));
    }
  }

  num          = num_trim(num, precision);
  num.overflow = 0;
  return num;
}
#define num_zerop(num)     ((num.low | num.high) == 0)
#define num_eq(num1, num2) (num1.low == num2.low && num1.high == num2.high)

cpp_num num_lshift(cpp_num num, unsigned int precision, unsigned int n) {
  if (n >= precision) {
    num.overflow = !num.unsignedp && !num_zerop(num);
    num.high = num.low = 0;
  } else {
    cpp_num      orig;
    unsigned int m = n;

    orig = num;
    if (m >= PART_PRECISION) {
      m        -= PART_PRECISION;
      num.high  = num.low;
      num.low   = 0;
    }
    if (m) {
      num.high   = (num.high << m) | (num.low >> (PART_PRECISION - m));
      num.low  <<= m;
    }
    num = num_trim(num, precision);

    if (num.unsignedp)
      num.overflow = 0;
    else {
      cpp_num maybe_orig = num_rshift(num, precision, n);
      num.overflow       = !num_eq(orig, maybe_orig);
    }
  }

  return num;
}

unsigned int precision = 64;
unsigned int n         = 16;

cpp_num num = {0, 3, 0, 0};

int main() {
  cpp_num res = num_lshift(num, 64, n);

  if (res.low != 0x30000)
    abort();

  if (res.high != 0)
    abort();

  if (res.overflow != 0)
    abort();

  exit(0);
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR 16341 */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 14,
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
// DEFAULT-NEXT:       text: "/* { dg-require-effective-target int32plus } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 15,
// DEFAULT-NEXT:           length: 47,
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
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
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
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Typedef {
// DEFAULT-NEXT:       name: "cpp_num_part",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Typedef {
// DEFAULT-NEXT:       name: "cpp_num",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "cpp_num",
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
// DEFAULT-NEXT: decl[6]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "cpp_num",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "cpp_num_part",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "high",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 11,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "cpp_num_part",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "low",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 12,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* True if value should be treated as unsigned.  */",
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 303,
// DEFAULT-NEXT:                       length: 51,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: User,
// DEFAULT-NEXT:                       line: 13,
// DEFAULT-NEXT:                       header: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "unsignedp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 13,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* True if the most recent calculation overflowed.  */",
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 381,
// DEFAULT-NEXT:                       length: 54,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: User,
// DEFAULT-NEXT:                       line: 14,
// DEFAULT-NEXT:                       header: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "overflow",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 14,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT: decl[7]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "num_positive",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "cpp_num",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "precision",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "precision",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "cpp_num_part",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: SubAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "precision",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: SizeOfType {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "cpp_num_part",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: BitAnd,
// DEFAULT-NEXT:                                       left: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "high",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: ShiftLeft,
// DEFAULT-NEXT:                                           left: Cast {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "precision",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: BitAnd,
// DEFAULT-NEXT:                               left: Member {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "num",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "low",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: ShiftLeft,
// DEFAULT-NEXT:                                   left: Cast {
// DEFAULT-NEXT:                                       ty: Named(
// DEFAULT-NEXT:                                           "cpp_num_part",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "precision",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
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
// DEFAULT-NEXT:               line: 17,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Named(
// DEFAULT-NEXT:               "cpp_num",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "num_trim",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "cpp_num",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "precision",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "precision",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "cpp_num_part",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: SubAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "precision",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: SizeOfType {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "cpp_num_part",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Less,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "precision",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: SizeOfType {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "cpp_num_part",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: BitAndAssign,
// DEFAULT-NEXT:                                           target: Member {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "num",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "high",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "cpp_num_part",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "precision",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "precision",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: SizeOfType {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: BitAndAssign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Cast {
// DEFAULT-NEXT:                                                           ty: Named(
// DEFAULT-NEXT:                                                               "cpp_num_part",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "precision",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "high",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 26,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Comment {
// DEFAULT-NEXT:       text: "/* Shift NUM, of width PRECISION, right by N bits.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 1069,
// DEFAULT-NEXT:           length: 54,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 40,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Named(
// DEFAULT-NEXT:               "cpp_num",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "num_rshift",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "cpp_num",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "precision",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "cpp_num_part",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "sign_mask",
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
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "num_positive",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "precision",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "num",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "unsignedp",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "sign_mask",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "sign_mask",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Unary {
// DEFAULT-NEXT:                                           op: BitNot,
// DEFAULT-NEXT:                                           value: Cast {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: GreaterEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "precision",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "high",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "low",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "sign_mask",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "precision",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: SizeOfType {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Comma(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "num",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "high",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "sign_mask",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: BitOrAssign,
// DEFAULT-NEXT:                                                   target: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "num",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "low",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "sign_mask",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "precision",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: Some(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       If {
// DEFAULT-NEXT:                                           condition: Const(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "precision",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: SizeOfType {
// DEFAULT-NEXT:                                                               ty: Named(
// DEFAULT-NEXT:                                                                   "cpp_num_part",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               8,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           then_branch: [
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: BitOrAssign,
// DEFAULT-NEXT:                                                           target: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "num",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "high",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "sign_mask",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Binary {
// DEFAULT-NEXT:                                                                   op: Sub,
// DEFAULT-NEXT:                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                       "precision",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: SizeOfType {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "cpp_num_part",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           8,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                           else_branch: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: GreaterEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: SizeOfType {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: SubAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: SizeOfType {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "cpp_num_part",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       8,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "high",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "high",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "sign_mask",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "n",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: ShiftRight,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "num",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "low",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "n",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "num",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "high",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Mul,
// DEFAULT-NEXT:                                                               left: SizeOfType {
// DEFAULT-NEXT:                                                                   ty: Named(
// DEFAULT-NEXT:                                                                       "cpp_num_part",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   8,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "n",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "high",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: ShiftRight,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "num",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "high",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "n",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "sign_mask",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Mul,
// DEFAULT-NEXT:                                                               left: SizeOfType {
// DEFAULT-NEXT:                                                                   ty: Named(
// DEFAULT-NEXT:                                                                       "cpp_num_part",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   8,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "n",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "num",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "num_trim",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "num",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "precision",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "num",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "overflow",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Unreachable(
// DEFAULT-NEXT:                   Comment {
// DEFAULT-NEXT:                       text: "/* Sign-extend.  */",
// DEFAULT-NEXT:                       loc: Loc {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           offset: 1439,
// DEFAULT-NEXT:                           length: 19,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 53,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 41,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Named(
// DEFAULT-NEXT:               "cpp_num",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "num_lshift",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "cpp_num",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "precision",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: GreaterEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "precision",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "overflow",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Member {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "num",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "unsignedp",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Not,
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "num",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "low",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "num",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "high",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "high",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "low",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Decl(
// DEFAULT-NEXT:                               Declaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Named(
// DEFAULT-NEXT:                                           "cpp_num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Name(
// DEFAULT-NEXT:                                       "orig",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Decl(
// DEFAULT-NEXT:                               Declaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: false,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Name(
// DEFAULT-NEXT:                                       "m",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   initializer: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "orig",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: GreaterEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: SizeOfType {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "cpp_num_part",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: SubAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: SizeOfType {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "cpp_num_part",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       8,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "high",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "m",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "high",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "num",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "high",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "m",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: ShiftRight,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "num",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "low",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Mul,
// DEFAULT-NEXT:                                                               left: SizeOfType {
// DEFAULT-NEXT:                                                                   ty: Named(
// DEFAULT-NEXT:                                                                       "cpp_num_part",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   8,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "m",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: ShiftLeftAssign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "low",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "num_trim",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "num",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "precision",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "num",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "unsignedp",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "overflow",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: Some(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       Decl(
// DEFAULT-NEXT:                                           Declaration {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Named(
// DEFAULT-NEXT:                                                       "cpp_num",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "maybe_orig",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "num_rshift",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [
// DEFAULT-NEXT:                                                                   Identifier(
// DEFAULT-NEXT:                                                                       "num",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Identifier(
// DEFAULT-NEXT:                                                                       "precision",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Identifier(
// DEFAULT-NEXT:                                                                       "n",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "num",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "overflow",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Unary {
// DEFAULT-NEXT:                                                       op: Not,
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: And,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "orig",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "low",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "maybe_orig",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "low",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "orig",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "high",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "maybe_orig",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "high",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "num",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 78,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "precision",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           64,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 109,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "n",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           16,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 110,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "cpp_num",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "num",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               List(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 112,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Function(
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
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "cpp_num",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "res",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "num_lshift",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "num",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "res",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "low",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               196608,
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "res",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "high",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "res",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "overflow",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:               line: 114,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
