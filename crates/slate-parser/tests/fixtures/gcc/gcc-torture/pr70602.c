/* PR tree-optimization/70602 */
/* { dg-require-effective-target int32plus } */

struct __attribute__((packed)) S {
  int s : 1;
  int t : 20;
};

int a, b, c;

int main() {
  for (; a < 1; a++) {
    struct S e[] = {{0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9},
                    {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9},
                    {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}};
    b            = b || e[0].s;
    c            = e[0].t;
  }
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* PR tree-optimization/70602 */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 0,
// DEFAULT-NEXT:                       length: 32,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-require-effective-target int32plus } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 33,
// DEFAULT-NEXT:                       length: 47,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "S",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "s",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       20,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Packed,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "a",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "b",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "c",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: None,
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Tagged {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: Some(
// DEFAULT-NEXT:                                           "S",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Array {
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "e",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           size: Unspecified,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           List(
// DEFAULT-NEXT:                                               [
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Integer(
// DEFAULT-NEXT:                                                                           9,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "b",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Member {
// DEFAULT-NEXT:                                       base: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "e",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "s",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Member {
// DEFAULT-NEXT:                                   base: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "e",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "t",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       0,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
