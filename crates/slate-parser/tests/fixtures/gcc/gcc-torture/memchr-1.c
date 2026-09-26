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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 wchar_t = i32;
// DEFAULT-NEXT:     global %3 c: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(49)) [linkage=internal];
// DEFAULT-NEXT:     global %4 s1: array<i8, 1> [storage=static] [const] = code_units<array<i8, 1>>([49]) [linkage=internal];
// DEFAULT-NEXT:     global %5 s4: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([49, 50, 51, 52]) [linkage=internal];
// DEFAULT-NEXT:     global %6 s4_2: array<array<i8, 4>, 2> [storage=static] [const] = aggregate<array<array<i8, 4>, 2>, zero_fill=false>(index0 = code_units<array<i8, 4>>([49, 50, 51, 52]), index1 = code_units<array<i8, 4>>([53, 54, 55, 56])) [linkage=internal];
// DEFAULT-NEXT:     global %7 s5_3: array<array<i8, 5>, 3> [storage=static] [const] = aggregate<array<array<i8, 5>, 3>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 50, 51, 52, 53]), index1 = code_units<array<i8, 5>>([54, 55, 56, 57, 0]), index2 = code_units<array<i8, 5>>([48, 49, 50, 51, 52])) [linkage=internal];
// DEFAULT-NEXT:     global %8 v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %9 v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %10 v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %11 v3: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %12 v4: volatile i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([109, 101, 109, 99, 104, 114, 40, 34, 34, 32, 43, 32, 49, 44, 32, 48, 44, 32, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 99, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 99, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 49, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 43> [storage=static] = code_units<array<i8, 43>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 105, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 43> [storage=static] = code_units<array<i8, 43>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 49, 91, 118, 48, 93, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 49, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 105, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 105, 52, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 48, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 32, 43, 32, 118, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 32, 45, 32, 118, 52, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 48, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 48, 93, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 32, 43, 32, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 32, 45, 32, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 49, 93, 32, 43, 32, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 49, 93, 32, 45, 32, 51, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 118, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 118, 48, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 50> [storage=static] = code_units<array<i8, 50>>([109, 101, 109, 99, 104, 114, 40, 115, 52, 95, 50, 91, 118, 48, 93, 32, 43, 32, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 52, 95, 50, 91, 118, 48, 93, 32, 45, 32, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([109, 101, 109, 99, 104, 114, 40, 34, 34, 44, 32, 48, 44, 32, 49, 41, 32, 33, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([109, 101, 109, 99, 104, 114, 40, 115, 53, 95, 51, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 48, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 49, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 49, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 50, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 50, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 48, 93, 91, 48, 93, 32, 43, 32, 105, 52, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 32, 45, 32, 105, 52, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([109, 101, 109, 99, 104, 114, 40, 38, 115, 53, 95, 51, 91, 49, 93, 91, 105, 48, 93, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 115, 53, 95, 51, 91, 49, 93, 32, 45, 32, 105, 48, 41, 32, 61, 61, 32, 38, 115, 53, 95, 51, 91, 49, 93, 91, 52, 93, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @memchr(%21 <unnamed>: ptr<const void>, %22 <unnamed>: i32, %23 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @test_narrow() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %15 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         let %16 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         let %17 i3: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         let %18 i4: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%24), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%25)), const<i32>(36), array_decay<ptr<i8>, length=Some(26)>(%26));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%3)), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%27)), const<i32>(38), array_decay<ptr<i8>, length=Some(29)>(%28));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%3), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%29)), const<i32>(39), array_decay<ptr<i8>, length=Some(37)>(%30));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%3), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%31)), const<i32>(40), array_decay<ptr<i8>, length=Some(39)>(%32));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(%3), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%33)), const<i32>(41), array_decay<ptr<i8>, length=Some(39)>(%34));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(1)>(%4)), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%35)), const<i32>(43), array_decay<ptr<i8>, length=Some(30)>(%36));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%37)), const<i32>(44), array_decay<ptr<i8>, length=Some(38)>(%38));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%39)), const<i32>(45), array_decay<ptr<i8>, length=Some(40)>(%40));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%41)), const<i32>(46), array_decay<ptr<i8>, length=Some(40)>(%42));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const array<i8, 1>>>(%4)), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%43)), const<i32>(48), array_decay<ptr<i8>, length=Some(31)>(%44));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%4), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%45)), const<i32>(49), array_decay<ptr<i8>, length=Some(39)>(%46));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%4), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%47)), const<i32>(50), array_decay<ptr<i8>, length=Some(41)>(%48));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<const array<i8, 1>>>(%4), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%49)), const<i32>(51), array_decay<ptr<i8>, length=Some(41)>(%50));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), const<i32>(0))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%51)), const<i32>(53), array_decay<ptr<i8>, length=Some(34)>(%52));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), const<i32>(0)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%53)), const<i32>(54), array_decay<ptr<i8>, length=Some(42)>(%54));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), const<i32>(0)))), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%55)), const<i32>(55), array_decay<ptr<i8>, length=Some(44)>(%56));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), const<i32>(0)))), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%57)), const<i32>(56), array_decay<ptr<i8>, length=Some(44)>(%58));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32>(%14))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%59)), const<i32>(58), array_decay<ptr<i8>, length=Some(35)>(%60));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32>(%14)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%61)), const<i32>(59), array_decay<ptr<i8>, length=Some(43)>(%62));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32>(%14)))), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%63)), const<i32>(60), array_decay<ptr<i8>, length=Some(45)>(%64));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32>(%14)))), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%65)), const<i32>(61), array_decay<ptr<i8>, length=Some(45)>(%66));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32, volatile>(%8))))), const<i32>(0), const<u64>(1)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%67)), const<i32>(63), array_decay<ptr<i8>, length=Some(35)>(%68));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32, volatile>(%8)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%69)), const<i32>(64), array_decay<ptr<i8>, length=Some(43)>(%70));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32, volatile>(%8)))), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%71)), const<i32>(65), array_decay<ptr<i8>, length=Some(45)>(%72));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(1)>(%4), read<i32, volatile>(%8)))), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%73)), const<i32>(66), array_decay<ptr<i8>, length=Some(45)>(%74));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32>(%14))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%75)), const<i32>(68), array_decay<ptr<i8>, length=Some(40)>(%76));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%77)), const<i32>(69), array_decay<ptr<i8>, length=Some(40)>(%78));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32>(%16))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%16))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%79)), const<i32>(70), array_decay<ptr<i8>, length=Some(40)>(%80));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32>(%17))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%17))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%81)), const<i32>(71), array_decay<ptr<i8>, length=Some(40)>(%82));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32>(%18))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%83)), const<i32>(72), array_decay<ptr<i8>, length=Some(40)>(%84));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32, volatile>(%8))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%8))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%85)), const<i32>(74), array_decay<ptr<i8>, length=Some(40)>(%86));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32, volatile>(%9))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%9))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%87)), const<i32>(75), array_decay<ptr<i8>, length=Some(40)>(%88));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32, volatile>(%10))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%10))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%89)), const<i32>(76), array_decay<ptr<i8>, length=Some(40)>(%90));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32, volatile>(%11))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%11))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%91)), const<i32>(77), array_decay<ptr<i8>, length=Some(40)>(%92));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%5), read<i32, volatile>(%12))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(%12))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%93)), const<i32>(78), array_decay<ptr<i8>, length=Some(40)>(%94));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6)), const<i32>(0), const<u64>(8)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%95)), const<i32>(80), array_decay<ptr<i8>, length=Some(34)>(%96));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), const<i32>(0))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%97)), const<i32>(82), array_decay<ptr<i8>, length=Some(40)>(%98));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), const<i32>(1))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%99)), const<i32>(83), array_decay<ptr<i8>, length=Some(40)>(%100));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), const<i32>(0)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%101)), const<i32>(85), array_decay<ptr<i8>, length=Some(48)>(%102));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), const<i32>(1)))), const<i32>(2))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%103)), const<i32>(86), array_decay<ptr<i8>, length=Some(48)>(%104));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), const<i32>(1)))), const<i32>(3))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%105)), const<i32>(87), array_decay<ptr<i8>, length=Some(48)>(%106));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), read<i32, volatile>(%8))))), const<i32>(0), const<u64>(4)), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%107)), const<i32>(89), array_decay<ptr<i8>, length=Some(42)>(%108));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(deref(ptr_offset<ptr<const array<i8, 4>>, subtract=false, element=array<i8, 4>, overflow=ub>(array_decay<ptr<const array<i8, 4>>, length=Some(2)>(%6), read<i32, volatile>(%8)))), const<i32>(1))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%109)), const<i32>(90), array_decay<ptr<i8>, length=Some(50)>(%110));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%111)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>)
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%112)), const<i32>(93), array_decay<ptr<i8>, length=Some(22)>(%113));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7)), const<i32>(0), const<u64>(15)), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%114)), const<i32>(94), array_decay<ptr<i8>, length=Some(44)>(%115));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(0)))), const<i32>(0)))), read<i32>(%14))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%116)), const<i32>(96), array_decay<ptr<i8>, length=Some(61)>(%117));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(0)))), const<i32>(0)))), read<i32>(%15))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%118)), const<i32>(97), array_decay<ptr<i8>, length=Some(61)>(%119));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(0)))), const<i32>(0)))), read<i32>(%16))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%16))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%120)), const<i32>(98), array_decay<ptr<i8>, length=Some(61)>(%121));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(0)))), const<i32>(0)))), read<i32>(%18))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%122)), const<i32>(99), array_decay<ptr<i8>, length=Some(61)>(%123));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(memchr, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), read<i32>(%14))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(3)>(%7), const<i32>(1)))), const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%124)), const<i32>(101), array_decay<ptr<i8>, length=Some(60)>(%125));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_wide() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
