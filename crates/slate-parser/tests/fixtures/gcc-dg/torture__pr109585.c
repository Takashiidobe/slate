/* { dg-do run } */

#include <stdlib.h>

struct P {
  long      v;
  struct P *n;
};

struct F {
  long     x;
  struct P fam[];
};

int __attribute__((noipa)) f(struct F *f, int i) {
  struct P *p = f->fam;
  asm("" : "+r"(f) : "r"(p));
  p->v = 0;
  p->n = 0;
  return f->fam->n != 0;
}

int
main() {
  struct F *m = malloc(sizeof(long) + 2 * sizeof(struct P));
  m->fam[0].n = &m->fam[1];
  if (f(m, 0))
    abort();
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

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
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "P",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "v",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 5,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "P",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 6,
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
// DEFAULT-NEXT:               line: 4,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "F",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 10,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "P",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Array {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "fam",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               size: Unspecified,
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "f",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "F",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "f",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "P",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:                                   Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "f",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "fam",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   "asm ( \"\" : \"+r\" ( f ) : \"r\" ( p ) ) ;",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "v",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "n",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Arrow {
// DEFAULT-NEXT:                               base: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "fam",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "n",
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
// DEFAULT-NEXT:               line: 14,
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
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "F",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "m",
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
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: SizeOfType {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Long,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: SizeOfType {
// DEFAULT-NEXT:                                                       ty: Tagged {
// DEFAULT-NEXT:                                                           kind: Struct,
// DEFAULT-NEXT:                                                           name: Some(
// DEFAULT-NEXT:                                                               "P",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
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
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Index {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "fam",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "n",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: AddrOf(
// DEFAULT-NEXT:                               Index {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "fam",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "f",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "m",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
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
