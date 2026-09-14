/* Test to verify that strlen() calls with conditional expressions
   and unterminated arrays or pointers to such things as arguments
   are evaluated without making assumptions about array sizes.  */

extern __SIZE_TYPE__ strlen(const char *);

unsigned nfails;

#define A(expr, N)                                                             \
  do {                                                                         \
    const char *_s = (expr);                                                   \
    unsigned    _n = strlen(_s);                                               \
    ((_n == N) ? 0                                                             \
               : (__builtin_printf("line %i: strlen ((%s) = (\"%s\"))"         \
                                   " == %u failed\n",                          \
                                   __LINE__, #expr, _s, N),                    \
                  ++nfails));                                                  \
  } while (0)

volatile int i0 = 0;

const char ca[2][3] = {"12"};
const char cb[2][3] = {{
                           '1',
                           '2',
                           '3',
                       },
                       {'4'}};

char va[2][3] = {"123"};
char vb[2][3] = {{
                     '1',
                     '2',
                     '3',
                 },
                 {'4', '5'}};

const char *s = "123456";

static void test_binary_cond_expr_global(void) {
  A(i0 ? "1" : ca[0], 2);
  A(i0 ? ca[0] : "123", 3);

  /* The call to strlen (cb[0]) is strictly undefined because the array
     isn't nul-terminated.  This test verifies that the strlen range
     optimization doesn't assume that the argument is necessarily nul
     terminated.
     Ditto for strlen (vb[0]).  */
  A(i0 ? "1" : cb[0], 4); /* GCC 8.2 failure */
  A(i0 ? cb[0] : "12", 2);

  A(i0 ? "1" : va[0], 3); /* GCC 8.2 failure */
  A(i0 ? va[0] : "1234", 4);

  A(i0 ? "1" : vb[0], 5); /* GCC 8.2 failure */
  A(i0 ? vb[0] : "12", 2);
}

static void test_binary_cond_expr_local(void) {
  const char lca[2][3] = {"12"};
  const char lcb[2][3] = {{
                              '1',
                              '2',
                              '3',
                          },
                          {'4'}};

  char lva[2][3] = {"123"};
  char lvb[2][3] = {{
                        '1',
                        '2',
                        '3',
                    },
                    {'4', '5'}};

  /* Also undefined as above.  */
  A(i0 ? "1" : lca[0], 2);
  A(i0 ? lca[0] : "123", 3);

  A(i0 ? "1" : lcb[0], 4); /* GCC 8.2 failure */
  A(i0 ? lcb[0] : "12", 2);

  A(i0 ? "1" : lva[0], 3); /* GCC 8.2 failure */
  A(i0 ? lva[0] : "1234", 4);

  A(i0 ? "1" : lvb[0], 5); /* GCC 8.2 failure */
  A(i0 ? lvb[0] : "12", 2);
}

static void test_ternary_cond_expr(void) {
  /* Also undefined.  */
  A(i0 == 0 ? s : i0 == 1 ? vb[0] : "123", 6);
  A(i0 == 0 ? vb[0] : i0 == 1 ? s : "123", 5);
  A(i0 == 0 ? "123" : i0 == 1 ? s : vb[0], 3);
}

const char (*pca)[3] = &ca[0];
const char (*pcb)[3] = &cb[0];

char (*pva)[3] = &va[0];
char (*pvb)[3] = &vb[0];

static void test_binary_cond_expr_arrayptr(void) {
  /* Also undefined.  */
  A(i0 ? *pca : *pcb, 4); /* GCC 8.2 failure */
  A(i0 ? *pcb : *pca, 2);

  A(i0 ? *pva : *pvb, 5); /* GCC 8.2 failure */
  A(i0 ? *pvb : *pva, 3);
}

int main(void) {
  test_binary_cond_expr_global();
  test_binary_cond_expr_local();

  test_ternary_cond_expr();
  test_binary_cond_expr_arrayptr();

  if (nfails)
    __builtin_abort();
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "strlen",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclaration {
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "nfails",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT: decl[2]: Declaration {
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "i0",
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 19,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "ca",
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   49,
// DEFAULT-NEXT:                                                   50,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "12",
// DEFAULT-NEXT:                                               ],
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 21,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "cb",
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
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "2",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "3",
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
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               52,
// DEFAULT-NEXT:                                                           ],
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 22,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
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
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "va",
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       List(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   49,
// DEFAULT-NEXT:                                                   50,
// DEFAULT-NEXT:                                                   51,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "123",
// DEFAULT-NEXT:                                               ],
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 29,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
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
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "vb",
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
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "2",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "3",
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
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               52,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "4",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   CharLiteral(
// DEFAULT-NEXT:                                                       CharLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               53,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           spelling: "5",
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 30,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "s",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       49,
// DEFAULT-NEXT:                                       50,
// DEFAULT-NEXT:                                       51,
// DEFAULT-NEXT:                                       52,
// DEFAULT-NEXT:                                       53,
// DEFAULT-NEXT:                                       54,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "123456",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:           line: 37,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "test_binary_cond_expr_global",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "ca",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 41,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "41",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : ca [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "ca",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                                   51,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "123",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 42,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "42",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? ca [ 0 ] : \"123\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "cb",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 4,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "4",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 49,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "49",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : cb [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "cb",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "12",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 50,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "50",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? cb [ 0 ] : \"12\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "va",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 52,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "52",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : va [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "va",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                                   51,
// DEFAULT-NEXT:                                                                   52,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "1234",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 4,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "4",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 53,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "53",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               52,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? va [ 0 ] : \"1234\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "vb",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 5,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "5",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 55,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "55",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : vb [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 5,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "5",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "vb",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "12",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 56,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "56",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? vb [ 0 ] : \"12\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 39,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "test_binary_cond_expr_local",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "lca",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       size: Expression(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 3,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "3",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
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
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "12",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "lcb",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       size: Expression(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 3,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "3",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: List(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           49,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           50,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "2",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           51,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "3",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: List(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           52,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "4",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "lva",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       size: Expression(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 3,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "3",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
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
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "123",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "lvb",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       size: Expression(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 3,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "3",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: List(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           49,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           50,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "2",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           51,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "3",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: List(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           52,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "4",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       InitializerItem {
// DEFAULT-NEXT:                                                           designators: [],
// DEFAULT-NEXT:                                                           value: Expr(
// DEFAULT-NEXT:                                                               CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           53,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "5",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "lca",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 77,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "77",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : lca [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "lca",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                                   51,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "123",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 78,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "78",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? lca [ 0 ] : \"123\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "lcb",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 4,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "4",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 80,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "80",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : lcb [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "lcb",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "12",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 81,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "81",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? lcb [ 0 ] : \"12\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "lva",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 83,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "83",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : lva [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "lva",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                                   51,
// DEFAULT-NEXT:                                                                   52,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "1234",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 4,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "4",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 84,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "84",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               52,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? lva [ 0 ] : \"1234\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "1",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "lvb",
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
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 5,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "5",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 86,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "86",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? \"1\" : lvb [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 5,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "5",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "lvb",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: StringLiteral(
// DEFAULT-NEXT:                                                           StringLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   49,
// DEFAULT-NEXT:                                                                   50,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               pieces: [
// DEFAULT-NEXT:                                                                   "12",
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 87,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "87",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? lvb [ 0 ] : \"12\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 58,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "test_ternary_cond_expr",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Equal,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "s",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Conditional {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                           then_value: Some(
// DEFAULT-NEXT:                                                               Index {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "vb",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 0,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           else_value: StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                       50,
// DEFAULT-NEXT:                                                                       51,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "123",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 6,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "6",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 92,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "92",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 == 0 ? s : i0 == 1 ? vb [ 0 ] : \"123\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 6,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "6",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Equal,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "vb",
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
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Conditional {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                           then_value: Some(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "s",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           else_value: StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                       50,
// DEFAULT-NEXT:                                                                       51,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "123",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 5,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "5",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 93,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "93",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 == 0 ? vb [ 0 ] : i0 == 1 ? s : \"123\"",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 5,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "5",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Equal,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "i0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           StringLiteral(
// DEFAULT-NEXT:                                                               StringLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       49,
// DEFAULT-NEXT:                                                                       50,
// DEFAULT-NEXT:                                                                       51,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   pieces: [
// DEFAULT-NEXT:                                                                       "123",
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Conditional {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "i0",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
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
// DEFAULT-NEXT:                                                           then_value: Some(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "s",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           else_value: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "vb",
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
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 94,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "94",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               50,
// DEFAULT-NEXT:                                                               51,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               49,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               91,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               93,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 == 0 ? \"123\" : i0 == 1 ? s : vb [ 0 ]",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 89,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Grouped(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pca",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "ca",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:           line: 96,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Grouped(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pcb",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "cb",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:           line: 97,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Declaration {
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
// DEFAULT-NEXT:                       inner: Grouped(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pva",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "va",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:           line: 99,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Declaration {
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
// DEFAULT-NEXT:                       inner: Grouped(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pvb",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "vb",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: IntegerLiteral(
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
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:           line: 100,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "test_binary_cond_expr_arrayptr",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Identifier(
// DEFAULT-NEXT:                                                                   "pca",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "pcb",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 4,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "4",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 105,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "105",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? * pca : * pcb",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Identifier(
// DEFAULT-NEXT:                                                                   "pcb",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "pca",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
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
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 106,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "106",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               99,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? * pcb : * pca",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Identifier(
// DEFAULT-NEXT:                                                                   "pva",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "pvb",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 5,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "5",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 108,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "108",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? * pva : * pvb",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 5,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "5",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               DoWhile {
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "_s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Paren(
// DEFAULT-NEXT:                                                   Conditional {
// DEFAULT-NEXT:                                                       condition: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Identifier(
// DEFAULT-NEXT:                                                                   "pvb",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "pva",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "_n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "strlen",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "_s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "_n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 3,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "3",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_value: Some(
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
// DEFAULT-NEXT:                                   else_value: Paren(
// DEFAULT-NEXT:                                       Comma {
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_printf",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               116,
// DEFAULT-NEXT:                                                               114,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               110,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               40,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               115,
// DEFAULT-NEXT:                                                               34,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               41,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               61,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               37,
// DEFAULT-NEXT:                                                               117,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               102,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               108,
// DEFAULT-NEXT:                                                               101,
// DEFAULT-NEXT:                                                               100,
// DEFAULT-NEXT:                                                               10,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "line %i: strlen ((%s) = (\\\"%s\\\")) == %u failed\\n",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 109,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "109",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   StringLiteral(
// DEFAULT-NEXT:                                                       StringLiteral {
// DEFAULT-NEXT:                                                           encoding: Plain,
// DEFAULT-NEXT:                                                           code_units: [
// DEFAULT-NEXT:                                                               105,
// DEFAULT-NEXT:                                                               48,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               63,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               98,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               58,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               42,
// DEFAULT-NEXT:                                                               32,
// DEFAULT-NEXT:                                                               112,
// DEFAULT-NEXT:                                                               118,
// DEFAULT-NEXT:                                                               97,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           pieces: [
// DEFAULT-NEXT:                                                               "i0 ? * pvb : * pva",
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "_s",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Unary {
// DEFAULT-NEXT:                                               op: PreIncrement,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "nfails",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: IntegerLiteral(
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
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 102,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[16]: Function(
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
// DEFAULT-NEXT:                           "test_binary_cond_expr_global",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_binary_cond_expr_local",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_ternary_cond_expr",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_binary_cond_expr_arrayptr",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Identifier(
// DEFAULT-NEXT:                       "nfails",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_abort",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 111,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
