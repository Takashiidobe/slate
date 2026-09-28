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
// DEFAULT-NEXT:     global %2 b: array<array<array<i8, 5>, 2>, 2> [storage=static] [const] [align=16] = aggregate<array<array<array<i8, 5>, 2>, 2>, zero_fill=false>(index0 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 0, 0, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 0, 0, 0])), index1 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 50, 51, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 51, 52, 0]))) [linkage=internal];
// DEFAULT-NEXT:     global %3 v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %4 v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %145 .str145: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %147 .str147: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %148 .str148: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %150 .str150: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %151 .str151: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %152 .str152: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %154 .str154: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %156 .str156: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %157 .str157: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 .str158: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %159 .str159: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %160 .str160: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %161 .str161: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %162 .str162: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %163 .str163: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %164 .str164: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %165 .str165: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %166 .str166: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %167 .str167: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %168 .str168: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %169 .str169: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %170 .str170: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 .str171: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %172 .str172: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %173 .str173: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %174 .str174: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 .str175: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %176 .str176: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %177 .str177: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %178 .str178: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %179 .str179: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %181 .str181: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %182 .str182: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %183 .str183: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %186 .str186: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %190 .str190: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %192 .str192: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %193 .str193: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %194 .str194: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %195 .str195: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %196 .str196: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %197 .str197: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %198 .str198: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %199 .str199: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %200 .str200: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %201 .str201: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %202 .str202: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %203 .str203: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %204 .str204: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %205 .str205: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %206 .str206: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %207 .str207: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %208 .str208: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %209 .str209: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %210 .str210: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %211 .str211: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %212 .str212: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %213 .str213: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %214 .str214: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %215 .str215: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %216 .str216: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %217 .str217: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %218 .str218: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %219 .str219: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %220 .str220: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %221 .str221: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %222 .str222: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 .str223: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %224 .str224: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %225 .str225: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %226 .str226: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %227 .str227: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %228 .str228: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %229 .str229: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %230 .str230: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %231 .str231: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %232 .str232: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %233 .str233: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %234 .str234: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %235 .str235: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %236 .str236: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %237 .str237: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %238 .str238: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %239 .str239: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %240 .str240: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %241 .str241: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %242 .str242: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %243 .str243: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %244 .str244: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %249 .str249: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %251 .str251: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %258 .str258: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strlen(%23 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_printf(%24 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @test_array_ref_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%26)), const<i32>(18), array_decay<ptr<i8>, length=Some(19)>(%27));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%29)), const<i32>(19), array_decay<ptr<i8>, length=Some(24)>(%30));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%31)), const<i32>(20), array_decay<ptr<i8>, length=Some(23)>(%32));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%33)), const<i32>(21), array_decay<ptr<i8>, length=Some(23)>(%34));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%35)), const<i32>(23), array_decay<ptr<i8>, length=Some(19)>(%36));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%37)), const<i32>(24), array_decay<ptr<i8>, length=Some(23)>(%38));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(1)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%39)), const<i32>(25), array_decay<ptr<i8>, length=Some(23)>(%40));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%41)), const<i32>(26), array_decay<ptr<i8>, length=Some(24)>(%42));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%43)), const<i32>(28), array_decay<ptr<i8>, length=Some(23)>(%44));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%45)), const<i32>(29), array_decay<ptr<i8>, length=Some(23)>(%46));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%47)), const<i32>(31), array_decay<ptr<i8>, length=Some(23)>(%48));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%49)), const<i32>(32), array_decay<ptr<i8>, length=Some(24)>(%50));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         let %7 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:         let %9 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%51)), const<i32>(38), array_decay<ptr<i8>, length=Some(19)>(%52));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%53)), const<i32>(39), array_decay<ptr<i8>, length=Some(24)>(%54));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%7)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%55)), const<i32>(40), array_decay<ptr<i8>, length=Some(24)>(%56));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32>(%7))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%57)), const<i32>(41), array_decay<ptr<i8>, length=Some(24)>(%58));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%59)), const<i32>(43), array_decay<ptr<i8>, length=Some(19)>(%60));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%7))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%61)), const<i32>(44), array_decay<ptr<i8>, length=Some(24)>(%62));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%8)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%63)), const<i32>(45), array_decay<ptr<i8>, length=Some(24)>(%64));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%65)), const<i32>(46), array_decay<ptr<i8>, length=Some(24)>(%66));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%67)), const<i32>(48), array_decay<ptr<i8>, length=Some(24)>(%68));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%69)), const<i32>(49), array_decay<ptr<i8>, length=Some(24)>(%70));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%9))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%71)), const<i32>(51), array_decay<ptr<i8>, length=Some(24)>(%72));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%73)), const<i32>(52), array_decay<ptr<i8>, length=Some(24)>(%74));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_array_off_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%75)), const<i32>(56), array_decay<ptr<i8>, length=Some(22)>(%76));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%77)), const<i32>(57), array_decay<ptr<i8>, length=Some(23)>(%78));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%79)), const<i32>(58), array_decay<ptr<i8>, length=Some(23)>(%80));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%81)), const<i32>(59), array_decay<ptr<i8>, length=Some(24)>(%82));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%83)), const<i32>(61), array_decay<ptr<i8>, length=Some(23)>(%84));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), const<i32>(1)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%85)), const<i32>(62), array_decay<ptr<i8>, length=Some(23)>(%86));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%87)), const<i32>(63), array_decay<ptr<i8>, length=Some(23)>(%88));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%89)), const<i32>(64), array_decay<ptr<i8>, length=Some(24)>(%90));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%91)), const<i32>(66), array_decay<ptr<i8>, length=Some(23)>(%92));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%93)), const<i32>(67), array_decay<ptr<i8>, length=Some(24)>(%94));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%95)), const<i32>(69), array_decay<ptr<i8>, length=Some(23)>(%96));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%97)), const<i32>(70), array_decay<ptr<i8>, length=Some(24)>(%98));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         let %11 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %12 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         let %13 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%99)), const<i32>(76), array_decay<ptr<i8>, length=Some(24)>(%100));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%11)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%101)), const<i32>(77), array_decay<ptr<i8>, length=Some(24)>(%102));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%103)), const<i32>(78), array_decay<ptr<i8>, length=Some(24)>(%104));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%105)), const<i32>(79), array_decay<ptr<i8>, length=Some(24)>(%106));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%107)), const<i32>(81), array_decay<ptr<i8>, length=Some(24)>(%108));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32>(%12)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%109)), const<i32>(82), array_decay<ptr<i8>, length=Some(24)>(%110));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%111)), const<i32>(83), array_decay<ptr<i8>, length=Some(24)>(%112));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%113)), const<i32>(84), array_decay<ptr<i8>, length=Some(24)>(%114));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%12))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%115)), const<i32>(86), array_decay<ptr<i8>, length=Some(24)>(%116));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%117)), const<i32>(87), array_decay<ptr<i8>, length=Some(24)>(%118));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32>(%13))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%119)), const<i32>(89), array_decay<ptr<i8>, length=Some(24)>(%120));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%1), read<i32, volatile>(%4)))), read<i32, volatile>(%5))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%121)), const<i32>(90), array_decay<ptr<i8>, length=Some(24)>(%122));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_array_ref_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%123)), const<i32>(94), array_decay<ptr<i8>, length=Some(22)>(%124));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%125)), const<i32>(95), array_decay<ptr<i8>, length=Some(22)>(%126));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%127)), const<i32>(97), array_decay<ptr<i8>, length=Some(26)>(%128));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%129)), const<i32>(98), array_decay<ptr<i8>, length=Some(26)>(%130));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%131)), const<i32>(99), array_decay<ptr<i8>, length=Some(26)>(%132));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%133)), const<i32>(101), array_decay<ptr<i8>, length=Some(27)>(%134));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%135)), const<i32>(102), array_decay<ptr<i8>, length=Some(27)>(%136));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%137)), const<i32>(103), array_decay<ptr<i8>, length=Some(27)>(%138));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%139)), const<i32>(105), array_decay<ptr<i8>, length=Some(22)>(%140));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%141)), const<i32>(106), array_decay<ptr<i8>, length=Some(22)>(%142));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%143)), const<i32>(108), array_decay<ptr<i8>, length=Some(26)>(%144));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%145)), const<i32>(109), array_decay<ptr<i8>, length=Some(26)>(%146));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%147)), const<i32>(110), array_decay<ptr<i8>, length=Some(26)>(%148));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%149)), const<i32>(112), array_decay<ptr<i8>, length=Some(27)>(%150));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%151)), const<i32>(113), array_decay<ptr<i8>, length=Some(27)>(%152));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%153)), const<i32>(114), array_decay<ptr<i8>, length=Some(27)>(%154));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%155)), const<i32>(116), array_decay<ptr<i8>, length=Some(27)>(%156));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%157)), const<i32>(117), array_decay<ptr<i8>, length=Some(27)>(%158));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%159)), const<i32>(118), array_decay<ptr<i8>, length=Some(27)>(%160));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%161)), const<i32>(119), array_decay<ptr<i8>, length=Some(27)>(%162));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%163)), const<i32>(120), array_decay<ptr<i8>, length=Some(27)>(%164));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         let %15 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %16 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         let %17 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%165)), const<i32>(126), array_decay<ptr<i8>, length=Some(23)>(%166));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%167)), const<i32>(127), array_decay<ptr<i8>, length=Some(23)>(%168));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%169)), const<i32>(129), array_decay<ptr<i8>, length=Some(28)>(%170));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%171)), const<i32>(130), array_decay<ptr<i8>, length=Some(28)>(%172));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%173)), const<i32>(131), array_decay<ptr<i8>, length=Some(28)>(%174));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%175)), const<i32>(133), array_decay<ptr<i8>, length=Some(28)>(%176));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%177)), const<i32>(134), array_decay<ptr<i8>, length=Some(28)>(%178));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%179)), const<i32>(135), array_decay<ptr<i8>, length=Some(28)>(%180));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%181)), const<i32>(137), array_decay<ptr<i8>, length=Some(23)>(%182));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%183)), const<i32>(138), array_decay<ptr<i8>, length=Some(23)>(%184));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%185)), const<i32>(140), array_decay<ptr<i8>, length=Some(28)>(%186));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%187)), const<i32>(141), array_decay<ptr<i8>, length=Some(28)>(%188));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%189)), const<i32>(142), array_decay<ptr<i8>, length=Some(28)>(%190));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%191)), const<i32>(144), array_decay<ptr<i8>, length=Some(28)>(%192));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%15)))), read<i32, volatile>(%3))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%193)), const<i32>(145), array_decay<ptr<i8>, length=Some(28)>(%194));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%195)), const<i32>(146), array_decay<ptr<i8>, length=Some(28)>(%196));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%15)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%197)), const<i32>(148), array_decay<ptr<i8>, length=Some(28)>(%198));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%15)))), read<i32, volatile>(%4))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%199)), const<i32>(149), array_decay<ptr<i8>, length=Some(28)>(%200));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%15))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%201)), const<i32>(150), array_decay<ptr<i8>, length=Some(28)>(%202));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%16))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%203)), const<i32>(151), array_decay<ptr<i8>, length=Some(28)>(%204));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%17))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%205)), const<i32>(152), array_decay<ptr<i8>, length=Some(28)>(%206));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_array_off_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%207)), const<i32>(156), array_decay<ptr<i8>, length=Some(26)>(%208));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%209)), const<i32>(157), array_decay<ptr<i8>, length=Some(27)>(%210));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%211)), const<i32>(158), array_decay<ptr<i8>, length=Some(27)>(%212));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%213)), const<i32>(159), array_decay<ptr<i8>, length=Some(28)>(%214));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%215)), const<i32>(161), array_decay<ptr<i8>, length=Some(26)>(%216));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%217)), const<i32>(162), array_decay<ptr<i8>, length=Some(26)>(%218));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%219)), const<i32>(163), array_decay<ptr<i8>, length=Some(26)>(%220));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%221)), const<i32>(165), array_decay<ptr<i8>, length=Some(27)>(%222));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), const<i32>(0)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%223)), const<i32>(166), array_decay<ptr<i8>, length=Some(27)>(%224));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%225)), const<i32>(167), array_decay<ptr<i8>, length=Some(27)>(%226));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), const<i32>(0)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%227)), const<i32>(169), array_decay<ptr<i8>, length=Some(27)>(%228));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), const<i32>(0)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%229)), const<i32>(170), array_decay<ptr<i8>, length=Some(27)>(%230));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%231)), const<i32>(171), array_decay<ptr<i8>, length=Some(27)>(%232));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%233)), const<i32>(172), array_decay<ptr<i8>, length=Some(27)>(%234));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%235)), const<i32>(173), array_decay<ptr<i8>, length=Some(27)>(%236));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         let %19 i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %20 i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:         let %21 i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%237)), const<i32>(179), array_decay<ptr<i8>, length=Some(28)>(%238));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%239)), const<i32>(180), array_decay<ptr<i8>, length=Some(28)>(%240));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%241)), const<i32>(181), array_decay<ptr<i8>, length=Some(28)>(%242));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%243)), const<i32>(182), array_decay<ptr<i8>, length=Some(28)>(%244));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32>(%19)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%245)), const<i32>(184), array_decay<ptr<i8>, length=Some(28)>(%246));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%4)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%247)), const<i32>(185), array_decay<ptr<i8>, length=Some(28)>(%248));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%249)), const<i32>(186), array_decay<ptr<i8>, length=Some(28)>(%250));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%3)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%251)), const<i32>(188), array_decay<ptr<i8>, length=Some(28)>(%252));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32>(%19)))), read<i32, volatile>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%253)), const<i32>(189), array_decay<ptr<i8>, length=Some(28)>(%254));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%3)))), read<i32, volatile>(%3)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%255)), const<i32>(190), array_decay<ptr<i8>, length=Some(28)>(%256));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32>(%19)))), read<i32, volatile>(%4)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%257)), const<i32>(192), array_decay<ptr<i8>, length=Some(28)>(%258));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32>(%19)))), read<i32, volatile>(%4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%259)), const<i32>(193), array_decay<ptr<i8>, length=Some(28)>(%260));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%19))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%261)), const<i32>(194), array_decay<ptr<i8>, length=Some(28)>(%262));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%20))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%263)), const<i32>(195), array_decay<ptr<i8>, length=Some(28)>(%264));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%0, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%2), read<i32, volatile>(%4)))), read<i32, volatile>(%4)))), read<i32>(%21))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%265)), const<i32>(196), array_decay<ptr<i8>, length=Some(28)>(%266));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
