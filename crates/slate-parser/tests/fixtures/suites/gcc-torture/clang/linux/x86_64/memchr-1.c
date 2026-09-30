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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(49)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: array<i8, 1> [storage=static] [const] = code_units<array<i8, 1>>([49]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s4:[0-9]+]] s4: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([49, 50, 51, 52]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s4_2:[0-9]+]] s4_2: array<array<i8, 4>, 2> [storage=static] [const] = aggregate<array<array<i8, 4>, 2>, zero_fill=false>(index0 = code_units<array<i8, 4>>([49, 50, 51, 52]), index1 = code_units<array<i8, 4>>([53, 54, 55, 56])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s5_3:[0-9]+]] s5_3: array<array<i8, 5>, 3> [storage=static] [const] = aggregate<array<array<i8, 5>, 3>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 50, 51, 52, 53]), index1 = code_units<array<i8, 5>>([54, 55, 56, 57, 0]), index2 = code_units<array<i8, 5>>([48, 49, 50, 51, 52])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v0:[0-9]+]] v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v4:[0-9]+]] v4: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([109, 101, 109, 99, 104, 114, 40, 34, 34, 32, 43, 32, 49, 44, 32, 48, 44, 32, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 43> [storage=static] = code_units<array<i8, 43>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 43> [storage=static] = code_units<array<i8, 43>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 52, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_67:[0-9]+]] .str[[VALUE_str_67]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_68:[0-9]+]] .str[[VALUE_str_68]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_69:[0-9]+]] .str[[VALUE_str_69]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_70:[0-9]+]] .str[[VALUE_str_70]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_71:[0-9]+]] .str[[VALUE_str_71]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 52, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_72:[0-9]+]] .str[[VALUE_str_72]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_73:[0-9]+]] .str[[VALUE_str_73]]: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_74:[0-9]+]] .str[[VALUE_str_74]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_75:[0-9]+]] .str[[VALUE_str_75]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 48, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_76:[0-9]+]] .str[[VALUE_str_76]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_77:[0-9]+]] .str[[VALUE_str_77]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_78:[0-9]+]] .str[[VALUE_str_78]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_79:[0-9]+]] .str[[VALUE_str_79]]: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 48, 93, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_80:[0-9]+]] .str[[VALUE_str_80]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_81:[0-9]+]] .str[[VALUE_str_81]]: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 32, 43, 32, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 32, 45, 32, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_82:[0-9]+]] .str[[VALUE_str_82]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_83:[0-9]+]] .str[[VALUE_str_83]]: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 32, 43, 32, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 32, 45, 32, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_84:[0-9]+]] .str[[VALUE_str_84]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_85:[0-9]+]] .str[[VALUE_str_85]]: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 118, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 118, 48, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_86:[0-9]+]] .str[[VALUE_str_86]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_87:[0-9]+]] .str[[VALUE_str_87]]: array<i8, 50> [storage=static] = code_units<array<i8, 50>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 118, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 118, 48, 93, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_88:[0-9]+]] .str[[VALUE_str_88]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_89:[0-9]+]] .str[[VALUE_str_89]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_90:[0-9]+]] .str[[VALUE_str_90]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([109, 101, 109, 99, 104, 114, 40, 34, 34, 44, 32, 48, 44, 32, 49, 41, 32, 33, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_91:[0-9]+]] .str[[VALUE_str_91]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_92:[0-9]+]] .str[[VALUE_str_92]]: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 115, 53, 95, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_93:[0-9]+]] .str[[VALUE_str_93]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_94:[0-9]+]] .str[[VALUE_str_94]]: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_95:[0-9]+]] .str[[VALUE_str_95]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_96:[0-9]+]] .str[[VALUE_str_96]]: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_97:[0-9]+]] .str[[VALUE_str_97]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_98:[0-9]+]] .str[[VALUE_str_98]]: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 50, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_99:[0-9]+]] .str[[VALUE_str_99]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_100:[0-9]+]] .str[[VALUE_str_100]]: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 52, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_101:[0-9]+]] .str[[VALUE_str_101]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_102:[0-9]+]] .str[[VALUE_str_102]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 49, 93, 91, 105, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 91, 49, 93, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_narrow:[0-9]+]] @test_narrow() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i0:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i3:[0-9]+]] i3: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i2]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i4:[0-9]+]] i4: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i3]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_2]])), const<i32>(36), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_c]])), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_4]])), const<i32>(38), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_5]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%[[VALUE_c]]), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_6]])), const<i32>(39), array_decay<ptr<i8>, length=Some(37)>(%[[VALUE_str_7]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%[[VALUE_c]]), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_8]])), const<i32>(40), array_decay<ptr<i8>, length=Some(39)>(%[[VALUE_str_9]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%[[VALUE_c]]), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_10]])), const<i32>(41), array_decay<ptr<i8>, length=Some(39)>(%[[VALUE_str_11]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]])), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_12]])), const<i32>(43), array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str_13]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_14]])), const<i32>(44), array_decay<ptr<i8>, length=Some(38)>(%[[VALUE_str_15]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_16]])), const<i32>(45), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_17]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_18]])), const<i32>(46), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_19]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const array<i8, 1>>>(%[[VALUE_s1]])), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_20]])), const<i32>(48), array_decay<ptr<i8>, length=Some(31)>(%[[VALUE_str_21]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%[[VALUE_s1]]), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_22]])), const<i32>(49), array_decay<ptr<i8>, length=Some(39)>(%[[VALUE_str_23]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_24]])), const<i32>(50), array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_25]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_26]])), const<i32>(51), array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_27]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), const<i32>(0))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_28]])), const<i32>(53), array_decay<ptr<i8>, length=Some(34)>(%[[VALUE_str_29]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), const<i32>(0)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_30]])), const<i32>(54), array_decay<ptr<i8>, length=Some(42)>(%[[VALUE_str_31]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), const<i32>(0)))), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_32]])), const<i32>(55), array_decay<ptr<i8>, length=Some(44)>(%[[VALUE_str_33]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_34]])), const<i32>(56), array_decay<ptr<i8>, length=Some(44)>(%[[VALUE_str_35]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i0]]))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_36]])), const<i32>(58), array_decay<ptr<i8>, length=Some(35)>(%[[VALUE_str_37]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i0]])))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_38]])), const<i32>(59), array_decay<ptr<i8>, length=Some(43)>(%[[VALUE_str_39]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i0]])))), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_40]])), const<i32>(60), array_decay<ptr<i8>, length=Some(45)>(%[[VALUE_str_41]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_42]])), const<i32>(61), array_decay<ptr<i8>, length=Some(45)>(%[[VALUE_str_43]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v0]]))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_44]])), const<i32>(63), array_decay<ptr<i8>, length=Some(35)>(%[[VALUE_str_45]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_46]])), const<i32>(64), array_decay<ptr<i8>, length=Some(43)>(%[[VALUE_str_47]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_48]])), const<i32>(65), array_decay<ptr<i8>, length=Some(45)>(%[[VALUE_str_49]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_s1]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_50]])), const<i32>(66), array_decay<ptr<i8>, length=Some(45)>(%[[VALUE_str_51]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32>(%[[VALUE_i0]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i0]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_52]])), const<i32>(68), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_53]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_54]])), const<i32>(69), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_55]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32>(%[[VALUE_i2]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i2]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_56]])), const<i32>(70), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_57]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32>(%[[VALUE_i3]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i3]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_58]])), const<i32>(71), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_59]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32>(%[[VALUE_i4]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i4]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_60]])), const<i32>(72), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_61]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32, volatile>(%[[VALUE_v0]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v0]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_62]])), const<i32>(74), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_63]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32, volatile>(%[[VALUE_v1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v1]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_64]])), const<i32>(75), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_65]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32, volatile>(%[[VALUE_v2]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v2]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_66]])), const<i32>(76), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_67]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32, volatile>(%[[VALUE_v3]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v3]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_68]])), const<i32>(77), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_69]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s4]]), read<i32, volatile>(%[[VALUE_v4]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_v4]]))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_70]])), const<i32>(78), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_71]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]])), const<i32>(0), const<u64>(8)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_72]])), const<i32>(80), array_decay<ptr<i8>, length=Some(34)>(%[[VALUE_str_73]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), const<i32>(0))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_74]])), const<i32>(82), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_75]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), const<i32>(1))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_76]])), const<i32>(83), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_77]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), const<i32>(0)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_78]])), const<i32>(85), array_decay<ptr<i8>, length=Some(48)>(%[[VALUE_str_79]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), const<i32>(1)))), const<i32>(2))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_80]])), const<i32>(86), array_decay<ptr<i8>, length=Some(48)>(%[[VALUE_str_81]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), const<i32>(1)))), const<i32>(3))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_82]])), const<i32>(87), array_decay<ptr<i8>, length=Some(48)>(%[[VALUE_str_83]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), read<i32, volatile>(%[[VALUE_v0]]))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_84]])), const<i32>(89), array_decay<ptr<i8>, length=Some(42)>(%[[VALUE_str_85]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%[[VALUE_s4_2]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_86]])), const<i32>(90), array_decay<ptr<i8>, length=Some(50)>(%[[VALUE_str_87]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_88]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_89]])), const<i32>(93), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_90]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]])), const<i32>(0), const<u64>(15)), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_91]])), const<i32>(94), array_decay<ptr<i8>, length=Some(44)>(%[[VALUE_str_92]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(0)))), const<i32>(0)))), read<i32>(%[[VALUE_i0]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i0]]))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_93]])), const<i32>(96), array_decay<ptr<i8>, length=Some(61)>(%[[VALUE_str_94]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(0)))), const<i32>(0)))), read<i32>(%[[VALUE_i1]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i1]]))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_95]])), const<i32>(97), array_decay<ptr<i8>, length=Some(61)>(%[[VALUE_str_96]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(0)))), const<i32>(0)))), read<i32>(%[[VALUE_i2]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i2]]))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_97]])), const<i32>(98), array_decay<ptr<i8>, length=Some(61)>(%[[VALUE_str_98]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(0)))), const<i32>(0)))), read<i32>(%[[VALUE_i4]]))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i4]]))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_99]])), const<i32>(99), array_decay<ptr<i8>, length=Some(61)>(%[[VALUE_str_100]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), read<i32>(%[[VALUE_i0]]))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i0]]))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%[[VALUE_s5_3]]), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_101]])), const<i32>(101), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_102]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_wide:[0-9]+]] @test_wide() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_narrow]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_wide]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
