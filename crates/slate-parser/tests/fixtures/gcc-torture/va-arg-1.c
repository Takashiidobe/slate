#include <stdarg.h>

void abort(void);
void exit(int);

typedef unsigned long L;
void f(L p0, L p1, L p2, L p3, L p4, L p5, L p6, L p7, L p8, ...) {
  va_list select;

  va_start(select, p8);

  if (va_arg(select, L) != 10)
    abort();
  if (va_arg(select, L) != 11)
    abort();
  if (va_arg(select, L) != 0)
    abort();

  va_end(select);
}

int main(void) {
  f(1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 11L, 0L);
  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
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
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "L",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Long,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "f",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p0",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p4",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p5",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p6",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p7",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "L",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "p8",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           variadic: true,
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "va_list",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "select",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "__builtin_c23_va_start",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "select",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "p8",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: VaArg {
// DEFAULT-NEXT:                               ap: Identifier(
// DEFAULT-NEXT:                                   "select",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "L",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               10,
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
// DEFAULT-NEXT:                           left: VaArg {
// DEFAULT-NEXT:                               ap: Identifier(
// DEFAULT-NEXT:                                   "select",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "L",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               11,
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
// DEFAULT-NEXT:                           left: VaArg {
// DEFAULT-NEXT:                               ap: Identifier(
// DEFAULT-NEXT:                                   "select",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "L",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
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
// DEFAULT-NEXT:                               "__builtin_va_end",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "select",
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
// DEFAULT-NEXT:               line: 6,
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "f",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   6,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   10,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:               line: 21,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
