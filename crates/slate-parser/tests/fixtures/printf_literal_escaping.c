#include <stdio.h>

int main(void) {
  int      d = 5;
  unsigned h = 0xAB;
  printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
  printf("}}%%{{%d}}\n", d);
  printf("%%%%%d%%%%\n", d);
  printf("{{}}%s{{}}\n", "mid");
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
// DEFAULT-NEXT:           name: "main",
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
// DEFAULT-NEXT:                           "d",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       5,
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
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "h",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       171,
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
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "{%d} %% \\\"quoted\\\" back\\\\slash %s|%c|%x\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "hi",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   88,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "h",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "{{[}][}]}}%%{{\{\{}}%d{{[}][}]}}\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%%%%%d%%%%\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "{{\{\{}}{{[}][}]}}%s{{\{\{}}{{[}][}]}}\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "mid",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
