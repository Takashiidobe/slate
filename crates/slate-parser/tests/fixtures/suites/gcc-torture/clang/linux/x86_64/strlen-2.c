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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = code_units<array<i8, 3>>([49, 0, 0]), index1 = code_units<array<i8, 3>>([49, 50, 0])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<array<array<i8, 5>, 2>, 2> [storage=static] [const] [align=16] = aggregate<array<array<array<i8, 5>, 2>, 2>, zero_fill=false>(index0 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 0, 0, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 0, 0, 0])), index1 = aggregate<array<array<i8, 5>, 2>, zero_fill=false>(index0 = code_units<array<i8, 5>>([49, 50, 51, 0, 0]), index1 = code_units<array<i8, 5>>([49, 50, 51, 52, 0]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v0:[0-9]+]] v0: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 105, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 38, 97, 91, 118, 49, 93, 91, 118, 50, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_67:[0-9]+]] .str[[VALUE_str_67]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_68:[0-9]+]] .str[[VALUE_str_68]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_69:[0-9]+]] .str[[VALUE_str_69]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_70:[0-9]+]] .str[[VALUE_str_70]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_71:[0-9]+]] .str[[VALUE_str_71]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_72:[0-9]+]] .str[[VALUE_str_72]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_73:[0-9]+]] .str[[VALUE_str_73]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_74:[0-9]+]] .str[[VALUE_str_74]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_75:[0-9]+]] .str[[VALUE_str_75]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_76:[0-9]+]] .str[[VALUE_str_76]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_77:[0-9]+]] .str[[VALUE_str_77]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_78:[0-9]+]] .str[[VALUE_str_78]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_79:[0-9]+]] .str[[VALUE_str_79]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_80:[0-9]+]] .str[[VALUE_str_80]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_81:[0-9]+]] .str[[VALUE_str_81]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_82:[0-9]+]] .str[[VALUE_str_82]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_83:[0-9]+]] .str[[VALUE_str_83]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_84:[0-9]+]] .str[[VALUE_str_84]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 105, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_85:[0-9]+]] .str[[VALUE_str_85]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_86:[0-9]+]] .str[[VALUE_str_86]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_87:[0-9]+]] .str[[VALUE_str_87]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_88:[0-9]+]] .str[[VALUE_str_88]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_89:[0-9]+]] .str[[VALUE_str_89]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_90:[0-9]+]] .str[[VALUE_str_90]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_91:[0-9]+]] .str[[VALUE_str_91]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_92:[0-9]+]] .str[[VALUE_str_92]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_93:[0-9]+]] .str[[VALUE_str_93]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_94:[0-9]+]] .str[[VALUE_str_94]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_95:[0-9]+]] .str[[VALUE_str_95]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_96:[0-9]+]] .str[[VALUE_str_96]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 116, 114, 108, 101, 110, 40, 97, 91, 118, 49, 93, 32, 43, 32, 118, 50, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_97:[0-9]+]] .str[[VALUE_str_97]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_98:[0-9]+]] .str[[VALUE_str_98]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_99:[0-9]+]] .str[[VALUE_str_99]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_100:[0-9]+]] .str[[VALUE_str_100]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_101:[0-9]+]] .str[[VALUE_str_101]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_102:[0-9]+]] .str[[VALUE_str_102]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_103:[0-9]+]] .str[[VALUE_str_103]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_104:[0-9]+]] .str[[VALUE_str_104]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_105:[0-9]+]] .str[[VALUE_str_105]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_106:[0-9]+]] .str[[VALUE_str_106]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_107:[0-9]+]] .str[[VALUE_str_107]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_108:[0-9]+]] .str[[VALUE_str_108]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_109:[0-9]+]] .str[[VALUE_str_109]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_110:[0-9]+]] .str[[VALUE_str_110]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_111:[0-9]+]] .str[[VALUE_str_111]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_112:[0-9]+]] .str[[VALUE_str_112]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_113:[0-9]+]] .str[[VALUE_str_113]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_114:[0-9]+]] .str[[VALUE_str_114]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_115:[0-9]+]] .str[[VALUE_str_115]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_116:[0-9]+]] .str[[VALUE_str_116]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_117:[0-9]+]] .str[[VALUE_str_117]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_118:[0-9]+]] .str[[VALUE_str_118]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_119:[0-9]+]] .str[[VALUE_str_119]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_120:[0-9]+]] .str[[VALUE_str_120]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_121:[0-9]+]] .str[[VALUE_str_121]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_122:[0-9]+]] .str[[VALUE_str_122]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_123:[0-9]+]] .str[[VALUE_str_123]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_124:[0-9]+]] .str[[VALUE_str_124]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_125:[0-9]+]] .str[[VALUE_str_125]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_126:[0-9]+]] .str[[VALUE_str_126]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_127:[0-9]+]] .str[[VALUE_str_127]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_128:[0-9]+]] .str[[VALUE_str_128]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_129:[0-9]+]] .str[[VALUE_str_129]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_130:[0-9]+]] .str[[VALUE_str_130]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_131:[0-9]+]] .str[[VALUE_str_131]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_132:[0-9]+]] .str[[VALUE_str_132]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_133:[0-9]+]] .str[[VALUE_str_133]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_134:[0-9]+]] .str[[VALUE_str_134]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_135:[0-9]+]] .str[[VALUE_str_135]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_136:[0-9]+]] .str[[VALUE_str_136]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_137:[0-9]+]] .str[[VALUE_str_137]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_138:[0-9]+]] .str[[VALUE_str_138]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_139:[0-9]+]] .str[[VALUE_str_139]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_140:[0-9]+]] .str[[VALUE_str_140]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_141:[0-9]+]] .str[[VALUE_str_141]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_142:[0-9]+]] .str[[VALUE_str_142]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_143:[0-9]+]] .str[[VALUE_str_143]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_144:[0-9]+]] .str[[VALUE_str_144]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_145:[0-9]+]] .str[[VALUE_str_145]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_146:[0-9]+]] .str[[VALUE_str_146]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_147:[0-9]+]] .str[[VALUE_str_147]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_148:[0-9]+]] .str[[VALUE_str_148]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_149:[0-9]+]] .str[[VALUE_str_149]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_150:[0-9]+]] .str[[VALUE_str_150]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_151:[0-9]+]] .str[[VALUE_str_151]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_152:[0-9]+]] .str[[VALUE_str_152]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_153:[0-9]+]] .str[[VALUE_str_153]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_154:[0-9]+]] .str[[VALUE_str_154]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_155:[0-9]+]] .str[[VALUE_str_155]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_156:[0-9]+]] .str[[VALUE_str_156]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_157:[0-9]+]] .str[[VALUE_str_157]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_158:[0-9]+]] .str[[VALUE_str_158]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_159:[0-9]+]] .str[[VALUE_str_159]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_160:[0-9]+]] .str[[VALUE_str_160]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_161:[0-9]+]] .str[[VALUE_str_161]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_162:[0-9]+]] .str[[VALUE_str_162]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_163:[0-9]+]] .str[[VALUE_str_163]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_164:[0-9]+]] .str[[VALUE_str_164]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_165:[0-9]+]] .str[[VALUE_str_165]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_166:[0-9]+]] .str[[VALUE_str_166]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_167:[0-9]+]] .str[[VALUE_str_167]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_168:[0-9]+]] .str[[VALUE_str_168]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 105, 48, 93, 91, 118, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_169:[0-9]+]] .str[[VALUE_str_169]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_170:[0-9]+]] .str[[VALUE_str_170]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 48, 93, 91, 118, 48, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_171:[0-9]+]] .str[[VALUE_str_171]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_172:[0-9]+]] .str[[VALUE_str_172]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 105, 48, 93, 91, 118, 49, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_173:[0-9]+]] .str[[VALUE_str_173]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_174:[0-9]+]] .str[[VALUE_str_174]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 105, 48, 93, 91, 118, 49, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_175:[0-9]+]] .str[[VALUE_str_175]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_176:[0-9]+]] .str[[VALUE_str_176]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 48, 93, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_177:[0-9]+]] .str[[VALUE_str_177]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_178:[0-9]+]] .str[[VALUE_str_178]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 49, 93, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_179:[0-9]+]] .str[[VALUE_str_179]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_180:[0-9]+]] .str[[VALUE_str_180]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 38, 98, 91, 118, 49, 93, 91, 118, 49, 93, 91, 105, 50, 93, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_181:[0-9]+]] .str[[VALUE_str_181]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_182:[0-9]+]] .str[[VALUE_str_182]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_183:[0-9]+]] .str[[VALUE_str_183]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_184:[0-9]+]] .str[[VALUE_str_184]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_185:[0-9]+]] .str[[VALUE_str_185]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_186:[0-9]+]] .str[[VALUE_str_186]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_187:[0-9]+]] .str[[VALUE_str_187]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_188:[0-9]+]] .str[[VALUE_str_188]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_189:[0-9]+]] .str[[VALUE_str_189]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_190:[0-9]+]] .str[[VALUE_str_190]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_191:[0-9]+]] .str[[VALUE_str_191]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_192:[0-9]+]] .str[[VALUE_str_192]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_193:[0-9]+]] .str[[VALUE_str_193]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_194:[0-9]+]] .str[[VALUE_str_194]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_195:[0-9]+]] .str[[VALUE_str_195]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_196:[0-9]+]] .str[[VALUE_str_196]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_197:[0-9]+]] .str[[VALUE_str_197]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_198:[0-9]+]] .str[[VALUE_str_198]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_199:[0-9]+]] .str[[VALUE_str_199]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_200:[0-9]+]] .str[[VALUE_str_200]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_201:[0-9]+]] .str[[VALUE_str_201]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_202:[0-9]+]] .str[[VALUE_str_202]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_203:[0-9]+]] .str[[VALUE_str_203]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_204:[0-9]+]] .str[[VALUE_str_204]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_205:[0-9]+]] .str[[VALUE_str_205]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_206:[0-9]+]] .str[[VALUE_str_206]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_207:[0-9]+]] .str[[VALUE_str_207]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_208:[0-9]+]] .str[[VALUE_str_208]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_209:[0-9]+]] .str[[VALUE_str_209]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_210:[0-9]+]] .str[[VALUE_str_210]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_211:[0-9]+]] .str[[VALUE_str_211]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_212:[0-9]+]] .str[[VALUE_str_212]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_213:[0-9]+]] .str[[VALUE_str_213]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_214:[0-9]+]] .str[[VALUE_str_214]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_215:[0-9]+]] .str[[VALUE_str_215]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_216:[0-9]+]] .str[[VALUE_str_216]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_217:[0-9]+]] .str[[VALUE_str_217]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_218:[0-9]+]] .str[[VALUE_str_218]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_219:[0-9]+]] .str[[VALUE_str_219]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_220:[0-9]+]] .str[[VALUE_str_220]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_221:[0-9]+]] .str[[VALUE_str_221]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_222:[0-9]+]] .str[[VALUE_str_222]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_223:[0-9]+]] .str[[VALUE_str_223]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_224:[0-9]+]] .str[[VALUE_str_224]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_225:[0-9]+]] .str[[VALUE_str_225]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_226:[0-9]+]] .str[[VALUE_str_226]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_227:[0-9]+]] .str[[VALUE_str_227]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_228:[0-9]+]] .str[[VALUE_str_228]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 105, 48, 93, 32, 43, 32, 118, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_229:[0-9]+]] .str[[VALUE_str_229]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_230:[0-9]+]] .str[[VALUE_str_230]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 48, 93, 91, 118, 48, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_231:[0-9]+]] .str[[VALUE_str_231]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_232:[0-9]+]] .str[[VALUE_str_232]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 105, 48, 93, 91, 118, 49, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_233:[0-9]+]] .str[[VALUE_str_233]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_234:[0-9]+]] .str[[VALUE_str_234]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 105, 48, 93, 32, 43, 32, 118, 49, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_235:[0-9]+]] .str[[VALUE_str_235]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_236:[0-9]+]] .str[[VALUE_str_236]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 48, 41, 32, 61, 61, 32, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_237:[0-9]+]] .str[[VALUE_str_237]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_238:[0-9]+]] .str[[VALUE_str_238]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 49, 41, 32, 61, 61, 32, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_239:[0-9]+]] .str[[VALUE_str_239]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_240:[0-9]+]] .str[[VALUE_str_240]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([115, 116, 114, 108, 101, 110, 40, 98, 91, 118, 49, 93, 91, 118, 49, 93, 32, 43, 32, 105, 50, 41, 32, 61, 61, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_array_ref_2_3:[0-9]+]] @test_array_ref_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str]])), const<i32>(18), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_3]])), const<i32>(19), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_5]])), const<i32>(20), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_6]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_7]])), const<i32>(21), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_8]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_9]])), const<i32>(23), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_11]])), const<i32>(24), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_13]])), const<i32>(25), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_15]])), const<i32>(26), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_17]])), const<i32>(28), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_19]])), const<i32>(29), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_20]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_21]])), const<i32>(31), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v2]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_23]])), const<i32>(32), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i0:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_25]])), const<i32>(38), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_27]])), const<i32>(39), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_29]])), const<i32>(40), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_30]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_31]])), const<i32>(41), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_33]])), const<i32>(43), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_35]])), const<i32>(44), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_36]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_37]])), const<i32>(45), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_38]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_39]])), const<i32>(46), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_40]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_41]])), const<i32>(48), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_42]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_43]])), const<i32>(49), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_44]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i2]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_45]])), const<i32>(51), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_46]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v2]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_47]])), const<i32>(52), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_48]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_array_off_2_3:[0-9]+]] @test_array_off_2_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_49]])), const<i32>(56), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_50]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_51]])), const<i32>(57), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_52]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_53]])), const<i32>(58), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_54]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_55]])), const<i32>(59), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_56]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_57]])), const<i32>(61), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_58]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_59]])), const<i32>(62), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_60]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_61]])), const<i32>(63), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_62]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_63]])), const<i32>(64), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_64]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_65]])), const<i32>(66), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_66]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_67]])), const<i32>(67), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_68]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_69]])), const<i32>(69), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_70]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_71]])), const<i32>(70), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_72]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i0_2:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1_2:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0_2]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2_2:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1_2]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0_2]])))), read<i32>(%[[VALUE_i0_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_73]])), const<i32>(76), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_74]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i0_2]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_75]])), const<i32>(77), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_76]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_77]])), const<i32>(78), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_78]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_79]])), const<i32>(79), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_80]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_81]])), const<i32>(81), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_82]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i1_2]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_83]])), const<i32>(82), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_84]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_85]])), const<i32>(83), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_86]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_87]])), const<i32>(84), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_88]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i1_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_89]])), const<i32>(86), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_90]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_91]])), const<i32>(87), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_92]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i2_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_93]])), const<i32>(89), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_94]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(ptr_offset<ptr<const array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_95]])), const<i32>(90), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_96]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_array_ref_2_2_5:[0-9]+]] @test_array_ref_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_97]])), const<i32>(94), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_98]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_99]])), const<i32>(95), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_100]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_101]])), const<i32>(97), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_102]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_103]])), const<i32>(98), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_104]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_105]])), const<i32>(99), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_106]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_107]])), const<i32>(101), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_108]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_109]])), const<i32>(102), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_110]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_111]])), const<i32>(103), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_112]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_113]])), const<i32>(105), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_114]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_115]])), const<i32>(106), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_116]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_117]])), const<i32>(108), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_118]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_119]])), const<i32>(109), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_120]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_121]])), const<i32>(110), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_122]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_123]])), const<i32>(112), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_124]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_125]])), const<i32>(113), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_126]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_127]])), const<i32>(114), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_128]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_129]])), const<i32>(116), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_130]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_131]])), const<i32>(117), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_132]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_133]])), const<i32>(118), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_134]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_135]])), const<i32>(119), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_136]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_137]])), const<i32>(120), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_138]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i0_3:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1_3:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0_3]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2_3:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1_3]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_139]])), const<i32>(126), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_140]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_141]])), const<i32>(127), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_142]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_143]])), const<i32>(129), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_144]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_145]])), const<i32>(130), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_146]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_147]])), const<i32>(131), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_148]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_149]])), const<i32>(133), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_150]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_151]])), const<i32>(134), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_152]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_153]])), const<i32>(135), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_154]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_155]])), const<i32>(137), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_156]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_157]])), const<i32>(138), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_158]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_159]])), const<i32>(140), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_160]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_161]])), const<i32>(141), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_162]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_163]])), const<i32>(142), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_164]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_165]])), const<i32>(144), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_166]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v0]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_167]])), const<i32>(145), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_168]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_169]])), const<i32>(146), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_170]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_171]])), const<i32>(148), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_172]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_3]])))), read<i32, volatile>(%[[VALUE_v1]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_173]])), const<i32>(149), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_174]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_175]])), const<i32>(150), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_176]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i1_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_177]])), const<i32>(151), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_178]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i2_3]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_179]])), const<i32>(152), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_180]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_array_off_2_2_5:[0-9]+]] @test_array_off_2_2_5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_181]])), const<i32>(156), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_182]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_183]])), const<i32>(157), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_184]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_185]])), const<i32>(158), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_186]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_187]])), const<i32>(159), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_188]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_189]])), const<i32>(161), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_190]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_191]])), const<i32>(162), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_192]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_193]])), const<i32>(163), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_194]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_195]])), const<i32>(165), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_196]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_197]])), const<i32>(166), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_198]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_199]])), const<i32>(167), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_200]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_201]])), const<i32>(169), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_202]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0)))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_203]])), const<i32>(170), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_204]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_205]])), const<i32>(171), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_206]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_207]])), const<i32>(172), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_208]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), const<i32>(2))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_209]])), const<i32>(173), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_210]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i0_4:[0-9]+]] i0: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i1_4:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i0_4]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i2_4:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i1_4]]), const<i32>(1));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_211]])), const<i32>(179), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_212]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_213]])), const<i32>(180), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_214]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_215]])), const<i32>(181), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_216]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_217]])), const<i32>(182), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_218]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_219]])), const<i32>(184), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_220]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_221]])), const<i32>(185), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_222]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32>(%[[VALUE_i0_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_223]])), const<i32>(186), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_224]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_225]])), const<i32>(188), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_226]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v0]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_227]])), const<i32>(189), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_228]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v0]])))), read<i32, volatile>(%[[VALUE_v0]])))), read<i32>(%[[VALUE_i0_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_229]])), const<i32>(190), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_230]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_231]])), const<i32>(192), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_232]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_4]])))), read<i32, volatile>(%[[VALUE_v1]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_233]])), const<i32>(193), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_234]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i0_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_235]])), const<i32>(194), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_236]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i1_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_237]])), const<i32>(195), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_238]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(5)>(deref(ptr_offset<ptr<const array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<const array<i8, 5>>, length=Some(2)>(deref(ptr_offset<ptr<const array<array<i8, 5>, 2>>, subtract=false, element=array<array<i8, 5>, 2>, overflow=ub>(array_decay<ptr<const array<array<i8, 5>, 2>>, length=Some(2)>(%[[VALUE_b]]), read<i32, volatile>(%[[VALUE_v1]])))), read<i32, volatile>(%[[VALUE_v1]])))), read<i32>(%[[VALUE_i2_4]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_239]])), const<i32>(196), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_240]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_array_ref_2_3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_array_off_2_3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_array_ref_2_2_5]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_array_off_2_2_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
