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
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %1 nfails: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 i0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %3 ca: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 0])) [linkage=external];
// DEFAULT-NEXT:     global %4 cb: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)))) [linkage=external];
// DEFAULT-NEXT:     global %5 va: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 51])) [linkage=external];
// DEFAULT-NEXT:     global %6 vb: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(53)))) [linkage=external];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 s: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%68)) [linkage=external];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 99, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 99, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 99, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 99, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 118, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([49, 50, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([105, 48, 32, 63, 32, 118, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 52, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 99, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([105, 48, 32, 63, 32, 108, 99, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 99, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 108, 99, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 118, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([49, 50, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([105, 48, 32, 63, 32, 108, 118, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 52, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 108, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 115, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 115, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 34, 49, 50, 51, 34, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 115, 32, 58, 32, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 pca: ptr<const array<i8, 3>> [storage=static] = addr_of<ptr<const array<i8, 3>>>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%3), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %54 pcb: ptr<const array<i8, 3>> [storage=static] = addr_of<ptr<const array<i8, 3>>>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%4), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %55 pva: ptr<array<i8, 3>> [storage=static] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%5), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %56 pvb: ptr<array<i8, 3>> [storage=static] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %147 .str147: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 99, 97, 32, 58, 32, 42, 112, 99, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %150 .str150: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 99, 98, 32, 58, 32, 42, 112, 99, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %152 .str152: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 118, 97, 32, 58, 32, 42, 112, 118, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %156 .str156: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 118, 98, 32, 58, 32, 42, 112, 118, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strlen(%67 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @test_binary_cond_expr_global() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %69
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%70)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%3), const<i32>(0)))));
// DEFAULT-NEXT:                 let %10 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%9)));
// DEFAULT-NEXT:                 let %157: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%157, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%71)), const<i32>(41), array_decay<ptr<i8>, length=Some(17)>(%72), read<ptr<const i8>>(%9), const<i32>(2));
// DEFAULT-NEXT:                     let %158: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %159: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%158), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%159));
// DEFAULT-NEXT:                     write<u32>(%157, read<u32>(%159));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %73
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%3), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%74)));
// DEFAULT-NEXT:                 let %12 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%11)));
// DEFAULT-NEXT:                 let %160: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%160, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%75)), const<i32>(42), array_decay<ptr<i8>, length=Some(19)>(%76), read<ptr<const i8>>(%11), const<i32>(3));
// DEFAULT-NEXT:                     let %161: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %162: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%161), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%162));
// DEFAULT-NEXT:                     write<u32>(%160, read<u32>(%162));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %77
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%78)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%4), const<i32>(0)))));
// DEFAULT-NEXT:                 let %14 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%13)));
// DEFAULT-NEXT:                 let %163: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%163, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%79)), const<i32>(49), array_decay<ptr<i8>, length=Some(17)>(%80), read<ptr<const i8>>(%13), const<i32>(4));
// DEFAULT-NEXT:                     let %164: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %165: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%164), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%165));
// DEFAULT-NEXT:                     write<u32>(%163, read<u32>(%165));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %81
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%4), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(%82)));
// DEFAULT-NEXT:                 let %16 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%15)));
// DEFAULT-NEXT:                 let %166: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%166, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%83)), const<i32>(50), array_decay<ptr<i8>, length=Some(18)>(%84), read<ptr<const i8>>(%15), const<i32>(2));
// DEFAULT-NEXT:                     let %167: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %168: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%167), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%168));
// DEFAULT-NEXT:                     write<u32>(%166, read<u32>(%168));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %85
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%86), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%5), const<i32>(0))))));
// DEFAULT-NEXT:                 let %18 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%17)));
// DEFAULT-NEXT:                 let %169: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%18), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%169, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%87)), const<i32>(52), array_decay<ptr<i8>, length=Some(17)>(%88), read<ptr<const i8>>(%17), const<i32>(3));
// DEFAULT-NEXT:                     let %170: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %171: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%170), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%171));
// DEFAULT-NEXT:                     write<u32>(%169, read<u32>(%171));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %89
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%5), const<i32>(0)))), array_decay<ptr<i8>, length=Some(5)>(%90)));
// DEFAULT-NEXT:                 let %20 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%19)));
// DEFAULT-NEXT:                 let %172: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%20), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%172, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%91)), const<i32>(53), array_decay<ptr<i8>, length=Some(20)>(%92), read<ptr<const i8>>(%19), const<i32>(4));
// DEFAULT-NEXT:                     let %173: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %174: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%173), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%174));
// DEFAULT-NEXT:                     write<u32>(%172, read<u32>(%174));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %93
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%94), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0))))));
// DEFAULT-NEXT:                 let %22 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%21)));
// DEFAULT-NEXT:                 let %175: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%22), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%175, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%95)), const<i32>(55), array_decay<ptr<i8>, length=Some(17)>(%96), read<ptr<const i8>>(%21), const<i32>(5));
// DEFAULT-NEXT:                     let %176: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %177: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%176), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%177));
// DEFAULT-NEXT:                     write<u32>(%175, read<u32>(%177));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %97
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0)))), array_decay<ptr<i8>, length=Some(3)>(%98)));
// DEFAULT-NEXT:                 let %24 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%23)));
// DEFAULT-NEXT:                 let %178: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%24), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%178, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%99)), const<i32>(56), array_decay<ptr<i8>, length=Some(18)>(%100), read<ptr<const i8>>(%23), const<i32>(2));
// DEFAULT-NEXT:                     let %179: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %180: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%179), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%180));
// DEFAULT-NEXT:                     write<u32>(%178, read<u32>(%180));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test_binary_cond_expr_local() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26 lca: array<array<i8, 3>, 2> [storage=automatic] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 0]));
// DEFAULT-NEXT:         let %27 lcb: array<array<i8, 3>, 2> [storage=automatic] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52))));
// DEFAULT-NEXT:         let %28 lva: array<array<i8, 3>, 2> [storage=automatic] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 51]));
// DEFAULT-NEXT:         let %29 lvb: array<array<i8, 3>, 2> [storage=automatic] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(53))));
// DEFAULT-NEXT:         do %101
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %30 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%102)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%26), const<i32>(0)))));
// DEFAULT-NEXT:                 let %31 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%30)));
// DEFAULT-NEXT:                 let %181: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%31), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%181, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%103)), const<i32>(77), array_decay<ptr<i8>, length=Some(18)>(%104), read<ptr<const i8>>(%30), const<i32>(2));
// DEFAULT-NEXT:                     let %182: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %183: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%182), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%183));
// DEFAULT-NEXT:                     write<u32>(%181, read<u32>(%183));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %105
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%26), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%106)));
// DEFAULT-NEXT:                 let %33 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%32)));
// DEFAULT-NEXT:                 let %184: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%33), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%184, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%107)), const<i32>(78), array_decay<ptr<i8>, length=Some(20)>(%108), read<ptr<const i8>>(%32), const<i32>(3));
// DEFAULT-NEXT:                     let %185: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %186: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%185), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%186));
// DEFAULT-NEXT:                     write<u32>(%184, read<u32>(%186));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %109
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %34 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%110)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%27), const<i32>(0)))));
// DEFAULT-NEXT:                 let %35 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%34)));
// DEFAULT-NEXT:                 let %187: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%35), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%187, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%111)), const<i32>(80), array_decay<ptr<i8>, length=Some(18)>(%112), read<ptr<const i8>>(%34), const<i32>(4));
// DEFAULT-NEXT:                     let %188: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %189: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%188), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%189));
// DEFAULT-NEXT:                     write<u32>(%187, read<u32>(%189));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %113
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %36 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%27), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(%114)));
// DEFAULT-NEXT:                 let %37 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%36)));
// DEFAULT-NEXT:                 let %190: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%37), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%190, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%115)), const<i32>(81), array_decay<ptr<i8>, length=Some(19)>(%116), read<ptr<const i8>>(%36), const<i32>(2));
// DEFAULT-NEXT:                     let %191: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %192: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%191), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%192));
// DEFAULT-NEXT:                     write<u32>(%190, read<u32>(%192));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %117
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %38 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%118), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%28), const<i32>(0))))));
// DEFAULT-NEXT:                 let %39 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%38)));
// DEFAULT-NEXT:                 let %193: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%39), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%193, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%119)), const<i32>(83), array_decay<ptr<i8>, length=Some(18)>(%120), read<ptr<const i8>>(%38), const<i32>(3));
// DEFAULT-NEXT:                     let %194: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %195: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%194), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%195));
// DEFAULT-NEXT:                     write<u32>(%193, read<u32>(%195));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %121
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %40 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%28), const<i32>(0)))), array_decay<ptr<i8>, length=Some(5)>(%122)));
// DEFAULT-NEXT:                 let %41 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%40)));
// DEFAULT-NEXT:                 let %196: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%41), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%196, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%123)), const<i32>(84), array_decay<ptr<i8>, length=Some(21)>(%124), read<ptr<const i8>>(%40), const<i32>(4));
// DEFAULT-NEXT:                     let %197: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %198: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%197), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%198));
// DEFAULT-NEXT:                     write<u32>(%196, read<u32>(%198));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %125
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %42 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%126), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%29), const<i32>(0))))));
// DEFAULT-NEXT:                 let %43 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%42)));
// DEFAULT-NEXT:                 let %199: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%43), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%199, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%127)), const<i32>(86), array_decay<ptr<i8>, length=Some(18)>(%128), read<ptr<const i8>>(%42), const<i32>(5));
// DEFAULT-NEXT:                     let %200: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %201: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%200), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%201));
// DEFAULT-NEXT:                     write<u32>(%199, read<u32>(%201));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %129
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %44 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%29), const<i32>(0)))), array_decay<ptr<i8>, length=Some(3)>(%130)));
// DEFAULT-NEXT:                 let %45 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%44)));
// DEFAULT-NEXT:                 let %202: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%45), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%202, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%131)), const<i32>(87), array_decay<ptr<i8>, length=Some(19)>(%132), read<ptr<const i8>>(%44), const<i32>(2));
// DEFAULT-NEXT:                     let %203: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %204: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%203), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%204));
// DEFAULT-NEXT:                     write<u32>(%202, read<u32>(%204));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @test_ternary_cond_expr() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %133
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %47 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(0)), read<ptr<const i8>>(%7), pointer_cast<ptr<const i8>, reason=usual_arith>(conditional<ptr<i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(1)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0)))), array_decay<ptr<i8>, length=Some(4)>(%134))));
// DEFAULT-NEXT:                 let %48 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%47)));
// DEFAULT-NEXT:                 let %205: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%48), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))
// DEFAULT-NEXT:                     write<u32>(%205, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%135)), const<i32>(92), array_decay<ptr<i8>, length=Some(38)>(%136), read<ptr<const i8>>(%47), const<i32>(6));
// DEFAULT-NEXT:                     let %206: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %207: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%206), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%207));
// DEFAULT-NEXT:                     write<u32>(%205, read<u32>(%207));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %137
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %49 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0))))), conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(1)), read<ptr<const i8>>(%7), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%138))));
// DEFAULT-NEXT:                 let %50 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%49)));
// DEFAULT-NEXT:                 let %208: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%50), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%208, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%139)), const<i32>(93), array_decay<ptr<i8>, length=Some(38)>(%140), read<ptr<const i8>>(%49), const<i32>(5));
// DEFAULT-NEXT:                     let %209: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %210: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%209), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%210));
// DEFAULT-NEXT:                     write<u32>(%208, read<u32>(%210));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %141
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %51 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%142)), conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%2), const<i32>(1)), read<ptr<const i8>>(%7), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%6), const<i32>(0)))))));
// DEFAULT-NEXT:                 let %52 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%51)));
// DEFAULT-NEXT:                 let %211: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%52), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%211, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%143)), const<i32>(94), array_decay<ptr<i8>, length=Some(38)>(%144), read<ptr<const i8>>(%51), const<i32>(3));
// DEFAULT-NEXT:                     let %212: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %213: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%212), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%213));
// DEFAULT-NEXT:                     write<u32>(%211, read<u32>(%213));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @test_binary_cond_expr_arrayptr() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %145
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %58 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%53))), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%54))));
// DEFAULT-NEXT:                 let %59 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%58)));
// DEFAULT-NEXT:                 let %214: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%59), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%214, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%146)), const<i32>(105), array_decay<ptr<i8>, length=Some(17)>(%147), read<ptr<const i8>>(%58), const<i32>(4));
// DEFAULT-NEXT:                     let %215: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %216: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%215), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%216));
// DEFAULT-NEXT:                     write<u32>(%214, read<u32>(%216));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %148
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %60 _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%54))), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%53))));
// DEFAULT-NEXT:                 let %61 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%60)));
// DEFAULT-NEXT:                 let %217: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%61), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%217, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%149)), const<i32>(106), array_decay<ptr<i8>, length=Some(17)>(%150), read<ptr<const i8>>(%60), const<i32>(2));
// DEFAULT-NEXT:                     let %218: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %219: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%218), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%219));
// DEFAULT-NEXT:                     write<u32>(%217, read<u32>(%219));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %62 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%55))), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%56)))));
// DEFAULT-NEXT:                 let %63 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%62)));
// DEFAULT-NEXT:                 let %220: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%63), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%220, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%152)), const<i32>(108), array_decay<ptr<i8>, length=Some(17)>(%153), read<ptr<const i8>>(%62), const<i32>(5));
// DEFAULT-NEXT:                     let %221: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %222: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%221), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%222));
// DEFAULT-NEXT:                     write<u32>(%220, read<u32>(%222));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %154
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %64 _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%2), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%56))), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%55)))));
// DEFAULT-NEXT:                 let %65 _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, read<ptr<const i8>>(%64)));
// DEFAULT-NEXT:                 let %223: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%65), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%223, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%155)), const<i32>(109), array_decay<ptr<i8>, length=Some(17)>(%156), read<ptr<const i8>>(%64), const<i32>(3));
// DEFAULT-NEXT:                     let %224: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                     let %225: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%224), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%1, read<u32>(%225));
// DEFAULT-NEXT:                     write<u32>(%223, read<u32>(%225));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%46);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%57);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%1), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
