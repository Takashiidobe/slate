/* PR tree-optimization/86532 - Wrong code due to a wrong strlen folding
   starting with r262522
   Exercise strlen() with a multi-dimensional array of strings with
   embedded nuls.  */

extern __SIZE_TYPE__ strlen(const char *);

static const char a[2][3][9] = {{"1", "1\0002"}, {"12\0003", "123\0004"}};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;
volatile int v3 = 3;
volatile int v4 = 4;
volatile int v5 = 5;
volatile int v6 = 6;
volatile int v7 = 7;

#define A(expr)                                                                \
  ((expr) ? (void)0                                                            \
          : (__builtin_printf("assertion on line %i: %s\n", __LINE__, #expr),  \
             __builtin_abort()))

void test_array_ref(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;
  int i5 = i4 + 1;
  int i6 = i5 + 1;
  int i7 = i6 + 1;

  A(strlen(a[0][0]) == 1);
  A(strlen(a[0][1]) == 1);

  A(strlen(a[1][0]) == 2);
  A(strlen(a[1][1]) == 3);

  A(strlen(&a[0][0][0]) == 1);
  A(strlen(&a[0][1][0]) == 1);

  A(strlen(&a[1][0][0]) == 2);
  A(strlen(&a[1][1][0]) == 3);

  A(strlen(&a[0][0][0] + 1) == 0);
  A(strlen(&a[0][1][0] + 1) == 0);
  A(strlen(&a[0][1][0] + 2) == 1);
  A(strlen(&a[0][1][0] + 3) == 0);
  A(strlen(&a[0][1][0] + 7) == 0);

  A(strlen(&a[1][0][0] + 1) == 1);
  A(strlen(&a[1][1][0] + 1) == 2);
  A(strlen(&a[1][1][0] + 2) == 1);
  A(strlen(&a[1][1][0] + 7) == 0);

  A(strlen(a[i0][i0]) == 1);
  A(strlen(a[i0][i1]) == 1);

  A(strlen(a[i1][i0]) == 2);
  A(strlen(a[i1][i1]) == 3);

  A(strlen(&a[i0][i0][i0]) == 1);
  A(strlen(&a[i0][i1][i0]) == 1);
  A(strlen(&a[i0][i1][i1]) == 0);
  A(strlen(&a[i0][i1][i2]) == 1);
  A(strlen(&a[i0][i1][i3]) == 0);
  A(strlen(&a[i0][i1][i3]) == 0);

  A(strlen(&a[i1][i0][i0]) == 2);
  A(strlen(&a[i1][i1][i0]) == 3);
  A(strlen(&a[i1][i1][i1]) == 2);
  A(strlen(&a[i1][i1][i2]) == 1);
  A(strlen(&a[i1][i1][i3]) == 0);
  A(strlen(&a[i1][i1][i4]) == 1);
  A(strlen(&a[i1][i1][i5]) == 0);
  A(strlen(&a[i1][i1][i6]) == 0);
  A(strlen(&a[i1][i1][i7]) == 0);

  A(strlen(&a[i0][i0][i0] + i1) == 0);
  A(strlen(&a[i0][i1][i0] + i1) == 0);
  A(strlen(&a[i0][i1][i0] + i7) == 0);

  A(strlen(&a[i1][i0][i0] + i1) == 1);
  A(strlen(&a[i1][i1][i0] + i1) == 2);
  A(strlen(&a[i1][i1][i0] + i2) == 1);
  A(strlen(&a[i1][i1][i0] + i3) == 0);
  A(strlen(&a[i1][i1][i0] + i4) == 1);
  A(strlen(&a[i1][i1][i0] + i5) == 0);
  A(strlen(&a[i1][i1][i0] + i6) == 0);
  A(strlen(&a[i1][i1][i0] + i7) == 0);

  A(strlen(a[i0][i0]) == 1);
  A(strlen(a[i0][i1]) == 1);

  A(strlen(a[i1][i0]) == 2);
  A(strlen(a[i1][i1]) == 3);

  A(strlen(&a[i0][i0][i0]) == 1);
  A(strlen(&a[i0][i1][i0]) == 1);

  A(strlen(&a[i1][i0][i0]) == 2);
  A(strlen(&a[i1][i1][i0]) == 3);

  A(strlen(&a[i0][i0][i0] + v1) == 0);
  A(strlen(&a[i0][i0][i0] + v2) == 0);
  A(strlen(&a[i0][i0][i0] + v7) == 0);

  A(strlen(&a[i0][i1][i0] + v1) == 0);
  A(strlen(&a[i0][i1][i0] + v2) == 1);
  A(strlen(&a[i0][i1][i0] + v3) == 0);

  A(strlen(&a[i1][i0][i0] + v1) == 1);
  A(strlen(&a[i1][i1][i0] + v1) == 2);
  A(strlen(&a[i1][i1][i0] + v2) == 1);
  A(strlen(&a[i1][i1][i0] + v3) == 0);
  A(strlen(&a[i1][i1][i0] + v4) == 1);
  A(strlen(&a[i1][i1][i0] + v5) == 0);
  A(strlen(&a[i1][i1][i0] + v6) == 0);
  A(strlen(&a[i1][i1][i0] + v7) == 0);
}

int main(void) { test_array_ref(); }


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "strlen",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Char {
// DEFAULT-NEXT:                                               signed: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Array {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               size: Expression(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 3,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "3",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 9,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "9",
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
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "1",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "1\\0002",
// DEFAULT-NEXT:                                                           ],
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
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "12\\0003",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                               52,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "123\\0004",
// DEFAULT-NEXT:                                                           ],
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
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v0",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v2",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v3",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 3,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "3",
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v4",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v5",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 5,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "5",
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v6",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v7",
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
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "test_array_ref",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i0",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i3",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i4",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i5",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i6",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i7",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i6",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 0,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 34,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "34",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[0][0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 35,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "35",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[0][1]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 0,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 37,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "37",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[1][0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 38,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "38",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[1][1]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 40,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "40",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][0][0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 1,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "1",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 41,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "41",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][1][0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 43,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "43",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][0][0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 1,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "1",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 44,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "44",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][1][0]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 46,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "46",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][0][0] + 1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 47,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "47",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][1][0] + 1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 2,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "2",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 48,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "48",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][1][0] + 2) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 3,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "3",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 49,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "49",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][1][0] + 3) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 7,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "7",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 50,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "50",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[0][1][0] + 7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 52,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "52",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][0][0] + 1) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 53,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "53",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][1][0] + 1) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 2,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "2",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 54,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "54",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][1][0] + 2) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 7,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "7",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 55,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "55",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[1][1][0] + 7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 57,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "57",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i0][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 58,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "58",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i0][i1]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 60,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "60",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i1][i0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 61,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "61",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i1][i1]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 63,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "63",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 64,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "64",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 65,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "65",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i1]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 66,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "66",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i2]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 67,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "67",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i3]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 68,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "68",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i3]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 70,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "70",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i0][i0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 71,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "71",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 72,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "72",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i1]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 73,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "73",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i2]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 74,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "74",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i3]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 75,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "75",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i4]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i5",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 76,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "76",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i5]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i6",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 77,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "77",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       54,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i6]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i7",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 78,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "78",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i7]) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 80,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "80",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0] + i1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 81,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "81",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0] + i1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i7",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 82,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "82",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0] + i7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 84,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "84",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i0][i0] + i1) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 85,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "85",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i1) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 86,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "86",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i2) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 87,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "87",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i3) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 88,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "88",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i4) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i5",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 89,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "89",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i5) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i6",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 90,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "90",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       54,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i6) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i7",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 91,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "91",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + i7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 93,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "93",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i0][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 94,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "94",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i0][i1]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 96,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "96",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i1][i0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 97,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "97",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(a[i1][i1]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 99,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "99",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 100,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "100",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0]) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 102,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "102",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i0][i0]) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 3,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 103,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "103",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0]) == 3",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 105,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "105",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0] + v1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 106,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "106",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0] + v2) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v7",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 107,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "107",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i0][i0] + v7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 109,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "109",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0] + v1) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 110,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "110",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0] + v2) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 111,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "111",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i0][i1][i0] + v3) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 113,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "113",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i0][i0] + v1) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 114,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "114",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v1) == 2",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 115,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "115",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v2) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 116,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "116",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v3) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 117,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "117",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v4) == 1",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v5",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 118,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "118",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v5) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v6",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 119,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "119",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       54,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v6) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strlen",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v7",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           else_value: Paren(
// DEFAULT-NEXT:                               Comma {
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       58,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       37,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "assertion on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 120,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "120",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       55,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "strlen(&a[i1][i1][i0] + v7) == 0",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__builtin_abort",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_array_ref",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
