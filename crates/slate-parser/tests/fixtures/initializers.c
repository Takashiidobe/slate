int scalar = 7;
char message[6] = "hello";
int matrix[2][2] = {{1, 2}, {3, 4}};
struct Point {
  int x;
  int y;
};
struct Point point = {.y = 9, .x = 4};
int values[4] = {[2] = 8, [0] = 1};
int selected =
#ifdef ENABLED
  11;
#else
  22;
#endif

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES ENABLED ENABLED

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "Point",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
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
// DEFAULT-NEXT:                                   "x",
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
// DEFAULT-NEXT:                                   "y",
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
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration {
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
// DEFAULT-NEXT:                       "scalar",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               7,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "message",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               6,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           StringLit(
// DEFAULT-NEXT:                               "hello",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "matrix",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       3,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Reference {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: "Point",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "point",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Field(
// DEFAULT-NEXT:                                           "y",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Field(
// DEFAULT-NEXT:                                           "x",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT: decl[5]: Declaration {
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
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "values",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Array(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Array(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
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
// DEFAULT-NEXT: decl[6]: Declaration {
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
// DEFAULT-NEXT:                       "selected",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               22,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
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
// DEFAULT-NEXT:               line: 16,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ENABLED
// ENABLED: tag[0]: TagDefinition {
// ENABLED-NEXT:       id: TagId(
// ENABLED-NEXT:           0,
// ENABLED-NEXT:       ),
// ENABLED-NEXT:       kind: Struct,
// ENABLED-NEXT:       name: Some(
// ENABLED-NEXT:           "Point",
// ENABLED-NEXT:       ),
// ENABLED-NEXT:       body: Record(
// ENABLED-NEXT:           [
// ENABLED-NEXT:               Field(
// ENABLED-NEXT:                   FieldDecl {
// ENABLED-NEXT:                       specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:                           ty: Integer(
// ENABLED-NEXT:                               Ranked {
// ENABLED-NEXT:                                   rank: Int,
// ENABLED-NEXT:                                   signed: true,
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                       declarators: [
// ENABLED-NEXT:                           FieldDeclarator {
// ENABLED-NEXT:                               declarator: Name(
// ENABLED-NEXT:                                   "x",
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           },
// ENABLED-NEXT:                       ],
// ENABLED-NEXT:                       provenance: Provenance {
// ENABLED-NEXT:                           file: FileId(
// ENABLED-NEXT:                               3,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                           kind: User,
// ENABLED-NEXT:                           line: 4,
// ENABLED-NEXT:                           header: None,
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:               Field(
// ENABLED-NEXT:                   FieldDecl {
// ENABLED-NEXT:                       specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:                           ty: Integer(
// ENABLED-NEXT:                               Ranked {
// ENABLED-NEXT:                                   rank: Int,
// ENABLED-NEXT:                                   signed: true,
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                       declarators: [
// ENABLED-NEXT:                           FieldDeclarator {
// ENABLED-NEXT:                               declarator: Name(
// ENABLED-NEXT:                                   "y",
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           },
// ENABLED-NEXT:                       ],
// ENABLED-NEXT:                       provenance: Provenance {
// ENABLED-NEXT:                           file: FileId(
// ENABLED-NEXT:                               3,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                           kind: User,
// ENABLED-NEXT:                           line: 5,
// ENABLED-NEXT:                           header: None,
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       ),
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 3,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[0]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Name(
// ENABLED-NEXT:                       "scalar",
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           Integer(
// ENABLED-NEXT:                               7,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 0,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[1]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Char {
// ENABLED-NEXT:                       signed: None,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Name(
// ENABLED-NEXT:                           "message",
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           Integer(
// ENABLED-NEXT:                               6,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   },
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           StringLit(
// ENABLED-NEXT:                               "hello",
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 1,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[2]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Array {
// ENABLED-NEXT:                           inner: Name(
// ENABLED-NEXT:                               "matrix",
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                           size: Expression(
// ENABLED-NEXT:                               Integer(
// ENABLED-NEXT:                                   2,
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           Integer(
// ENABLED-NEXT:                               2,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   },
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       List(
// ENABLED-NEXT:                           [
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [],
// ENABLED-NEXT:                                   value: List(
// ENABLED-NEXT:                                       [
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   Integer(
// ENABLED-NEXT:                                                       1,
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   Integer(
// ENABLED-NEXT:                                                       2,
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                       ],
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [],
// ENABLED-NEXT:                                   value: List(
// ENABLED-NEXT:                                       [
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   Integer(
// ENABLED-NEXT:                                                       3,
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   Integer(
// ENABLED-NEXT:                                                       4,
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                       ],
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 2,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[3]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Tag(
// ENABLED-NEXT:                   Definition(
// ENABLED-NEXT:                       TagId(
// ENABLED-NEXT:                           0,
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 3,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[4]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Tag(
// ENABLED-NEXT:                   Reference {
// ENABLED-NEXT:                       kind: Struct,
// ENABLED-NEXT:                       name: "Point",
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Name(
// ENABLED-NEXT:                       "point",
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       List(
// ENABLED-NEXT:                           [
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Field(
// ENABLED-NEXT:                                           "y",
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       Integer(
// ENABLED-NEXT:                                           9,
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Field(
// ENABLED-NEXT:                                           "x",
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       Integer(
// ENABLED-NEXT:                                           4,
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 7,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[5]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Name(
// ENABLED-NEXT:                           "values",
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           Integer(
// ENABLED-NEXT:                               4,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   },
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       List(
// ENABLED-NEXT:                           [
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Array(
// ENABLED-NEXT:                                           Integer(
// ENABLED-NEXT:                                               2,
// ENABLED-NEXT:                                           ),
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       Integer(
// ENABLED-NEXT:                                           8,
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Array(
// ENABLED-NEXT:                                           Integer(
// ENABLED-NEXT:                                               0,
// ENABLED-NEXT:                                           ),
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       Integer(
// ENABLED-NEXT:                                           1,
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 8,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[6]: Declaration {
// ENABLED-NEXT:       declaration: Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclarator {
// ENABLED-NEXT:                   declarator: Name(
// ENABLED-NEXT:                       "selected",
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           Integer(
// ENABLED-NEXT:                               11,
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:       provenance: Provenance {
// ENABLED-NEXT:           file: FileId(
// ENABLED-NEXT:               3,
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           kind: User,
// ENABLED-NEXT:           line: 9,
// ENABLED-NEXT:           header: None,
// ENABLED-NEXT:       },
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[7]: Function(
// ENABLED-NEXT:       FunctionDecl {
// ENABLED-NEXT:           ret_type: Integer(
// ENABLED-NEXT:               Ranked {
// ENABLED-NEXT:                   rank: Int,
// ENABLED-NEXT:                   signed: true,
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ),
// ENABLED-NEXT:           name: "main",
// ENABLED-NEXT:           body: [
// ENABLED-NEXT:               Return(
// ENABLED-NEXT:                   Integer(
// ENABLED-NEXT:                       0,
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           ],
// ENABLED-NEXT:           provenance: Provenance {
// ENABLED-NEXT:               file: FileId(
// ENABLED-NEXT:                   3,
// ENABLED-NEXT:               ),
// ENABLED-NEXT:               kind: User,
// ENABLED-NEXT:               line: 16,
// ENABLED-NEXT:               header: None,
// ENABLED-NEXT:           },
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// SLATE-FILECHECK-END ENABLED
