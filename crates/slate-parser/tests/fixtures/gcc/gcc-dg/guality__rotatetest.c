/* { dg-do run { target { { i?86-*-* x86_64-*-* } && lp64 } } } */
/* { dg-options "-g" } */

volatile int vv;

__attribute__((noclone, noinline)) long f1(unsigned long x) {
  long f = (x << 12) | (x >> (64 - 12));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) long f2(unsigned long x, int y) {
  long f = (x << y) | (x >> (64 - y));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) long f3(unsigned long x, int y) {
  long f = (x >> y) | (x << (64 - y));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f4(unsigned int x) {
  unsigned int f = (x << 12) | (x >> (32 - 12));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f5(unsigned int x, int y) {
  unsigned int f = (x << y) | (x >> (32 - y));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f6(unsigned int x, int y) {
  unsigned int f = (x >> y) | (x << (32 - y));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

int
main() {
  f1(0x123456789abcde0fUL);
  f2(0x123456789abcde0fUL, 18);
  f3(0x123456789abcde0fUL, 17);
  f4(0x12345678);
  f5(0x12345678, 18);
  f6(0x12345678, 17);
  return 0;
}



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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "vv",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 12,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "12",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 64,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "64",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 12,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "12",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 5,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f2",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "y",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 64,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "64",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f3",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "y",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 64,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "64",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 21,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f4",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 12,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "12",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 32,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "32",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 12,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "12",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 29,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f5",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "y",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 32,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "32",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
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
// DEFAULT-NEXT: decl[6]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "f6",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "y",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 32,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "32",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "y",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
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
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Postfix {
// DEFAULT-NEXT:                       op: Increment,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "vv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "f",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 45,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Function(
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
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1311768467463790095,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: true,
// DEFAULT-NEXT:                                       size: Long,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x123456789abcde0fUL",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1311768467463790095,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: true,
// DEFAULT-NEXT:                                       size: Long,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x123456789abcde0fUL",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 18,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "18",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1311768467463790095,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: true,
// DEFAULT-NEXT:                                       size: Long,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x123456789abcde0fUL",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 17,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "17",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f4",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 305419896,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x12345678",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f5",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 305419896,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x12345678",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 18,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "18",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "f6",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 305419896,
// DEFAULT-NEXT:                                   radix: Hex,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0x12345678",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 17,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "17",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:               line: 53,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
