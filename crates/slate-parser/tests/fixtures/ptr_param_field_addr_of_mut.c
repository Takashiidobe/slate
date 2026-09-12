// { dg-additional-options "-O2" }
#include <stdio.h>

typedef struct {
  int x;
} FIO_Dict_t;

typedef struct {
  int y;
} FIO_SyncCompressIO;

typedef struct {
  FIO_Dict_t         dict;
  int                cctx;
  FIO_SyncCompressIO io;
} cRess_t;

static void freeDict(FIO_Dict_t *dict) { dict->x = 0; }

static void syncDestroy(FIO_SyncCompressIO *io) { io->y = 0; }

static void freeCResources(cRess_t *const ress) {
  freeDict(&(ress->dict));
  syncDestroy(&ress->io);
}

int main(void) {
  cRess_t r = {.dict = {.x = 5}, .cctx = 9, .io = {.y = 7}};
  freeCResources(&r);
  printf("%d %d %d\n", r.dict.x, r.io.y, r.cctx);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "// { dg-additional-options \"-O2\" }",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 34,
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
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           fields: [
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
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 4,
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
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "FIO_Dict_t",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: None,
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
// DEFAULT-NEXT: decl[3]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           fields: [
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
// DEFAULT-NEXT:                               "y",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 8,
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
// DEFAULT-NEXT:               line: 7,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Typedef {
// DEFAULT-NEXT:       name: "FIO_SyncCompressIO",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "FIO_Dict_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "dict",
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
// DEFAULT-NEXT:                               "cctx",
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
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "FIO_SyncCompressIO",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "io",
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
// DEFAULT-NEXT:               line: 11,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Typedef {
// DEFAULT-NEXT:       name: "cRess_t",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 11,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "freeDict",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "FIO_Dict_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "dict",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "dict",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "x",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
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
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "syncDestroy",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "FIO_SyncCompressIO",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "io",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "io",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "y",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
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
// DEFAULT-NEXT:               line: 19,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "freeCResources",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "cRess_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "ress",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "freeDict",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "ress",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "dict",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "syncDestroy",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "ress",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "io",
// DEFAULT-NEXT:                                   },
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
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[10]: Function(
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
// DEFAULT-NEXT:                               "cRess_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "r",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [
// DEFAULT-NEXT:                                           Field(
// DEFAULT-NEXT:                                               "dict",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       value: List(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [
// DEFAULT-NEXT:                                                       Field(
// DEFAULT-NEXT:                                                           "x",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [
// DEFAULT-NEXT:                                           Field(
// DEFAULT-NEXT:                                               "cctx",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [
// DEFAULT-NEXT:                                           Field(
// DEFAULT-NEXT:                                               "io",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       value: List(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [
// DEFAULT-NEXT:                                                       Field(
// DEFAULT-NEXT:                                                           "y",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               7,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "freeCResources",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "r",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "%d %d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "r",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "dict",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "x",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "r",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "io",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "y",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "r",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "cctx",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               line: 26,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
