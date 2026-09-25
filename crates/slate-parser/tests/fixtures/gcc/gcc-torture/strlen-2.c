/* PR tree-optimization/86532 - Wrong code due to a wrong strlen folding  */

extern __SIZE_TYPE__ strlen(const char *);

static const char a[2][3]    = {"1", "12"};
static const char b[2][2][5] = {{"1", "12"}, {"123", "1234"}};

volatile int v0 = 0;
volatile int v1 = 1;
volatile int v2 = 2;

#define A(expr)                                                                \
  ((expr) ? (void)0                                                            \
          : (__builtin_printf("assertion on line %i: %s\n", __LINE__, #expr),  \
             __builtin_abort()))

void test_array_ref_2_3(void) {
  A(strlen(a[v0]) == 1);
  A(strlen(&a[v0][v0]) == 1);
  A(strlen(&a[0][v0]) == 1);
  A(strlen(&a[v0][0]) == 1);

  A(strlen(a[v1]) == 2);
  A(strlen(&a[v1][0]) == 2);
  A(strlen(&a[1][v0]) == 2);
  A(strlen(&a[v1][v0]) == 2);

  A(strlen(&a[v1][1]) == 1);
  A(strlen(&a[v1][1]) == 1);

  A(strlen(&a[v1][2]) == 0);
  A(strlen(&a[v1][v2]) == 0);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;

  A(strlen(a[v0]) == 1);
  A(strlen(&a[v0][v0]) == 1);
  A(strlen(&a[i0][v0]) == 1);
  A(strlen(&a[v0][i0]) == 1);

  A(strlen(a[v1]) == 2);
  A(strlen(&a[v1][i0]) == 2);
  A(strlen(&a[i1][v0]) == 2);
  A(strlen(&a[v1][v0]) == 2);

  A(strlen(&a[v1][i1]) == 1);
  A(strlen(&a[v1][i1]) == 1);

  A(strlen(&a[v1][i2]) == 0);
  A(strlen(&a[v1][v2]) == 0);
}

void test_array_off_2_3(void) {
  A(strlen(a[0] + 0) == 1);
  A(strlen(a[0] + v0) == 1);
  A(strlen(a[v0] + 0) == 1);
  A(strlen(a[v0] + v0) == 1);

  A(strlen(a[v1] + 0) == 2);
  A(strlen(a[1] + v0) == 2);
  A(strlen(a[v1] + 0) == 2);
  A(strlen(a[v1] + v0) == 2);

  A(strlen(a[v1] + 1) == 1);
  A(strlen(a[v1] + v1) == 1);

  A(strlen(a[v1] + 2) == 0);
  A(strlen(a[v1] + v2) == 0);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;

  A(strlen(a[i0] + i0) == 1);
  A(strlen(a[i0] + v0) == 1);
  A(strlen(a[v0] + i0) == 1);
  A(strlen(a[v0] + v0) == 1);

  A(strlen(a[v1] + i0) == 2);
  A(strlen(a[i1] + v0) == 2);
  A(strlen(a[v1] + i0) == 2);
  A(strlen(a[v1] + v0) == 2);

  A(strlen(a[v1] + i1) == 1);
  A(strlen(a[v1] + v1) == 1);

  A(strlen(a[v1] + i2) == 0);
  A(strlen(a[v1] + v2) == 0);
}

void test_array_ref_2_2_5(void) {
  A(strlen(b[0][v0]) == 1);
  A(strlen(b[v0][0]) == 1);

  A(strlen(&b[0][0][v0]) == 1);
  A(strlen(&b[0][v0][0]) == 1);
  A(strlen(&b[v0][0][0]) == 1);

  A(strlen(&b[0][v0][v0]) == 1);
  A(strlen(&b[v0][0][v0]) == 1);
  A(strlen(&b[v0][v0][0]) == 1);

  A(strlen(b[0][v1]) == 2);
  A(strlen(b[v1][0]) == 3);

  A(strlen(&b[0][0][v1]) == 0);
  A(strlen(&b[0][v1][0]) == 2);
  A(strlen(&b[v0][0][0]) == 1);

  A(strlen(&b[0][v0][v0]) == 1);
  A(strlen(&b[v0][0][v0]) == 1);
  A(strlen(&b[v0][v0][0]) == 1);

  A(strlen(&b[0][v1][v1]) == 1);
  A(strlen(&b[v1][0][v1]) == 2);
  A(strlen(&b[v1][v1][0]) == 4);
  A(strlen(&b[v1][v1][1]) == 3);
  A(strlen(&b[v1][v1][2]) == 2);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;

  A(strlen(b[i0][v0]) == 1);
  A(strlen(b[v0][i0]) == 1);

  A(strlen(&b[i0][i0][v0]) == 1);
  A(strlen(&b[i0][v0][i0]) == 1);
  A(strlen(&b[v0][i0][i0]) == 1);

  A(strlen(&b[i0][v0][v0]) == 1);
  A(strlen(&b[v0][i0][v0]) == 1);
  A(strlen(&b[v0][v0][i0]) == 1);

  A(strlen(b[i0][v1]) == 2);
  A(strlen(b[v1][i0]) == 3);

  A(strlen(&b[i0][i0][v1]) == 0);
  A(strlen(&b[i0][v1][i0]) == 2);
  A(strlen(&b[v0][i0][i0]) == 1);

  A(strlen(&b[i0][v0][v0]) == 1);
  A(strlen(&b[v0][i0][v0]) == 1);
  A(strlen(&b[v0][v0][i0]) == 1);

  A(strlen(&b[i0][v1][v1]) == 1);
  A(strlen(&b[v1][i0][v1]) == 2);
  A(strlen(&b[v1][v1][i0]) == 4);
  A(strlen(&b[v1][v1][i1]) == 3);
  A(strlen(&b[v1][v1][i2]) == 2);
}

void test_array_off_2_2_5(void) {
  A(strlen(b[0][0] + v0) == 1);
  A(strlen(b[0][v0] + v0) == 1);
  A(strlen(b[v0][0] + v0) == 1);
  A(strlen(b[v0][v0] + v0) == 1);

  A(strlen(b[0][0] + v1) == 0);
  A(strlen(b[0][v1] + 0) == 2);
  A(strlen(b[v0][0] + 0) == 1);

  A(strlen(b[0][v0] + v0) == 1);
  A(strlen(b[v0][0] + v0) == 1);
  A(strlen(b[v0][v0] + 0) == 1);

  A(strlen(b[0][v1] + v1) == 1);
  A(strlen(b[v1][0] + v1) == 2);
  A(strlen(b[v1][v1] + 0) == 4);
  A(strlen(b[v1][v1] + 1) == 3);
  A(strlen(b[v1][v1] + 2) == 2);

  int i0 = 0;
  int i1 = i0 + 1;
  int i2 = i1 + 1;

  A(strlen(b[i0][i0] + v0) == 1);
  A(strlen(b[i0][v0] + v0) == 1);
  A(strlen(b[v0][i0] + v0) == 1);
  A(strlen(b[v0][v0] + v0) == 1);

  A(strlen(b[i0][i0] + v1) == 0);
  A(strlen(b[i0][v1] + i0) == 2);
  A(strlen(b[v0][i0] + i0) == 1);

  A(strlen(b[i0][v0] + v0) == 1);
  A(strlen(b[v0][i0] + v0) == 1);
  A(strlen(b[v0][v0] + i0) == 1);

  A(strlen(b[i0][v1] + v1) == 1);
  A(strlen(b[v1][i0] + v1) == 2);
  A(strlen(b[v1][v1] + i0) == 4);
  A(strlen(b[v1][v1] + i1) == 3);
  A(strlen(b[v1][v1] + i2) == 2);
}

int main() {
  test_array_ref_2_3();
  test_array_off_2_3();

  test_array_ref_2_2_5();
  test_array_off_2_2_5();
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
// DEFAULT-NEXT:     global %1 a: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = code_units<array<i8, 3>>([49, 0, 0]), index1 = code_units<array<i8, 3>>([49, 50, 0])) [linkage=internal];
// DEFAULT-NEXT:     global %2 b: array<array<array<i8, 5>, 2>, 2> [storage=static] [const] = aggregate<array<array<array<i8, 5>, 2>, 2>, zero_fill=false>(index0 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 0, 0, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 0, 0, 0])), index1 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 50, 51, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 51, 52, 0]))) [linkage=internal];
// DEFAULT-NEXT:     global %3 v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %4 v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %145 .str145: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %147 .str147: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %148 .str148: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %150 .str150: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %151 .str151: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %152 .str152: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %154 .str154: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %156 .str156: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %157 .str157: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 .str158: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %159 .str159: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %160 .str160: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %161 .str161: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %162 .str162: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %163 .str163: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %164 .str164: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %165 .str165: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %166 .str166: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %167 .str167: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %168 .str168: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %169 .str169: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %170 .str170: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 .str171: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %172 .str172: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %173 .str173: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %174 .str174: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 .str175: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %176 .str176: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %177 .str177: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %178 .str178: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %179 .str179: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %181 .str181: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %182 .str182: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %183 .str183: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %186 .str186: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %190 .str190: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %192 .str192: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %193 .str193: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %194 .str194: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %195 .str195: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %196 .str196: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %197 .str197: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %198 .str198: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %199 .str199: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %200 .str200: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %201 .str201: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %202 .str202: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %203 .str203: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %204 .str204: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %205 .str205: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %206 .str206: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %207 .str207: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %208 .str208: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %209 .str209: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %210 .str210: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %211 .str211: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %212 .str212: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %213 .str213: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %214 .str214: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %215 .str215: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %216 .str216: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %217 .str217: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %218 .str218: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %219 .str219: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %220 .str220: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %221 .str221: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %222 .str222: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 .str223: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %224 .str224: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %225 .str225: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %226 .str226: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %227 .str227: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %228 .str228: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %229 .str229: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %230 .str230: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %231 .str231: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %232 .str232: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %233 .str233: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %234 .str234: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %235 .str235: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %236 .str236: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %237 .str237: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %238 .str238: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %239 .str239: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %240 .str240: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %241 .str241: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %242 .str242: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %243 .str243: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %244 .str244: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %249 .str249: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %251 .str251: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %258 .str258: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strlen(%23 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %6 @test_array_ref_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%24)), const<i32>(18), array_decay<ptr<i8>, length=Some(19)>(%25));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%26)), const<i32>(19), array_decay<ptr<i8>, length=Some(24)>(%27));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%28)), const<i32>(20), array_decay<ptr<i8>, length=Some(23)>(%29));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%30)), const<i32>(21), array_decay<ptr<i8>, length=Some(23)>(%31));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%32)), const<i32>(23), array_decay<ptr<i8>, length=Some(19)>(%33));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%34)), const<i32>(24), array_decay<ptr<i8>, length=Some(23)>(%35));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(1)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%36)), const<i32>(25), array_decay<ptr<i8>, length=Some(23)>(%37));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%38)), const<i32>(26), array_decay<ptr<i8>, length=Some(24)>(%39));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%40)), const<i32>(28), array_decay<ptr<i8>, length=Some(23)>(%41));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%42)), const<i32>(29), array_decay<ptr<i8>, length=Some(23)>(%43));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%44)), const<i32>(31), array_decay<ptr<i8>, length=Some(23)>(%45));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%46)), const<i32>(32), array_decay<ptr<i8>, length=Some(24)>(%47));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %7 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:         let %9 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%48)), const<i32>(38), array_decay<ptr<i8>, length=Some(19)>(%49));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%50)), const<i32>(39), array_decay<ptr<i8>, length=Some(24)>(%51));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%7)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%52)), const<i32>(40), array_decay<ptr<i8>, length=Some(24)>(%53));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32>(%7))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%54)), const<i32>(41), array_decay<ptr<i8>, length=Some(24)>(%55));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%56)), const<i32>(43), array_decay<ptr<i8>, length=Some(19)>(%57));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%7))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%58)), const<i32>(44), array_decay<ptr<i8>, length=Some(24)>(%59));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%8)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%60)), const<i32>(45), array_decay<ptr<i8>, length=Some(24)>(%61));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%62)), const<i32>(46), array_decay<ptr<i8>, length=Some(24)>(%63));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%64)), const<i32>(48), array_decay<ptr<i8>, length=Some(24)>(%65));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%66)), const<i32>(49), array_decay<ptr<i8>, length=Some(24)>(%67));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%9))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%68)), const<i32>(51), array_decay<ptr<i8>, length=Some(24)>(%69));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%70)), const<i32>(52), array_decay<ptr<i8>, length=Some(24)>(%71));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_array_off_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%72)), const<i32>(56), array_decay<ptr<i8>, length=Some(22)>(%73));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%74)), const<i32>(57), array_decay<ptr<i8>, length=Some(23)>(%75));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%76)), const<i32>(58), array_decay<ptr<i8>, length=Some(23)>(%77));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%78)), const<i32>(59), array_decay<ptr<i8>, length=Some(24)>(%79));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%80)), const<i32>(61), array_decay<ptr<i8>, length=Some(23)>(%81));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(1)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%82)), const<i32>(62), array_decay<ptr<i8>, length=Some(23)>(%83));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%84)), const<i32>(63), array_decay<ptr<i8>, length=Some(23)>(%85));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%86)), const<i32>(64), array_decay<ptr<i8>, length=Some(24)>(%87));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%88)), const<i32>(66), array_decay<ptr<i8>, length=Some(23)>(%89));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%90)), const<i32>(67), array_decay<ptr<i8>, length=Some(24)>(%91));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%92)), const<i32>(69), array_decay<ptr<i8>, length=Some(23)>(%93));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%94)), const<i32>(70), array_decay<ptr<i8>, length=Some(24)>(%95));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %11 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %12 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         let %13 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%96)), const<i32>(76), array_decay<ptr<i8>, length=Some(24)>(%97));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%98)), const<i32>(77), array_decay<ptr<i8>, length=Some(24)>(%99));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%100)), const<i32>(78), array_decay<ptr<i8>, length=Some(24)>(%101));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%102)), const<i32>(79), array_decay<ptr<i8>, length=Some(24)>(%103));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%104)), const<i32>(81), array_decay<ptr<i8>, length=Some(24)>(%105));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%106)), const<i32>(82), array_decay<ptr<i8>, length=Some(24)>(%107));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%108)), const<i32>(83), array_decay<ptr<i8>, length=Some(24)>(%109));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%110)), const<i32>(84), array_decay<ptr<i8>, length=Some(24)>(%111));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%112)), const<i32>(86), array_decay<ptr<i8>, length=Some(24)>(%113));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%114)), const<i32>(87), array_decay<ptr<i8>, length=Some(24)>(%115));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%13))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%116)), const<i32>(89), array_decay<ptr<i8>, length=Some(24)>(%117));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%118)), const<i32>(90), array_decay<ptr<i8>, length=Some(24)>(%119));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_array_ref_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%120)), const<i32>(94), array_decay<ptr<i8>, length=Some(22)>(%121));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%122)), const<i32>(95), array_decay<ptr<i8>, length=Some(22)>(%123));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%124)), const<i32>(97), array_decay<ptr<i8>, length=Some(26)>(%125));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%126)), const<i32>(98), array_decay<ptr<i8>, length=Some(26)>(%127));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%128)), const<i32>(99), array_decay<ptr<i8>, length=Some(26)>(%129));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%130)), const<i32>(101), array_decay<ptr<i8>, length=Some(27)>(%131));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%132)), const<i32>(102), array_decay<ptr<i8>, length=Some(27)>(%133));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%134)), const<i32>(103), array_decay<ptr<i8>, length=Some(27)>(%135));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%136)), const<i32>(105), array_decay<ptr<i8>, length=Some(22)>(%137));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%138)), const<i32>(106), array_decay<ptr<i8>, length=Some(22)>(%139));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%140)), const<i32>(108), array_decay<ptr<i8>, length=Some(26)>(%141));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%142)), const<i32>(109), array_decay<ptr<i8>, length=Some(26)>(%143));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%144)), const<i32>(110), array_decay<ptr<i8>, length=Some(26)>(%145));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%146)), const<i32>(112), array_decay<ptr<i8>, length=Some(27)>(%147));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%148)), const<i32>(113), array_decay<ptr<i8>, length=Some(27)>(%149));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%150)), const<i32>(114), array_decay<ptr<i8>, length=Some(27)>(%151));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%152)), const<i32>(116), array_decay<ptr<i8>, length=Some(27)>(%153));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%154)), const<i32>(117), array_decay<ptr<i8>, length=Some(27)>(%155));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%156)), const<i32>(118), array_decay<ptr<i8>, length=Some(27)>(%157));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%158)), const<i32>(119), array_decay<ptr<i8>, length=Some(27)>(%159));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%160)), const<i32>(120), array_decay<ptr<i8>, length=Some(27)>(%161));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %15 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %16 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         let %17 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%162)), const<i32>(126), array_decay<ptr<i8>, length=Some(23)>(%163));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%164)), const<i32>(127), array_decay<ptr<i8>, length=Some(23)>(%165));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%166)), const<i32>(129), array_decay<ptr<i8>, length=Some(28)>(%167));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%168)), const<i32>(130), array_decay<ptr<i8>, length=Some(28)>(%169));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%170)), const<i32>(131), array_decay<ptr<i8>, length=Some(28)>(%171));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%172)), const<i32>(133), array_decay<ptr<i8>, length=Some(28)>(%173));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%174)), const<i32>(134), array_decay<ptr<i8>, length=Some(28)>(%175));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%176)), const<i32>(135), array_decay<ptr<i8>, length=Some(28)>(%177));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%178)), const<i32>(137), array_decay<ptr<i8>, length=Some(23)>(%179));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%180)), const<i32>(138), array_decay<ptr<i8>, length=Some(23)>(%181));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%182)), const<i32>(140), array_decay<ptr<i8>, length=Some(28)>(%183));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%184)), const<i32>(141), array_decay<ptr<i8>, length=Some(28)>(%185));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%186)), const<i32>(142), array_decay<ptr<i8>, length=Some(28)>(%187));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%188)), const<i32>(144), array_decay<ptr<i8>, length=Some(28)>(%189));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%190)), const<i32>(145), array_decay<ptr<i8>, length=Some(28)>(%191));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%192)), const<i32>(146), array_decay<ptr<i8>, length=Some(28)>(%193));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%194)), const<i32>(148), array_decay<ptr<i8>, length=Some(28)>(%195));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%196)), const<i32>(149), array_decay<ptr<i8>, length=Some(28)>(%197));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%198)), const<i32>(150), array_decay<ptr<i8>, length=Some(28)>(%199));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%16))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%200)), const<i32>(151), array_decay<ptr<i8>, length=Some(28)>(%201));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%17))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%202)), const<i32>(152), array_decay<ptr<i8>, length=Some(28)>(%203));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_array_off_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%204)), const<i32>(156), array_decay<ptr<i8>, length=Some(26)>(%205));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%206)), const<i32>(157), array_decay<ptr<i8>, length=Some(27)>(%207));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%208)), const<i32>(158), array_decay<ptr<i8>, length=Some(27)>(%209));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%210)), const<i32>(159), array_decay<ptr<i8>, length=Some(28)>(%211));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%212)), const<i32>(161), array_decay<ptr<i8>, length=Some(26)>(%213));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%214)), const<i32>(162), array_decay<ptr<i8>, length=Some(26)>(%215));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%216)), const<i32>(163), array_decay<ptr<i8>, length=Some(26)>(%217));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%218)), const<i32>(165), array_decay<ptr<i8>, length=Some(27)>(%219));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%220)), const<i32>(166), array_decay<ptr<i8>, length=Some(27)>(%221));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%222)), const<i32>(167), array_decay<ptr<i8>, length=Some(27)>(%223));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%224)), const<i32>(169), array_decay<ptr<i8>, length=Some(27)>(%225));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%226)), const<i32>(170), array_decay<ptr<i8>, length=Some(27)>(%227));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%228)), const<i32>(171), array_decay<ptr<i8>, length=Some(27)>(%229));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%230)), const<i32>(172), array_decay<ptr<i8>, length=Some(27)>(%231));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%232)), const<i32>(173), array_decay<ptr<i8>, length=Some(27)>(%233));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %19 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %20 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:         let %21 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%234)), const<i32>(179), array_decay<ptr<i8>, length=Some(28)>(%235));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%236)), const<i32>(180), array_decay<ptr<i8>, length=Some(28)>(%237));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%238)), const<i32>(181), array_decay<ptr<i8>, length=Some(28)>(%239));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%240)), const<i32>(182), array_decay<ptr<i8>, length=Some(28)>(%241));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32>(%19)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%242)), const<i32>(184), array_decay<ptr<i8>, length=Some(28)>(%243));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%4)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%244)), const<i32>(185), array_decay<ptr<i8>, length=Some(28)>(%245));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%246)), const<i32>(186), array_decay<ptr<i8>, length=Some(28)>(%247));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%248)), const<i32>(188), array_decay<ptr<i8>, length=Some(28)>(%249));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%250)), const<i32>(189), array_decay<ptr<i8>, length=Some(28)>(%251));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%252)), const<i32>(190), array_decay<ptr<i8>, length=Some(28)>(%253));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%254)), const<i32>(192), array_decay<ptr<i8>, length=Some(28)>(%255));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%19)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%256)), const<i32>(193), array_decay<ptr<i8>, length=Some(28)>(%257));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%258)), const<i32>(194), array_decay<ptr<i8>, length=Some(28)>(%259));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%20))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%260)), const<i32>(195), array_decay<ptr<i8>, length=Some(28)>(%261));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%21))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%262)), const<i32>(196), array_decay<ptr<i8>, length=Some(28)>(%263));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
