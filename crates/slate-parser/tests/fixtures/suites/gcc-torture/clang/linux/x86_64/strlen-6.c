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
// DEFAULT-NEXT:     global %[[VALUE_nfails:[0-9]+]] nfails: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i0:[0-9]+]] i0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ca:[0-9]+]] ca: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 0])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cb:[0-9]+]] cb: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_va:[0-9]+]] va: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 51])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vb:[0-9]+]] vb: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(53)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 99, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 99, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 99, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 99, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 118, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([49, 50, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([105, 48, 32, 63, 32, 118, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 52, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 99, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([105, 48, 32, 63, 32, 108, 99, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 99, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 108, 99, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 118, 97, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([49, 50, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([105, 48, 32, 63, 32, 108, 118, 97, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 52, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([105, 48, 32, 63, 32, 34, 49, 34, 32, 58, 32, 108, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([105, 48, 32, 63, 32, 108, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 115, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 118, 98, 91, 48, 93, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 115, 32, 58, 32, 34, 49, 50, 51, 34, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([105, 48, 32, 61, 61, 32, 48, 32, 63, 32, 34, 49, 50, 51, 34, 32, 58, 32, 105, 48, 32, 61, 61, 32, 49, 32, 63, 32, 115, 32, 58, 32, 118, 98, 91, 48, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_pca:[0-9]+]] pca: ptr<const array<i8, 3>> [storage=static] = addr_of<ptr<const array<i8, 3>>>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_ca]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pcb:[0-9]+]] pcb: ptr<const array<i8, 3>> [storage=static] = addr_of<ptr<const array<i8, 3>>>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_cb]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pva:[0-9]+]] pva: ptr<array<i8, 3>> [storage=static] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_va]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pvb:[0-9]+]] pvb: ptr<array<i8, 3>> [storage=static] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 99, 97, 32, 58, 32, 42, 112, 99, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 99, 98, 32, 58, 32, 42, 112, 99, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 118, 97, 32, 58, 32, 42, 112, 118, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 46> [storage=static] = code_units<array<i8, 46>>([108, 105, 110, 101, 32, 37, 105, 58, 32, 115, 116, 114, 108, 101, 110, 32, 40, 40, 37, 115, 41, 32, 61, 32, 40, 34, 37, 115, 34, 41, 41, 32, 61, 61, 32, 37, 117, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([105, 48, 32, 63, 32, 42, 112, 118, 98, 32, 58, 32, 42, 112, 118, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_binary_cond_expr_global:[0-9]+]] @test_binary_cond_expr_global() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_ca]]), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE__n:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s]])));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE3]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_3]])), const<i32>(41), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_4]]), read<ptr<const i8>>(%[[VALUE__s]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE3]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_2:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_ca]]), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_2:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_2]])));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE7]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_6]])), const<i32>(42), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_7]]), read<ptr<const i8>>(%[[VALUE__s_2]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE9]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE7]], read<u32>(%[[VALUE9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_3:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_8]])), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_cb]]), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE__n_3:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_3]])));
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE11]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_9]])), const<i32>(49), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_10]]), read<ptr<const i8>>(%[[VALUE__s_3]]), const<i32>(4));
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE12]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE13]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE11]], read<u32>(%[[VALUE13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_4:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_cb]]), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_11]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_4:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_4]])));
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE15]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_12]])), const<i32>(50), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_13]]), read<ptr<const i8>>(%[[VALUE__s_4]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE17:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE16]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE17]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE15]], read<u32>(%[[VALUE17]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_5:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_14]]), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_va]]), const<i32>(0))))));
// DEFAULT-NEXT:                 let %[[VALUE__n_5:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_5]])));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_5]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE19]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_15]])), const<i32>(52), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_16]]), read<ptr<const i8>>(%[[VALUE__s_5]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE21:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE20]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE19]], read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_6:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_va]]), const<i32>(0)))), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_17]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_6:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_6]])));
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_6]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE23]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_18]])), const<i32>(53), array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_19]]), read<ptr<const i8>>(%[[VALUE__s_6]]), const<i32>(4));
// DEFAULT-NEXT:                     let %[[VALUE24:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE25:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE24]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE25]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE23]], read<u32>(%[[VALUE25]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_7:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_20]]), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0))))));
// DEFAULT-NEXT:                 let %[[VALUE__n_7:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_7]])));
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_7]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE27]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_21]])), const<i32>(55), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_22]]), read<ptr<const i8>>(%[[VALUE__s_7]]), const<i32>(5));
// DEFAULT-NEXT:                     let %[[VALUE28:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE29:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE28]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE29]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE27]], read<u32>(%[[VALUE29]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_8:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0)))), array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_23]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_8:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_8]])));
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE31]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_24]])), const<i32>(56), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_25]]), read<ptr<const i8>>(%[[VALUE__s_8]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE32:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE33:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE32]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE33]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE31]], read<u32>(%[[VALUE33]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_binary_cond_expr_local:[0-9]+]] @test_binary_cond_expr_local() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_lca:[0-9]+]] lca: array<array<i8, 3>, 2> [storage=automatic] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 0]));
// DEFAULT-NEXT:         let %[[VALUE_lcb:[0-9]+]] lcb: array<array<i8, 3>, 2> [storage=automatic] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52))));
// DEFAULT-NEXT:         let %[[VALUE_lva:[0-9]+]] lva: array<array<i8, 3>, 2> [storage=automatic] = aggregate<array<array<i8, 3>, 2>, zero_fill=true>(index0 = code_units<array<i8, 3>>([49, 50, 51]));
// DEFAULT-NEXT:         let %[[VALUE_lvb:[0-9]+]] lvb: array<array<i8, 3>, 2> [storage=automatic] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51))), index1 = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(53))));
// DEFAULT-NEXT:         do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_9:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_26]])), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_lca]]), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE__n_9:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_9]])));
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_9]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE35]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_27]])), const<i32>(77), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_28]]), read<ptr<const i8>>(%[[VALUE__s_9]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE36:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE37:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE36]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE37]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE35]], read<u32>(%[[VALUE37]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_10:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_lca]]), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_29]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_10:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_10]])));
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_10]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE39]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_30]])), const<i32>(78), array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_31]]), read<ptr<const i8>>(%[[VALUE__s_10]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE40:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE41:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE40]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE41]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE39]], read<u32>(%[[VALUE41]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_11:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_32]])), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_lcb]]), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE__n_11:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_11]])));
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_11]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE43]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_33]])), const<i32>(80), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_34]]), read<ptr<const i8>>(%[[VALUE__s_11]]), const<i32>(4));
// DEFAULT-NEXT:                     let %[[VALUE44:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE45:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE44]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE45]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE43]], read<u32>(%[[VALUE45]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_12:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_lcb]]), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_35]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_12:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_12]])));
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_12]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE47]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_36]])), const<i32>(81), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_37]]), read<ptr<const i8>>(%[[VALUE__s_12]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE48:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE49:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE48]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE49]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE47]], read<u32>(%[[VALUE49]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_13:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_38]]), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_lva]]), const<i32>(0))))));
// DEFAULT-NEXT:                 let %[[VALUE__n_13:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_13]])));
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_13]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE51]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_39]])), const<i32>(83), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_40]]), read<ptr<const i8>>(%[[VALUE__s_13]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE52:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE53:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE52]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE53]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE51]], read<u32>(%[[VALUE53]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_14:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_lva]]), const<i32>(0)))), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_41]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_14:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_14]])));
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_14]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE55]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_42]])), const<i32>(84), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_43]]), read<ptr<const i8>>(%[[VALUE__s_14]]), const<i32>(4));
// DEFAULT-NEXT:                     let %[[VALUE56:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE57:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE56]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE57]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE55]], read<u32>(%[[VALUE57]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_15:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_44]]), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_lvb]]), const<i32>(0))))));
// DEFAULT-NEXT:                 let %[[VALUE__n_15:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_15]])));
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_15]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE59]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_45]])), const<i32>(86), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_46]]), read<ptr<const i8>>(%[[VALUE__s_15]]), const<i32>(5));
// DEFAULT-NEXT:                     let %[[VALUE60:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE61:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE60]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE61]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE59]], read<u32>(%[[VALUE61]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_16:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_lvb]]), const<i32>(0)))), array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_47]])));
// DEFAULT-NEXT:                 let %[[VALUE__n_16:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_16]])));
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_16]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE63]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_48]])), const<i32>(87), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_49]]), read<ptr<const i8>>(%[[VALUE__s_16]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE64:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE65:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE64]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE65]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE63]], read<u32>(%[[VALUE65]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ternary_cond_expr:[0-9]+]] @test_ternary_cond_expr() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_17:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), read<ptr<const i8>>(%[[VALUE_s]]), pointer_cast<ptr<const i8>, reason=usual_arith>(conditional<ptr<i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(1)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0)))), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_50]]))));
// DEFAULT-NEXT:                 let %[[VALUE__n_17:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_17]])));
// DEFAULT-NEXT:                 let %[[VALUE67:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_17]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE67]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_51]])), const<i32>(92), array_decay<ptr<i8>, length=Some(38)>(%[[VALUE_str_52]]), read<ptr<const i8>>(%[[VALUE__s_17]]), const<i32>(6));
// DEFAULT-NEXT:                     let %[[VALUE68:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE69:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE68]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE69]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE67]], read<u32>(%[[VALUE69]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_18:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0))))), conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(1)), read<ptr<const i8>>(%[[VALUE_s]]), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_53]]))));
// DEFAULT-NEXT:                 let %[[VALUE__n_18:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_18]])));
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_18]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE71]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_54]])), const<i32>(93), array_decay<ptr<i8>, length=Some(38)>(%[[VALUE_str_55]]), read<ptr<const i8>>(%[[VALUE__s_18]]), const<i32>(5));
// DEFAULT-NEXT:                     let %[[VALUE72:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE73:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE72]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE73]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE71]], read<u32>(%[[VALUE73]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_19:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_56]])), conditional<ptr<const i8>>(eq<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(1)), read<ptr<const i8>>(%[[VALUE_s]]), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(2)>(%[[VALUE_vb]]), const<i32>(0)))))));
// DEFAULT-NEXT:                 let %[[VALUE__n_19:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_19]])));
// DEFAULT-NEXT:                 let %[[VALUE75:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_19]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE75]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_57]])), const<i32>(94), array_decay<ptr<i8>, length=Some(38)>(%[[VALUE_str_58]]), read<ptr<const i8>>(%[[VALUE__s_19]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE76:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE77:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE76]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE77]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE75]], read<u32>(%[[VALUE77]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_binary_cond_expr_arrayptr:[0-9]+]] @test_binary_cond_expr_arrayptr() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_20:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%[[VALUE_pca]]))), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%[[VALUE_pcb]]))));
// DEFAULT-NEXT:                 let %[[VALUE__n_20:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_20]])));
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_20]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE79]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_59]])), const<i32>(105), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_60]]), read<ptr<const i8>>(%[[VALUE__s_20]]), const<i32>(4));
// DEFAULT-NEXT:                     let %[[VALUE80:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE81:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE80]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE81]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE79]], read<u32>(%[[VALUE81]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE82:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_21:[0-9]+]] _s: ptr<const i8> [storage=automatic] = conditional<ptr<const i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%[[VALUE_pcb]]))), array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%[[VALUE_pca]]))));
// DEFAULT-NEXT:                 let %[[VALUE__n_21:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_21]])));
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_21]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE83]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_61]])), const<i32>(106), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_62]]), read<ptr<const i8>>(%[[VALUE__s_21]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE84:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE85:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE84]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE85]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE83]], read<u32>(%[[VALUE85]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE86:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_22:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%[[VALUE_pva]]))), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%[[VALUE_pvb]])))));
// DEFAULT-NEXT:                 let %[[VALUE__n_22:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_22]])));
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_22]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE87]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_63]])), const<i32>(108), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_64]]), read<ptr<const i8>>(%[[VALUE__s_22]]), const<i32>(5));
// DEFAULT-NEXT:                     let %[[VALUE88:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE89:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE88]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE89]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE87]], read<u32>(%[[VALUE89]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE90:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE__s_23:[0-9]+]] _s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i32>(read<i32, volatile>(%[[VALUE_i0]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%[[VALUE_pvb]]))), array_decay<ptr<i8>, length=Some(3)>(deref(read<ptr<array<i8, 3>>>(%[[VALUE_pva]])))));
// DEFAULT-NEXT:                 let %[[VALUE__n_23:[0-9]+]] _n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE__s_23]])));
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u32>(read<u32>(%[[VALUE__n_23]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE91]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(46)>(%[[VALUE_str_65]])), const<i32>(109), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_66]]), read<ptr<const i8>>(%[[VALUE__s_23]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE92:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_nfails]]);
// DEFAULT-NEXT:                     let %[[VALUE93:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE92]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_nfails]], read<u32>(%[[VALUE93]]));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE91]], read<u32>(%[[VALUE93]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_binary_cond_expr_global]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_binary_cond_expr_local]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_ternary_cond_expr]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_binary_cond_expr_arrayptr]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_nfails]]), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
