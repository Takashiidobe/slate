#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 3);
  p[0]   = 1;
  p[1]   = 2;
  p[2]   = 3;
  printf("%d\n", p[0] + p[1] + p[2]);
  free(p);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Typedef {
// DEFAULT-NEXT:       name: "__size_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Long,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 258,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "size_t",
// DEFAULT-NEXT:       ty: Named(
// DEFAULT-NEXT:           "__size_t",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 795,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:                   "printf",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Qualified {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_restrict: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:               variadic: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "malloc",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "size_t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "size",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               9,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 76,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "free",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Void,
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "ptr",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               9,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 94,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
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
// DEFAULT-NEXT:                       declarator: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "p",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "malloc",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: SizeOfType {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Int,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                   "%d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "p",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "p",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "p",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "free",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "p",
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
// SLATE-FILECHECK-END DEFAULT
