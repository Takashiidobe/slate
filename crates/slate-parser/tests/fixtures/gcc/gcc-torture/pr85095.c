/* PR target/85095 */

__attribute__((noipa)) unsigned long f1(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f2(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

__attribute__((noipa)) unsigned long f3(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f4(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

int main() {
  if (f1(16UL, -18UL) != -2UL || f1(16UL, -17UL) != -1UL ||
      f1(16UL, -16UL) != 1UL || f1(16UL, -15UL) != 2UL ||
      f2(24UL, -26UL) != -2UL || f2(24UL, -25UL) != -1UL ||
      f2(24UL, -24UL) != -1UL || f2(24UL, -23UL) != 0UL ||
      f3(32U, -34U) != -2U || f3(32U, -33U) != -1U || f3(32U, -32U) != 1U ||
      f3(32U, -31U) != 2U || f4(35U, -37U) != -2U || f4(35U, -36U) != -1U ||
      f4(35U, -35U) != -1U || f4(35U, -34U) != 0U)
    __builtin_abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR target/85095 */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 21,
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
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Long,
// DEFAULT-NEXT:                   signed: false,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "f1",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "b",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_add_overflow",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "i",
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
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoIpa,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Long,
// DEFAULT-NEXT:                   signed: false,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "f2",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "b",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_add_overflow",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "i",
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
// DEFAULT-NEXT:               line: 7,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoIpa,
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:           name: "f3",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "a",
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
// DEFAULT-NEXT:                           "b",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_add_overflow",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "i",
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
// DEFAULT-NEXT:               line: 12,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoIpa,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Long,
// DEFAULT-NEXT:                   signed: false,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "f4",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "a",
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
// DEFAULT-NEXT:                           "b",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_add_overflow",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "b",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "i",
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
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoIpa,
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                                               op: Or,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Or,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Or,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Or,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Or,
// DEFAULT-NEXT:                                                               left: Binary {
// DEFAULT-NEXT:                                                                   op: Or,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Or,
// DEFAULT-NEXT:                                                                       left: Binary {
// DEFAULT-NEXT:                                                                           op: Or,
// DEFAULT-NEXT:                                                                           left: Binary {
// DEFAULT-NEXT:                                                                               op: Or,
// DEFAULT-NEXT:                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                   op: Or,
// DEFAULT-NEXT:                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                       op: NotEqual,
// DEFAULT-NEXT:                                                                                       left: Call {
// DEFAULT-NEXT:                                                                                           callee: Identifier(
// DEFAULT-NEXT:                                                                                               "f1",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           arguments: [
// DEFAULT-NEXT:                                                                                               Integer(
// DEFAULT-NEXT:                                                                                                   16,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               Unary {
// DEFAULT-NEXT:                                                                                                   op: Minus,
// DEFAULT-NEXT:                                                                                                   value: Integer(
// DEFAULT-NEXT:                                                                                                       18,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ],
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Unary {
// DEFAULT-NEXT:                                                                                           op: Minus,
// DEFAULT-NEXT:                                                                                           value: Integer(
// DEFAULT-NEXT:                                                                                               2,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Binary {
// DEFAULT-NEXT:                                                                                       op: NotEqual,
// DEFAULT-NEXT:                                                                                       left: Call {
// DEFAULT-NEXT:                                                                                           callee: Identifier(
// DEFAULT-NEXT:                                                                                               "f1",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           arguments: [
// DEFAULT-NEXT:                                                                                               Integer(
// DEFAULT-NEXT:                                                                                                   16,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               Unary {
// DEFAULT-NEXT:                                                                                                   op: Minus,
// DEFAULT-NEXT:                                                                                                   value: Integer(
// DEFAULT-NEXT:                                                                                                       17,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ],
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Unary {
// DEFAULT-NEXT:                                                                                           op: Minus,
// DEFAULT-NEXT:                                                                                           value: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: NotEqual,
// DEFAULT-NEXT:                                                                                   left: Call {
// DEFAULT-NEXT:                                                                                       callee: Identifier(
// DEFAULT-NEXT:                                                                                           "f1",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       arguments: [
// DEFAULT-NEXT:                                                                                           Integer(
// DEFAULT-NEXT:                                                                                               16,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           Unary {
// DEFAULT-NEXT:                                                                                               op: Minus,
// DEFAULT-NEXT:                                                                                               value: Integer(
// DEFAULT-NEXT:                                                                                                   16,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ],
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: Binary {
// DEFAULT-NEXT:                                                                               op: NotEqual,
// DEFAULT-NEXT:                                                                               left: Call {
// DEFAULT-NEXT:                                                                                   callee: Identifier(
// DEFAULT-NEXT:                                                                                       "f1",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   arguments: [
// DEFAULT-NEXT:                                                                                       Integer(
// DEFAULT-NEXT:                                                                                           16,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       Unary {
// DEFAULT-NEXT:                                                                                           op: Minus,
// DEFAULT-NEXT:                                                                                           value: Integer(
// DEFAULT-NEXT:                                                                                               15,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ],
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   2,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Binary {
// DEFAULT-NEXT:                                                                           op: NotEqual,
// DEFAULT-NEXT:                                                                           left: Call {
// DEFAULT-NEXT:                                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                                   "f2",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               arguments: [
// DEFAULT-NEXT:                                                                                   Integer(
// DEFAULT-NEXT:                                                                                       24,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   Unary {
// DEFAULT-NEXT:                                                                                       op: Minus,
// DEFAULT-NEXT:                                                                                       value: Integer(
// DEFAULT-NEXT:                                                                                           26,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ],
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: Unary {
// DEFAULT-NEXT:                                                                               op: Minus,
// DEFAULT-NEXT:                                                                               value: Integer(
// DEFAULT-NEXT:                                                                                   2,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: NotEqual,
// DEFAULT-NEXT:                                                                       left: Call {
// DEFAULT-NEXT:                                                                           callee: Identifier(
// DEFAULT-NEXT:                                                                               "f2",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           arguments: [
// DEFAULT-NEXT:                                                                               Integer(
// DEFAULT-NEXT:                                                                                   24,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               Unary {
// DEFAULT-NEXT:                                                                                   op: Minus,
// DEFAULT-NEXT:                                                                                   value: Integer(
// DEFAULT-NEXT:                                                                                       25,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ],
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Unary {
// DEFAULT-NEXT:                                                                           op: Minus,
// DEFAULT-NEXT:                                                                           value: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Binary {
// DEFAULT-NEXT:                                                                   op: NotEqual,
// DEFAULT-NEXT:                                                                   left: Call {
// DEFAULT-NEXT:                                                                       callee: Identifier(
// DEFAULT-NEXT:                                                                           "f2",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       arguments: [
// DEFAULT-NEXT:                                                                           Integer(
// DEFAULT-NEXT:                                                                               24,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           Unary {
// DEFAULT-NEXT:                                                                               op: Minus,
// DEFAULT-NEXT:                                                                               value: Integer(
// DEFAULT-NEXT:                                                                                   24,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Unary {
// DEFAULT-NEXT:                                                                       op: Minus,
// DEFAULT-NEXT:                                                                       value: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: NotEqual,
// DEFAULT-NEXT:                                                               left: Call {
// DEFAULT-NEXT:                                                                   callee: Identifier(
// DEFAULT-NEXT:                                                                       "f2",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   arguments: [
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           24,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Unary {
// DEFAULT-NEXT:                                                                           op: Minus,
// DEFAULT-NEXT:                                                                           value: Integer(
// DEFAULT-NEXT:                                                                               23,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: NotEqual,
// DEFAULT-NEXT:                                                           left: Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "f3",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [
// DEFAULT-NEXT:                                                                   Integer(
// DEFAULT-NEXT:                                                                       32,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Unary {
// DEFAULT-NEXT:                                                                       op: Minus,
// DEFAULT-NEXT:                                                                       value: Integer(
// DEFAULT-NEXT:                                                                           34,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Unary {
// DEFAULT-NEXT:                                                               op: Minus,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   2,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: NotEqual,
// DEFAULT-NEXT:                                                       left: Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "f3",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Integer(
// DEFAULT-NEXT:                                                                   32,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Unary {
// DEFAULT-NEXT:                                                                   op: Minus,
// DEFAULT-NEXT:                                                                   value: Integer(
// DEFAULT-NEXT:                                                                       33,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "f3",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Minus,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   32,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "f3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Integer(
// DEFAULT-NEXT:                                                           32,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               31,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "f4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       35,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Unary {
// DEFAULT-NEXT:                                                       op: Minus,
// DEFAULT-NEXT:                                                       value: Integer(
// DEFAULT-NEXT:                                                           37,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: NotEqual,
// DEFAULT-NEXT:                                       left: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "f4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   35,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       36,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Unary {
// DEFAULT-NEXT:                                           op: Minus,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "f4",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               35,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   35,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "f4",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           35,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Unary {
// DEFAULT-NEXT:                                           op: Minus,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               34,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:               line: 22,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
