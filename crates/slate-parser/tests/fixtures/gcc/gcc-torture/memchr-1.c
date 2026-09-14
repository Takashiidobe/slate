/* PR tree-optimization/86711 - wrong folding of memchr

   Verify that memchr() of arrays initialized with string literals
   where the nul doesn't fit in the array doesn't find the nul.  */
typedef __SIZE_TYPE__  size_t;
typedef __WCHAR_TYPE__ wchar_t;

extern void *memchr(const void *, int, size_t);

#define A(expr)                                                                \
  ((expr) ? (void)0                                                            \
          : (__builtin_printf("assertion failed on line %i: %s\n", __LINE__,   \
                              #expr),                                          \
             __builtin_abort()))

static const char c     = '1';
static const char s1[1] = "1";
static const char s4[4] = "1234";

static const char s4_2[2][4] = {"1234", "5678"};
static const char s5_3[3][5] = {"12345", "6789", "01234"};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;
volatile int v3 = 3;
volatile int v4 = 3;

void test_narrow(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;

  A(memchr("" + 1, 0, 0) == 0);

  A(memchr(&c, 0, sizeof c) == 0);
  A(memchr(&c + 1, 0, sizeof c - 1) == 0);
  A(memchr(&c + i1, 0, sizeof c - i1) == 0);
  A(memchr(&c + v1, 0, sizeof c - v1) == 0);

  A(memchr(s1, 0, sizeof s1) == 0);
  A(memchr(s1 + 1, 0, sizeof s1 - 1) == 0);
  A(memchr(s1 + i1, 0, sizeof s1 - i1) == 0);
  A(memchr(s1 + v1, 0, sizeof s1 - v1) == 0);

  A(memchr(&s1, 0, sizeof s1) == 0);
  A(memchr(&s1 + 1, 0, sizeof s1 - 1) == 0);
  A(memchr(&s1 + i1, 0, sizeof s1 - i1) == 0);
  A(memchr(&s1 + v1, 0, sizeof s1 - v1) == 0);

  A(memchr(&s1[0], 0, sizeof s1) == 0);
  A(memchr(&s1[0] + 1, 0, sizeof s1 - 1) == 0);
  A(memchr(&s1[0] + i1, 0, sizeof s1 - i1) == 0);
  A(memchr(&s1[0] + v1, 0, sizeof s1 - v1) == 0);

  A(memchr(&s1[i0], 0, sizeof s1) == 0);
  A(memchr(&s1[i0] + 1, 0, sizeof s1 - 1) == 0);
  A(memchr(&s1[i0] + i1, 0, sizeof s1 - i1) == 0);
  A(memchr(&s1[i0] + v1, 0, sizeof s1 - v1) == 0);

  A(memchr(&s1[v0], 0, sizeof s1) == 0);
  A(memchr(&s1[v0] + 1, 0, sizeof s1 - 1) == 0);
  A(memchr(&s1[v0] + i1, 0, sizeof s1 - i1) == 0);
  A(memchr(&s1[v0] + v1, 0, sizeof s1 - v1) == 0);

  A(memchr(s4 + i0, 0, sizeof s4 - i0) == 0);
  A(memchr(s4 + i1, 0, sizeof s4 - i1) == 0);
  A(memchr(s4 + i2, 0, sizeof s4 - i2) == 0);
  A(memchr(s4 + i3, 0, sizeof s4 - i3) == 0);
  A(memchr(s4 + i4, 0, sizeof s4 - i4) == 0);

  A(memchr(s4 + v0, 0, sizeof s4 - v0) == 0);
  A(memchr(s4 + v1, 0, sizeof s4 - v1) == 0);
  A(memchr(s4 + v2, 0, sizeof s4 - v2) == 0);
  A(memchr(s4 + v3, 0, sizeof s4 - v3) == 0);
  A(memchr(s4 + v4, 0, sizeof s4 - v4) == 0);

  A(memchr(s4_2, 0, sizeof s4_2) == 0);

  A(memchr(s4_2[0], 0, sizeof s4_2[0]) == 0);
  A(memchr(s4_2[1], 0, sizeof s4_2[1]) == 0);

  A(memchr(s4_2[0] + 1, 0, sizeof s4_2[0] - 1) == 0);
  A(memchr(s4_2[1] + 2, 0, sizeof s4_2[1] - 2) == 0);
  A(memchr(s4_2[1] + 3, 0, sizeof s4_2[1] - 3) == 0);

  A(memchr(s4_2[v0], 0, sizeof s4_2[v0]) == 0);
  A(memchr(s4_2[v0] + 1, 0, sizeof s4_2[v0] - 1) == 0);

  /* The following calls must find the nul.  */
  A(memchr("", 0, 1) != 0);
  A(memchr(s5_3, 0, sizeof s5_3) == &s5_3[1][4]);

  A(memchr(&s5_3[0][0] + i0, 0, sizeof s5_3 - i0) == &s5_3[1][4]);
  A(memchr(&s5_3[0][0] + i1, 0, sizeof s5_3 - i1) == &s5_3[1][4]);
  A(memchr(&s5_3[0][0] + i2, 0, sizeof s5_3 - i2) == &s5_3[1][4]);
  A(memchr(&s5_3[0][0] + i4, 0, sizeof s5_3 - i4) == &s5_3[1][4]);

  A(memchr(&s5_3[1][i0], 0, sizeof s5_3[1] - i0) == &s5_3[1][4]);
}

#if 4 == __WCHAR_WIDTH__

static const wchar_t wc    = L'1';
static const wchar_t ws1[] = L"1";
static const wchar_t ws4[] = L"\x00123456\x12005678\x12340078\x12345600";

void test_wide(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;
  int i3 = i2 + 1;
  int i4 = i3 + 1;

  A(memchr(L"" + 1, 0, 0) == 0);
  A(memchr(&wc + 1, 0, 0) == 0);
  A(memchr(L"\x12345678", 0, sizeof(wchar_t)) == 0);

  const size_t nb  = sizeof ws4;
  const size_t nwb = sizeof(wchar_t);

  const char *pws1 = (const char *)ws1;
  const char *pws4 = (const char *)ws4;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  A(memchr(ws1, 0, sizeof ws1) == pws1 + 1);

  A(memchr(&ws4[0], 0, nb) == pws4 + 3);
  A(memchr(&ws4[1], 0, nb - 1 * nwb) == pws4 + 1 * nwb + 2);
  A(memchr(&ws4[2], 0, nb - 2 * nwb) == pws4 + 2 * nwb + 1);
  A(memchr(&ws4[3], 0, nb - 3 * nwb) == pws4 + 3 * nwb + 0);
#else
  A(memchr(ws1, 0, sizeof ws1) == pws1 + 0);

  A(memchr(&ws4[0], 0, nb) == pws4 + 0);
  A(memchr(&ws4[1], 0, nb - 1 * nwb) == pws4 + 1 * nwb + 1);
  A(memchr(&ws4[2], 0, nb - 2 * nwb) == pws4 + 2 * nwb + 2);
  A(memchr(&ws4[3], 0, nb - 3 * nwb) == pws4 + 3 * nwb + 3);
#endif
}

#elif 2 == __WCHAR_WIDTH__

static const wchar_t wc     = L'1';
static const wchar_t ws1[]  = L"1";
static const wchar_t ws2[2] = L"\x1234\x5678"; /* no terminating nul */
static const wchar_t ws4[]  = L"\x0012\x1200\x1234";

void test_wide(void) {
  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;

  A(sizeof(wchar_t) == 2);

  A(memchr(L"" + 1, 0, 0) == 0);
  A(memchr(&wc + 1, 0, 0) == 0);
  A(memchr(L"\x1234", 0, sizeof(wchar_t)) == 0);

  A(memchr(L"" + i1, i0, i0) == 0);
  A(memchr(&wc + i1, i0, i0) == 0);
  A(memchr(L"\x1234", i0, sizeof(wchar_t)) == 0);

  A(memchr(ws2, 0, sizeof ws2) == 0);
  A(memchr(ws2, i0, sizeof ws2) == 0);

  const size_t nb  = sizeof ws4;
  const size_t nwb = sizeof(wchar_t);

  const char *pws1 = (const char *)ws1;
  const char *pws4 = (const char *)ws4;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  A(memchr(ws1, i0, sizeof ws1) == pws1 + 1);

  A(memchr(&ws4[0], i0, nb) == pws4 + i1);
  A(memchr(&ws4[1], i0, nb - i1 * nwb) == pws4 + i1 * nwb);
  A(memchr(&ws4[2], i0, nb - i2 * nwb) == pws4 + i2 * nwb + i2);
#else
  A(memchr(ws1, i0, sizeof ws1) == pws1 + 0);

  A(memchr(&ws4[0], i0, nb) == pws4 + 0);
  A(memchr(&ws4[1], i0, nb - i1 * nwb) == pws4 + i1 * nwb + i1);
  A(memchr(&ws4[2], i0, nb - i2 * nwb) == pws4 + i2 * nwb + i2);
#endif
}

#else

void test_wide(void) {}

#endif

int main() {
  test_narrow();
  test_wide();
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
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "size_t",
// DEFAULT-NEXT:                   ),
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
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "wchar_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "memchr",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "c",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           CharLiteral(
// DEFAULT-NEXT:                               CharLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       49,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   spelling: "1",
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
// DEFAULT-NEXT:           line: 15,
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
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "s1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
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
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       49,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "1",
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
// DEFAULT-NEXT:           line: 16,
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "s4",
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
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       49,
// DEFAULT-NEXT:                                       50,
// DEFAULT-NEXT:                                       51,
// DEFAULT-NEXT:                                       52,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "1234",
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
// DEFAULT-NEXT:           line: 17,
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s4_2",
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
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   49,
// DEFAULT-NEXT:                                                   50,
// DEFAULT-NEXT:                                                   51,
// DEFAULT-NEXT:                                                   52,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "1234",
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   53,
// DEFAULT-NEXT:                                                   54,
// DEFAULT-NEXT:                                                   55,
// DEFAULT-NEXT:                                                   56,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "5678",
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
// DEFAULT-NEXT:           line: 19,
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
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s5_3",
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                                                   52,
// DEFAULT-NEXT:                                                   53,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "12345",
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   54,
// DEFAULT-NEXT:                                                   55,
// DEFAULT-NEXT:                                                   56,
// DEFAULT-NEXT:                                                   57,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "6789",
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               InitializerItem {
// DEFAULT-NEXT:                                   designators: [],
// DEFAULT-NEXT:                                   value: Expr(
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   48,
// DEFAULT-NEXT:                                                   49,
// DEFAULT-NEXT:                                                   50,
// DEFAULT-NEXT:                                                   51,
// DEFAULT-NEXT:                                                   52,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "01234",
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
// DEFAULT-NEXT:           line: 20,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 22,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 23,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 25,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "v4",
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
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 26,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "test_narrow",
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
// DEFAULT-NEXT:                           InitDeclarator {
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
// DEFAULT-NEXT:                           InitDeclarator {
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
// DEFAULT-NEXT:                           InitDeclarator {
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
// DEFAULT-NEXT:                           InitDeclarator {
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
// DEFAULT-NEXT:                           InitDeclarator {
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Paren(
// DEFAULT-NEXT:                       Conditional {
// DEFAULT-NEXT:                           condition: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Equal,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: StringLiteral(
// DEFAULT-NEXT:                                                   StringLiteral {
// DEFAULT-NEXT:                                                       encoding: Plain,
// DEFAULT-NEXT:                                                       code_units: [],
// DEFAULT-NEXT:                                                       pieces: [
// DEFAULT-NEXT:                                                           "",
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 36,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "36",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       34,
// DEFAULT-NEXT:                                                       34,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( \"\" + 1 , 0 , 0 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "c",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & c , 0 , sizeof c ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 39,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "39",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & c + 1 , 0 , sizeof c - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & c + i1 , 0 , sizeof c - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "c",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & c + v1 , 0 , sizeof c - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "s1",
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s1 , 0 , sizeof s1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s1 + 1 , 0 , sizeof s1 - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 45,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "45",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s1 + i1 , 0 , sizeof s1 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s1 + v1 , 0 , sizeof s1 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 , 0 , sizeof s1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 + 1 , 0 , sizeof s1 - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 + i1 , 0 , sizeof s1 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 51,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "51",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 + v1 , 0 , sizeof s1 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s1",
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
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ 0 ] , 0 , sizeof s1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ 0 ] + 1 , 0 , sizeof s1 - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ 0 ] + i1 , 0 , sizeof s1 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 56,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "56",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ 0 ] + v1 , 0 , sizeof s1 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ i0 ] , 0 , sizeof s1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 59,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "59",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ i0 ] + 1 , 0 , sizeof s1 - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ i0 ] + i1 , 0 , sizeof s1 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "i0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ i0 ] + v1 , 0 , sizeof s1 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ v0 ] , 0 , sizeof s1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "v0",
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ v0 ] + 1 , 0 , sizeof s1 - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "v0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ v0 ] + i1 , 0 , sizeof s1 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "v0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s1 [ v0 ] + v1 , 0 , sizeof s1 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + i0 , 0 , sizeof s4 - i0 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 69,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "69",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + i1 , 0 , sizeof s4 - i1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + i2 , 0 , sizeof s4 - i2 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + i3 , 0 , sizeof s4 - i3 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i4",
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + i4 , 0 , sizeof s4 - i4 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v0",
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + v0 , 0 , sizeof s4 - v0 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + v1 , 0 , sizeof s4 - v1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + v2 , 0 , sizeof s4 - v2 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + v3 , 0 , sizeof s4 - v3 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "s4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "v4",
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4 + v4 , 0 , sizeof s4 - v4 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "s4_2",
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s4_2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 , 0 , sizeof s4_2 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s4_2",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
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
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ 0 ] , 0 , sizeof s4_2 [ 0 ] ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s4_2",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
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
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 83,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "83",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ 1 ] , 0 , sizeof s4_2 [ 1 ] ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s4_2",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ 0 ] + 1 , 0 , sizeof s4_2 [ 0 ] - 1 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s4_2",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ 1 ] + 2 , 0 , sizeof s4_2 [ 1 ] - 2 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s4_2",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ 1 ] + 3 , 0 , sizeof s4_2 [ 1 ] - 3 ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s4_2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "v0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ v0 ] , 0 , sizeof s4_2 [ v0 ] ) == 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "s4_2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "v0",
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s4_2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "v0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       118,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s4_2 [ v0 ] + 1 , 0 , sizeof s4_2 [ v0 ] - 1 ) == 0",
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
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                           IntegerLiteral(
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
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       34,
// DEFAULT-NEXT:                                                       34,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       33,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( \"\" , 0 , 1 ) != 0",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "s5_3",
// DEFAULT-NEXT:                                           ),
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
// DEFAULT-NEXT:                                           SizeOfExpr(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( s5_3 , 0 , sizeof s5_3 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "s5_3",
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s5_3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s5_3 [ 0 ] [ 0 ] + i0 , 0 , sizeof s5_3 - i0 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "s5_3",
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s5_3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s5_3 [ 0 ] [ 0 ] + i1 , 0 , sizeof s5_3 - i1 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "s5_3",
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s5_3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 98,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "98",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       50,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s5_3 [ 0 ] [ 0 ] + i2 , 0 , sizeof s5_3 - i2 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Unary {
// DEFAULT-NEXT:                                                   op: AddrOf,
// DEFAULT-NEXT:                                                   operand: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "s5_3",
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
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "s5_3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
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
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       43,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s5_3 [ 0 ] [ 0 ] + i4 , 0 , sizeof s5_3 - i4 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:                                           "memchr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Unary {
// DEFAULT-NEXT:                                               op: AddrOf,
// DEFAULT-NEXT:                                               operand: Index {
// DEFAULT-NEXT:                                                   base: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s5_3",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "i0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Sub,
// DEFAULT-NEXT:                                               left: SizeOfExpr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "s5_3",
// DEFAULT-NEXT:                                                       ),
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
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "i0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Index {
// DEFAULT-NEXT:                                           base: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "s5_3",
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                           index: IntegerLiteral(
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
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_value: Some(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Abstract,
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
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       97,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       108,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       100,
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
// DEFAULT-NEXT:                                                       "assertion failed on line %i: %s\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 101,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "101",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       109,
// DEFAULT-NEXT:                                                       99,
// DEFAULT-NEXT:                                                       104,
// DEFAULT-NEXT:                                                       114,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       40,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       44,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       122,
// DEFAULT-NEXT:                                                       101,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       102,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       48,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       41,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       61,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       38,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       115,
// DEFAULT-NEXT:                                                       53,
// DEFAULT-NEXT:                                                       95,
// DEFAULT-NEXT:                                                       51,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       49,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       91,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       52,
// DEFAULT-NEXT:                                                       32,
// DEFAULT-NEXT:                                                       93,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "memchr ( & s5_3 [ 1 ] [ i0 ] , 0 , sizeof s5_3 [ 1 ] - i0 ) == & s5_3 [ 1 ] [ 4 ]",
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
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 28,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[14]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "test_wide",
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 191,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[15]: Function(
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
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_narrow",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "test_wide",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 195,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
