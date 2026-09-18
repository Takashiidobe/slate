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
// DEFAULT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG0:]],
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
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "y",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "scalar",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 7,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "7",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "message",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 6,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "6",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       104,
// DEFAULT-NEXT:                                       101,
// DEFAULT-NEXT:                                       108,
// DEFAULT-NEXT:                                       108,
// DEFAULT-NEXT:                                       111,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "hello",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "matrix",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 2,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "2",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 2,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "2",
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 1,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 2,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "2",
// DEFAULT-NEXT:                                                       },
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
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 3,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "3",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 4,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "4",
// DEFAULT-NEXT:                                                       },
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
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG0]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Reference {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: "Point",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
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
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 9,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "9",
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 4,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "4",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "values",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 4,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "4",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Array(
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 8,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "8",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [
// DEFAULT-NEXT:                                       Array(
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 0,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "0",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 1,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "1",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "selected",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 22,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "22",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
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
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ENABLED
// ENABLED: tag[{{[0-9]+}}]: TagDefinition {
// ENABLED-NEXT:       id: TagId(
// ENABLED-NEXT:           [[#TAG0:]],
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
// ENABLED-NEXT:                           FieldDeclaratorKind {
// ENABLED-NEXT:                               declarator: Name(
// ENABLED-NEXT:                                   "x",
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           },
// ENABLED-NEXT:                       ],
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
// ENABLED-NEXT:                           FieldDeclaratorKind {
// ENABLED-NEXT:                               declarator: Name(
// ENABLED-NEXT:                                   "y",
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           },
// ENABLED-NEXT:                       ],
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       ),
// ENABLED-NEXT:   }
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
// ENABLED-NEXT:                   declarator: Name(
// ENABLED-NEXT:                       "scalar",
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           IntegerLiteral(
// ENABLED-NEXT:                               IntegerLiteral {
// ENABLED-NEXT:                                   value: 7,
// ENABLED-NEXT:                                   radix: Decimal,
// ENABLED-NEXT:                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                       unsigned: false,
// ENABLED-NEXT:                                       size: None,
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                                   spelling: "7",
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Char {
// ENABLED-NEXT:                       signed: None,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Name(
// ENABLED-NEXT:                           "message",
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           IntegerLiteral(
// ENABLED-NEXT:                               IntegerLiteral {
// ENABLED-NEXT:                                   value: 6,
// ENABLED-NEXT:                                   radix: Decimal,
// ENABLED-NEXT:                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                       unsigned: false,
// ENABLED-NEXT:                                       size: None,
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                                   spelling: "6",
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   },
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           StringLiteral(
// ENABLED-NEXT:                               StringLiteral {
// ENABLED-NEXT:                                   encoding: Plain,
// ENABLED-NEXT:                                   code_units: [
// ENABLED-NEXT:                                       104,
// ENABLED-NEXT:                                       101,
// ENABLED-NEXT:                                       108,
// ENABLED-NEXT:                                       108,
// ENABLED-NEXT:                                       111,
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   pieces: [
// ENABLED-NEXT:                                       "hello",
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Array {
// ENABLED-NEXT:                           inner: Name(
// ENABLED-NEXT:                               "matrix",
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                           size: Expression(
// ENABLED-NEXT:                               IntegerLiteral(
// ENABLED-NEXT:                                   IntegerLiteral {
// ENABLED-NEXT:                                       value: 2,
// ENABLED-NEXT:                                       radix: Decimal,
// ENABLED-NEXT:                                       suffix: IntegerSuffix {
// ENABLED-NEXT:                                           unsigned: false,
// ENABLED-NEXT:                                           size: None,
// ENABLED-NEXT:                                       },
// ENABLED-NEXT:                                       spelling: "2",
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                               ),
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           IntegerLiteral(
// ENABLED-NEXT:                               IntegerLiteral {
// ENABLED-NEXT:                                   value: 2,
// ENABLED-NEXT:                                   radix: Decimal,
// ENABLED-NEXT:                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                       unsigned: false,
// ENABLED-NEXT:                                       size: None,
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                                   spelling: "2",
// ENABLED-NEXT:                               },
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
// ENABLED-NEXT:                                                   IntegerLiteral(
// ENABLED-NEXT:                                                       IntegerLiteral {
// ENABLED-NEXT:                                                           value: 1,
// ENABLED-NEXT:                                                           radix: Decimal,
// ENABLED-NEXT:                                                           suffix: IntegerSuffix {
// ENABLED-NEXT:                                                               unsigned: false,
// ENABLED-NEXT:                                                               size: None,
// ENABLED-NEXT:                                                           },
// ENABLED-NEXT:                                                           spelling: "1",
// ENABLED-NEXT:                                                       },
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   IntegerLiteral(
// ENABLED-NEXT:                                                       IntegerLiteral {
// ENABLED-NEXT:                                                           value: 2,
// ENABLED-NEXT:                                                           radix: Decimal,
// ENABLED-NEXT:                                                           suffix: IntegerSuffix {
// ENABLED-NEXT:                                                               unsigned: false,
// ENABLED-NEXT:                                                               size: None,
// ENABLED-NEXT:                                                           },
// ENABLED-NEXT:                                                           spelling: "2",
// ENABLED-NEXT:                                                       },
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
// ENABLED-NEXT:                                                   IntegerLiteral(
// ENABLED-NEXT:                                                       IntegerLiteral {
// ENABLED-NEXT:                                                           value: 3,
// ENABLED-NEXT:                                                           radix: Decimal,
// ENABLED-NEXT:                                                           suffix: IntegerSuffix {
// ENABLED-NEXT:                                                               unsigned: false,
// ENABLED-NEXT:                                                               size: None,
// ENABLED-NEXT:                                                           },
// ENABLED-NEXT:                                                           spelling: "3",
// ENABLED-NEXT:                                                       },
// ENABLED-NEXT:                                                   ),
// ENABLED-NEXT:                                               ),
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                           InitializerItem {
// ENABLED-NEXT:                                               designators: [],
// ENABLED-NEXT:                                               value: Expr(
// ENABLED-NEXT:                                                   IntegerLiteral(
// ENABLED-NEXT:                                                       IntegerLiteral {
// ENABLED-NEXT:                                                           value: 4,
// ENABLED-NEXT:                                                           radix: Decimal,
// ENABLED-NEXT:                                                           suffix: IntegerSuffix {
// ENABLED-NEXT:                                                               unsigned: false,
// ENABLED-NEXT:                                                               size: None,
// ENABLED-NEXT:                                                           },
// ENABLED-NEXT:                                                           spelling: "4",
// ENABLED-NEXT:                                                       },
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
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Tag(
// ENABLED-NEXT:                   Definition(
// ENABLED-NEXT:                       TagId(
// ENABLED-NEXT:                           [[#TAG0]],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Tag(
// ENABLED-NEXT:                   Reference {
// ENABLED-NEXT:                       kind: Struct,
// ENABLED-NEXT:                       name: "Point",
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
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
// ENABLED-NEXT:                                       IntegerLiteral(
// ENABLED-NEXT:                                           IntegerLiteral {
// ENABLED-NEXT:                                               value: 9,
// ENABLED-NEXT:                                               radix: Decimal,
// ENABLED-NEXT:                                               suffix: IntegerSuffix {
// ENABLED-NEXT:                                                   unsigned: false,
// ENABLED-NEXT:                                                   size: None,
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                               spelling: "9",
// ENABLED-NEXT:                                           },
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
// ENABLED-NEXT:                                       IntegerLiteral(
// ENABLED-NEXT:                                           IntegerLiteral {
// ENABLED-NEXT:                                               value: 4,
// ENABLED-NEXT:                                               radix: Decimal,
// ENABLED-NEXT:                                               suffix: IntegerSuffix {
// ENABLED-NEXT:                                                   unsigned: false,
// ENABLED-NEXT:                                                   size: None,
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                               spelling: "4",
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
// ENABLED-NEXT:                   declarator: Array {
// ENABLED-NEXT:                       inner: Name(
// ENABLED-NEXT:                           "values",
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                       size: Expression(
// ENABLED-NEXT:                           IntegerLiteral(
// ENABLED-NEXT:                               IntegerLiteral {
// ENABLED-NEXT:                                   value: 4,
// ENABLED-NEXT:                                   radix: Decimal,
// ENABLED-NEXT:                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                       unsigned: false,
// ENABLED-NEXT:                                       size: None,
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                                   spelling: "4",
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   },
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       List(
// ENABLED-NEXT:                           [
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Array(
// ENABLED-NEXT:                                           IntegerLiteral(
// ENABLED-NEXT:                                               IntegerLiteral {
// ENABLED-NEXT:                                                   value: 2,
// ENABLED-NEXT:                                                   radix: Decimal,
// ENABLED-NEXT:                                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                                       unsigned: false,
// ENABLED-NEXT:                                                       size: None,
// ENABLED-NEXT:                                                   },
// ENABLED-NEXT:                                                   spelling: "2",
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                           ),
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       IntegerLiteral(
// ENABLED-NEXT:                                           IntegerLiteral {
// ENABLED-NEXT:                                               value: 8,
// ENABLED-NEXT:                                               radix: Decimal,
// ENABLED-NEXT:                                               suffix: IntegerSuffix {
// ENABLED-NEXT:                                                   unsigned: false,
// ENABLED-NEXT:                                                   size: None,
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                               spelling: "8",
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                               InitializerItem {
// ENABLED-NEXT:                                   designators: [
// ENABLED-NEXT:                                       Array(
// ENABLED-NEXT:                                           IntegerLiteral(
// ENABLED-NEXT:                                               IntegerLiteral {
// ENABLED-NEXT:                                                   value: 0,
// ENABLED-NEXT:                                                   radix: Decimal,
// ENABLED-NEXT:                                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                                       unsigned: false,
// ENABLED-NEXT:                                                       size: None,
// ENABLED-NEXT:                                                   },
// ENABLED-NEXT:                                                   spelling: "0",
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                           ),
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ],
// ENABLED-NEXT:                                   value: Expr(
// ENABLED-NEXT:                                       IntegerLiteral(
// ENABLED-NEXT:                                           IntegerLiteral {
// ENABLED-NEXT:                                               value: 1,
// ENABLED-NEXT:                                               radix: Decimal,
// ENABLED-NEXT:                                               suffix: IntegerSuffix {
// ENABLED-NEXT:                                                   unsigned: false,
// ENABLED-NEXT:                                                   size: None,
// ENABLED-NEXT:                                               },
// ENABLED-NEXT:                                               spelling: "1",
// ENABLED-NEXT:                                           },
// ENABLED-NEXT:                                       ),
// ENABLED-NEXT:                                   ),
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ],
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Declaration(
// ENABLED-NEXT:       Declaration {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarators: [
// ENABLED-NEXT:               InitDeclaratorKind {
// ENABLED-NEXT:                   declarator: Name(
// ENABLED-NEXT:                       "selected",
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:                   initializer: Some(
// ENABLED-NEXT:                       Expr(
// ENABLED-NEXT:                           IntegerLiteral(
// ENABLED-NEXT:                               IntegerLiteral {
// ENABLED-NEXT:                                   value: 11,
// ENABLED-NEXT:                                   radix: Decimal,
// ENABLED-NEXT:                                   suffix: IntegerSuffix {
// ENABLED-NEXT:                                       unsigned: false,
// ENABLED-NEXT:                                       size: None,
// ENABLED-NEXT:                                   },
// ENABLED-NEXT:                                   spelling: "11",
// ENABLED-NEXT:                               },
// ENABLED-NEXT:                           ),
// ENABLED-NEXT:                       ),
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               },
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// ENABLED-NEXT: decl[{{[0-9]+}}]: Function(
// ENABLED-NEXT:       FunctionDefinition {
// ENABLED-NEXT:           specifiers: DeclarationSpecifiers {
// ENABLED-NEXT:               ty: Integer(
// ENABLED-NEXT:                   Ranked {
// ENABLED-NEXT:                       rank: Int,
// ENABLED-NEXT:                       signed: true,
// ENABLED-NEXT:                   },
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           },
// ENABLED-NEXT:           declarator: Function {
// ENABLED-NEXT:               inner: Name(
// ENABLED-NEXT:                   "main",
// ENABLED-NEXT:               ),
// ENABLED-NEXT:               parameters: Empty,
// ENABLED-NEXT:           },
// ENABLED-NEXT:           body: [
// ENABLED-NEXT:               Return(
// ENABLED-NEXT:                   IntegerLiteral(
// ENABLED-NEXT:                       IntegerLiteral {
// ENABLED-NEXT:                           value: 0,
// ENABLED-NEXT:                           radix: Decimal,
// ENABLED-NEXT:                           suffix: IntegerSuffix {
// ENABLED-NEXT:                               unsigned: false,
// ENABLED-NEXT:                               size: None,
// ENABLED-NEXT:                           },
// ENABLED-NEXT:                           spelling: "0",
// ENABLED-NEXT:                       },
// ENABLED-NEXT:                   ),
// ENABLED-NEXT:               ),
// ENABLED-NEXT:           ],
// ENABLED-NEXT:       },
// ENABLED-NEXT:   )
// SLATE-FILECHECK-END ENABLED
