/* PR tree-optimization/86622 - incorrect strlen of array of array plus
   variable offset
   Exercise strlen() with a multi-dimensional array of strings with
   offsets.  */

extern int           printf(const char *, ...);
extern __SIZE_TYPE__ strlen(const char *);

typedef char  A28[28];
typedef A28   A3_28[3];
typedef A3_28 A2_3_28[2];

static const A2_3_28 a = {
    /* [0][0]    [0][1]         [0][2] */
    {"1\00012", "123\0001234", "12345\000123456"},
    /* [1][0]    [1][1]         [1][2] */
    {"1234567\00012345678", "123456789\0001234567890",
     "12345678901\000123456789012"}};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;
volatile int v3 = 3;
volatile int v4 = 4;
volatile int v5 = 5;
volatile int v6 = 6;
volatile int v7 = 7;

#define A(expr, N)                                                             \
  ((strlen(expr) == N) ? (void)0                                               \
                       : (printf("line %i: strlen (%s = \"%s\") != %i\n",      \
                                 __LINE__, #expr, expr, N),                    \
                          __builtin_abort()))

/* Verify that strlen() involving pointer to array arguments computes
   the correct result.  */

void test_array_ptr(void) {
  /* Compute the length of the string at the refeenced array.  */
  A(*(&a[0][0] + 0), 1);
  A(*(&a[0][0] + 1), 3);
  A(*(&a[0][0] + 2), 5);

  A(*(&a[0][1] - 1), 1);
  A(*(&a[0][1] + 0), 3);
  A(*(&a[0][1] + 1), 5);

  A(*(&a[0][2] - 2), 1);
  A(*(&a[0][2] - 1), 3);
  A(*(&a[0][2] + 0), 5);

  A(*(&a[1][0] + 0), 7);
  A(*(&a[1][0] + 1), 9);
  A(*(&a[1][0] + 2), 11);

  A(*(&a[1][1] - 1), 7);
  A(*(&a[1][1] + 0), 9);
  A(*(&a[1][1] + 1), 11);

  A(*(&a[1][2] - 2), 7);
  A(*(&a[1][2] - 1), 9);
  A(*(&a[1][2] - 0), 11);

  /* Compute the length of the string past the first nul.  */
  A(*(&a[0][0] + 0) + 2, 2);
  A(*(&a[0][0] + 1) + 4, 4);
  A(*(&a[0][0] + 2) + 6, 6);

  /* Compute the length of the string past the second nul.  */
  A(*(&a[0][0] + 0) + 5, 0);
  A(*(&a[0][0] + 1) + 10, 0);
  A(*(&a[0][0] + 2) + 14, 0);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;
  int i5 = i4 + 1;

  A(*(&a[0][0] + i0), 1);
  A(*(&a[0][0] + i1), 3);
  A(*(&a[0][0] + i2), 5);

  A(*(&a[0][1] - i1), 1);
  A(*(&a[0][1] + i0), 3);
  A(*(&a[0][1] + i1), 5);

  A(*(&a[0][2] - i2), 1);
  A(*(&a[0][2] - i1), 3);
  A(*(&a[0][2] + i0), 5);

  A(*(&a[1][0] + i0), 7);
  A(*(&a[1][0] + i1), 9);
  A(*(&a[1][0] + i2), 11);

  A(*(&a[1][1] - i1), 7);
  A(*(&a[1][1] + i0), 9);
  A(*(&a[1][1] + i1), 11);

  A(*(&a[1][2] - i2), 7);
  A(*(&a[1][2] - i1), 9);
  A(*(&a[1][2] - i0), 11);

  A(*(&a[i0][i0] + i0), 1);
  A(*(&a[i0][i0] + i1), 3);
  A(*(&a[i0][i0] + i2), 5);

  A(*(&a[i0][i1] - i1), 1);
  A(*(&a[i0][i1] + i0), 3);
  A(*(&a[i0][i1] + i1), 5);

  A(*(&a[i0][i2] - i2), 1);
  A(*(&a[i0][i2] - i1), 3);
  A(*(&a[i0][i2] + i0), 5);

  A(*(&a[i1][i0] + i0), 7);
  A(*(&a[i1][i0] + i1), 9);
  A(*(&a[i1][i0] + i2), 11);

  A(*(&a[i1][i1] - i1), 7);
  A(*(&a[i1][i1] + i0), 9);
  A(*(&a[i1][i1] + i1), 11);

  A(*(&a[i1][i2] - i2), 7);
  A(*(&a[i1][i2] - i1), 9);
  A(*(&a[i1][i2] - i0), 11);

  A(*(&a[i0][i0] + v0), 1);
  A(*(&a[i0][i0] + v1), 3);
  A(*(&a[i0][i0] + v2), 5);

  A(*(&a[i0][i1] - v1), 1);
  A(*(&a[i0][i1] + v0), 3);
  A(*(&a[i0][i1] + v1), 5);

  A(*(&a[i0][i2] - v2), 1);
  A(*(&a[i0][i2] - v1), 3);
  A(*(&a[i0][i2] + v0), 5);

  A(*(&a[i1][i0] + v0), 7);
  A(*(&a[i1][i0] + v1), 9);
  A(*(&a[i1][i0] + v2), 11);

  A(*(&a[i1][i1] - v1), 7);
  A(*(&a[i1][i1] + v0), 9);
  A(*(&a[i1][i1] + v1), 11);

  A(*(&a[i1][i2] - v2), 7);
  A(*(&a[i1][i2] - v1), 9);
  A(*(&a[i1][i2] - v0), 11);

  A(*(&a[i0][i0] + v0) + i1, 0);
  A(*(&a[i0][i0] + v1) + i2, 1);
  A(*(&a[i0][i0] + v2) + i3, 2);

  A(*(&a[i0][i1] - v1) + v1, 0);
  A(*(&a[i0][i1] + v0) + v3, 0);
  A(*(&a[i0][i1] + v1) + v5, 0);

  A(*(&a[i0][v1] - i1) + i1, 0);
  A(*(&a[i0][v1] + i0) + i3, 0);
  A(*(&a[i0][v1] + i1) + i5, 0);
}

static const A3_28 *const pa0 = &a[0];
static const A3_28 *const pa1 = &a[1];

static const A3_28 *const paa[] = {&a[0], &a[1]};

/* Verify that strlen() involving pointers and arrays of pointers
   to array arguments computes the correct result.  */

void test_ptr_array(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;

  A(*((*pa0) + i0), 1);
  A(*((*pa0) + i1), 3);
  A(*((*pa0) + i2), 5);

  A(*(pa0[0] + i0), 1);
  A(*(pa0[0] + i1), 3);
  A(*(pa0[0] + i2), 5);

  A((*pa0)[i0] + i1, 0);
  A((*pa0)[i1] + i2, 1);
  A((*pa0)[i2] + i3, 2);

  A(*((*pa1) + i0), 7);
  A(*((*pa1) + i1), 9);
  A(*((*pa1) + i2), 11);

  A(*(pa1[0] + i0), 7);
  A(*(pa1[0] + i1), 9);
  A(*(pa1[0] + i2), 11);

  A((*pa1)[i0] + i1, 6);
  A((*pa1)[i1] + i2, 7);
  A((*pa1)[i2] + i3, 8);

  A(*(*(paa[0]) + i0), 1);
  A(*(*(paa[0]) + i1), 3);
  A(*(*(paa[0]) + i2), 5);

  A(*(*(paa[1]) + i0), 7);
  A(*(*(paa[1]) + i1), 9);
  A(*(*(paa[1]) + i2), 11);

  A(*(*(paa[1]) - i1), 5);
  A(*(*(paa[1]) - i2), 3);
  A(*(*(paa[1]) - i3), 1);

  A(*(*(paa[0]) + i0) + i1, 0);
  A(*(*(paa[0]) + i1) + i2, 1);
  A(*(*(paa[0]) + i2) + i3, 2);
}

int main(void) {
  test_array_ptr();

  test_ptr_array();
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR tree-optimization/86622 - incorrect strlen of array of array plus\n   variable offset\n   Exercise strlen() with a multi-dimensional array of strings with\n   offsets.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 174,
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
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
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
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "strlen",
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
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Typedef {
// DEFAULT-NEXT:       name: "A28",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Char {
// DEFAULT-NEXT:               signed: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Typedef {
// DEFAULT-NEXT:       name: "A3_28",
// DEFAULT-NEXT:       ty: Named(
// DEFAULT-NEXT:           "A28",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Typedef {
// DEFAULT-NEXT:       name: "A2_3_28",
// DEFAULT-NEXT:       ty: Named(
// DEFAULT-NEXT:           "A3_28",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 10,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "A2_3_28",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "a",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               List(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "1\\00012",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "123\\0001234",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "12345\\000123456",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "1234567\\00012345678",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "123456789\\0001234567890",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "12345678901\\000123456789012",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v0",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 19,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v1",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 20,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v2",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           2,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 21,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v3",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 22,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[11]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v4",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           4,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 23,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v5",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           5,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v6",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           6,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 25,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "v7",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           7,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 26,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Comment {
// DEFAULT-NEXT:       text: "/* Verify that strlen() involving pointer to array arguments computes\n   the correct result.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 1138,
// DEFAULT-NEXT:           length: 96,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 34,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[16]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "test_array_ptr",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Compute the length of the string at the refeenced array.  */",
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 1266,
// DEFAULT-NEXT:                       length: 63,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: User,
// DEFAULT-NEXT:                       line: 38,
// DEFAULT-NEXT:                       header: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           40,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           41,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           42,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           44,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] - 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           45,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] + 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           46,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] + 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           48,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] - 2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           49,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] - 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           50,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] + 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           52,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           53,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           54,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + 2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           56,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] - 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           57,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] + 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           58,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] + 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           60,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - 2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           61,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - 1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           62,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - 0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Compute the length of the string past the first nul.  */",
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 1791,
// DEFAULT-NEXT:                       length: 59,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: User,
// DEFAULT-NEXT:                       line: 63,
// DEFAULT-NEXT:                       header: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           65,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 0 ) + 2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           66,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 1 ) + 4",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               6,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   6,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           67,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 2 ) + 6",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               6,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           6,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Compute the length of the string past the second nul.  */",
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 1941,
// DEFAULT-NEXT:                       length: 60,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: User,
// DEFAULT-NEXT:                       line: 68,
// DEFAULT-NEXT:                       header: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               5,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           70,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 0 ) + 5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               5,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               10,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           71,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 1 ) + 10",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               10,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               14,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           72,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + 2 ) + 14",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               14,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
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
// DEFAULT-NEXT:                           "i0",
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i4",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i5",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i4",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           81,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           82,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           83,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           85,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           86,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           87,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 1 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           89,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] - i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           90,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           91,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 0 ] [ 2 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           93,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           94,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           95,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           97,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           98,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           99,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 1 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           101,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           102,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           103,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ 1 ] [ 2 ] - i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           105,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           106,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           107,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           109,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           110,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           111,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           113,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] - i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           114,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           115,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           117,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           118,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           119,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           121,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           122,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           123,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           125,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           126,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           127,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           129,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           130,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           131,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           133,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] - v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           134,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           135,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           137,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] - v2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           138,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] - v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           139,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i2 ] + v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           141,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           142,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           143,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i0 ] + v2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           145,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] - v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           146,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] + v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           147,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i1 ] + v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           149,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - v2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           150,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - v1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           151,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i1 ] [ i2 ] - v0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: AddrOf(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           153,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v0 ) + i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           154,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v1 ) + i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           155,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i0 ] + v2 ) + i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           157,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] - v1 ) + v1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           158,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + v0 ) + v3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           159,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ i1 ] + v1 ) + v5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
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
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "v1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "v5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           161,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ v1 ] - i1 ) + i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           162,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ v1 ] + i0 ) + i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           163,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( & a [ i0 ] [ v1 ] + i1 ) + i5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "a",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "v1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               line: 37,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[17]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "A3_28",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Pointer {
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "pa0",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       AddrOf(
// DEFAULT-NEXT:                           Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 165,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[18]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "A3_28",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Pointer {
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "pa1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       AddrOf(
// DEFAULT-NEXT:                           Index {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 166,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[19]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "A3_28",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                       is_const: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "paa",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               size: Unspecified,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               List(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 168,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[20]: Comment {
// DEFAULT-NEXT:       text: "/* Verify that strlen() involving pointers and arrays of pointers\n   to array arguments computes the correct result.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 4137,
// DEFAULT-NEXT:           length: 120,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[21]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "test_ptr_array",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i0",
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
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
// DEFAULT-NEXT:                           "i3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           180,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa0 ) + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           181,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa0 ) + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           182,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa0 ) + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           184,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa0 [ 0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           185,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa0 [ 0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           186,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa0 [ 0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           188,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa0 ) [ i0 ] + i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           189,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa0 ) [ i1 ] + i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           190,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa0 ) [ i2 ] + i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           192,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa1 ) + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           193,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa1 ) + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           194,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( ( * pa1 ) + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           196,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa1 [ 0 ] + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           197,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa1 [ 0 ] + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           198,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( pa1 [ 0 ] + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   6,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           200,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa1 ) [ i0 ] + i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           6,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           201,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa1 ) [ i1 ] + i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           202,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "( * pa1 ) [ i2 ] + i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Deref(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "pa1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           204,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           205,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           206,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   7,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           208,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) + i0 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           7,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           209,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) + i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           9,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           210,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) + i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           11,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           212,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) - i1 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           213,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) - i2 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           214,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 1 ] ) - i3 )",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Deref(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: Deref(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "paa",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           216,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i0 ) + i1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           217,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i1 ) + i2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Ternary {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_value: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           else_value: Comma(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "line %i: strlen (%s = \\\"%s\\\") != %i\\n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           218,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       StringLit(
// DEFAULT-NEXT:                                           "* ( * ( paa [ 0 ] ) + i2 ) + i3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Deref(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Deref(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "paa",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "i2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               line: 173,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[22]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "test_array_ptr",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "test_ptr_array",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 220,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
