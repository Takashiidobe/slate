#include <stdio.h>

struct event {
  int type;
  union {
    struct {
      char *value;
    } alias;
    struct {
      char *handle;
      char *suffix;
    } tag;
  } data;
};

int main(void) {
  struct event e;
  e.type            = 1;
  char h[]          = "H";
  char s[]          = "S";
  e.data.tag.handle = h;
  e.data.tag.suffix = s;
  printf("%d %s%s\n", e.type, e.data.tag.handle, e.data.tag.suffix);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

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
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "event",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:                               "type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 3,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Union,
// DEFAULT-NEXT:                                   name: None,
// DEFAULT-NEXT:                                   body: Some(
// DEFAULT-NEXT:                                       Fields(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Struct,
// DEFAULT-NEXT:                                                               name: None,
// DEFAULT-NEXT:                                                               body: Some(
// DEFAULT-NEXT:                                                                   Fields(
// DEFAULT-NEXT:                                                                       [
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "value",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "alias",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Struct,
// DEFAULT-NEXT:                                                               name: None,
// DEFAULT-NEXT:                                                               body: Some(
// DEFAULT-NEXT:                                                                   Fields(
// DEFAULT-NEXT:                                                                       [
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "handle",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "suffix",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "tag",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "data",
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
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
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
// DEFAULT-NEXT:                                   "event",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "e",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "e",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "type",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               1,
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
// DEFAULT-NEXT:                               "h",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "H",
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "S",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                               base: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "e",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "data",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "tag",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "handle",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "h",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "e",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "data",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "tag",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "suffix",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "s",
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
// DEFAULT-NEXT:                                   "%d %s%s\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "e",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "type",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "e",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "data",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "tag",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "handle",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "e",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "data",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "tag",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "suffix",
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
// DEFAULT-NEXT:               line: 15,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
