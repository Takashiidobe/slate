/* { dg-do run } */
/* Check that find_scan_insn properly handles swapped FP comparisons.  */
static double x;
static int    exit_code;

void __attribute__((noinline)) check_int(int a, int b) {
  exit_code += (a != b);
}

int
main(void) {
  x = 0.0;
  asm("" : "+m"(x));
  check_int(__builtin_isgreater(x, 1.0), 0);
  check_int(__builtin_isgreaterequal(x, 1.0), 0);
  check_int(__builtin_isless(x, 1.0), 1);
  check_int(__builtin_islessequal(x, 1.0), 1);
  check_int(__builtin_islessgreater(x, 1.0), 1);
  return exit_code;
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-do run } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 19,
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
// DEFAULT-NEXT:       text: "/* Check that find_scan_insn properly handles swapped FP comparisons.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 20,
// DEFAULT-NEXT:           length: 73,
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
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "x",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "exit_code",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "check_int",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
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
// DEFAULT-NEXT:                           signed: true,
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "exit_code",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Float(
// DEFAULT-NEXT:                               FloatLiteral {
// DEFAULT-NEXT:                                   value: Double(
// DEFAULT-NEXT:                                       0.0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   "asm ( \"\" : \"+m\" ( x ) ) ;",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_isgreater",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
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
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_isgreaterequal",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
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
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_isless",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
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
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_islessequal",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
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
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_islessgreater",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "x",
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
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "exit_code",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 9,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
