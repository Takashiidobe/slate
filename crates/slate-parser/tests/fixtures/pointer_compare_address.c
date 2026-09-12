

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t    timestamp    = 0;
  struct tm utc          = {};
  struct tm local        = {};
  int       utc_result   = gmtime_r(&timestamp, &utc) == &utc;
  int       local_result = localtime_r(&timestamp, &local) == &local;
  printf("%d %d\n", utc_result, local_result);
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
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "time_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "timestamp",
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
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "tm",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "utc",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "tm",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "local",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [],
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
// DEFAULT-NEXT:                           "utc_result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "gmtime_r",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               AddrOf(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "timestamp",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               AddrOf(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "utc",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: AddrOf(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "utc",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
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
// DEFAULT-NEXT:                           "local_result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "localtime_r",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               AddrOf(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "timestamp",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               AddrOf(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "local",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: AddrOf(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "local",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
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
// DEFAULT-NEXT:                                   "%d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "utc_result",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "local_result",
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
// DEFAULT-NEXT:               line: 6,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
