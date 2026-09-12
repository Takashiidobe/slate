#include <stdio.h>
#include <string.h>

int main(void) {
  char dst[16];
  char src[8] = "hello";
  memcpy(dst, src, 6);
  memset(dst + 5, 'A', 3);
  dst[8] = 0;
  char moved[16];
  memmove(moved, dst, 9);
  printf("%s\n", moved);
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "dst",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntLit(
// DEFAULT-NEXT:                                   16,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "src",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntLit(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "hello",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memcpy",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "dst",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "src",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   6,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memset",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "dst",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   65,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "dst",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "moved",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntLit(
// DEFAULT-NEXT:                                   16,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memmove",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "moved",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "dst",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   9,
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
// DEFAULT-NEXT:                                   "%s\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "moved",
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
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Comment {
// DEFAULT-NEXT:       text: "// LOWERING: #![feature(c_variadic)]",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 246,
// DEFAULT-NEXT:           length: 36,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 15,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Comment {
// DEFAULT-NEXT:       text: "// REWRITES: #![feature(c_variadic)]",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 284,
// DEFAULT-NEXT:           length: 36,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
